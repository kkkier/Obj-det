/*********************************************************************/
/*                                                                   */
/* FILE         clemdiag.hh                                          */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Define a clustering class, derived from the general  */
/*              Cluster class, that implements the EM algorithm with */
/*              a diagonal covariance matrix.                        */
/*                                                                   */
/* BEGIN        Tue Sep 04 2001                                      */
/* LAST CHANGE  Tue Sep 04 2001                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_CLEMDIAG_HH
#define LEIBE_CLEMDIAG_HH

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif
  
/****************/
/*   Includes   */
/****************/
#include <vector>
#include <cassert>

#include "cluster.hh"
#include "clemsigma.hh"

/*******************/
/*   Definitions   */
/*******************/

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                         Class ClEMDiag                            */
/*===================================================================*/
/* Define an EM clustering class with diagonal covariance matrix,    */
/* derived from the EM sigma cluster class.                          */
class ClEMDiag : public ClEMSigma
{
public:
  ClEMDiag( vector<FeatureVector> &vPoints ) 
    : ClEMSigma( vPoints ) {}

public:
  /*******************************/
  /*   Content Access Functions  */
  /*******************************/
  vector<FLOAT> getCovariances()     { return m_vCovMatrix; }
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

  virtual void  updateCovariances();

public:
  /************************/
  /*   Output Functions   */
  /************************/
  virtual void  printResults();

private:
  FLOAT evaluateGauss( FeatureVector pt, FeatureVector mean, 
                       vector<FLOAT> cov, int idx );
  int   covIdx( int nr, int x, int y ) 
  { return nr*m_nDim*m_nDim + y*m_nDim + x; }
  
  vector<FLOAT>   m_vCovMatrix;
};

#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif

#endif // LEIBE_EMDiag_HH
