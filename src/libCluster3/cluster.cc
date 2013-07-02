/*********************************************************************/
/*                                                                   */
/* FILE         cluster.cc                                           */
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

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif
  
/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>

#include <math.h>

#include "cluster.hh"

/*===================================================================*/
/*                         Class ClPoint                             */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

ClPoint::ClPoint()
  /* standard constructor */
{
  m_data.clear();
}


ClPoint::ClPoint( int dim, FLOAT value )
  /* alternate constructor */
{
  m_data.resize( dim );
  for( int i=0; i<dim; i++ )
    m_data[i] = value;
}


ClPoint::ClPoint( vector<FLOAT> data )
  /* alternate constructor */
{
  m_data = data;
}


ClPoint::ClPoint( const ClPoint &other )
  /* copy constructor */
{
  copyFromOther( other );
}


ClPoint& ClPoint::operator=( ClPoint other )
  /* assignment operator */
{
  copyFromOther( other );
  return *this;
}


ClPoint& ClPoint::operator=( vector<float> data )
  /* assignment operator */
{
  m_data.resize( data.size() );
  for(unsigned i=0; i<data.size(); i++ )
    m_data[i] = (FLOAT) data[i];
  return *this;
}


ClPoint& ClPoint::operator=( vector<double> data )
  /* assignment operator */
{
  m_data.resize( data.size() );
  for(unsigned i=0; i<data.size(); i++ )
    m_data[i] = (FLOAT) data[i];
  return *this;
}


ClPoint::~ClPoint()
  /* standard destructor */
{
}


void ClPoint::copyFromOther( const ClPoint &other )
  /* Auxiliary function to copy from another ClPoint.                */
{
  m_data = other.m_data;
}


/***********************************************************/
/*                 Manipulation Functions                  */
/***********************************************************/

ClPoint& ClPoint::add( const ClPoint &other )
  /* add the value of another ClPoint */
{
  assert( dim() == other.dim() );

  for( int i=0; i<dim(); i++ )
    m_data[i] += other.m_data[i];
  return *this;
}


ClPoint& ClPoint::sub( const ClPoint &other )
  /* subtract the value of another ClPoint */
{
  assert( dim() == other.dim() );

  for( int i=0; i<dim(); i++ )
    m_data[i] -= other.m_data[i];
  return *this;
}


ClPoint& ClPoint::mul( const ClPoint &other )
  /* multiply the value of another ClPoint */
{
  assert( dim() == other.dim() );

  for( int i=0; i<dim(); i++ )
    m_data[i] *= other.m_data[i];
  return *this;
}


ClPoint& ClPoint::mul( const FLOAT c )
  /* multiply with a constant */
{
  for( int i=0; i<dim(); i++ )
    m_data[i] *= c;
  return *this;
}


ClPoint& ClPoint::div( const ClPoint &other )
  /* divide by the value of another ClPoint */
{
  assert( dim() == other.dim() );

  for( int i=0; i<dim(); i++ )
    m_data[i] /= other.m_data[i];
  return *this;
}


ClPoint& ClPoint::div( const FLOAT c )
  /* divide by a constant */
{
  FLOAT d = 1.0/c;
  for( int i=0; i<dim(); i++ )
    m_data[i] *= d;
  return *this;
}


FLOAT ClPoint::distTo( const ClPoint &other ) const
  /* compute the distance to another ClPoint */
{ return sqrt( distSqrTo( other ) ); }


FLOAT ClPoint::distSqrTo( const ClPoint &other ) const
  /* compute the squared distance to another ClPoint */
{
  assert( dim() == other.dim() );
  
  FLOAT sum = 0.0;
  for (int i=0; i<dim(); i++) {
    FLOAT dist = m_data[i] - other.m_data[i];
    sum += dist*dist;
  }
  return sum;
}


FLOAT ClPoint::scalarProduct( const ClPoint &other ) const
  /* compute the scalar product with another ClPoint */
{
  assert( dim() == other.dim() );
  
  FLOAT sum = 0.0;
  for (int i=0; i<dim(); i++) {
    sum += m_data[i] *other.m_data[i];
  }
  return sum;
}


