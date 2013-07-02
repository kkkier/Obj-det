/*********************************************************************/
/*                                                                   */
/* FILE         opgrayimg.cc                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Define an image class that combines the pccv         */
/*              Image class with Qt functionality for displaying,    */
/*              and that contains many useful operators.             */
/*                                                                   */
/* BEGIN        FRI Aug 17 2001                                      */
/* LAST CHANGE  FRI Jun 07 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif
  
/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <cassert>
#include <algorithm>

#include <qimage.h>
#include <qcolor.h>

#include <gaussderiv.hh>
#include <canny.hh>
#include <morphology.hh>

#include "opgrayimage.hh"

/*===================================================================*/
/*                         Class OpGrayImage                         */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

OpGrayImage::OpGrayImage( const QImage& src )
  : GrayImage( src.width(), src.height() )
  /* copy from a QImage structure */
{
  copyFrom( src );
}


OpGrayImage& OpGrayImage::operator=( OpGrayImage other )
  /* assignment operator */
{
  GrayImage::operator=( other );
  return *this;
}


void OpGrayImage::copyFrom(const QImage& src)
  /* copy operator */
{
  QImage img(src);
  //cout << "    OpGrayImage::copyFrom()" << endl;

  /* if the image has not enough bitplanes, convert it to 32 bpp */
  if ( img.depth() != 32 ) 
    img = src.convertDepth( 32 );

  /* convert the QImage structure to a new image */
  for (int y=0; y < height(); y++)
    for (int x=0; x < width(); x++) {
      uint *p = (uint *)img.scanLine(y) + x;
      
      // convert rgb to grayvalue
      (*this)(x,y) = calcIntensity(*p);
    }
  //cout << "    done." << endl;
}


/***********************************************************/
/*                   Access Functions                      */
/***********************************************************/

QImage OpGrayImage::getQtImage( bool normalize ) const
{
  int   col;

  OpGrayImage grayImg( *this );
  QImage qtImg( width(), height(), 32, 256 ); 
  //-- set number of colors again --//  
  qtImg.setNumColors(256);

  if (normalize)
    // normalize the color range to [0,255]
    grayImg.opNormalizeRange( 0.0, 255.0 );
  else 
    // cut off at 0 and 255
  grayImg.opClipOutside( 0.0, 255.0 );

  /* set the colors to graylevels */
  for (int i=0; i < 255; i++)
    qtImg.setColor(i, qRgb(i,i,i) );

  /* copy the image data */
  for (int y=0; y < grayImg.height(); y++)
    for (int x=0; x < grayImg.width(); x++) {
      uint *p = (uint *) qtImg.scanLine(y) + x;
      col = qRound( grayImg(x,y).value() ); 
      *p  = qRgb(col, col, col);
    }

  return qtImg;
}


void OpGrayImage::getQtImage( QImage &qtImg, bool normalize )
{
  int   col;

  OpGrayImage grayImg( *this );
  qtImg.create( width(), height(), 32, 256 ); 

  if (normalize)
    // normalize the color range to [0,255]
    grayImg.opNormalizeRange( 0.0, 255.0 );
  else 
    // cut off at 0 and 255
  grayImg.opClipOutside( 0.0, 255.0 );

  /* copy the image data */
  for (int y=0; y < grayImg.height(); y++)
    for (int x=0; x < grayImg.width(); x++) {
      uint *p = (uint *) qtImg.scanLine(y) + x;
      col = qRound( grayImg(x,y).value() ); 
      *p  = qRgb(col, col, col);
    }

  /* set the colors to graylevels */
  for (int i=0; i < 255; i++)
    qtImg.setColor(i, qRgb(i,i,i) );
}


float OpGrayImage::getSum()
  /* return the sum over all values in the image */
{
  float sum = 0.0;
  for (int y=0; y < height(); y++)
    for (int x=0; x < width(); x++) 
      sum += (*this)(x,y).value();

  return sum;
}


void OpGrayImage::getMinMax( float &min, float &max )
  /* return the minimum and maximum value in the image */
{
  /* calculate the minimum and maximum gray value */
  min = 9999999999.0;
  max =-9999999999.0;
  for (int y=0; y < height(); y++)
    for (int x=0; x < width(); x++) {
      if ( (*this)(x,y).value() < min )
	min = (*this)(x,y).value();
      if ( (*this)(x,y).value() > max )
	max = (*this)(x,y).value();
    }
}


/***********************************************************/
/*                Image Processing Functions               */
/***********************************************************/

/*---------------------------------------------------------*/
/*                   Threshold operators                   */
/*---------------------------------------------------------*/
void OpGrayImage::opThresholdAbove ( float t1, 
				     float valtrue, float valfalse )
  /* threshold above:                                                */
  /*   set all values >= t1 to 'valtrue', all others to 'valfalse'   */
{
  for (int y=0; y < height(); y++)
    for ( int x=0; x < width(); x++)
      if ( (*this)(x,y).value() >= t1 )
	(*this)(x,y) = valtrue;
      else
	(*this)(x,y) = valfalse;
}

void OpGrayImage::opThresholdBelow ( float t1, 
				     float valtrue, float valfalse )
  /* threshold below:                                                */
  /*   set all values < t1 to 'valtrue', all others to 'valfalse'    */
{
  for (int y=0; y < height(); y++)
    for ( int x=0; x < width(); x++)
      if ( (*this)(x,y).value() < t1 )
	(*this)(x,y) = valtrue;
      else
	(*this)(x,y) = valfalse;
}

void OpGrayImage::opThresholdInside( float t1, float t2, 
				     float valtrue, float valfalse )
  /* threshold inside:                                               */
  /*   set all values in [t1,t2] to 'valtrue', all others to 'valfalse'*/
{
  for (int y=0; y < height(); y++)
    for ( int x=0; x < width(); x++)
      if ( ((*this)(x,y).value() >= t1) && ((*this)(x,y).value() <= t2) )
	(*this)(x,y) = valtrue;
      else
	(*this)(x,y) = valfalse;
}

void OpGrayImage::opThresholdOutside( float t1, float t2, 
				      float valtrue, float valfalse )
  /* threshold outside:                                              */
  /*   set all values in )t1,t2( to 'valtrue', all others to 'valfalse'*/
{
  for (int y=0; y < height(); y++)
    for ( int x=0; x < width(); x++)
      if ( ((*this)(x,y).value() < t1) || ((*this)(x,y).value() > t2) )
	(*this)(x,y) = valtrue;
      else
	(*this)(x,y) = valfalse;
}


/*---------------------------------------------------------*/
/*                      Clip operators                     */
/*---------------------------------------------------------*/
void OpGrayImage::opClipAbove ( float t1 )
  /* clip above:                                                     */
  /*   set all values > t1 to t1                                     */
{
  for (int y=0; y < height(); y++)
    for ( int x=0; x < width(); x++)
      if ( (*this)(x,y).value() > t1 )
	(*this)(x,y) = t1;
}

void OpGrayImage::opClipBelow ( float t1 )
  /* clip below:                                                     */
  /*   set all values < t1 to t1                                     */
{
  for (int y=0; y < height(); y++)
    for ( int x=0; x < width(); x++)
      if ( (*this)(x,y).value() < t1 )
	(*this)(x,y) = t1;
}

void OpGrayImage::opClipInside( float t1, float t2, float clipvalue )
  /* clip inside:                                                    */
  /*   set all values in [t1,t2] to 'clipvalue'                      */
{
  for (int y=0; y < height(); y++)
    for ( int x=0; x < width(); x++)
      if ( ((*this)(x,y).value() >= t1) && ((*this)(x,y).value() <= t2) )
	(*this)(x,y) = clipvalue;
}

