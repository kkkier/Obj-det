
#include "rotate.hh"

#include <math.h>

/**************/
/*   global   */
/**************/

inline float rotatePixel( int x, int y, int ox, int oy, GrayImage& sourceImg,
                          float fAngle, float fColorBack )
{
  float fCos = cos( fAngle );
  float fSin = sin( fAngle );

  float fa = (x-ox)*fCos - (y-oy)*fSin + ox;
  float fb = (x-ox)*fSin + (y-oy)*fCos + oy;
  int na = (int)fa;
  int nb = (int)fb;
  float a = fa - na;
  float b = fb - nb;

  // destination pixel value
  float res = fColorBack;
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

void rotate( GrayImage& sourceImg, float fAngle, float fColorBack, GrayImage& destImg )
{
  int w = sourceImg.width();
  int h = sourceImg.height();
  GrayImage result( w, h );

  int ox = w/2;
  int oy = h/2;
  for ( int y = 0; y < h; y++ )
    for ( int x = 0; x < w; x++ )
      result( x, y ) = rotatePixel( x, y, ox, oy, sourceImg, fAngle, fColorBack );

  destImg = result;
}

