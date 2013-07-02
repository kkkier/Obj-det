/******************************************************************************
 * $Id: pngio.h,v 1.1.1.1 2004/09/08 13:50:20 mfritz Exp $
 * $Author: mfritz $
 * $Date: 2004/09/08 13:50:20 $
 *------------------------------ Description ----------------------------------
 * Utility functions for reading and writing PNG images into/from images of
 * type RGBImage.
 *------------------------------- CVS logs ------------------------------------
 * $Log: pngio.h,v $
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.3  2004/04/26 15:16:33  leibe
 *
 * reinstated the original version of the library 'pccvImage'.
 *
 * Revision 1.4  2002/04/02 14:18:59  spengler
 * Read/write function added which will use a C stream (FILE *) as
 * source/target of operation. This allows to read/write PNG images
 * from/to pipes, e.g. stdin/stdout.
 *
 * Revision 1.3  2002/02/27 18:05:47  spengler
 * Function 'isPNG' added which allows to test whether a given file is
 * a PNG image or not.
 *
 * Revision 1.2  2001/12/10 14:08:36  spengler
 * Library completed with PNG writing capabilities.
 *
 * Revision 1.1  2001/12/06 14:34:11  spengler
 * Initial version of libpngio.so, a library encapsulating I/O for PNG
 * images. Currently, only loading of PNG images works. Writing such
 * images is not yet implemented.
 *
 *****************************************************************************/

#ifndef _SPENGLER_PNG_IO_H
#define _SPENGLER_PNG_IO_H

#include <stdio.h>
#include <string>
#include <rgbimage.h>

class PNGOpenFailedException{};
class PNGWriteFailedException{};

//-----------------------------------------------------------------------------
// Reads image data from a PNG image file 'name'  into an RGBImage instance. 
// If the given file does not exist, readPNG() throws an exception of type 
// FileOpenFailedException.
//-----------------------------------------------------------------------------
RGBImage readPNG(const std::string& name);
RGBImage readPNG(FILE *fp);

//-----------------------------------------------------------------------------
// Writes an RGBImage 'img' to a file specified by 'name' using the PNG file 
// format. If writePNG fails to open the given file for writing, an exception
// of type FileOpenFailedException is thrown. Other failures in writing the
// image into the file are indicated by a return value 'false'. In case of
// success, writePNG will return 'true'.
//-----------------------------------------------------------------------------
void writePNG(const std::string& name,const RGBImage& image);
void writePNG(FILE *fp,const RGBImage& image);

//-----------------------------------------------------------------------------
// Tests ,whether a given file is of image type PNG or not.
//-----------------------------------------------------------------------------
bool isPNG(const std::string& name,unsigned int *width=0,
	   unsigned int *height=0,int *bit_depth=0);

#ifdef _USE_PERSONAL_NAMESPACES
}
#endif

#endif // _SPENGLER_PNG_IO_H
