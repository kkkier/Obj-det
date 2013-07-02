/*********************************************************************/
/*                                                                   */
/* FILE         opinterestimage.hh                                   */
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

#ifndef OPINTERESTIMAGE_HH
#define OPINTERESTIMAGE_HH

/****************/
/*   Includes   */
/****************/
#include <vector>

#include <opgrayimage.hh>


/*****************************/
/*   Constant Declarations   */
/*****************************/
const float gNO_INTEREST_POINT =   0.0;
const float gINTEREST_POINT    = 255.0;


/*************************/
/*   Class Definitions   */
/*************************/
typedef struct _InterestPoint {
  int   x;
  int   y;
  float scale;
  float value;
  float angle;
  float l1;
  float l2;
} InterestPoint;

typedef std::vector<InterestPoint> PointVector;


/*-------------------------------------------------------------------*/
/*                         Sorting Operators                         */
/*-------------------------------------------------------------------*/
struct compInterestPtsScale
{
  bool operator()( InterestPoint a, InterestPoint b )
  { return (a.scale > b.scale); }
};


struct compInterestPtsX
{
  bool operator()( InterestPoint a, InterestPoint b )
  { return (a.x > b.x); }
};


struct compInterestPtsY
{
  bool operator()( InterestPoint a, InterestPoint b )
  { return (a.y > b.y); }
};


/****************************/
/*   Forward Declarations   */
/****************************/
//class OpInterestImage;


/*===================================================================*/
/*                       Class OpInterestImage                       */
/*===================================================================*/
/* Define an image class containing interest point specific functions */
class OpInterestImage : public OpGrayImage  {
public:
  OpInterestImage() : OpGrayImage() {}
  OpInterestImage(const int width, const int height) 
    : OpGrayImage(width, height) {}
  OpInterestImage(const GrayImage& other) : OpGrayImage(other) {}
  OpInterestImage(const QImage& src) : OpGrayImage(src) {}
  
public:
  /****************************************/
  /*   Interest Point Service Functions   */
  /****************************************/
  OpGrayImage 	opStrongestPercentage(float percentage=0.01);
  OpGrayImage 	opStrongestNumber(int number=100);
  OpGrayImage 	opThreshold(float threshold);
  OpGrayImage 	opClipBorder(int border);
  
  PointVector 	getInterestPointVector();
  int  		      numInterestPoints();
};


/*===================================================================*/
/*                       Class OpInterestResult                      */
/*===================================================================*/
/* Define a result class for interest point detectors */
class OpInterestResult {
public:
  OpInterestResult() { }
  OpInterestResult(GrayImage resImg) { m_resultImage = resImg; }
  ~OpInterestResult() { }
  
  OpInterestImage resultImage() { return m_resultImage; }

private:
  OpInterestImage m_resultImage;
};


#endif
