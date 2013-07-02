/*********************************************************************/
/*                                                                   */
/* FILE         mcmatcher-experiments2.cc                            */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Interleaved Object Categorization and Segmentation   */
/*              with an Implicit Shape Model.                        */
/*                                                                   */
/*              This file contains some experimental methods.        */
/*                                                                   */
/* COPYRIGHT    Bastian Leibe, TU Darmstadt, 2005.                   */
/*                                                                   */
/* BEGIN        Wed Feb 23 2005                                      */
/* LAST CHANGE  Tue Oct 11 2005                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <string>
#include <algorithm>

#include <qfiledialog.h>
#include <opgrayimage.hh>
#include <qtimgbrowser.hh>

#include <resources.hh>
#include <nnsearch.hh>
#include <balltree.hh>
//#include <kballtree.hh>

#include "kballtree/kbt/kbt.h"
#include "mcmatcher.hh"

using namespace KM;


void testMatchingLinear   ( const vector<FeatureVector> &vFeatures,
                            const vector<FeatureVector> &vClusters,
                            float dRadius,
                            vector<vector<unsigned> >   &vvMatches );
void testMatchingBTAnchors( const vector<FeatureVector> &vFeatures,
                            const vector<FeatureVector> &vClusters,
                            float dRadius,
                            vector<vector<unsigned> >   &vvMatches );
void testMatchingBTTrace  ( const vector<FeatureVector> &vFeatures,
                            const vector<FeatureVector> &vClusters,
                            const vector<ClStep>        &vTrace,
                            float dRadius, float dReduction, 
                            float dMaxLevel, int nLevels, float dQuantile,
                            float dMatchThresh,
                            bool bDisplayTree,
                            vector<vector<unsigned> >   &vvMatches );
void testMatchingBTKM     ( const vector<FeatureVector> &vFeatures,
                            const vector<FeatureVector> &vClusters,
                            const vector<ClStep>        &vTrace,
                            float dRadius, float dReduction, 
                            float dMaxLevel, int nLevels, float dQuantile,
                            float dMatchThresh,
                            vector<vector<unsigned> >   &vvMatches );
void testMatchingNN       ( /*const*/ vector<FeatureVector> &vFeatures,
                            const vector<FeatureVector> &vClusters,
                            float dRadius,
                            vector<vector<int> >        &vvMatches );

void cnvDescriptorsToFeatVec( const vector<CornerDescriptor*> vDesc, 
                              vector<FeatureVector>          &vPoints,
                              int dim );
void cnvFeatVecToDescriptors( const vector<FeatureVector> &vPoints, 
                              vector<CornerDescriptor *>  &vDesc );

void verifyMatches( const vector<vector<unsigned> > &vvCorrect, 
                    const vector<vector<unsigned> > &vvMatches,
                    long &nNumCorrect, long &nNumInserted, long &nNumMissed );
void verifyMatches( const vector<vector<unsigned> > &vvCorrect, 
                    const vector<vector<int> >      &vvMatches,
                    long &nNumCorrect, long &nNumInserted, long &nNumMissed );



void ISMReco::testMatching( unsigned nDet, unsigned nCue )
{
  if( nDet>=m_nNumDetectors ) {
    cerr << "ERROR in ISMReco::testMatching(): "
         << "Detector " << nDet << " doesn't exist!" << endl;
    return;
  }

  Detector &det = m_vDetectors[nDet];
  if( nCue>=det.m_nNumCues ) {
    cerr << "ERROR in ISMReco::testMatching(): "
         << "Detector " << nDet << " doesn't have a cue " << nCue << "!" 
         << endl;
    return;
  }

  cout << endl
       << "===============================" << endl
       << "Testing the Matching run-times " << endl
       << "===============================" << endl;
     
  vector<FeatureVector> vFeatures = (*det.m_vCodebooks[nCue]).getFeatures();
  vector<FeatureVector> vClusters = (*det.m_vCodebooks[nCue]).getClusters();
  vector<ClStep>        vTrace    = (*det.m_vCodebooks[nCue]).getClusterTrace();

  /* matching radius for Euclidean distances */
  float dFeatureSimFact  = det.m_vParMatching[nCue].params()->m_dFeatureSimFact;
  float dRejectionThresh = det.m_vParMatching[nCue].params()->m_dRejectionThresh;
  float dDimFact         = (float)vFeatures.front().numDims();
  float dRadius          = sqrt( -dFeatureSimFact*dDimFact * 
                                 log(dRejectionThresh) );

  long nNumCorrect  = 0;
  long nNumInserted = 0;
  long nNumMissed   = 0;

  /**************************/
  /* Test the Linear Search */
  /**************************/
  vector<vector<unsigned> > vvMatchesCorrect;
  testMatchingLinear( vFeatures, vClusters, dRadius, vvMatchesCorrect );

  cout << "-------------------------------" << endl;

  /*************************************/
  /* Test the Balltree(Anchors) Search */
  /*************************************/
  vector<vector<unsigned> > vvMatchesBTAnchors;
//   testMatchingBTAnchors( vFeatures, vClusters, dRadius, vvMatchesBTAnchors );

  /* Verify the matches */
//   verifyMatches( vvMatchesCorrect, vvMatchesBTAnchors, 
//                  nNumCorrect, nNumInserted, nNumMissed );

  cout << "-------------------------------" << endl;


  /***********************************/
  /* Test the Balltree(Trace) Search */
  /***********************************/
  //float dMaxLevel  = dRadius*dRadius;
  int   nLevels    = 10;
  int   nLevels2   = 8;
  int   nLevels3   = 6;
  float dQuant1    = 0.10;
  float dQuant2    = 0.01;
  float dQuant3    = 0.001;
  vector<vector<unsigned> > vvMatchesBTTrace;
//   testMatchingBTTrace( vFeatures, vClusters, vTrace, dRadius, 1.0, -1.0, 
//                        nLevels, 1.0, dRadius,
//                        false, vvMatchesBTTrace );

//   /* Verify the matches */
//   verifyMatches( vvMatchesCorrect, vvMatchesBTTrace, 
//                  nNumCorrect, nNumInserted, nNumMissed );

//   cout << "-------------------------------" << endl;

//   vvMatchesBTTrace.clear();
//   testMatchingBTTrace( vFeatures, vClusters, vTrace, dRadius, 1.0, -1.0, 
//                        nLevels, dQuant1,
//                        false, vvMatchesBTTrace );

//   /* Verify the matches */
//   verifyMatches( vvMatchesCorrect, vvMatchesBTTrace, 
//                  nNumCorrect, nNumInserted, nNumMissed );

  cout << "-------------------------------" << endl;

//   vvMatchesBTTrace.clear();
//   testMatchingBTTrace( vFeatures, vClusters, vTrace, dRadius, 1.0, -1.0, 
//                        nLevels, dQuant2,
//                        false, vvMatchesBTTrace );

//   /* Verify the matches */
//   verifyMatches( vvMatchesCorrect, vvMatchesBTTrace, 
//                  nNumCorrect, nNumInserted, nNumMissed );

  cout << "-------------------------------" << endl;

  vvMatchesBTTrace.clear();
  testMatchingBTTrace( vFeatures, vClusters, vTrace, dRadius, 1.0, -1.0, 
                       nLevels, dQuant3, dRadius,
                       false, vvMatchesBTTrace );

  /* Verify the matches */
  verifyMatches( vvMatchesCorrect, vvMatchesBTTrace, 
                 nNumCorrect, nNumInserted, nNumMissed );

  cout << "-------------------------------" << endl;

  vvMatchesBTTrace.clear();
  testMatchingBTKM( vFeatures, vClusters, vTrace, dRadius, 1.0, -1.0, 
                    nLevels, 1.0, dRadius,
                    vvMatchesBTTrace );

  /* Verify the matches */
  verifyMatches( vvMatchesCorrect, vvMatchesBTTrace, 
                 nNumCorrect, nNumInserted, nNumMissed );

  cout << "-------------------------------" << endl;

//   vvMatchesBTTrace.clear();
//   testMatchingBTTrace( vFeatures, vClusters, vTrace, dRadius, 1.0, 
//                        dRadius*dRadius*4,
//                        nLevels2, dQuant3,
//                        false, vvMatchesBTTrace );

//   /* Verify the matches */
//   verifyMatches( vvMatchesCorrect, vvMatchesBTTrace, 
//                  nNumCorrect, nNumInserted, nNumMissed );

//   cout << "-------------------------------" << endl;

//   vvMatchesBTTrace.clear();
//   testMatchingBTTrace( vFeatures, vClusters, vTrace, dRadius, 1.0, 
//                        dRadius*dRadius*4,
//                        nLevels3, dQuant3,
//                        false, vvMatchesBTTrace );

//   /* Verify the matches */
//   verifyMatches( vvMatchesCorrect, vvMatchesBTTrace, 
//                  nNumCorrect, nNumInserted, nNumMissed );

//   cout << "-------------------------------" << endl;

//   vvMatchesBTTrace.clear();
//   testMatchingBTTrace( vFeatures, vClusters, vTrace, dRadius, 1.0,
//                        dRadius*dRadius*4,
//                        nLevels, dQuant3,
//                        false, vvMatchesBTTrace );

//   /* Verify the matches */
//   verifyMatches( vvMatchesCorrect, vvMatchesBTTrace, 
//                  nNumCorrect, nNumInserted, nNumMissed );

//   cout << "-------------------------------" << endl;

  /******************************/
  /* Test the Nene/Nayar Search */
  /******************************/
  vector<vector<int> > vvMatchesNN;
//   testMatchingNN( vFeatures, vClusters, dRadius, vvMatchesNN );  

//   verifyMatches( vvMatchesCorrect, vvMatchesNN, 
//                  nNumCorrect, nNumInserted, nNumMissed );

  cout << "-------------------------------" << endl;



//   /*************************************/
//   /* Test the Balltree(Trace) Search */
//   /*************************************/
//   cout << "  Testing the Balltree Search (Trace): " << endl
//        << "    #features: " << nNumFeatures << endl
//        << "    #clusters: " << vClusters.size() << endl
//        << "    #dims    : " << vFeatures[0].numDims() << endl
//        << "    Matching radius: " << dRadius << endl
//        << endl;

//   /*-----------------------*/
//   /* Build up the balltree */
//   /*-----------------------*/
//   cout << "    Building up a balltree from the trace...  " << flush;

//   // TIMING CODE
//   time(&tu1);
//   tc1 = CPUTIME();
  
//   // for GLOH128
//   // Cluster:  7M
//   // Matching: 5M

//   KBallTree kbtNNTree;
//   float dMinThresh = dRadius2;
//   float dMaxThresh = 10.0*dMinThresh;
//   int   nLevels    = 10;
//   float dQuant     = 0.999;
//   kbtNNTree.build( vFeatures, vTrace, dMinThresh, dMaxThresh, nLevels, 
//                    dQuant );


//   // TIMING CODE
//   time(&tu2);
//   tc2 = CPUTIME();

//   cout << "\r    Building up a balltree from the trace...  " << flush;
//   cout << setw(12) << tc2-tc1 << "s (system), "
//        << tu2-tu1 << "s (user)" << endl;
  
//   /*---------------------------*/
//   /* Match the features (AllN) */
//   /*---------------------------*/
//   dPercentMatched = 1.0;

//   cout << "    Finding all neighbors (exact)...     " << flush;
  
//   vector<CornerDescriptor*> vEmptyCD;
//   vector<vector<CornerDescriptor*> > vvMatchesKBallTree( vFeatures.size(),
//                                                              vEmptyCD );

//   // TIMING CODE
//   time(&tu1);
//   tc1 = CPUTIME();
  
//   long nNumFoundKBallTree = 0;
//   for( unsigned i=0; i<vFeatures.size(); i++ ) {
//     vvMatchesKBallTree[i] = kbtNNTree.findNeighbors2( vFeatures[i], 
//                                                       dRadius2, 1 );
//     nNumFoundKBallTree += vvMatchesBallTree[i].size();
//   }

//   // TIMING CODE
//   time(&tu2);
//   tc2 = CPUTIME();
  
//   cout << setw(12) << tc2-tc1 << "s (system), "
//        << tu2-tu1 << "s (user)" << endl;

//   cout << "      found " << nNumFoundKBallTree << " matches." << endl
//        << endl;

//   cout << "    Verifying... " << flush;
//   nNumCorrect  = 0;
//   nNumInserted = 0;
//   nNumMissed   = 0;
//   float dEps2  = dDimFact;
//   for( unsigned i=0; i<nNumFeatures; i++ ) {
//     /* get the two vectors */
//     vector<unsigned>      vCorrect = vvMatchesCorrect[i];
//     vector<bool>          vMatched( vCorrect.size(), false );
//     vector<FeatureVector> vResult; 
//     kbtNNTree.cnvDescriptorsToFeatureVecs( vvMatchesKBallTree[i],
//                                            vResult );
    
//     cout << "\r    Verifying... " << setw(6) << i+1 << "/" << nNumFeatures 
//          << flush;

//     if( vCorrect.size()==0 )
//       continue;

//     /* verify the results */
//     for( unsigned j=0; j<vResult.size(); j++ ) {
//       int   nMinIdx  = -1;
//       float dMinDist = 9999999999999999999.9; 
//       for( unsigned k=0; k<vCorrect.size(); k++ )
//         if( !vMatched[k] ) {
//           float dDist = vResult[j].compSSD( vClusters[vCorrect[k]] );
//           if( dDist < dMinDist ) {
//             dMinDist = dDist;
//             nMinIdx = k;
//           }
//         }

//       if( nMinIdx>=0 )
//         if( dMinDist <= dEps2 ) { // found a match
//           vMatched[nMinIdx] = true;
//           nNumCorrect++;
//         } else                 // feature not in correct set
//           nNumInserted++;
//     }

//     for( unsigned k=0; k<vMatched.size(); k++ )
//       if( !vMatched[k] )
//         nNumMissed++;
//   }
//   cout << " done." << endl
//        << "      #Correct : " << nNumCorrect << endl
//        << "      #Inserted: " << nNumInserted << endl
//        << "      #Missed  : " << nNumMissed << endl;

//   cout << "-------------------------------" << endl;


//   /******************************/
//   /* Test the Nene/Nayar Search */
//   /******************************/
//   //float dEps = sqrt((dRadius*dRadius)/dDimFact);
//   float dEps = sqrt((dRadius*dRadius)/sqrt(dDimFact));
//   //float dEps = dRadius;
//   cout << "  Testing the Nene/Nayar search: " << endl
//        << "    #features: " << nNumFeatures << endl
//        << "    #clusters: " << vClusters.size() << endl
//        << "    #dims    : " << vFeatures[0].numDims() << endl
//        << "    Matching radius : " << dRadius << endl
//        << "    Radius/dimension: " << dEps << endl
//        << endl;

//   /*------------------------*/
//   /* Build up the hypercube */
//   /*------------------------*/
//   cout << "    Building up the hypercube structure...  " << flush;

//   // TIMING CODE
//   time(&tu1);
//   tc1 = CPUTIME();
  
//   NNSearch nnNNCube;
//   nnNNCube.init( vFeatures );

//   // TIMING CODE
//   time(&tu2);
//   tc2 = CPUTIME();

//   cout << "\r    Building up the hypercube structure..." << flush;
//   cout << setw(12) << tc2-tc1 << "s (system), "
//        << tu2-tu1 << "s (user)" << endl;
  
//   /*----------------------------------*/
//   /*  Verify the hypercube parameters */
//   /*----------------------------------*/
//   cout << "    Verifying the hypercube radius...     " << flush;

//   // TIMING CODE
//   time(&tu1);
//   tc1 = CPUTIME();
  
//   unsigned nNumToTest = max( 100, (int)nNumFeatures/20 );
//   float    dMaxDimDist = 0.0;
//   for( unsigned i=0; i<nNumToTest; i++ )
//     for( unsigned j=0; j<vvMatchesCorrect[i].size(); j++ ) {
//       FeatureVector fvDist = ( vFeatures[i] - 
//                                vClusters[vvMatchesCorrect[i][j]] );
//       for( unsigned k=0; k<fvDist.numDims(); k++ )
//         if( fabs(fvDist.at(k)) > dMaxDimDist )
//           dMaxDimDist = fabs(fvDist.at(k));
//     }

//   // TIMING CODE
//   time(&tu2);
//   tc2 = CPUTIME();

//   cout << "\r    Verifying the hypercube radius...     " << flush;
//   cout << setw(12) << tc2-tc1 << "s (system), "
//        << tu2-tu1 << "s (user)" << endl;

//   cout << "      eps=" << dMaxDimDist << endl;
//   dEps = dMaxDimDist;

//   /*-------------------------*/
//   /* Match the features (NN) */
//   /*-------------------------*/
//   cout << "    Finding nearest neighbors (exact)... " << flush;
  
//   // TIMING CODE
//   time(&tu1);
//   tc1 = CPUTIME();
  
//   for( unsigned i=0; i<vFeatures.size(); i++ ) {
//     float    dMinDist = 999999999999999.9;
//     unsigned nMinIdx = 0;

//     nMinIdx = nnNNCube.getNN( vFeatures[i], dRadius, dEps );
//   }

//   // TIMING CODE
//   time(&tu2);
//   tc2 = CPUTIME();
  
//   cout << setw(12) << tc2-tc1 << "s (system), "
//        << tu2-tu1 << "s (user)" << endl;

//   /*---------------------------*/
//   /* Match the features (AllN) */
//   /*---------------------------*/
//   cout << "    Finding all neighbors (exact)...     " << flush;
  
//   vector<vector<int> > vvMatchesNene( vFeatures.size(), vEmptyInt );

//   // TIMING CODE
//   time(&tu1);
//   tc1 = CPUTIME();
  
//   long nNumFoundNene = 0;
//   for( unsigned i=0; i<vFeatures.size(); i++ ) {
//     vvMatchesNene[i] = nnNNCube.getNNIdzs( vFeatures[i], dRadius, dEps );
//     nNumFoundNene += vvMatchesNene[i].size();
//   }

//   // TIMING CODE
//   time(&tu2);
//   tc2 = CPUTIME();
  
//   cout << setw(12) << tc2-tc1 << "s (system), "
//        << tu2-tu1 << "s (user)" << endl;

//   cout << "      found " << nNumFoundNene << " matches." << endl
//        << endl;

//   cout << "    Verifying... " << flush;
//   nNumCorrect  = 0;
//   nNumInserted = 0;
//   nNumMissed   = 0;
//   for( unsigned i=0; i<vFeatures.size(); i++ ) {
//     /* get the two vectors */
//     vector<unsigned> vCorrect = vvMatchesCorrect[i];
//     vector<int>      vResult  = vvMatchesNene[i];
    
//     /* sort the two vectors */
//     sort( vCorrect.begin(), vCorrect.end() );
//     sort( vResult.begin(), vResult.end() );

//     cout << "\r    Verifying... " << setw(6) << i+1 << "/" << nNumFeatures 
//          << flush;

//     /* verify the results */
//     vector<unsigned>::iterator itCorrect = vCorrect.begin();
//     vector<int>::iterator      itResult  = vResult.begin();
//     while( !(itCorrect==vCorrect.end() && itResult==vResult.end()) ) {
//       if( itCorrect==vCorrect.end() )
//         while( itResult!=vResult.end() ) {
//           nNumInserted++;
//           itResult++;
//         }
//       else if( itResult==vResult.end() )
//         while( itCorrect!=vCorrect.end() ) {
//           nNumMissed++;
//           itCorrect++;
//         }
//       else {
//         if( *itCorrect < *itResult ) {
//           nNumMissed++;
//           itCorrect++;
//         } else if( *itCorrect == *itResult ) {
//           nNumCorrect++;
//           itCorrect++;
//           itResult++;
//         } else {
//           nNumInserted++;
//           itResult++;
//         }
//       }
//     }
//   }
//   cout << " done." << endl
//        << "      #Correct : " << nNumCorrect << endl
//        << "      #Inserted: " << nNumInserted << endl
//        << "      #Missed  : " << nNumMissed << endl;

//   cout << "-------------------------------" << endl;

}


