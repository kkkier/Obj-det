/******************************************************************************
 * $Id: rgbpixel.h,v 1.1.1.1 2004/09/08 13:50:20 mfritz Exp $
 * $Author: mfritz $
 * $Date: 2004/09/08 13:50:20 $
 *------------------------------ Description ----------------------------------
 * Declaration of a RGB pixel class.
 *------------------------------- CVS logs ------------------------------------
 * $Log: rgbpixel.h,v $
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.2  2004/04/26 15:16:33  leibe
 *
 * reinstated the original version of the library 'pccvImage'.
 *
 *****************************************************************************/

#ifndef _SPENGLER_RGB_PIXEL_H
#define _SPENGLER_RGB_PIXEL_H

#ifdef _USE_PERSONAL_NAMESPACES
namespace Spengler {
#endif

/******************************************************************************
 *                              class RGBPixel                                *
 *****************************************************************************/
typedef unsigned char uchar;

class RGBPixel
{
 public:
  RGBPixel(const uchar red=0,const uchar green=0,const uchar blue=0)
    : _red(red), _green(green), _blue(blue) {}
  RGBPixel(const RGBPixel& src) 
    : _red(src._red), _green(src._green), _blue(src._blue){}
  
  uchar red() const {return _red;}
  uchar green() const {return _green;}
  uchar blue() const {return _blue;}

  RGBPixel& operator=(const RGBPixel& src){
    _red = src._red; _green = src._green; _blue = src._blue; return *this;}

 private:
  uchar _red, _green, _blue;
};

#ifdef _USE_PERSONAL_NAMESPACES
}
#endif

#endif // _SPENGLER_RGB_PIXEL_H
