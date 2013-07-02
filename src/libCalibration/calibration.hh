/*********************************************************************/
/*                                                                   */
/* FILE         calibration.hh                                       */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@vision.ee.ethz.ch                              */
/*                                                                   */
/* CONTENT      Storage class for camera calibration parameters.     */
/*                                                                   */
/* BEGIN        Fri Feb 24 2006                                      */
/* LAST CHANGE  Fri Feb 24 2006                                      */
/*                                                                   */
/*********************************************************************/


#ifndef CALIBRATION_HH
#define CALIBRATION_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <string>
#include <vector>

#include <featurevector.hh>
#include <featurematrix.hh>

/*******************/
/*   Definitions   */
/*******************/

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                         Class Calibration                         */
/*===================================================================*/
class Calibration
{
public:
  Calibration();
  Calibration( const Calibration &other );

  Calibration& operator=( const Calibration &other );

protected:
  void copyFrom( const Calibration &other );

public:
  /**********************/
  /*   Content Access   */
  /**********************/
  FeatureMatrix& getK()     { return m_mK; }
  FeatureMatrix& getR()     { return m_mR; }
  FeatureVector& getKappa() { return m_fvKappa; }
  FeatureVector& getT()     { return m_fvT; }
  FeatureVector& getGPN()   { return m_fvGPN; }
  float          getGPd()   { return m_dGPd; }
  FeatureVector& getVUP()   { prepareMatrices(); return m_fvVUP; }
  FeatureVector& getVPN()   { prepareMatrices(); return m_fvVPN; }

  void setK    ( const FeatureMatrix& mK )  { m_mK = mK; m_bValid=false; }
  void setR    ( const FeatureMatrix& mR )  { m_mR = mR; m_bValid=false; }
  void setKappa( const FeatureVector& fvKappa ) 
  { m_fvKappa = fvKappa; m_bValid=false;}
  void setT    ( const FeatureVector& fvT ) { m_fvT = fvT; m_bValid=false; }
  void setGPN  ( const FeatureVector& fvN ) { m_fvGPN = fvN; m_bValid=false; }
  void setGPd  ( float d )                  { m_dGPd = d; m_bValid=false; }

  float getImgScale  ()          { return m_dScaleI; }
  void  setImgScale  ( float s ) { m_dScaleI=s; m_bValid=false; }

  float getWorldScale()          { return m_dScaleW; }
  void  setWorldScale( float s ) { m_dScaleW=s; m_bValid=false; }

  void rescaleWorld  ( float s );

  void clear();
  void print();

public:
  /*****************************/
  /*   Coordinate Transforms   */
  /*****************************/  
  void  img2World        ( const FeatureVector& x, 
                           FeatureVector& m, FeatureVector &t );
  void  world2Img        ( const FeatureVector& m, FeatureVector& x );
  void  world2Img        ( const FeatureVector& m, float& x, float& y );

  bool  posOnGroundPlane ( const FeatureVector& x, FeatureVector& m );
  float distToCamera     ( const FeatureVector& m );
  float distFromGround   ( const FeatureVector& m );

  /*******************************/
  /*   Geometry Tool Functions   */
  /*******************************/  
  bool  intersectWithPlane( const FeatureVector& x,
                            /*const*/ FeatureVector& n, 
                            /*const*/ FeatureVector& p,
                            FeatureVector &m );
  bool  intersectWithPlane( const FeatureVector& r, const FeatureVector& t,
                            /*const*/ FeatureVector& n, 
                            /*const*/ FeatureVector& p,
                            FeatureVector &m );

  bool  projectIntoPlane  ( const FeatureVector& x,
                            /*const*/ FeatureVector& n,
                            FeatureVector &s );
  bool  projectIntoPlane  ( /*const*/ FeatureVector& r, const FeatureVector &t,
                            /*const*/ FeatureVector& n,
                            FeatureVector &s );
  bool  projectIntoGroundPlane( const FeatureVector& x,
                                FeatureVector &s );

  void  rotateInPlane         ( const FeatureVector &n, float dAngle,
                                const FeatureVector& dir,
                                FeatureVector &res );
  void  rotateInGroundPlane   ( float dAngle,
                                const FeatureVector& dir,
                                FeatureVector &res );
 
protected:
  void prepareMatrices   ();

public:
  /**************************/
  /*   File I/O Functions   */
  /**************************/
  bool load( string sFileName );
  bool save( string sFileName );
  
protected:
  FeatureMatrix m_mK;
  FeatureMatrix m_mR;
  FeatureVector m_fvKappa;
  FeatureVector m_fvT;
  FeatureVector m_fvGPN;
  float         m_dGPd;

  FeatureMatrix m_mKRt;
  FeatureMatrix m_mRKinv;
  FeatureVector m_fvKRtT;
  float         m_dNtT;

  FeatureVector m_fvVUP;
  FeatureVector m_fvVPN;
  float         m_dVPd;

  float         m_dScaleI;
  float         m_dScaleW;

  bool m_bValid;
};

#endif