void OpGrayImage::opClipOutside( float t1, float t2 )
  /* clip outside:                                                   */
  /*   set all values < t1 to t1, and all values > t2 to t2          */
{
  for (int y=0; y < height(); y++)
    for ( int x=0; x < width(); x++)
      if ( (*this)(x,y).value() < t1 ) 
	(*this)(x,y) = t1;
      else if ( (*this)(x,y).value() > t2 )
	(*this)(x,y) = t2;
}


/*---------------------------------------------------------*/
/*                 Color Range Operators                   */
/*---------------------------------------------------------*/
void   OpGrayImage::opNormalizeRange( float rangeMin, 
                                      float rangeMax )
  /* normalize the color range to the given range */
{
  float min, max;
  float scale, offset;

  /* calculate the minimum and maximum gray value */
  getMinMax( min, max );
  //   min = 9999999999.0;
  //   max =-9999999999.0;
  //   for (int y=0; y < height(); y++)
  //     for (int x=0; x < width(); x++) {
  //       if ( (*this)(x,y).value() < min )
  // 	min = (*this)(x,y).value();
  //       if ( (*this)(x,y).value() > max )
  // 	max = (*this)(x,y).value();
  //     }
  scale  = (rangeMax - rangeMin) / (max - min);
  offset = -min;

  /* transform the image data */
  for (int y=0; y < height(); y++)
    for (int x=0; x < width(); x++) {
      // normalize the color range to [rangeMin,rangeMax]
      (*this)(x,y) = ((*this)(x,y).value()+offset)*scale + rangeMin; 
    }
}


OpGrayImage OpGrayImage::opHistEq()
  /* Perform histogram equalization on the image. */
{
  int NUM_HISTCELLS = 256;

  OpGrayImage imgResult( width(), height() );

  /* Normalize the range to 0..NUM_HISTCELLS */
  OpGrayImage imgGray( *this );
  imgGray.opNormalizeRange(0.0, (float) NUM_HISTCELLS-1.0 );

  /* compute a grayvalue histogram */
  std::vector<float> histGray( NUM_HISTCELLS );
  for( int y=0; y<height(); y++ )
    for( int x=0; x<width(); x++ ) {
      int binidx = (int) floor( imgGray(x,y).value() + 0.5 );
      if( binidx < 0 )
        binidx = 0;
      if( binidx > NUM_HISTCELLS-1 )
        binidx = NUM_HISTCELLS-1;

      histGray[binidx] += 1.0;
    }

  /* normalize the histogram */
  float dFactor = ((float)NUM_HISTCELLS-1.0)/(float)(width()*height());
  for( int i=0; i<NUM_HISTCELLS-1; i++ )
    histGray[i] *= dFactor;

  /* get the cumulative histogram */
  std::vector<float> histCumul( histGray );
  for( int i=1; i<NUM_HISTCELLS-1; i++ )
    histCumul[i] = histCumul[i-1] + histGray[i];
  
  /* assign the new color to all pixels */
  //cout << "  Assigning the new pixel colors..." << endl;
  for( int y=0; y<height(); y++ )
    for( int x=0; x<width(); x++ ) {
      int binidx = (int) floor( imgGray(x,y).value() + 0.5 );
      if( binidx < 0 )
        binidx = 0;
      if( binidx > NUM_HISTCELLS-1 )
        binidx = NUM_HISTCELLS-1;

      imgResult(x,y) = std::max( 0.0, floor(histCumul[binidx]+0.5)-1.0 );
    }

  return imgResult;
}


/*---------------------------------------------------------*/
/*                  Derivative Operators                   */
/*---------------------------------------------------------*/
OpGrayImage OpGrayImage::opGauss        ( double sigma )
  /* filter the image with a Gaussian. */
{
  //cout << "  OpGrayImage::opGauss() called." << endl;

  //cout << "    creating a local result image..." << endl;
  OpGrayImage result(*this);
  
  //cout << "    applying the filter..." << endl;
  ApplyGauss( *this, sigma, result );

  //cout << "  done." << endl;
  return result;
}


OpGrayImage OpGrayImage::opGaussDerivGx ( double sigma )
  /* calculate the Gaussian derivative in x direction. */
{
  OpGrayImage result(*this);
  ApplyGaussGx( *this, sigma, result );
  return result;
}


OpGrayImage OpGrayImage::opGaussDerivGy ( double sigma )
  /* calculate the Gaussian derivative in y direction. */
{
  OpGrayImage result(*this);
  ApplyGaussGy( *this, sigma, result );
  return result;
}


OpGrayImage OpGrayImage::opGaussDerivGxx( double sigma )
  /* calculate the 2nd order Gaussian derivative in xx direction. */
{
  OpGrayImage result(*this);
  ApplyGaussGxx( *this, sigma, result );
  return result;
}


OpGrayImage OpGrayImage::opGaussDerivGxy( double sigma )
  /* calculate the 2nd order Gaussian derivative in xy direction. */
{
  OpGrayImage result(*this);
  ApplyGaussGxy( *this, sigma, result );
  return result;
}


OpGrayImage OpGrayImage::opGaussDerivGyy( double sigma )
  /* calculate the 2nd order Gaussian derivative in yy direction. */
{
  OpGrayImage result(*this);
  ApplyGaussGyy( *this, sigma, result );
  return result;
}


OpGrayImage OpGrayImage::opGaussMag ( double sigma )
  /* calculate the gradient magnitude */
{
  OpGrayImage result(*this);
  ApplyGaussMag( *this, sigma, result );
  return result;
}


OpGrayImage OpGrayImage::opGaussMag2( double sigma )
  /* calculate the square of the gradient magnitude */
{
  OpGrayImage result(*this);
  ApplyGaussMag2( *this, sigma, result );
  return result;
}


OpGrayImage OpGrayImage::opGaussLap ( double sigma )
  /* calculate the Laplacian */
{
  OpGrayImage result(*this);
  ApplyGaussLap( *this, sigma, result );
  return result;
}


/*---------------------------------------------------------*/
/*                Fast Derivative Operators                */
/*---------------------------------------------------------*/

OpGrayImage OpGrayImage::opFastGauss( double sigma )
  /* filter the image with a Gaussian */
{
  OpGrayImage result(*this);
  FastGauss( *this, sigma, result );
  return result;
}


OpGrayImage OpGrayImage::opFastGaussDx( double sigma )
  /* calculate the Gaussian derivative in x direction */
{
  OpGrayImage result(*this);
  FastGaussDx( *this, sigma, result );
  return result;
}


OpGrayImage OpGrayImage::opFastGaussDy( double sigma )
  /* calculate the Gaussian derivative in y direction */
{
  OpGrayImage result(*this);
  FastGaussDy( *this, sigma, result );
  return result;
}


OpGrayImage OpGrayImage::opFastGaussMag( double sigma )
  /* calculate the gradient magnitude */
{
  OpGrayImage result(*this);
  FastGaussMag( *this, sigma, result );
  return result;
}


OpGrayImage OpGrayImage::opFastGaussMag2( double sigma )
  /* calculate the square of the gradient magnitude */
{
  OpGrayImage result(*this);
  FastGaussMag2( *this, sigma, result );
  return result;
}


OpGrayImage OpGrayImage::opFastGaussDir( double sigma )
  /* calculate the gradient direction */
{
  OpGrayImage result(*this);
  FastGaussDir( *this, sigma, result );
  return result;
}


