/******************************************************************************
 * $Id: ppmio.cc,v 1.1.1.1 2004/09/08 13:50:20 mfritz Exp $
 * $Author: mfritz $
 * $Date: 2004/09/08 13:50:20 $
 *------------------------------ Description ----------------------------------
 * Utility functions for reading and writing PPM images. 
 *------------------------------- IMPORTANT -----------------------------------
 * Assure header file ppm.h has a conditional extern "C" statement enclosing
 * the function prototypes. Without this, compilation and linkage of C++
 * programs using libppm will fail.
 *------------------------------- CVS logs ------------------------------------
 * $Log: ppmio.cc,v $
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.3  2004/04/26 15:16:33  leibe
 *
 * reinstated the original version of the library 'pccvImage'.
 *
 * Revision 1.3  2001/08/22 16:26:49  spengler
 * Minor error eliminated (call of flcose() when fd == 0).
 *
 * Revision 1.2  2001/08/20 15:44:06  spengler
 * Version 1 of image class and related utility classes and libraries
 * finished. Ready for distribution
 *
 * Revision 1.1  2001/08/17 17:01:36  spengler
 * Running code before weekend (08/17/2001)
 *
 *****************************************************************************/

#include "ppmio.h"

#ifdef __cplusplus
extern "C" {
#endif
#define TRUE
#include <ppm.h>
#ifdef __cplusplus
}
#endif

#include <stdio.h>

//-----------------------------------------------------------------------------
// Read a PPM image file and convert it to a RGBImage.
//-----------------------------------------------------------------------------
RGBImage readPPM(const string& name){
  pixel **data;
  int width, height;
  pixval maxval;
  FILE *fd;

  // create file descriptor (open file...)
  fd = fopen(name.c_str(),"r");
  if(fd == 0){
    throw FileOpenFailedException();
  }

  // read ppm image
  data = ppm_readppm(fd,&width,&height,&maxval);

  // convert to RGBImage
  RGBImage img(width,height);
  for(int y=0;y<height;y++)
    for(int x=0;x<width;x++)
      img(x,y) = RGBPixel((uchar)PPM_GETR(data[y][x]),
			  (uchar)PPM_GETG(data[y][x]),
			  (uchar)PPM_GETB(data[y][x]));
  
  // clean up before leaving...
  ppm_freearray(data,height);
  fclose(fd);
  return img;
}

//-----------------------------------------------------------------------------
// Convert a RGBImage to a PPM image and write it to a file.
// Returns 'true' for success otherwise 'false'.
//-----------------------------------------------------------------------------
bool writePPM(const string& name,const RGBImage& img){
  pixel **data;
  FILE *fd;

  // create file descriptor (open file...)
  fd = fopen(name.c_str(),"w");
  if(fd == 0){
    fclose(fd);
    throw FileOpenFailedException();
  }

  // convert to PPM image
  data = ppm_allocarray(img.width(),img.height());
  if(data == 0)
    return false;

  for(int y=0;y<img.height();y++)
    for(int x=0;x<img.width();x++)
      PPM_ASSIGN(data[y][x],img(x,y).red(),img(x,y).green(),img(x,y).blue());
  
  // write to file
  ppm_writeppm(fd,data,img.width(),img.height(),PPM_MAXMAXVAL,0);

  // clean up before leaving...
  ppm_freearray(data,img.height());
  fclose(fd);
  return true;
}
