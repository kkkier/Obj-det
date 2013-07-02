/*********************************************************************/
/*                                                                   */
/* FILE         pyramiddata.cc                                       */
/* AUTHORS      Thomas Hug, modified by Bastian Leibe                */
/* EMAIL        thug@student.ethz.ch                                 */
/*                                                                   */
/* CONTENT      Implementation of the per-level data structure for   */
/*              an image pyramid.                                    */
/*                                                                   */
/* BEGIN        Tue Dec 11 2001                                      */
/* LAST CHANGE  Tue Dec 03 2002                                      */
/*                                                                   */
/*********************************************************************/
/***************************************************************************
    copyright            : (C) 2001 by Thomas Hug and ETH Zurich
    email                : thug@student.ethz.ch
 ***************************************************************************/

/****************/
/*   Includes   */
/****************/
#include <math.h>
#include <iostream>
#include <cassert>

#include "pyramiddata.hh"

/*===================================================================*/
/*                         Class PyramidData                         */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

PyramidData::PyramidData()
{
  refcount    = 1;
  levels      = 0;
  scaledImage = 0;
  scales      = 0;
}


PyramidData::PyramidData(const int levels) 
{
  this->levels = levels;
  refcount = 1;
  if (levels > 0) {
    scaledImage = new OpGrayImage[levels+1];
    scales = new float[levels+1];
  } else {
    this->levels = 0;
    scaledImage = 0;
    scales = 0;
  }
}


PyramidData::~PyramidData()
{
  if (refcount > 1) {
    throw ObjectInUseException();
  }
  if (scaledImage != 0) {
    delete[] scaledImage;
    delete[] scales;
  }
}


PyramidData* PyramidData::clone() const 
{
  PyramidData *tmp = new PyramidData(levels);
  tmp->sourceImage = sourceImage;
  for (int i=0; i<levels+1; i++) {
    int widthi = scaledImage[i].width();
    int heighti = scaledImage[i].height();
    tmp->scaledImage[i] = OpGrayImage(widthi, heighti);
    for (int x=0; x < widthi; x++) {
      for (int y=0; y < heighti; y++) {
        tmp->scaledImage[i](x,y) = scaledImage[i](x,y).value();
      }
    }
    tmp->scales[i] = scales[i];
  }
  return tmp;
}


/***********************************************************/
/*                Operators on Scale Levels                */
/***********************************************************/

void PyramidData::opNormalize(int order) 
{
  cout << "  PyramidData::opNormalize called. order=" << order << endl;
  if (scaledImage == 0) {
    throw OutOfBoundaryException();
  }
  if (refcount > 1) {
    PyramidData *tmp = clone();
    refcount--;
    *this = *tmp;
  }

  for (int z=1; z<levels; z++) {
    float normfac = pow(sqrt((float)2), order); //pow(pow(sqrt(2),z), order);
    for (int y=0; y<scaledImage[z].height(); y++) {
      for (int x=0; x<scaledImage[z].width(); x++) {
        scaledImage[z](x,y) = ( scaledImage[z](x,y).value() * normfac );
      }
    }
  }
  cout << "  PyramidData::opNormalize exit." << endl;
}


void PyramidData::opNonMaximumSuppression3D(int cubeSize) 
{
  cout << "  PyramidData::opNonMaximumSuppression3D called. cubeSize=" 
       << cubeSize << endl;
  if (scaledImage == 0) {
    throw OutOfBoundaryException();
  }
  cubeSize = (int) floor((double)cubeSize);
  //PyramidData result(*this);
  
  if (refcount > 1) {
    PyramidData *tmp = clone();
    refcount--;
    *this = *tmp;
  }
  
  for (int z=0; z<levels; z++) {
    for (int x=0; x<scaledImage[z].width(); x++) {
      for (int y=0; y<scaledImage[z].height(); y++) {
        if ( !isMaximum3D(x, y, z, cubeSize) ) {
          //result(x,y,z) = GrayPixel( 0.0 );
          int xp, yp;
          convertToPyramid( x, y, z, xp, yp );
          scaledImage[z](xp,yp) = GrayPixel(0.0);
        }
      }
    }
  }
}


void PyramidData::opNonMaximumSuppression2D(int windowSize) 
{
  cout << "  PyramidData::opNonMaximumSuppression2D called. windowSize=" 
       << windowSize << endl;
  if (scaledImage == 0) {
    throw OutOfBoundaryException();
  }
  if (refcount > 1) {
    PyramidData *tmp = clone();
    refcount--;
    *this = *tmp;
  }

  windowSize = (int) floor((double)windowSize);
  for (int z=0; z<levels; z++) {
    scaledImage[z] = scaledImage[z].opNonMaximumSuppression(windowSize);
  }
  cout << "  PyramidData::opNonMaximumSuppression2D exit." << endl;
}


