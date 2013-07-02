/*********************************************************************/
/*                                                                   */
/* FILE         clfastrnncagglo.cc                                   */
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

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>

#include <math.h>
#include <stdlib.h>

#include "resources.hh"
#include "clfastrnncagglo2.hh"

/****************/
/* Definitions  */
/****************/
const float MIN_SIM = -99999999999.9;

typedef pair<int,int> AggloPair;

/*===================================================================*/
/*                       Class ClFastRNNCAgglo                       */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

ClFastRNNCAgglo2::ClFastRNNCAgglo2( const vector<FeatureVector> &vPoints,
                                  int nMaxNodeSize )
  : Cluster( vPoints )
  , m_btNNTree()
{
  m_nMetricType = METRIC_NGC;
  m_nMaxNodeSize= nMaxNodeSize;
}


/***********************************************************/
/*                     Initialization                      */
/***********************************************************/

void ClFastRNNCAgglo2::initDataVectors()
{
  m_vCenters.resize( m_nNumClusters );
  m_vBelongsTo.resize( m_nNumPoints );
  m_vVariances.resize( m_nNumClusters, 0.0 );
  
  m_vTrace.clear();
}


void ClFastRNNCAgglo2::initClusterCenters()
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


void ClFastRNNCAgglo2::initCovariances()
{}


void ClFastRNNCAgglo2::initPriors()
{}


void ClFastRNNCAgglo2::initPosteriors()
{
  //doReestimationStep();
}


float ClFastRNNCAgglo2::sim( int idx1, int idx2 )
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
      cerr << "Error in ClFastRNNCAgglo::sim(): "
           << "Unknown similarity measure (" << m_nMetricType << ")" << endl;
  }

  return sim;
}


void ClFastRNNCAgglo2::agglomerate( int idx1, int idx2, int newidx )
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

  /* adjust the balltree */
  //m_btNNTree.mergePoints( idx1, idx2, newidx, newCenter, newVar );
  m_btNNTree.updatePoint( newidx, newCenter, newVar );

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
bool ClFastRNNCAgglo2::doClusterSteps( double minSimilarity, bool bVerbose )
{
  // TIMING CODE
  time_t  tu0, tu1, tu2, tu3;
  double  tc0, tc1, tc2, tc3;  
  vector<time_t> vTu;
  vector<double> vTc;
  vector<float>  vPercDone;
  time(&tu0);
  tc0 = CPUTIME();

  m_dMinSimilarity = minSimilarity;
  m_bMore = true;
  if( bVerbose )
    cout << "  ClFastRNNCAgglo2::doClusterSteps() called with min similarity: " 
         << m_dMinSimilarity << endl;

  /****************************************/
  /* Build up a Balltree search structure */
  /****************************************/
  if( bVerbose )
    cout << "  Building up a balltree..." << endl;
  if( m_nMaxNodeSize>0 )
    m_btNNTree.build( m_vCenters, m_nMaxNodeSize );
  else
    m_btNNTree.build( m_vCenters );
  //if( !m_btNNTree.checkTree() ) {
  //  cout << "    FAILED!" << endl;
  //  exit(0);
  //}

  /************************/
  /* Start the clustering */
  /************************/
  if( bVerbose )
    cout << "  Clustering..." << endl;
  time(&tu1);
  tc1 = CPUTIME();

  int nNumPoints = m_vCenters.size();
  vector<int>   vNN       ( nNumPoints, -1 );
  vector<int>   vLastNN   ( nNumPoints, -1 );
  vector<bool>  vInactive ( nNumPoints, false );
  vector<bool>  vProcessed( nNumPoints, false );
  vector<float> vSim      ( nNumPoints, MIN_SIM );
  vector<float> vLastSim  ( nNumPoints, MIN_SIM );

  vector<AggloPair> vRNNs;

  /******************************/
  /* Start with the first point */
  /******************************/
  //int last = 0;
  //vNN[last] = 0;
  //vInactive[vNN[last]] = true;
  //vProcessed[vNN[last]] = true;
  //m_btNNTree.makeInactive( vNN[last] );
  //vSim[last]       = MIN_SIM;
  //int nFirstActive = 1;
  int nNumActive   = nNumPoints - 1;

  int nCurPass = 0;

  /***************/
  /* Agglomerate */
  /***************/
  while( nNumActive > 0 ) {
    /*============================*/
    /* Find the NN for each point */
    /*============================*/
    for( int i=0; i<nNumPoints; i++ )
      if( !vProcessed[i] ) {
        int   lastidx    = i;
        vProcessed[i]    = true;
        vNN[lastidx]     = -1; //i;
        //vLastNN[lastidx] = -1;
        vSim[lastidx]    = MIN_SIM;
        //vLastSim[lastidx]= MIN_SIM;

        bool  bFinished = false;
        while( !bFinished ) {
          /*-------------------------*/
          /* Find the NN for point i */
          /*-------------------------*/
//           int   nnidx = lastidx;
//           float nnsim = vSim[lastidx];
//           for( int i=0; i<nNumPoints; i++ )
//             if( !vInactive[i] ) {
//               float d = sim( lastidx, i );
//               if( d > nnsim ) {
//                 nnsim = d;
//                 nnidx = i;
//               }
//             }
          
          int      nnidx = vLastNN[lastidx]; //lastidx;
          float    nnsim = vLastSim[lastidx]; 
          unsigned nResult;
          float    dDist;
          bool     bSuccess;
          if( bSuccess=m_btNNTree.findNN( lastidx, -vLastSim[lastidx], 
                                          nResult, dDist ) ) {
            /* Found an NN in search volume */
            nnidx = (int)nResult;
            nnsim = -dDist;

            vNN [lastidx] = nnidx;
            vSim[lastidx] = nnsim;
            if( nnsim > vLastSim[nnidx] ) {
              /* found a better path to NN than previous points */
              vLastNN [nnidx] = lastidx;
              vLastSim[nnidx] = nnsim;
            }

          } else {
            /* No NN found in search volume */
            vNN [lastidx] = vLastNN[lastidx];
            vSim[lastidx] = vLastSim[lastidx];            
          }

          /* check if an NN has been found */
          if( nnidx == lastidx )
            cerr << "  WARNING: nnidx == lastidx (" << lastidx << ")!" << endl;
          if( bSuccess && (nnidx<0) )
            cerr << "  WARNING: nnidx < 0 (" << lastidx << ")!" << endl;
          

          /*--------------------------*/
          /* Check for reciprocal NNs */
          /*--------------------------*/
          if( bSuccess && (nnsim>vLastSim[lastidx]) && 
              (vNN[nnidx]!=lastidx) ) {
            /*-----------------------------------------*/
            /* No RNN => Add the point to the NN chain */
            /*-----------------------------------------*/
            //vInactive[nnidx]  = true;
            //m_btNNTree.makeInactive( nnidx );
            //nNumActive--;
            if( vNN[nnidx]==lastidx )
              cerr << "  Warning: points should be RNNs (" << lastidx << ","
                   << nnidx << "), NN=(" << vNN[nnidx] << "," 
                   << vNN[lastidx] << ")!" << endl;

            lastidx = nnidx;
            
            if( vProcessed[nnidx] )
              /* NN has already been reached from somewhere else */
              bFinished = true;

          } else
            /*----------------------------------------------------------*/
            /* RNN => check if the chain links are sufficiently similar */
            /*----------------------------------------------------------*/
            if( nnsim >= m_dMinSimilarity ) {
              /*-------------------------------------------------*/
              /* Mark the two last chain links for agglomeration */
              /*-------------------------------------------------*/
              if( vNN[lastidx] < 0 ) {   // no NN found
                cerr << "  WARNING: vNN[lastidx] <= 0 (" << lastidx 
                     << ")!!!" << endl;
              }

              AggloPair apNew;
              apNew.first  = lastidx;
              apNew.second = vNN[lastidx];
              vRNNs.push_back( apNew );
              bFinished = true;
              
            } else {
              /*-------------------*/
              /* Discard the chain */
              /*-------------------*/ 
              //last = -1;
              cerr << "  Warning: Found a chain with too low similarity ("
                   << nnsim << ")." << endl;
              bFinished = true;
            }
          
          /* mark the point as already seen */
          vProcessed[nnidx] = true;
        } // while
      } // for
  
    nCurPass++;
    cout << "\r    Finished pass " << nCurPass << "..." << flush;
    //cout << "\r      found " << vRNNs.size() << " RNN pairs. " << flush;

    /*===================================================*/
    /* Finished one pass => Agglomerate stored RNN pairs */
    /*===================================================*/
    if( vRNNs.size()==0 )
      /* found no RNN pair => stop clustering */
      break;

    else {
      for( unsigned i=0; i<vRNNs.size(); i++ ) {
        int newidx = vRNNs[i].first;
        vInactive[vRNNs[i].first]  = true;
        m_btNNTree.makeInactive( vRNNs[i].first );
        vInactive[vRNNs[i].second] = true;
        m_btNNTree.makeInactive( vRNNs[i].second );
        
        agglomerate( vRNNs[i].first, vRNNs[i].second, newidx );
        writeTrace( vRNNs[i].first, vRNNs[i].second, vSim[vRNNs[i].first], 
                    newidx );
      }
      
      /* Invalidate all NN links to agglomerated pairs */
      for( int i=0; i<nNumPoints; i++ ) {
        assert( vNN[i]>=0 );
        if( !vInactive[i] && vInactive[vNN[i]] ) {
          vNN[i]  = -1;
          vSim[i] = vLastSim[i];
          vProcessed[i] = false;
        }
      }
      
      /* Make the new clusters active again */
      for( unsigned i=0; i<vRNNs.size(); i++ ) {
        int newidx = vRNNs[i].first;
        vInactive[newidx] = false;
        m_btNNTree.makeActive( newidx );

        vNN[newidx]        = -1;
        vSim[newidx]       = MIN_SIM;
        vLastNN [newidx]   = -1;
        vLastSim[newidx]   = MIN_SIM;
        vProcessed[newidx] = false;
        nNumActive--;
      }
      
      /* Clear the old RNN pairs */
      vRNNs.clear();
    }

    /* collect timing information */
    cout << "\r    Finished pass " << nCurPass << ": " << setw(3) 
         << (int)floor(100.0*(nNumPoints-nNumActive)/(float)nNumPoints +0.5)
         << "%... " << flush;
    time_t tu;
    time(&tu);
    vTu.push_back( tu );
    vTc.push_back( CPUTIME() );
    vPercDone.push_back( 100.0*(nNumPoints-nNumActive)/(float)nNumPoints );

    /*=====================*/
    /* Rebuild the NN tree */
    /*=====================*/
    //if( nCurPass<5 )
    //  m_btNNTree.updateRadii();
    if( m_nMaxNodeSize>0 )
      m_btNNTree.rebuild( m_nMaxNodeSize );
    else
      m_btNNTree.rebuild();

    /* collect timing information */
    time(&tu);
    vTu.push_back( tu );
    vTc.push_back( CPUTIME() );
    vPercDone.push_back( 100.0*(nNumPoints-nNumActive)/(float)nNumPoints );
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
    cout << "    ClFastRNNCAgglo finished  " << endl;
    cout << "    ----------------------" << endl;
    cout << "    Time spent for..." << endl;
    cout << "      Balltree creation     : " << setw(12) 
         << tc1-tc0 << "s (system), " 
         << tu1-tu0 << "s (user)" << endl; 
    cout << "      Clustering            : " << setw(12) 
         << tc2-tc1 << "s (system), " 
         << tu2-tu1 << "s (user)" << endl; 
    
    cout << "        Time for " << setw(4) 
         << (int)floor(vPercDone[0] +0.5)
         << "%      : " << setw(12)
         << vTc[0]-tc1 << "s (system), " 
         << vTu[0]-tu1 << "s (user)" << endl; 
    for(unsigned i=1; i<vTu.size(); i++ )
      cout << "        Time for " << setw(4) 
           << (int)floor(vPercDone[i] +0.5)
           << "%      : " << setw(12)
           << vTc[i]-vTc[i-1] << "s (system), " 
           << vTu[i]-vTu[i-1] << "s (user)" << endl; 
    cout << "        Time for  100%      : " << setw(12)
         << tc2-vTc[vTu.size()-1] << "s (system), " 
         << tu2-vTu[vTu.size()-1] << "s (user)" << endl; 
    
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

void ClFastRNNCAgglo2::doReestimationStep()
{}



/***********************************************************/
/*                         Update                          */
/***********************************************************/

void ClFastRNNCAgglo2::doUpdateStep()
{
}


/***********************************************************/
/*                     Error Estimation                    */
/***********************************************************/

FLOAT ClFastRNNCAgglo2::calculateError()
{
  return -1.0;
}


/***********************************************************/
/*                     Output Functions                    */
/***********************************************************/

void ClFastRNNCAgglo2::printResults()
{
  //cout << "  ClFastRNNCAgglo::printResults() called." << endl;

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
