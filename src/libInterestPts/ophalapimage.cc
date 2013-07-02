/*********************************************************************/
/*                                                                   */
/* FILE         ophalapimage.cc                                      */
/* AUTHORS      Thomas Hug, modified by Bastian Leibe                */
/* EMAIL        thug@student.ethz.ch                                 */
/*                                                                   */
/* CONTENT      Implementation of the Harris-Laplacian Interest      */
/*              Point Detector.                                      */
/*                                                                   */
/* BEGIN        Wed Dec 12 2001                                      */
/* LAST CHANGE  Wed Jan 09 2002                                      */
/*                                                                   */
/*********************************************************************/
/***************************************************************************
                          opharrislaplaciangrayimage.cpp  -  description

      Implementation of the Harris-Laplacian Interest Point Detector.
      Builds the scale space of a given image and applies both the Harris and
      (on a copy) the Laplacian operator on it.
      Checks if Interest Points found in the Harris Scale Space are a maximum
      in scale direction in the Laplacian Scale Space -> Point is result point.
		
      Returns a HarrisLaplacianResult, which consists of a image containing
      the Laplacian values of the result points, an image with the according
      scale and an image with the Harris values.

                             -------------------
    begin                : Wed Dec 12 2001
    change               : Tue Jan 08 2002
    copyright            : (C) 2001 by Thomas Hug and ETH Zurich
    email                : thug@student.ethz.ch
 ***************************************************************************/

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <math.h>

#include <scalespace.hh>
#include <pyramidscalespace.hh>
#include "opharrisimage.hh"
#include "ophalapimage.hh"
#include "oplindebergimage.hh"

/*===================================================================*/
/*                         Class OpHalapImage                        */
/*===================================================================*/

/***********************************************************/
/*                Harris-Laplacian Operators               */
/***********************************************************/

OpHalapResult OpHalapImage::opHarrisLaplacian( double sigma0, 
                                               float scaleChange,
                                               int level, double sigma2, 
                                               double alpha, double threshold)
{
  cout << "OpHalapImage::opHarrisLaplacian called." << endl;
  ScaleSpace harrisSS(*this, sigma0, scaleChange, level);
  ScaleSpace laplacian(harrisSS);
  laplacian.opLaplacian();
  
  OpGrayImage result(*this);
  OpGrayImage scales(*this);
  OpGrayImage harris(*this);
  
  //harrisSS.opHarris(sigma2, alpha);
  applyHarrisOnScaleSpace( harrisSS, sigma2, alpha );
  harrisSS.opNonMaximaSuppression2D();
  for (int x=0; x<this->width(); x++) {
    for (int y=0; y<this->height(); y++) {
      result(x,y) = 0.0;
      scales(x,y) = 0.0;
      harris(x,y) = 0.0;
      for (int z=1; z<harrisSS.getLevel(); z++) {
        if (harrisSS(x,y,z).value() > threshold) {
          try {
            //*** test for maximum in scales
            if (laplacian(x,y,z).value() > laplacian(x,y,z-1).value() &&
                laplacian(x,y,z).value() > laplacian(x,y,z+1).value()) {
              result(x,y) = laplacian(x,y,z).value();
              scales(x,y) = laplacian.getScaleAtLevel(z);
              harris(x,y) = harrisSS(x,y,z).value();
              break;
            }
          } catch (OutOfBoundaryException) { }
        }
      }
    }
  }
  OpHalapResult res(result, scales, harris);
  cout << "OpHalapImage::opHarrisLaplacian exit." << endl;
  return res;
}