void OpGrayImage::opFastGaussDxDy( double sigma, 
																	 GrayImage &img_dx, GrayImage &img_dy )
  /* calculate the Gaussian derivatives in x and y direction */
{
  FastGaussDxDy( *this, sigma, img_dx, img_dy );
}


void OpGrayImage::opFastGaussMagDir( double sigma, 
																		 GrayImage &img_mag, GrayImage &img_dir, 
																		 float multfactor )
  /* calculate the gradient magnitude and direction */
{
  FastGaussDxDy( *this, sigma, img_mag, img_dir, multfactor );
}


void OpGrayImage::opFastGaussDxDyMagDir( double sigma, 
																				 GrayImage &img_dx, GrayImage &img_dy,
																				 GrayImage &img_mag, 
																				 GrayImage &img_dir, 
																				 float multfactor )
  /* calculate the Gaussian derivatives and the gradient magnitude   */
	/* and direction */
{
  OpGrayImage img_gauss(*this);	
  FastGaussDxDyMagDir( *this, sigma, img_gauss, img_dx, img_dy, 
											 img_mag, img_dir, multfactor );
}


/*---------------------------------------------------------*/
/*                     Edge Operators                      */
/*---------------------------------------------------------*/

OpGrayImage OpGrayImage::opCannyEdges( float sigma, float tlow, float thigh ) const
	/* calculate an edge image using Canny's method. */
{
  OpGrayImage img_edges(*this);
  OpGrayImage imgOriginal( *this );
	canny( imgOriginal, sigma, tlow, thigh, img_edges );
	return img_edges;
}


void OpGrayImage::opCannyEdges( float sigma, float tlow, float thigh, 
																GrayImage &img_edges, GrayImage &img_dir )
	/* calculate an edge image using Canny's method. */
{
	canny( *this, sigma, tlow, thigh, img_edges, img_dir );
}


void OpGrayImage::opCannyEdges( float sigma, float tlow, float thigh, 
																GrayImage &img_edges, 
                                GrayImage &img_mag, GrayImage &img_dir )
	/* calculate an edge image using Canny's method. */
{
	canny( *this, sigma, tlow, thigh, img_edges, img_mag, img_dir );
}


OpGrayImage cannyEdgesDxDy( const OpGrayImage &imgDx, const OpGrayImage &imgDy,
                            float tlow, float thigh )
	/* calculate an edge image using Canny's method based on a given   */
  /* set of derivatives.                                             */
{
  assert( imgDx.width() == imgDy.width() );
  assert( imgDx.height() == imgDy.height() );

  OpGrayImage img_edges( imgDx.width(), imgDx.height() );	
	canny( imgDx, imgDy, tlow, thigh, img_edges );
	return img_edges;
}


/*---------------------------------------------------------*/
/*                    Region Operators                     */
/*---------------------------------------------------------*/

OpGrayImage OpGrayImage::opDistanceTransform( )
	/* Color the pixels in the image depending on the distance to the  */
	/* nearest background pixel. For simplification purposes, this     */
	/* function assumes a Manhattan distance metric.                   */
{
	std::cout << "  OpGrayImage::opDistanceTransform() called." << std::endl;
	OpGrayImage imgResult( width(), height() );

	/* Perform four passes over the image, one for every direction. */
	/* At every pass assign the distance in the current processing  */
	/* direction if no other direction has yielded a smaller value. */

	/* first sweep: x-direction */
	for( int y=0; y<height(); y++ ) {
		int dist = 0;
		for( int x=0; x<width(); x++ ) {
			if( (*this)(x,y).value() == 0.0 ) { // background pixel
				imgResult(x,y) = 0.0;
				dist = 0;
			} else 
				imgResult(x,y) = (float) ++dist;
		}
	}

	/* second sweep: neg. x-direction */
	for( int y=0; y<height(); y++ ) {
		int dist = 0;
		for( int x=width()-1; x>=0; x-- ) {
			if( (*this)(x,y).value() == 0.0 ) { // background pixel
				imgResult(x,y) = 0.0;
				dist = 0;
			} else
				if( imgResult(x,y).value() > ++dist )
					imgResult(x,y) = (float) dist;
		}
	}

	/* third sweep: y-direction */
	for( int x=0; x<width(); x++ ) {
		int dist = 0;
		for( int y=0; y<height(); y++ ) {
			if( (*this)(x,y).value() == 0.0 ) { // background pixel
				imgResult(x,y) = 0.0;
				dist = 0;
			} else
				if( imgResult(x,y).value() > ++dist )
					imgResult(x,y) = (float) dist;
		}
	}

	/* fourth sweep: neg. y-direction */
	for( int x=0; x<width(); x++ ) {
		int dist = 0;
		for( int y=height()-1; y>=0; y-- ) {
			if( (*this)(x,y).value() == 0.0 ) { // background pixel
				imgResult(x,y) = 0.0;
				dist = 0;
			} else
				if( imgResult(x,y).value() > ++dist )
					imgResult(x,y) = (float) dist;
		}
	}
	
	std::cout << "  done." << std::endl;
	return imgResult;
}


OpGrayImage OpGrayImage::opDistanceFromBorder()
{
	return opDistanceFromBorder( 0, 0, width()-1, height()-1 );
}


OpGrayImage OpGrayImage::opDistanceFromBorder( int x1, int y1, int x2, int y2 )
	/* Color the areas in the image depending on the number of areas   */
	/* between this pixel and the border. For simplification purposes, */
	/* this function assumes a Manhattan distance metric.              */
{
	std::cout << "  OpGrayImage::opDistanceFromBorder() called." << std::endl;
	OpGrayImage imgResult( width(), height() );

	/* Perform four passes over the image, one for every direction. */
	/* At every pass assign the distance in the current processing  */
	/* direction if no other direction has yielded a smaller value. */

	/* first sweep: x-direction */
	for( int y=y1; y<=y2; y++ ) {
		int dist = 0;
		float lastval = 0.0;
		for( int x=x1; x<=x2; x++ ) {
			if( (*this)(x,y).value() != lastval ) { // new area
				lastval = (*this)(x,y).value();
				dist++;
			}
			imgResult(x,y) = (float) dist;
		}
	}
		
	/* second sweep: y-direction */
	for( int x=x1; x<=x2; x++ ) {
		int dist = 0;
		float lastval = 0.0;
		for( int y=y1; y<=y2; y++ ) {
			if( (*this)(x,y).value() != lastval ) { // new area
				lastval = (*this)(x,y).value();
				dist++;
			}

			if( imgResult(x,y).value() > dist )
				imgResult(x,y) = (float) dist;
		}
	}

	/* third sweep: neg. x-direction */
	for( int y=y1; y<=y2; y++ ) {
		int dist = 0;
		float lastval = 0.0;
		for( int x=x2; x>=x1; x-- ) {
			if( (*this)(x,y).value() != lastval ) { // new area
				lastval = (*this)(x,y).value();
				dist++;
			}

			if( imgResult(x,y).value() > dist )
				imgResult(x,y) = (float) dist;
		}
	}

	/* fourth sweep: neg. y-direction */
	for( int x=x1; x<=x2; x++ ) {
		int dist = 0;
		float lastval = 0.0;
		for( int y=y2; y>=y1; y-- ) {
			if( (*this)(x,y).value() != lastval ) { // new area
				lastval = (*this)(x,y).value();
				dist++;
			}

			if( imgResult(x,y).value() > dist )
				imgResult(x,y) = (float) dist;
		}
	}
	
	std::cout << "  done." << std::endl;
	return imgResult;
}


