/*********************************************************************/
/*                                                                   */
/* FILE         clrnncagglo.cc                                       */
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
/*                                                                   */
/* BEGIN        Fri Jun 04 2004                                      */
/* LAST CHANGE  Fri Jun 04 2004                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>

#include <math.h>
#include <stdlib.h>

#include "resources.hh"
#include "clrnncagglo.hh"

/****************/
/* Definitions  */
/****************/

const float MIN_SIM = -99999999999.9;

/*===================================================================*/
/*                         Class ClFastAgglo                         */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

ClRNNCAgglo::ClRNNCAgglo( const vector<FeatureVector> &vPoints )
  : Cluster( vPoints )
{
  m_nMetricType = METRIC_NGC;
}


/***********************************************************/
/*                     Initialization                      */
/***********************************************************/

void ClRNNCAgglo::initDataVectors()
{
  m_vCenters.resize( m_nNumClusters );
  m_vBelongsTo.resize( m_nNumPoints );
  m_vVariances.resize( m_nNumClusters, 0.0 );
  
  m_vTrace.clear();
}


void ClRNNCAgglo::initClusterCenters()
{
  /* set the cluster centers initially (one to each point) */
  for(int i=0; i < m_nNumClusters; i++ ) {
    m_vCenters[i]       = m_vPoints[i];
    m_vVariances[i]     = 0.0;
    m_vBelongsTo[i]     = i;
    m_vValid.push_back( true );

    /* initialize all Feature Vectors to each corresponding cluster center */
    vector<int> tempFV;
    tempFV.push_back( i );
    m_vvAllBelongingFV.push_back( tempFV );
  }
  
  /* free the memory used for points */
  m_vPoints.clear();
}


void ClRNNCAgglo::initCovariances()
{}


void ClRNNCAgglo::initPriors()
{}


void ClRNNCAgglo::initPosteriors()
{
  //doReestimationStep();
}


float ClRNNCAgglo::sim( int idx1, int idx2 )
{
  float sim = MIN_SIM;
  FeatureVector &p1 = m_vCenters[idx1];
  FeatureVector &p2 = m_vCenters[idx2];

  switch( m_nMetricType ) {
  case METRIC_NGC:
    sim = p1.compCorrelation( p2 );
    break;
    
  case METRIC_EUCLID:
    sim = -(m_vVariances[idx1] + m_vVariances[idx2] + p1.compSSD( p2 ));
    break;
    
  default:
      cerr << "Error in ClRNNCAgglo::sim(): "
           << "Unknown similarity measure (" << m_nMetricType << ")" << endl;
  }

  return sim;
}


void ClRNNCAgglo::agglomerate( int idx1, int idx2, int newidx )
  /*******************************************************************/
  /* Merge 2 neighboring clusters if compactness is guaranteed.      */
  /*******************************************************************/
{
  /*--------------------------------*/
  /* Compute the new cluster center */
  /*--------------------------------*/
  /* The combined cluster center can be computed as follows:       */
  /*   c_new = (1/N+M)*(N*c_x + M*c_y)                             */
  FeatureVector aCenter, bCenter, newCenter;
  float   N, M;
  aCenter = m_vCenters[idx1];
  bCenter = m_vCenters[idx2];
  N       = (float) m_vvAllBelongingFV[idx1].size();
  M       = (float) m_vvAllBelongingFV[idx2].size();
  aCenter.multFactor( N );
  bCenter.multFactor( M );
  newCenter = (aCenter + bCenter)/(float)(N + M);
  //newCenter.addVector( bCenter );
  //newCenter.divFactor( N + M );
  
  
  /*--------------------------*/
  /* Compute the new variance */
  /*--------------------------*/
  /* The new variance can be computed as                           */
  /*   sig_new^2 = (1/N+M)*(N*sig_x^2 + M*sig_y^2 +                */
  /*                        NM/(N+M)*(mu_x - mu_y)^2)              */
  double aVar, bVar, newVar;
  aVar = m_vVariances[idx1];          
  bVar = m_vVariances[idx2];
  newVar = ( ( N*aVar + M*bVar + 
               N*M/(N+M)*m_vCenters[idx1].compSSD( m_vCenters[idx2]) ) / 
             (N + M) );
  

  /*------------------------------------------*/
  /* Add the new center to the representation */
  /*------------------------------------------*/
  /* 2 cluster centers will be invalid */
  m_vValid[idx1] = false;
  m_vValid[idx2] = false;

  /* add the new cluster center and variance */
  m_vCenters[newidx]   = newCenter;
  m_vVariances[newidx] = newVar;
  m_vValid[newidx]     = true;

  /* adjust the corresponding Feature Vectors to the new Center */
  if( newidx != idx1 )
    for( int i=0; i<(int)m_vvAllBelongingFV[idx1].size(); i++ ) {
      //new cluster assignment
      m_vBelongsTo[ m_vvAllBelongingFV[idx1][i] ] = newidx; 
    }
  if( newidx != idx2 )
    for( int i=0; i<(int)m_vvAllBelongingFV[idx2].size(); i++ ) {
      //new cluster assignment
      m_vBelongsTo[ m_vvAllBelongingFV[idx2][i] ] = newidx; 
    }
  
  vector<int> fvecBothAB( m_vvAllBelongingFV[idx1] );
  fvecBothAB.insert( fvecBothAB.end(), m_vvAllBelongingFV[idx2].begin(),
                     m_vvAllBelongingFV[idx2].end() );
//   for(int i=0; i < m_vvAllBelongingFV[b].size(); i++) {
//     fvecBothAB.push_back( m_vvAllBelongingFV[b][i] );
//   }
  m_vvAllBelongingFV[newidx] = fvecBothAB;
}


