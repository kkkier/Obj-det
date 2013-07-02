/******************************************************************************
 * $Id: pngio.cc,v 1.1.1.1 2004/09/08 13:50:20 mfritz Exp $
 * $Author: mfritz $
 * $Date: 2004/09/08 13:50:20 $
 *------------------------------ Description ----------------------------------
 * Utility functions for reading and writing PNG images. 
 *------------------------------- CVS logs ------------------------------------
 * $Log: pngio.cc,v $
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.3  2004/04/26 15:16:33  leibe
 *
 * reinstated the original version of the library 'pccvImage'.
 *
 * Revision 1.4  2002/04/02 14:18:58  spengler
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

#include "pngio.h"

#include <png.h>
#include <sys/types.h>

//-----------------------------------------------------------------------------
// Read a PNG image file and convert it to a RGBImage.
//-----------------------------------------------------------------------------
RGBImage readPNG(const string& name){
  FILE *fp;
  RGBImage result;

  // open file
  fp = fopen(name.c_str(),"rb");
  if(fp == NULL){
    throw PNGOpenFailedException();
  }
  
  try{
    result = readPNG(fp);
  }
  catch(PNGOpenFailedException e){
    fclose(fp);
    throw;
  }
  
  fclose(fp);
  return result;
}

//-----------------------------------------------------------------------------
// Read a PNG image file and convert it to a RGBImage.
//-----------------------------------------------------------------------------
RGBImage readPNG(FILE *fp){
  png_structp png_ptr = NULL;
  png_infop  info_ptr = NULL;
  unsigned char buffer[8];
  png_uint_32 width,height;
  int bit_depth,color_type;
  unsigned char *data = NULL;
  png_bytep *row_pointers = NULL;

  // check for PNG file type
  fread(buffer,1,8,fp);
  if(!png_check_sig(buffer,8))
    throw PNGOpenFailedException();
  

  // create libpng structures (no user-defined error-handlers)
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
  if(!png_ptr)
    throw PNGOpenFailedException();

  info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr){
    png_destroy_read_struct(&png_ptr,NULL,NULL);
    throw PNGOpenFailedException();
  }

  if(setjmp(png_jmpbuf(png_ptr))){
    png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
    throw PNGOpenFailedException();
  }

  // read image info
  png_init_io(png_ptr,fp);
  png_set_sig_bytes(png_ptr,8);
  png_read_info(png_ptr,info_ptr);
  png_get_IHDR(png_ptr,info_ptr,&width,&height,&bit_depth,&color_type,
	       NULL,NULL,NULL);

  // set up image transforming
  if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png_ptr);
  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_gray_1_2_4_to_8(png_ptr);
  if(png_get_valid(png_ptr,info_ptr,PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png_ptr);
  if(bit_depth == 16)
    png_set_strip_16(png_ptr);
  if(color_type & PNG_COLOR_MASK_ALPHA)
    png_set_strip_alpha(png_ptr);
  if(bit_depth < 8)
    png_set_packing(png_ptr);
  if(color_type == PNG_COLOR_TYPE_GRAY || 
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png_ptr);
  if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_RGB_ALPHA)
    png_set_bgr(png_ptr);
  png_read_update_info(png_ptr,info_ptr);
  png_get_IHDR(png_ptr,info_ptr,&width,&height,&bit_depth,&color_type,
	       NULL,NULL,NULL);

  // read image data
  data = new unsigned char[width*height*3];
  row_pointers = new png_bytep[height];
  for(int i=0;i<height;i++)
    row_pointers[i] = data+(i*width*3);
  png_read_image(png_ptr,row_pointers);
  png_read_end(png_ptr,NULL);
  png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
  delete[] row_pointers;

  // copy image data to RGBImage entity
  RGBImage image((int)width,(int)height);
  unsigned char *current = data;
  for(int y=0;y<height;y++)
    for(int x=0;x<width;x++)
      image(x,y) = RGBPixel(*(current++),*(current++),*(current++));

  // clean up
  delete[] data;

  return image;
}

//-----------------------------------------------------------------------------
// Converts a RGBImage to a PNG image and writes it to a file.
// Returns 'true' for success otherwise 'false'.
//-----------------------------------------------------------------------------
void writePNG(const string& name,const RGBImage& image){
  FILE *fp;

  // open file
  fp = fopen(name.c_str(),"wb");
  if(fp == NULL)
    throw PNGWriteFailedException();

  try{
    writePNG(fp,image);
  }
  catch(PNGWriteFailedException e){
    fclose(fp);
    throw;
  }

  fclose(fp);
}

//-----------------------------------------------------------------------------
// Converts a RGBImage to a PNG image and writes it to a file.
// Returns 'true' for success otherwise 'false'.
//-----------------------------------------------------------------------------
void writePNG(FILE *fp,const RGBImage& image){
  png_structp png_ptr;
  png_infop info_ptr;

  // create and initialize libpng structures
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
  if(png_ptr == NULL)
    throw PNGWriteFailedException();

  // allocate and initialize info structure
  info_ptr = png_create_info_struct(png_ptr);
  if(info_ptr == NULL){
    png_destroy_write_struct(&png_ptr,(png_infopp)NULL);
    throw PNGWriteFailedException();
  }

  // set error handling
  if(setjmp(png_jmpbuf(png_ptr))){
    png_destroy_write_struct(&png_ptr,&info_ptr);
    throw PNGWriteFailedException();
  }

  // initialize I/O
  png_init_io(png_ptr,fp);

  // set up and write image header information
  png_set_IHDR(png_ptr,info_ptr,image.width(),image.height(),8,
	       PNG_COLOR_TYPE_RGB,PNG_INTERLACE_NONE,
	       PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);
  png_write_info(png_ptr,info_ptr);

  // write image data
  unsigned char *data = new unsigned char[image.width()*image.height()*3];
  png_bytep *row_pointers = new png_bytep[image.height()];
  unsigned char *current = data;
  for(int y=0;y<image.height();y++){
    for(int x=0;x<image.width();x++){
      *(current++) = image(x,y).red();
      *(current++) = image(x,y).green();
      *(current++) = image(x,y).blue();
    }    
    row_pointers[y] = data+(y*image.width()*3);
  }
  png_write_image(png_ptr,row_pointers);

  // finish sequential writing of PNG image and clean up
  png_write_end(png_ptr,NULL);
  delete[] row_pointers;
  delete[] data;
  png_destroy_write_struct(&png_ptr,&info_ptr);
}

//-----------------------------------------------------------------------------
// Is given file a PNG image?
//-----------------------------------------------------------------------------
bool isPNG(const string& name,unsigned int *width, unsigned int *height,
	   int *bit_depth)
{
  png_structp png_ptr = NULL;
  png_infop  info_ptr = NULL;
  unsigned char buffer[8];
  png_uint_32 w,h;
  int bd,color_type;
  FILE *fp;

  // open file and check for PNG file type
  fp = fopen(name.c_str(),"rb");
  if(fp == NULL){
    return false;
  }
  
  fread(buffer,1,8,fp);
  if(!png_check_sig(buffer,8)){
    fclose(fp);
    return false;
  }

  // create libpng structures (no user-defined error-handlers)
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
  if(!png_ptr)
    throw PNGOpenFailedException();

  info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr){
    png_destroy_read_struct(&png_ptr,NULL,NULL);
    throw PNGOpenFailedException();
  }

  if(setjmp(png_jmpbuf(png_ptr))){
    png_destroy_read_struct(&png_ptr,&info_ptr,NULL);
    throw PNGOpenFailedException();
  }

  // read image info
  png_init_io(png_ptr,fp);
  png_set_sig_bytes(png_ptr,8);
  png_read_info(png_ptr,info_ptr);
  png_get_IHDR(png_ptr,info_ptr,&w,&h,&bd,&color_type,
	       NULL,NULL,NULL);

  // return requested image informations
//   std::cerr << w << '\t' << h << '\t' << bd;
  if(width != 0){
    *width = (unsigned int)w;
//     std::cerr << '\t' << *width << '\t';
  }
  if(height != 0){
    *height = (unsigned int)h;
//     std::cerr << *height << '\t';
  }
  if(bit_depth != 0){
    *bit_depth = (int)bd;
//     std::cerr << *bit_depth;
  }
//   std::cerr << std::endl << std::flush;

  fclose(fp);
  return true;
}
