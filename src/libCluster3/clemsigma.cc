/*********************************************************************/
/*                                                                   */
/* FILE         clemsigma.cc                                         */
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

#include "clemsigma.hh"


/*===================================================================*/
/*                         Class ClEMSigma                           */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

/***********************************************************/
/*                   Clustering Functions                  */
/***********************************************************/

/*---------------------------------------------------------*/
/*            EM clustering with per-class sigma           */
/*---------------------------------------------------------*/

FLOAT ClEMSigma::evaluateGauss( FeatureVector pt, FeatureVector mean, 
                                FLOAT sigma )
  /* evaluates the standard Normal distribution function for x:      */
  /* p(x) = (1/2*pi*sigma^2)*exp(-(x-mean)^2/2*sigma^2)              */
{
  FLOAT sigma2, exponent, normconst;

  sigma2    = sigma*sigma;
  if (sigma2 == 0.0) 
    sigma2 = EPSILON_SIGMA * EPSILON_SIGMA;
  exponent  = -( pt.compSSD(mean) / (2.0*sigma2) );
  //normconst = pow( 2.0 * PI, ((FLOAT) m_nDim)*0.5 ) * sigma2;
  normconst = m_fNormConst * sigma2;
  
  return exp(exponent) / normconst;
}


/*---------------------------------------------------------*/
/*                     Initialization                      */
/*---------------------------------------------------------*/

void ClEMSigma::initDataVectors()
{
  m_vCenters.resize( m_nNumClusters );
  m_vSigmas.resize( m_nNumClusters );
  m_vPriors.resize( m_nNumClusters );
  m_vPosteriors.resize( m_nNumPoints*m_nNumClusters );

  m_fNormConst = pow( 2.0 * PI, ((FLOAT) m_nDim)*0.5 );
}


void ClEMSigma::initClusterCenters()
{
  /**********************************/
  /* Initialize the cluster centers */
  /**********************************/
  /* set the cluster centers to randomly selected points */
  for ( int i=0; i < m_nNumClusters; i++ ) {
    int idx = (int) floor(((FLOAT)m_nNumPoints)*rand()/(RAND_MAX+1.0));
    m_vCenters[i] = m_vPoints[idx];
  }  
}


void ClEMSigma::initCovariances()
{
  /*******************************/
  /* Initialize the sigma values */
  /*******************************/
  /* set the sigma values to the distances to the nearest cluster centers */
  for ( int i=0; i < m_nNumClusters; i++ ) {
    /* calculate the distance to the nearest other cluster center */
    FLOAT dist2 = 0.0;
    FLOAT min_dist2 = 999999999999.9;
    for ( int other=0; other < m_nNumClusters; other++ )
      if (i != other){
        dist2 = m_vCenters[i].compSSD( m_vCenters[other] );
        if ( dist2 < min_dist2 )
          min_dist2 = dist2;
      }
    
    m_vSigmas[i] = sqrt(min_dist2);
  }
}


void ClEMSigma::initPriors()
{
  /*********************************/
  /* Initialize the cluster priors */
  /*********************************/
  /* set the class priors all to 1/NumClusters (equally probable) */
  for ( int j=0; j < m_nNumClusters; j++ ) {
    m_vPriors[j] = 1.0 / ((double) m_nNumClusters);
  }
}


void ClEMSigma::initPosteriors()
{
  /******************************************/
  /* Initialize the posterior probabilities */
  /******************************************/
  /* calculate the posterior probabilities for all points */
  for ( int i=0; i < m_nNumPoints; i++ ) {
    for ( int j=0; j < m_nNumClusters; j++ ) {
      m_vPosteriors[clusterIdx(i,j)] = evaluateGauss( m_vPoints[i], 
                                                      m_vCenters[j], 
                                                      m_vSigmas[j] );
    }
  }
}


/*---------------------------------------------------------*/
/*                      Reestimation                       */
/*---------------------------------------------------------*/

void ClEMSigma::doReestimationStep()
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
                                                        m_vSigmas[j] ) * 
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

void ClEMSigma::doUpdateStep()
{
  /******************************/
  /* update the cluster priors */
  /******************************/
  //cout << "      Updating the cluster priors..." << endl;
  updatePriors();  

  /******************************/
  /* update the cluster centers */
  /******************************/
  //cout << "      Updating the cluster centers..." << endl;
  updateClusterCenters();
    
  /***************************/
  /* update the sigma values */
  /***************************/
  //cout << "      Updating the covariances..." << endl;
  updateCovariances();
}


