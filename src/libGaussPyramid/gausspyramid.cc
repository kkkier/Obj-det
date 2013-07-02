/*********************************************************************/
/*                                                                   */
/* FILE         gausspyramid.cc                                      */
/* AUTHORS      Thomas Hug, modified by Bastian Leibe                */
/* EMAIL        thug@student.ethz.ch, leibe@inf.ethz.ch              */
/*                                                                   */
/* CONTENT      Implementation of a Gaussian Pyramid.                */
/*                                                                   */
/* BEGIN        Mon Dec 02 2002                                      */
/* LAST CHANGE  Mon Dec 02 2002                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <math.h>
#include <assert.h>
#include <iostream>

#include "gausspyramid.hh"


/*===================================================================*/
/*                      Class GaussPyramid                      */
/*===================================================================*/

/***********************************************************/
/*                       Constructors                      */
/***********************************************************/

GaussPyramid::GaussPyramid()
{
  data = 0;
}


GaussPyramid::GaussPyramid(const GaussPyramid& source) 
{
  data = source.data;
  if (source.data != 0) {
    data->refcount++;
  }
}


GaussPyramid::GaussPyramid(const OpGrayImage& source, int level) 
  /*******************************************************************/
  /* Build up a Gaussian pyramid from the given source image with a  */
  /* sqrt(2) scaling between levels. The program will try to create  */
  /* a pyramid with <level> levels, but will stop when the image     */
  /* size falls below GaussPyramid::minImageSize. For this reason, a */
  /* calling program should always check getNumLevels() to see how   */
  /* many levels were really created.                                */
  /*******************************************************************/
{
  cout << "GaussPyramid::GaussPyramid called." << endl;
  if (level < 0) {
    data = 0;
    return;
  }
  data = new PyramidData(level);
  cout << "  Generating pyramid " << endl;
  
  /*********************************************/
  /*   Filter the first level with sigma=1.0   */
  /*********************************************/
  OpGrayImage sourceImg = source;
  sourceImg = sourceImg.opFastGauss(1.0);
  data->scaledImage[0] = sourceImg;
  //data->scaledImage[0] = sourceImg.opFastGauss(1.0);
  data->scales[0] = 1.0;
     
  /**********************************/
  /*   Build up the higher levels   */
  /**********************************/
  for (int z = 1; z < data->levels; z+=2) {
    if (sourceImg.width() <= minImageSize || 
        sourceImg.height() <= minImageSize) {
      data->levels = z - 1;
      break;
    }

    /* filter with sigma=sqrt(2) for the intermediate level, */
    /* ...and with sigma=2 for the next full level.          */
    //OpGrayImage newLevel1 = sourceImg.opFastGauss(sqrt(2));
    //OpGrayImage newLevel2 = sourceImg.opFastGauss(2);
    OpGrayImage newLevel1 = sourceImg.opFastGauss(1.0);
    OpGrayImage newLevel2 = sourceImg.opFastGauss(sqrt(3.0));
    /* Together with the initial filter with sigma=1, this results   */
    /* in scales of sqrt(2) and 2.                                   */

    newLevel1 = resampleDown15(newLevel1);
    newLevel2 = resampleDown20(newLevel2);
    data->scaledImage[z] = newLevel1;
    data->scaledImage[z+1] = newLevel2;
    sourceImg = newLevel2;
  }
  
  //data->scales[1] = sqrt(2.0);
  //data->scales[2] = 2.0;
  for (int i=1; i<data->levels; i++) {
    data->scales[i] = data->scales[i-1] * sqrt(2.0);
  }
  cout << "GaussPyramid::GaussPyramid exit." << endl;
}


