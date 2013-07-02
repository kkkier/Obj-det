/*********************************************************************/
/*                                                                   */
/* FILE         laplacepyramid.hh                                    */
/* AUTHORS      Thomas Hug, modified by Bastian Leibe                */
/* EMAIL        thug@student.ethz.ch, leibe@inf.ethz.ch              */
/*                                                                   */
/* CONTENT      Implementation of a Gaussian/Laplacian Pyramid.      */
/*                                                                   */
/* BEGIN        Mon Dec 02 2002                                      */
/* LAST CHANGE  Mon Dec 02 2002                                      */
/*                                                                   */
/*********************************************************************/


#ifndef LAPLACEPYRAMID_HH
#define LAPLACEPYRAMID_HH

/****************/
/*   Includes   */
/****************/
#include "pyramiddata.hh"
#include "gausspyramid.hh"


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                        Class LaplacePyramid                         */
/*===================================================================*/
/* Define a pyramid scale space class. */
class LaplacePyramid : public GaussPyramid
{
public:
  LaplacePyramid();
  LaplacePyramid(const LaplacePyramid& source);
  LaplacePyramid(const OpGrayImage& source, int level);
  ~LaplacePyramid();
  
public:
  /***************************/
  /*   Data Access Methods   */
  /***************************/
  OpGrayImage  getImage(int l);

public:
  /*****************************/
  /*   Data Access Operators   */
  /*****************************/
  GrayPixel       operator()( const int x, const int y, const int z ) const;
  GrayPixel&      operator()( const int x, const int y, const int z );
  LaplacePyramid& operator= ( LaplacePyramid other );
  
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
  
  OpGrayImage getCharacteristicScaleImg() const;
  float       getCharacteristicScale( int x, int y ) const;
  void        getCharacteristicScale( int x, int y, 
                                      float &scale, float &value );
  
protected:
  PyramidData* differences;
  bool m_bNormalized;
};

#endif
