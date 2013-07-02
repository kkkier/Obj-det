/*********************************************************************/
/*                                                                   */
/* FILE         clpostagglo.cc                                       */
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

  
/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>

#include <math.h>
#include <stdlib.h>

#include <queue>

// #define NRANSI
// extern "C"
// {
// #include "nrutil.h"
// }

#include "resources.hh"
#include "clpostagglo.hh"
//#include "evaluate.hh"

/****************/
/* Definitions  */
/****************/

/*===================================================================*/
/*                         Class ClSimAgglomerative                  */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

ClPostAgglo::ClPostAgglo( const vector<FeatureVector> &vPoints, 
                          const vector<float> &mSimMatrix )
  : Cluster( vPoints )
{
  m_mSimMatrix = mSimMatrix;
  m_nSimMatrixDim = vPoints.size();

  if ( (int)m_mSimMatrix.size() != m_nSimMatrixDim*m_nSimMatrixDim ) {
    cerr << "ERROR in ClPostAgglo::ClPostAgglo(): "
         << "Wrong size of similarity matrix!" << endl;
    assert(false);
  }
}


/***********************************************************/
/*                     Initialization                      */
/***********************************************************/

void ClPostAgglo::initDataVectors()
{
  m_vCenters.resize( m_nNumClusters );
  m_vBelongsTo.resize( m_nNumPoints );
  
  m_vTrace.clear();
}


void ClPostAgglo::initClusterCenters()
{
  /* allocate the priority queue on the heap */
  m_pqSimQueue = 
    new priority_queue<ClSimilarity, vector<ClSimilarity>, ClSimCompare>;
  
  /* set the cluster centers initially (one to each point) */
  for(int i=0; i < m_nNumClusters; i++ ) {
    m_vCenters[i] = m_vPoints[i];
    m_vBelongsTo[i] = i;
    m_vValid.push_back( true );

    /* initialize all Feature Vectors to each corresponding cluster center */
    vector<int> tempFV;
    tempFV.push_back( i );
    m_vvAllBelongingFV.push_back( tempFV );
  }
}


void ClPostAgglo::initCovariances()
{}


void ClPostAgglo::initPriors()
{}


void ClPostAgglo::initPosteriors()
{
  //doReestimationStep();
}


/*************************************************************/
/*       Call update function for merging in each step       */
/*************************************************************/
bool ClPostAgglo::doClusterSteps( double minSimilarity, bool bVerbose )
{
  // TIMING CODE
  time_t  tu1, tu2, tu3;
  double  tc1, tc2, tc3;  
  time(&tu1);
  tc1 = CPUTIME();

  m_dMinSimilarity = minSimilarity;
  m_bMore = true;
  if( bVerbose )
    cout << "  input minSimilarity from doClusterSteps(): " 
         << m_dMinSimilarity << endl;

  /* compute the similarity index between all cluster centers */
  if( bVerbose )
    cout << "    transferring the similarities into the queue..." << endl;
  for(int i=0; i <(int)m_vCenters.size(); i++)
    for(int j=0; j <(int)m_vCenters.size(); j++) {
      if ( i > j ) {
        int idx = i*m_nSimMatrixDim + j;
        if( m_mSimMatrix[idx] >= m_dMinSimilarity ) {
          /* and store in queue */
          ClSimilarity currentClSim( m_mSimMatrix[idx], i, j );
          m_pqSimQueue->push ( currentClSim );
        }
      }
    }
  
  time(&tu2);
  tc2 = CPUTIME();

  if( bVerbose )
    cout << "  Clustering..." << endl;
  while ( m_bMore ) {
    doUpdateStep();
    m_fError = calculateError();
  }

  time(&tu3);
  tc3 = CPUTIME();

  if( bVerbose ) {
    cout << "    ----------------------" << endl;
    cout << "    ClPostAgglo finished  " << endl;
    cout << "    ----------------------" << endl;
    cout << "    Time spent for..." << endl;
    cout << "      Similarity Computation: " << setw(12) 
         << tc2-tc1 << "s (system), " 
         << tu2-tu1 << "s (user)" << endl; 
    cout << "      Clustering            : " << setw(12) 
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

void ClPostAgglo::doReestimationStep()
{}



/***********************************************************/
/*                         Update                          */
/***********************************************************/

void ClPostAgglo::doUpdateStep()
{
  /*********************************************************************/
  /* merge 2 neighbouring cluster centers if compactness is guaranteed */
  /*********************************************************************/
  if ( m_pqSimQueue->empty() ) {
    m_bMore = false;
    //return;
  }

  /* check if current minimal distance is between valid clusters */
  int a, b;
  if ( m_bMore ) {
    a = m_pqSimQueue->top().m_A;
    b = m_pqSimQueue->top().m_B;
    while ( (!m_vValid[a]) || (!m_vValid[b]) ) {
      m_pqSimQueue->pop();
      //cout << "  POPPED (a/b): " << a << "/" << b << endl; //TMP
      if ( m_pqSimQueue->empty() ) {
        m_bMore = false;
        break;
      }
      a = m_pqSimQueue->top().m_A;
      b = m_pqSimQueue->top().m_B;
    }
  }

  //cout << " FRONT Q(a/b):" << m_pqSimQueue->top().m_sim << "\t" 
  //     << a << "/" << b <<endl;


  /* merge two cluster centers if their similarity index is greater */
  /* than m_dMinSimilarity */
  double similarity;
  if ( m_bMore )
    similarity = m_pqSimQueue->top().m_sim;
  if ( m_bMore && (similarity > m_dMinSimilarity) ) {
    /* pop the top element of the queue */
    m_pqSimQueue->pop();
    
    /* compute the new cluster center */
    FeatureVector aCenter, bCenter, newCenter;
    aCenter = m_vCenters[a];
    bCenter = m_vCenters[b];
    aCenter.multFactor( (double)m_vvAllBelongingFV[a].size() );
    bCenter.multFactor( (double)m_vvAllBelongingFV[b].size() );
    newCenter = (aCenter + bCenter)/(double)(m_vvAllBelongingFV[a].size() + 
                                             m_vvAllBelongingFV[b].size() );
    
    /* 2 cluster centers will be invalid */
    m_vValid[a] = false;
    m_vValid[b] = false;
    /* append new cluster center */
    m_vCenters.push_back( newCenter );
    m_vValid.push_back( true );
    int newidx = (int)m_vCenters.size()-1;
    /* adjust the corresponding Feature Vectors to the new Center */
    for(int i=0; i < (int)m_vvAllBelongingFV[a].size(); i++) {
      //new cluster assignment
      m_vBelongsTo[ m_vvAllBelongingFV[a][i] ] = newidx; 
    }
    for(int i=0; i < (int)m_vvAllBelongingFV[b].size(); i++) {
      //new cluster assignment
      m_vBelongsTo[ m_vvAllBelongingFV[b][i] ] = newidx; 
    }

    /* write an entry to the cluster trace */
    writeTrace ( a, b, similarity, newidx );

    vector<int> fvecBothAB( m_vvAllBelongingFV[a] );
    for(int i=0; i < (int)m_vvAllBelongingFV[b].size(); i++) {
      fvecBothAB.push_back( m_vvAllBelongingFV[b][i] );
    }
    m_vvAllBelongingFV.push_back( fvecBothAB );

    
    /* fill in the newly computed similarities */
    int lastCenter = (int)m_vCenters.size() - 1;
   
    for(int i=0; i < lastCenter; i++) {
      if ( m_vValid[i] ) {
        double currentsim = 0.0;
        for(int k=0; k < (int)m_vvAllBelongingFV[i].size(); k++) {
          for(int l=0; l < (int)m_vvAllBelongingFV[lastCenter].size(); l++) {
            /* compute the new similarity value */
            int firstFV  = m_vvAllBelongingFV[i][k];
            int secondFV = m_vvAllBelongingFV[lastCenter][l];

            /* read out the value from m_mSimMatrix to be faster */
            if ( firstFV > secondFV ) {
              int idx = firstFV*m_nSimMatrixDim + secondFV;
              currentsim += m_mSimMatrix[idx];
            }
            else {
              int idx = secondFV*m_nSimMatrixDim + firstFV;
              currentsim += m_mSimMatrix[idx];
            }
            
          }
        }

        int totalFV = ( m_vvAllBelongingFV[i].size() * 
                        m_vvAllBelongingFV[lastCenter].size() );
        currentsim /= (double)totalFV;

        if( currentsim >= m_dMinSimilarity ) {
          /* store in queue */
          ClSimilarity currentClSim( currentsim, i, lastCenter );
          m_pqSimQueue->push( currentClSim );
        }
      }
    }
    
    //cout << "Current Sim: " << similarity << endl;//TEMP
    
  } else {   
    /* delete the queue from the heap */
    delete m_pqSimQueue;

    /* delete the allocated similarity matrix */
    //m_mSimMatrix.clear();

    /* prepare the result data */
    //cout << " Size of m_vCenters before is: " << m_vCenters.size() << endl;
    vector<FeatureVector> resultingCenters;
    vector<int> newIndices;
    int nofValidFalses = 0;
    for(int i=0; i < (int)m_vCenters.size(); i++) {
      if ( m_vValid[i] ) {
        resultingCenters.push_back( m_vCenters[i] );
        newIndices.push_back( i - nofValidFalses );
      }
      else {
        newIndices.push_back( -1 );
        nofValidFalses++;
      }
    }
    m_vCenters = resultingCenters;
    
    /* correct the following indices */
    for(int i=0; i < (int)m_vBelongsTo.size(); i++){
      m_vBelongsTo[i] = newIndices[ m_vBelongsTo[i] ];
    }
    
    /* stop agglomerative clustering */
    m_bMore = false;
    //cout << " Last similarity is: " << similarity << endl;
    //cout << " Size of m_vBelongsTo after is: " << m_vBelongsTo.size() << endl;
    //cout << " Size of m_vCenters after is: " << m_vCenters.size() << endl;
    //cout << " Similarity based Agglomerative clustering finished." << endl;
  }
}


/***********************************************************/
/*                     Error Estimation                    */
/***********************************************************/

FLOAT ClPostAgglo::calculateError()
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

void ClPostAgglo::printResults()
{
  //cout << "  ClPostAgglo::printResults() called." << endl;

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

