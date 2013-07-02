/*********************************************************************/
/*                                                                   */
/* FILE         grayimage.h                                          */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      GrayImage is an instantiation of the template class  */
/*              Image<Pixel> with value type GrayPixel. GrayPixel    */
/*              implements the basic functionality of a standard     */
/*              grayvalue pixel with an internal storage as float.   */
/*                                                                   */
/* BEGIN        WED Aug 22 2001                                      */
/* LAST CHANGE  FRI Jun 07 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_GRAYIMAGE_HH
#define LEIBE_GRAYIMAGE_HH

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>
#include <fstream>

#include <image.h>
#include <rgbimage.h>
#include <hsiimage.h>

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                         Class GrayPixel                           */
/*===================================================================*/
/* Define a pixel class with an internal storage as float.           */

class GrayPixel
{
 public:
  GrayPixel( const float value=0.0 )
    : m_value(value) {}
  GrayPixel( const GrayPixel& src ) 
    : m_value(src.m_value) {}
  GrayPixel( const RGBPixel& src ) 
    : m_value( ( ((float)src.red()) + 
		 ((float)src.green()) + 
		 ((float)src.blue()) )/3.0 ) {}
  
  inline float value() const { return m_value; }

  inline GrayPixel& operator=( const float& value ){
    m_value = value; return *this;}
  inline GrayPixel& operator=( const double& value ){
    m_value = (float) value; return *this;}
  inline GrayPixel& operator=( const GrayPixel& src ){
    m_value = src.m_value; return *this;}
  inline GrayPixel& operator=( const RGBPixel& src ){
    m_value = ( ((float)src.red()) + 
		((float)src.green()) + 
		((float)src.blue()) )/3.0; 
    return *this;}

 private:
  float m_value;
};


/*===================================================================*/
/*                         Class GrayImage                           */
/*===================================================================*/
/* Define an image class with an internal storage as GrayPixel.      */
//typedef Image<GrayPixel> GrayImage;

class GrayImage: public Image<GrayPixel>
{
public:
  GrayImage(): Image<GrayPixel>() {}
  GrayImage( const int width, const int height ) 
		: Image<GrayPixel>(width, height) {}
  GrayImage( const GrayImage& other ) : Image<GrayPixel>(other) {}

  GrayImage& operator=( GrayImage other );
  GrayImage& operator=( RGBImage other );

  std::vector<float> getData();
	void loadFromData( int w, int h, float data[] );
	void loadFromData( int w, int h, std::vector<float> data );
	
public:
  /**************************/
  /*   File I/O Functions   */
  /**************************/
  bool load( std::string filename );

private:
  bool readPGM  ( std::ifstream &ifile );
  bool readPCFPM( std::ifstream &ifile ); 

public:
  /*******************************/
  /*   Basic Image Manipulation  */
  /*******************************/
	GrayImage operator+( const GrayImage &other ) const;
	GrayImage operator-( const GrayImage &other ) const;
	GrayImage operator*( const GrayImage &other ) const;
	GrayImage operator/( const GrayImage &other ) const;
	GrayImage operator+( float factor ) const;
	GrayImage operator-( float factor ) const;
	GrayImage operator*( float factor ) const;
	GrayImage operator/( float factor ) const;

	GrayImage add( const GrayImage &other, int pos_x=0, int pos_y=0 ) const;
	GrayImage sub( const GrayImage &other, int pos_x=0, int pos_y=0 ) const;
	GrayImage mul( const GrayImage &other, int pos_x=0, int pos_y=0 ) const;
	GrayImage div( const GrayImage &other, int pos_x=0, int pos_y=0 ) const;
	GrayImage add( float factor ) const;
	GrayImage sub( float factor ) const;
	GrayImage mul( float factor ) const;
	GrayImage div( float factor ) const;

  GrayImage copy( const GrayImage &other,  int pos_x=0, int pos_y=0 ) const;

  GrayImage extractRegion( int x1, int y1, int x2, int y2 ) const;

  GrayImage rotate( float dAngle, float dBackColor=0.0 ) const;
  GrayImage rotateLeft90() const;
  GrayImage rotateRight90() const;

  GrayImage flipHorizontal() const;
  GrayImage flipVertical() const;

protected:
  inline float rotatePixel( int x, int y, int ox, int oy, 
                            const GrayImage& sourceImg,
                            float dAngle, float dBackColor ) const;
};
 

//-----------------------------------------------------------------------------
// Conversion Functions
//-----------------------------------------------------------------------------

GrayImage getIntensityImage( RGBImage &rgbimg );
  /*******************************************************************/
  /* Get an intensity image out of an RGB image.                     */
  /*******************************************************************/
 
GrayImage extractImageBand( int num, RGBImage &rgbImg ); 
  /*******************************************************************/
  /* Extract a color band (r, g, or b) out of an RGB image and store */
  /* it as a grayvalue image. The parameter num determines the band. */
  /* Only the values 0 (=red), 1 (=green), or 2 (=blue) are allowed. */
  /*******************************************************************/

GrayImage extractImageBand( int num, HSIImage &hsiImg ); 
  /*******************************************************************/
  /* Extract a color band (r, g, or b) out of an HSI image and store */
  /* it as a grayvalue image. The parameter num determines the band. */
  /* Only the values 0 (=hue), 1 (=saturation), or 2 (=intensity)    */
  /* allowed.                                                        */
  /*******************************************************************/

std::vector<GrayImage> extractImageBands( RGBImage &rgbImg );
  /*******************************************************************/
  /* Extract all three image bands from an RGB image and store them  */
  /* in a vector of grayvalue images for separate processing.        */
  /*******************************************************************/

std::vector<GrayImage> extractImageBands( HSIImage &hsiImg );
  /*******************************************************************/
  /* Extract all three image bands from an RGB image and store them  */
  /* in a vector of grayvalue images for separate processing.        */
  /*******************************************************************/

RGBImage  combineImageBands( GrayImage &r, GrayImage &g, GrayImage &b );
  /*******************************************************************/
  /* Combine three image bands (stored as grayvalue images) into an  */
  /* RGB image. All input images must have the same dimensions. The  */
  /* pixel values are rounded.                                       */
  /*******************************************************************/

HSIImage  combineImageBandsHSI( GrayImage &h, GrayImage &s, GrayImage &i );
  /*******************************************************************/
  /* Combine three image bands (stored as grayvalue images) into an  */
  /* HSI image. All input images must have the same dimensions. The  */
  /* pixel values are rounded.                                       */
  /*******************************************************************/


#ifdef _USE_PERSONAL_NAMESPACES
}
#endif

#endif // LEIBE_GRAYIMAGE_HH

