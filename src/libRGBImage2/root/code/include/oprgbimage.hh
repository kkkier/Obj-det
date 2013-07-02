/*********************************************************************/
/*                                                                   */
/* FILE         oprgbimage.hh                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Define an image class that combines the rgb          */
/*              Image class with Qt functionality for displaying,    */
/*              and that contains many useful image processing ope-  */
/*              rators.                                              */
/*                                                                   */
/* BEGIN        Fri Jun 21 2002                                      */
/* LAST CHANGE  Fri Jun 21 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_OPRGBIMAGE_HH
#define LEIBE_OPRGBIMAGE_HH

#ifdef _USE_PERSONAL_NAMESPACES
namespace Leibe {
#endif
  
/****************/
/*   Includes   */
/****************/
#include <string>
#include <fstream>

#include <qimage.h>

#include <image.h>
#include <rgbimage.h>

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                         Class OpRGBImage                         */
/*===================================================================*/
/* Define an image class combining the rgb Image class with Qt. */
class OpRGBImage : public RGBImage
{
public:
  OpRGBImage() : RGBImage() {}
  OpRGBImage( const int width, const int height ) : RGBImage(width, height) {}
  OpRGBImage( const RGBImage& other ) : RGBImage(other) {}
  OpRGBImage( const QImage& src );

  OpRGBImage& operator=( OpRGBImage other );

private:
  void copyFrom( const QImage& src );

public:
  /*******************************/
  /*   Content Access Functions  */
  /*******************************/
  QImage getQtImage() const;
  void getQtImage( QImage &img ) const;

public:
  /********************************/
  /*   Basic Image Manipulation   */
  /********************************/
  OpRGBImage extractRegion( int x1, int y1, int x2, int y2 );

  OpRGBImage rotateLeft90();
  OpRGBImage rotateRight90();
};


#ifdef _USE_PERSONAL_NAMESPACES
}
#endif

#endif
