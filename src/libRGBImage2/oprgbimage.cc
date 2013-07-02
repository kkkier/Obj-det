/*********************************************************************/
/*                                                                   */
/* FILE         oprgbimage.cc                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Define an image class that combines the rgb          */
/*              Image class with Qt functionality for displaying,    */
/*              and that contains many useful image processing ope-  */
/*              rators.                                              */
/*                                                                   */
/* BEGIN        Fri Jun 21 2002                                      */
/* LAST CHANGE  Fri Jun 21 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif
  
/****************/
/*   Includes   */
/****************/
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <math.h>

#include <qimage.h>
#include <qcolor.h>

#include "oprgbimage.hh"

/*===================================================================*/
/*                         Class OpRGBImage                         */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

OpRGBImage::OpRGBImage( const QImage& src )
  : RGBImage( src.width(), src.height() )
  /* copy from a QImage structure */
{
  copyFrom( src );
}


OpRGBImage& OpRGBImage::operator=( OpRGBImage other )
  /* assignment operator */
{
  RGBImage::operator=( other );
  return *this;
}


void OpRGBImage::copyFrom(const QImage& src)
  /* copy operator */
{
  QImage img(src);
  //cout << "    OpRGBImage::copyFrom()" << endl;

  /* if the image has not enough bitplanes, convert it to 32 bpp */
  if ( img.depth() != 32 ) 
    img = src.convertDepth( 32 );

  /* convert the QImage structure to a new image */
  for (int y=0; y < height(); y++)
    for (int x=0; x < width(); x++) {
      QRgb *p = (QRgb *)img.scanLine(y) + x;
      
      // convert rgb to grayvalue
      (*this)(x,y) = RGBPixel((uchar)qRed(*p), 
															(uchar)qGreen(*p), 
															(uchar)qBlue(*p));
    }
  //cout << "    done." << endl;
}


/***********************************************************/
/*                   Access Functions                      */
/***********************************************************/

QImage OpRGBImage::getQtImage( ) const
{
  QImage qtImg( width(), height(), 32 ); 

  /* copy the image data */
  for (int y=0; y < this->height(); y++)
    for (int x=0; x < this->width(); x++) {
      uint *p = (uint *) qtImg.scanLine(y) + x;
      *p  = qRgb( (int) (*this)(x,y).red(), 
									(int) (*this)(x,y).green(), 
									(int) (*this)(x,y).blue()   );
    }

  return qtImg;
}


void OpRGBImage::getQtImage( QImage &qtImg ) const
{
  qtImg.create( width(), height(), 32 ); 

  /* copy the image data */
  for (int y=0; y < this->height(); y++)
    for (int x=0; x < this->width(); x++) {
      uint *p = (uint *) qtImg.scanLine(y) + x;
      *p  = qRgb( (int) (*this)(x,y).red(), 
									(int) (*this)(x,y).green(), 
									(int) (*this)(x,y).blue()   );
		}

}

/***********************************************************/
/*                 Basic Image Manipulation                */
/***********************************************************/

OpRGBImage OpRGBImage::extractRegion( int x1, int y1, int x2, int y2)
{
  //assert( (x1>=0) && (y1>=0) && (x2<width()) && (y2<height()) );
  assert( (x1 <= x2) && (y1 <= y2) );

  OpRGBImage result(x2-x1+1,y2-y1+1);
  int x, y;
  int src_x, src_y;

  for( y=y1; y<=y2; y++ )
    for( x=x1; x<=x2; x++ ) {
      src_x = (x<0)? 0 : x;
      src_y = (y<0)? 0 : y;
      src_x = (src_x>=width())? width()-1 : src_x;
      src_y = (src_y>=height())? height()-1 : src_y;

      result(x-x1,y-y1) = (*this)(src_x,src_y);
    }

  return result;
}


OpRGBImage OpRGBImage::rotateLeft90()
{
  OpRGBImage result( height(), width() );
  int x, y;
  
  for( y=0; y<height(); y++ )
    for( x=0; x<width(); x++ )
      result(y,width()-x-1) = (*this)(x,y);

  return result;
}


OpRGBImage OpRGBImage::rotateRight90()
{
  OpRGBImage result( height(), width() );
  int x, y;
  
  for( y=0; y<height(); y++ )
    for( x=0; x<width(); x++ )
      result(height()-y-1,x) = (*this)(x,y);

  return result;
}