OpGrayImage OpGrayImage::opAreaDistanceTransform( int MAX_COLORS )
{
	return opAreaDistanceTransform( 0, 0, width()-1, height()-1, MAX_COLORS );
}


OpGrayImage OpGrayImage::opAreaDistanceTransform( int x1, int y1, 
																									int x2, int y2, 
																									int MAX_COLORS )
	/* Color the areas in the image depending on the number of areas   */
	/* between this pixel and the border. For simplification purposes, */
	/* this function assumes a Manhattan distance metric.              */
	/* The function begins by labeling connected components (using a   */
	/* maximum of MAX_COLORS labels), then calculates the minimum dis- */
	/* tance to the border for all areas.                              */
{
	std::cout << "  OpGrayImage::opAreaDistanceTransform() called." << std::endl;
	OpGrayImage imgResult( width(), height() );

	/* color the blobs in the image using the connected components     */
	/* algorithm.                                                      */
	int  equivalent[MAX_COLORS];
	long num_points[MAX_COLORS];
	int  new_color[MAX_COLORS];

	/* initialize the equivalency list */
	for( int i=0; i<MAX_COLORS; i++ ) {
		equivalent[i] = i;
		num_points[i] = 0;
	}

	/*****************************************/
	/*   Do the first sweep over the image   */
	/*****************************************/
	int nextColor = 0;

	std::cout << "    Doing a first sweep over the image..." << std::endl;
	/* special case: first pixel */
	imgResult(0,0) = (float) nextColor++;
	num_points[0]++;

	/* special case: first row */
	for( int x=1; x<width(); x++ )
		if( (*this)(x,0).value() == (*this)(x-1,0).value() ) {
			/* The pixel is connected to the pixel to the left */
			imgResult(x,0) = imgResult(x-1,0).value();
			num_points[(int)imgResult(x,0).value()]++;
		} else {
			/* The pixel is not connected yet => create a new color */
			imgResult(x,0) = (float) nextColor;
			num_points[(int)imgResult(x,0).value()]++;
			if( nextColor < MAX_COLORS ) 
				nextColor++;
		}
	
	for( int y=1; y<height(); y++ ) {
		/* special case: first column */
		if( (*this)(0,y).value() == (*this)(0,y-1).value() ) {
			/* The pixel is connected to the pixel above */
			imgResult(0,y) = imgResult(0,y-1).value();
			num_points[(int)imgResult(0,y).value()]++;
		} else {
			/* The pixel is not connected yet => create a new color */
			imgResult(0,y) = (float) nextColor;
			num_points[(int)imgResult(0,y).value()]++;
			if( nextColor < MAX_COLORS ) 
				nextColor++;
		}			

		for( int x=1; x<width(); x++ ) {
			float xc     = (*this)(x,y).value();
			float xl     = (*this)(x-1,y).value();
			float xu     = (*this)(x,y-1).value();
			int color_xu = (int) imgResult(x,y-1).value();
			int color_xl = (int) imgResult(x-1,y).value();
			if( xc == xu )
				if( xc != xl ) {
					/* The pixel is connected to the pixel above */
					imgResult(x,y) = (float) color_xu;
					num_points[(int)imgResult(x,y).value()]++;
					
				} else {  //( xc == xl )
					/* The pixel is connected to both the left and upper     */
					/* pixel. => the colors (left) and (up) are equivalent.  */
					imgResult(x,y) = (float) color_xl;
					num_points[(int)imgResult(x,y).value()]++;
					int c1 = equivalent[color_xu];
					int c2 = equivalent[color_xl];
					if( c1 < c2 ) {
						equivalent[c2] = std::min(c1,equivalent[c2]);
						equivalent[color_xl] = c1;
					} else {
						equivalent[c1] = std::min(c2,equivalent[c1]);
						equivalent[color_xu] = c2;
					}
				}

				else //( xc != xu )
					if( xc == xl ) {
						/* The pixel is connected to the pixel to the left */
						imgResult(x,y) = (float) color_xl;
						num_points[(int)imgResult(x,y).value()]++;
						
					} else { //( xc != xl )
						/* The pixel is not connected yet => create a new color. */
						imgResult(x,y) = (float) nextColor;
						num_points[(int)imgResult(x,y).value()]++;
						if( nextColor < MAX_COLORS )
							nextColor++;
					}
		}
	}
	std::cout << "    Assigned " << nextColor << " color labels..." << std::endl;

	/*********************************/
	/*   Combine equivalent colors   */
	/*********************************/
	/* solve for transitivities and add blob sizes */
	std::cout << "    Resolving transitivities..." << std::endl;
	for( int i=0; i<nextColor; i++ ) {
		if( equivalent[i] != i ) {
			num_points[equivalent[i]] += num_points[i];
			num_points[i] = 0;
		}
		equivalent[i] = equivalent[equivalent[i]];
	}

	/*******************************/
	/*   Assign new color values   */
	/*******************************/
	/* Perform four passes over the image, one for every direction. */
	/* At every pass assign the distance in the current processing  */
	/* direction if no other direction has yielded a smaller value. */

	std::cout << "    Assigning new color values..." << std::endl;
	/* initialize the new colors */
	for( int i=0; i<nextColor; i++ )
		new_color[i] = -1;

	/* first sweep: x-direction */
	for( int y=y1; y<=y2; y++ ) {
		int dist = 0;
		float lastval = imgResult(x1,y).value();
		for( int x=x1+1; x<=x2; x++ ) {
			if( imgResult(x,y).value() != lastval ) { // new area
				lastval = imgResult(x,y).value();
				dist++;
			}
			int idx = equivalent[(int)imgResult(x,y).value()];
			int nc = new_color[idx];
			if( (nc == -1) || (dist < nc) )
				new_color[idx] = dist;
		}
	}
		
	/* second sweep: y-direction */
	for( int x=x1; x<=x2; x++ ) {
		int dist = 0;
		float lastval = imgResult(x,y1).value();
		for( int y=y1+1; y<=y2; y++ ) {
			if( imgResult(x,y).value() != lastval ) { // new area
				lastval = imgResult(x,y).value();
				dist++;
			}
			int idx = equivalent[(int)imgResult(x,y).value()];
			int nc = new_color[idx];
			if( (nc == -1) || (dist < nc) )
				new_color[idx] = dist;
		}
	}

	/* third sweep: neg. x-direction */
	for( int y=y1; y<=y2; y++ ) {
		int dist = 0;
		float lastval = imgResult(x2,y).value();
		for( int x=x2-1; x>=x1; x-- ) {
			if( imgResult(x,y).value() != lastval ) { // new area
				lastval = imgResult(x,y).value();
				dist++;
			}
			int idx = equivalent[(int)imgResult(x,y).value()];
			int nc = new_color[idx];
			if( (nc == -1) || (dist < nc) )
				new_color[idx] = dist;
		}
	}

	/* fourth sweep: neg. y-direction */
	for( int x=x1; x<=x2; x++ ) {
		int dist = 0;
		float lastval = imgResult(x,y2).value();
		for( int y=y2-1; y>=y1; y-- ) {
			if( imgResult(x,y).value() != lastval ) { // new area
				lastval = imgResult(x,y).value();
				dist++;
			}
			int idx = equivalent[(int)imgResult(x,y).value()];
			int nc = new_color[idx];
			if( (nc == -1) || (dist < nc) )
				new_color[idx] = dist;
		}
	}

	/*******************************************************************/
	/*   Do a second sweep over the image & assign the proper colors   */
	/*******************************************************************/
	std::cout << "    Doing a second sweep over the image..." << std::endl;
 	for( int y=0; y<height(); y++ )
 		for( int x=0; x<width(); x++ ) {
			int idx = equivalent[(int)imgResult(x,y).value()];
 			imgResult(x,y) = (float) new_color[idx];
		}
	
	std::cout << "  done." << std::endl;
	return imgResult;
}


