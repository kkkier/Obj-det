/*********************************************************************/
/*                                                                   */
/* FILE         scalespacedata.cc                                    */
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

/****************/
/*   Includes   */
/****************/
#include "scalespacedata.hh"

/*===================================================================*/
/*                        Class ScaleSpaceData                       */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

ScaleSpaceData::ScaleSpaceData()
{
  refcount = 1;
  level = 0;
  scaledImage = 0;
  scales = 0;
}


ScaleSpaceData::ScaleSpaceData(const int level) 
{
  this->level = level;
  refcount = 1;
  if (level > 0) {
    scaledImage = new OpGrayImage[level+1];
    scales = new float[level+1];
  } else {
    this->level = 0;
    scaledImage = 0;
    scales = 0;
  }
}


ScaleSpaceData::~ScaleSpaceData()
{
  if (refcount > 1) {
    throw ObjectInUseException();
  }
  if (scaledImage != 0) {
    delete[] scaledImage;
    delete[] scales;
  }
}


ScaleSpaceData* ScaleSpaceData::clone() const 
{
  ScaleSpaceData *tmp = new ScaleSpaceData(level);
  tmp->sourceImage = sourceImage;
  for (int i=0; i<level+1; i++) {
    int widthi = scaledImage[i].width();
    int heighti = scaledImage[i].height();
    tmp->scaledImage[i] = OpGrayImage(widthi, heighti);
    for (int x=0; x < widthi; x++) {
      for (int y=0; y < heighti; y++) {
        tmp->scaledImage[i](x,y) = scaledImage[i](x,y).value();
      }
    }
    tmp->scales[i] = scales[i];
  }
  return tmp;
}
