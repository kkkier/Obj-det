/*********************************************************************/
/*                                                                   */
/* FILE         ophsiimage.hh                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Define an image class that combines the HSI          */
/*              Image class with Qt functionality for displaying,    */
/*              and that contains many useful image processing ope-  */
/*              rators.                                              */
/*                                                                   */
/* BEGIN        FRI Jun 14 2002                                      */
/* LAST CHANGE  FRI Jun 14 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_OPHSIIMAGE_HH
#define LEIBE_OPHSIIMAGE_HH

using namespace std;
  
/****************/
/*   Includes   */
/****************/
#include <qimage.h>

#include <image.h>
#include <hsiimage.h>

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                         Class OpHSIImage                          */
/*===================================================================*/
/* Define an image class providing the HSI Image class with useful   */
/* functions.                                                        */
class OpHSIImage : public HSIImage
{
public:
  OpHSIImage() : HSIImage() {}
  OpHSIImage( const int width, const int height ) : HSIImage(width, height) {}
  OpHSIImage( const HSIImage& other ) : HSIImage(other) {}
  OpHSIImage( const QImage& src );

  OpHSIImage& operator=( OpHSIImage other );

	void loadFromData( int w, int h, 
										 float data_h[], float data_s[], float data_i[] );
	void loadFromData( int w, int h, float data_hsi[] );

private:
  void copyFrom( const QImage& src );

public:
  /*******************************/
  /*   Content Access Functions  */
  /*******************************/
  QImage getQtImage();
  void getQtImage( QImage &img );

public:
  /*******************************/
  /*   Basic Image Manipulation  */
  /*******************************/
	OpHSIImage operator+( const HSIImage &other );
	OpHSIImage operator-( const HSIImage &other );
	OpHSIImage operator*( const HSIImage &other );
	OpHSIImage operator/( const HSIImage &other );
	OpHSIImage operator+( float factor );
	OpHSIImage operator-( float factor );
	OpHSIImage operator*( float factor );
	OpHSIImage operator/( float factor );

	OpHSIImage add( const HSIImage &other, int pos_x=0, int pos_y=0 );
	OpHSIImage sub( const HSIImage &other, int pos_x=0, int pos_y=0 );
	OpHSIImage mul( const HSIImage &other, int pos_x=0, int pos_y=0 );
	OpHSIImage div( const HSIImage &other, int pos_x=0, int pos_y=0 );
	OpHSIImage add( float fact_h, float fact_s, float fact_i );
	OpHSIImage sub( float fact_h, float fact_s, float fact_i );
	OpHSIImage mul( float fact_h, float fact_s, float fact_i );
	OpHSIImage div( float fact_h, float fact_s, float fact_i );

  /* Background Subtraction */
	OpHSIImage subabs( const HSIImage &other, int pos_x=0, int pos_y=0 );

  OpHSIImage extractRegion( int x1, int y1, int x2, int y2 );

  OpHSIImage rotateLeft90();
  OpHSIImage rotateRight90();
public:
private:

};


//----------------------------------------------------------------------------
// Related Functions
//----------------------------------------------------------------------------

	HSIPixel boundHSIRange( double h, double s, double i );
  /*******************************************************************/
	/* Bound the passed values to the valid HSI range.                 */
  /*******************************************************************/

//----------------------------------------------------------------------------
// Conversion Functions
//----------------------------------------------------------------------------

HSIImage getHSIImage( RGBImage &rgbimg );
  /*******************************************************************/
  /* Get an HSI image out of an RGB image.                           */
  /*******************************************************************/
 
RGBImage getRGBImage( HSIImage &hsiImg ); 
  /*******************************************************************/
  /* Get an RGB image out of an HSI image.                           */
  /*******************************************************************/


#ifdef _USE_PERSONAL_NAMESPACES
}
#endif

#endif // LEIBE_OPGRAYIMAGE_HH