OpGrayImage OpGrayImage::opConnectedComponents( int minSize, int MAX_COLORS ) const
	/* color the blobs in the image using the connected components     */
	/* algorithm. The function assumes that any value above 0.0 is a   */
	/* valid color.                                                    */
{
	std::cout << "  OpGrayImage::opConnectedComponents() called." << std::endl;
	OpGrayImage imgResult( width(), height() );
	int  equivalent[MAX_COLORS];
	long num_points[MAX_COLORS];
	int  new_color[MAX_COLORS];
	int  blob[MAX_COLORS];

	/* initialize the equivalency list */
	for( int i=0; i<MAX_COLORS; i++ ) {
		equivalent[i] = i;
		num_points[i] = 0;
	}

	/*****************************************/
	/*   Do the first sweep over the image   */
	/*****************************************/
	int nextColor = 1;

	std::cout << "    Doing a first sweep over the image..." << std::endl;
	/* special case: first pixel */
	if( (*this)(0,0).value() > 0.0 ) {
		imgResult(0,0) = (float) nextColor++;
		num_points[1]++;
	}

	/* special case: first row */
	for( int x=1; x<width(); x++ )
		if( (*this)(x,0).value() > 0.0 )
			if( (*this)(x-1,0).value() > 0.0 ) {
				/* The pixel is connected to the pixel to the left */
				imgResult(x,0) = imgResult(x-1,0).value();
				num_points[(int)imgResult(x,0).value()]++;
			} else {
				/* The pixel is not connected yet => create a new color */
				imgResult(x,0) = (float) nextColor;
				num_points[(int)imgResult(x,0).value()]++;
				if( nextColor < MAX_COLORS ) 
					nextColor++;
			}
	
	for( int y=1; y<height(); y++ ) {
		/* special case: first column */
		if( (*this)(0,y).value() > 0.0 )
			if( (*this)(0,y-1).value() > 0.0 ) {
				/* The pixel is connected to the pixel above */
				imgResult(0,y) = imgResult(0,y-1).value();
				num_points[(int)imgResult(0,y).value()]++;
			} else {
				/* The pixel is not connected yet => create a new color */
				imgResult(0,y) = (float) nextColor;
				num_points[(int)imgResult(0,y).value()]++;
				if( nextColor < MAX_COLORS ) 
					nextColor++;
			}			

		for( int x=1; x<width(); x++ ) {
			int xc       = (int) ceil((*this)(x,y).value());
			int xl       = (int) ceil((*this)(x-1,y).value());
			int xu       = (int) ceil((*this)(x,y-1).value());
			int color_xu = (int) imgResult(x,y-1).value();
			int color_xl = (int) imgResult(x-1,y).value();
			if( xc == 0 )
				continue;
			else {
				if( xu > 0 )
					if( xl == 0 ) {
						/* The pixel is connected to the pixel above */
						imgResult(x,y) = (float) color_xu;
						num_points[(int)imgResult(x,y).value()]++;

					} else {  //( xl > 0 )
						/* The pixel is connected to both the left and upper     */
						/* pixel. => the colors (left) and (up) are equivalent.  */
						imgResult(x,y) = (float) color_xl;
						num_points[(int)imgResult(x,y).value()]++;
						int c1 = equivalent[color_xu];
						int c2 = equivalent[color_xl];
						if( c1 < c2 ) {
							equivalent[c2] = std::min(c1,equivalent[c2]);
							equivalent[color_xl] = c1;
						} else {
							equivalent[c1] = std::min(c2,equivalent[c1]);
							equivalent[color_xu] = c2;
						}
					}

				else //( xu == 0 )
					if( xl > 0 ) {
						/* The pixel is connected to the pixel to the left */
						imgResult(x,y) = (float) color_xl;
						num_points[(int)imgResult(x,y).value()]++;

					} else { //( xl == 0 )
						/* The pixel is not connected yet => create a new color. */
						imgResult(x,y) = (float) nextColor;
						num_points[(int)imgResult(x,y).value()]++;
						if( nextColor < MAX_COLORS )
							nextColor++;
					}
			}
		}
	}
	std::cout << "    Assigned " << nextColor << " color labels..." << std::endl;

	/*********************************/
	/*   Combine equivalent colors   */
	/*********************************/
	/* solve for transitivities and add blob sizes */
	std::cout << "    Resolving transitivities..." << std::endl;
	for( int i=1; i<nextColor; i++ ) {
		if( equivalent[i] != i ) {
			num_points[equivalent[i]] += num_points[i];
			num_points[i] = 0;
		}
		equivalent[i] = equivalent[equivalent[i]];
	}

	/* sort according to blob size */
	std::cout << "    Sorting according to blob size..." << std::endl;
	int numBlobs = 0;
	for( int i=1; i<nextColor; i++ )
		if( (equivalent[i] == i) && (num_points[i]>=minSize) )
			blob[numBlobs++] = i;

	for( int j=0; j<numBlobs-1; j++ )
		for( int i=1; i<numBlobs-j; i++ )
			if( num_points[blob[i]] > num_points[blob[i-1]] ) {
				int tmp = blob[i-1];
				blob[i-1] = blob[i];
				blob[i] = tmp;
			}

	/* assign new color values */
	std::cout << "    Assigning new color values..." << std::endl;
	for( int i=0; i<numBlobs; i++ )
		new_color[blob[i]] = i+1;
	for( int i=0; i<nextColor; i++ ) {
		/* delete blobs that are too small */
		if( (equivalent[i] == i) && (num_points[i]<minSize) )
			new_color[i] = 0;
		/* resolve transitivities */
		if( equivalent[i] != i )
			new_color[i] = new_color[equivalent[i]];
	}

	std::cout << "    Found " << numBlobs << " connected components..." << std::endl;

	for( int i=0; i<numBlobs; i++ )
		std::cout << "      Component " << setw(3) << i+1 << ": Color " 
				 << new_color[blob[i]] << ", " << num_points[blob[i]] << " pixels." 
				 << std::endl;

	/*******************************************************************/
	/*   Do a second sweep over the image & assign the proper colors   */
	/*******************************************************************/
	std::cout << "    Doing a second sweep over the image..." << std::endl;
 	for( int y=0; y<height(); y++ )
 		for( int x=0; x<width(); x++ )
 			imgResult(x,y) = (float) new_color[ (int) imgResult(x,y).value() ];

	std::cout << "  done." << std::endl;
	return imgResult;
}


void OpGrayImage::opComputeCoG( int &x1, int &y1 )
	/* Compute the image's center of gravity (useful for binary imgs). */
{
  float cx   = 0.0;
  float cy   = 0.0;
  long count = 0;
 	for( int y=0; y<height(); y++ )
 		for( int x=0; x<width(); x++ ) 
      if( (*this)(x,y).value() > 0.0 ) {
      cx += (float)x;
      cy += (float)y;
      count++;
    }
  x1 = (int) floor( cx /((double)count) + 0.5);
  y1 = (int) floor( cy /((double)count) + 0.5);
}


/*---------------------------------------------------------*/
/*                 Generic Filter Operators                */
/*---------------------------------------------------------*/

void OpGrayImage::opCreateGaussKernel1D( float sigma )
	/* create a 1D Gaussian kernel */
{
	MakeGaussKernel1D( sigma, *this );
}


