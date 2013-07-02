/*********************************************************************/
/*                                                                   */
/* FILE         clfastrnncagglo.hh                                   */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Implements an average-link agglomerative clustering  */
/*              method based on the Reciprocal Nearest Neighbor      */
/*              (RNN) Chain strategy by C. de Rham and P. Benzecri.  */
/*              Details on this algorithm can be found in            */
/*                                                                   */
/*                C. de Rham,                                        */
/*                La Classification Hierarchique Ascendante Selon la */
/*                Methode des Voisins Reciproques.                   */
/*                Les Cahiers de l'Analyse des Donnees, Vol. 5,      */
/*                pp. 135-144, 1980.                                 */
/*                                                                   */
/*                J.P. Benzecri,                                     */
/*                Construction d'une Classification Ascendante Hier- */
/*                archique par la Recherche en Chaine des Voisins    */
/*                Reciproques.                                       */
/*                Les Cahiers de l'Analyse des Donnees, Vol. 7,      */
/*                pp. 209-218, 1982.                                 */
/*                                                                   */
/*              This file contains an improvement to the basic RNNC  */
/*              algorithm that does not need to compute and store an */
/*              explicit O(N^2) distance matrix. As a result, it is  */
/*              extremely efficient and has a worst-case runtime     */
/*              performance of O(N^2) while needing only O(N) space. */
/*              (A further improvement to an expected-time perfor-   */
/*              mance of only O(NlogN) can be easily achieved by in- */
/*              tegrating a fast Nearest-Neighbor search strategy).  */
/*                                                                   */
/*              The algorithm is written for average-link clustering */
/*              with correlation or Euclidean distance as a distance */
/*              measure, but it can be easily extended to any dis-   */
/*              tance measure where the group average criterion      */
/*               d(X,Y)=(1/|X||Y|)sum_{xi in X}sum_{yj in Y}d(xi,yj) */
/*              can be reformulated to only depend on the cluster    */
/*              means (and possibly the variances).                  */
/*                                                                   */
/*              This version of the code contains an additional      */
/*              speedup by implementing the NN-search using a ball-  */
/*              tree search structure.                               */
/*                                                                   */
/* BEGIN        Fri Jun 04 2004                                      */
/* LAST CHANGE  Tue Sep 06 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef CLFASTRNNCAGGLO_HH
#define CLFASTRNNCAGGLO_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <cassert>

#include <balltree.hh>

#include "cluster.hh"
#include "clstep.hh"

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                       Class ClRNNCAgglo                           */
/*===================================================================*/
/* Define an agglomerative clustering class derived from the Cluster */
/* class, based on the similarity between all pairs of clusters.     */
class ClFastRNNCAgglo : public Cluster
{
public:
  ClFastRNNCAgglo( const vector<FeatureVector> &vPoints, 
                   int nMaxNodeSize=-1 ); 

public:
  /*******************************/
  /*   Content Access Functions  */
  /*******************************/
  vector<int>    getClusterAssignment() { return m_vBelongsTo; }
  vector<double> getVariances()         { return m_vVariances; }
  vector<ClStep> getClusterTrace()      { return m_vTrace;     }

  void setMetric( int nMetricType )     { m_nMetricType = nMetricType; }
  int  getMetric() const                { return m_nMetricType; }

public:
  /****************************/
  /*   Clustering Functions   */
  /****************************/
  bool  doClusterSteps( FLOAT eps, int max_iter, bool bVerbose=true ) {
    cout << "For class type 'ClFastRNNCAgglo' please use the "
         << "'doClusterSteps( double minSimilarity )' function." << endl;
    return false;
  }
  bool  doClusterSteps( double minSimilarity, bool bVerbose=true );

protected:
  float sim       ( int idx1, int idx2 );
  void agglomerate( int idx1, int idx2, int newidx );

  void writeTrace ( int nIdx1, int nIdx2, float dSim, int nNewIdx )
  {
    m_vTrace.push_back( ClStep(nIdx1,nIdx2,dSim,nNewIdx) );
  }

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

private:
  bool     m_bMore;
  double   m_dMinSimilarity;

  int      m_nMetricType;

  vector<double>         m_vVariances;
  vector<int>            m_vBelongsTo;
  vector<bool>           m_vValid;
  vector< vector<int> >  m_vvAllBelongingFV;

  vector<ClStep>         m_vTrace;

  BallTree               m_btNNTree;
  int                    m_nMaxNodeSize;
};


#endif
