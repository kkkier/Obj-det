/*********************************************************************/
/*                                                                   */
/* FILE         optimizer.cc                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@vision.ee.ethz.ch                              */
/*                                                                   */
/* CONTENT      Codebook optimizer, to be used with the clusterer/   */
/*              scmatcher/mcmatcher program series.                  */
/*              Compresses the occurrences and occurrence maps, thus */
/*              optimizing the codebook for speed.                   */
/*                                                                   */
/* BEGIN        Tue Jul 25 2006                                      */
/* LAST CHANGE  Thu Jul 27 2006                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include <math.h>

#include <histogram.hh>
#include <resources.hh>
#include <clpostagglo.hh>

#include "optimizer.hh"

const string VERSION = 
( "\n" 
  "Codebook optimizer \n"
  "author : Bastian Leibe, ETH Zurich, 2006 \n"
  "version: 0.4 \n"
  "email  : leibe@vision.ee.ethz.ch \n"
  "URL    : http://vision.ee.ethz.ch/bleibe \n"
  "\n" );

const string USAGE = 
( "Usage: \n"
  "./optimizer [options] <in:cb> <in:occs> <out:cb> <out:occs> \n"
  "\n"
  "Options: \n"
  "-rescale         - rescale patch segm. masks \n"
  "  -ms <masksize> - rescale masks to the given size [16]\n"
  "\n"
  "-masks           - cluster and compact the occurrence masks \n"
  "  -mt <thresh>   - clustering threshold (correlation, range [0,1]) [0.9]\n"
  "  -mf <file.trc> - optional output file for clustering trace \n"
  "\n"
  "-occs            - cluster and compact the occurrences \n"
  "  -os <value>    - 3D coordinate aspect factor (s vs. x,y) [50]\n"
  "  -ot <value>    - clustering threshold for occ positions  [75]\n"
  "\n"
  "-filter          - filter out cases with too few/too many occurrences\n"
  "  -fmin <value>  - minimum #occurrences [5]\n"
  "  -fmax <value>  - maximum #occurrences [1000]\n"
  "\n"
  "-remove          - remove certain clusters with their occurrences\n"
  "  -rin <file>    - read the cluster numbers to be removed from a file\n"
  "\n"
  "-centers         - only save the cluster centers, not all features\n"
  "\n"
  "  -v             - verbose output \n"
  "  -wocc <file>   - write out the #occs to a file\n"
  "\n" );

Histogram computeOccHistogram( const VecVecOccurrence &vvOccurrences )
{
  Histogram h( 6, -1.0, 5.0 );
  
  for(unsigned i=0; i<vvOccurrences.size(); i++ ) {
    unsigned nNum = vvOccurrences[i].size();
    if( nNum == 0 )
      h.insertValue( -1.0 );
    else
      h.insertValue( log10( (float)nNum ) );
  }

  return h;
}


void computeClusterCenters( const vector<OpGrayImage> &vPatches,
                            const vector<int>         &vClusterAssignment,
                            vector<OpGrayImage>       &vClusters )
/*******************************************************************/
/* Compute the cluster centers for each cluster.                   */
/*******************************************************************/
{
  assert( vPatches.size()==vClusterAssignment.size() );

  vClusters.clear();
  if( vPatches.size()==0 )
    return;

  /*-----------------------------*/
  /* Compute the cluster centers */
  /*-----------------------------*/
  /* get the number of clusters */
  int nNumClusters = 0;
  for( int i=0; i<(int)vClusterAssignment.size(); i++ ) {
    if ( vClusterAssignment[i] > nNumClusters ) 
      nNumClusters = vClusterAssignment[i];
  }
  nNumClusters++;

  /* initialize the cluster centers */
  OpGrayImage imgTmp( vPatches.front().width(), vPatches.front().height() );
  vector<OpGrayImage> vTmp( nNumClusters, imgTmp );
  vClusters = vTmp;

  vector<int> vNumMembers( nNumClusters, 0 );
  for( int i=0; i<(int)vClusterAssignment.size(); i++ ) {
    int idx = vClusterAssignment[i];

    vClusters[idx] = vClusters[idx].add(vPatches[i]);
    vNumMembers[idx]++;
  }

  for( int i=0; i<(int)vNumMembers.size(); i++ ) {
    vClusters[i] = vClusters[i].div((float)vNumMembers[i]);
  }
}


