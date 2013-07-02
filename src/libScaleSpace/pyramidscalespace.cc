/*********************************************************************/
/*                                                                   */
/* FILE         pyramidscalespace.cc                                 */
/* AUTHORS      Thomas Hug, modified by Bastian Leibe                */
/* EMAIL        thug@student.ethz.ch                                 */
/*                                                                   */
/* CONTENT      Implementation of an image pyramid scale space.      */
/*                                                                   */
/* BEGIN        Wed Jan 16 2002                                      */
/* LAST CHANGE  Fri Jan 25 2002                                      */
/*                                                                   */
/*********************************************************************/
/***************************************************************************
    copyright            : (C) 2002 by Thomas Hug and ETH Zurich
    email                : thug@student.ethz.ch
 ***************************************************************************/

/****************/
/*   Includes   */
/****************/
#include <math.h>
#include <iostream>

#include "pyramidscalespace.hh"


/*===================================================================*/
/*                      Class PyramidScaleSpace                      */
/*===================================================================*/

/***********************************************************/
/*                       Constructors                      */
/***********************************************************/

PyramidScaleSpace::PyramidScaleSpace()
{
  data = 0;
  differences = 0;
  minImageSize = 14;
}


PyramidScaleSpace::PyramidScaleSpace(const PyramidScaleSpace& source) 
{
  data = source.data;
  if (source.data != 0) {
    data->refcount++;
  }
  differences = source.differences;
  if (source.differences != 0) {
    differences->refcount++;
  }
  minImageSize = source.minImageSize;
}


PyramidScaleSpace::PyramidScaleSpace(const OpGrayImage& source, int level,
                                     int minSize ) 
{
  //cout << "PyramidScaleSpace::PyramidScaleSpace called." << endl;
  minImageSize = minSize;
  if (level < 0) {
    data = 0;
    differences = 0;
    return;
  }
  data = new ScaleSpaceData(level);
  differences = new ScaleSpaceData(level);
  //cout << "  Generating pyramid scale space" << endl;
  
  OpGrayImage sourceImg = source;
  sourceImg = sourceImg.opGauss(1.0);
  data->scaledImage[0] = sourceImg;
  //differences->scaledImage[0] = sourceImg;
  data->scales[0] = 1.0;
  differences->scales[0] = 1.0;
  
  OpGrayImage laplaceImg(sourceImg);
  //laplaceImg = laplaceImg.opGaussLap(1.0);
  int laplace[3] = { -1, 2, -1 };
  for (int i=0; i<sourceImg.width(); i++) {
    for (int j=0; j<sourceImg.height(); j++) {
      float sum = 0;
      for (int k=0; k<3; k++) {
        if (i-1+k < 0) {
          sum += sourceImg(i+1, j).value() * laplace[k];
        } else if (i-1+k >= sourceImg.width()) {
          sum += sourceImg(sourceImg.width()-1, j).value() * laplace[k];
        } else {
          sum += sourceImg(i-1+k, j).value() * laplace[k];
        }
      }
      laplaceImg(i,j) = sum;
      sum = 0;
      for (int k=0; k<3; k++) {
        if (j-1+k < 0) {
          sum += sourceImg(i, j+1).value() * laplace[k];
        } else if (j-1+k >= sourceImg.height()) {
          sum += sourceImg(i, sourceImg.height()-1).value() * laplace[k];
        } else {
          sum += sourceImg(i, j-1+k).value() * laplace[k];
        }
      }
      laplaceImg(i,j) = laplaceImg(i,j).value() + sum;
    }
  }
  differences->scaledImage[0] = laplaceImg;
  
  for (int z = 1; z <= data->level; z+=2) {
    if (sourceImg.width() <= minImageSize || 
        sourceImg.height() <= minImageSize) {
      data->level = z - 1;
      differences->level = z - 1;
      break;
    }
    OpGrayImage newLevel1 = sourceImg.opGauss(sqrt(2.0));
    OpGrayImage newLevel2 = sourceImg.opGauss(2);
    OpGrayImage diff1(sourceImg);
    OpGrayImage diff2(newLevel1);
    for (int i=0; i<sourceImg.width(); i++) {
      for (int j=0; j<sourceImg.height(); j++) {
        diff1(i,j) = diff1(i,j).value() - newLevel1(i,j).value();
        diff2(i,j) = diff2(i,j).value() - newLevel2(i,j).value();
      }
    }
    differences->scaledImage[z] = diff1;
    differences->scaledImage[z+1] = diff2;
    newLevel1 = resampleDown15(newLevel1);
    newLevel2 = resampleDown20(newLevel2);
    data->scaledImage[z] = newLevel1;
    data->scaledImage[z+1] = newLevel2;
    sourceImg = newLevel2;
  }
  
  for (int i=1; i<data->level; i++) {
    data->scales[level] = sqrt(2.0);
  }
  //cout << "PyramidScaleSpace::PyramidScaleSpace exit." << endl;
}


