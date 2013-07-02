/*********************************************************************/
/*                                                                   */
/* FILE         laplacepyramid.cc                                    */
/* AUTHORS      Thomas Hug, modified by Bastian Leibe                */
/* EMAIL        thug@student.ethz.ch, leibe@inf.ethz.ch              */
/*                                                                   */
/* CONTENT      Implementation of a Gaussian/Laplacian Pyramid.      */
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

#include "laplacepyramid.hh"

#include "polcoe.h"


/*===================================================================*/
/*                        Class LaplacePyramid                       */
/*===================================================================*/

/***********************************************************/
/*                       Constructors                      */
/***********************************************************/

LaplacePyramid::LaplacePyramid()
{
  data = 0;
  differences = 0;
  m_bNormalized = false;
}


LaplacePyramid::LaplacePyramid(const LaplacePyramid& source)
{
  data = source.data;
  if (source.data != 0) {
    data->refcount++;
  }
  differences = source.differences;
  if (source.differences != 0) {
    differences->refcount++;
  }
  m_bNormalized = source.m_bNormalized;
}


LaplacePyramid::LaplacePyramid(const OpGrayImage& source, int level)
  /*******************************************************************/
  /* Build up a Laplacian pyramid from the given source image with a */
  /* sqrt(2) scaling between levels. The program will try to create  */
  /* a pyramid with <level> levels, but will stop when the image     */
  /* size falls below GaussPyramid::minImageSize. For this reason, a */
  /* calling program should always check getNumLevels() to see how   */
  /* many levels were really created.                                */
  /*******************************************************************/
{
  cout << "LaplacePyramid::LaplacePyramid called." << endl;
  if (level < 0) {
    data = 0;
    differences = 0;
    return;
  }
  data        = new PyramidData(level);
  differences = new PyramidData(level);
  m_bNormalized = false;
  cout << "  Generating pyramid " << endl;
  
  /*********************************************/
  /*   Filter the first level with sigma=1.0   */
  /*********************************************/
  OpGrayImage sourceImg = source;
  sourceImg = sourceImg.opFastGauss(1.0);
  data->scaledImage[0] = sourceImg;
  //data->scaledImage[0] = sourceImg.opFastGauss(1.0);
  data->scales[0] = 1.0;
  differences->scales[0] = 1.0;
  
  /**************************************************/
  /*   Calculate the Laplacian on the first level   */
  /**************************************************/
  OpGrayImage laplaceImg(sourceImg);
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
  
  /**********************************/
  /*   Build up the higher levels   */
  /**********************************/
  for (int z=1; z < data->levels; z+=2) {
    if (sourceImg.width() <= minImageSize || 
        sourceImg.height() <= minImageSize) {
      data->levels = z - 1;
      differences->levels = z - 1;
      break;
    }

    /* filter with sigma=sqrt(2) for the intermediate level, */
    /* ...and with sigma=2 for the next full level.          */
    //OpGrayImage newLevel1 = sourceImg.opFastGauss(sqrt(2));
    //OpGrayImage newLevel2 = sourceImg.opFastGauss(2);
    OpGrayImage newLevel1 = sourceImg.opFastGauss(1);
    OpGrayImage newLevel2 = sourceImg.opFastGauss(sqrt(3.0));

    /* approximate the Laplacian for the original level by a DoG */
    OpGrayImage diff1 = resampleDown15(sourceImg);
    OpGrayImage diff2 = resampleDown20(newLevel1);
    newLevel1 = resampleDown15(newLevel1);
    for (int i=0; i<diff1.width(); i++)
      for (int j=0; j<diff1.height(); j++)
        diff1(i,j) = diff1(i,j).value() - newLevel1(i,j).value();
    differences->scaledImage[z] = diff1;
    //differences->scaledImage[z].mul(1.18);

    /* approximate the Laplacian for the intermediate level by a DoG */
    newLevel2 = resampleDown20(newLevel2);
    for (int i=0; i<diff2.width(); i++)
      for (int j=0; j<diff2.height(); j++)
        diff2(i,j) = diff2(i,j).value() - newLevel2(i,j).value();
    differences->scaledImage[z+1] = diff2;
    //differences->scaledImage[z+1].mul(1.7);

    /* resample the higher level */
    data->scaledImage[z]   = newLevel1;
    data->scaledImage[z+1] = newLevel2;
    sourceImg = newLevel2;
  }
  
  data->scales[0] = 1.0;
  differences->scales[0] = 1.0;
  for (int i=1; i<data->levels; i++) {
    data->scales[i] = data->scales[i-1] * sqrt(2.0);
    differences->scales[i] = differences->scales[i-1] * sqrt(2.0);
  }
  cout << "LaplacePyramid::LaplacePyramid exit." << endl;
}


LaplacePyramid::~LaplacePyramid()
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
/*                   Data Access Methods                   */
/***********************************************************/

OpGrayImage LaplacePyramid::getImage(int level) 
{
  assert( differences != 0 );
  if ( (level > differences->levels) || (level < 0) || 
       (differences->scaledImage == 0) ) {
    cout << "--Image out of level range." << endl;
    throw PyramidData::OutOfBoundaryException();
  }
  return differences->scaledImage[level];
}


/***********************************************************/
/*                  Data Access Operators                  */
/***********************************************************/

GrayPixel LaplacePyramid::operator()(const int x, const int y, 
                                   const int z) const 
{
  int xs, ys;
  convertToPyramid( x, y, z, xs, ys );
  return differences->scaledImage[z](xs,ys);
}


GrayPixel& LaplacePyramid::operator()(const int x, const int y, 
                                    const int z) 
{
  int xs = x, ys = y;
  convertToPyramid( x, y, z, xs, ys );
  return differences->scaledImage[z](xs,ys);
}


LaplacePyramid& LaplacePyramid::operator=( LaplacePyramid other ) 
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
  m_bNormalized = other.m_bNormalized;
  return *this;
}


/***********************************************************/
/*                Operators on Scale Levels                */
/***********************************************************/

void LaplacePyramid::opNormalize(int order) 
{
  cout << "  LaplacePyramid::opNormalize called. order=" << order << endl;
  assert( differences != 0 );
  
  differences->opNormalize( order );
  m_bNormalized = true;

  cout << "  LaplacePyramid::opNormalize exit." << endl;
}


void LaplacePyramid::opNonMaximumSuppression3D(int cubeSize) 
{
  cout << "  LaplacePyramid::opNonMaximumSuppression3D called. cubeSize=" 
       << cubeSize << endl;
  assert( differences != 0 );

  differences->opNonMaximumSuppression3D( cubeSize );

  cout << "  LaplacePyramid::opNonMaximumSuppression3D exit." << endl;
}


void LaplacePyramid::opNonMaximumSuppression2D(int windowSize) 
{
  cout << "  LaplacePyramid::opNonMaximaSuppression2D called. windowSize=" 
       << windowSize << endl;
  assert( differences != 0 );

  differences->opNonMaximumSuppression2D( windowSize );
  
  cout << "  LaplacePyramid::opNonMaximumSuppression2D exit." << endl;
}


void LaplacePyramid::opSuppressCornerEffects() {
  cout << "  LaplacePyramid::opSuppressCornerEffects called." << endl;
  assert( differences != 0 );

  differences->opSuppressCornerEffects();

  cout << "  LaplacePyramid::opSuppressCornerEffects exit." << endl;
}


bool LaplacePyramid::isMaximum3D(int x, int y, int z, int dist) const 
{
  assert( differences != 0 );
  return differences->isMaximum3D( x, y, z, dist );
}


bool LaplacePyramid::isMaximum2D(int x, int y, int z, int dist) const 
{
  assert( differences != 0 );
  return differences->isMaximum2D( x, y, z, dist );
}


OpGrayImage LaplacePyramid::getCharacteristicScaleImg() const
  /* Return an image containing the characteristic scales for all    */
  /* pixels.                                                         */
{
  cout << "  LaplacePyramid::getCharacteristicScaleImg called." << endl;
  assert( differences != 0 );
  OpGrayImage resultImg( differences->scaledImage[0].width(), 
                         differences->scaledImage[0].height() );

  cout << "    Iterating over all pixels..." << endl;
  cout << "    Image size: (" << resultImg.width() << "," 
       << resultImg.height() << ")" << endl;
  for( int y=0; y<resultImg.height(); y++ )
    for( int x=0; x<resultImg.width(); x++ )
      try{
        //cout << "    Testing pixel (" << x << "," << y << ")" << endl;
        resultImg(x,y) = getCharacteristicScale( x, y );
      } catch( PyramidData::OutOfBoundaryException ) {
        cerr << "Out of Boundary Exception encountered at pixel (" << x 
             << "," << y << ")!" << endl;
      } catch( PyramidData::ObjectInUseException ) {
        cerr << "Object in Use Exception encountered at pixel (" << x 
             << "," << y << ")!" << endl;
      } catch(exception &er) {
        cerr << "Unknown Exception encountered at pixel (" << x 
             << "," << y << ")!" << endl;
        cerr << "Error: " << er.what() << endl;
      }
  cout << "  LaplacePyramid::getCharacteristicScaleImg exit." << endl;
  return resultImg;
}


