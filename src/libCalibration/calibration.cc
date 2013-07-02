/*********************************************************************/
/*                                                                   */
/* FILE         calibration.cc                                       */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@vision.ee.ethz.ch                              */
/*                                                                   */
/* CONTENT      Storage class for camera calibration parameters.     */
/*                                                                   */
/* BEGIN        Fri Feb 24 2006                                      */
/* LAST CHANGE  Fri Feb 24 2006                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <fstream>
#include <iostream>
#include <iomanip>
#include <math.h>

#include "calibration.hh"

/*===================================================================*/
/*                        Class Calibration                          */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/
Calibration::Calibration()
  : m_mK(3,3)
  , m_mR(3,3)
  , m_fvKappa(3)
  , m_fvT(3)
  , m_fvGPN(3)
  , m_mKRt(3,3)
  , m_mRKinv(3,3)
  , m_fvKRtT(3)
  , m_fvVUP(3)
  , m_fvVPN(3)
{
  clear();
}


Calibration::Calibration( const Calibration &other )
  : m_mK(3,3)
  , m_mR(3,3)
  , m_fvKappa(3)
  , m_fvT(3)
  , m_fvGPN(3)
  , m_mKRt(3,3)
  , m_mRKinv(3,3)
  , m_fvKRtT(3)
  , m_fvVUP(3)
  , m_fvVPN(3)
{
  copyFrom( other );
}


Calibration& Calibration::operator=( const Calibration &other )
{
  copyFrom( other );
  return *this;
}


void Calibration::copyFrom( const Calibration &other )
{
  m_mK       = other.m_mK;
  m_mR       = other.m_mR;
  m_fvKappa  = other.m_fvKappa;
  m_fvT      = other.m_fvT;
  m_fvGPN    = other.m_fvGPN;
  m_dGPd     = other.m_dGPd;

  m_dScaleI  = other.m_dScaleI;
  m_dScaleW  = other.m_dScaleW;

  m_mKRt     = other.m_mKRt;
  m_mRKinv   = other.m_mRKinv;
  m_fvKRtT   = other.m_fvKRtT;
  m_dNtT     = other.m_dNtT;
  m_fvVUP    = other.m_fvVUP;
  m_fvVPN    = other.m_fvVPN;
  m_dVPd     = other.m_dVPd;

  m_bValid   = other.m_bValid;
}


void Calibration::clear()
{
  m_mK      = createUnitMatrix(3);
  m_mR      = createUnitMatrix(3);
  m_fvKappa = FeatureVector(3);
  m_fvT     = FeatureVector(3);
  m_fvGPN   = FeatureVector(3);
  m_dGPd    = 0.0;
  m_dScaleI = 1.0;
  m_dScaleW = 1.0;

  m_mKRt    = createUnitMatrix(3);
  m_mRKinv  = createUnitMatrix(3);
  m_fvKRtT  = FeatureVector(3);
  m_dNtT    = 0.0;
  m_fvVUP   = FeatureVector(3);
  m_fvVPN   = FeatureVector(3);
  m_dVPd    = 0.0;

  m_bValid   = false; 
}


/***********************************************************/
/*                     Content Access                      */
/***********************************************************/

void Calibration::rescaleWorld( float s )
{
  m_mR  *= s;
  m_fvT *= s;
  m_bValid = false;
}


void Calibration::print()
{
  prepareMatrices();
  cout << "------------------------------" << endl
       << "K=" << m_mK << endl 
       << "kappa=" << m_fvKappa << endl
       << "R=" << m_mR << endl
       << "t=" << m_fvT << endl
       << "GP=" << m_fvGPN << ", d=" << m_dGPd << endl
       << "------------------------------" << endl;
}


/***********************************************************/
/*                  Coordinate Transform                   */
/***********************************************************/

void  Calibration::prepareMatrices()
{
  if( !m_bValid ) {
    if( m_dScaleI==0.0 )
      m_dScaleI = 1.0;
    if( m_dScaleW==0.0 )
      m_dScaleW = 1.0;

    m_mKRt   = m_mK * m_mR.transpose();
    m_mRKinv = m_mR * m_mK.invert();
    m_fvKRtT = m_mKRt * m_fvT;
    m_dNtT   = m_fvGPN.dot( m_fvT );
    
    FeatureVector fvVUP(3); fvVUP.at(1) = 1.0;
    FeatureVector fvVPN(3); fvVPN.at(2) = 1.0;
    m_fvVUP  = m_mR * fvVUP;
    m_fvVPN  = m_mR * fvVPN;
    m_dVPd   = -m_fvVPN.dot(m_fvT);

    m_bValid = true;
  }
}


void  Calibration::img2World( const FeatureVector& x, 
                              FeatureVector& m, FeatureVector &t )
{
  /*-------------------------------------*/
  /* Prepare the transformation matrices */
  /*-------------------------------------*/
  prepareMatrices();

  /*------------------------------------*/
  /* Make sure the image point is valid */
  /*------------------------------------*/
  FeatureVector r(3);
  if( x.numDims()==3 )
    r = x;
  else if( x.numDims()==2 ){
    r.at(0) = x.at(0);
    r.at(1) = x.at(1);
    r.at(2) = 1.0;
  } else {
    cerr << "ERROR in Calibration::img2World(): "
         << "Image point must have either 2 or 3 dimensions!" << endl;
    m.clear();
    t.clear();
    return;
  }
  r.at(0) *= m_dScaleI;
  r.at(1) *= m_dScaleI;

  /*--------------------------*/
  /* Transform the coordinate */
  /*--------------------------*/
  m = m_mRKinv*r;
  t = m_fvT;
}


void  Calibration::world2Img( const FeatureVector& m, FeatureVector& x )
{
  /*-------------------------------------*/
  /* Prepare the transformation matrices */
  /*-------------------------------------*/
  prepareMatrices();

  /*------------------------------------*/
  /* Make sure the world point is valid */
  /*------------------------------------*/
  assert( m.numDims()==3 );

  /*---------------------*/
  /* Transform the point */
  /*---------------------*/
  x = m_mKRt*m - m_fvKRtT;

  /*---------------------------*/
  /* Normalize by w coordinate */
  /*---------------------------*/
  if( x.at(2)==0.0 )
    cerr << "WARNING in Calibration::world2Img(): "
         << "Transformed point is invalid!" << endl;
  else {
    x.at(0) /= x.at(2)*m_dScaleI;
    x.at(1) /= x.at(2)*m_dScaleI;
    x.at(2) = 1.0;
  }
}


void  Calibration::world2Img( const FeatureVector& m, float& x, float& y )
{
  FeatureVector r;
  world2Img( m, r );
  
  x = r.at(0);
  y = r.at(1);
}


bool Calibration::posOnGroundPlane( const FeatureVector& x, FeatureVector &m )
{
  /*-------------------------------------*/
  /* Prepare the transformation matrices */
  /*-------------------------------------*/
  prepareMatrices();

  /*------------------------------------------*/
  /* Transform the point to world coordinates */
  /*------------------------------------------*/
  m.clear();
  FeatureVector r;
  FeatureVector t;
  img2World( x, r, t );

  /*----------------------------------------------------*/
  /* Intersect the resulting line with the ground plane */
  /*----------------------------------------------------*/
  float d1 = m_dNtT + m_dGPd; // !!!CHECK!!! (minus?)
  float d2 = m_fvGPN.dot( r );
  if( d2==0.0 ) {
    cerr << "WARNING in Calibration::posOnGroundPlane(): "
         << "Line stretches to infinity!" << endl;
    return false;

  } else {
    float lambda = -d1/d2;
    m = r*lambda + t;
    return true;
  }
}


