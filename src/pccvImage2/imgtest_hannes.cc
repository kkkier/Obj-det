/******************************************************************************
 * $Id: imgtest_hannes.cc,v 1.1.1.1 2004/09/08 13:50:20 mfritz Exp $
 * $Author: mfritz $
 * $Date: 2004/09/08 13:50:20 $
 *------------------------------ Description ----------------------------------
 * Testing application for 'Image' template class, PPM I/O, and display
 * facilities for RGBImages.
 *------------------------------ Compilation ----------------------------------
 * > g++ -L$PWD -I$PWD -I$QTDIR/include -lrgbdisp -lppmio
 *   -o imgtest imgtest.cc
 *------------------------------- CVS logs ------------------------------------
 * $Log: imgtest_hannes.cc,v $
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.2  2004/04/26 15:16:32  leibe
 *
 * reinstated the original version of the library 'pccvImage'.
 *
 * Revision 1.6  2001/08/21 09:39:37  spengler
 * Instructions for compilation added.
 *
 * Revision 1.5  2001/08/20 15:44:06  spengler
 * Version 1 of image class and related utility classes and libraries
 * finished. Ready for distribution
 *
 * Revision 1.4  2001/08/20 11:55:19  spengler
 * multithreaded version of imgtest. Serves as a prototype for the
 * mechanism implemented in librgbdisp.
 *
 * Revision 1.3  2001/08/17 17:01:36  spengler
 * Running code before weekend (08/17/2001)
 *
 * Revision 1.2  2001/08/16 12:38:04  spengler
 * Image template now works!
 *
 * Revision 1.1  2001/08/15 16:27:06  spengler
 * Basic functionalities implemented but not linkable due to template
 * related errors. It seems as if the static member _ilist of class Image<>
 * cannot be found by the linker.
 *
 *****************************************************************************/

#include <iostream>
#include <unistd.h>

#include <ppmio.h>
#include <image.h>
//#include <rgbdisplay.h>

using namespace std;

int main(int argc, char *argv[])
{
  RGBImage img,out;

  if(argc < 2){
    std::cerr << "Usage: imgtest <ppm_image> [<img_out>]\n";
    exit(1);
  }

  try{
    RGBImage tmp = RGBImage(readPPM(std::string(argv[1])));
    img = tmp;
  }
  catch(FileOpenFailedException e){
    std::cerr << "ERROR: couldn't opne file '" << argv[1] << "'\n";
    exit(1);
  }

  //RGBDisplay disp1(img);
  
  int x,y;
  try{
    RGBImage tmp = RGBImage(img.width(),img.height());
    out = tmp;
    for(y=0;y<img.height();y++)
      for(x=0;x<img.width();x++)
	out(x,y)=RGBPixel(255-img(x,y).red(),
			  255-img(x,y).green(),
			  255-img(x,y).blue());
  }
  catch(RGBImage::OutOfBoundaryException e){
    std::cerr << "ERROR: coordinates (" << x << ',' << y 
	      << ") are out of boundaries (" << img.width()-1 << ','
	      << img.height()-1 << ")\n";
    exit(1);
  }

  //RGBDisplay disp2(out);

  //RGBDisplay::wait();

  if(argc == 3)
    writePPM(string(argv[2]),out);
}
