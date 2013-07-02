/*********************************************************************/
/*                                                                   */
/* FILE         scalespace.hh                                        */
/* AUTHORS      Thomas Hug, modified by Bastian Leibe                */
/* EMAIL        thug@student.ethz.ch                                 */
/*                                                                   */
/* CONTENT      Implementation of an image scale space.              */
/*                                                                   */
/* BEGIN        Wed Dec 05 2001                                      */
/* LAST CHANGE  Wed Jan 16 2002                                      */
/*                                                                   */
/*********************************************************************/
/***************************************************************************
    copyright            : (C) 2001 by Thomas Hug and ETH Zurich
    email                : thug@student.ethz.ch
 ***************************************************************************/

#ifndef SCALESPACE_HH
#define SCALESPACE_HH

//#ifdef _USE_PERSONAL_NAMESPACES
//namespace Hug {
//#endif

/****************/
/*   Includes   */
/****************/
#include <opgrayimage.hh>
#include "scalespacedata.hh"


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                         Class ScaleSpace                          */
/*===================================================================*/
/* Define a scale space class. */
class ScaleSpace {
public:
  ScaleSpace();
  ScaleSpace(const ScaleSpace& source);
  ScaleSpace(const OpGrayImage& source, float sigma0, 
             float scaleChange, int level);
  ~ScaleSpace();
  
public:
  /***************************/
  /*   Data Access Methods   */
  /***************************/
  float getScaleAtLevel(int level) const;
  int	  getLevelAtScale(float scale) const;
  float getLevel() const;
  float getCharacteristicScale(int x, int y) const;
  float getCharacteristicScaleValue(int x, int y) const;

  OpGrayImage& getImage(int l);

public:
  /*****************************/
  /*   Data Access Operators   */
  /*****************************/
  GrayPixel   operator()(const int x, const int y, const int z) const;
  GrayPixel&  operator()(const int x, const int y, const int z);
  ScaleSpace& operator=(ScaleSpace other);
  
public:
  /*********************************/
  /*   Operators on Scale Levels   */
  /*********************************/
  ScaleSpace& opSquareGradient();
  ScaleSpace& opLaplacian();
  ScaleSpace& opDifferenceOfGaussian();
  ScaleSpace& opNormalize(const int order=1);
  ScaleSpace& opNonMaximaSuppression2D(const int windowSize=3);
  ScaleSpace& opNonMaximaSuppression3D(const int cubeSize=3);
    
  bool isMaximum3D(int x, int y, int z, int dist=1) const;
  bool isMaximum2D(int x, int y, int z, int dist=1) const;
  
public:
  class OutOfBoundaryException {};

public:
  ScaleSpaceData* data;  
};

//-----------------------------------------------------------------------------
// Related Functions
//-----------------------------------------------------------------------------

ScaleSpace buildScaleSpace(OpGrayImage &img, float sigma0, float scaleChange, 
                           int level);

//#ifdef _USE_PERSONAL_NAMESPACES
//}
//#endif

#endif
