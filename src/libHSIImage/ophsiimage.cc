/*********************************************************************/
/*                                                                   */
/* FILE         ophsiimage.cc                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Define an image class that combines the HSI          */
/*              Image class with Qt functionality for displaying,    */
/*              and that contains many useful image processing ope-  */
/*              rators.                                              */
/*                                                                   */
/* BEGIN        FRI Jun 14 2002                                      */
/* LAST CHANGE  FRI Jun 14 2002                                      */
/*                                                                   */
/*********************************************************************/
  

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <math.h>
#include <algorithm>
#include <cassert>

#include <qimage.h>
#include <qcolor.h>

#include "ophsiimage.hh"

/*******************/
/*   Definitions   */
/*******************/
#define MIN_SATURATION 0.01       // minimal saturation for image subtraction

/*===================================================================*/
/*                         Class OpHSIImage                          */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

OpHSIImage::OpHSIImage( const QImage& src )
  : HSIImage( src.width(), src.height() )
  /* copy from a QImage structure */
{
	cout << "  OpHSIImage::OpHSIImage( QImage ) called." << endl;
  copyFrom( src );
	cout << "  done." << endl;
}


OpHSIImage& OpHSIImage::operator=( OpHSIImage other )
  /* assignment operator */
{
  HSIImage::operator=( other );
  return *this;
}


void OpHSIImage::loadFromData( int w, int h, 
															 float data_h[], float data_s[], float data_i[] )
	/* copy the data from a 3-band float array */
{
	OpHSIImage img( width(), height() );

  for (int y=0; y < height(); y++)
    for (int x=0; x < width(); x++) {
			int idx = y*width()+x;

			img(x,y) = HSIPixel( data_h[idx], data_s[idx], data_i[idx] );
		}
	*this = img;
}


void OpHSIImage::loadFromData( int w, int h, 
															 float data_hsi[] )
	/* copy the data from a mixed-band float array */
{
	OpHSIImage img( width(), height() );

  for (int y=0; y < height(); y++)
    for (int x=0; x < width(); x++) {
			int idx = (y*width()+x) * 3;

			img(x,y) = HSIPixel( data_hsi[idx+0], data_hsi[idx+1], data_hsi[idx+2] );
		}
	*this = img;
}


void OpHSIImage::copyFrom(const QImage& src)
  /* copy operator */
{
  QImage img(src);
  cout << "    OpHSIImage::copyFrom() called." << endl;

	cout << "    Converting image to 32 bpp..." << endl;
  /* if the image has not enough bitplanes, convert it to 32 bpp */
  if ( img.depth() != 32 ) 
    img = src.convertDepth( 32 );

	cout << "    Converting QImage structure to new image..." << endl;
	OpHSIImage tmp( src.width(), src.height() );
  /* convert the QImage structure to a new image */
  for (int y=0; y < height(); y++)
    for (int x=0; x < width(); x++) {
      uint *p = (uint *)img.scanLine(y) + x;
      
      // convert rgb to grayvalue
      //(*this)(x,y) = RGBPixel( (uchar) qRed(*p), 
      tmp(x,y) = RGBPixel( (uchar) qRed(*p), 
													 (uchar) qGreen(*p), 
													 (uchar) qBlue(*p) );
    }
	*this = tmp;

  cout << "    done." << endl;
}


/***********************************************************/
/*                   Access Functions                      */
/***********************************************************/

QImage OpHSIImage::getQtImage( )
{
  QImage qtImg( width(), height(), 32 ); 

  /* copy the image data */
  for (int y=0; y < this->height(); y++)
    for (int x=0; x < this->width(); x++) {
      uint *p = (uint *) qtImg.scanLine(y) + x;
      *p  = qRgb( (int) floor((*this)(x,y).hue() + 0.5), 
									(int) floor((*this)(x,y).saturation() + 0.5), 
									(int) floor((*this)(x,y).intensity() + 0.5) );
    }

  return qtImg;
}


void OpHSIImage::getQtImage( QImage &qtImg )
{
  qtImg.create( width(), height(), 32 ); 

  /* copy the image data */
  for (int y=0; y < this->height(); y++)
    for (int x=0; x < this->width(); x++) {
      uint *p = (uint *) qtImg.scanLine(y) + x;
      *p  = qRgb( (int) floor((*this)(x,y).hue() + 0.5), 
									(int) floor((*this)(x,y).saturation() + 0.5), 
									(int) floor((*this)(x,y).intensity() + 0.5) );
		}

}


/***********************************************************/
/*            Basic Image Manipulation Functions           */
/***********************************************************/

OpHSIImage OpHSIImage::operator+( const HSIImage &other )
	/* add another image to this image */
{
	return add( other );
}


OpHSIImage OpHSIImage::operator-( const HSIImage &other )
	/* subtract another image from this image */
{
	return sub( other );
}


OpHSIImage OpHSIImage::operator*( const HSIImage &other )
	/* multiplicate this image with another image */
{
	return mul( other );
}


OpHSIImage OpHSIImage::operator/( const HSIImage &other )
	/* divide this image by another image */
{
	return div( other );
}


