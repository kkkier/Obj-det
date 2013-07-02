/*********************************************************************/
/*                                                                   */
/* FILE         clem.cc                                              */
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
  
/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>

#include <math.h>
#include <stdlib.h>

#include "clem.hh"


/*===================================================================*/
/*                            Class ClEM                             */
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

FLOAT ClEM::evaluateGauss( FeatureVector fvPt, FeatureVector fvMean, 
                           FeatureMatrix mSigma )
  /* evaluates the standard Normal distribution function for x:      */
  /* p(x) = (1/2*pi*|sigma|^1/2)*exp(-0.5(x-mean)T*sigma^-1*(x-mean) */
{
  //cout << "      ClEM::evaluateGauss() called..." << endl;
  FLOAT sigma2, exponent, normconst;

  sigma2 = sqrt(mSigma.det());
  if (sigma2 == 0.0) 
    sigma2 = EPSILON_SIGMA * EPSILON_SIGMA;
  normconst = 1.0 / (m_fNormConst * sigma2);
  
  FeatureVector xm = fvPt - fvMean;

  FeatureMatrix mInvCov = mSigma.invert() + m_mRegul;
  exponent = - 0.5 * ( mInvCov.leftMulVecT(xm) * xm ).at(0);
  
  //cout << "        pt       : " << fvPt << endl;
  //cout << "        mean     : " << fvMean << endl;
  //cout << "        pt-mean  : " << xm << endl;
  //cout << "        mSigma^-1: " << endl
  //     << mInvCov;
  //cout << "        exponent : " << exponent << endl;
  //cout << "        normcon  : " << normconst << endl;
  //cout << "        result   : " << exp(exponent)*normconst << endl;

  //cout << "      done." << endl;
  return exp(exponent) * normconst;
}


/*---------------------------------------------------------*/
/*                     Initialization                      */
/*---------------------------------------------------------*/

void ClEM::initDataVectors()
{
  m_vCenters.resize( m_nNumClusters );
  m_vSigmas.resize ( m_nNumClusters );
  m_vPriors.resize ( m_nNumClusters );
  m_vPosteriors.resize( m_nNumPoints*m_nNumClusters );

  m_fNormConst = pow( 2.0 * PI, ((FLOAT) m_nDim)*0.5 );

//   /* normalize the data points to zero mean, unit variance */
//   m_vOriginalPoints = m_vPoints;
  
//   /* convert into Featurevectors */
//   vector<FeatureVector> vFvPoints;
//   convertClPointsToFeatureVecs( m_vPoints, vFvPoints );

//   /* calculate the mean */
//   FeatureVector fvTmp( m_nDim );
//   m_fvMean = fvTmp;
//   for ( int i=0; i<m_nNumPoints; i++ )
//     m_fvMean += vFvPoints[i];
//   m_fvMean /= (double) nTotalBins;

//   /* subtract the mean */
//   for ( int i=0; i<m_nNumPoints; i++ )
//     vFvPoints[i] -= m_fvMean;

//   /* calculate the covariance */
//   Matrix mCov( m_nDim, m_nDim );
//   for ( int i=0; i<m_nNumPoints; i++ )
//     mCov += mulVecVecT( vFvPoints[i], vFvPoints[i] );
//   mCov /= (float) m_nNumPoints;

//   /* and invert it */
//   Matrix mInvCov = mCov.invert();

  /* normalize all points with the inverted covariance */
  //for ( int i=0; i<m_nNumPoints; i++ )
  // vFvPoints[i] -= m_fvMean;
  
}


void ClEM::initClusterCenters()
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


void ClEM::initCovariances()
{
  /******************************/
  /* Initialize the covariances */
  /******************************/
  /* compute the mean and covariance of all data points */
  FeatureVector fvMean( m_nDim );
  for ( int i=0; i < m_nNumPoints; i++ ) {
    FeatureVector &fvPt = m_vPoints[i];
    fvMean += fvPt;
  }
  fvMean /= (float)m_nNumPoints;

  FeatureMatrix mCov( m_nDim, m_nDim );
  for ( int i=0; i < m_nNumPoints; i++ ) {
    /* calculate the distance of every point to the cluster center.*/
    FeatureVector &fvPt = m_vPoints[i];
    FeatureVector xm = fvPt - fvMean;
    
    FeatureMatrix mSigma = mulVecVecT( xm, xm );
    mCov     += mSigma;
  }
  mCov /= (float)m_nNumPoints;

  /* compute the mean and covariance of all data points */
//   FeatureVector fvMean( m_nDim );
//   FeatureMatrix mCov( m_nDim, m_nDim );
//   for ( int i=0; i < m_nNumPoints; i++ ) {
//     FeatureVector fvPt( m_vPoints[i].getData() );
//     fvMean += fvPt;
//     mCov   += mulVecTVec( fvPt, fvPt ); 
//   }
//   fvMean /= (float)m_nNumPoints;
//   mCov   -= mulVecTVec( fvMean, fvMean );
//   mCov   /= (float)m_nNumPoints;

  /* create a regularization matrix */
  m_mRegul = createUnitMatrix( m_nDim ) * EPSILON_COV;
//   for( int d=0; d<m_nDim; d++)
//     if( mCov(d,d)/FRAC > m_mRegul(d,d) )
//       m_mRegul(d,d) = mCov(d,d)/FRAC;
  
  /* regularize the covariance matrix */
  //mCov += m_mRegul;

  /* regularize the covariance matrix */
  mCov = (mCov.invert()+m_mRegul).invert() + m_mRegul;

  /* weight with the number of clusters and the dimensionality */
//   mCov /= pow( (float) m_nNumClusters, 1.0/(float)m_nDim );

  /* set the initial covariances to this value */
  for ( int i=0; i < m_nNumClusters; i++ )
    m_vSigmas[i] = mCov;
}


void ClEM::initPriors()
{
  /*********************************/
  /* Initialize the cluster priors */
  /*********************************/
  /* set the class priors all to 1/NumClusters (equally probable) */
  for ( int j=0; j < m_nNumClusters; j++ ) {
    m_vPriors[j] = 1.0 / ((double) m_nNumClusters);
  }
}


void ClEM::initPosteriors()
{
  /******************************************/
  /* Initialize the posterior probabilities */
  /******************************************/
  /* calculate the posterior probabilities for all points */
  for ( int i=0; i < m_nNumPoints; i++ ) {
    for ( int j=0; j < m_nNumClusters; j++ ) {
      //m_vPosteriors[clusterIdx(i,j)] = 1.0/((FLOAT) m_nNumClusters);
      m_vPosteriors[clusterIdx(i,j)] = evaluateGauss( m_vPoints[i], 
                                                      m_vCenters[j], 
                                                      m_vSigmas[j] );
    }
  }
}


/*---------------------------------------------------------*/
/*                      Reestimation                       */
/*---------------------------------------------------------*/

void ClEM::doReestimationStep()
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

void ClEM::doUpdateStep()
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


void ClEM::updatePriors()
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


void ClEM::updateClusterCenters()
{
  /******************************/
  /* update the cluster centers */
  /******************************/
  for ( int j=0; j < m_nNumClusters; j++ ) {

    /* calculate the weighted mean over all points */
    FeatureVector fvMean( m_nDim );
    FLOAT sum_prob = 0.0;
    for ( int i=0; i < m_nNumPoints; i++ ) {
      fvMean   += ( m_vPoints[i] * m_vPosteriors[clusterIdx(i,j)] );
      sum_prob += m_vPosteriors[clusterIdx(i,j)];
    }
    
    sum_prob = regularize( sum_prob );
    m_vCenters[j] = fvMean / sum_prob;
   }
}


void ClEM::updateCovariances()
{
  /**********************************/
  /* update the covariance matrices */
  /**********************************/
  for ( int j=0; j < m_nNumClusters; j++ ) {
    
    /* calculate the weighted variance over all points */
    FLOAT  sum_prob = 0.0;
    FeatureMatrix mSigma( m_nDim, m_nDim );

    for ( int i=0; i < m_nNumPoints; i++ ) {
      /* calculate the distance of every point to the cluster center.*/
      FeatureVector &fvPt   = m_vPoints[i];
      FeatureVector &fvMean = m_vCenters[j];
      FeatureVector xm = fvPt - fvMean;

      FeatureMatrix mCov = mulVecVecT( xm, xm );
      mSigma   += mCov * m_vPosteriors[clusterIdx(i,j)];
      sum_prob += m_vPosteriors[clusterIdx(i,j)];
    }
      
    sum_prob = regularize( sum_prob );
    m_vSigmas[j] = mSigma / sum_prob;
    /* regularize the result */
    //m_vSigmas[j] = (1.0-EPSILON_SIGMA)*m_vSigmas[j] + EPSILON_SIGMA;
    m_vSigmas[j] += m_mRegul;
  }
}


/*---------------------------------------------------------*/
/*                     Error Estimation                    */
/*---------------------------------------------------------*/

FLOAT ClEM::calculateError()
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

void ClEM::printResults()
{
  //cout << "  ClEM::printResults() called." << endl;
  for (int j=0; j<m_nNumClusters; j++) {
    cout << "    Cluster " << setw(2) << j
         << ": prior=" << setw(7) << setprecision(6) << m_vPriors[j];
    if ( m_nDim <= 3 )
      cout << ", mean=" << m_vCenters[j];
    cout << endl;
    cout << "      sigma=" << endl 
         << setw(7) << setprecision(6) << m_vSigmas[j] 
         << endl;
  }
  //cout << "  done." << endl;
}


void ClEM::printPosteriors()
{
  //cout << "  ClEM::printPosteriors() called." << endl;
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


vector<int> ClEM::getClusterAssignment()
{
  /****************************************************/
  /* assign every point to the nearest cluster center */
  /****************************************************/
  vector<int> vBelongsTo( m_nNumPoints );
  FLOAT min_dist;
  int   min_idx;
  for ( int i=0; i < m_nNumPoints; i++ ) {
    FeatureVector &fvPt = m_vPoints[i]; 
    
    /* calculate the min weighted distance to every cluster center */
    min_dist = ( m_vPriors[0]*m_vPosteriors[clusterIdx(0,0)]*
                 fvPt.compSSD( m_vCenters[0] ) );
    min_idx  = 0;
    for ( int j=0; j < m_nNumClusters; j++ ) {
      FLOAT dist = ( m_vPriors[j] * m_vPosteriors[clusterIdx(i,j)] *
                     fvPt.compSSD( m_vCenters[j] ) );
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


FLOAT ClEM::regularize( FLOAT val )
{
  return (1.0 - EPSILON_PROB)*val + EPSILON_PROB;
}


