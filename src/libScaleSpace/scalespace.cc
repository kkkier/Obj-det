/*********************************************************************/
/*                                                                   */
/* FILE         scalespace.cc                                        */
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

/****************/
/*   Includes   */
/****************/
#include <math.h>
#include <iostream>

#include "scalespace.hh"


/*===================================================================*/
/*                          Class ScaleSpace                         */
/*===================================================================*/

/***********************************************************/
/*                       Constructors                      */
/***********************************************************/

ScaleSpace::ScaleSpace(){
  data = 0;
}


ScaleSpace::ScaleSpace(const ScaleSpace& source) 
{
  data = source.data;
  if (source.data != 0) {
    data->refcount++;
  }
}


ScaleSpace::ScaleSpace(const OpGrayImage& source, float sigma0, 
                       float scaleChange, int level) 
{
  cout << "ScaleSpace::ScaleSpace called." << endl;
  data = new ScaleSpaceData(level);
  cout << "  Generating scale space" << endl;
  data->sourceImage = source;
  for (int i = 0; i <= level; i++) {
    data->scaledImage[i] = OpGrayImage(source);
    float sigma = sigma0 * (float) pow((double)scaleChange, i);
    cout << "    Apply gauss on image. level=" << i << " sigma=" << sigma 
         << endl;
    data->scaledImage[i] = data->scaledImage[i].opGauss(sigma);
    data->scales[i] = sigma;
  }
  cout << "ScaleSpace::ScaleSpace exit." << endl;
}


ScaleSpace::~ScaleSpace() {
  if (data != 0) {
    if (data->refcount > 1) {
      data->refcount--;
    } else {
      delete data;
    }
  }
}


/***********************************************************/
/*                   Data Access Methods                   */
/***********************************************************/

float ScaleSpace::getScaleAtLevel(int level) const 
{
  if (level > data->level || level < 0 || data->scales == 0) {
    throw OutOfBoundaryException();
  }
  return data->scales[level];
}


int ScaleSpace::getLevelAtScale(float scale) const 
{
  if (data->scales == 0) {
    throw OutOfBoundaryException();
  }
  for (int i=0; i<=data->level; i++) {
    if (data->scales[i] == scale) {
      return i;
    }
  }
  return -1;
}


float ScaleSpace::getLevel() const 
{
  return data->level;
}


OpGrayImage& ScaleSpace::getImage(int level) 
{
  if (level > data->level || level < 0 || data->scaledImage == 0) {
    cout << "--Image out of level range." << endl;
    throw OutOfBoundaryException();
  }
  return data->scaledImage[level];
}


bool ScaleSpace::isMaximum3D(int x, int y, int z, int dist) const 
{
  if (z > data->level || z < 0 || data->scaledImage == 0) {
    cout << "--Coordinates out of level range." << endl;
		throw OutOfBoundaryException();
  }
  if (x < 0 || x > data->scaledImage[z].width() ||
      y < 0 || y > data->scaledImage[z].height()) {
    cout << "--Coordinates out of image range." << endl;
    throw OutOfBoundaryException();
  }
  float actual = data->scaledImage[z](x,y).value();
  for (int i = (z-dist); i <= (z+dist); i++) {
    if (i >= 0 && i <= data->level) {
      for (int j = (x-dist); j <= (x+dist); j++) {
        for (int k = (y-dist); k <= (y+dist); k++) {
          if (j >= 0 && j < data->scaledImage[i].width() &&
              k >= 0 && k < data->scaledImage[i].height() &&
              !(i == z && j == x && k == y)) {
            float neighbour = data->scaledImage[i](j,k).value();
            if (neighbour >= actual) {
              return false;
            }
          }
        }
      }
    }
  }
  return true;
}


bool ScaleSpace::isMaximum2D(int x, int y, int z, int dist) const 
{
  if (z > data->level || z < 0 || data->scaledImage == 0) {
    cout << "--Coordinates out of level range." << endl;
    throw OutOfBoundaryException();
  }
  if (x < 0 || x > data->scaledImage[z].width() ||
      y < 0 || y > data->scaledImage[z].height()) {
    cout << "--Coordinates out of image range." << endl;
    throw OutOfBoundaryException();
  }
  float actual = data->scaledImage[z](x,y).value();
  for (int j = (x-dist); j <= (x+dist); j++) {
    for (int k = (y-dist); k <= (y+dist); k++) {
      try {
        if (j >= 0 && j < data->scaledImage[z].width() &&
            k >= 0 && k < data->scaledImage[z].height() &&
            !(j == x && k == y)) {
          float neighbour = data->scaledImage[z](j,k).value();
          if (neighbour >= actual) {
            return false;
          }
        }
      } catch (OutOfBoundaryException) {  }	
      // why does this obviously not work here???
    }
  }
  return true;
}


float ScaleSpace::getCharacteristicScale(int x, int y) const 
  /* Returns the characteristic scale of point(x,y). */
{
  if (data->scaledImage == 0 ||
      x < 0 || x > data->scaledImage[0].width() ||
      y < 0 || y > data->scaledImage[0].height()) {
    cout << "--Coordinates out of image range." << endl;
    throw OutOfBoundaryException();
  }
  for (int z=1; z<data->level; z++) {
    float actual = data->scaledImage[z](x,y).value();
    float last = data->scaledImage[z-1](x,y).value();
    float next = data->scaledImage[z+1](x,y).value();
    if (actual > last && actual > next) {
      //if (isMaximum3D(x,y,z)) {
      return data->scales[z];
    }
  }
  return 0.0;
}


float ScaleSpace::getCharacteristicScaleValue(int x, int y) const 
  /* Returns the characteristic scale value of point(x,y). */
{
  if (data->scaledImage == 0 ||
      x < 0 || x > data->scaledImage[0].width() ||
      y < 0 || y > data->scaledImage[0].height()) {
    cout << "--Coordinates out of image range." << endl;
    throw OutOfBoundaryException();
  }
  for (int z=1; z<data->level; z++) {
    float actual = data->scaledImage[z](x,y).value();
    float last = data->scaledImage[z-1](x,y).value();
    float next = data->scaledImage[z+1](x,y).value();
    if (actual > last && actual > next) {
      //if (isMaximum3D(x,y,z)) {
      return data->scaledImage[z](x,y).value();
    }
  }
  return 0.0;
}

		
/***********************************************************/
/*                  Data Access Operators                  */
/***********************************************************/

GrayPixel  ScaleSpace::operator()(const int x, const int y, const int z) const
{
  if (z > data->level || z < 0 || data->scaledImage == 0) {
    throw OutOfBoundaryException();
  }
  if (x < 0 || x > data->scaledImage[z].width() ||
      y < 0 || y > data->scaledImage[z].height()) {
    throw OutOfBoundaryException();
  }
  return data->scaledImage[z](x,y);
}


GrayPixel& ScaleSpace::operator()(const int x, const int y, const int z) 
{
  if (z > data->level || z < 0 || data == 0) {
    throw OutOfBoundaryException();
  }
  if (x < 0 || x > data->scaledImage[z].width() ||
      y < 0 || y > data->scaledImage[z].height()) {
    throw OutOfBoundaryException();
  }
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  return data->scaledImage[z](x,y);
}


ScaleSpace& ScaleSpace::operator=(ScaleSpace other) 
{
  if (data != 0) {
    if (data->refcount > 1) {
      data->refcount--;
    } else {
      delete data;
    }
  }
  data = other.data;
  if (data != 0) {
    data->refcount++;
  }
  return *this;
}


/***********************************************************/
/*                Operators on Scale Levels                */
/***********************************************************/

ScaleSpace& ScaleSpace::opSquareGradient() 
{
  cout << "  ScaleSpace::opSquareGradient called." << endl;
  if (data == 0) {
    throw OutOfBoundaryException();
  }
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  for (int z=0; z <= data->level; z++) {
    cout << "    Calculating Gauss Derivatives." << endl;
    OpGrayImage dx(data->scaledImage[z]);
    OpGrayImage dy(data->scaledImage[z]);
    dx = dx.opGaussDerivGx(data->scales[z]);
    dy = dy.opGaussDerivGy(data->scales[z]);
    for (int i=0; i<data->scaledImage[z].width(); i++) {
      for (int j=0; j<data->scaledImage[z].height(); j++) {
        data->scaledImage[z](i,j) = dx(i,j).value() * dx(i,j).value()
          + dy(i,j).value() * dy(i,j).value();
      }
    }
  }
  cout << "    Normalizing image according to scale." << endl;
  *this = this->opNormalize(2);
  cout << "  ScaleSpace::opLaplacian exit." << endl;
  return *this;
}


ScaleSpace& ScaleSpace::opLaplacian() 
{
  cout << "  ScaleSpace::opLaplacian called." << endl;
  if (data == 0) {
    throw OutOfBoundaryException();
  }
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  for (int i=0; i <= data->level; i++) {
    cout << "    Applying Laplacian on image " << i << "." << endl;
    data->scaledImage[i] = data->scaledImage[i].opGaussLap(data->scales[i]);
    //data->scaledImage[i] = data->sourceImage.opGaussLap(data->scales[i]);
  }
  cout << "    Normalizing image according to scale." << endl;
  *this = this->opNormalize(2);
  for (int z=0; z <= data->level; z++) {
    for (int x=0; x < data->scaledImage[z].width(); x++) {
      for (int y=0; y < data->scaledImage[z].height(); y++) {
        if (data->scaledImage[z](x,y).value() < 0.0) {
          data->scaledImage[z](x,y) = - data->scaledImage[z](x,y).value();
        }
      }
    }
  }
  cout << "  ScaleSpace::opLaplacian exit." << endl;
  return *this;
}


ScaleSpace& ScaleSpace::opDifferenceOfGaussian() 
  /* Apply DOG on scale space. Assumes that the scale space is unmo- */
  /* dified, e.g. consists still of gaussians.                       */
{
  cout << "  ScaleSpace::opDifferenceOfGaussian called." << endl;
  if (data == 0) {
    throw OutOfBoundaryException();
  }
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  for (int z=data->level; z >= 1; z--) {
    for (int x=0; x < data->scaledImage[z].width(); x++) {
      for (int y=0; y < data->scaledImage[z].height(); y++) {
        data->scaledImage[z](x,y) = fabs(data->scaledImage[z-1](x,y).value() -
                                         data->scaledImage[z](x,y).value());
      }
    }
  }
  if (data->level >= 1) {
    for (int x=0; x < data->scaledImage[0].width(); x++) {
      for (int y=0; y < data->scaledImage[0].height(); y++) {
        data->scaledImage[0](x,y) = data->scaledImage[1](x,y);
      }
    }
  }
  cout << "  ScaleSpace::opDifferenceOfGaussian exit." << endl;
  return *this;
}


ScaleSpace& ScaleSpace::opNormalize(int order) 
{
  cout << "  ScaleSpace::opNormalize called. order=" << order << endl;
  if (data == 0) {
    throw OutOfBoundaryException();
  }
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  for (int z=0; z<=data->level; z++) {
    float normfac = pow(data->scales[z], order);
    for (int i=0; i<data->scaledImage[z].width(); i++) {
      for (int j=0; j<data->scaledImage[z].height(); j++) {
        data->scaledImage[z](i,j) = data->scaledImage[z](i,j).value() * 
          normfac;
      }
    }
  }
  cout << "  ScaleSpace::opNormalize exit." << endl;
  return *this;
}


ScaleSpace& ScaleSpace::opNonMaximaSuppression3D(int cubeSize) 
{  
  cout << "  ScaleSpace::opNonMaximaSuppression3D called. cubeSize=" 
       << cubeSize << endl;
  if (data == 0) {
    throw OutOfBoundaryException();
  }
  cubeSize = (int) floor((double)cubeSize);
  ScaleSpace result(*this);
  
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  
  for (int z=0; z<=data->level; z++) {
    for (int x=0; x<data->scaledImage[z].width(); x++) {
      for (int y=0; y<data->scaledImage[z].height(); y++) {
        if (!isMaximum3D(x, y, z, cubeSize)) {
          result.data->scaledImage[z](x,y) = 0.0;
        }
      }
    }
  }
  *this = result;
  return *this;
}


ScaleSpace& ScaleSpace::opNonMaximaSuppression2D(int windowSize) 
{
  cout << "  ScaleSpace::opNonMaximaSuppression2D called. windowSize=" 
       << windowSize << endl;
  if (data == 0) {
    throw OutOfBoundaryException();
  }
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  windowSize = (int) floor((double)windowSize);
  for (int z=0; z<=data->level; z++) {
    data->scaledImage[z] = 
      data->scaledImage[z].opNonMaximumSuppression(windowSize);
  }
  return *this;
}


//-----------------------------------------------------------------------------
// Related Functions
//-----------------------------------------------------------------------------

ScaleSpace buildScaleSpace(OpGrayImage &img, float sigma0, float scaleChange, 
                           int level) 
{
  return ScaleSpace(img, sigma0, scaleChange, level);
}
