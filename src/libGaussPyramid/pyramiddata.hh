/*********************************************************************/
/*                                                                   */
/* FILE         pyramiddata.hh                                       */
/* AUTHORS      Thomas Hug, modified by Bastian Leibe                */
/* EMAIL        thug@student.ethz.ch                                 */
/*                                                                   */
/* CONTENT      Implementation of the per-level data structure for   */
/*              an image pyramid.                                    */
/*                                                                   */
/* BEGIN        Tue Dec 11 2001                                      */
/* LAST CHANGE  Tue Dec 03 2002                                      */
/*                                                                   */
/*********************************************************************/
/***************************************************************************
    copyright            : (C) 2001 by Thomas Hug and ETH Zurich
    email                : thug@student.ethz.ch
 ***************************************************************************/

#ifndef PYRAMIDDATA_HH
#define PYRAMIDDATA_HH

/****************/
/*   Includes   */
/****************/
#include <opgrayimage.hh>


/****************************/
/*   Forward Declarations   */
/****************************/
class ScaleSpace;
class GaussPyramid;
class LaplacePyramid;

using namespace std;

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                        Class PyramidData                          */
/*===================================================================*/
/* Define a per-level data structure for image scale spaces. */
class PyramidData {
public:
  PyramidData();
  PyramidData( const int );
  ~PyramidData();
  
  PyramidData* clone() const;
  
public:
  /*********************************/
  /*   Operators on Scale Levels   */
  /*********************************/
  void opNormalize( int order=1 );
  void opNonMaximumSuppression2D( int windowSize=3 );
  void opNonMaximumSuppression3D( int cubeSize=3 );
  void opSuppressCornerEffects();
  
  bool isMaximum3D(int x, int y, int z, int dist=1) const;
  bool isMaximum2D(int x, int y, int z, int dist=1) const;
  int  getMaximumOverScales( int x, int y ) const;
  
  float interpolatePixel  ( int xk, int yk, int z ) const;
  void  convertToPyramid  ( int xk, int yk, int z, int &xp, int &yp ) const;
  void  convertToKartesian( int xp, int yp, int z, int &xk, int &yk ) const;

public:
  class ObjectInUseException{};
  class OutOfBoundaryException{};
  
  friend class ScaleSpace;
  friend class GaussPyramid;
  friend class LaplacePyramid;

public:
  int refcount;
  int levels;

  OpGrayImage  sourceImage;
  OpGrayImage* scaledImage;
  float*       scales;  
};

//#ifdef _USE_PERSONAL_NAMESPACES
//}
//#endif

#endif
