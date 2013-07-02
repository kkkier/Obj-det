/*********************************************************************/
/*                                                                   */
/* FILE         clkmeans.cc                                          */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Define a clustering class, derived from the general  */
/*              Cluster class, that implements a simple k-means al-  */
/*              gorithm.                                             */
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
#include <stdlib.h>

#include "clkmeans.hh"


/*===================================================================*/
/*                         Class ClKMeans                            */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

/***********************************************************/
/*                   Clustering Functions                  */
/***********************************************************/

/*---------------------------------------------------------*/
/*                    k-means Clustering                   */
/*---------------------------------------------------------*/

/*---------------------------------------------------------*/
/*                     Initialization                      */
/*---------------------------------------------------------*/

void ClKMeans::initDataVectors()
{
  m_vCenters.resize( m_nNumClusters );
  m_vBelongsTo.resize( m_nNumPoints );
}


void ClKMeans::initClusterCenters()
{
  /* set the cluster centers to randomly selected points */
  for ( int i=0; i < m_nNumClusters; i++ ) {
    int idx = (int) floor(((FLOAT)m_nNumPoints)*rand()/(RAND_MAX+1.0));
    m_vCenters[i] = m_vPoints[idx];
  } 
}


void ClKMeans::initCovariances()
{}


void ClKMeans::initPriors()
{}


void ClKMeans::initPosteriors()
{
  doReestimationStep();
}


/*---------------------------------------------------------*/
/*                      Reestimation                       */
/*---------------------------------------------------------*/

void ClKMeans::doReestimationStep()
{
  /****************************************************/
  /* assign every point to the nearest cluster center */
  /****************************************************/
  FLOAT min_dist;
  int   min_idx;
  for ( int i=0; i < m_nNumPoints; i++ ) {
    FeatureVector &pt = m_vPoints[i]; 
    
    /* calculate the min distance to every cluster center */
    min_dist = pt.compSSD( m_vCenters[0] );
    min_idx  = 0;
    for ( int j=0; j < m_nNumClusters; j++ ) {
      FLOAT dist = pt.compSSD( m_vCenters[j] );
      if ( dist < min_dist ) {
        min_dist = dist;
        min_idx  = j;
      }
    }
    
    /* update the cluster assignment */
    m_vBelongsTo[i] = min_idx;      
  }
}


/*---------------------------------------------------------*/
/*                         Update                          */
/*---------------------------------------------------------*/

void ClKMeans::doUpdateStep()
{
  /******************************/
  /* update the cluster centers */
  /******************************/
  for ( int j=0; j < m_nNumClusters; j++ ) {

    /* calculate the mean over all member points */
    FeatureVector fvMean( m_nDim );
    FLOAT max_dist = 0.0;
    int   nCount = 0;
    for ( int i=0; i < m_nNumPoints; i++ )
      if ( m_vBelongsTo[i] == j ) {
        fvMean.addVector( m_vPoints[i] );
        nCount++;
	
        //FLOAT dist = m_vCenters[j].compSSD( m_vPoints[i] );
        //if ( dist > max_dist )
        //  max_dist = dist;
      }
    
    if( nCount > 0 )
      m_vCenters[j] = fvMean.div( (FLOAT) nCount );
    
  }
}


/*---------------------------------------------------------*/
/*                     Error Estimation                    */
/*---------------------------------------------------------*/

FLOAT ClKMeans::calculateError()
{
  /* calculate the error */
  FLOAT error = 0.0;
  for ( int i=0; i < m_nNumPoints; i++ ) {
    /* for every point, calculate the distance to the cluster center */
    error += m_vPoints[i].compSSD( m_vCenters[m_vBelongsTo[i]] );
  }
  return sqrt(error);
}


/***********************************************************/
/*                     Output Functions                    */
/***********************************************************/

void ClKMeans::printResults()
{
  //cout << "  ClKMeans::printResults() called." << endl;

  /* calculate the number of points in each cluster */
  vector<int> nPointsInCluster( m_nNumClusters, 0 );
  for( int i=0; i<m_nNumPoints; i++ )
    nPointsInCluster[ m_vBelongsTo[i] ]++;

  for (int j=0; j<m_nNumClusters; j++) {
    cout << "    Cluster " << setw(2) << j << ":";
    if ( m_nDim <= 3 )
      cout << " mean=" << m_vCenters[j] << ",";
    cout << " points=" << setw(5) << nPointsInCluster[j] << endl;
  }
  cout << "  done." << endl;
}



#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif
