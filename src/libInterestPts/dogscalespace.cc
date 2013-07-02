/*********************************************************************/
/*                                                                   */
/* FILE         dogscalespace.cc                                     */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Implementation of an image scale space.              */
/*                                                                   */
/* BEGIN        Mon Dec 15 2003                                      */
/* LAST CHANGE  Mon Dec 15 2003                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <math.h>
#include <iostream>

#include "polcoe.h"
#include "dogscalespace.hh"


/*===================================================================*/
/*                         Class DoGScaleSpace                       */
/*===================================================================*/

/***********************************************************/
/*                       Constructors                      */
/***********************************************************/

DoGScaleSpace::DoGScaleSpace(){
  data = 0;
}


DoGScaleSpace::DoGScaleSpace(const DoGScaleSpace& source) 
{
  data = source.data;
  if (source.data != 0) {
    data->refcount++;
  }
}


DoGScaleSpace::DoGScaleSpace( /*const*/ OpGrayImage& imgSource, 
                              int nNumOctaves, 
                              int nLevelsPerOctave, float dSigma0 ) 
{
  bool bVerbose = false;
  if( bVerbose )
    std::cout << "DoGScaleSpace::ScaleSpace called." << std::endl;
  m_nNumLevels = nLevelsPerOctave * nNumOctaves + 1;
  //m_dSigmaStep = 2.0 / sqrt( (float)nLevelsPerOctave ); 
  m_dSigmaStep = pow( 2.0, 1.0/(float)nLevelsPerOctave );
  data = new ScaleSpaceData( m_nNumLevels );

  /********************************/
  /*   Generate the Scale Space   */
  /********************************/
  if( bVerbose )
    std::cout << "  Generating scale space with " << nNumOctaves 
              << " octaves ("
              << m_nNumLevels << " levels)" << std::endl;
  data->sourceImage = imgSource;
  OpGrayImage imgExtra;
  if( m_nNumLevels > 0 ) {
    /* create the first level */
    float dSigma = dSigma0;
    if( bVerbose )
      std::cout << "    Apply gauss on image. level=" << 0 << " sigma=" 
                << dSigma 
         << std::endl;
    //data->scaledImage[0] = imgSource.opFastGauss( dSigma0 );
    data->scaledImage[0] = imgSource.opGauss( dSigma0 );
    data->scales[0] = dSigma0;

    /* finish the first octave */
    for( int j=1; j<nLevelsPerOctave; j++ ) {
      dSigma = dSigma * m_dSigmaStep;
      //dSigma = sqrt(dSigma*dSigma + m_dSigmaStep*m_dSigmaStep);
      if( bVerbose )
        std::cout << "    Apply gauss on image. level=" << j << " sigma=" 
                  << dSigma 
           << std::endl;
      //data->scaledImage[j] = data->scaledImage[j-1].opFastGauss(m_dSigmaStep);
      //data->scaledImage[j] = imgSource.opFastGauss( dSigma );
      data->scaledImage[j] = imgSource.opGauss( dSigma );
      data->scales[j] = dSigma;
    }

    /* create the remaining octaves */
    for (int i = 1; i < nNumOctaves; i++) {
      /* create the first image of the new octave */
      int idxlast = (i-1)*nLevelsPerOctave;
      int idx     = i*nLevelsPerOctave;
      dSigma = data->scales[idxlast] * 2.0;
      //dSigma = sqrt(data->scales[idxlast]*data->scales[idxlast] + 4.0 );
      if( bVerbose )
        std::cout << "    Apply gauss on image. level=" << idx 
                  << " sigma=" << dSigma 
           << std::endl;
      //data->scaledImage[idx] = data->scaledImage[idxlast].opFastGauss(2.0);
      //data->scaledImage[idx] = imgSource.opFastGauss( dSigma );
      data->scaledImage[idx] = imgSource.opGauss( dSigma );
      data->scales[idx] = dSigma;
      
      /* finish the octave */
      for( int j=idx+1; j<idx+nLevelsPerOctave; j++ ) {
        dSigma = dSigma * m_dSigmaStep;
        //dSigma = sqrt(dSigma*dSigma + m_dSigmaStep*m_dSigmaStep);
        if( bVerbose )
          std::cout << "    Apply gauss on image. level=" << j 
                    << " sigma=" << dSigma 
             << std::endl;
        //data->scaledImage[j]= data->scaledImage[j-1].opFastGauss(m_dSigmaStep);
        //data->scaledImage[j] = imgSource.opFastGauss( dSigma );
        data->scaledImage[j] = imgSource.opGauss( dSigma );
        data->scales[j] = dSigma;
      }
    }
    
    /* finish the last image */
    int idxlast = (nNumOctaves-1)*nLevelsPerOctave;
    int idx     = nNumOctaves*nLevelsPerOctave;
    dSigma = data->scales[idxlast] * 2.0;
    //dSigma = sqrt(data->scales[idxlast]*data->scales[idxlast] + 4.0 );
    if( bVerbose )
      std::cout << "    Apply gauss on image. level=" << idx 
                << " sigma=" << dSigma 
                << std::endl;
    //data->scaledImage[idx] = data->scaledImage[idxlast].opFastGauss(2.0);
    //data->scaledImage[idx] = imgSource.opFastGauss( dSigma );
    data->scaledImage[idx] = imgSource.opGauss( dSigma );
    data->scales[idx] = dSigma;

    /* finish one extra image */
    dSigma = data->scales[idx] * m_dSigmaStep;
    //dSigma = sqrt(dSigma*dSigma + m_dSigmaStep*m_dSigmaStep);
    if( bVerbose )
      std::cout << "    Apply gauss on extra  level=" << idx+1 
                << " sigma=" << dSigma 
                << std::endl;
    //imgExtra = data->scaledImage[idx].opFastGauss(m_dSigmaStep);
    //imgExtra = imgSource.opFastGauss( dSigma );
    imgExtra = imgSource.opGauss( dSigma );
  }

  /*************************************/
  /*   Compute the Difference Images   */
  /*************************************/
  if( bVerbose )
    std::cout << "  Computing difference images..." << std::endl;
  for( int i=0; i<m_nNumLevels-1; i++ )
    data->scaledImage[i] = data->scaledImage[i+1] - data->scaledImage[i];
  data->scaledImage[m_nNumLevels-1] = ( imgExtra -
                                        data->scaledImage[m_nNumLevels-1] );

  if( bVerbose )
    std::cout << "DoGScaleSpace::ScaleSpace exit." << std::endl;
}