void ClEMSigma::updatePriors()
{
  /******************************/
  /* update the cluster priors */
  /******************************/
  for ( int j=0; j < m_nNumClusters; j++ ) {
    
    /* calculate the mean over all posteriors */
    m_vPriors[j] = 0.0;
    for ( int i=0; i < m_nNumPoints; i++ )
      m_vPriors[j] += m_vPosteriors[clusterIdx(i,j)];
    m_vPriors[j] = regularize( m_vPriors[j] / m_nNumPoints );
  }
}


void ClEMSigma::updateClusterCenters()
{
  /******************************/
  /* update the cluster centers */
  /******************************/
  for ( int j=0; j < m_nNumClusters; j++ ) {

    /* calculate the weighted mean over all points */
    FeatureVector fvMean( m_nDim );
    FLOAT sum_prob = 0.0;
    for ( int i=0; i < m_nNumPoints; i++ ) {
      fvMean   += m_vPoints[i] * m_vPosteriors[clusterIdx(i,j)];
      sum_prob += m_vPosteriors[clusterIdx(i,j)];
    }
    
    sum_prob = regularize( sum_prob );
    m_vCenters[j] = fvMean / sum_prob;
   }
}


void ClEMSigma::updateCovariances()
{
  /***************************/
  /* update the sigma values */
  /***************************/
  for ( int j=0; j < m_nNumClusters; j++ ) {
    
    /* calculate the weighted variance over all points */
    FLOAT sum_prob = 0.0;
    FLOAT sigma = 0.0;
    
    for ( int i=0; i < m_nNumPoints; i++ ) {
      /* calculate the squared distance of every point to the      */
      /* cluster center.                                           */
      FLOAT dist = m_vPoints[i].compSSD( m_vCenters[j] );
      
      sigma    += m_vPosteriors[clusterIdx(i,j)] * dist;  
      sum_prob += m_vPosteriors[clusterIdx(i,j)];
    }
    
    sum_prob = regularize( sum_prob );
    m_vSigmas[j] = sqrt( sigma / (((FLOAT) m_nDim)*sum_prob) );
    m_vSigmas[j] = (1.0-EPSILON_SIGMA)*m_vSigmas[j] + EPSILON_SIGMA;
  }
}


/*---------------------------------------------------------*/
/*                     Error Estimation                    */
/*---------------------------------------------------------*/

FLOAT ClEMSigma::calculateError()
{
  /* calculate the error */
  FLOAT error = 0.0;
  for ( int i=0; i < m_nNumPoints; i++ ) {
    /* for every point, calculate the distance to each cluster center */
    /* and weigh it by the cluster prior and posteriors.              */
    for( int j=0; j < m_nNumClusters; j++ ) {
      FLOAT dist2 = m_vPoints[i].compSSD( m_vCenters[j] );
    
      error += m_vPosteriors[clusterIdx(i,j)]*m_vPriors[j]*dist2;
    }
  }
  error = m_fError-2.0;
  return error;
  //return sqrt(error);
}


/***********************************************************/
/*                     Output Functions                    */
/***********************************************************/

void ClEMSigma::printResults()
{
  //cout << "  ClEMSigma::printResults() called." << endl;
  for (int j=0; j<m_nNumClusters; j++) {
    cout << "    Cluster " << setw(2) << j
         << ": prior=" << setw(7) << setprecision(6) << m_vPriors[j];
    if ( m_nDim <= 3 )
      cout << ", mean=" << m_vCenters[j];
    cout << ", sigma=" << setw(7) << setprecision(6) << m_vSigmas[j] << endl;
  }
  //cout << "  done." << endl;
}


void ClEMSigma::printPosteriors()
{
  //cout << "  ClEMSigma::printPosteriors() called." << endl;
  for (int i=0; i<m_nNumPoints; i++) {
    cout << "    Point " << setw(2) << i << ":";
    for (int j=0; j<m_nNumClusters; j++) {
      cout << "  " << setw(5) << setprecision(4) 
           << m_vPosteriors[clusterIdx(i,j)];
    }
    cout << endl;
  }
  //cout << "  done." << endl;
}


FLOAT ClEMSigma::regularize( FLOAT val )
{
  return (1.0 - EPSILON_PROB)*val + EPSILON_PROB;
}


#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif
