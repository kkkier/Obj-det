/*********************************************************************/
/*                                                                   */
/* FILE         clpostagglo.hh                                       */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Implements an agglomerative clustering method basing */
/*              merging decisions on the predicted intra-cluster si- */
/*              milarity after the merging step.                     */
/*                                                                   */
/*                                                                   */
/* BEGIN        Tue Oct 22 2002                                      */
/* LAST CHANGE  Fri Feb 07 2003                                      */
/*                                                                   */
/*********************************************************************/

#ifndef CLPOSTAGGLO_HH
#define CLPOSTAGGLO_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <cassert>
#include <queue>

#include "cluster.hh"
#include "clsimilarity.hh"
#include "clstep.hh"

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                       Class ClSimAgglomerative                    */
/*===================================================================*/
/* Define an agglomerative clustering class derived from the Cluster */
/* class, based on the similarity between all pairs of clusters.     */
class ClPostAgglo : public Cluster
{
public:
  ClPostAgglo( const vector<FeatureVector> &vPoints, 
               const vector<float> &mSimMatrix ); 

public:
  /*******************************/
  /*   Content Access Functions  */
  /*******************************/
  vector<int>    getClusterAssignment() { return m_vBelongsTo; }
  vector<ClStep> getClusterTrace()      { return m_vTrace;     }

public:
  /****************************/
  /*   Clustering Functions   */
  /****************************/
  bool  doClusterSteps( FLOAT eps, int max_iter, bool bVerbose=true ) {
    cout << "For class type 'SimAgglomerative' please use the "
         << "'doClusterSteps( double minSimilarity )' function." << endl;
    return false;
  }
  bool  doClusterSteps( double minSimilarity, bool bVerbose=true );

protected:
  virtual void  initDataVectors();
  virtual void  initClusterCenters();
  virtual void  initCovariances();
  virtual void  initPriors();
  virtual void  initPosteriors();

  virtual void  doReestimationStep();
  virtual void  doUpdateStep();
  virtual FLOAT calculateError(); //is a double

public:
  /************************/
  /*   Output Functions   */
  /************************/
  virtual void  printResults();

protected:
  void writeTrace ( int nIdx1, int nIdx2, float dSim, int nNewIdx )
  {
    m_vTrace.push_back( ClStep(nIdx1,nIdx2,dSim,nNewIdx) );
  }

private:
  bool     m_bMore;
  int      m_nSimMatrixDim;
  double   m_dMinSimilarity;

  vector<float>          m_mSimMatrix;
  vector<int>            m_vBelongsTo;
  vector<bool>           m_vValid;
  vector<double>         m_vSqNormalizedTerms;
  vector<FeatureVector>  m_vNormalizedPoints;
  vector< vector<int> >  m_vvAllBelongingFV;

  priority_queue<ClSimilarity, vector<ClSimilarity>, ClSimCompare> *m_pqSimQueue;

  vector<ClStep>         m_vTrace;
};


#endif
