/*********************************************************************/
/*                                                                   */
/* FILE         clemsigma.hh                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Define a clustering class, derived from the general  */
/*              Cluster class, that implements the EM algorithm with */
/*              a per-cluster sigma.                                 */
/*                                                                   */
/* BEGIN        Tue Sep 04 2001                                      */
/* LAST CHANGE  Tue Sep 04 2001                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_CLEMSIGMA_HH
#define LEIBE_CLEMSIGMA_HH

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif
  
/****************/
/*   Includes   */
/****************/
#include <vector>
#include <cassert>

#include "cluster.hh"

/*************************/
/*   Class Definitions   */
/*************************/
/*******************/
/*   Definitions   */
/*******************/
const float EPSILON_PROB  = 0.0001 * 100;
const float EPSILON_SIGMA = 0.002; //* 100


/*===================================================================*/
/*                         Class ClEMSigma                           */
/*===================================================================*/
/* Define an EM sigma clustering class derived from the Cluster class. */
class ClEMSigma : public Cluster
{
public:
  ClEMSigma( vector<FeatureVector> &vPoints ) 
    : Cluster( vPoints ) {}

public:
  /*******************************/
  /*   Content Access Functions  */
  /*******************************/
  vector<FLOAT> getSigmas()     { return m_vSigmas; }

public:
  /****************************/
  /*   Clustering Functions   */
  /****************************/

protected:
  virtual void  initDataVectors();
  void          initClusterCenters();
  virtual void  initCovariances();
  void          initPriors();
  virtual void  initPosteriors();

  virtual void  doReestimationStep();
  void          doUpdateStep();
  FLOAT         calculateError();

  void          updatePriors();
  void          updateClusterCenters();
  virtual void  updateCovariances();

public:
  /************************/
  /*   Output Functions   */
  /************************/
  virtual void  printResults();
  void          printPosteriors();

protected:
  FLOAT regularize( FLOAT val );
   
protected:
  FLOAT evaluateGauss( FeatureVector pt, FeatureVector mean, FLOAT sigma );
  int   clusterIdx( int pt, int c) { return pt*m_nNumClusters + c; }

  vector<FLOAT>   m_vSigmas;
  vector<FLOAT>   m_vPriors;
  vector<FLOAT>   m_vPosteriors;

  FLOAT m_fNormConst;
};

#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif

#endif // LEIBE_EMSIGMA_HH