/***********************************************************/
/*                  Associated Functions                   */
/***********************************************************/

ostream& operator<<( ostream& os, const ClPoint& pt )
  /* print operator */
{
  os << "(";
  for( int i=0; i<pt.dim()-1; i++ )
    os << setw(5) << setprecision(4) << pt.m_data[i] << ",";
  os << setw(5) << setprecision(4) << pt.m_data[pt.dim()-1] << ")";

  return os;
}


/*===================================================================*/
/*                         Class Cluster                             */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

Cluster::Cluster()
  /* standard constructor */
{
  m_nNumPoints = 0;
  m_nDim = 0;
  m_vPoints.clear();

  init();
}


Cluster::Cluster( const vector<FeatureVector> &vPoints )
  /* alternate constructor */
{
  assert( vPoints.size() >= 1 );

  m_nNumPoints = vPoints.size();
  m_nDim = vPoints[0].numDims();
  /* check if all the points have the same dimensionality */
  for( int i=1; i<m_nNumPoints; i++ ) {
    assert( vPoints[i].numDims() == m_nDim );
  }

  m_vPoints = vPoints;

  init();
}


Cluster::~Cluster()
  /* standard destructor */
{
  m_vPoints.clear();
  m_vCenters.clear();
}


void Cluster::init()
{
  m_nNumClusters = 0;
  m_vCenters.clear();
  m_fError = 0.0;
}


/***********************************************************/
/*                    Access Functions                     */
/***********************************************************/

void Cluster::addPoint( FeatureVector &pt )
  /* add a point to the list of points to be clustered. */
{
  if( m_nNumPoints == 0 )
    /* add the first point */
    m_nDim = pt.numDims();
  else {
    /* check for consistency */
    assert( pt.numDims() == m_nDim );
  }

  m_vPoints.push_back( pt );
  m_nNumPoints++;
}


/***********************************************************/
/*                   Clustering Functions                  */
/***********************************************************/

void Cluster::initClusters( int nNumClusters, bool bVerbose )
  /* initialize the clustering algorithm with random cluster centers */
{
  if( bVerbose )
    cout << "    Cluster::initClusters(" << nNumClusters << ") called."
         << endl;

  m_nNumClusters = nNumClusters;;

  if( bVerbose )
    cout << "      Resizing the data vectors..." << endl;
  initDataVectors();

  /**********************************/
  /* Initialize the cluster centers */
  /**********************************/
  if( bVerbose )
    cout << "      Initializing the cluster centers..." << endl;
  initClusterCenters();

  /*******************************/
  /* Initialize the sigma values */
  /*******************************/
  if( bVerbose )
    cout << "      Initializing the sigma values..." << endl;
  initCovariances();

  /*********************************/
  /* Initialize the cluster priors */
  /*********************************/
  if( bVerbose )
    cout << "      Initializing the cluster priors..." << endl;
  initPriors();
  
  /******************************************/
  /* Initialize the posterior probabilities */
  /******************************************/
  if( bVerbose )
    cout << "      Initializing the posterior probabilities..." << endl;
  initPosteriors();
  
  /***********************/
  /* Calculate the error */
  /***********************/
  if( bVerbose )
    cout << "      Calculating the error..." << endl;
  m_fError = calculateError();

  if( bVerbose )
    cout << "    done." << endl;
}