DoGScaleSpace::~DoGScaleSpace() {
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

float DoGScaleSpace::getScaleAtLevel(int level) const 
{
  if (level > data->level || level < 0 || data->scales == 0) {
    throw OutOfBoundaryException();
  }
  return data->scales[level];
}


int DoGScaleSpace::getLevelAtScale(float scale) const 
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


float DoGScaleSpace::getLevel() const 
{
  return data->level;
}


OpGrayImage& DoGScaleSpace::getImage(int level) 
{
  if (level > data->level || level < 0 || data->scaledImage == 0) {
    std::cout << "--Image out of level range." << std::endl;
    throw OutOfBoundaryException();
  }
  return data->scaledImage[level];
}


bool DoGScaleSpace::isMaximum3D(int x, int y, int z, int dist) const 
{
  if (z > data->level || z < 0 || data->scaledImage == 0) {
    std::cout << "--Coordinates out of level range." << std::endl;
		throw OutOfBoundaryException();
  }
  if (x < 0 || x > data->scaledImage[z].width() ||
      y < 0 || y > data->scaledImage[z].height()) {
    std::cout << "--Coordinates out of image range." << std::endl;
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


bool DoGScaleSpace::isMinimum3D(int x, int y, int z, int dist) const 
{
  if (z > data->level || z < 0 || data->scaledImage == 0) {
    std::cout << "--Coordinates out of level range." << std::endl;
		throw OutOfBoundaryException();
  }
  if (x < 0 || x > data->scaledImage[z].width() ||
      y < 0 || y > data->scaledImage[z].height()) {
    std::cout << "--Coordinates out of image range." << std::endl;
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
            if (neighbour <= actual) {
              return false;
            }
          }
        }
      }
    }
  }
  return true;
}