PyramidScaleSpace::~PyramidScaleSpace() 
{
  if (data != 0) {
    if (data->refcount > 1) {
      data->refcount--;
    } else {
      delete data;
    }
  }
  if (differences != 0) {
    if (differences->refcount > 1) {
      differences->refcount--;
    } else {
      delete differences;
    }
  }
}


/***********************************************************/
/*                  Data Access Operators                  */
/***********************************************************/

GrayPixel PyramidScaleSpace::operator()(const int x, const int y, 
                                        const int z) const 
{
  if (z > data->level || z < 0 || data->scaledImage == 0) {
    cout << "Coordinates out of image boundaries (" <<
      x << "," << y << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }
  int xs, ys;
  if (z % 2 == 0) {
    xs = (int) floor( (float) x / pow(2.0,(double)(z/2)));
    ys = (int) floor( (float) y / pow(2.0,(double)(z/2)));
  } else {
    xs = (int) floor( (float) x / pow(2.0,(double)((z-1)/2)) / 1.5);
    ys = (int) floor( (float) y / pow(2.0,(double)((z-1)/2)) / 1.5);
  }
  if (xs >= data->scaledImage[z].width())
    xs--;
  if (ys >= data->scaledImage[z].height())
    ys--;
  if (xs < 0 || xs >= data->scaledImage[z].width() ||
      ys < 0 || ys >= data->scaledImage[z].height()) {
    cout << "Coordinates out of image boundaries (" <<
      x << "," << y << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }
  return data->scaledImage[z](xs,ys);
}


GrayPixel& PyramidScaleSpace::operator()(const int x, const int y, 
                                         const int z) 
{
  if (z > data->level || z < 0 || data == 0) {
    cout << "Coordinates out of image boundaries (" <<
      x << "," << y << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  int xs = x, ys = y;
  if (z % 2 == 0) {
    xs = (int) floor( (float) x / pow(2.0,(double)(z/2)));
    ys = (int) floor( (float) y / pow(2.0,(double)(z/2)));
  } else {
    xs = (int) floor( floor((float) x / pow(2.0,(double)((z-1)/2))) / 1.5);
    ys = (int) floor( floor((float) y / pow(2.0,(double)((z-1)/2))) / 1.5);
  }
  if (xs >= data->scaledImage[z].width())
    xs--;
  if (ys >= data->scaledImage[z].height())
    ys--;
  if (xs < 0 || xs >= data->scaledImage[z].width() ||
      ys < 0 || ys >= data->scaledImage[z].height()) {
    cout << "Coordinates out of image boundaries (" <<
      x << "," << y << "," << z << ") ";
    cout << "-> (" << xs << "," << ys << "," << z << ") / (" <<
      data->scaledImage[z].width() << "," << data->scaledImage[z].height() 
         << ")" << endl;
    throw OutOfBoundaryException();
  }
  return data->scaledImage[z](xs,ys);
}


PyramidScaleSpace& PyramidScaleSpace::operator=(PyramidScaleSpace other) 
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
  if (differences != 0) {
    if (differences->refcount > 1) {
      differences->refcount--;
    } else {
      delete differences;
    }
  }
  differences = other.differences;
  if (differences != 0) {
    differences->refcount++;
  }
  minImageSize = other.minImageSize;
  return *this;
}


/***********************************************************/
/*                Operators on Scale Levels                */
/***********************************************************/

PyramidScaleSpace& PyramidScaleSpace::opSquareGradient() 
{
  //cout << "  PyramidScaleSpace::opSquareGradient called." << endl;
  if (data == 0) {
    throw OutOfBoundaryException();
  }
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  for (int z=0; z <= data->level; z++) {
    //cout << "    Calculating Gauss Derivatives." << endl;
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
  //cout << "    Normalizing image according to scale." << endl;
  *this = this->opNormalize(2);
  //cout << "  PyramidScaleSpace::opLaplacian exit." << endl;
  return *this;
}


PyramidScaleSpace& PyramidScaleSpace::opLaplacian() 
{
  //cout << "  PyramidScaleSpace::opLaplacian called." << endl;
  if (data == 0) {
    throw OutOfBoundaryException();
  }
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  for (int i=0; i <= data->level; i++) {
    //cout << "    Applying Laplacian on image " << i << "." << endl;
    data->scaledImage[i] = data->scaledImage[i].opGaussLap(data->scales[i]);
  }
  //cout << "    Normalizing image according to scale." << endl;
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
  //cout << "  PyramidScaleSpace::opLaplacian exit." << endl;
  return *this;
}


PyramidScaleSpace& PyramidScaleSpace::opDifferenceOfGaussian() 
  /* Apply DOG on scale space. Assumes that the scale space is unmo- */
  /* dified, e.g. consists still of gaussians.                       */
{
  //cout << "  PyramidScaleSpace::opDifferenceOfGaussian called." << endl;
  if (data == 0 || differences == 0) {
    throw OutOfBoundaryException();
  }
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  if (differences->refcount > 1) {
    ScaleSpaceData *tmp = differences->clone();
    differences->refcount--;
    differences = tmp;
  }
  
  for (int z=data->level; z >= 0; z--) {
    data->scaledImage[z] = differences->scaledImage[z];
    for (int x=0; x < data->scaledImage[z].width(); x++) {
      for (int y=0; y < data->scaledImage[z].height(); y++) {
        data->scaledImage[z](x,y) = fabs(data->scaledImage[z](x,y).value());
      }
    }
    if (z % 2 == 0 && z !=0) {
      data->scaledImage[z] = resampleDown20(data->scaledImage[z]);
    } else if (z % 2 ==1) {
      data->scaledImage[z] = resampleDown15(data->scaledImage[z]);
    }
  }
  //cout << "  PyramidScaleSpace::opDifferenceOfGaussian exit." << endl;
  return *this;
}


PyramidScaleSpace& PyramidScaleSpace::opNormalize(int order) 
{
  //cout << "  PyramidScaleSpace::opNormalize called. order=" << order << endl;
  if (data == 0) {
    throw OutOfBoundaryException();
  }
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  for (int z=1; z<=data->level; z++) {
    float normfac = pow(pow(sqrt(2.0),(double)z), order);
    for (int i=0; i<data->scaledImage[z].width(); i++) {
      for (int j=0; j<data->scaledImage[z].height(); j++) {
        data->scaledImage[z](i,j) = ( data->scaledImage[z](i,j).value() * 
                                      normfac );
      }
    }
  }
  //cout << "  PyramidScaleSpace::opNormalize exit." << endl;
  return *this;
}


PyramidScaleSpace& PyramidScaleSpace::opNonMaximaSuppression3D(int cubeSize) 
{
  //cout << "  PyramidScaleSpace::opNonMaximaSuppression3D called. cubeSize=" 
  //     << cubeSize << endl;
  if (data == 0) {
    throw OutOfBoundaryException();
  }
  cubeSize = (int) floor((double)cubeSize);
  PyramidScaleSpace result(*this);
  
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  
  for (int z=0; z<=data->level; z++) {
    for (int x=0; x<data->scaledImage[z].width(); x++) {
      for (int y=0; y<data->scaledImage[z].height(); y++) {
        if (!isMaximum3D(x, y, z, cubeSize)) {
          result(x,y,z) = GrayPixel(0.0);
        }
      }
    }
  }
  *this = result;
  return *this;
}


PyramidScaleSpace& PyramidScaleSpace::opNonMaximaSuppression2D(int windowSize) 
{
  //cout << "  PyramidScaleSpace::opNonMaximaSuppression2D called. windowSize=" 
  //     << windowSize << endl;
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
  //cout << "  PyramidScaleSpace::opNonMaximaSuppression2D exit." << endl;
  return *this;
}


PyramidScaleSpace& PyramidScaleSpace::opSuppressCornerEffects() {
  //cout << "  PyramidScaleSpace::opSuppressCornerEffects called." << endl;
  if (data == 0) {
    throw OutOfBoundaryException();
  }
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  /* vectors for every corner point. direction to corner. */
  int neighbour[4][2] = { { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 } };
  int cornerpos = 1, cornerwidth = 3;
  for (int z=0; z<=data->level; z++) {
    int pos[4][2] = {
      { cornerpos, cornerpos },
      { cornerpos, data->scaledImage[z].height() - cornerpos - cornerwidth },
      { data->scaledImage[z].width()  - cornerpos - cornerwidth, cornerpos },
      { data->scaledImage[z].width()  - cornerpos - cornerwidth,
        data->scaledImage[z].height() - cornerpos - cornerwidth} };
    // for ever corner do...
    for (int i=0; i<4; i++) {
      // look in corner rectangle with edge length cornerwidth
      for (int x=0; x<cornerwidth; x++) {
        for (int y=0; y<cornerwidth; y++) {
          // if maximum found, set neighbour to same value, 
          // so non-max suppression takes
          // care of it...
          if (isMaximum(data->scaledImage[z], pos[i][0]+x, pos[i][1]+y)) {
            data->scaledImage[z](pos[i][0] + neighbour[i][0] + x, 
                                 pos[i][1] + neighbour[i][1] + y) =
              data->scaledImage[z](pos[i][0] + x, pos[i][1] + y).value();
          }
        }
      }
    }
  }
  //cout << "  PyramidScaleSpace::opSuppressCornerEffects exit." << endl;
  return *this;
}


/***********************************************************/
/*                   Data Access Methods                   */
/***********************************************************/

float PyramidScaleSpace::getScaleAtLevel(int level) const 
{
  if (level > data->level || level < 0 || data->scales == 0) {
    throw OutOfBoundaryException();
  }
  return data->scales[level];
}


int PyramidScaleSpace::getLevelAtScale(float scale) const 
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


float PyramidScaleSpace::getLevel() const 
{
  return data->level;
}


OpGrayImage& PyramidScaleSpace::getImage(int level) 
{
  if (level > data->level || level < 0 || data->scaledImage == 0) {
    cout << "--Image out of level range." << endl;
    throw OutOfBoundaryException();
  }
  return data->scaledImage[level];
}


bool PyramidScaleSpace::isMaximum3D(int x, int y, int z, int dist) const 
{
  if (z > data->level || z < 0 || data == 0) {
    cout << "--Coordinates out of level range." << endl;
    throw OutOfBoundaryException();
  }
  if (x < 0 || x > data->scaledImage[0].width() ||
      y < 0 || y > data->scaledImage[0].height()) {
    cout << "--Coordinates out of image range." << endl;
    throw OutOfBoundaryException();
  }
  
  int xc = x, yc = y;
  convertToPyramid(xc,yc,z);
  float actual = data->scaledImage[z](xc,yc).value();
  for (int i = (z-dist); i <= (z+dist); i++) {
    if (i >= 0 && i <= data->level) {
      xc = x, yc = y;
      convertToPyramid(xc, yc, i);
      for (int j = (xc-dist); j <= (xc+dist); j++) {
        for (int k = (yc-dist); k <= (yc+dist); k++) {
          if (!(i == z && j == x && k == y)) {
            try {
              float neighbour = data->scaledImage[z](xc,yc).value();
              if (neighbour >= actual) {
                return false;
              }
            } catch (OutOfBoundaryException) { }
          }
        }
      }
    }
  }
  return true;
}


bool PyramidScaleSpace::isMaximum2D(int x, int y, int z, int dist) const 
{
  if (z > data->level || z < 0 || data->scaledImage == 0) {
    cout << "--Coordinates out of level range." << endl;
    throw OutOfBoundaryException();
  }
  if (x < 0 || x > data->scaledImage[0].width() ||
      y < 0 || y > data->scaledImage[0].height()) {
    cout << "--Coordinates out of image range." << endl;
    throw OutOfBoundaryException();
  }
  int xc = x, yc = y;
  convertToPyramid(xc,yc,z);
  float actual = data->scaledImage[z](x,y).value();
  for (int j = (xc-dist); j <= (xc+dist); j++) {
    for (int k = (yc-dist); k <= (yc+dist); k++) {
      if (!(j == xc && k == yc)) {
        try {
          float neighbour = data->scaledImage[z](xc,yc).value();
          if (neighbour >= actual) {
            return false;
          }
        } catch (OutOfBoundaryException) { }
      }
    }
  }
  return true;
}


float PyramidScaleSpace::getCharacteristicScale(int x, int y) const 
  /* Returns the characteristic scale of point(x,y). */
{
  if (data->scaledImage == 0 ||
      x < 0 || x > data->scaledImage[0].width() ||
      y < 0 || y > data->scaledImage[0].height()) {
    cout << "--Coordinates out of image range." << endl;
    throw OutOfBoundaryException();
  }
  PyramidScaleSpace tmp(*this);
  for (int z=1; z<data->level; z++) {
    float actual = tmp(x,y,z).value();
    float last = tmp(x,y,z-1).value();
    float next = tmp(x,y,z+1).value();
    if (actual > last && actual > next) {
      return data->scales[z];
    }
  }
  return 0.0;
}


float PyramidScaleSpace::getCharacteristicScaleValue(int x, int y) const 
  /* Returns the characteristic scale value of point(x,y). */
{
  if (data->scaledImage == 0 ||
      x < 0 || x > data->scaledImage[0].width() ||
      y < 0 || y > data->scaledImage[0].height()) {
    cout << "--Coordinates out of image range." << endl;
    throw OutOfBoundaryException();
  }
  PyramidScaleSpace tmp(*this);
  for (int z=1; z<data->level; z++) {
    float actual = tmp(x,y,z).value();
    float last = tmp(x,y,z-1).value();
    float next = tmp(x,y,z+1).value();
    if (actual > last && actual > next) {
      return tmp(x,y,z).value();
    }
  }
  return 0.0;
}


void PyramidScaleSpace::convertToPyramid(int &x, int &y, int z) const 
{
  if (z > data->level || z < 0 || data == 0) {
    cout << "Coordinates out of scale boundaries (" <<
      x << "," << y << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }
  if (x < 0 || x > data->scaledImage[0].width() ||
      y < 0 || y > data->scaledImage[0].height()) {
    cout << "Coordinates out of kartesian image boundaries (" <<
      x << "," << y << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }
  if (z % 2 == 0) {
    x = (int) floor( (float) x / pow(2.0,(double)(z/2)));
    y = (int) floor( (float) y / pow(2.0,(double)(z/2)));
  } else {
    x = (int) floor( floor( (float) x / pow(2.0,(double)((z-1)/2))) / 1.5);
    y = (int) floor( floor( (float) y / pow(2.0,(double)((z-1)/2))) / 1.5);
  }
  if (x >= data->scaledImage[z].width())
    x--;
  if (y >= data->scaledImage[z].height())
    y--;
  if (x < 0 || x >= data->scaledImage[z].width() ||
			y < 0 || y >= data->scaledImage[z].height()) {
    cout << "Coordinates out of pyramid image boundaries (" <<
      x << "," << y << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }
}


void PyramidScaleSpace::convertToKartesian(int &x, int &y, int z) const 
{
  if (z > data->level || z < 0 || data == 0) {
    cout << "Coordinates out of scale boundaries (" <<
      x << "," << y << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }
  if (x < 0 || x > data->scaledImage[z].width() ||
      y < 0 || y > data->scaledImage[z].height()) {
    cout << "Coordinates out of pyramid image boundaries (" <<
      x << "," << y << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }
  int middle = 0;
  if (z % 2 == 0) {
    middle = (int) pow(2.0,(double)(z/2)) / 2;
    x = (int) floor( (float) x * pow(2.0,(double)(z/2)) + middle);
    y = (int) floor( (float) y * pow(2.0,(double)(z/2)) + middle);
  } else {
    middle = (int) floor(pow(2.0,(double)((z-1)/2)) / 2 * 1.5);
    x = (int) floor( (float) x * pow(2.0,(double)((z-1)/2)) * 1.5 + middle);
    y = (int) floor( (float) y * pow(2.0,(double)((z-1)/2)) * 1.5 + middle);
  }
  // Possible case of overflow: ceiling of pixel-size for new level 
  //    dimensions in resampling by factor 2.
  //    (Ex: old dim level 0: (5,5), new dim level 2: (3,3) middle=1.
  //    Pt(2,2,2) -> (5,5,0) overflow!)
  if (x >= data->scaledImage[0].width()) {
    x = x - middle + (int) floor( (double)(data->scaledImage[0].width() - x) 
                                  / 2);
  }
  if (y >= data->scaledImage[0].height()) {
    y = y - middle + (int) floor( (double)(data->scaledImage[0].height() - y) 
                                  / 2);
  }
  // Should be ok after test for overflow. Testing for debugging.
  if (x < 0 || x >= data->scaledImage[0].width() ||
      y < 0 || y >= data->scaledImage[0].height()) {
    cout << "Coordinates out of kartesian image boundaries (" <<
      x << "," << y << "," << z << ")" << endl;
    throw OutOfBoundaryException();
  }
}

OpGrayImage PyramidScaleSpace::resampleUp(OpGrayImage& source) const 
  /* Resampling schema...   */
  /* ___________________    */
  /* |  | |  | |  | |  |    */
  /* |  | |  | |  | |  |    */
  /* -------------------    */
  /* |  | |  | |  | |  |    */
  /* -------------------    */
  /* |  | |  | |  | |  |    */
  /* |  | |  | |  | |  |    */
  /* -------------------    */
  /* ...                    */
  /*                        */
{
  //cout << "PyramidScaleSpace::resampleUp called." << endl;
  int imageWidth =  (int) ceil( (double)source.width()  * 1.5 );
  int imageHeight = (int) ceil( (double)source.height() * 1.5 );
  
  OpGrayImage result(imageWidth, imageHeight);
  OpGrayImage copy(source);
  
  // average 4 neighbours
  //cout << "--AV4." << endl;
  for (int i=0; i<source.width(); i+=2) {
    for (int j=0; j<source.height(); j+=2) {
      float sum1 = source(i,j).value(), sum2 = source(i,j).value();
      float sum3 = source(i,j).value(), sum4 = source(i,j).value();
      int valid1 = 1, valid2 = 1, valid3 = 1, valid4 = 1;
      if (i - 1 >= 0) {
        sum1 += source(i-1, j).value();
        sum3 += source(i-1, j).value();
        valid1++;
        valid3++;
        if (j - 1 >= 0) {
          sum1 += source(i-1, j-1).value();
          valid1++;
        }
        if (j + 1 < source.height()) {
          sum3 += source(i-1, j+1).value();
          valid3++;
        }
      }
      if (i + 1 < source.width()) {
        sum2 += source(i+1, j).value();
        sum4 += source(i+1, j).value();
        valid2++;
        valid4++;
        if (j - 1 >= 0) {
          sum2 += source(i+1, j-1).value();
          valid2++;
        }
        if (j + 1 < source.height()) {
          sum4 += source(i+1, j+1).value();
          valid4++;
        }
      }
      if (j - 1 >= 0) {
        sum1 += source(i, j-1).value();
        sum2 += source(i, j-1).value();
        valid1++;
        valid2++;
      }
      if (j + 1 < source.height()) {
        sum3 += source(i, j+1).value();
        sum4 += source(i, j+1).value();
        valid3++;
        valid4++;
      }
      sum1 /= (float)valid1; sum2 /= (float)valid2;
      sum3 /= (float)valid3; sum4 /= (float)valid4;
      result( (int)(i*1.5), (int)(j*1.5)) = sum1;
      result( (int)(i*1.5) +1, (int)(j*1.5)) = sum2;
      result( (int)(i*1.5), (int)(j*1.5) +1) = sum3;
      result( (int)(i*1.5) +1, (int)(j*1.5) +1) = sum4;
    }
  } //*/
  // average 2 neighbours vertical
  // cout << "--AV2|." << endl;
  for (int i=1; i<source.width(); i+=2) {
    for (int j=0; j<source.height(); j+=2) {
      float sum1 = source(i,j).value(), sum2 = source(i,j).value();
      int valid1 = 1, valid2 = 1;
      if (j - 1 > 0) {
        sum1 += source(i, j-1).value();
        valid1++;
      }
      if (j + 1 < source.height()) {
        sum2 += source(i, j+1).value();
        valid2++;
      }
      result( (int)ceil(i*1.5), (int)(j*1.5) ) = sum1 / (float)valid1;
      result( (int)ceil(i*1.5), (int)(j*1.5) + 1) = sum2 / (float)valid2;
    }
  }
  // average 2 neighbours horizontal
  //cout << "--AV2-." << endl;
  for (int i=0; i<source.width(); i+=2) {
    for (int j=1; j<source.height(); j+=2) {
      float sum1 = source(i,j).value(), sum2 = source(i,j).value();
      int valid1 = 1, valid2 = 1;
      if (i - 1 > 0) {
        sum1 += source(i - 1, j).value();
        valid1++;
      }
      if (i + 1 < source.height()) {
        sum2 += source(i + 1, j).value();
        valid2++;
      }
      //cout << "----pos(" << ceil(i*1.5) << "," << ceil(j*1.5) << ")" << endl;
      result( (int)(i*1.5), (int)ceil(j*1.5) ) = sum1 / (float)valid1;
      result( (int)(i*1.5) + 1, (int)ceil(j*1.5)) = sum2 / (float)valid2;
    }
  }
  // add remaining
  //cout << "--AV1." << endl;
  for (int i=1; i<source.width(); i+=2) {
    for (int j=1; j<source.height(); j+=2) {
      //cout << "----pos(" << ceil(i*1.5) << "," << ceil(j*1.5) << ")" << endl;
      result( (int)ceil(i*1.5), (int)ceil(j*1.5)) = source(i,j).value();
    }
  }
  
  //cout << "PyramidScaleSpace::resampleUp exit." << endl;
  return result;
}


OpGrayImage PyramidScaleSpace::resampleDown15(OpGrayImage& source) const 
{
  //cout << "PyramidScaleSpace::resampleDown15 called." << endl;
  int imageWidth = (int)  floor((double)source.width() / 1.5);
  int imageHeight = (int) floor((double)source.height() / 1.5);
  
  OpGrayImage result(imageWidth, imageHeight);
  OpGrayImage copy(source);
  
  int posX[2][4] = { { 0, 0, 1, 1 }, { 1, 1, 0, 0 } };
  int posY[2][4] = { { 0, 1, 0, 1 }, { 1, 0, 1, 0 } };
  
  short caseX = 1, caseY = 1;
  /*if ((float)source.width() / 1.5 - floor(source.width() / 1.5) < 0.5) {
    caseX = 1;
    }
    if ((float)source.height() / 1.5 - floor(source.height() / 1.5) < 0.5) {
    caseY = 1;
    }*/
  
  // average 4 neighbours
  for (int i=posX[caseX][3]; i<imageWidth; i+=2) {
    for (int j=posY[caseY][3]; j<imageHeight; j+=2) {
      float sum = source((int)(1.5*(i-posX[caseX][3])),
                         (int)(1.5*(j-posY[caseY][3]))).value();
      sum += source((int)(1.5*(i-posX[caseX][3]))+1,
                    (int)(1.5*(j-posY[caseY][3]))).value();
      sum += source((int)(1.5*(i-posX[caseX][3])),
                    (int)(1.5*(j-posY[caseY][3]))+1).value();
      sum += source((int)(1.5*(i-posX[caseX][3]))+1,
                    (int)(1.5*(j-posY[caseY][3]))+1).value();
      result(i,j) = sum / 4.0;
    }
  }
  // average 2 neighbours vertical
  for (int i=posX[caseX][1]; i<imageWidth; i+=2) {
    for (int j=posY[caseY][1]; j<imageHeight; j+=2) {
      float sum = source((int)(1.5*(i-posX[caseX][1]))+2,
                         (int)(1.5*(j-posY[caseY][1]))).value();
      sum += source((int)(1.5*(i-posX[caseX][1]))+2,
                    (int)(1.5*(j-posY[caseY][1]))+1).value();
      result(i,j) = sum / 2.0;
    }
  }
  // average 2 neighbours horizontal
  for (int i=posX[caseX][2]; i<imageWidth; i+=2) {
    for (int j=posY[caseY][2]; j<imageHeight; j+=2) {
      float sum = source((int)(1.5*(i-posX[caseX][2])),
                         (int)(1.5*(j-posY[caseY][2]))+2).value();
      sum += source((int)(1.5*(i-posX[caseX][2]))+1,
                    (int)(1.5*(j-posY[caseY][2]))+2).value();
      result(i,j) = sum / 2.0;
    }
  }
  // add remaining
  for (int i=posX[caseX][0]; i<imageWidth; i+=2) {
    for (int j=posY[caseY][0]; j<imageHeight; j+=2) {
      result(i,j) = source((int)(1.5*(i-posX[caseX][0]))+2,
                           (int)(1.5*(j-posY[caseY][0]))+2).value();
    }
  }
  
  //cout << "PyramidScaleSpace::resampleDown15 exit." << endl;
  return result;
}


OpGrayImage PyramidScaleSpace::resampleDown20(OpGrayImage& source) const 
{
  //cout << "PyramidScaleSpace::resampleDown20 called." << endl;
  int imageWidth  = (int)ceil((double)source.width()/2);
  int imageHeight = (int)ceil((double)source.height()/2);
  
  OpGrayImage result(imageWidth, imageHeight);
  for (int i=0; i<imageWidth; i++) {
    for (int j=0; j<imageHeight; j++) {
      result(i,j) = source(2*i,2*j);
    }
  }
  
  //cout << "PyramidScaleSpace::resampleDown20 exit." << endl;
  return result;
}