void Cluster::initClusters( vector<FeatureVector> vCenters, bool bVerbose )
  /* initialize the clustering algorithm with given cluster centers. */
{
  if( bVerbose )
    cout << "    Cluster::initClusters(" << vCenters.size() << ") called"
         << " with given cluster centers." << endl;

  m_nNumClusters = vCenters.size();

  if( bVerbose )
    cout << "      Resizing the data vectors..." << endl;
  initDataVectors();

  /**********************************/
  /* Initialize the cluster centers */
  /**********************************/
  if( bVerbose )
    cout << "      Copying the cluster centers..." << endl;
  m_vCenters = vCenters;

  /*******************************/
  /* Initialize the sigma values */
  /*******************************/
  if( bVerbose )
    cout << "      Initializing the sigma values..." << endl;
  initCovariances();

  /*********************************/
  /* Initialize the cluster priors */
  /*********************************/
  if( bVerbose )
    cout << "      Initializing the cluster priors..." << endl;
  initPriors();
  
  /******************************************/
  /* Initialize the posterior probabilities */
  /******************************************/
  if( bVerbose )
    cout << "      Initializing the posterior probabilities..." << endl;
  initPosteriors();
  
  /***********************/
  /* Calculate the error */
  /***********************/
  if( bVerbose )
    cout << "      Calculating the error..." << endl;
  m_fError = calculateError();

  if( bVerbose )
    cout << "    done." << endl;
}


bool Cluster::doClusterSteps( FLOAT eps, int max_iter, bool bVerbose )
  /* Iterate the clustering algorithm until either the error doesn't */
  /* decrease by more than eps anymore, or a maximum number of ite-  */
  /* rations is reached. Returns true if the convergence limit is    */
  /* reached, else false.                                            */
{
  int   iter = 0;
  FLOAT old_error = 999999999.9;
  
  if( bVerbose )
    cout << "    Cluster::doClusterSteps(" << eps << "," << max_iter 
         << ") called." << endl;

  /************************************/
  /* Iterate the clustering procedure */
  /************************************/
  iter = 0;
  while ( (iter < max_iter) && ((iter<1) || (old_error - m_fError > eps)) ) {
    iter++;
    old_error = m_fError;

    /****************************************************/
    /* assign every point to the nearest cluster center */
    /****************************************************/
    doReestimationStep();
    
    /******************************/
    /* update the cluster centers */
    /******************************/
    doUpdateStep();

    /*************************/
    /* recalculate the error */
    /*************************/
    m_fError = calculateError();

    if( bVerbose )
      cout << "      Iteration " << setw(3) << iter 
           << ": error = " << setprecision(6) << m_fError 
           << ", difference = " << setprecision(6)
           << old_error - m_fError << endl;
  } /* end while */

  if (old_error - m_fError > eps)
    return false;
  else
    return true;
}


/***********************************************************/
/*                  Associated Functions                   */
/***********************************************************/

vector<ClPoint> cnvFeatureVecsToClPoints( const vector<FeatureVector> &vFeatureVecs )
  /* Convert the feature vector data to a vector of cluster points.  */
{
  vector<ClPoint> vPoints( vFeatureVecs.size() );

  for(unsigned i=0; i<vFeatureVecs.size(); i++ ) {
    vPoints[i] = vFeatureVecs[i].getData();
  }
  return vPoints;
}


void cnvClPointsToFeatureVecs( const vector<ClPoint> &vPoints,
                               vector<FeatureVector> &vFeatureVecs )
  /* Copy the cluster points to a vector of (pre-initialized)        */
  /* feature vectors.                                                */
{
  //assert( vPoints.size() == vFeatureVecs.size() );
  vFeatureVecs.clear();

  for(unsigned i=0; i<vPoints.size(); i++ )
    //vFeatureVecs[i].setData( vPoints[i].getData() );
    vFeatureVecs.push_back( vPoints[i].getData() );
}


// vector<ClPoint> cnvHistogramsToClPoints( const vector<Histogram> &vHistos )
//   /* Convert the histogram data to a vector of cluster points.       */
// {
//   vector<ClPoint> vPoints( vHistos.size() );

//   for( int i=0; i<vHistos.size(); i++ ) {
//     vPoints[i] = vHistos[i].getData();
//   }
//   return vPoints;
// }


// void cnvClPointsToHistograms( const vector<ClPoint> &vPoints,
// 			      vector<Histogram> &vHistos )
//   /* Copy the cluster points to a vector of (pre-initialized) histo- */
//   /* grams.                                                          */
// {
//   assert( vPoints.size() == vHistos.size() );

//   for( int i=0; i<vPoints.size(); i++ )
//     vHistos[i].setData( vPoints[i].getData() );
// }


#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif
