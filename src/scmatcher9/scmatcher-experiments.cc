/*********************************************************************/
/*                                                                   */
/* FILE         scmatcher-experiments.cc                             */
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
/* LAST CHANGE  Wed Sep 21 2005                                      */
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

#include "scmatcher.hh"


void ISMReco::testMatching()
{
  cout << endl
       << "===============================" << endl
       << "Testing the Matching run-times " << endl
       << "===============================" << endl;
     
  vector<FeatureVector> vFeatures = m_cbCodebook.getFeatures();
  vector<FeatureVector> vClusters = m_cbCodebook.getClusters();
  vector<ClStep>        vTrace    = m_cbCodebook.getClusterTrace();

  // TIMING CODE
  time_t  tu0, tu1, tu2, tu3;
  double  tc0, tc1, tc2, tc3;
  
  /* matching radius for Euclidean distances */
  float dFeatureSimFact  = m_parMatching.params()->m_dFeatureSimFact;
  float dRejectionThresh = m_parMatching.params()->m_dRejectionThresh;
  float dDimFact         = (float)vFeatures.front().numDims();
  float dRadius          = sqrt( -dFeatureSimFact*dDimFact * 
                                 log(dRejectionThresh) );
  float dRadius2         = dRadius*dRadius;

  unsigned nNumFeatures  = vFeatures.size();

  long nNumCorrect  = 0;
  long nNumInserted = 0;
  long nNumMissed   = 0;

  /**************************/
  /* Test the Linear Search */
  /**************************/
  cout << "  Testing the Linear Search: " << endl
       << "    #features: " << nNumFeatures << endl
       << "    #clusters: " << vClusters.size() << endl
       << "    #dims    : " << vFeatures[0].numDims() << endl
       << endl;

  //for( int k=0; k<1; k++ ) {
  int   k=0;
  float dPercentMatched = 1.0 - k*0.01;
  
  vector<unsigned> vEmpty;
  vector<int>      vEmptyInt;
  //vector<int>          vNNCorrect( vFeatures.size(), -1 );
  vector<vector<unsigned> > vvMatchesCorrect( vFeatures.size(), vEmpty );

  cout << "    Matching to " << setw(3) 
       << (int)floor(dPercentMatched*100.0+0.5) 
       << "% of the clusters... " << flush;
  
  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  unsigned nNumClusters = (unsigned)floor(vClusters.size()*dPercentMatched 
                                          + 0.5);
  long nNumFoundLinear = 0;
  for( unsigned i=0; i<vFeatures.size(); i++ ) {
    //float dMinDist = vFeatures[i].compSSD( vClusters[0] );
    //int   nMinIdx  = 0;
    vvMatchesCorrect[i].clear();
    for( unsigned j=1; j<nNumClusters; j++ ) {
      float dDist = vFeatures[i].compSSD( vClusters[j] );
      //if( dDist < dMinDist ) {
      //  dMinDist = dDist;
      //  nMinIdx = j;
      //}
      if( dDist <= dRadius2 )
        vvMatchesCorrect[i].push_back( j );
    }
    nNumFoundLinear += vvMatchesCorrect[i].size();
  }
  
  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();

    cout << setw(12) << tc2-tc1 << "s (system), "
         << tu2-tu1 << "s (user)" << endl;

    cout << "      found " << nNumFoundLinear << " matches." << endl;
    //}

  cout << "-------------------------------" << endl;

  /*************************************/
  /* Test the Balltree(Anchors) Search */
  /*************************************/
  cout << "  Testing the Balltree Search (Anchors): " << endl
       << "    #features: " << nNumFeatures << endl
       << "    #clusters: " << vClusters.size() << endl
       << "    #dims    : " << vFeatures[0].numDims() << endl
       << "    Matching radius: " << dRadius << endl
       << endl;

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
  
  /*-------------------------*/
  /* Match the features (NN) */
  /*-------------------------*/
  dPercentMatched = 1.0;

  cout << "    Finding nearest neighbors (exact)... " << flush;
  
  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  for( unsigned i=0; i<vFeatures.size(); i++ ) {
    float    dMinDist = 999999999999999.9;
    unsigned nMinIdx = 0;

    btNNTree.findNN( vFeatures[i], dRadius2, nMinIdx, dMinDist );
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
  
  vector<vector<unsigned> > vvMatchesBallTree( vFeatures.size(), vEmpty );

  // TIMING CODE
  time(&tu1);
  tc1 = CPUTIME();
  
  long nNumFoundBallTree = 0;
  for( unsigned i=0; i<vFeatures.size(); i++ ) {
    vvMatchesBallTree[i] = btNNTree.findNeighbors( vFeatures[i], dRadius2 );
    nNumFoundBallTree += vvMatchesBallTree[i].size();
  }

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();
  
  cout << setw(12) << tc2-tc1 << "s (system), "
       << tu2-tu1 << "s (user)" << endl;

  cout << "      found " << nNumFoundBallTree << " matches." << endl
       << endl;

  cout << "    Verifying... " << flush;
  nNumCorrect  = 0;
  nNumInserted = 0;
  nNumMissed   = 0;
  for( unsigned i=0; i<nNumFeatures; i++ ) {
    /* get the two vectors */
    vector<unsigned> vCorrect = vvMatchesCorrect[i];
    vector<unsigned> vResult  = vvMatchesBallTree[i];
    
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
  
//   vector<KM::CornerDescriptor*> vEmptyCD;
//   vector<vector<KM::CornerDescriptor*> > vvMatchesKBallTree( vFeatures.size(),
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


void ISMReco::computeOccurrencesIDL()
{
  m_qsLastIDL = QFileDialog::getOpenFileName( m_qsLastIDL,
                           "IDL files (*.idl);;All files (*.*)",
                                                this);
  if ( m_qsLastIDL.isEmpty() )
    return;
  
  computeOccurrencesIDL( m_qsLastIDL );
}


void ISMReco::computeOccurrencesIDL( const QString& qsIdlFile )
  /*******************************************************************/
  /* Load a set of images and compute occurrences from them. That    */
  /* means, load each image separately, extract patches from it,     */
  /* match them to the codebook, and record for each codebook entry  */
  /* where (relative to the object center) it matched. In order to   */
  /* compute the object center, this version of the method assumes   */
  /* that a segmentation mask is available for each image (the ob-   */
  /* ject center is then taken as the center of gravity of the seg-  */
  /* mentation mask). If no segmentation mask is available, the      */
  /* method needs to be adapted to take the image center as refe-    */
  /* rence point.                                                    */
  /* The difference of this function to the regular version is that  */
  /* it reads in an IDL file containing the training image names,    */
  /* and also possibly annotation bboxes. If no training segmenta-   */
  /* tion is given, it uses those bboxes to create a blurred ellip-  */
  /* tical prior as a substitute for the segmentation mask.          */
  /*******************************************************************/
{
  /*-----------------------------------------*/
  /* Extract the path from the idl file name */
  /*-----------------------------------------*/
  string sFileName( qsIdlFile.latin1() );
  string sPath;
  unsigned pos = sFileName.rfind( "/" );
  if( pos != string::npos )
    sPath = sFileName.substr( 0, pos + 1 );

  /*-------------------*/
  /* Load the idl file */
  /*-------------------*/
  ImgDescrList idlList;
  idlList.load( qsIdlFile.latin1());
  
  /*--------------------*/
  /* Process all images */
  /*--------------------*/
  m_ismReco.initOccurrences( m_cbCodebook.getNumClusters() );

  int nOccMapIdx = 0;
  VecVecOccurrence       vvOccurrences;
  vector<OpGrayImage>    vOccMaps;
  
  /* process the training images */
  cout << "  Processing the training images..." << endl;
  for( int i=0; i<(int)idlList.size(); i++ ) {
    string sName = sPath + idlList[i].sName;
    cout << "    Processing image " << i << ": " << sName << endl;

    for(int j=0; j<(int)idlList[i].vRectList.size(); j++) {
      /* initialize the global variables */
      m_vImagePatches.clear();
      m_vFeatures.clear();
      m_vPointsInside.clear();
      
      /* initialize the occurrences */
      vvOccurrences.clear();
      VecVecOccurrence tmp( m_cbCodebook.getNumClusters() );
      vvOccurrences = tmp;
      vOccMaps.clear();

      /* get the bounding box */
      Rect rBBox = idlList[i].vRectList[j];
      if( (rBBox.x1-rBBox.x2)==0 || (rBBox.y1-rBBox.y2)==0 ) {
        cerr << "      WARNING in ISMReco::computeOccurrencesIDL(): "
             << "Invalid bounding box!" << endl;
        continue;
      }

      /* load the next image, calculate the interest points and ex-    */
      /* tract all patches.                                            */
      processImageBBox( sName.c_str(), rBBox );
      
      /* extract the pose information from the file name */
      int nPose = 0;

      /* extract the bbox aspect ratio */
      float dBBRatio = fabs(rBBox.x1-rBBox.x2)/fabs(rBBox.y1-rBBox.y2);

      /* match the extracted patches to the codebook */
      cout << "    Comparing image patches with all matches." << endl;
      compareFeatures( false );
      
      /* for every matched cluster, save an "occurrence" vector */
      int center_x, center_y;
      if( m_bMapsOn )
        m_grayImgMap.opComputeCoG( center_x, center_y );
      else {
        center_x = m_qresultImg.width()/2;
        center_y = m_qresultImg.height()/2;
      }
      cout << "    Object center is at (" << center_x << "," << center_y << ")"
           << endl;
      
      cout << "    Counting occurrences..." << endl;
      long nCountOccs = 0;
      float dRejectionThresh = m_parMatching.params()->m_dRejectionThresh;
      /* for all extracted patches */
      for( int j=0; j<(int)m_vvAllNeighbors.size(); j++ ) {
        if( m_vvAllNeighbors[j].size() > 0 )
          if( m_bMapsOn ) {
            /* extract the segmentation map for this patch */
            int nPatchSize = (int) floor(m_vPointsInside[j].scale*
                                         m_fcCue.params()->m_dScaleFactor+0.5);
            int nPatchArea = (2*nPatchSize+1)*(2*nPatchSize+1);
            int nNormSize  = m_fcCue.params()->m_nPatchSize;
            int nNormArea  = (2*nNormSize+1)*(2*nNormSize+1);
            float dAreaFactor = (float)nPatchArea/((float) nNormArea);
            OpGrayImage imgMap; 
            
            if( m_vPointsInside[j].l1 == m_vPointsInside[j].l2 ) {
              /*-=-=-=-=-=-=-=-=-*/
              /* Circular region */
              /*-=-=-=-=-=-=-=-=-*/
              imgMap = 
                m_grayImgMap.extractRegion( m_vPointsInside[j].x - nPatchSize,
                                            m_vPointsInside[j].y - nPatchSize,
                                            m_vPointsInside[j].x + nPatchSize,
                                            m_vPointsInside[j].y + nPatchSize);
              
              /* for compatibility reasons, rescale the map to a */
              /* standard size.                                  */
              if( nPatchSize != m_fcCue.params()->m_nPatchSize )
                imgMap = imgMap.opRescaleToWidth( 2*nNormSize+1 );
              
            } else {
              /*-=-=-=-=-=-=-=-=-=-*/
              /* Elliptical region */
              /*-=-=-=-=-=-=-=-=-=-*/
              m_fcCue.extractAffineRegion( m_grayImgMap, m_vPointsInside[j], 
                                           2*m_fcCue.params()->m_nPatchSize+1, 
                                           imgMap );
            }
            
            /* if the patch lies on the object */
            if( imgMap.getSum() >= m_fcCue.params()->m_nMinFigurePixels*255.0){
              /* get the index of the next occurrence map */
              int nNextOccMap    = nOccMapIdx + vOccMaps.size();
              bool bMatchedEntry = false;
              
              /* for all matched codebook entries */
              for( int k=0; k<(int)m_vvAllNeighbors[j].size(); k++ ) 
                if( m_vvAllNeighborsSim[j][k] > dRejectionThresh ) {
                  /* create a new occurrence */
                  ClusterOccurrence occ;
                  occ.dSimilarity = m_vvAllNeighborsSim[j][k];
                  occ.nCategory = m_nCateg;
                  if( nPose != -1 )
                    occ.nPose = nPose;
                  else if( m_bSavePose )
                    occ.nPose = m_nPose;
                  else
                    occ.nPose = -1;
                  
                  occ.dScale     = m_vPointsInside[j].scale;
                  occ.dPosX      = m_vPointsInside[j].x - center_x;
                  occ.dPosY      = m_vPointsInside[j].y - center_y;
                  occ.nOccMapIdx = nNextOccMap;
                  occ.nImgNumber = i;
                  occ.dWeight    = 1.0;
                  occ.dAngle     = m_vPointsInside[j].angle;
                  if( fabs(m_vPointsInside[j].l2) >= 0.01 )
                    occ.dAxisRatio = ( m_vPointsInside[j].l1 /
                                       m_vPointsInside[j].l2 );
                  else
                    occ.dAxisRatio = m_vPointsInside[j].l1/0.01;
                  occ.dBBRatio   = dBBRatio;
                  
                  /* store the occurrence */
                  vvOccurrences[m_vvAllNeighbors[j][k]].push_back( occ );
                  nCountOccs++;
                  bMatchedEntry = true;
                }
              
              if( bMatchedEntry )
                /* store a segmentation map for the occurrences */
                vOccMaps.push_back( imgMap );
            } // end if (patch lies on object)
            
          } else { /* no map available => always store the occurrence */
            /* for all matched codebook entries */
            for( int k=0; k<(int)m_vvAllNeighbors[j].size(); k++ ) 
              if( m_vvAllNeighborsSim[j][k] > dRejectionThresh ) {
                /* create a new occurrence */
                ClusterOccurrence occ;
                occ.dSimilarity = m_vvAllNeighborsSim[j][k];
                occ.nCategory = m_nCateg;
                if( nPose != -1 )
                  occ.nPose = nPose;
                else if( m_bSavePose )
                  occ.nPose = m_nPose;
                else
                  occ.nPose = -1;
                
                occ.dScale     = m_vPointsInside[j].scale;
                occ.dPosX      = m_vPointsInside[j].x - center_x;
                occ.dPosY      = m_vPointsInside[j].y - center_y;
                occ.nOccMapIdx = -1;
                occ.nImgNumber = i;
                occ.dWeight    = 1.0;
                occ.dAngle     = m_vPointsInside[j].angle;
                if( fabs(m_vPointsInside[j].l2) >= 0.01 )
                  occ.dAxisRatio = m_vPointsInside[j].l1/m_vPointsInside[j].l2;
                else
                  occ.dAxisRatio = m_vPointsInside[j].l1/0.01;
                occ.dBBRatio   = dBBRatio;
                
                /* store only the occurrence, no segm. map */
                vvOccurrences[m_vvAllNeighbors[j][k]].push_back( occ );
                nCountOccs++;
              }
          } // end else (no maps available)
      } // end forall extracted patches
      cout << "    found " << nCountOccs << " occurrences for this image." 
           << endl;
      
      /* store the occurrences */
      m_ismReco.addOccurrences( vvOccurrences, vOccMaps, nOccMapIdx );
    }
  }
  cout << "  done." << endl;

  /* compute the occurrence weights */
  cout << "  Computing occurrence weights..." << endl;
  m_ismReco.finishOccurrences();

  cout << "done." << endl;
}



