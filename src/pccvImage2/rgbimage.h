/******************************************************************************
 * $Id: rgbimage.h,v 1.1.1.1 2004/09/08 13:50:20 mfritz Exp $
 * $Author: mfritz $
 * $Date: 2004/09/08 13:50:20 $
 *------------------------------ Description ----------------------------------
 * RGBImage is an instantiation of the template class Image<Pixel> with value
 * type RGBPixel. RGBPixel implements the basic functionality of a standard
 * RGB pixel with an 8bit channel for red, green, and blue.
 *------------------------------- CVS logs ------------------------------------
 * $Log: rgbimage.h,v $
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.2  2004/04/26 15:16:33  leibe
 *
 * reinstated the original version of the library 'pccvImage'.
 *
 * Revision 1.2  2001/08/20 15:44:07  spengler
 * Version 1 of image class and related utility classes and libraries
 * finished. Ready for distribution
 *
 * Revision 1.1  2001/08/17 17:01:36  spengler
 * Running code before weekend (08/17/2001)
 *
 *****************************************************************************/

#ifndef _SPENGLER_RGB_PIXEL_H
#define _SPENGLER_RGB_PIXEL_H

#ifdef _USE_PERSONAL_NAMESPACES
namespace Spengler {
#endif

#include <image.h>

/******************************************************************************
 *                              class RGBPixel                                *
 *****************************************************************************/
typedef unsigned char uchar;

class RGBPixel
{
 public:
  RGBPixel();
  RGBPixel(const uchar red,const uchar green,const uchar blue);
  RGBPixel(const RGBPixel& src);
  
  uchar red() const {return _red;}
  uchar green() const {return _green;}
  uchar blue() const {return _blue;}

  RGBPixel& operator=(const RGBPixel& src);

 private:
  uchar _red, _green, _blue;

};

typedef Image<RGBPixel> RGBImage;

#ifdef _USE_PERSONAL_NAMESPACES
}
#endif

#endif // _SPENGLER_RGB_PIXEL_H