void PyramidData::opSuppressCornerEffects() {
  cout << "  PyramidData::opSuppressCornerEffects called." << endl;
  if (scaledImage == 0) {
    throw OutOfBoundaryException();
  }
  if (refcount > 1) {
    PyramidData *tmp = clone();
    refcount--;
    *this = *tmp;
  }

  /* vectors for every corner point. direction to corner. */
  int neighbour[4][2] = { { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
  int cornerpos = 1, cornerwidth = 3;
  for (int z=0; z<levels; z++) {
    int pos[4][2] = {
      { cornerpos, cornerpos },
      { cornerpos, scaledImage[z].height() - cornerpos - cornerwidth },
      { scaledImage[z].width()  - cornerpos - cornerwidth, cornerpos },
      { scaledImage[z].width()  - cornerpos - cornerwidth,
        scaledImage[z].height() - cornerpos - cornerwidth} };
    // for ever corner do...
    for (int i=0; i<4; i++) {
      // look in corner rectangle with edge length cornerwidth
      for (int x=0; x<cornerwidth; x++) {
        for (int y=0; y<cornerwidth; y++) {
          // if maximum found, set neighbour to same value, 
          // so non-max suppression takes
          // care of it...
          if (isMaximum(scaledImage[z], pos[i][0]+x, pos[i][1]+y)) {
            scaledImage[z](pos[i][0] + neighbour[i][0] + x, 
                           pos[i][1] + neighbour[i][1] + y) =
              scaledImage[z](pos[i][0] + x, pos[i][1] + y).value();
          }
        }
      }
    }
  }
  cout << "  PyramidData::opSuppressCornerEffects exit." << endl;
}


bool PyramidData::isMaximum3D(int x, int y, int z, int dist) const 
{
  if (z > levels-1 || z < 0 || scaledImage == 0 ) {
    cout << "--Coordinates out of level range." << endl;
    throw OutOfBoundaryException();
  }
  if (x < 0 || x > scaledImage[0].width() ||
      y < 0 || y > scaledImage[0].height()) {
    cout << "--Coordinates out of image range." << endl;
    throw OutOfBoundaryException();
  }
  
  int xc, yc;
  convertToPyramid( x, y, z, xc, yc );
  float actual = scaledImage[z](xc,yc).value();
  for (int i = (z-dist); i <= min((z+dist),levels); i++) {
    if (i >= 0 && i < levels) {
      convertToPyramid( x, y, i, xc, yc );
      for (int j = (xc-dist); j <= (xc+dist); j++) {
        for (int k = (yc-dist); k <= (yc+dist); k++) {
          if (!(i == z && j == x && k == y)) {
            try {
              float neighbour = scaledImage[z](xc,yc).value();
              if (neighbour >= actual) {
                return false;
              }
            } catch (OutOfBoundaryException) { }
          }
        }
      }
    }
  }
  return true;
}


bool PyramidData::isMaximum2D(int x, int y, int z, int dist) const 
{
  if (z > levels-1 || z < 0 || scaledImage == 0) {
    cout << "--Coordinates out of level range." << endl;
    throw OutOfBoundaryException();
  }
  if (x < 0 || x > scaledImage[0].width() ||
      y < 0 || y > scaledImage[0].height()) {
    cout << "--Coordinates out of image range." << endl;
    throw OutOfBoundaryException();
  }

  int xc, yc;
  convertToPyramid( x, y, z, xc, yc );
  float actual = scaledImage[z](x,y).value();
  for (int j = (xc-dist); j <= (xc+dist); j++) {
    for (int k = (yc-dist); k <= (yc+dist); k++) {
      if (!(j == xc && k == yc)) {
        try {
          float neighbour = scaledImage[z](xc,yc).value();
          if (neighbour >= actual) {
            return false;
          }
        } catch (OutOfBoundaryException) { }
      }
    }
  }
  return true;
}


int PyramidData::getMaximumOverScales( int x, int y ) const 
  /* Return the level at which the maximum value occurs for a given  */
  /* pixel coordinate.                                               */
{
  if ( scaledImage == 0 ) {
    cout << "--Coordinates out of level range." << endl;
    throw OutOfBoundaryException();
  }
  if (x < 0 || x >= scaledImage[0].width() ||
      y < 0 || y >= scaledImage[0].height()) {
    cout << "--Coordinates out of image range." << endl;
    throw OutOfBoundaryException();
  }

  int xp, yp;
  float last, actual, next;
  //convertToPyramid( x, y, 0, xp, yp );
  //last   = fabs(scaledImage[0](xp,yp).value());
  //last = fabs( interpolatePixel( x, y, 0 ) );
  last = fabs( scaledImage[0](x,y).value() );
  float max_val = last;
  int   max_idx = 0;
  for (int z=1; z<levels; z+=2) {
    //convertToPyramid( x, y, z, xp, yp );
    //actual = fabs(scaledImage[z](xp,yp).value());
    actual = fabs( interpolatePixel( x, y, z ) );
    if( z < levels-1 ) {
      //convertToPyramid( x, y, z+1, xp, yp );
      //next = fabs(scaledImage[z+1](xp,yp).value());
      next = fabs( interpolatePixel( x, y, z+1 ) );
    }

    //if( (actual > last) && (actual > next) )
    // return z;

    if( actual > max_val ) {
      max_val = actual;
      max_idx = z;
    }
    if( next > max_val ) {
      max_val = next;
      max_idx = z+1;
    }
    last = next; //actual;
    //actual = next;
  }
  
  return max_idx;
}


float PyramidData::interpolatePixel( int xk, int yk, int z ) const
  /* Recover the interpolated value of pixel (xk,yk) at scale z by   */
  /* bilinear interpolation.                                         */
{
  assert( (xk>=0) && (xk<=scaledImage[0].width()-1) );
  assert( (yk>=0) && (yk<=scaledImage[0].height()-1) );

  /* calculate the exact pixel positions at level z */
  float x, y;
  int factor;
  if (z % 2 == 0) {
    factor = (1 << z/2);
    x = (float) xk / (float) factor; 
    y = (float) yk / (float) factor; 
  } else {
    factor = (1 << (z-1)/2);
    x = (float) ((float) xk / (float) factor) / 1.5;
    y = (float) ((float) yk / (float) factor) / 1.5;
  }  

//   cout << "      Interpolating pixel (" << xk << "," << yk << "," << z 
//        << ") -> (" << x << "," << y << ")" 
//        << " of image(" << scaledImage[z].width() << "," 
//        << scaledImage[z].height() << ")" << endl;

//   bool boundary = false;
//   if ( x >= scaledImage[z].width()-1 ) {
//     x = scaledImage[z].width() - 1;
//     boundary = true;
//   } 
//   if ( y >= scaledImage[z].height()-1 ) {
//     y = scaledImage[z].height() - 1;
//     boundary = true;
//   }
  bool boundary = false;
  if ( x >= scaledImage[z].width() ) {
    x = scaledImage[z].width()-1;
    boundary = true;
  } 
  if ( y >= scaledImage[z].height() ) {
    y = scaledImage[z].height()-1;
    boundary = true;
  }
//   if ( boundary )
//     return scaledImage[z]((int)floor(x),(int)floor(y)).value();

  assert( (x>=0.0) && (x<=scaledImage[z].width()) ); // -1
  assert( (y>=0.0) && (y<=scaledImage[z].height()) ); // -1

  int x0 = (int) floor(x);
  int x1 = (int) ceil(x);
  int y0 = (int) floor(y);
  int y1 = (int) ceil(y);
  if( x0 > scaledImage[z].width()-1 )  x0--;
  if( x1 > scaledImage[z].width()-1 )  x1 = x0;
  if( y0 > scaledImage[z].height()-1 ) y0--;
  if( y1 > scaledImage[z].height()-1 ) y1 = y0;

  assert( (x0>=0) && (x0<=scaledImage[z].width()-1) );
  assert( (x1>=0) && (x1<=scaledImage[z].width()-1) );
  assert( (y0>=0) && (y0<=scaledImage[z].height()-1) );
  assert( (y1>=0) && (y1<=scaledImage[z].height()-1) );

  if( (x0 == x1) && (y0 == y1) )
    return scaledImage[z](x0,y0).value();

  float Tz = scales[z];  // 2.0
  //float Tz = (float) factor;
  float px0y0 = scaledImage[z](x0,y0).value();
  float px1y0 = scaledImage[z](x1,y0).value();
  float px0y1 = scaledImage[z](x0,y1).value();
  float px1y1 = scaledImage[z](x1,y1).value();
//   cout << "      Pixel values: (" << x0 << "," << y0 << ")=" << px0y0
//        << ",  (" << x1 << "," << y0 << ")=" << px1y0
//        << ",  (" << x0 << "," << y1 << ")=" << px0y1
//        << ",  (" << x1 << "," << y1 << ")=" << px1y1 << endl;

//   float a = ( (px1y0 - px0y0) / Tz );
//   float b = ( (px0y1 - px0y0) / Tz );
//   float c = ( (px0y1 - px1y1) / Tz ) + a;

//   float result = a*(x-x0) + b*(y-y0) + c*(x-x0)*(y-y0) + px1y1;

  float t  = (x-x0);
  float u  = (y-y0);

  float result = ( (1.0-t)*(1.0-u)*px0y0 + t*(1.0-u)*px1y0 + t*u*px1y1 + 
                   (1.0-t)*u*px0y1 );

  return result;
}


void PyramidData::convertToPyramid( int xk, int yk, int z, 
                                    int &xp, int &yp ) const 
  /* convert the pixel from Kartesian to pyramid coordinates */
{
  if (z > levels-1 || z < 0 ) {
    cout << "Coordinates out of scale boundaries (" <<
      xk << "," << yk << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }
  if ( (xk < 0 || xk > scaledImage[0].width()) ||
       (yk < 0 || yk > scaledImage[0].height()) ) {
    cout << "Coordinates out of kartesian image boundaries (" <<
      xk << "," << yk << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }

  if (z % 2 == 0) {
    int factor = (1 << z/2);
    xp = xk / factor; 
    yp = yk / factor; 
    //xp = (int) floor( xk / scales[z] ); 
    //yp = (int) floor( yk / scales[z] ); 
  } else {
    int factor = (1 << (z-1)/2);
    xp = (int) floor( (float) (xk / factor) / 1.5 );
    yp = (int) floor( (float) (yk / factor) / 1.5 );
    //xp = (int) floor( xk / scales[z] );
    //yp = (int) floor( yk / scales[z] );
  }

  if (xp >= scaledImage[z].width())
    xp--;
  if (yp >= scaledImage[z].height())
    yp--;
  if (xp < 0 || xp >= scaledImage[z].width() ||
			yp < 0 || yp >= scaledImage[z].height()) {
    cout << "Coordinates out of pyramid image boundaries (" <<
      xp << "," << yp << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }
}


void PyramidData::convertToKartesian( int xp, int yp, int z, 
                                      int &xk, int &yk ) const 
  /* convert the pixel from pyramid to Kartesian coordinates */
{
  if (z > levels-1 || z < 0 ) {
    cout << "Coordinates out of scale boundaries (" <<
      xp << "," << yp << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }
  if ( (xp < 0 || xp > scaledImage[z].width()) ||
       (yp < 0 || yp > scaledImage[z].height()) ) {
    cout << "Coordinates out of pyramid image boundaries (" <<
      xp << "," << yp << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }

  int middle = 0;
  if (z % 2 == 0) {
    middle = (1 << (z/2)-1);   //(int) pow(2,z/2) / 2;
    int factor = (1 << z/2);
    //xk = (int) floor( (float) xp * pow(2,z/2) + middle);
    //yk = (int) floor( (float) yp * pow(2,z/2) + middle);
    xk = xp * factor + middle;
    yk = yp * factor + middle;
  } else {
    middle = (int) floor((1 << (z-1)/2) / (2.0 * 1.5));
    int factor = (1 << (z-1)/2);
    xk = (int) floor( (float)(xp * factor) * 1.5 + middle);
    yk = (int) floor( (float)(yp * factor) * 1.5 + middle);
  }

  // Possible case of overflow: ceiling of pixel-size for new level 
  //    dimensions in resampling by factor 2.
  //    (Ex: old dim level 0: (5,5), new dim level 2: (3,3) middle=1.
  //    Pt(2,2,2) -> (5,5,0) overflow!)
  if (xk >= scaledImage[0].width()) {
    xk = xk - middle + (int) floor( (double)(scaledImage[0].width() - 
                                             xk) / 2);
  }
  if (yk >= scaledImage[0].height()) {
    yk = yk - middle + (int) floor( (double)(scaledImage[0].height() - 
                                             yk) / 2);
  }
  // Should be ok after test for overflow. Testing for debugging.
  if ( (xk < 0 || xk >= scaledImage[0].width()) ||
       (yk < 0 || yk >= scaledImage[0].height())) {
    cout << "Coordinates out of kartesian image boundaries (" <<
      xk << "," << yk << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }
}

