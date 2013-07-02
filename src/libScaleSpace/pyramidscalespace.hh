/*********************************************************************/
/*                                                                   */
/* FILE         pyramidscalespace.hh                                 */
/* AUTHORS      Thomas Hug, modified by Bastian Leibe                */
/* EMAIL        thug@student.ethz.ch                                 */
/*                                                                   */
/* CONTENT      Implementation of an image pyramid scale space.      */
/*                                                                   */
/* BEGIN        Wed Jan 16 2002                                      */
/* LAST CHANGE  Wed Jan 16 2002                                      */
/*                                                                   */
/*********************************************************************/
/***************************************************************************
    copyright            : (C) 2002 by Thomas Hug and ETH Zurich
    email                : thug@student.ethz.ch
 ***************************************************************************/


#ifndef PYRAMIDSCALESPACE_HH
#define PYRAMIDSCALESPACE_HH

/****************/
/*   Includes   */
/****************/
#include "scalespacedata.hh"


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                      Class PyramidScaleSpace                      */
/*===================================================================*/
/* Define a pyramid scale space class. */
class PyramidScaleSpace {
public:
  PyramidScaleSpace();
  PyramidScaleSpace(const PyramidScaleSpace& source);
  PyramidScaleSpace(const OpGrayImage& source, int level, int minSize=14 );
  ~PyramidScaleSpace();
  
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
  GrayPixel          operator()(const int x, const int y, const int z) const;
  GrayPixel&         operator()(const int x, const int y, const int z);
  PyramidScaleSpace& operator=(PyramidScaleSpace other);
  
public:
  /*********************************/
  /*   Operators on Scale Levels   */
  /*********************************/
  PyramidScaleSpace& opSquareGradient();
  PyramidScaleSpace& opLaplacian();
  PyramidScaleSpace& opDifferenceOfGaussian();
  PyramidScaleSpace& opNormalize(const int order=1);
  PyramidScaleSpace& opNonMaximaSuppression2D(const int windowSize=3);
  PyramidScaleSpace& opNonMaximaSuppression3D(const int cubeSize=3);
  PyramidScaleSpace& opSuppressCornerEffects();
  
  bool isMaximum3D(int x, int y, int z, int dist=1) const;
  bool isMaximum2D(int x, int y, int z, int dist=1) const;
  
  void convertToPyramid(int &x, int &y, int z) const;
  void convertToKartesian(int &x, int &y, int z) const;
  
protected:
  /*************************/
  /*   Service Functions   */
  /*************************/
  OpGrayImage resampleUp(OpGrayImage& source) const;
  OpGrayImage resampleDown15(OpGrayImage& source) const;
  OpGrayImage resampleDown20(OpGrayImage& source) const;

 public:
  class OutOfBoundaryException {};

public:
  ScaleSpaceData* data;
  ScaleSpaceData* differences;

public:
  int minImageSize;
};

#endif
