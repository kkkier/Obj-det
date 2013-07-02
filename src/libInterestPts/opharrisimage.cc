/*********************************************************************/
/*                                                                   */
/* FILE         opharrisimage.cc                                     */
/* AUTHORS      Thomas Hug                                           */
/* EMAIL        thug@student.ethz.ch                                 */
/*                                                                   */
/* CONTENT      Implementation of the Harris Interest Point          */
/*              Detector.                                            */
/*              First, compute two images with derivatives Dx        */
/*              and Dy (param sigma1). From these two images,        */
/*              build three images as (Dx)^2, (Dy)^2 and DxDy.       */
/*              Weight those images with Gauss (param sigma2).       */
/*              Build matrix C and calculate Det(C)-alpha*trace(C)^2.*/
/*              Select only Pixels above threshold as interest       */
/*              points. (=0.01*max_intensity)                        */
/*                                                                   */
/* BEGIN        Mon Nov 05 2001                                      */
/* LAST CHANGE  Tue Jan 08 2001                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <math.h>

#include <scalespace.hh>
#include <pyramidscalespace.hh>
#include "opinterestimage.hh"
#include "opharrisimage.hh"


/*===================================================================*/
/*                        Class OpHarrisImage                        */
/*===================================================================*/

/***********************************************************/
/*                     Harris Operators                    */
/***********************************************************/

OpGrayImage OpHarrisImage::opImprovedHarris(double sigma1, double sigma2, 
                                            double alpha)
{
  float matrix[] = {0, 0, 0, 0};
  
  if (gDEBUG) {
    cout << "Calling OpHarrisImage::opImprovedHarris() with sigma1=" 
         << sigma1 << " sigma2="
         << sigma2 << " alpha=" << alpha << endl;
    cout << "  Creating images" << endl;
  }
  OpGrayImage img_dxdx(*this);
  OpGrayImage img_dydy(*this);
  OpGrayImage img_dxdy(this->width(), this->height());
  OpGrayImage result(this->width(), this->height());
  
  /** Calculate the intensity variations of the original image and
      weight them with a gauss filter (@param sigma2)  */
  calcIntensityVariations(img_dxdx, img_dydy, img_dxdy, sigma1);
  img_dxdx = img_dxdx.opGauss(sigma2);
  img_dydy = img_dydy.opGauss(sigma2);
  img_dxdy = img_dxdy.opGauss(sigma2);
  
  /** Build a matrix C out of the intensity variation images and
      compute the resulting harris image with the formula
      img(x,y) = det(C) - alpha * trace(C)^2  */
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
      matrix[0] = img_dxdx(i,j).value();
      matrix[1] = img_dxdy(i,j).value();
      matrix[2] = matrix[1];
      matrix[3] = img_dydy(i,j).value();
      float trace_val = trace(matrix);
      result(i,j) = det(matrix) - alpha * (trace_val * trace_val);
    }
  }
  if (gDEBUG) { cout << "Exiting OpHarrisImage::opImprovedHarris." << endl; }
  return result;
}


OpGrayImage OpHarrisImage::opNormalizedHarris(double sigma1, 
                                              double sigma2, double alpha)
{
  float matrix[] = {0, 0, 0, 0};
  
  if (gDEBUG) {
    cout << "Calling OpHarrisImage::opNormalizedHarris() with sigma1=" 
         << sigma1 << " sigma2="
         << sigma2 << " alpha=" << alpha << endl;
    cout << "  Creating images" << endl;
  }
  OpGrayImage img_dxdx(*this);
  OpGrayImage img_dydy(*this);
  OpGrayImage img_dxdy(this->width(), this->height());
  OpGrayImage result(this->width(), this->height());
  
  /** Calculate the intensity variations of the original image and
      weight them with a gauss filter (@param sigma2)  */
  calcIntensityVariations(img_dxdx, img_dydy, img_dxdy, sigma1);
  img_dxdx = img_dxdx.opGauss(sigma2);
  img_dydy = img_dydy.opGauss(sigma2);
  img_dxdy = img_dxdy.opGauss(sigma2);
  
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
      img_dxdx(i,j) = img_dxdx(i,j).value() * sigma1 * sigma1;
      img_dydy(i,j) = img_dydy(i,j).value() * sigma1 * sigma1;
      img_dxdy(i,j) = img_dxdy(i,j).value() * sigma1 * sigma1;
    }
  }
  
  /** Build a matrix C out of the intensity variation images and
      compute the resulting harris image with the formula
      img(x,y) = det(C) - alpha * trace(C)^2  */
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
      matrix[0] = img_dxdx(i,j).value();
      matrix[1] = img_dxdy(i,j).value();
      matrix[2] = matrix[1];
      matrix[3] = img_dydy(i,j).value();
      float trace_val = trace(matrix);
      result(i,j) = det(matrix) - alpha * (trace_val * trace_val);
    }
  }
  if (gDEBUG) { cout << "Exiting OpHarrisImage::opNormalizedHarris." 
                     << endl; }
  return result;
}


OpGrayImage OpHarrisImage::opPyramidHarris(int level, double sigma1, 
                                           double sigma2, double alpha)
  /* Using Harris in a Scale Space Pyramid.                          */
  /* The normalization factor has to be adapted to the level of the  */
  /* pyramid the detector is applied to.                             */
  /*   Calculation: normfactor = sigma1 ^ level                      */
{
  float matrix[] = {0, 0, 0, 0};
	
  if (gDEBUG) {
    cout << "Calling OpHarrisImage::opPyramidHarris() with sigma1=" 
         << sigma1 << " sigma2="
         << sigma2 << " alpha=" << alpha << " level=" << level <<endl;
    cout << "  Creating images" << endl;
  }
  OpGrayImage img_dxdx(*this);
  OpGrayImage img_dydy(*this);
  OpGrayImage img_dxdy(this->width(), this->height());
  OpGrayImage result(this->width(), this->height());
  
  /** Calculate the intensity variations of the original image and
      weight them with a gauss filter (@param sigma2)  */
  calcIntensityVariations(img_dxdx, img_dydy, img_dxdy, sigma1);
  img_dxdx = img_dxdx.opGauss(sigma2);
  img_dydy = img_dydy.opGauss(sigma2);
  img_dxdy = img_dxdy.opGauss(sigma2);
  
  float norm = pow((double)sigma1, (double)level);
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
      img_dxdx(i,j) = img_dxdx(i,j).value() * norm * norm;
      img_dydy(i,j) = img_dydy(i,j).value() * norm * norm;
      img_dxdy(i,j) = img_dxdy(i,j).value() * sigma1 * sigma1;
    }
  }
  
  /** Build a matrix C out of the intensity variation images and
      compute the resulting harris image with the formula
      img(x,y) = det(C) - alpha * trace(C)^2  */
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
      matrix[0] = img_dxdx(i,j).value();
      matrix[1] = img_dxdy(i,j).value();
      matrix[2] = matrix[1];
      matrix[3] = img_dydy(i,j).value();
      float trace_val = trace(matrix);
      result(i,j) = det(matrix) - alpha * (trace_val * trace_val);
    }
  }
  if (gDEBUG) { cout << "Exiting OpHarrisImage::opPyramidHarris." 
                     << endl; }
  return result;
}


OpGrayImage OpHarrisImage::opWindowHarris(double sigma, double alpha, 
                                          int windowSize)
{
  float matrix[] = {0, 0, 0, 0};
  
  if (gDEBUG) {
    cout << "Calling OpHarrisImage::opWindowHarris() with sigma=" 
         << sigma << " alpha="
         << alpha << " window size=" << windowSize << endl;
    cout << "  Creating images" << endl;
  }
  OpGrayImage img_dxdx(*this);
  OpGrayImage img_dydy(*this);
  OpGrayImage img_dxdy(this->width(), this->height());
  OpGrayImage result(this->width(), this->height());
  
  /** Calculate the intensity variations of the original image.
      Afterwards, bild the matrix C by simply summing up the intensities
      around a pixel in a specific window size (@param window_size)
      Calculate the resulting harris image with the formula
      image(x,y) = det(C) - alpha * trace(C)^2  */
  calcIntensityVariations(img_dxdx, img_dydy, img_dxdy, sigma);
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
      matrix[0] = sumWindow(img_dxdx, i, j, windowSize);
      matrix[1] = sumWindow(img_dxdy, i, j, windowSize);
      matrix[2] = matrix[1];
      matrix[3] = sumWindow(img_dydy, i, j, windowSize);
      result(i,j) = det(matrix) - alpha * (trace(matrix) * trace(matrix));
    }
  }
  if (gDEBUG) { cout << "Exiting OpHarrisImage::opWindowHarris." << endl; }
  return result;
}



/***********************************************************/
/*                Private Service Functions                */
/***********************************************************/

void OpHarrisImage::calcIntensityVariations(OpGrayImage& img_dxdx, 
                                            OpGrayImage& img_dydy,
                                            OpGrayImage& img_dxdy, 
                                            double sigma) 
{
  if (gDEBUG) { cout << "  Computing GaussDeriv (sigma)" << endl; }
  img_dxdx = img_dxdx.opGaussDerivGx(sigma);
  img_dydy = img_dydy.opGaussDerivGy(sigma);
  
  if (gDEBUG) { cout << "  Computing Matrix-Elements" << endl; }
  for (int i = 0; i < this->width(); i++) {
    for (int j = 0; j < this->height(); j++) {
      img_dxdy(i,j) = img_dxdx(i,j).value() * img_dydy(i,j).value();
      img_dxdx(i,j) = img_dxdx(i,j).value() * img_dxdx(i,j).value();
      img_dydy(i,j) = img_dydy(i,j).value() * img_dydy(i,j).value();
    }
  }
}


float OpHarrisImage::sumWindow(OpGrayImage& img, int x, int y, 
                               int windowSize) 
{
  int start_x = x - (int) floor(windowSize / 2.0);
  int start_y = y - (int) floor(windowSize / 2.0);
  float sum = 0;
  for (int i = start_x; i < start_x + windowSize; i++) {
    for (int j = start_y; j < start_y + windowSize; j++) {
      try {
        sum += img(i,j).value();
      } catch (OutOfBoundaryException) {}
    }
  }
  return sum;
}


inline float OpHarrisImage::det(float *matrix) {
  return (matrix[0] * matrix[3] - matrix[1] * matrix[2]);
}


inline float OpHarrisImage::trace(float *matrix) {
  return (matrix[0] + matrix[3]);
}


//-----------------------------------------------------------------------------
// Related Functions
//-----------------------------------------------------------------------------
void applyHarrisOnScaleSpace( ScaleSpace& sspace, const float sigma2, 
                              const float alpha, 
                              const short type )
  /* Apply the Harris detector to a regular scale space. */
{
  cout << "  opScaleSpaceHarris called on regular ScaleSpace." << endl;
  if (sspace.data == 0) {
    throw GrayImage::OutOfBoundaryException();
  }
  if (sspace.data->refcount > 1) {
    ScaleSpaceData *tmp = sspace.data->clone();
    sspace.data->refcount--;
    sspace.data = tmp;
  }
  for (int i=0; i <= sspace.data->level; i++) {
    cout << "    Applying Harris on image " << i << "." << endl;
    OpHarrisImage tmp(sspace.data->scaledImage[i]);
    if (type == kImprovedHarris) {
      sspace.data->scaledImage[i] = tmp.opImprovedHarris(sspace.data->scales[i],
                                                  sigma2, alpha);
    } else if (type == kWindowHarris) {
      sspace.data->scaledImage[i] = tmp.opWindowHarris(sspace.data->scales[i],
                                                       alpha);
    } else {
      sspace.data->scaledImage[i] = tmp.opNormalizedHarris(sspace.data->scales[i], 
                                                           sigma2, 
                                                           alpha);
    }
  }
  cout << "  opScaleSpaceHarris exit." << endl;
}


void applyHarrisOnScaleSpace( PyramidScaleSpace& sspace, const float sigma2, 
                              const float alpha,
                              const short type )
{  /* Apply the Harris detector to a pyramid scale space. */

  cout << "  opScaleSpaceHarris called on PyramidScaleSpace." << endl;
  if (sspace.data == 0) {
    throw GrayImage::OutOfBoundaryException();
  }
  if (sspace.data->refcount > 1) {
    ScaleSpaceData *tmp = sspace.data->clone();
    sspace.data->refcount--;
    sspace.data = tmp;
  }
  for (int i=0; i <= sspace.data->level; i++) {
    cout << "    Applying Harris on image " << i << "." << endl;
    OpHarrisImage tmp(sspace.data->scaledImage[i]);
    if (type == kImprovedHarris) {
      sspace.data->scaledImage[i] = tmp.opImprovedHarris(pow(sqrt(2.0),
                                                             (double)i), 
                                                         sigma2, 
                                                         alpha);
    } else if (type == kWindowHarris) {
      sspace.data->scaledImage[i] = tmp.opWindowHarris(pow(sqrt(2.0),
                                                           (double)i), alpha);
    } else {
      sspace.data->scaledImage[i] = tmp.opPyramidHarris(i, sqrt(2.0), sigma2, 
                                                        alpha);
    }
	}
  cout << "  opScaleSpaceHarris exit." << endl;
}



