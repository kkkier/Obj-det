/*********************************************************************/
/*                                                                   */
/* FILE         grayimage.h                                          */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      GrayImage is an instantiation of the template class  */
/*              Image<Pixel> with value type GrayPixel. GrayPixel    */
/*              implements the basic functionality of a standard     */
/*              grayvalue pixel with an internal storage as float.   */
/*                                                                   */
/* BEGIN        WED Aug 22 2001                                      */
/* LAST CHANGE  WED Aug 22 2001                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_GRAYIMAGE_H
#define LEIBE_GRAYIMAGE_H

#ifdef _USE_PERSONAL_NAMESPACES
namespace Leibe {
#endif

/****************/
/*   Includes   */
/****************/
#include <vector>

#include <image.h>
#include <rgbimage.h>

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
  
  float value() const { return m_value; }

  GrayPixel& operator=( const float& value ){
    m_value = value; return *this;}
  GrayPixel& operator=( const double& value ){
    m_value = (float) value; return *this;}
  GrayPixel& operator=( const GrayPixel& src ){
    m_value = src.m_value; return *this;}
  GrayPixel& operator=( const RGBPixel& src ){
    m_value = ( ((float)src.red()) + 
		((float)src.green()) + 
		((float)src.blue()) )/3.0; 
    return *this;}

 private:
  float m_value;
};

typedef Image<GrayPixel> GrayImage;
 

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

vector<GrayImage> extractImageBands( RGBImage &rgbImg );
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


#ifdef _USE_PERSONAL_NAMESPACES
}
#endif

#endif // LEIBE_GRAYIMAGE_H

