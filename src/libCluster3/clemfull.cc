/*********************************************************************/
/*                                                                   */
/* FILE         clemfull.cc                                          */
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

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif
  
/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>

#include <math.h>

#include "clemfull.hh"


/*===================================================================*/
/*                         Class ClEMFull                            */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

/***********************************************************/
/*                   Clustering Functions                  */
/***********************************************************/

/*---------------------------------------------------------*/
/*        EM clustering with full covariance matrix        */
/*---------------------------------------------------------*/

FLOAT ClEMFull::evaluateGauss( FeatureVector fvPt, FeatureVector fvMean, 
                               vector<FLOAT> cov, 
                               int idx )
  /* evaluates the standard Normal distribution function for x:      */
  /* p(x) = 1/(2*pi*sigma^2)^d/2*exp(-(x-mean)^2/2*sigma^2)          */
{
  FLOAT sigma2, exponent, normconst;
  FLOAT dist;

  //sigma2 = 0.0;
  sigma2 = 1.0;
  for ( int i=0; i<m_nDim; i++ )
    //sigma2 += cov[covIdx(0,i,i)]*cov[covIdx(0,i,i)];
    sigma2 *= cov[covIdx(0,i,i)];
  sigma2 = sqrt(sigma2);
  if (sigma2 == 0.0) 
    sigma2 = EPSILON_SIGMA;

  exponent = 0.0;
  for ( int i=0; i<m_nDim; i++) {
    dist = (fvPt.at(i) - fvMean.at(i));
    exponent += (dist*dist) / cov[covIdx(0,i,i)];
  }
  //normconst = pow( 2.0 * PI, ((FLOAT) m_nDim)*0.5 ) * sigma2;
  normconst = m_fNormConst * sigma2;
  
  return exp(-0.5*exponent) / normconst;
}


/*---------------------------------------------------------*/
/*                     Initialization                      */
/*---------------------------------------------------------*/

void ClEMFull::initDataVectors()
{
  m_vCenters.resize  ( m_nNumClusters );
  m_vCovMatrix.resize( m_nNumClusters*m_nDim*m_nDim );
  m_vPriors.resize   ( m_nNumClusters );
  m_vPosteriors.resize( m_nNumPoints*m_nNumClusters );

  m_fNormConst = pow( 2.0 * PI, ((FLOAT) m_nDim)*0.5 );
}


void ClEMFull::initCovariances()
{
  /******************************/
  /* Initialize the covariances */
  /******************************/
  /* set the sigma values to the distances to the nearest cluster centers */
  for ( int j=0; j < m_nNumClusters; j++ ) {
    /* calculate the distance to the nearest other cluster center */
    FLOAT dist2 = 0.0;
    FLOAT min_dist2 = 999999999999.9;
    for ( int other=0; other < m_nNumClusters; other++ )
      if (j != other){
        dist2 = m_vCenters[j].compSSD( m_vCenters[other] );
        if ( dist2 < min_dist2 )
          min_dist2 = dist2;
      }
    
    for( int d=0; d<m_nDim; d++ ) {
      m_vCovMatrix[covIdx(j,d,d)] = regularize( min_dist2 );
    }
  }
}


void ClEMFull::initPosteriors()
{
  /******************************************/
  /* Initialize the posterior probabilities */
  /******************************************/
  /* calculate the posterior probabilities for all points */
  for ( int i=0; i < m_nNumPoints; i++ ) {
    for ( int j=0; j < m_nNumClusters; j++ ) {
      //m_vPosteriors[clusterIdx(i,j)] = evaluateGauss( m_vPoints[i], 
      //   					      m_vCenters[j], 
      //   					      m_vCovMatrix, j );
      m_vPosteriors[clusterIdx(i,j)] = 1.0/((FLOAT) m_nNumClusters);
    }
  }
}


/*---------------------------------------------------------*/
/*                      Reestimation                       */
/*---------------------------------------------------------*/