void testMatchingLinear( const vector<FeatureVector> &vFeatures,
                         const vector<FeatureVector> &vClusters,
                         float dRadius,
                         vector<vector<unsigned> >   &vvMatches )
{
  unsigned nNumFeatures  = vFeatures.size();
  float dRadius2 = dRadius*dRadius;
  cout << "  Testing the Linear Search: " << endl
       << "    #features: " << nNumFeatures << endl
       << "    #clusters: " << vClusters.size() << endl
       << "    #dims    : " << vFeatures[0].numDims() << endl
       << "    Matching radius: " << dRadius << endl
       << "    Squared  radius: " << dRadius2 << endl
       << endl;

  // TIMING CODE
  time_t  tu1, tu2;
  double  tc1, tc2;

  int   k=0;
  float dPercentMatched = 1.0 - k*0.01;
  
  vector<unsigned> vEmpty;
  vector<vector<unsigned> > vvTmp( vFeatures.size(), vEmpty );
  vvMatches = vvTmp;

  cout << "    Matching to " << setw(3) 
       << (int)floor(dPercentMatched*100.0+0.5) 
       << "% of the clusters... " << flush;
  
  unsigned nNumClusters = (unsigned)round(vClusters.size()*dPercentMatched);

  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();

  long nNumFoundLinear = 0;
  for( unsigned i=0; i<vFeatures.size(); i++ ) {
    //float dMinDist = vFeatures[i].compSSD( vClusters[0] );
    //int   nMinIdx  = 0;
    vvMatches[i].clear();
    vvMatches[i].reserve( 20 );
    for( unsigned j=1; j<nNumClusters; j++ ) {
      float dDist = vFeatures[i].compSSD( vClusters[j] );
      //if( dDist < dMinDist ) {
      //  dMinDist = dDist;
      //  nMinIdx = j;
      //}
      if( dDist <= dRadius2 )
        vvMatches[i].push_back( j );
    }
    nNumFoundLinear += vvMatches[i].size();
  }
  
  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();

    cout << setw(12) << tc2-tc1 << "s (system), "
         << tu2-tu1 << "s (user)" << endl;

    cout << "      found " << nNumFoundLinear << " matches." << endl;
    //}
}


void testMatchingBTAnchors( const vector<FeatureVector> &vFeatures,
                            const vector<FeatureVector> &vClusters,
                            float dRadius,
                            vector<vector<unsigned> >   &vvMatches )
{
  float dRadiusBT  = 1.0*dRadius;
  float dRadiusBT2 = dRadiusBT*dRadiusBT;
  unsigned nNumFeatures  = vFeatures.size();
  cout << "  Testing the Balltree Search (Anchors): " << endl
       << "    #features: " << nNumFeatures << endl
       << "    #clusters: " << vClusters.size() << endl
       << "    #dims    : " << vFeatures[0].numDims() << endl
       << "    Matching radius: " << dRadius << endl
       << "    Approx.  radius: " << dRadiusBT << endl
       << endl;

  // TIMING CODE
  time_t  tu1, tu2;
  double  tc1, tc2;

  /*-----------------------*/
  /* Build up the balltree */
  /*-----------------------*/
  int nMaxNodeSize = 400;
  cout << "    Building up an anchors balltree...  " << flush;

  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  BallTree btNNTree;
  btNNTree.build( vClusters, nMaxNodeSize );

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();

  cout << "\r    Building up an anchors balltree...   " << flush;
  cout << setw(12) << tc2-tc1 << "s (system), "
       << tu2-tu1 << "s (user)" << endl;
  
  // check the tree
  //btNNTree.checkTree();

  // check 
  //cout << "   Checking a random node..." << endl; 
  //btNNTree.printStorageNodeDist( 0, 1 );

  /*-------------------------*/
  /* Match the features (NN) */
  /*-------------------------*/
  float dPercentMatched = 1.0;

  cout << "    Finding nearest neighbors (exact)... " << flush;
  
  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  for( unsigned i=0; i<vFeatures.size(); i++ ) {
    float    dMinDist = 999999999999999.9;
    unsigned nMinIdx = 0;

    btNNTree.findNN( vFeatures[i], dRadiusBT2, nMinIdx, dMinDist );
  }

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();
  
  cout << setw(12) << tc2-tc1 << "s (system), "
       << tu2-tu1 << "s (user)" << endl;

  /*---------------------------*/
  /* Match the features (AllN) */
  /*---------------------------*/
  dPercentMatched = 1.0;

  cout << "    Finding all neighbors (exact)...     " << flush;
  
  vector<unsigned> vEmpty;
  vector<vector<unsigned> > vvTmp( nNumFeatures, vEmpty );
  vvMatches = vvTmp;

  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  long nNumFoundBallTree = 0;
  for( unsigned i=0; i<vFeatures.size(); i++ ) {
    vvMatches[i]       = btNNTree.findNeighbors( vFeatures[i], dRadiusBT2 );
    nNumFoundBallTree += vvMatches[i].size();
  }

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();
  
  cout << setw(12) << tc2-tc1 << "s (system), "
       << tu2-tu1 << "s (user)" << endl;

  cout << "      found " << nNumFoundBallTree << " matches." << endl
       << endl;
}