void OpGrayImage::opCreateGaussKernel2D( float sigma )
	/* create a 2D Gaussian kernel */
{
	MakeGaussKernel2D( sigma, *this );
}


// OpGrayImage OpGrayImage::opConvolveWith( GrayImage &mask )
// {
// 	OpGrayImage res( width(), height() );

//   for (int y=0; y < height(); y++)
//     for ( int x=0; x < width(); x++)
// 			res(x,y) = 0.0;

// 	int radius = 
//   for (int y=0; y < height(); y++)
//     for ( int x=0; x < width(); x++) {
// 			for (int ym=0; ym < height(); ym++)
// 				for ( int xm=0; xm < width(); xm++) {
// 					res(x,y) = res(x,y) + img(
// 				}
// 		}
// 	return res;
// }


OpGrayImage OpGrayImage::opNonMaximumSuppression( float suppressval ) 
{
  OpGrayImage result(*this);
  
  int w = this->width();
  int h = this->height();

  /* clear the first and last column */
  for (int i=0; i<w; i++) {
    result(i,0)   = suppressval;
    result(i,h-1) = suppressval;
  }

  /* clear the first and last row */
  for (int i=0; i<h; i++) {
    result(0,i)   = suppressval;
    result(w-1,i) = suppressval;
  }

  // Run through the window and compare pixel with neighbours.
  // If it is not a maximum, set it to suppressval,
  // otherwise leave it as it is.
  for (int i=1; i<w-1; i++) {
    for (int j=1; j<h-1; j++) {
      try {
        if (!isMaximum(*this, i, j)) {
          result(i,j) = suppressval;
        }
      } catch(OutOfBoundaryException) { }
    }
  }
  return result;
}


OpGrayImage OpGrayImage::opNonMaximumSuppression( int windowSize, 
                                                  float suppressval ) 
{
  OpGrayImage result(*this);
  
  int w = this->width();
  int h = this->height();

  // Run through the window and compare pixel with neighbours.
  // If it is not a maximum, set it to suppressval,
  // otherwise leave it as it is.
  for (int i=0; i<w; i++) {
    for (int j=0; j<h; j++) {
      try {
        if (!isMaximum(*this, i, j, windowSize)) {
          result(i,j) = suppressval;
        }
      } catch(OutOfBoundaryException) { }
    }
  }
  return result;
}


/*---------------------------------------------------------*/
/*                   Morphology Operators                  */
/*---------------------------------------------------------*/
void OpGrayImage::opCreateCrossMask ( int radius, float value )
	/* Create a cross-shaped mask */
{
	createCrossMask( radius, value, *this );
}


void OpGrayImage::opCreateCircleMask( int radius, float value )
	/* Create a circle-shaped mask */
{
	createCircleMask( radius, value, *this );
}


OpGrayImage OpGrayImage::opDilate ( GrayImage &mask )
	/* dilate the image with the specified mask */
{
	GrayImage result;
	dilate( *this, mask, result );
	return result;
}


OpGrayImage OpGrayImage::opErode  ( GrayImage &mask )
	/* erode the image with the specified mask */
{
	GrayImage result;
	erode( *this, mask, result );
	return result;
}


OpGrayImage OpGrayImage::opOpening( GrayImage &mask )
	/* perform an opening operation on the image with the given mask */
{
	GrayImage result;
	opening( *this, mask, result );
	return result;
}


OpGrayImage OpGrayImage::opClosing( GrayImage &mask )
	/* perform a closing operation on the image with the given mask */
{
	GrayImage result;
	closing( *this, mask, result );
	return result;
}


OpGrayImage OpGrayImage::opDilateBlockMask ( int radius, 
																						 float FG_VALUE, 
																						 float BG_VALUE )
	/* dilate the image with a block mask of size 2*radius+1 (fast!) */
{
	GrayImage result;
	dilateBlockMask( *this, radius, result, FG_VALUE, BG_VALUE );
	return result;
}


OpGrayImage OpGrayImage::opErodeBlockMask  ( int radius, 
																						 float FG_VALUE, 
																						 float BG_VALUE )
	/* erode the image with a block mask of size 2*radius+1 (fast!) */
{
	GrayImage result;
	erodeBlockMask( *this, radius, result, FG_VALUE, BG_VALUE );
	return result;
}


OpGrayImage OpGrayImage::opOpeningBlockMask( int radius, 
																						 float FG_VALUE, 
																						 float BG_VALUE )
	/* perform an opening operation on the image with a block mask of  */
	/* size 2*radius+1 (fast!)                                         */
{
	GrayImage result;
	openingBlockMask( *this, radius, result, FG_VALUE, BG_VALUE );
	return result;
}


OpGrayImage OpGrayImage::opClosingBlockMask( int radius, 
																						 float FG_VALUE, 
																						 float BG_VALUE )
	/* perform a closing operation on the image with a block mask of   */
	/* size 2*radius+1 (fast!)                                         */
{
	GrayImage result;
	closingBlockMask( *this, radius, result, FG_VALUE, BG_VALUE );
	return result;
}


/*---------------------------------------------------------*/
/*                   Rescaling Operators                   */
/*---------------------------------------------------------*/

OpGrayImage OpGrayImage::opRescaleToWidth( int w )
{
  assert(w>0);

  int h = (int) floor( ((float)height() * w)/((float) width()) + 0.5);
  float factor = ((float)width())/((float)w);

  OpGrayImage result(w,h);

  if( factor <= 1.0 ) {
    /*----------------------------------------------*/
    /* Scale the image up by bilinear interpolation */
    /*----------------------------------------------*/
    float y = 0.0;
    for( int yk=0; yk<h; yk++, y+=factor ) {
      float x = 0.0;
      for( int xk=0; xk<w; xk++, x+=factor ) {
        /* recover the projected pixel coordinates */
        //float x = ((float) xk) * factor;
        //float y = ((float) yk) * factor;

        /* make sure they are within the allowed range */
        assert( (x>=0.0) && (x<=width()) );  // -1
        assert( (y>=0.0) && (y<=height()) ); // -1

        int x0 = (int) floor(x);
        int x1 = (int) ceil(x);
        int y0 = (int) floor(y);
        int y1 = (int) ceil(y);
        if( x0 > width()-1 )  x0--;
        if( x1 > width()-1 )  x1 = x0;
        if( y0 > height()-1 ) y0--;
        if( y1 > height()-1 ) y1 = y0;

        assert( (x0>=0) && (x0<=width()-1) );
        assert( (x1>=0) && (x1<=width()-1) );
        assert( (y0>=0) && (y0<=height()-1) );
        assert( (y1>=0) && (y1<=height()-1) );

        /* interpolate the pixel values */
        if( (x0 == x1) && (y0 == y1) )
          result(xk,yk) = (*this)(x0,y0).value();

        else {
          float px0y0 = (*this)(x0,y0).value();
          float px1y0 = (*this)(x1,y0).value();
          float px0y1 = (*this)(x0,y1).value();
          float px1y1 = (*this)(x1,y1).value();

          float t  = (x-x0);
          float u  = (y-y0);
          result(xk,yk) = ( (1.0-t)*(1.0-u)*px0y0 + t*(1.0-u)*px1y0 + 
                            t*u*px1y1 + (1.0-t)*u*px0y1 );
        }
      }
    }

  } else {
    /*---------------------------------------------------*/
    /* Scale the image down by filtering and subsampling */
    /*---------------------------------------------------*/
    /* filter the image with the scaling factor */
    OpGrayImage imgFiltered( (*this).opFastGauss(sqrt(factor)) );

    /* subsample the image */
    float y=0.0;
    for( int yk=0; yk<h; yk++, y+=factor ) {
      float x=0.0;
      for( int xk=0; xk<w; xk++, x+=factor ) {
        /* recover the projected pixel coordinates */
        //float x = ((float) xk) * factor;
        //float y = ((float) yk) * factor;

        /* make sure they are within the allowed range */
        assert( (x>=0.0) && (x<=width()) );  // -1
        assert( (y>=0.0) && (y<=height()) ); // -1

        int x0 = (int) floor(x);
        int x1 = (int) ceil(x);
        int y0 = (int) floor(y);
        int y1 = (int) ceil(y);
        if( x0 > width()-1 )  x0--;
        if( x1 > width()-1 )  x1 = x0;
        if( y0 > height()-1 ) y0--;
        if( y1 > height()-1 ) y1 = y0;

        assert( (x0>=0) && (x0<=width()-1) );
        assert( (x1>=0) && (x1<=width()-1) );
        assert( (y0>=0) && (y0<=height()-1) );
        assert( (y1>=0) && (y1<=height()-1) );
        
        /* interpolate the pixel values */
        if( (x0 == x1) && (y0 == y1) )
          result(xk,yk) = imgFiltered(x0,y0).value();

        else {
          float px0y0 = imgFiltered(x0,y0).value();
          float px1y0 = imgFiltered(x1,y0).value();
          float px0y1 = imgFiltered(x0,y1).value();
          float px1y1 = imgFiltered(x1,y1).value();

          float dx= x1-x0;
          float dy= y1-y0;

          float t  = (x-x0);
          float u  = (y-y0);
          if( dx > 0.0 ) t /= dx;
          if( dy > 0.0 ) u /= dy;

          result(xk,yk) = ( (1.0-t)*(1.0-u)*px0y0 + t*(1.0-u)*px1y0 + 
                            t*u*px1y1 + (1.0-t)*u*px0y1 );

          //float a = (px1y0 - px0y0);
          //float b = (px0y1 - px0y0);
          //float c = (px1y1 - px1y0) - a;
          //float t  = (x-x0)/dx;
          //float u  = (y-y0)/dy;

          //result(xk,yk) = dx*dy*(px0y0 + a*t + b*u + c*u*t);

        }
      }
    }
  }

  return result;
}


