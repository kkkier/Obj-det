/******************************************************************************
 * $Id: hsiimage.h,v 1.1.1.1 2004/09/08 13:50:20 mfritz Exp $
 * $Author: mfritz $
 * $Date: 2004/09/08 13:50:20 $
 *-------------------------------- Description --------------------------------
 * HSIImage is an instantiation of the template class Image<Pixel> with value
 * type HSIPixel. HSIPixel implements the basic functionalities of a standard
 * HSI pixel.
 *---------------------------------- CVS logs ---------------------------------
 * $Log: hsiimage.h,v $
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.2  2004/04/26 15:16:32  leibe
 *
 * reinstated the original version of the library 'pccvImage'.
 *
 * Revision 1.2  2002/05/31 14:00:01  spengler
 * Bug corrected: Invalid default values in constructor of HSIPixel. If
 * saturation is 0.0 (black), hue must be 'undefined'. Otherwise an
 * OutOfRangeException will be thrown.
 *
 * Revision 1.1  2002/03/25 10:15:28  spengler
 * Initial version of header for class 'HSIPixel'.
 *
 *
 *****************************************************************************/

#ifndef _SPENGLER_HSI_PIXEL_H
#define _SPENGLER_HSI_PIXEL_H

#include <image.h>
#include <rgbimage.h>

class HSIPixel {
 public:
  static const double UNDEFINED;
  
  HSIPixel(const double h=UNDEFINED,const double s=0.0,const double i=0.0);
  HSIPixel(const HSIPixel&);
  HSIPixel(const RGBPixel&);
  operator RGBPixel() const;

  double hue() const {return _h;}
  double saturation() const {return _s;}
  double intensity() const {return _i;}

  HSIPixel& operator=(const HSIPixel&);
  HSIPixel& operator=(const RGBPixel&);

  class OutOfRangeException{};
  class InternalErrorException{};

 private:
  double _h;
  double _s;
  double _i;
};

typedef Image<HSIPixel> HSIImage;

#endif // _SPENGLER_HSI_PIXEL_H

