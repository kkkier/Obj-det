/******************************************************************************
 * $Id: rgbimage.cc,v 1.1.1.1 2004/09/08 13:50:20 mfritz Exp $
 * $Author: mfritz $
 * $Date: 2004/09/08 13:50:20 $
 *------------------------------- CVS logs ------------------------------------
 * $Log: rgbimage.cc,v $
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.2  2004/04/26 15:16:33  leibe
 *
 * reinstated the original version of the library 'pccvImage'.
 *
 *****************************************************************************/

#ifdef _USE_PERSONAL_NAMESPACES
namespace Spengler {
#endif

#include "rgbimage.h"

  RGBPixel::RGBPixel() {
    _red = _green = _blue = 0;
  }

  RGBPixel::RGBPixel(const uchar red,const uchar green,const uchar blue){
    _red = red;
    _green = green;
    _blue = blue;
  }

  RGBPixel::RGBPixel(const RGBPixel& src){
    _red = src._red;
    _green = src._green;
    _blue = src._blue;
  }
  
  RGBPixel& RGBPixel::operator=(const RGBPixel& src){
    _red = src._red;
    _green = src._green; 
    _blue = src._blue; 
    return *this;
  }

#ifdef _USE_PERSONAL_NAMESPACES
}
#endif