bool DoGScaleSpace::isExtremum3D(int x, int y, int z, int dist) const 
{
  if (z > data->level || z < 0 || data->scaledImage == 0) {
    std::cout << "--Coordinates out of level range." << std::endl;
		throw OutOfBoundaryException();
  }
  if (x < 0 || x > data->scaledImage[z].width() ||
      y < 0 || y > data->scaledImage[z].height()) {
    std::cout << "--Coordinates out of image range." << std::endl;
    throw OutOfBoundaryException();
  }

  float actual = data->scaledImage[z](x,y).value();

  bool bMaximum = true;
  bool bMinimum = true;
  for (int k = (y-dist); k <= (y+dist); k++)
    for (int j = (x-dist); j <= (x+dist); j++) {
      if (j >= 0 && j < data->scaledImage[z].width() &&
          k >= 0 && k < data->scaledImage[z].height() ) {
        if( !(j == x && k == y) ) {
          float neighbour = data->scaledImage[z](j,k).value();
          if (neighbour <= actual)
            bMinimum = false;
          if (neighbour >= actual)
            bMaximum = false;
        }
      } else {
        bMinimum = bMaximum = false;
      }
    }
  
  if( !bMinimum && !bMaximum )
    return false;
      
  float bScMaximum = true;
  float bScMinimum = true;
  for (int i = (z-dist); i <= (z+dist); i++) {
    if ( i != z )
      if (i >= 0 && i <= data->level) {
        for (int k = (y-dist); k <= (y+dist); k++)
          for (int j = (x-dist); j <= (x+dist); j++) {
            if (j >= 0 && j < data->scaledImage[i].width() &&
                k >= 0 && k < data->scaledImage[i].height() ) {
              float neighbour = data->scaledImage[i](j,k).value();
              if (neighbour <= actual)
                bScMinimum = false;
              if (neighbour >= actual)
                bScMaximum = false;
            }
          }
      }
  }

  if( !bScMinimum && !bScMaximum )
    return false;
  else
    return true;
}


