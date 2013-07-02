/*********************************************************************/
/*                                                                   */
/* FILE         morphology.hh                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Functions for mathematical morphology on grayvalue   */
/*              images.                                              */
/*                                                                   */
/* BEGIN        WED Jun 12 2002                                      */
/* LAST CHANGE  WED Jun 12 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_MORPHOLOGY_HH
#define LEIBE_MORPHOLOGY_HH

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif
  
/****************/
/*   Includes   */
/****************/
#include <grayimage.hh>

/*******************/
/*   Definitions   */
/*******************/

/***************************/
/*   Function Prototypes   */
/***************************/

/*========================================================================*/
/*           Morphology operators for a "GrayImage" image class           */
/*========================================================================*/

void createCrossMask ( int radius, float value, GrayImage &mask );
void createCircleMask( int radius, float value, GrayImage &mask );

void dilate          ( const GrayImage &image, const GrayImage &mask, 
											 GrayImage &result );
void erode           ( const GrayImage &image, const GrayImage &mask, 
											 GrayImage &result );
void opening         ( const GrayImage &image, const GrayImage &mask, 
											 GrayImage &result );
void closing         ( const GrayImage &image, const GrayImage &mask, 
											 GrayImage &result );

void dilateBlockMask ( const GrayImage &image, int radius, GrayImage &result, 
											 float FG_VALUE=1.0, float BG_VALUE=0.0 );
void erodeBlockMask  ( const GrayImage &image, int radius, GrayImage &result, 
											 float FG_VALUE=1.0, float BG_VALUE=0.0 );
void openingBlockMask( const GrayImage &image, int radius, GrayImage &result, 
											 float FG_VALUE=1.0, float BG_VALUE=0.0 );
void closingBlockMask( const GrayImage &image, int radius, GrayImage &result, 
											 float FG_VALUE=1.0, float BG_VALUE=0.0 );


#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif

#endif // #ifdef LEIBE_MORPHOLOGY_HH 