GaussPyramid::~GaussPyramid() 
{
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

float GaussPyramid::getScaleAtLevel(int level) const 
{
  assert( data != 0 );
  if ( (level > data->levels) || (level < 0) || (data->scales == 0) ) {
    throw PyramidData::OutOfBoundaryException();
  }
  return data->scales[level];
}


int GaussPyramid::getLevelAtScale(float scale) const 
{
  assert( data != 0 );
  if ( data->scales == 0 ) {
    throw PyramidData::OutOfBoundaryException();
  }
  for (int i=0; i<data->levels; i++) {
    if (data->scales[i] == scale) {
      return i;
    }
  }
  return -1;
}


int GaussPyramid::getNumLevels() const 
{
  assert( data != 0 );
  return data->levels;
}


OpGrayImage GaussPyramid::getImage(int level) 
{
  assert( data != 0 );
  if ( (level > data->levels) || (level < 0) || (data->scaledImage == 0) ) {
    cout << "--Image out of level range." << endl;
    throw PyramidData::OutOfBoundaryException();
  }
  return data->scaledImage[level];
}


/***********************************************************/
/*                  Data Access Operators                  */
/***********************************************************/

GrayPixel GaussPyramid::operator()(const int x, const int y, 
                                   const int z) const 
{
  int xs, ys;
  convertToPyramid( x, y, z, xs, ys );
  return data->scaledImage[z](xs,ys);
}


GrayPixel& GaussPyramid::operator()(const int x, const int y, 
                                    const int z) 
{
  int xs, ys;
  convertToPyramid( x, y, z, xs, ys );
  return data->scaledImage[z](xs,ys);
}


GaussPyramid& GaussPyramid::operator=( GaussPyramid other ) 
{
  if( this->data == other.data )
    return *this;

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

void GaussPyramid::opNormalize(int order) 
{
  cout << "  GaussPyramid::opNormalize called. order=" << order << endl;
  assert( data != 0 );

  data->opNormalize( order );

  cout << "  GaussPyramid::opNormalize exit." << endl;
}


void GaussPyramid::opNonMaximumSuppression3D(int cubeSize) 
{
  cout << "  GaussPyramid::opNonMaximumSuppression3D called. cubeSize=" 
       << cubeSize << endl;
  assert( data != 0 );

  data->opNonMaximumSuppression3D( cubeSize );

  cout << "  GaussPyramid::opNonMaximumSuppression3D exit." << endl;
}


void GaussPyramid::opNonMaximumSuppression2D(int windowSize) 
{
  cout << "  GaussPyramid::opNonMaximumSuppression2D called. windowSize=" 
       << windowSize << endl;
  assert( data != 0 );

  data->opNonMaximumSuppression2D( windowSize );

  cout << "  GaussPyramid::opNonMaximumSuppression2D exit." << endl;
}


void GaussPyramid::opSuppressCornerEffects() {
  cout << "  GaussPyramid::opSuppressCornerEffects called." << endl;
  assert( data != 0 );

  data->opSuppressCornerEffects();

  cout << "  GaussPyramid::opSuppressCornerEffects exit." << endl;
}


bool GaussPyramid::isMaximum3D(int x, int y, int z, int dist) const 
{
  assert( data != 0 );
  return data->isMaximum3D( x, y, z, dist );
}


bool GaussPyramid::isMaximum2D(int x, int y, int z, int dist) const 
{
  assert( data != 0 );
  return data->isMaximum2D( x, y, z, dist );
}


void GaussPyramid::convertToPyramid( int xk, int yk, int z, 
                                     int &xp, int &yp ) const 
  /* convert the pixel from Kartesian to pyramid coordinates */
{
  assert( data != 0 );
  data->convertToPyramid( xk, yk, z, xp, yp );
}


void GaussPyramid::convertToKartesian( int xp, int yp, int z, 
                                       int &xk, int &yk ) const 
  /* convert the pixel from pyramid to Kartesian coordinates */
{
  assert( data != 0 );
  data->convertToKartesian( xp, yp, z, xk, yk );
}


OpGrayImage GaussPyramid::resampleUp(const OpGrayImage& source) const
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
  cout << "GaussPyramid::resampleUp called." << endl;
  int imageWidth =  (int) ceil( (double)source.width()  * 1.5 );
  int imageHeight = (int) ceil( (double)source.height() * 1.5 );
  
  OpGrayImage result(imageWidth, imageHeight);
  OpGrayImage copy(source);
  
  // average 4 neighbours
  cout << "--AV4." << endl;
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
  cout << "--AV2-." << endl;
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
  
  cout << "GaussPyramid::resampleUp exit." << endl;
  return result;
}


OpGrayImage GaussPyramid::resampleDown15(const OpGrayImage& source) const 
{
  cout << "GaussPyramid::resampleDown15 called." << endl;
  int imageWidth = (int)  floor((double)source.width() / 1.5);
  int imageHeight = (int) floor((double)source.height() / 1.5);
  
  OpGrayImage result(imageWidth, imageHeight);
  
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
  
  cout << "GaussPyramid::resampleDown15 exit." << endl;
  return result;
}


OpGrayImage GaussPyramid::resampleDown20(const OpGrayImage& source) const 
{
  cout << "GaussPyramid::resampleDown20 called." << endl;
  //int imageWidth  = (int)ceil((float)source.width()/2.0);
  //int imageHeight = (int)ceil((float)source.height()/2.0);
  int imageWidth  = (int)floor((float)source.width()/2.0);
  int imageHeight = (int)floor((float)source.height()/2.0);
  
  OpGrayImage result(imageWidth, imageHeight);
  for (int i=0; i<imageWidth; i++) {
    for (int j=0; j<imageHeight; j++) {
      result(i,j) = source(2*i,2*j);
    }
  }
  
  cout << "GaussPyramid::resampleDown20 exit." << endl;
  return result;
}