OpGrayImage OpGrayImage::opRescaleToHeight( int h )
{
  assert(h>0);

  int w = (int) floor( ((float)width() * h)/((float) height()) + 0.5);

  return opRescaleToWidth( w );
}


OpGrayImage OpGrayImage::opRescaleToSize( int w, int h )
/* Rescale by different factors in x and y direction. */
{
  assert((w>0) && (h>0));

  OpGrayImage imgRes1 = this->opDeformToWidth   ( w );
  OpGrayImage imgRes2 = imgRes1.opDeformToHeight( h );

  return imgRes2;
}


OpGrayImage OpGrayImage::opDeformToWidth( int w )
/* Deform the image in x direction (while keeping it constant in */
/* y direction).                                                 */
{
  assert(w>0);

  float factorx = ((float)width()) /((float)w);

  int h = height();
  OpGrayImage result(w,h);

  if( factorx <= 1.0 ) {
    /*----------------------------------------------*/
    /* Scale the image up by bilinear interpolation */
    /*----------------------------------------------*/
    float y = 0.0;
    for( int yk=0; yk<h; yk++, y+=1.0 ) {
      float x = 0.0;
      for( int xk=0; xk<w; xk++, x+=factorx ) {
        /* recover the projected pixel coordinates */
        //float x = ((float) xk) * factor;
        //float y = ((float) yk) * factor;

        /* make sure they are within the allowed range */
        assert( (x>=0.0) && (x<=width()) );  // -1
        assert( (y>=0.0) && (y<=height()) ); // -1

        int x0 = (int) floor(x);
        int x1 = (int) ceil(x);
        int y0 = (int) floor(y);
        int y1 = (int) ceil(y);
        if( x0 > width()-1 )  x0--;
        if( x1 > width()-1 )  x1 = x0;
        if( y0 > height()-1 ) y0--;
        if( y1 > height()-1 ) y1 = y0;

        assert( (x0>=0) && (x0<=width()-1) );
        assert( (x1>=0) && (x1<=width()-1) );
        assert( (y0>=0) && (y0<=height()-1) );
        assert( (y1>=0) && (y1<=height()-1) );

        /* interpolate the pixel values */
        if( (x0 == x1) && (y0 == y1) )
          result(xk,yk) = (*this)(x0,y0).value();

        else {
          float px0y0 = (*this)(x0,y0).value();
          float px1y0 = (*this)(x1,y0).value();
          float px0y1 = (*this)(x0,y1).value();
          float px1y1 = (*this)(x1,y1).value();

          float t  = (x-x0);
          float u  = (y-y0);
          result(xk,yk) = ( (1.0-t)*(1.0-u)*px0y0 + t*(1.0-u)*px1y0 + 
                            t*u*px1y1 + (1.0-t)*u*px0y1 );
        }
      }
    }

  } else {
    /*---------------------------------------------------*/
    /* Scale the image down by filtering and subsampling */
    /*---------------------------------------------------*/
    /* filter the image with the scaling factor */
    OpGrayImage imgFiltered( (*this).opFastGauss(pow((double)factorx,0.25)) );

    /* subsample the image */
    float y=0.0;
    for( int yk=0; yk<h; yk++, y+=1.0 ) {
      float x=0.0;
      for( int xk=0; xk<w; xk++, x+=factorx ) {
        /* recover the projected pixel coordinates */
        //float x = ((float) xk) * factor;
        //float y = ((float) yk) * factor;

        /* make sure they are within the allowed range */
        assert( (x>=0.0) && (x<=width()) );  // -1
        assert( (y>=0.0) && (y<=height()) ); // -1

        int x0 = (int) floor(x);
        int x1 = (int) ceil(x);
        int y0 = (int) floor(y);
        int y1 = (int) ceil(y);
        if( x0 > width()-1 )  x0--;
        if( x1 > width()-1 )  x1 = x0;
        if( y0 > height()-1 ) y0--;
        if( y1 > height()-1 ) y1 = y0;

        assert( (x0>=0) && (x0<=width()-1) );
        assert( (x1>=0) && (x1<=width()-1) );
        assert( (y0>=0) && (y0<=height()-1) );
        assert( (y1>=0) && (y1<=height()-1) );
        
        /* interpolate the pixel values */
        if( (x0 == x1) && (y0 == y1) )
          result(xk,yk) = imgFiltered(x0,y0).value();

        else {
          float px0y0 = imgFiltered(x0,y0).value();
          float px1y0 = imgFiltered(x1,y0).value();
          float px0y1 = imgFiltered(x0,y1).value();
          float px1y1 = imgFiltered(x1,y1).value();

          float dx= x1-x0;
          float dy= y1-y0;

          float t  = (x-x0);
          float u  = (y-y0);
          if( dx > 0.0 ) t /= dx;
          if( dy > 0.0 ) u /= dy;

          result(xk,yk) = ( (1.0-t)*(1.0-u)*px0y0 + t*(1.0-u)*px1y0 + 
                            t*u*px1y1 + (1.0-t)*u*px0y1 );

          //float a = (px1y0 - px0y0);
          //float b = (px0y1 - px0y0);
          //float c = (px1y1 - px1y0) - a;
          //float t  = (x-x0)/dx;
          //float u  = (y-y0)/dy;

          //result(xk,yk) = dx*dy*(px0y0 + a*t + b*u + c*u*t);

        }
      }
    }
  }

  return result;
}


