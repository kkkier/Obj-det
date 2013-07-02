/*********************************************************************/
/*                                                                   */
/* FILE         cluster.hh                                           */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Define a general cluster class that serves as a ba-  */
/*              sis for derivative classes implementing the specific */
/*              clustering algorithms.                               */
/*                                                                   */
/* BEGIN        Tue Sep 04 2001                                      */
/* LAST CHANGE  Tue Sep 04 2001                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_CLUSTER_HH
#define LEIBE_CLUSTER_HH

using namespace std;
  
/****************/
/*   Includes   */
/****************/
#include <vector>
#include <cassert>

#include <featurevector.hh>

/*******************/
/*   Definitions   */
/*******************/
#ifndef PI
#define PI 3.141592654
#endif

typedef float FLOAT;


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                         Class ClPoint                             */
/*===================================================================*/
/* Define cluster point class */
class ClPoint
{
public:
  ClPoint();
  ClPoint( int dim, FLOAT val=0.0 );
  ClPoint( vector<FLOAT> data );
  ClPoint( const ClPoint &other );
  ~ClPoint();

  ClPoint& operator=( ClPoint other );
  ClPoint& operator=( vector<float> data );
  ClPoint& operator=( vector<double> data );

private:
  void copyFromOther( const ClPoint &other );

public:
  /*******************************/
  /*   Content Access Functions  */
  /*******************************/
  int    dim() const         { return m_data.size(); }

  FLOAT  at( int idx ) const { assert( idx < dim() ); return m_data[idx]; }
  FLOAT& at( int idx )       { assert( idx < dim() ); return m_data[idx]; }

  void   setToNull()         { for( int i=0; i<dim(); i++ ) m_data[i] = 0.0; }
  void   set( int idx, FLOAT val ) 
  { assert( idx < dim() ); m_data[idx] = val; }

  vector<FLOAT> getData() const { return m_data; }

  friend ostream& operator<<( ostream& os, const ClPoint& pt );

public:
  /******************************/
  /*   Manipulation Functions   */
  /******************************/
  ClPoint& add( const ClPoint &other );
  ClPoint& sub( const ClPoint &other );
  ClPoint& mul( const ClPoint &other );
  ClPoint& mul( const FLOAT c );
  ClPoint& div( const ClPoint &other );
  ClPoint& div( const FLOAT c );

  FLOAT distTo( const ClPoint &other ) const;
  FLOAT distSqrTo( const ClPoint &other ) const;
  FLOAT scalarProduct( const ClPoint &other ) const;

private:
  vector<FLOAT> m_data;
};


/****************************/
/*   Associated Functions   */
/****************************/
ostream& operator<<( ostream& os, const ClPoint& pt );


/*===================================================================*/
/*                         Class Cluster                             */
/*===================================================================*/
/* Define a general cluster class */
class Cluster
{
 public:
  Cluster();
  Cluster( const vector<FeatureVector> &vPoints );
  virtual ~Cluster();

protected:
  virtual void init();

public:
  /*******************************/
  /*   Content Access Functions  */
  /*******************************/
  void addPoint( FeatureVector &pt );

  virtual vector<FeatureVector> getClusterCenters() { return m_vCenters; }

public:
  /****************************/
  /*   Clustering Functions   */
  /****************************/
  void  initClusters( int nNumClusters, bool bVerbose=true );
  void  initClusters( vector<FeatureVector> vCenters, bool bVerbose=true );

  virtual bool  doClusterSteps( FLOAT eps, int max_iter, bool bVerbose=true );

protected:
  virtual void  initDataVectors()    = 0;
  virtual void  initClusterCenters() = 0;
  virtual void  initCovariances()    = 0;
  virtual void  initPriors()         = 0;
  virtual void  initPosteriors()     = 0;

  virtual void  doReestimationStep() = 0;
  virtual void  doUpdateStep()       = 0;
  virtual FLOAT calculateError()     = 0;

public:
  /************************/
  /*   Output Functions   */
  /************************/
  virtual void  printResults() = 0;

 protected:
  int                   m_nNumPoints;
  int                   m_nDim;
  vector<FeatureVector> m_vPoints;

  int                   m_nNumClusters;
  vector<FeatureVector> m_vCenters;
  FLOAT                 m_fError;
};


/****************************/
/*   Associated Functions   */
/****************************/
vector<ClPoint>   cnvFeatureVecsToClPoints( const vector<FeatureVector> &vFeatureVecs );
void              cnvClPointsToFeatureVecs( const vector<ClPoint> &vPoints,
                                            vector<FeatureVector> &vFeatureVecs );

// vector<ClPoint>   cnvHistogramsToClPoints( const vector<Histogram> &vHistos );
// void              cnvClPointsToHistograms( const vector<ClPoint> &vPoints,
//                                            vector<Histogram> &vHistos );

#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif

#endif // LEIBE_CLUSTER_HH
