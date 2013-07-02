/******************************************************************************
 * $Id: rgbwidget.h,v 1.1.1.1 2004/09/08 13:50:20 mfritz Exp $
 * $Author: mfritz $
 * $Date: 2004/09/08 13:50:20 $
 *------------------------------ Description ----------------------------------
 * Implementation of a QWidget for displaying images of type Image<RGBPixel>.
 * RGBImageWidget provides two ways for displaying an RGBImage 
 * (Image<RGBPixel>):
 * 1. Constructing an instance of RGBImageWidget requires a reference to an
 *    RGBImage object. If the image object is not valid, an image of size
 *    'default_width' x 'default_height' (see below) is created which shows
 *    a red cross in order to indicate missing image data. If the image data 
 *    is valid, a corresponding widget is created which resizes itself to the
 *    size of its image.
 * 2. Calling method 'display' of an existing RGBImageWidget. If the image data
 *    is valid, the widget displays the new data and resizes itself to the
 *    new image's dimensions. In case of invalid image data, a red cross of
 *    size 'default_width' x 'default_height' is created and displayed.
 *------------------------------ Requirements ---------------------------------
 * libqt.so.2.2.0 - Qt library, version 2.2.0
 *------------------------------- CVS logs ------------------------------------
 * $Log: rgbwidget.h,v $
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.2  2004/04/26 15:16:33  leibe
 *
 * reinstated the original version of the library 'pccvImage'.
 *
 * Revision 1.2  2002/04/02 14:26:03  spengler
 * Handler mechanism added: Installation of a  RGBImageWidget::Handler
 * instance will enable probing of displayed RGBImage.
 *
 * Revision 1.1  2001/08/20 15:44:07  spengler
 * Version 1 of image class and related utility classes and libraries
 * finished. Ready for distribution
 *
 * Revision 1.1  2001/08/17 17:01:36  spengler
 * Running code before weekend (08/17/2001)
 *
 *****************************************************************************/

#ifndef _SPENGLER_RGB_WIDGET_H
#define _SPENGLER_RGB_WIDGET_H

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Spengler {
#endif

#include <qwidget.h>
#include <qpixmap.h>
#include <qframe.h>
#include <qlabel.h>

#include <rgbimage.h>

const int default_width = 100;
const int default_height = 100;

/******************************************************************************
 *                           class RGBImageWidget                             *
 *****************************************************************************/
class RGBImageWidget : public QWidget
{
 public:
  class Handler;
  
  RGBImageWidget(const RGBImage& image,QWidget *parent=0, const char *name=0,
		 WFlags f=0);
  
  void display(const RGBImage&);
  Handler* setHandler(Handler *);

 protected:
  virtual void paintEvent(QPaintEvent *);
  virtual void mouseReleaseEvent(QMouseEvent *);

  void copyImage(const RGBImage&);

 private:
  QPixmap _image;
  Handler *_handler;
};

/******************************************************************************
 *                         class RGBImageWidget::Handler                      *
 *****************************************************************************/
class RGBImageWidget::Handler
{
 public:
  virtual ~Handler(){};

  virtual void operator()(const int x,const int y) = 0;
};

#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif

#endif // _SPENGLER_RGB_WIDGET_H