OpGrayImage OpGrayImage::opDeformToHeight( int h )
/* Deform the image in y direction (while keeping it constant in */
/* x direction).                                                 */
{
  assert(h>0);

  float factory = ((float)height()) /((float)h);

  int w = width();
  OpGrayImage result(w,h);

  if( factory <= 1.0 ) {
    /*----------------------------------------------*/
    /* Scale the image up by bilinear interpolation */
    /*----------------------------------------------*/
    float y = 0.0;
    for( int yk=0; yk<h; yk++, y+=factory ) {
      float x = 0.0;
      for( int xk=0; xk<w; xk++, x+=1.0 ) {
        /* recover the projected pixel coordinates */
        //float x = ((float) xk) * factor;
        //float y = ((float) yk) * factor;

        /* make sure they are within the allowed range */
        assert( (x>=0.0) && (x<=width()) );  // -1
        assert( (y>=0.0) && (y<=height()) ); // -1

        int x0 = (int) floor(x);
        int x1 = (int) ceil(x);
        int y0 = (int) floor(y);
        int y1 = (int) ceil(y);
        if( x0 > width()-1 )  x0--;
        if( x1 > width()-1 )  x1 = x0;
        if( y0 > height()-1 ) y0--;
        if( y1 > height()-1 ) y1 = y0;

        assert( (x0>=0) && (x0<=width()-1) );
        assert( (x1>=0) && (x1<=width()-1) );
        assert( (y0>=0) && (y0<=height()-1) );
        assert( (y1>=0) && (y1<=height()-1) );

        /* interpolate the pixel values */
        if( (x0 == x1) && (y0 == y1) )
          result(xk,yk) = (*this)(x0,y0).value();

        else {
          float px0y0 = (*this)(x0,y0).value();
          float px1y0 = (*this)(x1,y0).value();
          float px0y1 = (*this)(x0,y1).value();
          float px1y1 = (*this)(x1,y1).value();

          float t  = (x-x0);
          float u  = (y-y0);
          result(xk,yk) = ( (1.0-t)*(1.0-u)*px0y0 + t*(1.0-u)*px1y0 + 
                            t*u*px1y1 + (1.0-t)*u*px0y1 );
        }
      }
    }

  } else {
    /*---------------------------------------------------*/
    /* Scale the image down by filtering and subsampling */
    /*---------------------------------------------------*/
    /* filter the image with the scaling factor */
    OpGrayImage imgFiltered( (*this).opFastGauss(pow((double)factory,0.25)) );

    /* subsample the image */
    float y=0.0;
    for( int yk=0; yk<h; yk++, y+=factory ) {
      float x=0.0;
      for( int xk=0; xk<w; xk++, x+=1.0 ) {
        /* recover the projected pixel coordinates */
        //float x = ((float) xk) * factor;
        //float y = ((float) yk) * factor;

        /* make sure they are within the allowed range */
        assert( (x>=0.0) && (x<=width()) );  // -1
        assert( (y>=0.0) && (y<=height()) ); // -1

        int x0 = (int) floor(x);
        int x1 = (int) ceil(x);
        int y0 = (int) floor(y);
        int y1 = (int) ceil(y);
        if( x0 > width()-1 )  x0--;
        if( x1 > width()-1 )  x1 = x0;
        if( y0 > height()-1 ) y0--;
        if( y1 > height()-1 ) y1 = y0;

        assert( (x0>=0) && (x0<=width()-1) );
        assert( (x1>=0) && (x1<=width()-1) );
        assert( (y0>=0) && (y0<=height()-1) );
        assert( (y1>=0) && (y1<=height()-1) );
        
        /* interpolate the pixel values */
        if( (x0 == x1) && (y0 == y1) )
          result(xk,yk) = imgFiltered(x0,y0).value();

        else {
          float px0y0 = imgFiltered(x0,y0).value();
          float px1y0 = imgFiltered(x1,y0).value();
          float px0y1 = imgFiltered(x0,y1).value();
          float px1y1 = imgFiltered(x1,y1).value();

          float dx= x1-x0;
          float dy= y1-y0;

          float t  = (x-x0);
          float u  = (y-y0);
          if( dx > 0.0 ) t /= dx;
          if( dy > 0.0 ) u /= dy;

          result(xk,yk) = ( (1.0-t)*(1.0-u)*px0y0 + t*(1.0-u)*px1y0 + 
                            t*u*px1y1 + (1.0-t)*u*px0y1 );

          //float a = (px1y0 - px0y0);
          //float b = (px0y1 - px0y0);
          //float c = (px1y1 - px1y0) - a;
          //float t  = (x-x0)/dx;
          //float u  = (y-y0)/dy;

          //result(xk,yk) = dx*dy*(px0y0 + a*t + b*u + c*u*t);

        }
      }
    }
  }

  return result;
}


/***********************************************************/
/*                    Service Functions                    */
/***********************************************************/

float OpGrayImage::calcIntensity( QRgb rgbVal )
{
  return (qRed(rgbVal) + qGreen(rgbVal) + qBlue(rgbVal)) / 3.0;
}


//-----------------------------------------------------------------------------
// Related Functions
//-----------------------------------------------------------------------------

GrayImage createGaussKernel1D( float sigma )
	/* create a 1D Gaussian kernel */
{
	GrayImage kernel;
	MakeGaussKernel1D( sigma, kernel );
	return kernel;
}


GrayImage createGaussKernel2D( float sigma )
	/* create a 2D Gaussian kernel */
{
	GrayImage kernel;
	MakeGaussKernel2D( sigma, kernel );
	return kernel;
}


GrayImage createCrossMask ( int radius, float value )
	/* Create a cross-shaped mask */
{
	GrayImage mask;
	createCrossMask( radius, value, mask );
	return mask;
}


GrayImage createCircleMask( int radius, float value )
	/* Create a circle-shaped mask */
{
	GrayImage mask;
	createCircleMask( radius, value, mask );
	return mask;
}


bool isMaximum(GrayImage& img, int x, int y) 
  /* Tests if the point @(x,y) is a maximum in the image in a 3*3    */
  /* neighborhood. The @img is also passed as parameter for perfor-  */
  /* mance reasons.                                                  */
{
  float center = img(x,y).value();

  try {
    if( img(x-1,y-1).value() < center )
      if( img(x-1,y  ).value() < center )
        if( img(x-1,y+1).value() < center )
          if( img(x  ,y+1).value() < center )
            if( img(x+1,y+1).value() < center )
              if( img(x+1,y  ).value() < center )
                if( img(x+1,y-1).value() < center )
                  if( img(x  ,y-1).value() < center )
                    return true;
  } catch (OpGrayImage::OutOfBoundaryException) {}
  return false;
}


bool isMaximum(GrayImage& img, int x, int y, int windowSize) 
  /* Tests if the point @(x,y) is a maximum in the image the range   */
  /* of the given @windowSize. The @img is also passed as parameter  */
  /* for performance reasons.                                        */
{
  if ((windowSize % 2) == 0) {
    windowSize++;
  }
  int winrad = (windowSize-1)/2;
  int start_x = x - winrad;
  int start_y = y - winrad;
  
  for (int i = start_x; i < start_x + windowSize; i++) {
    for (int j = start_y; j < start_y + windowSize; j++) {
      try {
        if ( img(i,j).value() >= img(x,y).value() &&
             !(i == x && j == y) ) {
          return false;
        }
      } catch (OpGrayImage::OutOfBoundaryException) {}
		}
  }
  return true;
}


