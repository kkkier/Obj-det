/*********************************************************************/
/*                                                                   */
/* FILE         opharrisimage.hh                                     */
/* AUTHORS      Thomas Hug, modified by Bastian Leibe                */
/* EMAIL        thug@student.ethz.ch                                 */
/*                                                                   */
/* CONTENT      Implementation of the Harris Interest Point          */
/*              Detector.                                            */
/*                                                                   */
/* BEGIN        Mon Nov 05 2001                                      */
/* LAST CHANGE  Wed Jan 23 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifndef HUG_OPHARRISIMAGE_HH
#define HUG_OPHARRISIMAGE_HH

//#ifdef _USE_PERSONAL_NAMESPACES
//namespace Hug {
//#endif

/****************/
/*   Includes   */
/****************/
#include <math.h>

#include <scalespace.hh>
#include <pyramidscalespace.hh>
#include "opinterestimage.hh"


/*****************************/
/*   Constant Declarations   */
/*****************************/
const bool  gDEBUG            = true;
const short kImprovedHarris   = 0;
const short kNormalizedHarris = 1;
const short kWindowHarris     = 2;
const short kPyramidHarris    = 3;


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                       Class OpInterestImage                       */
/*===================================================================*/
/* Define an image class for calculating Harris interest points. */
class OpHarrisImage : public OpInterestImage {
public:
  /***************************/
  /**      Constructors      **/
  /***************************/
  OpHarrisImage() : OpInterestImage() {}
  OpHarrisImage(const int width, const int height) 
    : OpInterestImage(width, height) {}
  OpHarrisImage(const GrayImage& other) : OpInterestImage(other) {}
  OpHarrisImage(const QImage& src) : OpInterestImage(src) {}
  
  /***************************/
  /**      Harris Ops       **/
  /***************************/
  OpGrayImage opWindowHarris    (double sigma=1.0, double alpha=0.06, 
                                 int windowSize=5);
  OpGrayImage opImprovedHarris  (double sigma1=1.0, double sigma2=2.0, 
                                 double alpha=0.06);
  OpGrayImage opNormalizedHarris(double sigma1=1.0, double sigma2=2.0, 
                                 double alpha=0.06);
  OpGrayImage opPyramidHarris   (int level, double sigma1=sqrt(2.0), 
                                 double sigma2=2.0, double alpha=0.06);
  
private:
  /***************************/
  /**  Matrix Computations  **/
  /***************************/
  float det(float *matrix);
  float trace(float *matrix);
  
  /***************************/
  /**    Harris Helpers     **/
  /***************************/
  void calcIntensityVariations(OpGrayImage& img_dxdx, OpGrayImage& img_dydy,
                               OpGrayImage& img_dxdy, double sigma);
  float sumWindow(OpGrayImage& img, int x, int y, int windowSize);
  
};

//-----------------------------------------------------------------------------
// Related Functions
//-----------------------------------------------------------------------------
void applyHarrisOnScaleSpace( ScaleSpace& sspace, const float sigma2=2.0, 
                              const float alpha=0.06, 
                              const short type=kNormalizedHarris );

void applyHarrisOnScaleSpace( PyramidScaleSpace& sspace, 
                              const float sigma2=2.0, 
                              const float alpha=0.06,
                              const short type=kNormalizedHarris );



//#ifdef _USE_PERSONAL_NAMESPACES
//}
//#endif

#endif
