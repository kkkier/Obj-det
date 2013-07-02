/*********************************************************************/
/*                                                                   */
/* FILE         grayimage.cc                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Several conversion functions between GrayImage and   */
/*              RGBImage.                                            */
/*                                                                   */
/* BEGIN        Thu Aug 30 2001                                      */
/* LAST CHANGE  Thu Aug 30 2001                                      */
/*                                                                   */
/*********************************************************************/

#ifdef _USE_PERSONAL_NAMESPACES
namespace Leibe {
#endif

/****************/
/*   Includes   */
/****************/
#include <math.h>
#include <stdio.h>
#include <cassert>

#include <iostream>
#include <fstream>
#include <string>

#include <image.h>
#include <rgbimage.h>

#include "grayimage.hh"


/*===================================================================*/
/*                         Class GrayImage                           */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

GrayImage& GrayImage::operator=( GrayImage other )
  /* assignment operator */
{
  Image<GrayPixel>::operator=( other );
  return *this;
}


GrayImage& GrayImage::operator=( RGBImage other )
  /* assignment operator */
{
  *this = getIntensityImage( other );
  return *this;
}


std::vector<float> GrayImage::getData()
  /* return the raw image data */
{
  std::vector<float> raw;

  for( int y=0; y<height(); y++ )
    for( int x=0; x<width(); x++ )
      raw.push_back( (*this)(x,y).value() );
  return raw;
}


void GrayImage::loadFromData( int w, int h, float data[] )
{
	GrayImage img( w, h );

  for (int y=0; y < h; y++)
    for (int x=0; x < w; x++) {
			int idx = y*w+x;

			img(x,y) = data[idx];
		}
	*this = img;
}


void GrayImage::loadFromData( int w, int h, std::vector<float> data )
{
	GrayImage img( w, h );

  for (int y=0; y < h; y++)
    for (int x=0; x < w; x++) {
			int idx = y*w+x;

			img(x,y) = data[idx];
		}
	*this = img;
}


/***********************************************************/
/*                  File I/O Functions                     */
/***********************************************************/

bool GrayImage::load( std::string filename )
{
  std::cout << "  OpGrayImage::load() called." << std::endl;

  /* try to open the file */
  std::cout << "    Trying to open the file..." << std::endl;
  std::ifstream ifile;
  ifile.open( filename.c_str() );
  if ( !ifile ) {
    std::cerr << "Error in OpGrayImage::load(): Couldn't open file '" << filename 
	 << "'!" << std::endl;
    return false;
  }

  /* read the first 2 characters => must be "P9" */
  std::cout << "    Reading the first 2 characters to determine the file type..." 
	<< std::endl;
  char c1, c2;
  ifile >> c1 >> c2;
  if (c1 != 'P') {
    std::cerr << "Error in OpGrayImage::load(): " 
	 << "File '" << filename << "' is not a pnm image (read '" << c1 << c2 
	 << "')!" << std::endl;
    std::cerr << "Error in OpGrayImage::load(): " 
	 << "File '" << filename << "' has unknown format ('" << c1 << c2 
	 << "')!" << std::endl;
    std::cerr << "(Only float image ('P9') implemented yet...)" << std::endl;
    return false;
  }

  if( ifile.eof() ) {
    std::cerr << "Error in OpGrayImage::load(): " 
	 << "Premature end of file after magic number!" << std::endl;
    return false;
  }

  /* skip the comment, but read the ROI definition, if present */
  std::cout << "    Skipping over the comments..." << std::endl;
  char line [256];
  int w, h;
  int x1=0, y1=0, x2=0, y2=0;
  /* read the comment lines */
  ifile.getline( line, 256, '\n');
  while ((strlen(line)==0) || (line[0] == '#'))
    {
      /* look for a ROI definition in the comments */
      if ((line[0]== '#' && line[1] == '$') ||
	  (line[0]== '#' && line[1] == ' ' && line[2] == '$'))
	if (line[2]=='R'&&line[3]=='O'&&line[4]=='I')
	  sscanf(line+6*sizeof(char), "%d%d%d%d", &x1, &y1, &x2, &y2);

      if ( ifile.eof() ) {
	std::cerr << "Error in OpGrayImage::load(): " 
	     << "Premature end of file (expected image size)!" << std::endl;
	return false;
      }
      
      ifile.getline(line, 256, '\n');
    }

  /* read the image size */
  std::cout << "    Reading the image size..." << std::endl;
  sscanf (line, "%d %d\n", &w, &h);
  if ( (w == 0) && (h == 0) ) {
    std::cerr << "Error in OpGrayImage::load(): Image is of size 0!" << std::endl;
    return false;
  }

  if ( ifile.eof() ) {
    std::cerr << "Error in OpGrayImage::load(): " 
	 << "Premature end of file (expected number of colors)!" << std::endl;
    return false;
  }

  /* read the number of colors */
  std::cout << "    Reading the number of colors..." << std::endl;
  int bytes;
  ifile >> bytes;
  if (bytes != 255) {
    std::cerr << "Warning in OpGrayImage::load(): Image does not have 255 levels ("
	 << bytes << ")!" << std::endl;
    return false;
  }
  
  if ( ifile.eof() ) {
    std::cerr << "Error in OpGrayImage::load(): " 
	 << "Premature end of file (expected image data)!" << std::endl;
    return false;
  }
  ifile.getline(line, 256, '\n'); // read the rest of the line
    
  /* prepare the image */
  std::cout << "    Preparing the image..." << std::endl;
  GrayImage img( w, h );
  *this = img;
  
  /* read the image data */
  std::cout << "    Reading the image data..." << std::endl;
  switch (c2) {
    case '2':
    case '3':
    case '6':
    case '4':
    case '7':
    case '8':
    case 'A':
      std::cerr << "Error in OpGrayImage::load(): " 
	   << "Sorry, file format '" << c1 << c2 << "' not implemented yet!" 
	   << std::endl;
      return false;
      break;

  case '5':
    if ( !readPGM( ifile ) )
      return false;
    break;

  case '9':
    if ( !readPCFPM( ifile ) )
      return false;
    break;

  default:
    std::cerr << "Error in OpGrayImage::load(): " 
	 << "Unknown file format 'P" << c2 << "'!" << std::endl;
    return false;
  }
  
  /* close the input file */
  ifile.close();

  std::cout << "  done." << std::endl;
  return true;
}


bool GrayImage::readPGM( std::ifstream &ifile )
{
  for (int y=0; y<height(); y++)
    for (int x=0; x<width(); x++) {
      // BinBytePCRead(ifile, (long*)i.GetLinePtr(h), 1*width());
      unsigned char buf;
      ifile.read( (char*) &buf, 1 );
      (*this)(x,y) = (float) buf;
    }

  return true;
}


bool GrayImage::readPCFPM( std::ifstream &ifile )
{
  for (int y=0; y<height(); y++)
    for (int x=0; x<width(); x++) {
      // BinBytePCRead(ifile, (long*)i.GetLinePtr(h), 1*width());
      float buf;
      ifile.read((char*) &buf, 4 );
      (*this)(x,y) = buf;
    }

  return true;
}


/*---------------------------------------------------------*/
/*                Basic Image Manipulation                 */
/*---------------------------------------------------------*/

GrayImage GrayImage::operator+( const GrayImage &other ) const
	/* add another image to this image */
{
	return add( other );
}


GrayImage GrayImage::operator-( const GrayImage &other ) const
	/* subtract another image from this image */
{
	return sub( other );
}


GrayImage GrayImage::operator*( const GrayImage &other ) const
	/* multiplicate this image with another image */
{
	return mul( other );
}


GrayImage GrayImage::operator/( const GrayImage &other ) const
	/* divide this image by another image */
{
	return div( other );
}


GrayImage GrayImage::operator+( float factor ) const
	/* add a constant factor to this image */
{
	return add( factor );
}


GrayImage GrayImage::operator-( float factor ) const
	/* subtract a constant factor from this image */
{
	return sub( factor );
}


GrayImage GrayImage::operator*( float factor ) const
	/* multiplicate this image with a constant factor */
{
	return mul( factor );
}


GrayImage GrayImage::operator/( float factor ) const
	/* divide this image by a constant factor */
{
	return div( factor );
}


GrayImage GrayImage::add( const GrayImage &other, int pos_x, int pos_y ) const
	/* add another image to this image starting at (pos_x,pos_y) */
{
	GrayImage res( *this );
	for( int y=0; y<other.height(); y++ )
		for( int x=0; x<other.width(); x++ ) {
			int xx = pos_x + x; 
			int yy = pos_y + y;
      if( (xx>=0) && (xx<width()) && (yy>=0) && (yy<height()) )
        res(xx,yy) = res(xx,yy).value() + other(x,y).value(); 
		}
	
	return res;
}


GrayImage GrayImage::sub( const GrayImage &other, int pos_x, int pos_y ) const
	/* subtract another image from this image starting at (pos_x,pos_y)*/
{
	GrayImage res( *this );
	for( int y=0; y<other.height(); y++ )
		for( int x=0; x<other.width(); x++ ) {
			int xx = pos_x + x; 
			int yy = pos_y + y;
      if( (xx>=0) && (xx<width()) && (yy>=0) && (yy<height()) )
        res(xx,yy) = res(xx,yy).value() - other(x,y).value(); 
		}
	
	return res;
}


GrayImage GrayImage::mul( const GrayImage &other, int pos_x, int pos_y ) const
	/* multiply this image with another image starting at (pos_x,pos_y)*/
{
	GrayImage res( *this );
	for( int y=0; y<other.height(); y++ )
		for( int x=0; x<other.width(); x++ ) {
			int xx = pos_x + x; 
			int yy = pos_y + y;
      if( (xx>=0) && (xx<width()) && (yy>=0) && (yy<height()) )
        res(xx,yy) = res(xx,yy).value() * other(x,y).value(); 
		}

	return res;
}


GrayImage GrayImage::div( const GrayImage &other, int pos_x, int pos_y ) const
	/* divide this image by another image starting at (pos_x,pos_y).   */
{
	GrayImage res( *this );
	for( int y=0; y<other.height(); y++ )
		for( int x=0; x<other.width(); x++ ) {
			int xx = pos_x + x; 
			int yy = pos_y + y;
      if( (xx>=0) && (xx<width()) && (yy>=0) && (yy<height()) )
        res(xx,yy) = res(xx,yy).value() / other(x,y).value(); 
		}

	return res;
}


GrayImage GrayImage::add( float factor ) const
	/* add a constant factor to this image */
{
	GrayImage res( width(), height() );
	for( int y=0; y<this->height(); y++ )
		for( int x=0; x<this->width(); x++ )
			res(x,y) = (*this)(x,y).value() + factor;
	return res;
}


GrayImage GrayImage::sub( float factor ) const
	/* subtract a constant factor from this image */
{
	GrayImage res( width(), height() );
	for( int y=0; y<this->height(); y++ )
		for( int x=0; x<this->width(); x++ )
			res(x,y) = (*this)(x,y).value() - factor;
	return res;
}


GrayImage GrayImage::mul( float factor ) const
	/* multiply this image with a constant factor */
{
	GrayImage res( width(), height() );
	for( int y=0; y<this->height(); y++ )
		for( int x=0; x<this->width(); x++ )
			res(x,y) = (*this)(x,y).value() * factor;
	return res;
}


GrayImage GrayImage::div( float factor ) const
	/* divide this image by a constant factor */
{
	GrayImage res( width(), height() );
	for( int y=0; y<this->height(); y++ )
		for( int x=0; x<this->width(); x++ )
			res(x,y) = (*this)(x,y).value() / factor;
	return res;
}


GrayImage GrayImage::copy( const GrayImage &other, int pos_x, int pos_y ) const
  /* Copy the other image into this image starting from the */
  /* specified position.                                    */
{
	GrayImage res( *this );
	for( int y=0; y<other.height(); y++ )
		for( int x=0; x<other.width(); x++ ) {
			int xx = pos_x + x; 
			int yy = pos_y + y;
      if( (xx>=0) && (xx<width()) && (yy>=0) && (yy<height()) )
        res(xx,yy) = other(x,y).value(); 
		}

	return res;
}


GrayImage GrayImage::extractRegion( int x1, int y1, int x2, int y2) const
{
  //assert( (x1>=0) && (y1>=0) && (x2<width()) && (y2<height()) );
  assert( (x1 <= x2) && (y1 <= y2) );

  GrayImage result(x2-x1+1,y2-y1+1);
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


inline float GrayImage::rotatePixel( int x, int y, int ox, int oy, 
                                     const GrayImage& sourceImg,
                                     float dAngle, float dBackColor ) const
{
  float fCos = cos( dAngle );
  float fSin = sin( dAngle );

  float fa = (x-ox)*fCos - (y-oy)*fSin + ox;
  float fb = (x-ox)*fSin + (y-oy)*fCos + oy;
  int na = (int)fa;
  int nb = (int)fb;
  float a = fa - na;
  float b = fb - nb;

  // destination pixel value
  float res = dBackColor;
  if ( na >= 0 && na < (sourceImg.width()-1) &&
       nb >= 0 && nb < (sourceImg.height()-1) )
  {
    res = (1.0f-b) * (1.0f-a) * sourceImg( na, nb ).value() +
      (1.0f-b) * a * sourceImg( na+1, nb ).value() +
      b * (1.0f-a) * sourceImg( na, nb+1 ).value() +
      b * a * sourceImg( na+1, nb+1 ).value();
  }

  return res;
}


GrayImage GrayImage::rotate( float dAngle, float dBackColor ) const
{
  int w = this->width();
  int h = this->height();
  int w2 = (int) floor(1.5*w + 0.5);
  int h2 = (int) floor(1.5*h + 0.5);
  int offsetx = (w2-w)/2;
  int offsety = (h2-h)/2;
  GrayImage result( w, h );
  GrayImage bigimg( w2, h2 );

  // draw into a bigger image, for rotation to come out nicely
  int ox = w/2;
  int oy = h/2;
  for ( int y=0; y<h2; y++ )
    for ( int x=0; x<w2; x++ )
      bigimg(x,y) = rotatePixel( x-offsetx, y-offsety, ox, oy, 
                                 (*this), dAngle, dBackColor );

  // extract the center region
  result = bigimg.extractRegion( offsetx, offsety, offsetx+w-1, offsety+h-1 );

  return result;
}


GrayImage GrayImage::rotateLeft90() const
{
  GrayImage result( height(), width() );
  int x, y;
  
  for( y=0; y<height(); y++ )
    for( x=0; x<width(); x++ )
      result(y,width()-x-1) = (*this)(x,y);

  return result;
}


GrayImage GrayImage::rotateRight90() const
{
  GrayImage result( height(), width() );
  int x, y;
  
  for( y=0; y<height(); y++ )
    for( x=0; x<width(); x++ )
      result(height()-y-1,x) = (*this)(x,y);

  return result;
}


GrayImage GrayImage::flipHorizontal() const
{
  int w = width();
  int h = height();
  GrayImage result( w, h );

  for( int y=0; y<h; y++ )
    for( int x=0; x<w; x++ )
      result(x,y) = (*this)(w-x-1,y);

  return result;
}


GrayImage GrayImage::flipVertical() const
{
  int w = width();
  int h = height();
  GrayImage result( w, h );

  for( int y=0; y<h; y++ )
    for( int x=0; x<w; x++ )
      result(x,y) = (*this)(x,h-y-1);

  return result;
}


/***********************************************************/
/*                  Conversion Functions                   */
/***********************************************************/

GrayImage getIntensityImage( RGBImage &rgbImg )
  /*******************************************************************/
  /* Get an intensity image out of an RGB image.                     */
  /*******************************************************************/
{
  GrayImage grayImg( rgbImg.width(), rgbImg.height() );

  for ( int y=0; y<rgbImg.height(); y++ )
    for ( int x=0; x<rgbImg.width(); x++ )
      grayImg(x,y) = rgbImg(x,y);

  return grayImg;
}

GrayImage extractImageBand( int num, RGBImage &rgbImg )
  /*******************************************************************/
  /* Extract a color band (r, g, or b) out of an RGB image and store */
  /* it as a grayvalue image. The parameter num determines the band. */
  /* Only the values 0 (=red), 1 (=green), or 2 (=blue) are allowed. */
  /*******************************************************************/
{
  GrayImage grayImg( rgbImg.width(), rgbImg.height() );

  switch( num ) {
  case 0:   // red
    for ( int y=0; y<rgbImg.height(); y++ )
      for ( int x=0; x<rgbImg.width(); x++ )
	grayImg(x,y) = ((float)rgbImg(x,y).red());
    break;

  case 1:   // green
    for ( int y=0; y<rgbImg.height(); y++ )
      for ( int x=0; x<rgbImg.width(); x++ )
	grayImg(x,y) = ((float)rgbImg(x,y).green());
    break;

  case 2:   // blue
    for ( int y=0; y<rgbImg.height(); y++ )
      for ( int x=0; x<rgbImg.width(); x++ )
	grayImg(x,y) = ((float)rgbImg(x,y).blue());
    break;

  default:
    std::cerr << "Error in getImageBand(): Parameter 'num' must be between 0 and 2!"
	 << std::endl;
  }

  return grayImg;
}


GrayImage extractImageBand( int num, HSIImage &hsiImg )
  /*******************************************************************/
  /* Extract a color band (r, g, or b) out of an HSI image and store */
  /* it as a grayvalue image. The parameter num determines the band. */
  /* Only the values 0 (=hue), 1 (=saturation), or 2 (=intensity)    */
  /* allowed.                                                        */
  /*******************************************************************/
{
  GrayImage grayImg( hsiImg.width(), hsiImg.height() );

  switch( num ) {
  case 0:   // hue
    for ( int y=0; y<hsiImg.height(); y++ )
      for ( int x=0; x<hsiImg.width(); x++ )
				grayImg(x,y) = ((float)hsiImg(x,y).hue());
    break;
		
  case 1:   // saturation
    for ( int y=0; y<hsiImg.height(); y++ )
      for ( int x=0; x<hsiImg.width(); x++ )
				grayImg(x,y) = ((float)hsiImg(x,y).saturation());
    break;
		
  case 2:   // intensity
    for ( int y=0; y<hsiImg.height(); y++ )
      for ( int x=0; x<hsiImg.width(); x++ )
				grayImg(x,y) = ((float)hsiImg(x,y).intensity());
    break;
		
  default:
    std::cerr << "Error in getImageBand(): Parameter 'num' must be between 0 and 2!"
				 << std::endl;
  }

  return grayImg;
}


std::vector<GrayImage> extractImageBands( RGBImage &rgbImg )
  /*******************************************************************/
  /* Extract all three image bands from an RGB image and store them  */
  /* in a vector of grayvalue images for separate processing.        */
  /*******************************************************************/
{
  GrayImage r = extractImageBand( 0, rgbImg );
  GrayImage g = extractImageBand( 1, rgbImg );
  GrayImage b = extractImageBand( 2, rgbImg );

  std::vector<GrayImage> vImgBands(3);
  vImgBands[0] = r;
  vImgBands[1] = g;
  vImgBands[2] = b;

  return vImgBands;
}


std::vector<GrayImage> extractImageBands( HSIImage &hsiImg )
  /*******************************************************************/
  /* Extract all three image bands from an HSI image and store them  */
  /* in a vector of grayvalue images for separate processing.        */
  /*******************************************************************/
{
  GrayImage h = extractImageBand( 0, hsiImg );
  GrayImage s = extractImageBand( 1, hsiImg );
  GrayImage i = extractImageBand( 2, hsiImg );

  std::vector<GrayImage> vImgBands(3);
  vImgBands[0] = h;
  vImgBands[1] = s;
  vImgBands[2] = i;

  return vImgBands;
}


RGBImage  combineImageBands( GrayImage &r, GrayImage &g, GrayImage &b )
  /*******************************************************************/
  /* Combine three image bands (stored as grayvalue images) into an  */
  /* RGB image. All input images must have the same dimensions. The  */
  /* pixel values are rounded.                                       */
  /*******************************************************************/
{
  assert( (r.width() == g.width()) && (g.width() == b.width()) );
  assert( (r.height() == g.height()) && (g.height() == b.height()) );

  RGBImage rgbImg( r.width(), r.height() );

  for ( int y=0; y<rgbImg.height(); y++ )
    for ( int x=0; x<rgbImg.width(); x++ )
      rgbImg(x,y) = RGBPixel( (uchar)floor(r(x,y).value()+0.5),
			      (uchar)floor(g(x,y).value()+0.5),
			      (uchar)floor(b(x,y).value()+0.5) );

  return rgbImg;
}


HSIImage  combineImageBandsHSI( GrayImage &h, GrayImage &s, GrayImage &i )
  /*******************************************************************/
  /* Combine three image bands (stored as grayvalue images) into an  */
  /* HSI image. All input images must have the same dimensions. The  */
  /* pixel values are rounded.                                       */
  /*******************************************************************/
{
  assert( (h.width() == s.width()) && (s.width() == i.width()) );
  assert( (h.height() == s.height()) && (s.height() == i.height()) );

  HSIImage hsiImg( h.width(), h.height() );

  for ( int y=0; y<hsiImg.height(); y++ )
    for ( int x=0; x<hsiImg.width(); x++ )
      hsiImg(x,y) = HSIPixel( h(x,y).value(),
															s(x,y).value(),
															i(x,y).value() );

  return hsiImg;
}


#ifdef _USE_PERSONAL_NAMESPACES
}
#endif