void compactOccurrences( VecVecOccurrence &vvOccurrences, 
                         float dPosFactor, float dPosThresh, bool bVerbose )
{
  if( bVerbose )
    cout << "  Compacting the occurrences... " << endl
         << "    dPosFactor: " << dPosFactor << endl
         << "    dPosThresh: " << dPosThresh << endl
         << endl;

  // for each codebook entry
  long nCountBefore = 0;
  long nCountAfter  = 0;
  for(unsigned i=0; i<vvOccurrences.size(); i++ ) {
    VecOccurrence vOccs = vvOccurrences[i];
    
    if( vOccs.empty() )
      continue;

    /*============================*/
    /* First sort the occurrences */
    /*============================*/
    stable_sort( vOccs.begin(), vOccs.end(), compOccMapIdx() );
    //stable_sort( vOccs.begin(), vOccs.end(), compOccCateg() );

    /*=====================================*/
    /* Process each OccMapIndex separately */
    /*=====================================*/
    unsigned lo = 0;
    unsigned hi = 0;
    int nCurIdx = vOccs[0].nOccMapIdx;
    int nNewIdx = nCurIdx;
    VecOccurrence vNewOccs;
    while( hi<vOccs.size() ) {
      // select the current range
      while( hi<vOccs.size() && (nNewIdx=vOccs[hi].nOccMapIdx)==nCurIdx )
        hi++;
      
      /*--------------------------------------------------*/
      /* Collect all occ positions over the current range */
      /*--------------------------------------------------*/
      vector<FeatureVector> vOccPos;
      vector<float>         vOccWeight;
      // TODO: dAngle, dAxisRatio, dBBRatio
      for(unsigned k=lo; k<hi; k++ ) {
        FeatureVector fvPos(3);
        fvPos.at(0) = vOccs[k].dPosX;
        fvPos.at(1) = vOccs[k].dPosY;
        fvPos.at(2) = log(vOccs[k].dScale)*dPosFactor;
        
        vOccPos.push_back( fvPos );
        vOccWeight.push_back( vOccs[k].dWeight );
      }

      if( (hi-lo)==1 ) {
        vNewOccs.push_back( vOccs[lo] );
//         if( bVerbose )
//           cout << "\r      left      1 --> 1 occs." << endl;
        lo = hi;
        nCurIdx = nNewIdx;
        continue;
      }

      /*-----------------------*/
      /* Cluster the positions */
      /*-----------------------*/
      // compute similarity matrix
      int nNumOccs = vOccPos.size();
      vector<float> mSimMatrix( nNumOccs*nNumOccs );
      for( int m=0; m<nNumOccs; m++ )
        for( int n=0; n<nNumOccs; n++ )
          if( m > n ) {
            int idx = m*nNumOccs + n;
            float dDist = vOccPos[m].compSSD( vOccPos[n] );
            mSimMatrix[idx] = -dDist;
          }
      

//       if( bVerbose )
//         cout << "    Cluster " << i << ", MapIdx " << nCurIdx << ": " 
//              << vOccPos.size() << " occs..." << endl;
      //ClRNNCAgglo clAgglo( vOccPos );
      //clAgglo.setMetric( METRIC_EUCLID );
      ClPostAgglo clAgglo( vOccPos, mSimMatrix );
      clAgglo.initClusters( (int)vOccPos.size(), false );
      clAgglo.doClusterSteps( (double)dPosThresh, false );

      vector<int> vPosAssignment = clAgglo.getClusterAssignment();

      /*-----------------------------*/
      /* Compute the cluster centers */
      /*-----------------------------*/
//       if( bVerbose )
//         cout << "    Computing the cluster centers..." << endl;
      unsigned nNumClusters = 0;
      for(unsigned j=0; j<vPosAssignment.size(); j++ ) {
        if ( vPosAssignment[j] > (int)nNumClusters ) 
          nNumClusters = (unsigned)vPosAssignment[j];
      }
      nNumClusters++;
//       if( bVerbose )
//         cout << "\r      compacted " << vOccPos.size() << " --> " 
//              << nNumClusters << " occs." << endl;
      
      FeatureVector fvTmp( 3 );
      vector<FeatureVector> vNewPos    ( nNumClusters, fvTmp );
      vector<float>         vNewWeight ( nNumClusters, 0.0 );
      vector<int>           vNumMembers( nNumClusters, 0 );
      for(unsigned j=0; j<vPosAssignment.size(); j++ ) {
        int idx = vPosAssignment[j];
        
        vNewPos[idx]    += vOccPos[j];
        vNewWeight[idx] += vOccWeight[j];
        vNumMembers[idx]++;
      }

      for(unsigned j=0; j<nNumClusters; j++ ) {
        vNewPos[j] = vNewPos[j].div((float)vNumMembers[j]);
      }

      /*----------------------------*/
      /* Create compact occurrences */
      /*----------------------------*/
      for(unsigned j=0; j<nNumClusters; j++ ) {
        ClusterOccurrence occ;
        occ.dSimilarity = 1.0;
        occ.nCategory   = -1;
        occ.nPose       = -1;
        occ.nImgNumber  = -1;

        occ.dPosX       = vNewPos[j].at(0);
        occ.dPosY       = vNewPos[j].at(1);
        occ.dScale      = exp(vNewPos[j].at(2)/dPosFactor);
        occ.dWeight     = vNewWeight[j];
        occ.nOccMapIdx  = nCurIdx;
        
        occ.dAngle      = -1.0;
        occ.dAxisRatio  = -1.0;
        occ.dBBRatio    = -1.0;

        vNewOccs.push_back(occ);
      }

      /*----------------------------*/
      /* Prepare for the next range */
      /*----------------------------*/
      lo = hi;
      nCurIdx = nNewIdx;
    }

    /*===================================================*/
    /* Replace the old occurrences by the compacted ones */
    /*===================================================*/
    nCountBefore += vvOccurrences[i].size();
    if( bVerbose )
      cout << "\r    Cluster " << i << ": " << vvOccurrences[i].size() 
           << " --> " << vNewOccs.size() << " occurrences.        " << flush;
    vvOccurrences[i] = vNewOccs;
    nCountAfter += vvOccurrences[i].size();
  }
  
  if( bVerbose )
    cout << endl
         << "    Compacted occurrences from " << nCountBefore 
         << " to " << nCountAfter << "." << endl;
}


int main( int argc, char **argv )
{
  cout << VERSION;

  if( argc<2 ) {
    cout << USAGE;
    exit(-1);
  }

  string sInCB, sInOcc, sOutCB, sOutOcc;
  
  /*------------------------*/
  /* Read in the parameters */
  /*------------------------*/
  int    nMaskSize    = 16;
  float  dMaskThresh  = 0.90;

  float  dPosFactor   = 50.0;
  float  dPosThresh   = 75.0;

  int    nMinNumOccs  =    5;
  int    nMaxNumOccs  = 1000;

  bool   bRescaleMasks   = false;
  bool   bClusterMasks   = false;
  bool   bClusterOccs    = false;
  bool   bFilterOccs     = false;
  bool   bRemoveClusters = false;
  bool   bOnlySaveCenters= false;
  bool   bVerbose        = true;

  string sMaskTrace="";
  bool   bWriteMaskTrace = false;
  string sClusterList="";
  bool   bLoadClusterList = false;
  string sOccList="";
  bool   bSaveOccList = false;

  for(int i=1;i<argc;i++){ 
    if(!strcmp(argv[i],"-rescale")){
      bRescaleMasks = true;
    } else if(!strcmp(argv[i],"-ms")){
      nMaskSize = atoi( argv[++i] );

    } else if(!strcmp(argv[i],"-masks")){
      bClusterMasks = true;
    } else if(!strcmp(argv[i],"-mt")){
      dMaskThresh = atof( argv[++i] );
    } else if(!strcmp(argv[i],"-mf")){
      bWriteMaskTrace = true; sMaskTrace = argv[++i];

    } else if(!strcmp(argv[i],"-occs")){
      bClusterOccs = true;
    } else if(!strcmp(argv[i],"-os")){
      dPosFactor = atof( argv[++i] );
    } else if(!strcmp(argv[i],"-ot")){
      dPosThresh = atof( argv[++i] );

    } else if(!strcmp(argv[i],"-filter")){
      bFilterOccs = true;
    } else if(!strcmp(argv[i],"-fmin")){
      nMinNumOccs = atoi( argv[++i] );
    } else if(!strcmp(argv[i],"-fmax")){
      nMaxNumOccs = atoi( argv[++i] );

    } else if(!strcmp(argv[i],"-remove")){
      bRemoveClusters = true;
    } else if(!strcmp(argv[i],"-rin")){
      bLoadClusterList = true; sClusterList = argv[++i];

    } else if(!strcmp(argv[i],"-centers")){
      bOnlySaveCenters = true;

    } else if(!strcmp(argv[i],"-v")){
      bVerbose = true;
    } else if(!strcmp(argv[i],"-wocc")){
      bSaveOccList = true; sOccList = argv[++i];

    } else {
      if( argc!=i+4 ) {
        cerr << "ERROR: incorrect number of parameters ('" 
             << argv[i] << "')!" << endl << endl
             << USAGE;
        exit(-1);
      } else {
        sInCB   = argv[i++];
        sInOcc  = argv[i++];
        sOutCB  = argv[i++];
        sOutOcc = argv[i++];
      }
    }  
  }


  /***************************/
  /*   Read the input data   */
  /***************************/
  Codebook cbCodebook;
  ISM      ismReco(0);

  /*-------------------*/
  /* Load the Codebook */
  /*-------------------*/
  if( bVerbose )
    cout << "  Loading codebook...    " << flush;
  cbCodebook.loadCodebook( sInCB, false, false );
  int nNumClusters = cbCodebook.getNumClusters();
  if( bVerbose )
    cout << "read " << nNumClusters << " clusters." << endl;
  //m_cbCodebook.normalizeClusters( m_parCue.params()->m_nFeatureType );

  /*----------------------*/
  /* Load the Occurrences */
  /*----------------------*/
  if( bVerbose )
    cout << "  Loading occurrences... " << flush;
  ismReco.loadOccurrences( sInOcc, cbCodebook.getNumClusters(), false );
  if( bVerbose )
    cout << "read " << ismReco.getNumOccs() << " occurrences and " 
         << ismReco.getOccMaps().size() << " occ maps." << endl;

  if( bVerbose ) {
    Histogram h = computeOccHistogram( ismReco.getOccurrences() );
    cout << endl
         << "    -------------------------------" << endl
         << "    Codebook before optimization: " << endl
         << "    #Clusters    : " << cbCodebook.getNumClusters() << endl
         << "    #Occurrences : " << ismReco.getNumOccs() << endl
         << "    #Occ maps    : " << ismReco.getOccMaps().size() << endl
         << endl
         << "    Histogram #Occurrences/Cluster: " << endl
         << "         0      : " << h.at(0) << endl
         << "       1 -    10: " << h.at(1) << endl
         << "      10 -   100: " << h.at(2) << endl
         << "     100 -  1000: " << h.at(3) << endl
         << "    1000 - 10000: " << h.at(4) << endl
         << "      >10000    : " << h.at(5) << endl
         << "    -------------------------------" << endl;
  }

  /*******************************************************************/
  //   Rescale the Occurrence Masks 
  /*******************************************************************/
  if( bRescaleMasks ) {
    /*---------------------*/
    /* Rescale the OccMaps */
    /*---------------------*/
    if( bVerbose )
      cout << "  Rescaling occurrence maps to " 
           << nMaskSize << "x" << nMaskSize << "... " << endl;
    vector<OpGrayImage> vOccMaps = ismReco.getOccMaps();
    int nNumMaps  = (int)vOccMaps.size();
    
    // resize all OccMaps
    for( int i=0; i<nNumMaps; i++ )
      vOccMaps[i] = vOccMaps[i].opRescaleToWidth( nMaskSize );

    /*------------------------*/
    /* Write the OccMaps Back */
    /*------------------------*/
    ismReco.setOccMaps( vOccMaps );
  }

    
  /*******************************************************************/
  //   Cluster and Compact the Occurrence Masks 
  /*******************************************************************/
  if( bClusterMasks ) {
    // get the occ maps
    vector<OpGrayImage> vOccMaps = ismReco.getOccMaps();

    if( bVerbose )
      cout << "  Preparing occurrence maps... " << endl;
    // and convert them into featurevectors
    int nNumFeatures = (int)vOccMaps.size();
    vector<FeatureVector> vFeatures;
    for( int i=0; i <nNumFeatures; i++) {
      FeatureVector fvData = vOccMaps[i].getData();
      fvData.normalizeZeroMeanUnitStdDev2();
      vFeatures.push_back( fvData );
    }
    // free the occ maps to save memory
    vOccMaps.clear();
    
    /*--------------------------------*/
    /* Cluster them using Correlation */
    /*--------------------------------*/
    // TIMING CODE
    time_t  tu1, tu2, tu3;
    double  tc1, tc2, tc3;
    
    if( bVerbose )
      cout << "  Clustering occurrence maps... " << endl;
    vector<int>    vClusterAssignment;
    vector<ClStep> vClusterTrace;

    { // make a block to save memory
      // TIMING CODE
      time(&tu1);
      tc1 = CPUTIME();
    
      ClRNNCAgglo clAgglo( vFeatures );
      vFeatures.clear();
    
      cout << "    Initializing with " << nNumFeatures << " Clusters..." 
           << endl;
      clAgglo.initClusters( nNumFeatures, false );
      clAgglo.setMetric( METRIC_NGC );
    
      if( bVerbose )
        cout << "    Clustering..." << endl;
      clAgglo.doClusterSteps( dMaskThresh );
    
      /* get the cluster assignment */
      vClusterAssignment = clAgglo.getClusterAssignment();
      vClusterTrace      = clAgglo.getClusterTrace();
    
      // TIMING CODE
      time(&tu2);
      tc2 = CPUTIME();
    }
    
    // get the occ maps again
    vOccMaps = ismReco.getOccMaps();

    if( bVerbose )
      cout << "    Computing cluster centers... " << endl;
    vector<OpGrayImage> vClusters;
    computeClusterCenters( vOccMaps, vClusterAssignment, vClusters );
    vOccMaps.clear();

    // TIMING CODE
    time(&tu3);
    tc3 = CPUTIME();
    
    if( bVerbose )
      cout << "  done. Found " << vClusters.size() << " clusters." << endl;
    
    if( bVerbose ) {
      cout << "  ----------------------" << endl;
      cout << "  Time spent for..." << endl;
      cout << "    Clustering    : " << setw(12)
           << tc2-tc1 << "s (system), "
           << tu2-tu1 << "s (user)" << endl;
      cout << "    Postprocessing: " << setw(12)
           << tc3-tc2 << "s (system), "
           << tu3-tu2 << "s (user)" << endl;
      cout << endl;
      cout << "  ----------------------" << endl;
      cout << endl;
    }
    
    /*----------------------------------*/
    /* Optional: Save the Cluster Trace */
    /*----------------------------------*/
    if( bWriteMaskTrace ) {
      if( bVerbose )
        cout << "  Saving the cluster trace..." << endl;
      
      ofstream ofile( sMaskTrace.c_str() );
      if ( !ofile ) {
        cerr <<"Can't open output file: " << sMaskTrace << endl;
        exit(-1);
      }
      
      ofile << vClusterTrace.size() << endl;
      for(int i=0; i < (int)vClusterTrace.size(); i++) {
        ofile << vClusterTrace[i].nIdx1 << " "
              << vClusterTrace[i].nIdx2 << " "
              << vClusterTrace[i].dSim  << " "
              << vClusterTrace[i].nNewIdx << endl;
      }
      ofile.close();
    }

    /*----------------------------------*/
    /* Adapt the Mapping to Occurrences */
    /*----------------------------------*/
    VecVecOccurrence vvOccurrences = ismReco.getOccurrences();
    
    if( bVerbose )
      cout << "  Adapting mapping to occurrences... " << endl;
    for(unsigned i=0; i<vvOccurrences.size(); i++ )
      for(unsigned j=0; j<vvOccurrences[i].size(); j++ ) {
        int idx = vvOccurrences[i][j].nOccMapIdx;
        vvOccurrences[i][j].nOccMapIdx = vClusterAssignment[ idx ];
      }

    /*----------------------------------------*/
    /* Write the Occurrences and OccMaps Back */
    /*----------------------------------------*/
    ismReco.setOccurrences( vvOccurrences );
    ismReco.setOccMaps( vClusters );
  }

  
  /*******************************************************************/
  //   Cluster and Compact the Occurrences 
  /*******************************************************************/
  if( bClusterOccs ) {
    /*-------------------------*/
    /* Compact the Occurrences */
    /*-------------------------*/
    VecVecOccurrence vvOccurrences = ismReco.getOccurrences();
    compactOccurrences( vvOccurrences, dPosFactor, -dPosThresh, bVerbose );

    /*--------------------------------------*/
    /* Write the Compacted Occurrences Back */
    /*--------------------------------------*/
    ismReco.setOccurrences( vvOccurrences );
  }


  /*******************************************************************/
  //   Filter the Occurrences 
  /*******************************************************************/
  if( bFilterOccs ) {
    if( bVerbose )
      cout << "  Filtering the occurrences... " << endl
           << "    min #occs: " << nMinNumOccs << endl
           << "    max #occs: " << nMaxNumOccs << endl
           << endl;

    /*------------------------*/
    /* Filter the Occurrences */
    /*------------------------*/
    VecVecOccurrence vvOccurrences = ismReco.getOccurrences();
    long nCountMin = 0;
    long nCountMax = 0;
    for(unsigned i=0; i<vvOccurrences.size(); i++ ) {
      VecOccurrence vOccs = vvOccurrences[i];

      if( (int)vOccs.size()<nMinNumOccs ) {
        nCountMin++;
        vvOccurrences[i].clear();
      } else if( (int)vOccs.size()>nMaxNumOccs ) {
        nCountMax++;
        vvOccurrences[i].clear();
      }
    }

    /*-------------------------------------*/
    /* Write the Filtered Occurrences Back */
    /*-------------------------------------*/
    ismReco.setOccurrences( vvOccurrences );

    /*---------------------------*/
    /* Print out a status report */
    /*---------------------------*/
    if( bVerbose )
      cout << "  Filtering finished. Removed " << nCountMin+nCountMax
           << " entries total:" << endl
           << "    " << nCountMin << " below min," << endl
           << "    " << nCountMax << " above max." << endl
           << endl;
  }


  /*******************************************************************/
  //   Remove Clusters 
  /*******************************************************************/
  if( bRemoveClusters ) {
    if( bVerbose ) {
      cout << "  Removing clusters... " << endl;
    }

    /*---------------------------------*/
    /* Find the clusters to be removed */
    /*---------------------------------*/
    VecVecOccurrence vvOccurrences = ismReco.getOccurrences();

    vector<bool> vRemoveCluster( nNumClusters, false );
    if( bLoadClusterList ) {
      if( bVerbose )
        cout << "    Loading list from file '" << sClusterList << "'..."
             << endl;
      // open the input file
      ifstream ifile( sClusterList.c_str() );
      if( !ifile ) {
        cerr << "ERROR: Couldn't open file '" << sClusterList << "'!" << endl;
        exit(0);
      }

      // read the cluster numbers from file
      vector<float> vClEntries;
      while( !ifile.eof() ) {
        float x;

        ifile >> x;
        if( !ifile.eof() )
          vClEntries.push_back( x );
      }

      // close the input file again.
      ifile.close();

      // check for consistency
      if( (int)vClEntries.size() != nNumClusters ) {
        cerr << "ERROR: File contains incorrect number of cluster entries!" 
             << endl
             << "  (should contain " << nNumClusters 
             << " entries, but contains " << vClEntries.size() 
             << " entries instead)." << endl;
        exit(0);
      }

      // set the deletion flags
      for(unsigned i=0; i<vClEntries.size(); i++ )
        if( vClEntries[i]<=0.0 )
          vRemoveCluster[i] = true;
      
    } else {
      // base the decision on the #occs
      for(unsigned i=0; i<vvOccurrences.size(); i++ ) {
        VecOccurrence vOccs = vvOccurrences[i];

        if( (int)vOccs.size()==0 )
          vRemoveCluster[i] = true;
      }
    }

    /*------------------------*/
    /* Filter the Occurrences */
    /*------------------------*/
    VecVecOccurrence vvNewOccs;
    for(unsigned i=0; i<vvOccurrences.size(); i++ ) {
      if( !vRemoveCluster[i] )
        vvNewOccs.push_back( vvOccurrences[i] );
    }

    /*-----------------------------------*/
    /* Remove the corresponding Clusters */
    /*-----------------------------------*/
    vector<FeatureVector> vFeatures    = cbCodebook.getFeatures();
    vector<int>           vAssignment  = cbCodebook.getClusterAssignment();
    vector<ClStep>        vClusterTrace= cbCodebook.getClusterTrace();
    int nNumFeatures = vFeatures.size();

    /* Get the new cluster numbers */
    //cout << "    Getting the new cluster numbers... " << endl;
    vector<int> vNewClusterNum( nNumClusters );
    int nCurrent = 0;
    for( int i=0; i<nNumClusters; i++ )
      if( vRemoveCluster[i] )
        vNewClusterNum[i] = -1;
      else
        vNewClusterNum[i] = nCurrent++;

    /* Get the new assignment numbers */
    //cout << "    Getting the new assignment numbers... " << endl;
    nCurrent = 0;
    vector<int> vNewFeatureNum( nNumFeatures );
    for( int i=0; i<nNumFeatures; i++ )
      if( vRemoveCluster[vAssignment[i]] )
        vNewFeatureNum[i] = -1;
      else
        vNewFeatureNum[i] = nCurrent++;
    
    /* Delete all flagged entries */
    //cout << "    Deleting all flagged entries... " << endl;
    vector<FeatureVector> vNewFeatures;
    vector<int>           vNewAssignment;
    for( int i=0; i<(int)vAssignment.size(); i++ )
      if( !vRemoveCluster[vAssignment[i]] ) {
        vNewFeatures.push_back( vFeatures[i] );
        vNewAssignment.push_back(vNewClusterNum[vAssignment[i]]);
      }
    
    /* Adjust the cluster trace accordingly */
    //cout << "    Adjusting the cluster trace... " << endl;
    for( int i=0; i<(int)vClusterTrace.size(); i++ ) {
      vClusterTrace[i].nIdx1   = vNewFeatureNum[vClusterTrace[i].nIdx1];
      vClusterTrace[i].nIdx2   = vNewFeatureNum[vClusterTrace[i].nIdx2];
      vClusterTrace[i].nNewIdx = vNewFeatureNum[vClusterTrace[i].nNewIdx];
    }

    /*----------------------------------*/
    /* Write the Filtered Clusters Back */
    /*----------------------------------*/
    assert( vNewFeatures.size() == vNewAssignment.size() );

    cbCodebook.clear();
    cbCodebook.addFeatures         ( vNewFeatures );
    cbCodebook.setClusterAssignment( vNewAssignment );
    cbCodebook.setClusterTrace     ( vClusterTrace );
    cbCodebook.computeClusterCenters();

    /*-------------------------------------*/
    /* Write the Filtered Occurrences Back */
    /*-------------------------------------*/
    ismReco.setOccurrences( vvNewOccs );

    /*---------------------------*/
    /* Print out a status report */
    /*---------------------------*/
    if( bVerbose )
      cout << "  Finished removing clusters." << endl;
  }


  /*******************************************************************/
  //   Only Save Cluster Centers 
  /*******************************************************************/
  if( bOnlySaveCenters ) {
    if( bVerbose ) {
      cout << "  Only saving cluster centers... " << flush;
    }

    /*---------------------------------------------------*/
    /* Replace the basic features by the cluster centers */
    /*---------------------------------------------------*/
    vector<FeatureVector> vClusters = cbCodebook.getFeatures();
    int nNumClusters = (int)vClusters.size();
    
    // create a new assignment file
    vector<int> vNewAssignment( nNumClusters );
    for( int i=0; i<nNumClusters; i++ )
      vNewAssignment[i] = i;
    
    // create a mock-up cluster trace
    vector<ClStep> vNewTrace;

    // save the clusters in the codebook
    cbCodebook.clear();
    cbCodebook.addFeatures( vClusters );
    cbCodebook.setClusterAssignment( vNewAssignment );
    cbCodebook.setClusterTrace( vNewTrace );
    cbCodebook.computeClusterCenters();

    cout << "done." << endl;
  }


  /*******************************************************************/
  //   Visualize the savings in a histogram 
  /*******************************************************************/
  if( bVerbose ) {
    Histogram h = computeOccHistogram( ismReco.getOccurrences() );
    cout << endl
         << "    -------------------------------" << endl
         << "    Codebook after optimization: " << endl
         << "    #Clusters    : " << cbCodebook.getNumClusters() << endl
         << "    #Occurrences : " << ismReco.getNumOccs() << endl
         << "    #Occ maps    : " << ismReco.getOccMaps().size() << endl
         << endl
         << "    Histogram #Occurrences/Cluster: " << endl
         << "         0      : " << h.at(0) << endl
         << "       1 -    10: " << h.at(1) << endl
         << "      10 -   100: " << h.at(2) << endl
         << "     100 -  1000: " << h.at(3) << endl
         << "    1000 - 10000: " << h.at(4) << endl
         << "      >10000    : " << h.at(5) << endl
         << "    -------------------------------" << endl;
  }
 

  /*******************************************************************/
  //   Write out the results  
  /*******************************************************************/
  /*--------------------*/
  /* Save the #Occ List */
  /*--------------------*/
  if( bSaveOccList ) {
    if( bVerbose )
      cout << "  Saving #occ list... " << flush;
    
    // open the output file
    ofstream ofile( sOccList.c_str() );
    if( !ofile ) {
      cerr << "ERROR: Couldn't open file '" << sOccList << "'!" << endl;
      exit(0);
    }

    // write the #occs 
    VecVecOccurrence vvOccurrences = ismReco.getOccurrences();
    for(int i=0; i<nNumClusters; i++ )
      ofile << vvOccurrences[i].size() << "  ";
    ofile << endl;

    // close the output file again.
    ofile.close();
  
    if( bVerbose )
      cout << "written " << nNumClusters << " entries." << endl;
  }

  /*----------------------------*/
  /* Save the Modified Codebook */
  /*----------------------------*/
  if( bVerbose )
    cout << "  Saving codebook...    " << flush;
  cbCodebook.saveCodebook( sOutCB, false, false );
  if( bVerbose )
    cout << "written " << cbCodebook.getNumClusters() << " clusters." << endl;
  
  /*-------------------------*/
  /* Copy the Parameter File */
  /*-------------------------*/
  string sRawInCB( sInCB );
  int pos = sRawInCB.rfind( "." );
  sRawInCB.erase( pos );

  string sRawOutCB( sOutCB );
  pos = sRawOutCB.rfind( "." );
  sRawOutCB.erase( pos );
  
  string sCommand = "cp " + sRawInCB + ".params " + sRawOutCB + ".params";
//   if( bVerbose )
//     cout << "    Issuing command '" << sCommand << "'..." << endl;
  system( sCommand.c_str() );

  /*-------------------------------*/
  /* Save the Modified Occurrences */
  /*-------------------------------*/
  if( bVerbose )
    cout << "  Saving occurrences... " << flush;
  ismReco.saveOccurrences( sOutOcc, false );
  if( bVerbose )
    cout << "written " << ismReco.getNumOccs() << " occurrences and " 
         << ismReco.getOccMaps().size() << " occ maps." << endl;

}

