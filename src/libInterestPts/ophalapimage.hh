/*********************************************************************/
/*                                                                   */
/* FILE         ophalapimage.hh                                      */
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
    copyright            : (C) 2001 by Thomas Hug and ETH Zurich
    email                : thug@student.ethz.ch
 ***************************************************************************/

#ifndef OPHALAPIMAGE_H
#define OPHALAPIMAGE_H

//#ifdef _USE_PERSONAL_NAMESPACES
//namespace Hug {
//#endif

/****************/
/*   Includes   */
/****************/
#include "opinterestimage.hh"
#include "oplindebergimage.hh"


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                        Class OpHalapResult                        */
/*===================================================================*/
/* Define a result class for the Harris-Laplacian detector. */
class OpHalapResult : public OpLindebergResult  {
public:
  OpHalapResult() {}
  OpHalapResult(GrayImage resImg, GrayImage sclImg, GrayImage harrisImg)
    : OpLindebergResult(resImg, sclImg) { 
    m_harrisImage = harrisImg;
    //m_scaleImage = sclImg;
  }
  ~OpHalapResult() {}
  
  //OpInterestImage scaleImage()  { return m_scaleImage; }
  OpInterestImage harrisImage() { return m_harrisImage; }

private:
  //OpInterestImage	m_scaleImage;
  OpInterestImage m_harrisImage;
};


/*===================================================================*/
/*                        Class OpHalapImage                         */
/*===================================================================*/
/* Define an image class for calculating Harris-Laplacian interest points. */
class OpHalapImage : public OpInterestImage  {
public:
  OpHalapImage() : OpInterestImage() {}
  OpHalapImage(const int width, const int height) 
    : OpInterestImage(width, height) {}
  OpHalapImage(const GrayImage& other) : OpInterestImage(other) {}
  OpHalapImage(const QImage& src) : OpInterestImage(src) {}
  
public:
  /**********************************/
  /*   Harris-Laplacian Operators   */
  /**********************************/
  OpHalapResult opHarrisLaplacian    (double sigma0=1.0, 
                                      float scaleChange=1.15,
                                      int level=25, 
                                      double sigma2=2.0, 
                                      double alpha=0.06, 
                                      double threshold=0.0);
  
  OpHalapResult opDoGHarrisLaplacian (double sigma0=1.0, 
                                      int level=25,
                                      double sigma2=2.0, 
                                      double alpha=0.06, 
                                      double threshold=0.0);
  
  OpHalapResult opFastHarrisLaplacian(int level=15,
                                      double sigma2=2.0, 
                                      double alpha=0.06, 
                                      double threshold=0.0);
};

//#ifdef _USE_PERSONAL_NAMESPACES
//}
//#endif

#endif