bool DoGScaleSpace::isMaximum2D(int x, int y, int z, int dist) const 
{
  if (z > data->level || z < 0 || data->scaledImage == 0) {
    std::cout << "--Coordinates out of level range." << std::endl;
    throw OutOfBoundaryException();
  }
  if (x < 0 || x > data->scaledImage[z].width() ||
      y < 0 || y > data->scaledImage[z].height()) {
    std::cout << "--Coordinates out of image range." << std::endl;
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


bool DoGScaleSpace::isNotOnEdge(int x, int y, int z, float dEdgeThresh) const
  /* (adapted version of David Lowe's code) */
{
  float H00, H11, H01, det, trace, inc;
  OpGrayImage &img = data->scaledImage[z];
  
  /* Compute 2x2 Hessian values from pixel differences. */
  H00 = img(x-1,y).value() - 2.0*img(x,y).value() + img(x+1,y).value();
  H11 = img(x,y-1).value() - 2.0*img(x,y).value() + img(x,y+1).value();
  H01 = ( (img(x+1,y+1).value() - img(x+1,y-1).value()) - 
          (img(x-1,y+1).value() - img(x-1,y-1).value()) ) / 4.0;
  
  /* Compute determinant and trace of the Hessian. */
  det   = H00 * H11 - H01 * H01;
  trace = H00 + H11;
  
  /* To detect an edge response, we require the ratio of smallest
     to largest principle curvatures of the DOG function
     (eigenvalues of the Hessian) to be below a threshold.  For
     efficiency, we use Harris' idea of requiring the determinant to
     be above a threshold times the squared trace.
  */
  inc = dEdgeThresh + 1.0;
  return (det * inc * inc  > dEdgeThresh * trace * trace);
}


float DoGScaleSpace::getCharacteristicScale( int x, int y, 
                                             bool bInterpolate ) const 
  /* Returns the characteristic scale of point(x,y). */
{
  if (data->scaledImage == 0 ||
      x < 0 || x > data->scaledImage[0].width() ||
      y < 0 || y > data->scaledImage[0].height()) {
    std::cout << "--Coordinates out of image range." << std::endl;
    throw OutOfBoundaryException();
  }

  float maxval   = 0.0;
  float maxscale = -1.0;
  for (int z=1; z<m_nNumLevels-1; z++) {
    float actual = data->scaledImage[z](x,y).value();
    float last = data->scaledImage[z-1](x,y).value();
    float next = data->scaledImage[z+1](x,y).value();
    if ( ((actual > last) && (actual > next)) ||
         ((actual < last) && (actual < next)) ) {
      float scale = data->scales[z];
      float value = actual;
      if( bInterpolate )
        interpolateScales( data->scales[z-1], last,
                           data->scales[z], actual,
                           data->scales[z+1], next,
                           scale, value );

      if( fabs(value) > maxval ) {
        maxval   = fabs(value);
        maxscale = scale;
      }
    }
  }
  return maxscale;
}


void DoGScaleSpace::getCharacteristicScales( int x, int y, 
                                             std::vector<float> &vScales, 
                                             std::vector<float> &vValues,
                                             bool bInterpolate ) const 
  /* Returns the characteristic scale of point(x,y). */
{
  if (data->scaledImage == 0 ||
      x < 0 || x > data->scaledImage[0].width() ||
      y < 0 || y > data->scaledImage[0].height()) {
    std::cout << "--Coordinates out of image range." << std::endl;
    throw OutOfBoundaryException();
  }

  float dNormFactor = 1.0 / (m_dSigmaStep - 1.0); 
  vScales.clear();
  vValues.clear();
  for (int z=1; z<m_nNumLevels-1; z++) {
    float actual = data->scaledImage[z](x,y).value();
    float last = data->scaledImage[z-1](x,y).value();
    float next = data->scaledImage[z+1](x,y).value();
    if ( (actual > last) && (actual > next)) {
      /* scale maximum */
      float scale = data->scales[z];
      float value = actual;
      if( bInterpolate )
        interpolateScales( data->scales[z-1], last,
                           data->scales[z], actual,
                           data->scales[z+1], next,
                           scale, value );
      vScales.push_back( scale );
      vValues.push_back( value * dNormFactor );

    } else if ( (actual < last) && (actual < next)) {
      /* scale minimum */
      float scale = data->scales[z];
      float value = actual;
      if( bInterpolate )
        interpolateScales( data->scales[z-1], last,
                           data->scales[z], actual,
                           data->scales[z+1], next,
                           scale, value );
      vScales.push_back( scale );
      vValues.push_back( value * dNormFactor );
    }
  }
}


PointVector DoGScaleSpace::getScaleMaxima3D( float dThreshold, 
                                             bool  bInterpolate,
                                             bool  bRemoveEdges,
                                             float dEdgeThresh ) const 
  /* Returns the characteristic scale of point(x,y). */
{
  PointVector vPoints;

  // factor for making the results comparable between different 
  // settings for the #levels/octave parameter
  float dNormFactor = 1.0 / (m_dSigmaStep - 1.0); 

  int w = data->sourceImage.width();
  int h = data->sourceImage.height();
  /* for every pixel of the image */
  for( int y=0; y<h; y++ )
    for( int x=0; x<w; x++ )
      /* search all scale extrema */
      for (int z=1; z<m_nNumLevels-1; z++) {
        float actual = data->scaledImage[z](x,y).value();
        float last = data->scaledImage[z-1](x,y).value();
        float next = data->scaledImage[z+1](x,y).value();
        //if ( isMaximum3D(x,y,z,1) || isMinimum3D(x,y,z,1) ) {
        if ( isExtremum3D(x,y,z,1) ) {
          /* scale extremum */
          float scale = data->scales[z];
          float value = actual;
          if( bInterpolate )
            interpolateScales( data->scales[z-1], last,
                               data->scales[z], actual,
                               data->scales[z+1], next,
                               scale, value );

          /* if value is above threshold */
          float normval = fabs(value)*dNormFactor;
          if( normval >= dThreshold ) {
            bool bNotOnEdge = true;
            if( bRemoveEdges )
              bNotOnEdge = isNotOnEdge( x, y, z, dEdgeThresh );
            if( bNotOnEdge ) {
              InterestPoint pt;
              pt.x     = x;
              pt.y     = y;
              pt.scale = scale;
              pt.value = normval;
              pt.angle = 0.0;
              pt.l1    = scale;
              pt.l2    = scale;
              
              vPoints.push_back( pt );
            }
          }
        }
      }
  return vPoints;
}


void DoGScaleSpace::interpolateScales( float scale1, float val1,
                                       float scale2, float val2,
                                       float scale3, float val3,
                                       float &scale, float &value ) const
  {
  /* interpolate the characteristic scale using a 2nd order poly-    */
  /* nomial.                                                         */
  float idx[3];
  float val[3];

  idx[0] = scale1;  val[0] = val1;
  idx[1] = scale2;  val[1] = val2;
  idx[2] = scale3;  val[2] = val3;
      
  float c[3];
  /* interpolate the polynomial */
  polcoe( idx, val, 2, c );
  
  /* find the maximum: the parabole's apex is at           */
  /*   ( -c1/2c2 | -(c1^2-4c0c2)/4c2 )                     */
  scale = -c[1]/(2.0*c[2]);
  value  = -(c[1]*c[1]-4.0*c[0]*c[2])/(4.0*c[2]);

  if( (scale < 1.0) || (scale > data->scales[m_nNumLevels-1]) ) {
    scale = -1.0;
    value = 0.0;
  }
}


// float DoGScaleSpace::getCharacteristicScaleValue(int x, int y) const 
//   /* Returns the characteristic scale value of point(x,y). */
// {
//   if (data->scaledImage == 0 ||
//       x < 0 || x > data->scaledImage[0].width() ||
//       y < 0 || y > data->scaledImage[0].height()) {
//     cout << "--Coordinates out of image range." << endl;
//     throw OutOfBoundaryException();
//   }
//   for (int z=1; z<data->level; z++) {
//     float actual = data->scaledImage[z](x,y).value();
//     float last = data->scaledImage[z-1](x,y).value();
//     float next = data->scaledImage[z+1](x,y).value();
//     if (actual > last && actual > next) {
//       //if (isMaximum3D(x,y,z)) {
//       return data->scaledImage[z](x,y).value();
//     }
//   }
//   return 0.0;
// }

		
/***********************************************************/
/*                  Data Access Operators                  */
/***********************************************************/

GrayPixel  DoGScaleSpace::operator()(const int x, const int y, const int z) const
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


GrayPixel& DoGScaleSpace::operator()(const int x, const int y, const int z) 
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


DoGScaleSpace& DoGScaleSpace::operator=( const DoGScaleSpace &other ) 
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

DoGScaleSpace& DoGScaleSpace::opSquareGradient() 
{
  std::cout << "  DoGScaleSpace::opSquareGradient called." << std::endl;
  if (data == 0) {
    throw OutOfBoundaryException();
  }
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  for (int z=0; z <= data->level; z++) {
    std::cout << "    Calculating Gauss Derivatives." << std::endl;
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
  std::cout << "    Normalizing image according to scale." << std::endl;
  *this = this->opNormalize(2);
  std::cout << "  DoGScaleSpace::opLaplacian exit." << std::endl;
  return *this;
}


DoGScaleSpace& DoGScaleSpace::opLaplacian() 
{
  std::cout << "  DoGScaleSpace::opLaplacian called." << std::endl;
  if (data == 0) {
    throw OutOfBoundaryException();
  }
  if (data->refcount > 1) {
    ScaleSpaceData *tmp = data->clone();
    data->refcount--;
    data = tmp;
  }
  for (int i=0; i <= data->level; i++) {
    std::cout << "    Applying Laplacian on image " << i << "." << std::endl;
    data->scaledImage[i] = data->scaledImage[i].opGaussLap(data->scales[i]);
    //data->scaledImage[i] = data->sourceImage.opGaussLap(data->scales[i]);
  }
  std::cout << "    Normalizing image according to scale." << std::endl;
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
  std::cout << "  DoGScaleSpace::opLaplacian exit." << std::endl;
  return *this;
}


DoGScaleSpace& DoGScaleSpace::opDifferenceOfGaussian() 
  /* Apply DOG on scale space. Assumes that the scale space is unmo- */
  /* dified, e.g. consists still of gaussians.                       */
{
  std::cout << "  DoGScaleSpace::opDifferenceOfGaussian called." << std::endl;
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
  std::cout << "  DoGScaleSpace::opDifferenceOfGaussian exit." << std::endl;
  return *this;
}


DoGScaleSpace& DoGScaleSpace::opNormalize(int order) 
{
  std::cout << "  DoGScaleSpace::opNormalize called. order=" << order << std::endl;
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
  std::cout << "  DoGScaleSpace::opNormalize exit." << std::endl;
  return *this;
}


DoGScaleSpace& DoGScaleSpace::opNonMaximaSuppression3D(int cubeSize) 
{  
  std::cout << "  DoGScaleSpace::opNonMaximaSuppression3D called. cubeSize=" 
       << cubeSize << std::endl;
  if (data == 0) {
    throw OutOfBoundaryException();
  }
  cubeSize = (int) floor((double)cubeSize);
  DoGScaleSpace result(*this);
  
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


DoGScaleSpace& DoGScaleSpace::opNonMaximaSuppression2D(int windowSize) 
{
  std::cout << "  DoGScaleSpace::opNonMaximaSuppression2D called. windowSize=" 
       << windowSize << std::endl;
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

DoGScaleSpace buildDoGScaleSpace( OpGrayImage &img, 
                                  int nNumOctaves, int nLevsPerOctave,
                                  float dSigma0 )
{
  return DoGScaleSpace( img, nNumOctaves, nLevsPerOctave, dSigma0 );
}
