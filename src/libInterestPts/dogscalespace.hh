/*********************************************************************/
/*                                                                   */
/* FILE         dogscalespace.hh                                     */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Implementation of an image scale space.              */
/*                                                                   */
/* BEGIN        Mon Dec 15 2003                                      */
/* LAST CHANGE  Mon Dec 15 2003                                      */
/*                                                                   */
/*********************************************************************/

#ifndef DOGSCALESPACE_HH
#define DOGSCALESPACE_HH


/****************/
/*   Includes   */
/****************/
#include <opgrayimage.hh>
#include "opinterestimage.hh"
#include <scalespacedata.hh>


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                        Class DoGScaleSpace                        */
/*===================================================================*/
/* Define a scale space class. */
class DoGScaleSpace {
public:
  DoGScaleSpace();
  DoGScaleSpace( const DoGScaleSpace& source );
  DoGScaleSpace( /*const*/ OpGrayImage& imgSource, int nNumOctaves, 
                 int nLevelsPerOctave, float dSigma0 );
  ~DoGScaleSpace();
  
public:
  /***************************/
  /*   Data Access Methods   */
  /***************************/
  int   getNumLevels() const     { return m_nNumLevels; }

  float getScaleAtLevel(int level) const;
  int	  getLevelAtScale(float scale) const;
  float getLevel() const;
  float getCharacteristicScale ( int x, int y, bool bInterpolate=false ) const;
  void  getCharacteristicScales( int x, int y, 
                                 std::vector<float> &vScales, 
                                 std::vector<float> &vValues,
                                 bool bInterpolate=false ) const;

  PointVector getScaleMaxima3D ( float dThreshold, 
                                 bool  bInterpolate,
                                 bool  bRemoveEdges=false,
                                 float dEdgeThresh=10.0 ) const ;

  OpGrayImage& getImage(int l);

public:
  /*****************************/
  /*   Data Access Operators   */
  /*****************************/
  GrayPixel   operator()(const int x, const int y, const int z) const;
  GrayPixel&  operator()(const int x, const int y, const int z);
  DoGScaleSpace& operator=( const DoGScaleSpace &other);
  
public:
  /*********************************/
  /*   Operators on Scale Levels   */
  /*********************************/
  DoGScaleSpace& opSquareGradient();
  DoGScaleSpace& opLaplacian();
  DoGScaleSpace& opDifferenceOfGaussian();
  DoGScaleSpace& opNormalize(const int order=1);
  DoGScaleSpace& opNonMaximaSuppression2D(const int windowSize=3);
  DoGScaleSpace& opNonMaximaSuppression3D(const int cubeSize=3);
    
  bool isMaximum3D(int x, int y, int z, int dist=1) const;
  bool isMinimum3D(int x, int y, int z, int dist=1) const;
  bool isExtremum3D(int x, int y, int z, int dist=1) const;
  bool isMaximum2D(int x, int y, int z, int dist=1) const;

  bool isNotOnEdge(int x, int y, int z, float dEdgeThresh) const;
  
public:
  class OutOfBoundaryException {};

protected:
  void interpolateScales( float scale1, float val1,
                          float scale2, float val2,
                          float scale3, float val3,
                          float &scale, float &value ) const;

public:
  ScaleSpaceData* data;  
  float  m_dSigmaStep;
  int    m_nNumLevels;
};

//-----------------------------------------------------------------------------
// Related Functions
//-----------------------------------------------------------------------------

DoGScaleSpace buildDoGScaleSpace( OpGrayImage &img, 
                                  int nNumOctaves, int nLevsPerOctave,
                                  float dSigma0 );

//#ifdef _USE_PERSONAL_NAMESPACES
//}
//#endif

#endif
