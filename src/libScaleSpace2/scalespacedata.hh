/*********************************************************************/
/*                                                                   */
/* FILE         scalespacedata.hh                                    */
/* AUTHORS      Thomas Hug, modified by Bastian Leibe                */
/* EMAIL        thug@student.ethz.ch                                 */
/*                                                                   */
/* CONTENT      Implementation of the per-level data structure for   */
/*              an image scale space.                                */
/*                                                                   */
/* BEGIN        Tue Dec 11 2001                                      */
/* LAST CHANGE  Wed Jan 16 2002                                      */
/*                                                                   */
/*********************************************************************/
/***************************************************************************
    copyright            : (C) 2001 by Thomas Hug and ETH Zurich
    email                : thug@student.ethz.ch
 ***************************************************************************/

#ifndef SCALESPACEDATA_HH
#define SCALESPACEDATA_HH

//#ifdef _USE_PERSONAL_NAMESPACES
//namespace Hug {
//#endif

/****************/
/*   Includes   */
/****************/
#include <opgrayimage.hh>


/****************************/
/*   Forward Declarations   */
/****************************/
class ScaleSpace;
class PyramidScaleSpace;


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                       Class ScaleSpaceData                        */
/*===================================================================*/
/* Define a per-level data structure for image scale spaces. */
class ScaleSpaceData {
public:
  ScaleSpaceData();
  ScaleSpaceData(const int);
  ~ScaleSpaceData();
  
  ScaleSpaceData* clone() const;
  
public:
  class ObjectInUseException{};
  
  friend class ScaleSpace;
  friend class PyramidScaleSpace;

public:
  int refcount;
  int level;

  OpGrayImage  sourceImage;
  OpGrayImage* scaledImage;
  float* scales;  
};

//#ifdef _USE_PERSONAL_NAMESPACES
//}
//#endif

#endif
