/*********************************************************************/
/*                                                                   */
/* FILE         clemfull.hh                                          */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Define a clustering class, derived from the general  */
/*              Cluster class, that implements the EM algorithm with */
/*              a full covariance matrix.                            */
/*                                                                   */
/* BEGIN        Thu Dec 18 2003                                      */
/* LAST CHANGE  Thu Dec 18 2003                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_CLEMFULL_HH
#define LEIBE_CLEMFULL_HH

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif
  
/****************/
/*   Includes   */
/****************/
#include <vector>
#include <cassert>

#include "cluster.hh"
#include "matrix.hh"

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                         Class ClEMFull                            */
/*===================================================================*/
/* Define an EM clustering class with diagonal covariance matrix,    */
/* derived from the EM sigma cluster class.                          */
class ClEMFull : public Cluster
{
public:
  ClEMFull( vector<FeatureVector> &vPoints ) 
    : ClEMSigma( vPoints ) {}
  
public:
  /*******************************/
  /*   Content Access Functions  */
  /*******************************/
  Matrix        getCovariances()     { return m_mCov; }
  vector<int>   getClusterAssignment();


public:
  /****************************/
  /*   Clustering Functions   */
  /****************************/

protected:
  virtual void  initDataVectors();
  virtual void  initCovariances();
  virtual void  initPosteriors();

  virtual void  doReestimationStep();
  FLOAT         calculateError();

  virtual void  updateCovariances();

public:
  /************************/
  /*   Output Functions   */
  /************************/
  virtual void  printResults();
  void          printPosteriors();

private:
  FLOAT evaluateGauss( FeatureVector pt, FeatureVector mean, 
                       vector<FLOAT> cov, int idx );
  int   covIdx( int nr, int x, int y ) 
  { return nr*m_nDim*m_nDim + y*m_nDim + x; }
  
  Matrix   m_mCov;
  vector<FLOAT>   m_vPriors;
  vector<FLOAT>   m_vPosteriors;
};

#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif

#endif // LEIBE_EMDiag_HH