/*************************************************************/
/*       Call update function for merging in each step       */
/*************************************************************/
bool ClRNNCAgglo::doClusterSteps( double minSimilarity, bool bVerbose )
{
  // TIMING CODE
  time_t  tu1, tu2, tu3;
  double  tc1, tc2, tc3;  
  int     NUM_TIMINGSTEPS = 10;
  vector<time_t> vTu(NUM_TIMINGSTEPS);
  vector<double> vTc(NUM_TIMINGSTEPS);
  time(&tu1);
  tc1 = CPUTIME();

  m_dMinSimilarity = minSimilarity;
  m_bMore = true;
  if( bVerbose )
    cout << "  ClRNNCAgglo::doClusterSteps() called with min similarity: " 
         << m_dMinSimilarity << endl;
  
  if( bVerbose )
    cout << "  Clustering..." << endl;

  int nNumPoints = m_vCenters.size();
  vector<int>   vNN      ( nNumPoints );
  vector<bool>  vInactive( nNumPoints );
  vector<float> vSim     ( nNumPoints );

  /******************************/
  /* Start with the first point */
  /******************************/
  int last = 0;
  vNN[last] = 0;
  vInactive[vNN[last]] = true;
  vSim     [last]  = MIN_SIM;
  int nNumActive   = nNumPoints - 1;
  int nFirstActive = 1;

  int nQuantileStep = nNumPoints/NUM_TIMINGSTEPS;
  int nCurStep      = 0;

  /***************/
  /* Agglomerate */
  /***************/
  while( nNumActive > 0 ) {
    /* collect timing information */
    if( (nNumPoints-nNumActive)>(nCurStep+1)*nQuantileStep ) {
      if( bVerbose )
        cout << "\r    Finished " << setw(3) 
             << (int)round(100.0*(nNumPoints-nNumActive)/(float)nNumPoints)
             << "%... " << flush;
      time(&(vTu[nCurStep]));
      vTc[nCurStep] = CPUTIME();
      nCurStep++;
    }

    /*=====================================*/
    /* Find the NN for the last chain link */
    /*=====================================*/
    int   nnidx = nFirstActive;
    float nnsim = sim( vNN[last], nnidx );
    for( int i=nFirstActive+1; i<nNumPoints; i++ )
      if( !vInactive[i] ) {
        float d = sim( vNN[last], i );
        if( d > nnsim ) {
          nnsim = d;
          nnidx = i;
        }
      }

    /*==========================*/
    /* Check for reciprocal NNs */
    /*==========================*/
    if( nnsim > vSim[last] ) {
      /*-----------------------------------------*/
      /* No RNN => Add the point to the NN chain */
      /*-----------------------------------------*/
      last++;
      vNN[last]  = nnidx;
      vSim[last] = nnsim;
      vInactive[nnidx] = true;
      nNumActive--;

      /* recompute the 'first' counter */
      if( (nnidx==nFirstActive) && (nNumActive>0) )
        for( int i=nFirstActive+1; i<nNumPoints; i++ )
          if( !vInactive[i] ) {
            nFirstActive = i;
            break;
          }

    } else
      /*----------------------------------------------------------*/
      /* RNN => check if the chain links are sufficiently similar */
      /*----------------------------------------------------------*/
      if( vSim[last] >= m_dMinSimilarity ) {
        /*--------------------------------------*/
        /* Agglomerate the two last chain links */
        /*--------------------------------------*/
        int newidx = vNN[last]; //min( vNN[last], vNN[last-1] );
        agglomerate( vNN[last], vNN[last-1], newidx );
        writeTrace ( vNN[last], vNN[last-1], vSim[last], newidx );
        vInactive[newidx] = false;
        nNumActive++;
        last -= 2;
        
        /* recompute the 'first' counter */
        if( newidx < nFirstActive )
          nFirstActive = newidx;

      } else {
        /*-------------------*/
        /* Discard the chain */
        /*-------------------*/
        last = -1;
      }

    /*=============================*/
    /* Check if the chain is empty */
    /*=============================*/
    if( last < 0 ){
      /* empty chain => start new chain */
      last++;
      vNN[last]  = nFirstActive;
      vSim[last] = MIN_SIM;
      vInactive[nFirstActive] = true;
      nNumActive--;

      /* recompute the 'first' counter */
      if( nNumActive > 0 )
        for( int i=nFirstActive+1; i<nNumPoints; i++ )
          if( !vInactive[i] ) {
            nFirstActive = i;
            break;
          }
    } 
  }

  time(&tu2);
  tc2 = CPUTIME();


  /***************************/
  /* Prepare the result data */
  /***************************/
  vector<FeatureVector> vNewCenters;
  vector<int> vNewIndices;
  int nNumInvalid = 0;
  for( int i=0; i<(int)m_vCenters.size(); i++ ) {
    if ( m_vValid[i] ) {
      vNewCenters.push_back( m_vCenters[i] );
      vNewIndices.push_back( i - nNumInvalid );
    }
    else {
      vNewIndices.push_back( -1 );
      nNumInvalid++;
    }
  }
  m_vCenters = vNewCenters;
  
  /* correct the following indices */
  for( int i=0; i<(int)m_vBelongsTo.size(); i++ ){
    m_vBelongsTo[i] = vNewIndices[ m_vBelongsTo[i] ];
  }
  
  /* stop agglomerative clustering */
  if( bVerbose ) {
    cout << "  Size of m_vBelongsTo after is: " << m_vBelongsTo.size() << endl;
    cout << "  Size of m_vCenters after is: " << m_vCenters.size() << endl;
    cout << "  Agglomerative clustering finished." << endl;
    cout << endl;
  }

  time(&tu3);
  tc3 = CPUTIME();

  if( bVerbose ) {
    cout << "    ----------------------" << endl;
    cout << "    ClRNNCAgglo finished  " << endl;
    cout << "    ----------------------" << endl;
    cout << "    Time spent for..." << endl;
    cout << "      Clustering            : " << setw(12) 
         << tc2-tc1 << "s (system), " 
         << tu2-tu1 << "s (user)" << endl; 
    
    cout << "        Time for " << setw(4) 
         << (int)floor(100.0*nQuantileStep/(float)nNumPoints  +0.5)
         << "%      : " << setw(12)
         << vTc[0]-tc1 << "s (system), " 
         << vTu[0]-tu1 << "s (user)" << endl; 
    for(int i=1; i<nCurStep; i++ )
      cout << "        Time for " << setw(4) 
           << (int)floor(100.0*(i+1)*nQuantileStep/(float)nNumPoints +0.5)
           << "%      : " << setw(12)
           << vTc[i]-vTc[i-1] << "s (system), " 
           << vTu[i]-vTu[i-1] << "s (user)" << endl; 
    cout << "        Time for  100%      : " << setw(12)
         << tc2-vTc[nCurStep-1] << "s (system), " 
         << tu2-vTu[nCurStep-1] << "s (user)" << endl; 
    
    cout << "      Postprocessing        : " << setw(12) 
         << tc3-tc2 << "s (system), " 
         << tu3-tu2 << "s (user)" << endl; 
    cout << endl;
    cout << "    ----------------------" << endl;
  }
  return true;
}



/***********************************************************/
/*                      Reestimation                       */
/***********************************************************/

void ClRNNCAgglo::doReestimationStep()
{}



/***********************************************************/
/*                         Update                          */
/***********************************************************/

void ClRNNCAgglo::doUpdateStep()
{
}


/***********************************************************/
/*                     Error Estimation                    */
/***********************************************************/

FLOAT ClRNNCAgglo::calculateError()
{
  return -1.0;
}


/***********************************************************/
/*                     Output Functions                    */
/***********************************************************/

void ClRNNCAgglo::printResults()
{
  //cout << "  ClRNNCAgglo::printResults() called." << endl;

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
  //cout << "  done." << endl;
}