void testMatchingBTTrace( const vector<FeatureVector> &vFeatures,
                          const vector<FeatureVector> &vClusters,
                          const vector<ClStep>        &vTrace,
                          float dRadius, float dReduction, 
                          float dMaxLevel, int nLevels, float dQuantile,
                          float dMatchThresh,
                          bool bDisplayTree,
                          vector<vector<unsigned> >   &vvMatches )
{
  float dRadius2      = dRadius*dRadius;
  float dRadiusBT     = dReduction*dMatchThresh;
  float dRadiusBT2    = dRadiusBT*dRadiusBT;
  float dMatchThresh2 = dMatchThresh*dMatchThresh;
  unsigned nNumFeatures  = vFeatures.size();
  bool bReinsert = false;
  cout << "  Testing the Balltree Search (Trace): " << endl
       << "    #features: " << nNumFeatures << endl
       << "    #clusters: " << vClusters.size() << endl
       << "    #dims    : " << vFeatures[0].numDims() << endl
       << "    #levels  : " << nLevels << endl
       << "    Clustering radius: " << dRadius << endl
       << "    Matching radius  : " << dMatchThresh << endl
       << "    Approx.  radius  : " << dRadiusBT << endl
       << "    Quantile : " << dQuantile << endl
       << endl;

  // TIMING CODE
  time_t  tu1, tu2;
  double  tc1, tc2;

  /*-----------------------*/
  /* Build up the balltree */
  /*-----------------------*/
  cout << "    Building up a balltree from the trace...  " << endl; //flush;

  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  BallTree kbtNNTree;
  //float dMinThresh = dRadiusBT2;
  //float dMaxThresh = 10.0*dMinThresh;
  //float dQuant     = 0.999;
  kbtNNTree.buildFromClusterTrace( vFeatures, vTrace, -dRadius2, dMaxLevel,
                                   nLevels, dQuantile, false ); 

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();

  cout << "\r    Building up a balltree from the trace...  " << flush;
  cout << setw(12) << tc2-tc1 << "s (system), "
       << tu2-tu1 << "s (user)" << endl;

  // remove all points and insert them again
  if( bReinsert ) {
    cout << "    Removing all points and inserting them again... " << endl;
    kbtNNTree.clearPoints();
    for(unsigned i=0; i<vClusters.size(); i++ )
      kbtNNTree.insertPoint( vClusters[i] );
    cout << "    Updating the node radii... " << endl;
    kbtNNTree.updateRadii( dQuantile );
  }

  if( bDisplayTree ) {
    // check the tree
    kbtNNTree.checkTree();
    
    // check 
    cout << "   Checking a random node..." << endl; 
    kbtNNTree.printStorageNodeDist( 0, 1 );
  }

  /*---------------------------*/
  /* Match the features (AllN) */
  /*---------------------------*/
  //float dPercentMatched = 1.0;

  cout << "    Finding all neighbors (exact)...     " << flush;
  
  vector<unsigned> vEmpty;
  vector<vector<unsigned> > vvTmp( vFeatures.size(), vEmpty );
  vvMatches = vvTmp;

  // clear the matching statistics
  kbtNNTree.clearStats();

  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  long nNumFoundBallTree = 0;
  for( unsigned i=0; i<vFeatures.size(); i++ ) {
    vvMatches[i]       = kbtNNTree.findNeighbors( vFeatures[i], dMatchThresh2);
    nNumFoundBallTree += vvMatches[i].size();
  }

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();
  
  cout << setw(12) << tc2-tc1 << "s (system), "
       << tu2-tu1 << "s (user)" << endl;

  cout << "      found " << nNumFoundBallTree << " matches." << endl;

  // print the matching statistics
  long nCntChecked, nCntVisited, nCntDists;
  kbtNNTree.getStats( nCntChecked, nCntVisited, nCntDists );
  cout << "      avg #nodes checked : " << nCntChecked/(float)nNumFeatures 
       << endl
       << "      avg #nodes visited : " << nCntVisited/(float)nNumFeatures
       << endl 
       << "      avg #points checked: " << nCntDists/(float)nNumFeatures 
       << endl
       << endl;
}



void testMatchingBTKM( const vector<FeatureVector> &vFeatures,
                       const vector<FeatureVector> &vClusters,
                       const vector<ClStep>        &vTrace,
                       float dRadius, float dReduction, 
                       float dMaxLevel, int nLevels, float dQuantile,
                       float dMatchThresh,
                       vector<vector<unsigned> >   &vvMatches )
{
  float dRadius2      = dRadius*dRadius;
  float dRadiusBT     = dReduction*dMatchThresh;
  float dRadiusBT2    = dRadiusBT*dRadiusBT;
  float dMatchThresh2 = dMatchThresh*dMatchThresh;
  unsigned nNumFeatures  = vFeatures.size();
  bool bReinsert = false;
  cout << "  Testing the Balltree Search (Krystian): " << endl
       << "    #features: " << nNumFeatures << endl
       << "    #clusters: " << vClusters.size() << endl
       << "    #dims    : " << vFeatures[0].numDims() << endl
       << "    #levels  : " << nLevels << endl
       << "    Clustering radius: " << dRadius << endl
       << "    Matching radius  : " << dMatchThresh << endl
       << "    Approx.  radius  : " << dRadiusBT << endl
       << "    Quantile : " << dQuantile << endl
       << endl;

  // TIMING CODE
  time_t  tu1, tu2;
  double  tc1, tc2;

  /*-----------------------*/
  /* Build up the balltree */
  /*-----------------------*/
  // convert the feature vectors
  vector<CornerDescriptor*> vDesc;
  vector<CornerDescriptor*> vTest;
  vector<CornerDescriptor*> vNodes;

  cout << "    Converting the features... " << endl;
  cnvFeatVecToDescriptors( vFeatures, vDesc );
  cnvFeatVecToDescriptors( vFeatures, vTest );

  cout << "    Building up a balltree from the trace...  " << endl; //flush;

  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  //KBallTree kbtNNTree;
  float dMinThresh = dRadiusBT2;
  float dMaxThresh = 6.0*dMinThresh;
  uint  nMinClSize = 2;
//   KM::traceToBallTree( vTrace, vDesc, vNodes, dMinThresh, dMaxThresh,
//                        nLevels, dQuantile ); 
  KM::traceToBallTree( vDesc, vTrace, vNodes, dMinThresh, (uint)nLevels,
                       dMaxThresh, dQuantile, nMinClSize ); 

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();

  cout << "\r    Building up a balltree from the trace...  " << flush;
  cout << setw(12) << tc2-tc1 << "s (system), "
       << tu2-tu1 << "s (user)" << endl;

  /*---------------------------*/
  /* Match the features (AllN) */
  /*---------------------------*/
  //float dPercentMatched = 1.0;

  cout << "    Finding all neighbors (exact)...     " << flush;
  
  vector<unsigned> vEmpty;
  vector<vector<unsigned> > vvTmp( vFeatures.size(), vEmpty );
  vvMatches = vvTmp;

   vector<CornerDescriptor*> vEmptyCD;
   vector<vector<CornerDescriptor*> > vvMatchesKBT( vFeatures.size(),
                                                        vEmptyCD );

  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  long nNumFoundBallTree = 0;
  for( unsigned i=0; i<vFeatures.size(); i++ ) {
    KM::testBallTree( vTest[i], vNodes, dMatchThresh2, vvMatchesKBT[i], 1 );
    nNumFoundBallTree += vvMatchesKBT[i].size();
  }

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();
  
  cout << setw(12) << tc2-tc1 << "s (system), "
       << tu2-tu1 << "s (user)" << endl;

  cout << "      found " << nNumFoundBallTree << " matches." << endl;

  // convert the returned matches back
  cout << "    Converting the matching descriptors back... " << endl;
  for( unsigned i=0; i<vFeatures.size(); i++ ) {
    vector<FeatureVector> vMatches;
    cnvDescriptorsToFeatVec( vvMatchesKBT[i], vMatches, 
                             vFeatures[0].numDims() );

    float dMinDist = 9999999999999999999999.9;
    int   nMinIdx = -1;
    for(unsigned k=0; k<vMatches.size(); k++ ) {
      for(unsigned j=0; j<vClusters.size(); j++ ) {
        float dDist = vMatches[k].compSSD( vClusters[j] );
        if( dDist<dMinDist ) {
          dMinDist = dDist;
          nMinIdx  = j;
        }
      }
      vvMatches[i].push_back( nMinIdx );
    }
  } 
}


void cnvDescriptorsToFeatVec( const vector<CornerDescriptor*> vDesc, 
                              vector<FeatureVector>          &vPoints,
                              int dim )
  // Convert the KM descriptors to a vector of feature vectors      
{
  float *vec;
  vPoints.clear();
  for( unsigned i=0; i<vDesc.size(); i++ ){
    vPoints.push_back( FeatureVector(dim) );
    vec=vDesc[i]->getVec();
    for( int v=0; v<dim; v++ ){    
      vPoints[i].at(v) = vec[v];
    }
  }
}


void cnvFeatVecToDescriptors( const vector<FeatureVector> &vPoints, 
                              vector<CornerDescriptor *>  &vDesc )
// Convert the feature vectors to a vector of KM descriptors.       
{
  float *vec;
  int dim=vPoints[0].numDims();
  CornerDescriptor *ds;
  for( unsigned i=0; i<vPoints.size(); i++ ){
    ds=new CornerDescriptor();   
    ds->allocVec(dim);
    vec=ds->getVec();
    for( int v=0; v<dim; v++ ){    
      vec[v]=vPoints[i].at(v);       
    }
    vDesc.push_back(ds);
  }
}


void testMatchingNN( /*const*/ vector<FeatureVector> &vFeatures,
                     const vector<FeatureVector> &vClusters,
                     float dRadius,
                     vector<vector<int> >   &vvMatches )
{
  float dModRadius = 0.5*dRadius;
  unsigned nNumFeatures  = vFeatures.size();
  cout << "  Testing the Nene/Nayar Search: " << endl
       << "    #features: " << nNumFeatures << endl
       << "    #clusters: " << vClusters.size() << endl
       << "    #dims    : " << vFeatures[0].numDims() << endl
       << "    Matching radius: " << dRadius << endl
       << "    Modified radius: " << dModRadius << endl
       << endl;

  // TIMING CODE
  time_t  tu1, tu2;
  double  tc1, tc2;

  /*-------------------------------*/
  /* Build up the search structure */
  /*-------------------------------*/
  cout << "    Building up the Nene/Nayar structure...  " << flush;

  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  NNSearch nnNNCube;
  nnNNCube.init( vClusters );

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();

  cout << "\r    Building up the Nene/Nayar structure...   " << flush;
  cout << setw(12) << tc2-tc1 << "s (system), "
       << tu2-tu1 << "s (user)" << endl;
  
  /*-------------------------*/
  /* Match the features (NN) */
  /*-------------------------*/
  float dPercentMatched = 1.0;

  cout << "    Finding nearest neighbors (exact)... " << flush;
  
  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  for( unsigned i=0; i<vFeatures.size(); i++ ) {
    //float    dMinDist = 999999999999999.9;
    unsigned nMinIdx = 0;

    nMinIdx = nnNNCube.getNN( vFeatures[i], dRadius, dModRadius );
  }

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();
  
  cout << setw(12) << tc2-tc1 << "s (system), "
       << tu2-tu1 << "s (user)" << endl;

  /*---------------------------*/
  /* Match the features (AllN) */
  /*---------------------------*/
  dPercentMatched = 1.0;

  cout << "    Finding all neighbors (exact)...     " << flush;
  
  vector<int> vEmptyInt;
  vector<vector<int> > vvTmp( vFeatures.size(), vEmptyInt );
  vvMatches = vvTmp;

  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  long nNumFound = 0;
  for( unsigned i=0; i<vFeatures.size(); i++ ) {
    vvMatches[i] = nnNNCube.getNNIdzs( vFeatures[i], dRadius, 
                                               dModRadius );
    nNumFound   += vvMatches[i].size();
  }

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();
  
  cout << setw(12) << tc2-tc1 << "s (system), "
       << tu2-tu1 << "s (user)" << endl;

  cout << "      found " << nNumFound << " matches." << endl
       << endl;
}


