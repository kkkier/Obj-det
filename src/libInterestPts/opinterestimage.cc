/*********************************************************************/
/*                                                                   */
/* FILE         opinterestimage.cc                                   */
/* AUTHORS      Thomas Hug, modified by Bastian Leibe                */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Provides some useful operations for images contai-   */
/*              ning interest points, like non-maximum suppression   */
/*              or point selection according to maximal intensity or */
/*              the n strongest responses.                           */
/*                                                                   */
/* BEGIN        Fri Dec 14 2001                                      */
/* LAST CHANGE  Thu Sep 26 2002                                      */
/*                                                                   */
/*********************************************************************/
/***************************************************************************
    copyright            : (C) 2001 by Thomas Hug and ETH Zurich
    email                : thug@student.ethz.ch
 ***************************************************************************/

/****************/
/*   Includes   */
/****************/
#include <math.h>
#include <q3sortedlist.h>
#include <iostream>

#include "opinterestimage.hh"


/*===================================================================*/
/*                       Class OpInterestImage                       */
/*===================================================================*/

/***********************************************************/
/*             Interest Point Service Functions            */
/***********************************************************/
OpGrayImage OpInterestImage::opStrongestPercentage(float percentage) 
  /* Selects only a percentage of points relative to the strongest   */
  /* response in the whole image.                                    */
{
  float max_intensity = 0.0; float min_intensity = 0.0; float threshold = 0.0;
  OpGrayImage result(*this);
  
  this->getMinMax(min_intensity, max_intensity);
  threshold = max_intensity * percentage;
  for (int x = 0; x < this->width(); x++) {
    for (int y = 0; y < this->height(); y++) {
      if (result(x,y).value() < threshold) {
        result(x,y) = gNO_INTEREST_POINT;
      }
    }
  }
  return result;
}


OpGrayImage OpInterestImage::opStrongestNumber(int number) 
  /* Selects the #number strongest responses in the image. Returns   */
  /* the image containing only those responses.                      */
{
  cout << "OpInterestImage::opStrongestNumber called. number=" 
       << number << endl;
  Q3SortedList<float> intensities;
  OpGrayImage result(*this);
  
  cout << "  Scanning image..." << endl;
  for (int x = 0; x < this->width(); x++) {
    for (int y = 0; y < this->height(); y++) {
      if (result(x,y).value() > gNO_INTEREST_POINT) {
        float tmp = result(x,y).value();
        intensities.append(&tmp);
      }
    }
  }
  if (intensities.count() <= (uint) number) {
    cout << "OpInterestImage::opStrongestNumber exit." << endl;
    return *this;
  }
  cout << "  Sorting intensities..." << endl;
  intensities.sort();
  float* threshold = intensities.at(intensities.count() - number-1);
  for (int x = 0; x < this->width(); x++) {
    for (int y = 0; y < this->height(); y++) {
      if (result(x,y).value() < *threshold) {
        result(x,y) = gNO_INTEREST_POINT;
      }
    }
  }
  cout << "OpInterestImage::opStrongestNumber exit." << endl;
  return result;
}


OpGrayImage OpInterestImage::opThreshold(float threshold) 
{
  cout << "OpInterestImage::opThreshold called. threshold=" 
       << threshold << endl;
  OpGrayImage result(*this);
  
  for (int x = 0; x < this->width(); x++) {
    for (int y = 0; y < this->height(); y++) {
      if (result(x,y).value() < threshold) {
        result(x,y) = 0.0;
      }
    }
  }
  cout << "OpInterestImage::opThreshold exit." << endl;
  return result;
}


OpGrayImage OpInterestImage::opClipBorder(int border) 
  /* Sets all points in the border region with width @border to      */
  /* gNO_INTEREST_POINT. This is to eliminate wrong points due to    */
  /* border effects of certain filters, like opGauss and opLaplace.  */
{
  cout << "OpInterestImage::opClipBorder called. border=" 
       << border << endl;
  if (border > (int) floor((double)this->width()/2.0) ||
      border > (int) floor((double)this->height()/2.0)) {
    return *this;
  }
  OpGrayImage result(*this);
  
  for (int x = 0; x < this->width(); x++) {
    for (int y = 0; y < border; y++) {
      result(x,y) = gNO_INTEREST_POINT;
    }
    for (int y = this->height() - border; y < this->height(); y++) {
      result(x,y) = gNO_INTEREST_POINT;
    }
  }
  for (int y = 0; y < this->height(); y++) {
    for (int x = 0; x < border; x++) {
      result(x,y) = gNO_INTEREST_POINT;
    }
    for (int x = this->width() - border; x < this->width(); x++) {
      result(x,y) = gNO_INTEREST_POINT;
    }
  }
  cout << "OpInterestImage::opClipBorder exit." << endl;
  return result;
}


PointVector OpInterestImage::getInterestPointVector() 
  /* Returns a Vector of all Interest Points in the image; The       */
  /* return type 'InterestPoint' consist of (x,y) position and the   */
  /* gray value.                                                     */
{
  PointVector result(0);
  GrayImage tmp(*this);
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
      if (tmp(i,j).value() > gNO_INTEREST_POINT) {
        InterestPoint pt;
        pt.x = i;
        pt.y = j;
        pt.value = tmp(i,j).value();
        pt.scale = 1.0;
        pt.angle = 0.0;
        pt.l1    = 1.0;
        pt.l2    = 1.0;
        PointVector::iterator where = result.begin();
        result.insert(where, pt);
      }
    }
  }
  return result;
}


int OpInterestImage::numInterestPoints() 
  /* Returns the number of interest points in the image. */
{
  GrayImage img(*this);
  int num = 0;
  
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
      try {
        if (img(i,j).value() > gNO_INTEREST_POINT) {
          num++;
        }
      } catch (OutOfBoundaryException) {}
    }
  }
  return num;
}