void ClEMFull::doReestimationStep()
{
  /**************************************************************/
  /* For every point, calculate the new posterior probabilities */
  /**************************************************************/
  //cout << "      Calculating the new posterior probabilities..." << endl;
  for ( int i=0; i < m_nNumPoints; i++ ) {
    
    FLOAT sum = 0.0;
    for ( int j=0; j < m_nNumClusters; j++ ) {
      /* calculate the probability of belonging to each cluster */
      m_vPosteriors[clusterIdx(i,j)] = ( evaluateGauss( m_vPoints[i], 
                                                        m_vCenters[j], 
                                                        m_vCovMatrix, j ) * 
                                         m_vPriors[j] );	
      sum += m_vPosteriors[clusterIdx(i,j)];
    }
    
    sum = regularize( sum );
    for ( int j=0; j < m_nNumClusters; j++ ) 
      m_vPosteriors[clusterIdx(i,j)] = 
        regularize( m_vPosteriors[clusterIdx(i,j)] / sum );
  }
}


/*---------------------------------------------------------*/
/*                         Update                          */
/*---------------------------------------------------------*/

void ClEMFull::updateCovariances()
{
  /********************************/
  /* update the covariance matrix */
  /********************************/
  for ( int j=0; j < m_nNumClusters; j++ ) {
    
    /* calculate the weighted variance over all points */
    FLOAT sum_prob = 0.0;
    for( int d=0; d < m_nDim; d++ ) {
      /* for every diagonal element of component j's covariance matrix */
      m_vCovMatrix[covIdx(j,d,d)] = 0.0;
      FLOAT sigma = 0.0;
      
      for ( int i=0; i < m_nNumPoints; i++ ) {
	/* calculate the squared distance of every point to the      */
	/* cluster center.                                           */
	FLOAT diff = (m_vPoints[i].at(d) - m_vCenters[j].at(d));
	
	sigma    += m_vPosteriors[clusterIdx(i,j)] * diff*diff;
	sum_prob += m_vPosteriors[clusterIdx(i,j)];
      }
      
      m_vCovMatrix[covIdx(j,d,d)] = sigma;
    }
    
    sum_prob = regularize( sum_prob );
    for( int d=0; d < m_nDim; d++ ) {
      m_vCovMatrix[covIdx(j,d,d)] = ( m_vCovMatrix[covIdx(j,d,d)] / sum_prob );
      //(((FLOAT) m_nDim)*sum_prob) );
      m_vCovMatrix[covIdx(j,d,d)] = ( (1.0-EPSILON_SIGMA) *
				      m_vCovMatrix[covIdx(j,d,d)] +
				      EPSILON_SIGMA );
    }
  }
}


/***********************************************************/
/*                     Output Functions                    */
/***********************************************************/

void ClEMFull::printResults()
{
  //cout << "  ClEMFull::printResults() called." << endl;
  for (int j=0; j<m_nNumClusters; j++) {
    cout << "    Cluster " << setw(2) << j
	 << ": prior=" << setw(7) << setprecision(6) << m_vPriors[j];
    if ( m_nDim <= 3 )
      cout << ", mean=" << m_vCenters[j];
    cout << ", sigma=(";

    cout << setprecision(5) << setiosflags(ios::showpoint);
    for( int d=0; d<m_nDim-1; d++ )
      cout << m_vCovMatrix[covIdx(j,d,d)] 
	   << ", ";
    cout << setprecision(5)
	 << m_vCovMatrix[covIdx(j,m_nDim-1,m_nDim-1)] << ")" << endl;
  }
  //cout << "  done." << endl;
}


vector<int> ClEMFull::getClusterAssignment()
{
  /****************************************************/
  /* assign every point to the nearest cluster center */
  /****************************************************/
  vector<int> vBelongsTo( m_nNumPoints );
  FLOAT min_dist;
  int   min_idx;
  for ( int i=0; i < m_nNumPoints; i++ ) {
    FeatureVector &pt = m_vPoints[i]; 
    
    /* calculate the min weighted distance to every cluster center */
    min_dist = ( m_vPriors[0]*m_vPosteriors[clusterIdx(0,0)]*
                 pt.compSSD( m_vCenters[0] ) );
    min_idx  = 0;
    for ( int j=0; j < m_nNumClusters; j++ ) {
      FLOAT dist = ( m_vPriors[j] * m_vPosteriors[clusterIdx(i,j)] *
                     pt.compSSD( m_vCenters[j] ) );
      if ( dist < min_dist ) {
        min_dist = dist;
        min_idx  = j;
      }
    }
    
    /* update the cluster assignment */
    vBelongsTo[i] = min_idx;      
  }
  
  return vBelongsTo;
}


#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif
