/*********************************************************************/
/*                                                                   */
/* FILE         clem.hh                                              */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Define a clustering class, derived from the general  */
/*              Cluster class, that serves as a parent class for all */
/*              EM implementations.                                  */
/*                                                                   */
/* BEGIN        Fri Jan 02 2004                                      */
/* LAST CHANGE  Fri Jan 02 2004                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_CLEM_HH
#define LEIBE_CLEM_HH

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <cassert>

#include <featurematrix.hh>
#include "cluster.hh"

/*************************/
/*   Class Definitions   */
/*************************/
const float  EPSILON_SIGMA = 0.00001;
const float  EPSILON_PROB  = EPSILON_SIGMA; // * 100;
const float  EPSILON_COV   = 1.0;
const float  FRAC          = 100.0;

/*===================================================================*/
/*                           Class ClEM                              */
/*===================================================================*/
/* Define a general  EM parent class derived from the Cluster class. */
class ClEM : public Cluster
{
public:
  ClEM( vector<FeatureVector> &vPoints ) 
    : Cluster( vPoints ) {}

public:
  /*******************************/
  /*   Content Access Functions  */
  /*******************************/
  virtual vector<FeatureVector> getClusterCenters() { return m_vCenters; }
  vector<FeatureMatrix>         getCovariances()    { return m_vSigmas; }
  vector<int>   getClusterAssignment();

public:
  /****************************/
  /*   Clustering Functions   */
  /****************************/
  FLOAT         calculateError();

protected:
  virtual FLOAT evaluateGauss( FeatureVector pt, FeatureVector mean, 
                               FeatureMatrix mSigma);

  virtual void  initDataVectors();
  void          initClusterCenters();
  virtual void  initCovariances();
  void          initPriors();
  virtual void  initPosteriors();

  virtual void  doReestimationStep();
  void          doUpdateStep();

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
  int   clusterIdx( int pt, int c) { return pt*m_nNumClusters + c; }

  vector<FeatureMatrix>  m_vSigmas;
  vector<FLOAT>          m_vPriors;
  vector<FLOAT>          m_vPosteriors;
  FeatureMatrix          m_mRegul;

  FLOAT m_fNormConst;
};

#endif