OpHalapResult OpHalapImage::opFastHarrisLaplacian( int level, double sigma2, 
                                                   double alpha, 
                                                   double threshold )
{
  cout << "OpHalapImage::opFastHarrisLaplacian called." << endl;
  
  PyramidScaleSpace harrisSS(*this, level);
  PyramidScaleSpace laplacian(harrisSS);
  
  laplacian.opDifferenceOfGaussian();
  laplacian.opNormalize(2);
  
  OpGrayImage result(*this);
  OpGrayImage scales(*this);
  OpGrayImage harris(*this);
  for (int x=0; x<this->width(); x++) {
    for (int y=0; y<this->height(); y++) {
      result(x,y) = 0.0;
      scales(x,y) = 0.0;
      harris(x,y) = 0.0;
    }
  }
  
  //harrisSS.opHarris(sigma2, alpha);
  applyHarrisOnScaleSpace( harrisSS, sigma2, alpha );
  harrisSS.opSuppressCornerEffects();
  harrisSS.opNonMaximaSuppression2D();
  
  int xc = 0,  yc = 0;
  int xo = -1, yo = -1;
  for (int z=1; z<harrisSS.getLevel(); z++) {
    harrisSS.getImage(z) = 
      OpInterestImage(harrisSS.getImage(z)).opStrongestPercentage(0.01);
    for (int x=0; x<this->width(); x++) {
      for (int y=0; y<this->height(); y++) {
        xc = x; yc = y;
        harrisSS.convertToPyramid(xc,yc,z);
        if (xc != xo || yc != yo) {
          if (harrisSS.getImage(z)(xc,yc).value() > threshold) {
            try {
              //*** test for maximum in scales
              int xk = xc, yk = yc;
              harrisSS.convertToKartesian(xk, yk, z);
              if (laplacian.getImage(z)(xc,yc).value() > 
                  laplacian(xk,yk,z-1).value() &&
                  laplacian.getImage(z)(xc,yc).value() > 
                  laplacian(xk,yk,z+1).value()) {
                result(xk,yk) = laplacian(x,y,z).value();
                scales(xk,yk) = pow(sqrt(2.0),(double)z);
                harris(xk,yk) = harrisSS(x,y,z).value();
                break;
              }
            } catch (OutOfBoundaryException) { }
          }
        }
        xo = xc; yo = yc;
      }
    }
    xc = 0;  yc = 0;
    xo = -1; yo = -1;
  }
  OpHalapResult res(result, scales, harris);
  cout << "OpHalapImage::opFastHarrisLaplacian exit." << endl;
  return res;
}


OpHalapResult OpHalapImage::opDoGHarrisLaplacian( double sigma0, int level, 
                                                  double sigma2, double alpha, 
                                                  double threshold)
{
  cout << "OpHalapImage::opFastHarrisLaplacian called." << endl;
  ScaleSpace harrisSS(*this, sigma0, sqrt(2.0), level);
  ScaleSpace laplacian(harrisSS);
  laplacian.opDifferenceOfGaussian();
  laplacian.opNormalize(2);
  
  OpGrayImage result(*this);
  OpGrayImage scales(*this);
  OpGrayImage harris(*this);
  
  //harrisSS.opHarris(sigma2, alpha);
  applyHarrisOnScaleSpace( harrisSS, sigma2, alpha );
  harrisSS.opNonMaximaSuppression2D();
  
  for (int x=0; x<this->width(); x++) {
    for (int y=0; y<this->height(); y++) {
      result(x,y) = 0.0;
      scales(x,y) = 0.0;
      harris(x,y) = 0.0;
      for (int z=1; z<harrisSS.getLevel(); z++) {
        if (harrisSS(x,y,z).value() > threshold) {
          try {
            //*** test for maximum in scales
            if (laplacian(x,y,z).value() > laplacian(x,y,z-1).value() &&
                laplacian(x,y,z).value() > laplacian(x,y,z+1).value()) {
              result(x,y) = laplacian(x,y,z).value();
              scales(x,y) = pow(sqrt(2.0),(double)z);
              harris(x,y) = harrisSS(x,y,z).value();
              break;
						}
          } catch (OutOfBoundaryException) { }
        }
      }
    }
  }
  OpHalapResult res(result, scales, harris);
  cout << "OpHalapImage::opFastHarrisLaplacian exit." << endl;
  return res;
}
