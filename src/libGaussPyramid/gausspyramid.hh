/*********************************************************************/
/*                                                                   */
/* FILE         gausspyramid.hh                                      */
/* AUTHORS      Thomas Hug, modified by Bastian Leibe                */
/* EMAIL        thug@student.ethz.ch, leibe@inf.ethz.ch              */
/*                                                                   */
/* CONTENT      Implementation of a Gaussian Pyramid.                */
/*                                                                   */
/* BEGIN        Mon Dec 02 2002                                      */
/* LAST CHANGE  Mon Dec 02 2002                                      */
/*                                                                   */
/*********************************************************************/


#ifndef GAUSSPYRAMID_HH
#define GAUSSPYRAMID_HH

/****************/
/*   Includes   */
/****************/
#include "pyramiddata.hh"


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                        Class GaussPyramid                         */
/*===================================================================*/
/* Define a pyramid scale space class. */
class GaussPyramid {
public:
  static const int minImageSize = 14;
  
  GaussPyramid();
  GaussPyramid(const GaussPyramid& source);
  GaussPyramid(const OpGrayImage& source, int level);
  ~GaussPyramid();
  
public:
  /***************************/
  /*   Data Access Methods   */
  /***************************/
  float getScaleAtLevel(int level) const;
  int	  getLevelAtScale(float scale) const;
  int   getNumLevels() const;

  virtual OpGrayImage getImage(int l);

  PyramidData* getData() {return data;}

public:
  /*****************************/
  /*   Data Access Operators   */
  /*****************************/
  virtual GrayPixel  operator()(const int x, const int y, const int z) const;
  virtual GrayPixel& operator()(const int x, const int y, const int z);
  GaussPyramid& operator=(GaussPyramid other);
  
public:
  /*********************************/
  /*   Operators on Scale Levels   */
  /*********************************/
  virtual void opNormalize( int order=1 );
  virtual void opNonMaximumSuppression2D( int windowSize=3 );
  virtual void opNonMaximumSuppression3D( int cubeSize=3 );
  virtual void opSuppressCornerEffects();
  
  virtual bool isMaximum3D(int x, int y, int z, int dist=1) const;
  virtual bool isMaximum2D(int x, int y, int z, int dist=1) const;
  
  void convertToPyramid  (int xk, int yk, int z, int &xp, int &yp ) const;
  void convertToKartesian(int xp, int yp, int z, int &xk, int &yk ) const;
  
protected:
  /*************************/
  /*   Service Functions   */
  /*************************/
  OpGrayImage resampleUp(const OpGrayImage& source) const;
  OpGrayImage resampleDown15(const OpGrayImage& source) const;
  OpGrayImage resampleDown20(const OpGrayImage& source) const;

protected:
  PyramidData* data;
};

#endif
