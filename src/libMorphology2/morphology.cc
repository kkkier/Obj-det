/*********************************************************************/
/*                                                                   */
/* FILE         morphology.cc                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Functions for mathematical morphology on grayvalue   */
/*              images.                                              */
/*                                                                   */
/* BEGIN        WED Jun 12 2002                                      */
/* LAST CHANGE  WED Jun 12 2002                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cassert>

#include "morphology.hh"

/******************/
/*   Prototypes   */
/******************/

/*========================================================================*/
/*           Morphology operators for a "GrayImage" image class           */
/*========================================================================*/

void createCrossMask( int radius, float value, GrayImage &mask )
	/* Create a cross-shaped mask image */
{
	// initialize the mask
	int width  = 2*radius+1;
	int height = 2*radius+1;
	GrayImage tmp( width, height );
  mask = tmp;

	// make the horizontal bar
	for( int x=0; x<width; x++ )
		mask( x, radius) = value;

	// make the vertical bar
	for( int y=0; y<height; y++ )
		mask(radius, y) = value;	
}


void createCircleMask( int radius, float value, GrayImage &mask )
	/* Create a circle-shaped mask image */
{
	// initialize the mask
	int width  = 2*radius+1;
	int height = 2*radius+1;
	int rad2   = radius*radius;
	GrayImage tmp( width, height );
  mask = tmp;

	// make the circle
	for( int y=-radius; y<=radius; y++ )
		for( int x=-radius; x<=radius; x++ )
			if( (x*x + y*y) <= rad2 )
				mask(x+radius, y+radius) = value;	
}


void dilate( const GrayImage &image, const GrayImage &mask, GrayImage &result )
	/* dilate the image with the specified mask */
{
	// only makes sense for masks of odd size
	assert( (mask.width()  % 2) == 1 );
	assert( (mask.height() % 2) == 1 );

	// initialize the result image
	result = image;
	int rad_x = (mask.width() -1) / 2;
	int rad_y = (mask.height()-1) / 2;

  // DILATE
  for( int y=rad_y; y<image.height()-rad_y; y++ )
		for( int x=rad_x; x<image.width()-rad_x; x++ ) {

      float max = -9999999999999.9;
			for( int v=-rad_y; v<=rad_y; v++) 
				for( int u=-rad_x; u<=rad_x; u++)
					if( mask(u+rad_x,v+rad_y).value() > 0.0 ) {
						float val = mask(u+rad_x,v+rad_y).value() * image(x+u,y+v).value();
						if( val > max )
							max = val;
					}
			
			result(x,y) = max;
		}
}


void erode( const GrayImage &image, const GrayImage &mask, GrayImage &result )
	/* erode the image with the specified mask */
{
	// only makes sense for masks of odd size
	assert( (mask.width()  % 2) == 1 );
	assert( (mask.height() % 2) == 1 );

	// initialize the result image
	result = image;
	int rad_x = (mask.width() -1) / 2;
	int rad_y = (mask.height()-1) / 2;

  // DILATE
  for( int y=rad_y; y<image.height()-rad_y; y++ )
		for( int x=rad_x; x<image.width()-rad_x; x++ ) {

      float min = 9999999999999.9;
			for( int v=-rad_y; v<=rad_y; v++) 
				for( int u=-rad_x; u<=rad_x; u++)
					if( mask(u+rad_x,v+rad_y).value() > 0.0 ) {
						float val = mask(u+rad_x,v+rad_y).value() * image(x+u,y+v).value();
						if( val < min )
							min = val;
					}
			
			result(x,y) = min;
		}
}


void dilateBlockMask( const GrayImage &image, int radius, GrayImage &result, 
											float FG_VALUE, float BG_VALUE )
	/* dilate the image with a block mask of size 2*radius+1 (fast!) */
{
	// initialize the result image
	result = image;

  // DILATE
  for(int y=radius; y<image.height()-radius; y++)
    {
      // compute first position
      int x = radius;

      float sum = 0.0;
      for(int u=-radius; u<=radius; u++)
				for(int v=-radius; v<=radius; v++)
					sum += image(x+u,y+v).value();
			
      if(sum >= FG_VALUE)
        result(x,y) = (float) FG_VALUE;
      else
        result(x,y) = (float) BG_VALUE;
      
			for(x=radius+1; x<image.width()-radius; x++)
				{
					// subtract first column, add last column
					float first_col = 0;
					float last_col  = 0;
					for(int v=-1*radius; v<=radius; v++)
						{
							first_col += image(x-radius-1,y+v).value();
							last_col  += image(x+radius  ,y+v).value();
						}
					
					sum -= first_col;
					sum += last_col;
					
					
					if(sum >= FG_VALUE)
						result(x,y) = (float) FG_VALUE;
					else
						result(x,y) = (float) BG_VALUE;
				}
    }
}


void erodeBlockMask( const GrayImage &image, int radius, GrayImage &result, 
										 float FG_VALUE, float BG_VALUE )
	/* erode the image with a block mask of size 2*radius+1 (fast!) */
{
	// initialize the result image
	result = image;

  // ERODE
  for(int y=radius; y<image.height()-radius; y++)
    {
      // compute first position
      int x = radius;

      float sum = 0.0;
      for(int u=-radius; u<=radius; u++)
				for(int v=-radius; v<=radius; v++)
					sum += image(x+u,y+v).value();
			
      if(sum < (2*radius+1)*(2*radius+1)*FG_VALUE)
        result(x,y) = (float) BG_VALUE;
      else
        result(x,y) = (float) FG_VALUE;

			for(x=radius+1; x<image.width()-radius; x++)
				{
					// subtract first column, add last column
					float first_col = 0;
					float last_col  = 0;
					for(int v=-1*radius; v<=radius; v++)
						{
							first_col += image(x-radius-1,y+v).value();
							last_col  += image(x+radius  ,y+v).value();
						}
					
					sum -= first_col;
					sum += last_col;
					
					
					if(sum < (2*radius+1)*(2*radius+1)*FG_VALUE)
						result(x,y) = (float) BG_VALUE;
					else
						result(x,y) = (float) FG_VALUE;
				}
    }
}


void opening( const GrayImage &image, const GrayImage &mask, 
							GrayImage &result )
	/* perform an opening operation on the image with the given mask */
{
	GrayImage tmp;
	erode ( image, mask, tmp    );
	dilate( tmp  , mask, result );
}


void closing( const GrayImage &image, const GrayImage &mask, 
							GrayImage &result )
	/* perform a closing operation on the image with the given mask */
{
	GrayImage tmp;
	dilate( image, mask, tmp    );
	erode ( tmp  , mask, result );
}


void openingBlockMask( const GrayImage &image, int radius, GrayImage &result, 
											 float FG_VALUE, float BG_VALUE )
	/* perform an opening operation on the image with a block mask of  */
	/* size 2*radius+1 (fast!)                                         */
{
	GrayImage tmp;
	erodeBlockMask ( image, radius, tmp   , FG_VALUE, BG_VALUE );
	dilateBlockMask( tmp  , radius, result, FG_VALUE, BG_VALUE );
}


void closingBlockMask( const GrayImage &image, int radius, GrayImage &result, 
											 float FG_VALUE, float BG_VALUE )
	/* perform a closing operation on the image with a block mask of   */
	/* size 2*radius+1 (fast!)                                         */
{
	GrayImage tmp;
	dilateBlockMask( image, radius, tmp   , FG_VALUE, BG_VALUE );
	erodeBlockMask ( tmp  , radius, result, FG_VALUE, BG_VALUE );
}