OpHSIImage OpHSIImage::operator+( float factor )
	/* add a constant factor to this image */
{
	return add( factor, factor, factor );
}


OpHSIImage OpHSIImage::operator-( float factor )
	/* subtract a constant factor from this image */
{
	return sub( factor, factor, factor );
}


OpHSIImage OpHSIImage::operator*( float factor )
	/* multiplicate this image with a constant factor */
{
	return mul( factor, factor, factor );
}


OpHSIImage OpHSIImage::operator/( float factor )
	/* divide this image by a constant factor */
{
	return div( factor, factor, factor );
}


OpHSIImage OpHSIImage::add( const HSIImage &other, int pos_x, int pos_y )
	/* add another image to this image starting at (pos_x,pos_y) */
{
	OpHSIImage res( width(), height() );
	int minx = std::min( 0, pos_x );
	int miny = std::min( 0, pos_y );
	int maxx = std::min( width() , pos_x + other.width() );
	int maxy = std::min( height(), pos_y + other.height() );
	for( int y=miny; y<maxy; y++ )
		for( int x=minx; x<maxx; x++ ) {
			int xx = pos_x + x; 
			int yy = pos_y + y;
			res(xx,yy) = boundHSIRange( (*this)(xx,yy).hue() 
																	+ other(x,y).hue(), 
																	(*this)(xx,yy).saturation() 
																	+ other(x,y).saturation(),
																	(*this)(xx,yy).intensity() 
																	+ other(x,y).intensity());
		}
	
	return res;
}


OpHSIImage OpHSIImage::sub( const HSIImage &other, int pos_x, int pos_y )
	/* subtract another image from this image starting at (pos_x,pos_y)*/
{
	OpHSIImage res( width(), height() );
	int minx = std::min( 0, pos_x );
	int miny = std::min( 0, pos_y );
	int maxx = std::min( width() , pos_x + other.width() );
	int maxy = std::min( height(), pos_y + other.height() );
	for( int y=miny; y<maxy; y++ )
		for( int x=minx; x<maxx; x++ ) {
			int xx = pos_x + x; 
			int yy = pos_y + y;
			res(xx,yy)=boundHSIRange( (*this)(xx,yy).hue()       
																- other(x,y).hue(), 
																(*this)(xx,yy).saturation()
																- other(x,y).saturation(),
																(*this)(xx,yy).intensity() 
																- other(x,y).intensity());
		}
	
	return res;
}


OpHSIImage OpHSIImage::mul( const HSIImage &other, int pos_x, int pos_y )
	/* multiply this image with another image starting at (pos_x,pos_y)*/
{
	OpHSIImage res( other.width(), other.height() );
	int minx = std::min( 0, pos_x );
	int miny = std::min( 0, pos_y );
	int maxx = std::min( width() , pos_x + other.width() );
	int maxy = std::min( height(), pos_y + other.height() );
	for( int y=miny; y<maxy; y++ )
		for( int x=minx; x<maxx; x++ ) {
			int xx = pos_x + x; 
			int yy = pos_y + y;
			res(xx,yy)=boundHSIRange( (*this)(xx,yy).hue() 
																* other(x,y).hue(), 
																(*this)(xx,yy).saturation()
																* other(x,y).saturation(),
																(*this)(xx,yy).intensity() 
																* other(x,y).intensity());
		}

	return res;
}


OpHSIImage OpHSIImage::div( const HSIImage &other, int pos_x, int pos_y )
	/* divide this image by another image starting at (pos_x,pos_y).   */
{
	OpHSIImage res( width(),height() );
	int minx = std::min( 0, pos_x );
	int miny = std::min( 0, pos_y );
	int maxx = std::min( width() , pos_x + other.width() );
	int maxy = std::min( height(), pos_y + other.height() );
	for( int y=miny; y<maxy; y++ )
		for( int x=minx; x<maxx; x++ ) {
			int xx = pos_x + x; 
			int yy = pos_y + y;
			res(xx,yy)=boundHSIRange( (*this)(xx,yy).hue()       
																/ other(x,y).hue(), 
																(*this)(xx,yy).saturation()
																/ other(x,y).saturation(),
																(*this)(xx,yy).intensity() 
																/ other(x,y).intensity() );
		}

	return res;
}


OpHSIImage OpHSIImage::add( float fact_h, float fact_s, float fact_i )
	/* add a constant factor to this image */
{
	OpHSIImage res( width(), height() );
	for( int y=0; y<this->height(); y++ )
		for( int x=0; x<this->width(); x++ )
			res(x,y) = boundHSIRange( (*this)(x,y).hue()        + fact_h, 
																(*this)(x,y).saturation() + fact_s,
																(*this)(x,y).intensity()  + fact_i );
	return res;
}


OpHSIImage OpHSIImage::sub( float fact_h, float fact_s, float fact_i )
	/* subtract a constant factor from this image */
{
	OpHSIImage res( width(), height() );
	for( int y=0; y<this->height(); y++ )
		for( int x=0; x<this->width(); x++ )
			res(x,y) = boundHSIRange( (*this)(x,y).hue()        - fact_h, 
																(*this)(x,y).saturation() - fact_s,
																(*this)(x,y).intensity()  - fact_i );
	return res;
}


OpHSIImage OpHSIImage::mul( float fact_h, float fact_s, float fact_i )
	/* multiply this image with a constant factor */
{
	OpHSIImage res( width(), height() );
	for( int y=0; y<this->height(); y++ )
		for( int x=0; x<this->width(); x++ )
			res(x,y) = boundHSIRange( (*this)(x,y).hue()        * fact_h, 
																(*this)(x,y).saturation() * fact_s,
																(*this)(x,y).intensity()  * fact_i );
	return res;
}


OpHSIImage OpHSIImage::div( float fact_h, float fact_s, float fact_i )
	/* divide this image by a constant factor */
{
	OpHSIImage res( width(), height() );
	for( int y=0; y<this->height(); y++ )
		for( int x=0; x<this->width(); x++ )
			res(x,y) = boundHSIRange( (*this)(x,y).hue()        / fact_h, 
																(*this)(x,y).saturation() / fact_s,
																(*this)(x,y).intensity()  / fact_i );
	return res;
}


OpHSIImage OpHSIImage::subabs( const HSIImage &other, 
                               int pos_x, int pos_y )
	/* subtract another image from this image starting at (pos_x,pos_y)*/
	/* and retain the absolute values for saturation and intensity.    */
{
	OpHSIImage res( width(), height() );
	int minx = std::min( 0, pos_x );
	int miny = std::min( 0, pos_y );
	int maxx = std::min( width() , pos_x + other.width() );
	int maxy = std::min( height(), pos_y + other.height() );
	for( int y=miny; y<maxy; y++ )
		for( int x=minx; x<maxx; x++ ) {
			int xx = pos_x + x; 
			int yy = pos_y + y;
			double dh = fabs( (*this)(xx,yy).hue() - other(x,y).hue() );
			if( dh > 180.0 )
				dh = 360.0 - dh;
			res(xx,yy)=boundHSIRange( dh, 
																fabs( (*this)(xx,yy).saturation()
																			- other(x,y).saturation() ),
																fabs( (*this)(xx,yy).intensity() 
																			- other(x,y).intensity()) );
		}
	
	return res;
}


OpHSIImage OpHSIImage::extractRegion( int x1, int y1, int x2, int y2)
{
  //assert( (x1>=0) && (y1>=0) && (x2<width()) && (y2<height()) );
  assert( (x1 <= x2) && (y1 <= y2) );

  OpHSIImage result(x2-x1+1,y2-y1+1);
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


OpHSIImage OpHSIImage::rotateLeft90()
{
  OpHSIImage result( height(), width() );
  int x, y;
  
  for( y=0; y<height(); y++ )
    for( x=0; x<width(); x++ )
      result(y,width()-x-1) = (*this)(x,y);

  return result;
}


OpHSIImage OpHSIImage::rotateRight90()
{
  OpHSIImage result( height(), width() );
  int x, y;
  
  for( y=0; y<height(); y++ )
    for( x=0; x<width(); x++ )
      result(height()-y-1,x) = (*this)(x,y);

  return result;
}


/***********************************************************/
/*                Image Processing Functions               */
/***********************************************************/

//----------------------------------------------------------------------------
// Related Functions
//----------------------------------------------------------------------------

HSIPixel boundHSIRange( double h, double s, double i )
	/* Bound the passed values to the valid HSI range.                 */
{
	double _h, _s, _i;

	if( h==HSIPixel::UNDEFINED )
		_h = h;
	else {
		while( h<0.0 )
			h += 360.0;
		while( h>360.0 )
			h -= 360.0;
		_h = h;
	}

	if( s<MIN_SATURATION )
		_s = MIN_SATURATION;
	else if( s>1.0 )
		_s = 1.0;
	else
		_s = s;
	if( _s==0.0 )
		_h = HSIPixel::UNDEFINED;

	if( i<0.0 )
		_i = 0.0;
	else if( i>1.0 )
		_i = 1.0;
	else
		_i = i;

	return HSIPixel( _h, _s, _i );
}


//----------------------------------------------------------------------------
// Conversion Functions
//----------------------------------------------------------------------------

HSIImage getHSIImage( RGBImage &rgbImg )
  /*******************************************************************/
  /* Get an HSI image out of an RGB image.                           */
  /*******************************************************************/
{
  HSIImage hsiImg( rgbImg.width(), rgbImg.height() );

  for ( int y=0; y<rgbImg.height(); y++ )
    for ( int x=0; x<rgbImg.width(); x++ )
      hsiImg(x,y) = rgbImg(x,y);

  return hsiImg;
}


RGBImage getRGBImage( HSIImage &hsiImg )
  /*******************************************************************/
  /* Get an RGB image out of an HSI image.                           */
  /*******************************************************************/
{
  RGBImage rgbImg( hsiImg.width(), hsiImg.height() );

  for ( int y=0; y<hsiImg.height(); y++ )
    for ( int x=0; x<hsiImg.width(); x++ )
      rgbImg(x,y) = hsiImg(x,y);

  return rgbImg;
}


