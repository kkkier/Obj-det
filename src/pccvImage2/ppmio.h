/******************************************************************************
 * $Id: ppmio.h,v 1.1.1.1 2004/09/08 13:50:20 mfritz Exp $
 * $Author: mfritz $
 * $Date: 2004/09/08 13:50:20 $
 *------------------------------ Description ----------------------------------
 * Utility functions for reading and writing PPM images into/from images of
 * type RGBImage.
 *------------------------------- CVS logs ------------------------------------
 * $Log: ppmio.h,v $
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.2  2004/04/26 15:16:33  leibe
 *
 * reinstated the original version of the library 'pccvImage'.
 *
 * Revision 1.2  2001/08/20 15:44:06  spengler
 * Version 1 of image class and related utility classes and libraries
 * finished. Ready for distribution
 *
 * Revision 1.1  2001/08/17 17:01:36  spengler
 * Running code before weekend (08/17/2001)
 *
 *****************************************************************************/

#ifndef _SPENGLER_PPM_IO_H
#define _SPENGLER_PPM_IO_H

#ifdef _USE_PERSONAL_NAMESPACES
namespace Spengler {
#endif

#include <string>
#include <rgbimage.h>

class FileOpenFailedException{};

//-----------------------------------------------------------------------------
// Reads image data from a PPM image file 'name'  into an RGBImage instance. 
// If the given file does not exist, readPPM() throws an exception of type 
// FileOpenFailedException.
//-----------------------------------------------------------------------------
RGBImage readPPM(const std::string& name);

//-----------------------------------------------------------------------------
// Writes an RGBImage 'img' to a file specified by 'name' using the PPM file 
// format. If writePPM fails to open the given file for writing, an exception
// of type FileOpenFailedException is thrown. Other failures in writing the
// image into the file are indicated by a return value 'false'. In case of
// success, writePPM will return 'true'.
//-----------------------------------------------------------------------------
bool writePPM(const std::string& name,const RGBImage& image);

#ifdef _USE_PERSONAL_NAMESPACES
}
#endif

#endif // _SPENGLER_PPM_IO_H