float Calibration::distToCamera( const FeatureVector &m )
{
  /*-------------------------------------*/
  /* Prepare the transformation matrices */
  /*-------------------------------------*/
  prepareMatrices();

  /* get the absolute distance to the camera center */
  float dDist = sqrt(m_fvT.compSSD( m ));

  /* get the signed distance to the camera plane */
  float dSigned = (m_fvVPN.dot(m) + m_dVPd);

  if( dSigned < 0 )
    return -dDist;
  else
    return dDist;
}


float Calibration::distFromGround( const FeatureVector &m )
{
  return (m_fvGPN.dot(m)+m_dGPd)/sqrt(m_fvGPN.dot(m_fvGPN));
}


bool Calibration::intersectWithPlane( const FeatureVector& x, 
                                      /*const*/ FeatureVector& n, 
                                      /*const*/ FeatureVector& p,
                                      FeatureVector &m )
{
  /*-------------------------------------*/
  /* Prepare the transformation matrices */
  /*-------------------------------------*/
  prepareMatrices();

  /*------------------------------------------*/
  /* Transform the point to world coordinates */
  /*------------------------------------------*/
  m.clear();
  FeatureVector r;
  FeatureVector t;
  img2World( x, r, t );

  /*-------------------------------*/
  /* Compute the main intersection */
  /*-------------------------------*/
  return intersectWithPlane( r, t, n, p, m );
}


bool Calibration::intersectWithPlane( const FeatureVector& r, 
                                      const FeatureVector& t,
                                      /*const*/ FeatureVector& n, 
                                      /*const*/ FeatureVector& p,
                                      FeatureVector &m )
{
  /*-------------------------------------*/
  /* Prepare the transformation matrices */
  /*-------------------------------------*/
  prepareMatrices();

  /*---------------------------------------------*/
  /* Get the plane with normal n through point p */
  /*---------------------------------------------*/
  float d = -n.dot(p);
  
  /*---------------------------------------------*/
  /* Intersect the ray r*lambda+t with the plane */
  /*---------------------------------------------*/
  float s1 = n.dot(t)+d;
  float s2 = n.dot(r);
  if( s2!=0.0 )
    m = r*(-s1/s2) + t;

  else {
    cerr << "WARNING in Calibration::intersectWithPlane(): "
         << "Line is parallel to plane!" << endl;
    m = FeatureVector(3);
    return false;
  }
  return true;
}


bool Calibration::projectIntoPlane( const FeatureVector& x, 
                                    /*const*/ FeatureVector& n, 
                                    FeatureVector &s )
{
  /*-------------------------------------*/
  /* Prepare the transformation matrices */
  /*-------------------------------------*/
  prepareMatrices();

  /*------------------------------------------*/
  /* Transform the point to world coordinates */
  /*------------------------------------------*/
  s.clear();
  FeatureVector r;
  FeatureVector t;
  img2World( x, r, t );

  /*-------------------------------*/
  /* Compute the main intersection */
  /*-------------------------------*/
  return projectIntoPlane( r, t, n, s );
}


bool Calibration::projectIntoPlane( /*const*/ FeatureVector& r, 
                                    const FeatureVector& t,
                                    /*const*/ FeatureVector& n, 
                                    FeatureVector &s )
{
  /*-------------------------------------*/
  /* Prepare the transformation matrices */
  /*-------------------------------------*/
  prepareMatrices();

  /*-----------------------------------------------*/
  /* Get the projected vector by computing Nx(RxN) */
  /*-----------------------------------------------*/
  s = n.cross( r.cross(n) );
  
  /*---------------------------------------------------*/
  /* Normalize the vector (if the projection is valid) */
  /*---------------------------------------------------*/
  if( !(s.at(0)==0.0 && s.at(1)==0.0 && s.at(2)==0.0) )
    s /= sqrt(s.dot(s));

  else {
    cerr << "WARNING in Calibration::projectIntoPlane(): "
         << "Line is orthogonal to plane!" << endl;
    s = FeatureVector(3);
    return false;
  }
  return true;
}


bool Calibration::projectIntoGroundPlane( const FeatureVector& x, 
                                          FeatureVector &s )
{
  return projectIntoPlane( x, m_fvGPN, s );
}


void Calibration::rotateInPlane( const FeatureVector &n, float dAngle,
                                 const FeatureVector &dir,
                                 FeatureVector &res)
{
  /*---------------------------------------*/
  /* Build up the rotation matrix around n */
  /*---------------------------------------*/
  float nx  = n.at(0);
  float ny  = n.at(1);
  float nz  = n.at(2);

  float c     = cos(dAngle);
  float s     = sin(dAngle);
  float omc   = 1.0-c;
  float nxomc = nx*omc;
  float nyomc = ny*omc;

  FeatureMatrix mRot(3,3);
  mRot(0,0) = c + nx*nxomc;
  mRot(0,1) = nx*nxomc - nz*s;
  mRot(0,2) = ny*s + nz*nxomc;
  mRot(1,0) = -mRot(0,1);
  mRot(1,1) = c + ny*nyomc;
  mRot(1,2) = -nx*s + nz*nyomc;
  mRot(2,0) = -mRot(0,2);
  mRot(2,1) = -mRot(1,2);
  mRot(2,2) = c + nz*nz*omc;

  /*-------------------*/
  /* Rotate the vector */
  /*-------------------*/
  res = mRot*dir;
}


void Calibration::rotateInGroundPlane( float dAngle,
                                       const FeatureVector &dir,
                                       FeatureVector &res)
{
  rotateInPlane( m_fvGPN, dAngle, dir, res );
}


/***********************************************************/
/*                        File I/O                         */
/***********************************************************/

bool Calibration::load( string sFileName )
{
  ifstream ifile( sFileName.c_str() );
  if( !ifile ) {
    cerr << "ERROR: Couldn't open file " << sFileName << "!" << endl;
    return false;
  }

  /* load matrix K */
  for( int r=0; r<3; r++ )
    for( int c=0; c<3; c++ )
      ifile >> m_mK(r,c);

  /* load vector kappa */
  for( int d=0; d<3; d++ )
    ifile >> m_fvKappa.at(d);

  /* load matrix R */
  for( int r=0; r<3; r++ )
    for( int c=0; c<3; c++ )
      ifile >> m_mR(r,c);

  /* load vector t */
  for( int d=0; d<3; d++ )
    ifile >> m_fvT.at(d);

  /* load vector M */
  for( int d=0; d<3; d++ )
    ifile >> m_fvGPN.at(d);
  ifile >> m_dGPd;
  m_dGPd = - m_dGPd;

  ifile.close();

  /* check for NaN (=> GP normal 0) */
  if( m_fvGPN.at(0)==0.0 && m_fvGPN.at(1)==0.0 && m_fvGPN.at(2)==0.0 )
    return false;

  return true;
}


bool Calibration::save( string sFileName )
{
  ofstream ofile( sFileName.c_str() );
  if( !ofile ) {
    cerr << "ERROR: Couldn't open file " << sFileName << "!" << endl;
    return false;
  }

  /* save matrix K */
  for( int r=0; r<3; r++ ) {
    for( int c=0; c<3; c++ )
      ofile << m_mK(r,c) << " ";
    ofile << endl;
  }
  ofile << endl;

  /* save vector kappa */
  for( int d=0; d<3; d++ )
    ofile << m_fvKappa.at(d) << " ";
  ofile << endl << endl;

  /* save matrix R */
  for( int r=0; r<3; r++ ) {
    for( int c=0; c<3; c++ )
      ofile << m_mR(r,c) << " ";
    ofile << endl;
  }
  ofile << endl;

  /* save vector t */
  for( int d=0; d<3; d++ )
    ofile << m_fvT.at(d) << " ";
  ofile << endl << endl;

  /* save vector M */
  for( int d=0; d<3; d++ )
    ofile << m_fvGPN.at(d) << " ";
  ofile << m_dGPd;
  ofile << endl;

  ofile.close();
  return true;
}