void verifyMatches( const vector<vector<unsigned> > &vvCorrect, 
                    const vector<vector<unsigned> > &vvMatches,
                    long &nNumCorrect, long &nNumInserted, long &nNumMissed )
{
  cout << "    Verifying... " << flush;
  nNumCorrect  = 0;
  nNumInserted = 0;
  nNumMissed   = 0;
  unsigned nNumFeatures  = vvCorrect.size();
  for( unsigned i=0; i<nNumFeatures; i++ ) {
    /* get the two vectors */
    vector<unsigned> vCorrect = vvCorrect[i];
    vector<unsigned> vResult  = vvMatches[i];
    
    /* sort the two vectors */
    sort( vCorrect.begin(), vCorrect.end() );
    sort( vResult.begin(), vResult.end() );

    cout << "\r    Verifying... " << setw(6) << i+1 << "/" << nNumFeatures 
         << flush;

    /* verify the results */
    vector<unsigned>::iterator itCorrect = vCorrect.begin();
    vector<unsigned>::iterator itResult  = vResult.begin();
    while( !(itCorrect==vCorrect.end() && itResult==vResult.end()) ) {
      if( itCorrect==vCorrect.end() )
        while( itResult!=vResult.end() ) {
          nNumInserted++;
          itResult++;
        }
      else if( itResult==vResult.end() )
        while( itCorrect!=vCorrect.end() ) {
          nNumMissed++;
          itCorrect++;
        }
      else {
        if( *itCorrect < *itResult ) {
          nNumMissed++;
          itCorrect++;
        } else if( *itCorrect == *itResult ) {
          nNumCorrect++;
          itCorrect++;
          itResult++;
        } else {
          nNumInserted++;
          itResult++;
        }
      }
    }
  }
  cout << " done." << endl
       << "      #Correct : " << nNumCorrect << endl
       << "      #Inserted: " << nNumInserted << endl
       << "      #Missed  : " << nNumMissed << endl;
}


void verifyMatches( const vector<vector<unsigned> > &vvCorrect, 
                    const vector<vector<int> >      &vvMatches,
                    long &nNumCorrect, long &nNumInserted, long &nNumMissed )
{
  cout << "    Verifying... " << flush;
  nNumCorrect  = 0;
  nNumInserted = 0;
  nNumMissed   = 0;
  unsigned nNumFeatures  = vvCorrect.size();
  for( unsigned i=0; i<nNumFeatures; i++ ) {
    /* get the two vectors */
    vector<unsigned> vCorrect = vvCorrect[i];
    vector<int>      vResult  = vvMatches[i];
    
    /* sort the two vectors */
    sort( vCorrect.begin(), vCorrect.end() );
    sort( vResult.begin(), vResult.end() );

    cout << "\r    Verifying... " << setw(6) << i+1 << "/" << nNumFeatures 
         << flush;

    /* verify the results */
    vector<unsigned>::iterator itCorrect = vCorrect.begin();
    vector<int>::iterator      itResult  = vResult.begin();
    while( !(itCorrect==vCorrect.end() && itResult==vResult.end()) ) {
      if( itCorrect==vCorrect.end() )
        while( itResult!=vResult.end() ) {
          nNumInserted++;
          itResult++;
        }
      else if( itResult==vResult.end() )
        while( itCorrect!=vCorrect.end() ) {
          nNumMissed++;
          itCorrect++;
        }
      else {
        if( (int)*itCorrect < *itResult ) {
          nNumMissed++;
          itCorrect++;
        } else if( (int)*itCorrect == *itResult ) {
          nNumCorrect++;
          itCorrect++;
          itResult++;
        } else {
          nNumInserted++;
          itResult++;
        }
      }
    }
  }
  cout << " done." << endl
       << "      #Correct : " << nNumCorrect << endl
       << "      #Inserted: " << nNumInserted << endl
       << "      #Missed  : " << nNumMissed << endl;
}