float LaplacePyramid::getCharacteristicScale(int x, int y) const 
  /* Returns the characteristic scale of point(x,y). */
{
  assert( differences != 0 );
  if ( (differences->scaledImage == 0) ||
       (x < 0) || (x > data->scaledImage[0].width()) ||
       (y < 0) || (y > data->scaledImage[0].height()) ) {
    cout << "--Coordinates out of image range." << endl;
    throw PyramidData::OutOfBoundaryException();
  }

  //if( !m_bNormalized )
  //differences->opNormalize( 1 );

  //cout << "      Calling getMaximumOverScales()..." << endl;
  int z = differences->getMaximumOverScales( x, y );
  //cout << "      done, result=" << z << endl;

  /* if an interpolated characteristic scale is needed, */
  /* this would be the place to calculate it.           */
  
  float result = 0.0;
  float value = -1.0;
  if ( z >= 0 ) {
    result = (float) differences->scales[z];

    if( differences->levels >= 3 ) {
      /* interpolate the characteristic scale using a 2nd order poly-    */
      /* nomial.                                                         */
      float idx[3];
      float val[3];
      if( z == 0 ) {
        idx[0] = (float) 0; idx[1] = (float) 1; idx[2] = (float) 2;
      } else if( z == differences->levels-1 ) {
        idx[0] = (float) differences->levels-3; 
        idx[1] = (float) differences->levels-2; 
        idx[2] = (float) differences->levels-1;
      } else {
        idx[0] = (float) z-1; idx[1] = (float) z; idx[2] = (float) z+1;
      }
      
      int xp, yp;
      convertToPyramid( x, y, (int)idx[0], xp, yp );
      val[0] = differences->scaledImage[(int)idx[0]](xp,yp).value();
      convertToPyramid( x, y, (int)idx[1], xp, yp );
      val[1] = differences->scaledImage[(int)idx[1]](xp,yp).value();
      convertToPyramid( x, y, (int)idx[2], xp, yp );
      val[2] = differences->scaledImage[(int)idx[2]](xp,yp).value();
      
      float scidx[3];
      for( int i=0; i<3; i++ )
        scidx[i] = differences->scales[(int)idx[i]];

      float c[3];
      /* interpolate the polynomial */
      //polcoe( idx, val, 2, c );
      polcoe( scidx, val, 2, c );
      
      /* find the maximum: the parabole's apex is at           */
      /*   ( -c1/2c2 | -(c1^2-4c0c2)/4c2 )                     */
      /* If c[2]<0, take the apex, else take the border value. */
      if( c[2] < 0.0 ) {
        result = -c[1]/(2.0*c[2]);
        value  = -(c[1]*c[1]-4.0*c[0]*c[2])/(4.0*c[2]);
      }

      /* try different formula */
      if( (z > 0) && (z < differences->levels-1) )
        result = scidx[1] + 1.0 + (val[0]-val[2])/(2.0*(val[0]+val[2]-2.0*val[1]));
      
      if ( result < 1.0 ) result = 1.0;
      if ( result > 4.0*differences->scales[differences->levels-1] ) 
        result = 4.0*differences->scales[differences->levels-1];
    }
  }
  return result;
}


void LaplacePyramid::getCharacteristicScale( int x, int y, 
                                             float &scale, float &value )
  /* Returns the characteristic scale and value of point(x,y). */
{
  assert( differences != 0 );
  if (data->scaledImage == 0 ||
      x < 0 || x > data->scaledImage[0].width() ||
      y < 0 || y > data->scaledImage[0].height()) {
    cout << "--Coordinates out of image range." << endl;
    throw PyramidData::OutOfBoundaryException();
  }

  if( !m_bNormalized )
    differences->opNormalize( 1 );

  int z = differences->getMaximumOverScales( x, y );
  if ( z >= 0 ) {
    scale = differences->scales[z];

    int xp, yp;
    convertToPyramid( x, y, z, xp, yp );
    value = differences->scaledImage[z](xp,yp).value();

  } else {
    scale = 0.0;
    value = -1.0;
  }
}
