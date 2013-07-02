/******************************************************************************
 * $Id: rgbwidget.cc,v 1.1.1.1 2004/09/08 13:50:20 mfritz Exp $
 * $Author: mfritz $
 * $Date: 2004/09/08 13:50:20 $
 *------------------------------ Description ----------------------------------
 * Implementation of a QWidget for displaying images of type Image<RGBPixel>.
 * See rgbwidget.h for details.
 *------------------------------- CVS logs ------------------------------------
 * $Log: rgbwidget.cc,v $
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.2  2004/04/26 15:16:33  leibe
 *
 * reinstated the original version of the library 'pccvImage'.
 *
 * Revision 1.3  2002/04/02 14:26:03  spengler
 * Handler mechanism added: Installation of a  RGBImageWidget::Handler
 * instance will enable probing of displayed RGBImage.
 *
 * Revision 1.2  2001/08/22 14:55:35  spengler
 * Error in handling empty images eliminated.
 *
 * Revision 1.1  2001/08/20 15:44:07  spengler
 * Version 1 of image class and related utility classes and libraries
 * finished. Ready for distribution
 *
 * Revision 1.1  2001/08/17 17:01:36  spengler
 * Running code before weekend (08/17/2001)
 *
 *****************************************************************************/

#ifdef _USE_PERSONAL_NAMESPACES
namespace Spengler {
#endif

#include "rgbwidget.h"

#include <qlayout.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qpen.h>

/******************************************************************************
 *                             class RGBImageWidget                           *
 *****************************************************************************/

//============================== Implementation ===============================

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
RGBImageWidget::RGBImageWidget(const RGBImage& image,QWidget *parent,
			       const char *name, WFlags f)
  : QWidget(parent,name,f)
{
  // do not erase widget background when paint events occur
  setBackgroundMode(QWidget::NoBackground);

  // create pixmap which contains the image data
  copyImage(image);

  // sets widget's geometry
  setFixedSize(_image.width(),_image.height());

  // reset handler
  _handler = 0;
}
  
//-----------------------------------------------------------------------------
// Display: Copies the given image to the internal buffer _image, adjusts the
// display buffer and repaints the widget.
//-----------------------------------------------------------------------------
void RGBImageWidget::display(const RGBImage& image){
  copyImage(image);
  setFixedSize(image.width(),image.height());
  repaint();
}

//-----------------------------------------------------------------------------
// setHandler: Sets a new (external) handler for events signaling a click
// with the left mouse button.
//-----------------------------------------------------------------------------
RGBImageWidget::Handler *
RGBImageWidget::setHandler(RGBImageWidget::Handler *handler)
{
  Handler *result = _handler;
  _handler = handler;
  return result;
}

//-----------------------------------------------------------------------------
// paintEvent: copies the prepared pixmap _buffer to the display.
//-----------------------------------------------------------------------------
void RGBImageWidget::paintEvent(QPaintEvent* event){
  bitBlt(this,0,0,&_image);
}

//-----------------------------------------------------------------------------
// mouseReleaseEvent: if the left mouse button is released, the handler is 
// called if it was set before.
//-----------------------------------------------------------------------------
void RGBImageWidget::mouseReleaseEvent(QMouseEvent *e){
  if(e->button() == Qt::LeftButton  && _handler != 0)
    (*_handler)(e->x(),e->y());
}

//-----------------------------------------------------------------------------
// copyImage: copies image to internal buffer
//-----------------------------------------------------------------------------
void RGBImageWidget::copyImage(const RGBImage& image){
  if(image.width()*image.height() > 0){
    _image.resize(image.width(),image.height());
    
    QPainter p(&_image);
    for(int y=0;y<image.height();y++)
      for(int x=0;x<image.width();x++){
	p.setPen(QColor(image(x,y).red(),
			image(x,y).green(),
			image(x,y).blue()));
	p.drawPoint(x,y);
      }
  }
  else{
    _image.resize(default_width,default_height);
    _image.fill();

    QPainter p(&_image);
    p.setPen(QPen(QColor(255,0,0),5));
    p.drawLine(0,0,_image.width(),_image.height());
    p.drawLine(0,_image.height(),_image.width(),0);
  }
}

#ifdef _USE_PERSONAL_NAMESPACES
}
#endif

