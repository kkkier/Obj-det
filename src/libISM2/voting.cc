/*********************************************************************/
/*                                                                   */
/* FILE         voting.cc                                            */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Implements functions for creating, loading, and      */
/*              saving subparts and parts.                           */
/*                                                                   */
/*                                                                   */
/* BEGIN        Wed Mar 19 2003                                      */
/* LAST CHANGE  Wed Mar 19 2003                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <algorithm>
#include <qimage.h>

#include <qtimgbrowser.hh>
#include <visualhistogram.hh>

#include "voting.hh"


/********************************/
/*   Function Implementations   */
/********************************/

vector< vector<float> > getOccurrenceWeights( VecVecOccurrence vvOccurrences,
                                              bool bNormPerPose )
  /*******************************************************************/
  /* Compute the occurrence weights in order to normalize per pose.  */
  /* (This function is called in preparation of the voting phase).   */
  /*******************************************************************/
{
  vector<float> tmpVec(NUM_POSES);
  vector< vector<float> > vOccWeights(vvOccurrences.size(), tmpVec);
  for( int k=0; k<vvOccurrences.size(); k++ ) {
    /* initialize the counters */
    vector<int> vCount(NUM_POSES);
    for( int j=0; j<NUM_POSES; j++ )
      vCount[j] = 0;

    /* count the number of occurrences per pose */
    for( int j=0; j<vvOccurrences[k].size(); j++ ) {
      int nPose = vvOccurrences[k][j].nPose - 25;
      if( (nPose>=0) && (nPose<NUM_POSES) )
        vCount[nPose]++;
    }
    
    for( int j=0; j<NUM_POSES; j++ )
      if( bNormPerPose ) {
        /* normalize the weights by the occurrence counts per pose */
        if( vCount[j] > 0 )
          vOccWeights[k][j] = 1.0 / (float) vCount[j];
        else
          vOccWeights[k][j] = 1.0;

      } else {
        /* normalize the weights by the total occurrence counts. */
        vOccWeights[k][j] = 1.0/(float)vOccWeights[k].size();
      }
  }

  return vOccWeights;
}


void doPatchVoting( const VecVecOccurrence        &vvOccurrences, 
                    const vector<int>             &vNegOccs,
                    const PointVector             &vPointsInside,
                    const vector<int>             &vNearestNeighbor, 
                    const vector<float>           &vNearestNeighborSim,
                    const vector< vector<int> >   &vvAllNeighbors, 
                    const vector< vector<float> > &vvAllNeighborsSim,
                    float dRejectionThresh,
                    int nImgWidth, int nImgHeight, int nStepSize,
                    bool bNormPerPatch, bool bNormPerPose,
                    VotingSpace &vsHoughVotesGlobal,
                    VotingSpace &vsHoughVotesPose )
  /*******************************************************************/
  /* Collect all the patch votes for voting based on occurrences of  */
  /* codebook entries.                                               */
  /*******************************************************************/
{
  /*****************************************/
  /*   Initialize the occurrence weights   */
  /*****************************************/
  cout << "  Initializing the occurrence weights..." << endl;
  vector< vector<float> > vOccWeights = getOccurrenceWeights(vvOccurrences, 
                                                             bNormPerPose);
  cout << "  done." << endl;


  /************************************/
  /*   Initialize result structures   */
  /************************************/
  /* initialize a global result histogram */
  VisualHistogram hHoughValuesGlob( nImgWidth/nStepSize, 
                                    0.0, (float) nImgWidth,
                                    nImgHeight/nStepSize,
                                    0.0, (float) nImgHeight );

  /* initialize pose-specific result histograms */
  vector<VisualHistogram> hHoughValuesLoc( NUM_POSES, hHoughValuesGlob );

  /* initialize the Hough transform structures */
  vsHoughVotesGlobal.clear();
  VotingSpace vsTemp( nImgWidth/nStepSize,  0.0, (float) nImgWidth,
                      nImgHeight/nStepSize, 0.0, (float) nImgHeight );
  vsHoughVotesGlobal = vsTemp;

  vsHoughVotesPose.clear();
  VotingSpace vsTemp2( nImgWidth/nStepSize,  0.0, (float) nImgWidth,
                       nImgHeight/nStepSize, 0.0, (float) nImgHeight,
                       NUM_POSES, 0, NUM_POSES-1 );
  vsHoughVotesPose = vsTemp2;


  /*******************************************/
  /*   Create hypotheses for object center   */
  /*******************************************/
  /* always use all matches */
  for( int j=0; j<(int)vvAllNeighbors.size(); j++ ) 
    for( int k=0; k<(int)vvAllNeighbors[j].size(); k++ ) 
      if( vvAllNeighborsSim[j][k] > dRejectionThresh ) {
        int clusterId = vvAllNeighbors[j][k];
        float dVoteWeight = 1.0;
        if( bNormPerPatch )
          dVoteWeight = 1.0 / (float)vvAllNeighbors[j].size();
        
        /* process all valid hypotheses for this cluster */
        int nNumHypos = (int)vvOccurrences[clusterId].size();
        float dWeight = 1.0 / (float)( nNumHypos + vNegOccs[clusterId] );
        for( int kk=0; kk<nNumHypos; kk++ ) {
          if( vvOccurrences[clusterId][kk].dScale == 0.0 ) {
            cerr << "  Warning: occurrence " << k << " of cluster " 
                 << clusterId << " has scale zero!" << endl;
            cerr << "  Patch voting aborted." << endl;
            return;
          }
          float scale = ( vPointsInside[j].scale / 
                          vvOccurrences[clusterId][kk].dScale );
          float dOccWeight = vvOccurrences[clusterId][kk].dWeight;
          int posX = (int) floor( vPointsInside[j].x - 
                                  vvOccurrences[clusterId][kk].dPosX*scale );
          int posY = (int) floor( vPointsInside[j].y - 
                                  vvOccurrences[clusterId][kk].dPosY*scale );
          hHoughValuesGlob.incrementValue( (float)posX, (float)posY, 
                                           dWeight*dVoteWeight);
          vsHoughVotesGlobal.insertVote( HoughVote( (float)posX,
                                                    (float)posY,
                                                    dWeight*dVoteWeight *
                                                    dOccWeight,
                                                    (float)1.0,
                                                    j, clusterId, kk ) );
          
          
          int nPose = vvOccurrences[clusterId][k].nPose - 25;
          if( (nPose>=0) && (nPose<NUM_POSES) ) {
            float dPoseWeight = vOccWeights[clusterId][nPose]*dVoteWeight;
            hHoughValuesLoc[nPose].incrementValue( (float)posX, (float)posY, 
                                                   dPoseWeight*dOccWeight );
            
            vsHoughVotesPose.insertVote( HoughVote( (float)posX,
                                                    (float)posY,
                                                    (float)nPose,
                                                    dPoseWeight*dOccWeight,
                                                    (float)1.0,
                                                    j, clusterId, kk ) );
          }
        }
      }
  cout << "  done." << endl;

  cout << "  Debugging info for VotingSpace vsHoughVotesGlobal:" << endl;
  vsHoughVotesGlobal.print();
  vsHoughVotesGlobal.printContent();
}


void doPatchVotingScale( const VecVecOccurrence        &vvOccurrences,
                         const vector<float>           &vOccSumWeights,
                         const vector<int>             &vNegOccs,
                         const PointVector             &vPointsInside,
                         const vector<int>             &vNearestNeighbor, 
                         const vector<float>           &vNearestNeighborSim,
                         const vector< vector<int> >   &vvAllNeighbors, 
                         const vector< vector<float> > &vvAllNeighborsSim,
                         float dRejectionThresh,
                         int nImgWidth, int nImgHeight, int nStepSize,
                         float dScaleMin, float dScaleMax, int nScaleSteps,
                         bool bNormPerPatch, bool bNormPerPose, 
                         bool bRestrictScale,
                         float dMinVoteWeight,
                         VotingSpace &vsHoughVotesGlobal,
                         VotingSpace &vsHoughVotesScale )
  /*******************************************************************/
  /* Collect all the patch votes for voting based on occurrences of  */
  /* codebook entries (extended version for scale voting).           */
  /*******************************************************************/
{
  /************************************/
  /*   Initialize result structures   */
  /************************************/
  if( nScaleSteps == 0 )
    nScaleSteps = 1;

  /* initialize the Hough transform structures */
  vsHoughVotesGlobal.clear();
  //VotingSpace vsTemp( nImgWidth/nStepSize,  0.0, (float) nImgWidth,
  //                    nImgHeight/nStepSize, 0.0, (float) nImgHeight );
  //vsHoughVotesGlobal = vsTemp;

  vsHoughVotesScale.clear();
  VotingSpace vsTemp2( nImgWidth/nStepSize,  0.0, (float) nImgWidth,
                       nImgHeight/nStepSize, 0.0, (float) nImgHeight,
                       nScaleSteps, dScaleMin, dScaleMax );
  vsHoughVotesScale = vsTemp2;

  //cout << "  Debugging info for VotingSpace vsHoughVotesGlobal (before):" 
  //     << endl;
  //vsHoughVotesGlobal.print();
  cout << "  Debugging info for VotingSpace vsHoughVotesScale (before):" 
       << endl;
  vsHoughVotesScale.print();

  /*******************************************/
  /*   Create hypotheses for object center   */
  /*******************************************/
  float dScaleRange = dScaleMax - dScaleMin;
  if( dScaleRange == 0.0 )
    dScaleRange = 1.0;
  float dScaleCellSize = dScaleRange / ((float)nScaleSteps);

  /* always use all matches */
  cout << "  Filling in the voting space..." << endl;
  long nCountMatched = 0;
  long nCountVotes = 0;
  long nCountWeights = 0;
  long nCountAll = 0;
  for( int j=0; j<(int)vvAllNeighbors.size(); j++ ) {
    float dMatchWeight = 1.0;
    if( bNormPerPatch )
      dMatchWeight = 1.0 / (float)vvAllNeighbors[j].size();

    for( int k=0; k<(int)vvAllNeighbors[j].size(); k++ ) 
      if( vvAllNeighborsSim[j][k] > dRejectionThresh ) {
        int clusterId = vvAllNeighbors[j][k];
        
        nCountMatched++;
        /* process all valid hypotheses for this cluster */
        int nNumHypos = (int)( vvOccurrences[clusterId].size() );
        if( nNumHypos == 0 ) {
          cerr << "  Warning: No occurrences for cluster " << clusterId 
               << "!" << endl;
          cerr << "    Patch voting aborted for this cluster." << endl;
          //return;
          continue;
        }
        
        /* compute the sum weight of the occurrences */
        float dSumWeight = vOccSumWeights[clusterId];
        
        //float dWeight = 1.0 / (float)( nNumHypos  + vNegOccs[clusterId] );
        float dOccWeightNorm = 1.0 / (float)( dSumWeight + vNegOccs[clusterId] );
        for( int kk=0; kk<nNumHypos; kk++ ) {
          if( vvOccurrences[clusterId][kk].dScale == 0.0 ) {
            cerr << "  Warning: occurrence " << kk << " of cluster " 
                 << clusterId << " has scale zero!" << endl;
            cerr << "  Patch voting aborted." << endl;
            return;
          }
          
          float dScale = ( vPointsInside[j].scale / 
                           vvOccurrences[clusterId][kk].dScale );
          float dOccWeight = vvOccurrences[clusterId][kk].dWeight;


//           float dScaleFactor = 3.0;
//           int   nNormSize    = 12;
//           int   nPatchSize   = (int) floor( vPointsInside[j].scale*
//                                         dScaleFactor+0.5);
//           int   nPatchArea   = (2*nPatchSize+1)*(2*nPatchSize+1);
//           int   nNormArea    = (2*nNormSize+1)*(2*nNormSize+1);
//           float dAreaFactor  = (float)nPatchArea/((float) nNormArea);

//           float dRescaleWeight = ( (dAreaFactor<=1.0)? dAreaFactor :
//                                    1.0/dAreaFactor );

          nCountAll++;
          if( bRestrictScale )
            if( (dScale < dScaleMin - dScaleCellSize) || 
                (dScale > dScaleMax + dScaleCellSize) )
              /* if scale is out of range, don't store the vote */
              continue;
          
          float dPosX   = floor( vPointsInside[j].x - 
                                 vvOccurrences[clusterId][kk].dPosX * dScale );
          float dPosY   = floor( vPointsInside[j].y - 
                                 vvOccurrences[clusterId][kk].dPosY * dScale );
          float dWeight = ( dMatchWeight * dOccWeight * dOccWeightNorm );

          nCountWeights++;
          if( dWeight < dMinVoteWeight )
            /* if vote weight is too small, don't store the vote */
            continue;

          //vsHoughVotesGlobal.insertVote( HoughVote( dPosX, dPosY,
          //                                          dWeight, 1.0,
          //                                          j, clusterId, kk ) );
          
          vsHoughVotesScale.insertVote( HoughVote( dPosX, dPosY, dScale,
                                                   dWeight, 1.0,
                                                   j, clusterId, kk ) );

          nCountVotes++;
        }
      }
  }
  
  cout << "    Generated " << nCountVotes << " votes." << endl;
  cout << "      from " << nCountMatched << " matched codebook entries." 
       << endl;
  if( bRestrictScale )
    cout << "      (discarded " << nCountAll - nCountWeights 
         << " votes as outside of scale range)." << endl; 
  cout << "      (discarded " << nCountWeights - nCountVotes
       << " votes because of insufficient weight)." << endl; 
  cout << "  done." << endl;
}


vector<Hypothesis> getPatchHypotheses( VotingSpace &vsHoughVotesGlobal,
                                       PointVector       vPointsInside,
                                       int nImgWidth, int nImgHeight, 
                                       int nStepSize, 
                                       int nCateg,
                                       float dMSMESizeX, float dMSMESizeY,
                                       int   nPatchSize,
                                       OpGrayImage        &imgPatchVotesGlob,
                                       OpGrayImage        &imgPatchMaxima )
  /*******************************************************************/
  /* Extract all object hypotheses from the voting space as local    */
  /* maxima in the Hough accumulator array.                          */
  /*******************************************************************/
{
  /*************************************/
  /*   Calculate patch voting scores   */
  /*************************************/
  OpGrayImage tmpImg( nImgWidth/nStepSize, nImgHeight/nStepSize );
  imgPatchVotesGlob = tmpImg;

  float dCellSizeX = ( (float)nImgWidth  / (float)(nImgWidth/nStepSize) );
  float dCellSizeY = ( (float)nImgHeight / (float)(nImgHeight/nStepSize) );
  for( int y=0; y<nImgHeight/nStepSize; y++ )
    for( int x=0; x<nImgWidth/nStepSize; x++ ) {
      FeatureVector fvWindowPos( 2 );
      fvWindowPos.setValue( 0, (x+0.5)*dCellSizeX );
      fvWindowPos.setValue( 1, (y+0.5)*dCellSizeY );
      
      vsHoughVotesGlobal.setWindowSize( dMSMESizeX, dMSMESizeY );
      //imgPatchVotesGlob(x,y) = vsHoughVotesGlobal.getVoteSum( fvWindowPos );
      imgPatchVotesGlob(x,y) = vsHoughVotesGlobal.getBinVoteSum( x, y );
    }
  
  /******************************************/
  /*   Find maxima of patch voting scores   */
  /******************************************/
  imgPatchMaxima = imgPatchVotesGlob.opNonMaximumSuppression();

  vector<Hypothesis> vPatchHypos = 
    extractHypotheses2D( imgPatchVotesGlob, 
                         vsHoughVotesGlobal, vPointsInside, 
                         nImgWidth, nImgHeight, nStepSize, 
                         dMSMESizeX, dMSMESizeY, nPatchSize, 
                         nCateg, 0 );
  cout << "=======================================" << endl;
  cout << "Single-Patch Voting Hypotheses:" << endl;
  for( int i=0; i<vPatchHypos.size(); i++ ) {
    cout << "  " << setw(2) << i+1 << ". ";
    printHypothesis( vPatchHypos[i] );
  }
  cout << "=======================================" << endl;
  cout << endl;

  return vPatchHypos;
}
  

vector<Hypothesis> getPatchHypothesesScale( VotingSpace &vsHoughVotesScale,
                                            PointVector       vPointsInside,
                                            int nImgWidth, int nImgHeight, 
                                            int nStepSize, 
                                            float dScaleMin, float dScaleMax,
                                            int nScaleSteps,
                                            int nCateg,
                                            float dMSMESizeX, float dMSMESizeY,
                                            float dMSMESizeS, 
                                            int   nPatchSize,
                                            vector<OpGrayImage> &vImgPatchVotesScale,
                                            vector<OpGrayImage> &vImgPatchMaxima )
  /*******************************************************************/
  /* Extract all object hypotheses from the voting space as local    */
  /* maxima in the Hough accumulator array (extended version with    */
  /* scale search).                                                  */
  /*******************************************************************/
{
  /*******************************************/
  /*   Compute scale normalization factors   */
  /*******************************************/
  vector<float> vScaleFactor( nScaleSteps, 0.0 );
  vsHoughVotesScale.setWindowSize ( dMSMESizeX, dMSMESizeY, dMSMESizeS );
  for( int s=0; s<vsHoughVotesScale.numBins(2); s++ )
    for( int y=0; y<vsHoughVotesScale.numBins(1); y++ )
      for( int x=0; x<vsHoughVotesScale.numBins(0); x++ ) {
        vector<int> vBinIdx(3);
        vBinIdx[0] = x;
        vBinIdx[1] = y;
        vBinIdx[2] = s;
        
        vector<HoughVote> vVotes = vsHoughVotesScale.getBinVotes(vBinIdx);
        for( int i=0; i<vVotes.size(); i++ )
          vScaleFactor[s] += vVotes[i].getValue();
      }
  for( int s=0; s<nScaleSteps; s++ )
    if( vScaleFactor[s] == 0.0 )
      vScaleFactor[s] = 1.0;

  /*************************************/
  /*   Calculate patch voting scores   */
  /*************************************/
  float dScaleRange = dScaleMax - dScaleMin;
  //int   nScaleSteps = (int) floor(dScaleRange / dMSMESizeS);

  OpGrayImage tmpImg( nImgWidth/nStepSize, nImgHeight/nStepSize );
  vector<OpGrayImage> tmpVec( nScaleSteps, tmpImg );
  vImgPatchVotesScale = tmpVec;
  vImgPatchMaxima     = tmpVec;

  FeatureVector fvWindowSize( 3 );
  fvWindowSize.setValue( 0, dMSMESizeX );
  fvWindowSize.setValue( 1, dMSMESizeY );
  fvWindowSize.setValue( 2, dMSMESizeS );

  float dCellSizeX = ( (float)nImgWidth  / (float)(nImgWidth/nStepSize) );
  float dCellSizeY = ( (float)nImgHeight / (float)(nImgHeight/nStepSize) );
  float dCellSizeS = ( dScaleRange / (float)nScaleSteps );
  vsHoughVotesScale.setWindowSize ( dMSMESizeX, dMSMESizeY, dMSMESizeS );
  for( int y=0; y<nImgHeight/nStepSize; y++ )
    for( int x=0; x<nImgWidth/nStepSize; x++ )
      for( int s=0; s<nScaleSteps; s++ ) {
      FeatureVector fvWindowPos( 3 );
      fvWindowPos.setValue( 0, (x+0.5)*dCellSizeX );
      fvWindowPos.setValue( 1, (y+0.5)*dCellSizeY );
      fvWindowPos.setValue( 2, dScaleMin+(s+0.5)*dCellSizeS );
      
      vImgPatchVotesScale[s](x,y) = vsHoughVotesScale.getVoteSum( fvWindowPos);
      //vImgPatchVotesScale[s](x,y) = vsHoughVotesScale.getBinVoteSum(x,y,s);
    }
  
  /******************************************/
  /*   Find maxima of patch voting scores   */
  /******************************************/
  for( int s=0; s<nScaleSteps; s++ )
    // !!! should be opNonMaximumSuppresion3D !!!
    vImgPatchMaxima[s] = vImgPatchVotesScale[s].opNonMaximumSuppression();

  vector<Hypothesis> vPatchHypos = 
    extractHypotheses3D( vImgPatchVotesScale, 
                         vsHoughVotesScale, vPointsInside, 
                         nImgWidth, nImgHeight, nStepSize, 
                         dMSMESizeX, dMSMESizeY, dMSMESizeS, nPatchSize, 
                         nCateg );
  cout << "=======================================" << endl;
  cout << "Single-Patch Voting Hypotheses:" << endl;
  for( int i=0; i<vPatchHypos.size(); i++ ) {
    cout << "  " << setw(2) << i+1 << ". ";
    printHypothesis( vPatchHypos[i] );
  }
  cout << "=======================================" << endl;
  cout << endl;

  return vPatchHypos;
}
  

vector<Hypothesis> getPatchHypothesesMSME(VotingSpace &vsHoughVotesGlobal,
                                          PointVector       vPointsInside,
                                          int nImgWidth, int nImgHeight, 
                                          int nStepSize, 
                                          int nCateg,
                                          float dMSMESizeX, float dMSMESizeY,
                                          int   nPatchSize,
                                          bool  bExtendSearch, 
                                          int   nAddedRangeX,
                                          int   nAddedRangeY,
                                          OpGrayImage  &imgPatchVotesGlob,
                                          OpGrayImage  &imgPatchMaxima )
  /*******************************************************************/
  /* Extract all object hypotheses from the voting space as local    */
  /* maxima in the Hough accumulator array, the refine them using    */
  /* MSME.                                                           */
  /*******************************************************************/
{
  /*************************************/
  /*   Calculate patch voting scores   */
  /*************************************/
  /* search an extended range in order to find half-visible objects */
  if( !bExtendSearch ) {
    nAddedRangeX = 0;
    nAddedRangeY = 0;
  }

  float dCellSizeX = ( (float)nImgWidth  / (float)(nImgWidth/nStepSize) );
  float dCellSizeY = ( (float)nImgHeight / (float)(nImgHeight/nStepSize) );
  int minx = 0 - nAddedRangeX/nStepSize;
  int miny = 0 - nAddedRangeY/nStepSize;
  int maxx = (nImgWidth/nStepSize) + nAddedRangeX/nStepSize;
  int maxy = (nImgHeight/nStepSize)+ nAddedRangeY/nStepSize;

  OpGrayImage tmpImg( maxx - minx, maxy - miny );
  imgPatchVotesGlob = tmpImg;
  vsHoughVotesGlobal.setWindowSize( dMSMESizeX, dMSMESizeY );
  cout << "  Collecting bin votes..." << endl;
  for( int y=miny, yy=0; y<maxy; y++, yy++ )
    for( int x=minx, xx=0; x<maxx; x++, xx++ ) {
      FeatureVector fvWindowPos( 2 );
      fvWindowPos.setValue( 0, (x+0.5)*dCellSizeX );
      fvWindowPos.setValue( 1, (y+0.5)*dCellSizeY );
      
      //imgPatchVotesGlob(xx,yy) = vsHoughVotesGlobal.getVoteSum( fvWindowPos );
      imgPatchVotesGlob(xx,yy) = vsHoughVotesGlobal.getBinVoteSum(x,y);
      //cout << "    bin(" << x << "," << y << "): " 
      //     << imgPatchVotesGlob(xx,yy).value() << endl;
    }
  cout << "  done." << endl;
  
  /******************************************/
  /*   Find maxima of patch voting scores   */
  /******************************************/
  imgPatchMaxima = imgPatchVotesGlob.opNonMaximumSuppression();

  /* Refine the maxima using MSME */
  vector<Hypothesis> vPatchHypos = 
    extractHypotheses2DMSME( imgPatchVotesGlob, 
                             vsHoughVotesGlobal, vPointsInside, 
                             nImgWidth, nImgHeight, nStepSize, 
                             nAddedRangeX, nAddedRangeY,
                             dMSMESizeX, dMSMESizeY, nPatchSize,
                             nCateg, 0 );
//     extractHypotheses2D( imgPatchVotesGlob, 
//                          vsHoughVotesGlobal, vPointsInside, 
//                          nImgWidth, nImgHeight, nStepSize, 
//                          dMSMESizeX, dMSMESizeY, nPatchSize, 
//                          nCateg, 0 );
  cout << "=======================================" << endl;
  cout << "Single-Patch Voting Hypotheses:" << endl;
  for( int i=0; i<vPatchHypos.size(); i++ ) {
    cout << "  " << setw(2) << i+1 << ". ";
    printHypothesis( vPatchHypos[i] );
  }
  cout << "=======================================" << endl;
  cout << endl;

  return vPatchHypos;
}
  

vector<Hypothesis> getPatchHypothesesScaleMSME(VotingSpace &vsHoughVotesScale,
                                              const PointVector &vPointsInside,
                                               int nImgWidth, int nImgHeight, 
                                               int   nStepSize, 
                                               float dScaleMin, 
                                               float dScaleMax,
                                               int   nScaleSteps,
                                               int   nCateg,
                                               float dMSMESizeX, 
                                               float dMSMESizeY,
                                               float dMSMESizeS, 
                                               int   nPatchSize,
                                               float dThresh,
                                               bool  bExtendSearch, 
                                               int   nAddedRangeX,
                                               int   nAddedRangeY,
                                               bool  bAdaptiveScale, 
                                               float dAdaptMinScale,
                                               vector<OpGrayImage>  &vImgPatchVotesScale,
                                               vector<OpGrayImage>  &vImgPatchMaxima )
  /*******************************************************************/
  /* Extract all object hypotheses from the voting space as local    */
  /* maxima in the Hough accumulator array, the refine them using    */
  /* MSME (extended version for scale search).                       */
  /*******************************************************************/
{
  /* prepare some variables */
  float dScaleRange = dScaleMax - dScaleMin;
  //int   nScaleSteps = (int) floor(dScaleRange / dMSMESizeS);
  if( nScaleSteps == 0 ) nScaleSteps = 1;

  float dCellSizeX = ( (float)nImgWidth  / (float)(nImgWidth/nStepSize) );
  float dCellSizeY = ( (float)nImgHeight / (float)(nImgHeight/nStepSize) );
  float dCellSizeS = ( dScaleRange / (float)nScaleSteps );

  /*************************************/
  /*   Calculate patch voting scores   */
  /*************************************/
    /* search an extended range in order to find half-visible objects */
  if( !bExtendSearch ) {
    nAddedRangeX = 0;
    nAddedRangeY = 0;
  }

  int minx = 0 - nAddedRangeX/nStepSize;
  int miny = 0 - nAddedRangeY/nStepSize;
  int maxx = (nImgWidth/nStepSize) + nAddedRangeX/nStepSize;
  int maxy = (nImgHeight/nStepSize)+ nAddedRangeY/nStepSize;

  OpGrayImage tmpImg( maxx - minx, maxy - miny );
  vector<OpGrayImage> tmpVec( nScaleSteps, tmpImg );
  vImgPatchVotesScale = tmpVec;
  vImgPatchMaxima     = tmpVec;
  vsHoughVotesScale.setWindowSize( dMSMESizeX, dMSMESizeY, dMSMESizeS );
  cout << "  Collecting bin votes..." << endl;
  long nCountEmpty = 0;
  for( int y=miny, yy=0; y<maxy; y++, yy++ )
    for( int x=minx, xx=0; x<maxx; x++, xx++ )
      for( int s=0; s<nScaleSteps; s++ ) {
        FeatureVector fvWindowPos( 3 );
        fvWindowPos.setValue( 0, (x+0.5)*dCellSizeX );
        fvWindowPos.setValue( 1, (y+0.5)*dCellSizeY );
        fvWindowPos.setValue( 2, dScaleMin+(s+0.5)*dCellSizeS );
      
        /* use scale-adapted window size (only for large scales) */
        if( bAdaptiveScale )
          if( fvWindowPos.at(2) > dAdaptMinScale )
            vsHoughVotesScale.setWindowSize( dMSMESizeX*fvWindowPos.at(2), 
                                             dMSMESizeY*fvWindowPos.at(2), 
                                             dMSMESizeS*fvWindowPos.at(2) );
          else
            vsHoughVotesScale.setWindowSize( dMSMESizeX*dAdaptMinScale, 
                                             dMSMESizeY*dAdaptMinScale, 
                                             dMSMESizeS*dAdaptMinScale );
        float dVoteSum = vsHoughVotesScale.getVoteSum(fvWindowPos); // / vScaleFactor[s];
        vImgPatchVotesScale[s](xx,yy) = dVoteSum;
        if( dVoteSum <= MIN_VOTE_WEIGHT ) 
          nCountEmpty++;
        //vImgPatchVotesScale[s](xx,yy) = vsHoughVotesScale.getBinVoteSum(x,y,s);
    }
  cout << "  done." << endl;
  cout << "    (" << nCountEmpty << " out of " 
       << (maxy-miny)*(maxx-minx)*nScaleSteps << " bins were empty)." << endl;
  
  /******************************************/
  /*   Find maxima of patch voting scores   */
  /******************************************/
  cout << "  Refining candidate maxima..." << endl;
  for( int s=0; s<nScaleSteps; s++ )
    // !!! should be opNonMaximumSuppresion3D !!!
    vImgPatchMaxima[s] = vImgPatchVotesScale[s].opNonMaximumSuppression();

  /* Refine the maxima using MSME */
  vector<Hypothesis> vPatchHypos = 
    extractHypotheses3DMSME( vImgPatchVotesScale, 
                             vsHoughVotesScale, vPointsInside, 
                             nImgWidth, nImgHeight, nStepSize,
                             dScaleMin, dScaleMax,
                             nAddedRangeX, nAddedRangeY,
                             dMSMESizeX, dMSMESizeY, dMSMESizeS, nPatchSize,
                             dThresh,
                             nCateg, bAdaptiveScale, dAdaptMinScale );
  cout << "  done." << endl;

  cout << "=======================================" << endl;
  cout << "Single-Patch Voting Hypotheses:" << endl;
  for( int i=0; i<vPatchHypos.size(); i++ ) {
    cout << "  " << setw(2) << i+1 << ". ";
    printHypothesis( vPatchHypos[i] );
  }
  cout << "=======================================" << endl;
  cout << endl;

  return vPatchHypos;
}
  

vector<Hypothesis> getPatchHypothesesPose( VotingSpace &vsHoughVotesPose,
                                           PointVector       vPointsInside,
                                           vector<OpGrayImage> &vAvgImages,
                                           vector<OpGrayImage> &vOccMaps,
                                           int nImgWidth, int nImgHeight, 
                                           int nStepSize, 
                                           int nCateg,
                                           float dMSMESizeX, float dMSMESizeY,
                                           float dMSMESizeP,
                                           float dWindowSizeX, 
                                           float dWindowSizeY,
                                           int   nPatchSize,
                                           vector<OpGrayImage> &vImgPatchVotesPose,
                                           vector<OpGrayImage> &vImgPatchMaxima )
  /*******************************************************************/
  /* Extract the pose-specific object hypotheses from the voting     */
  /* space as local maxima in the Hough accumulator array.           */
  /* This routine is not fully functional!                           */
  /*******************************************************************/
{
  /*************************************/
  /*   Calculate patch voting scores   */
  /*************************************/
  OpGrayImage tmpImg( nImgWidth/nStepSize, nImgHeight/nStepSize );
  vector<OpGrayImage> tmpVec( NUM_POSES, tmpImg );
  vImgPatchVotesPose = tmpVec;
  vImgPatchMaxima    = tmpVec;

  FeatureVector fvPatchWindowSize( 2 );
  fvPatchWindowSize.setValue( 0, dWindowSizeX );
  fvPatchWindowSize.setValue( 1, dWindowSizeY );

  float dCellSizeX = ( (float)nImgWidth  / (float)(nImgWidth/nStepSize) );
  float dCellSizeY = ( (float)nImgHeight / (float)(nImgHeight/nStepSize) );
  for( int y=0; y<nImgHeight/nStepSize; y++ )
    for( int x=0; x<nImgWidth/nStepSize; x++ )
      for( int p=0; p<NUM_POSES; p++ ) {
        FeatureVector fvWindowPos( 3 );
        fvWindowPos.setValue( 0, (x+0.5)*dCellSizeX );
        fvWindowPos.setValue( 1, (y+0.5)*dCellSizeY );
        fvWindowPos.setValue( 2, (float)p );
        
//         vector<HoughVote> vSupporting = getSupportingVotes( vsHoughVotesPose, 
//                                                             fvWindowPos, 
//                                                             fvWindowSize );

//         OpGrayImage imgHypoArea = 
//           ::getHypothesisArea( vSupporting, vPointsInside, vvOccMaps,
//                                nImgWidth, nImgHeight, nPatchSize );

//         float cover = ::computeAreaCoverage( imgHypoArea, vAvgImages[p],
//                                              (int)((x+0.5)*dCellSizeX), 
//                                              (int)((y+0.5)*dCellSizeY),
//                                              0.1, 1.0 );
//         float inter = ::computeAreaOverlap( imgHypoArea, vAvgImages[p],
//                                             (int)((x+0.5)*dCellSizeX), 
//                                             (int)((y+0.5)*dCellSizeY),
//                                             0.1, 1.0 );

        OpGrayImage imgModelArea = vAvgImages[p].div( 255.0 );
        float dAreaFactor = 1.0 / imgModelArea.getSum();

        vsHoughVotesPose.setWindowSize( dMSMESizeX, dMSMESizeY, dMSMESizeP );
        vImgPatchVotesPose[p](x,y) = vsHoughVotesPose.getVoteSum(fvWindowPos);
        vImgPatchVotesPose[p](x,y) = vsHoughVotesPose.getBinVoteSum(x,y,p);
      }
  
  /******************************************/
  /*   Find maxima of patch voting scores   */
  /******************************************/
  for( int p=0; p<NUM_POSES; p++ )
    // !!! should be opNonMaximumSuppresion3D !!!
    vImgPatchMaxima[p] = vImgPatchVotesPose[p].opNonMaximumSuppression();

  vector<Hypothesis> vPatchHypos = 
    extractHypotheses3D( vImgPatchVotesPose, 
                         vsHoughVotesPose, vPointsInside, 
                         nImgWidth, nImgHeight, nStepSize, 
                         dMSMESizeX, dMSMESizeY, dMSMESizeP, nPatchSize, 
                         nCateg );
  cout << "=======================================" << endl;
  cout << "Single-Patch Voting Hypotheses:" << endl;
  for( int i=0; i<vPatchHypos.size(); i++ ) {
    cout << "  " << setw(2) << i+1 << ". ";
    printHypothesis( vPatchHypos[i] );
  }
  cout << "=======================================" << endl;
  cout << endl;

  return vPatchHypos;
}
  

vector<Hypothesis> extractHypotheses2D( OpGrayImage imgVotes, 
                                        VotingSpace &vsHoughVotesGlobal,
                                        PointVector       vPointsInside,
                                        int nImgWidth, int nImgHeight, 
                                        int nStepSize,
                                        float dMSMESizeX, float dMSMESizeY,
                                        int nPatchSize,
                                        int nCategory, int nPose )
  /*******************************************************************/
  /* Extract hypotheses from a 2D voting space.                      */
  /*******************************************************************/
{
  vector<Hypothesis> vHypotheses;
  
  /* find the vote maxima */
  OpGrayImage imgMaxima = imgVotes.opNonMaximumSuppression();

  /* find the maximum vote value */
  float min, max;
  imgMaxima.getMinMax( min, max );

  /* extract the best hypotheses */
  float dCellSizeX = ( (float)nImgWidth  / (float)(nImgWidth/nStepSize) );
  float dCellSizeY = ( (float)nImgHeight / (float)(nImgHeight/nStepSize) );
  for( int y=0; y<imgMaxima.height(); y++ )
    for( int x=0; x<imgMaxima.width(); x++ )
      if( imgMaxima(x,y).value() >= 0.1*max ) {
        Hypothesis newHypo;
        newHypo.x         = (int)((x+0.5)*nStepSize); //dCellSizeX);
        newHypo.y         = (int)((y+0.5)*nStepSize); //dCellSizeY);
        newHypo.dScore    = imgMaxima(x,y).value();
        newHypo.dScale    = 1.0;
        newHypo.nCategory = nCategory;
        newHypo.nPose     = nPose;

        /* extract the bounding box of the contributing patches */
        getHypothesisBoundingBox( vsHoughVotesGlobal, vPointsInside, 
                                  x, y, nImgWidth, nImgHeight, nStepSize,
                                  nPatchSize, 
                                  newHypo.nBoxX1, newHypo.nBoxY1,
                                  newHypo.nBoxWidth, newHypo.nBoxHeight );

        vHypotheses.push_back( newHypo );
      }
 
  /* sort the hypothesis list */
  int numHypos = vHypotheses.size();
	for( int j=0; j<numHypos-1; j++ )
		for( int i=1; i<numHypos-j; i++ )
			if( vHypotheses[i].dScore > vHypotheses[i-1].dScore ) {
				Hypothesis tmp = vHypotheses[i-1];
				vHypotheses[i-1] = vHypotheses[i];
				vHypotheses[i] = tmp;
			}

  return vHypotheses;
}


vector<Hypothesis> extractHypotheses2DMSME( OpGrayImage imgVotes, 
                                            VotingSpace &vsHoughVotesGlobal,
                                            PointVector       vPointsInside,
                                            int nImgWidth, int nImgHeight, 
                                            int nStepSize,
                                            int nAddedRangeX,
                                            int nAddedRangeY,
                                            float dMSMESizeX, float dMSMESizeY,
                                            int nPatchSize,
                                            int nCategory, int nPose )
  /*******************************************************************/
  /* Extract hypotheses from a 2D voting space and refine them using */
  /* MSME.                                                           */
  /*******************************************************************/
{
  cout << "called extractHypotheses2DMSME()..." << endl;
  vector<Hypothesis> vHypotheses;
  
  /* find the vote maxima */
  OpGrayImage imgMaxima = imgVotes.opNonMaximumSuppression();

  /* find the maximum vote value */
  float min, max;
  imgMaxima.getMinMax( min, max );

  /* prepare some variables */
  float dCellSizeX = ( (float)nImgWidth  / (float)(nImgWidth/nStepSize) );
  float dCellSizeY = ( (float)nImgHeight / (float)(nImgHeight/nStepSize) );

  /* extract the best hypotheses */
  vsHoughVotesGlobal.setWindowSize( dMSMESizeX, dMSMESizeY );
  cout << "  Extracting maxima..." << endl;
  for( int y=0; y<imgMaxima.height(); y++ )
    for( int x=0; x<imgMaxima.width(); x++ )
      if( imgMaxima(x,y).value() >= 0.1*max ) {
        /*------------------------------------*/
        /* NEW: refine the maximum using MSME */
        /*------------------------------------*/
        FeatureVector fvStart( 2 );
        fvStart.setValue( 0, (x-nAddedRangeX/nStepSize+0.5)*dCellSizeX );
        fvStart.setValue( 1, (y-nAddedRangeY/nStepSize+0.5)*dCellSizeY );

        FeatureVector fvResult( 2 );
        float dScore;
        vsHoughVotesGlobal.applyMSME( fvStart, fvResult, dScore );
       
        Hypothesis newHypo;
        newHypo.x         = (int)fvResult.at(0);
        newHypo.y         = (int)fvResult.at(1);
        newHypo.dScore    = dScore;
        newHypo.dScale    = 1.0;
        newHypo.nCategory = nCategory;
        newHypo.nPose     = nPose;

        /* extract the bounding box of the contributing patches */
        getHypothesisBoundingBox( vsHoughVotesGlobal, vPointsInside, 
                                  x-nAddedRangeX/nStepSize, 
                                  y-nAddedRangeY/nStepSize, 
                                  nImgWidth, nImgHeight, nStepSize,
                                  nPatchSize, 
                                  newHypo.nBoxX1, newHypo.nBoxY1,
                                  newHypo.nBoxWidth, newHypo.nBoxHeight );

        vHypotheses.push_back( newHypo );
      }
  cout << "  done." << endl;
 
  /* sort the hypothesis list */
  int numHypos = vHypotheses.size();
	for( int j=0; j<numHypos-1; j++ )
		for( int i=1; i<numHypos-j; i++ )
			if( vHypotheses[i].dScore > vHypotheses[i-1].dScore ) {
				Hypothesis tmp = vHypotheses[i-1];
				vHypotheses[i-1] = vHypotheses[i];
				vHypotheses[i] = tmp;
			}

  cout << "done." << endl;
  return vHypotheses;
}


vector<Hypothesis> refineHypotheses2DMSME( vector<Hypothesis> &vInitialHypos, 
                                           VotingSpace &vsHoughVotesGlobal,
                                           PointVector       vPointsInside,
                                           int nImgWidth, int nImgHeight, 
                                           int nStepSize,
                                           float dMSMESizeX, float dMSMESizeY,
                                           int nPatchSize,
                                           int nCategory, int nPose )
  /*******************************************************************/
  /* Refine hypotheses from a 2D voting space with uniform sampling  */
  /* (also using MSME).                                              */
  /*******************************************************************/
{
  vector<Hypothesis> vHypotheses;
  
  /* prepare some variables */
  /* refine the existing hypotheses */
  vsHoughVotesGlobal.setWindowSize( dMSMESizeX, dMSMESizeY );
  for( int i=0; i<vInitialHypos.size(); i++ ) {
    int x = vInitialHypos[i].x;
    int y = vInitialHypos[i].y;

    /*------------------------------------*/
    /* NEW: refine the maximum using MSME */
    /*------------------------------------*/
    FeatureVector fvStart( 2 );
    fvStart.setValue( 0, x );
    fvStart.setValue( 1, y ); 

    FeatureVector fvResult( 2 );
    float dScore;
    vsHoughVotesGlobal.applyMSME( fvStart, fvResult, dScore );
       
    Hypothesis newHypo;
    newHypo.x         = (int)fvResult.at(0);
    newHypo.y         = (int)fvResult.at(1);
    newHypo.dScore    = dScore;
    newHypo.nCategory = nCategory;
    newHypo.nPose     = nPose;
    
    /* extract the bounding box of the contributing patches */
    getHypothesisBoundingBox( vsHoughVotesGlobal, vPointsInside, 
                              x, y, 
                              nImgWidth, nImgHeight, nStepSize,
                              nPatchSize, 
                              newHypo.nBoxX1, newHypo.nBoxY1,
                              newHypo.nBoxWidth, newHypo.nBoxHeight );
    
    vHypotheses.push_back( newHypo );
  }
  
  /* sort the hypothesis list */
  int numHypos = vHypotheses.size();
	for( int j=0; j<numHypos-1; j++ )
		for( int i=1; i<numHypos-j; i++ )
			if( vHypotheses[i].dScore > vHypotheses[i-1].dScore ) {
				Hypothesis tmp = vHypotheses[i-1];
				vHypotheses[i-1] = vHypotheses[i];
				vHypotheses[i] = tmp;
			}

  return vHypotheses;
}


vector<Hypothesis> refineHypotheses3DMSME( vector<Hypothesis> &vInitialHypos, 
                                           VotingSpace &vsHoughVotesScale,
                                           PointVector       vPointsInside,
                                           int nImgWidth, int nImgHeight, 
                                           int nStepSize,
                                           float dMSMESizeX, float dMSMESizeY,
                                           float dMSMESizeS,
                                           int nPatchSize,
                                           int nCategory, int nPose )
  /*******************************************************************/
  /* Refine hypotheses from a 3D voting space with uniform sampling  */
  /* (also using MSME).                                              */
  /*******************************************************************/
{
  vector<Hypothesis> vHypotheses;
  
  /* prepare some variables */
  /* refine the existing hypotheses */
  vsHoughVotesScale.setWindowSize( dMSMESizeX, dMSMESizeY, dMSMESizeS );
  for( int i=0; i<vInitialHypos.size(); i++ ) {
    int x       = vInitialHypos[i].x;
    int y       = vInitialHypos[i].y;
    float scale = vInitialHypos[i].dScale;

    /*------------------------------------*/
    /* NEW: refine the maximum using MSME */
    /*------------------------------------*/
    FeatureVector fvStart( 3 );
    fvStart.setValue( 0, x );
    fvStart.setValue( 1, y ); 
    fvStart.setValue( 2, scale ); 

    FeatureVector fvResult( 3 );
    float dScore;
    vsHoughVotesScale.applyMSME( fvStart, fvResult, dScore );
       
    Hypothesis newHypo;
    newHypo.x         = (int)fvResult.at(0);
    newHypo.y         = (int)fvResult.at(1);
    newHypo.dScale    = fvResult.at(2);
    newHypo.dScore    = dScore;
    newHypo.nCategory = nCategory;
    newHypo.nPose     = nPose;
    
    /* extract the bounding box of the contributing patches */
//     getHypothesisBoundingBox( vsHoughVotesScale, vPointsInside, 
//                               x, y, 
//                               nImgWidth, nImgHeight, nStepSize,
//                               nPatchSize, 
//                               newHypo.nBoxX1, newHypo.nBoxY1,
//                               newHypo.nBoxWidth, newHypo.nBoxHeight );
    
    vHypotheses.push_back( newHypo );
  }
  
  /* sort the hypothesis list */
  int numHypos = vHypotheses.size();
	for( int j=0; j<numHypos-1; j++ )
		for( int i=1; i<numHypos-j; i++ )
			if( vHypotheses[i].dScore > vHypotheses[i-1].dScore ) {
				Hypothesis tmp = vHypotheses[i-1];
				vHypotheses[i-1] = vHypotheses[i];
				vHypotheses[i] = tmp;
			}

  return vHypotheses;
}


vector<Hypothesis> extractHypotheses3D( vector<OpGrayImage> vImgVotes, 
                                        VotingSpace &vsHoughVotesPose,
                                        PointVector       vPointsInside,
                                        int nImgWidth, int nImgHeight, 
                                        int nStepSize,
                                        float dMSMESizeX, float dMSMESizeY,
                                        float dMSMESizeP,
                                        int nPatchSize,
                                        int nCategory )
  /*******************************************************************/
  /* Extract hypotheses from a 3D voting space.                      */
  /*******************************************************************/
{
  vector<Hypothesis> vHypotheses;
  
  /* find the vote maxima */
  vector<OpGrayImage> vImgMaxima( NUM_POSES );
  for( int p=0; p<NUM_POSES; p++ )
    // !!! should be opNonMaximumSuppression3D !!!
    vImgMaxima[p] = vImgVotes[p].opNonMaximumSuppression();

  /* find the maximum vote value */
  float min, max;
  vImgMaxima[0].getMinMax( min, max );
  for( int p=1; p<NUM_POSES; p++ ) {
    float min_in_pose, max_in_pose;
    vImgMaxima[p].getMinMax( min_in_pose, max_in_pose );

    if( min_in_pose < min )
      min = min_in_pose;
    if( max_in_pose > max )
      max = max_in_pose;
  }

  /* extract the best hypotheses */
  float dCellSizeX = ( (float)nImgWidth  / (float)(nImgWidth/nStepSize) );
  float dCellSizeY = ( (float)nImgHeight / (float)(nImgHeight/nStepSize) );
  for( int p=0; p<NUM_POSES; p++ )
    for( int y=0; y<vImgMaxima[p].height(); y++ )
      for( int x=0; x<vImgMaxima[p].width(); x++ )
        if( vImgMaxima[p](x,y).value() >= 0.1*max ) {
          Hypothesis newHypo;
          newHypo.x         = (int)((x+0.5)*nStepSize); //*dCellSizeX);
          newHypo.y         = (int)((y+0.5)*nStepSize); //dCellSizeY);
          newHypo.dScore    = vImgMaxima[p](x,y).value();
          newHypo.dScale    = 1.0;
          newHypo.nCategory = nCategory;
          newHypo.nPose     = p;
          
          /* extract the bounding box of the contributing patches */
          getHypothesisBoundingBox( vsHoughVotesPose, vPointsInside, 
                                    x, y, p, nImgWidth, nImgHeight, nStepSize,
                                    nPatchSize, 
                                    newHypo.nBoxX1, newHypo.nBoxY1,
                                    newHypo.nBoxWidth, newHypo.nBoxHeight );
          
          vHypotheses.push_back( newHypo );
        }
  
  /* sort the hypothesis list */
  int numHypos = vHypotheses.size();
	for( int j=0; j<numHypos-1; j++ )
		for( int i=1; i<numHypos-j; i++ )
			if( vHypotheses[i].dScore > vHypotheses[i-1].dScore ) {
				Hypothesis tmp = vHypotheses[i-1];
				vHypotheses[i-1] = vHypotheses[i];
				vHypotheses[i] = tmp;
			}

  return vHypotheses;
}


vector<Hypothesis> extractHypotheses3DMSME( /*const*/ vector<OpGrayImage> &vImgVotes, 
                                            VotingSpace &vsHoughVotes,
                                            const PointVector &vPointsInside,
                                            int nImgWidth, int nImgHeight, 
                                            int nStepSize,
                                            float dScaleMin, float dScaleMax,
                                            int   nAddedRangeX,
                                            int   nAddedRangeY,
                                            float dMSMESizeX, float dMSMESizeY,
                                            float dMSMESizeS,
                                            int   nPatchSize,
                                            float dThresh,
                                            int   nCategory,
                                            bool  bAdaptiveScale,
                                            float dAdaptMinScale )
  /*******************************************************************/
  /* Extract hypotheses from a 3D voting space and refine them using */
  /* MSME.                                                           */
  /*******************************************************************/
{
  vector<Hypothesis> vHypotheses;
  
  /* find the vote maxima */
  int   nScaleSteps = vImgVotes.size();;
  vector<OpGrayImage> vImgMaxima( nScaleSteps );
  for( int s=0; s<nScaleSteps; s++ )
    // !!! should be opNonMaximumSuppression3D !!!
    vImgMaxima[s] = vImgVotes[s].opNonMaximumSuppression();

  /* find the maximum vote value */
  float min, max;
  vImgMaxima[0].getMinMax( min, max );
  for( int s=1; s<nScaleSteps; s++ ) {
    float min_in_scale, max_in_scale;
    vImgMaxima[s].getMinMax( min_in_scale, max_in_scale );

    if( min_in_scale < min )
      min = min_in_scale;
    if( max_in_scale > max )
      max = max_in_scale;
  }

  /* prepare some variables */
  float dScaleRange = dScaleMax - dScaleMin;

  float dCellSizeX = ( (float)nImgWidth  / (float)(nImgWidth/nStepSize) );
  float dCellSizeY = ( (float)nImgHeight / (float)(nImgHeight/nStepSize) );
  float dCellSizeS = ( dScaleRange / (float)nScaleSteps );

  /* extract the best hypotheses */
  vsHoughVotes.setWindowSize( dMSMESizeX, dMSMESizeY, dMSMESizeS );
  for( int s=0; s<nScaleSteps; s++ )
    for( int y=0; y<vImgMaxima[s].height(); y++ )
      for( int x=0; x<vImgMaxima[s].width(); x++ )
        if( (vImgMaxima[s](x,y).value() >= 0.15*max) && 
            (vImgMaxima[s](x,y).value() >= 0.9*dThresh) ) {
          /*------------------------------------*/
          /* NEW: refine the maximum using MSME */
          /*------------------------------------*/
          FeatureVector fvStart( 3 );
          fvStart.setValue( 0, (x-nAddedRangeX/nStepSize+0.5)*dCellSizeX );
          fvStart.setValue( 1, (y-nAddedRangeY/nStepSize+0.5)*dCellSizeY );
          fvStart.setValue( 2, dScaleMin+(s+0.5)*dCellSizeS );
          
          FeatureVector fvResult( 3 );
          float dScore;

          /* use scale-adapted window size (only for large scales) */
//           if( fvStart.at(2) > 1.0 )
//             vsHoughVotes.setWindowSize( dMSMESizeX*fvStart.at(2), 
//                                         dMSMESizeY*fvStart.at(2), 
//                                         dMSMESizeS );
//           else
//             vsHoughVotes.setWindowSize( dMSMESizeX, 
//                                         dMSMESizeY, 
//                                         dMSMESizeS );
          
          vsHoughVotes.applyMSME( fvStart, fvResult, dScore, 
                                  bAdaptiveScale, dAdaptMinScale );
          
          Hypothesis newHypo;
          newHypo.x         = (int)fvResult.at(0);
          newHypo.y         = (int)fvResult.at(1);
          newHypo.dScale    = (float)fvResult.at(2);
          newHypo.dScore    = dScore;
          newHypo.nCategory = nCategory;
          newHypo.nPose     = 0;
          
          /* extract the bounding box of the contributing patches */
//           getHypothesisBoundingBox( vHoughVotesScale, vPointsInside, 
//                                     newHypo.x, 
//                                     newHypo.y,
//                                     newHypo.dScale,
//                                     nImgWidth, nImgHeight, nStepSize,
//                                     nPatchSize, 
//                                     newHypo.nBoxX1, newHypo.nBoxY1,
//                                     newHypo.nBoxWidth, newHypo.nBoxHeight );
          
          vHypotheses.push_back( newHypo );
        }
  
  /* sort the hypothesis list */
  int numHypos = vHypotheses.size();
	for( int j=0; j<numHypos-1; j++ )
		for( int i=1; i<numHypos-j; i++ )
			if( vHypotheses[i].dScore > vHypotheses[i-1].dScore ) {
				Hypothesis tmp = vHypotheses[i-1];
				vHypotheses[i-1] = vHypotheses[i];
				vHypotheses[i] = tmp;
			}
  
  return vHypotheses;
}


void getHypothesisBoundingBox( VotingSpace &vsHoughVotesGlobal,
                               PointVector       vPointsInside,
                               int x, int y, 
                               int nImgWidth, int nImgHeight, int nStepSize,
                               int nPatchSize,
                               int &x1, int &y1, 
                               int &width, int &height )
  /*******************************************************************/
  /* Compute the bounding box of a hypothesis (as determined by the  */
  /* patches from its support (not yet from its segmentation!)).     */
  /*******************************************************************/
{
  float dCellSizeX = (float)nImgWidth /(float)(nImgWidth/nStepSize);
  float dCellSizeY = (float)nImgHeight/(float)(nImgHeight/nStepSize);

  FeatureVector fvWindowPos( 2 );
  fvWindowPos.setValue( 0, (x+0.5)*dCellSizeX );
  fvWindowPos.setValue( 1, (y+0.5)*dCellSizeY );
  
  /* get all contributing votes */
  vector<HoughVote> vVotes;
  vVotes = vsHoughVotesGlobal.getSupportingVotes( fvWindowPos );
  
  /* extract the bounding box */
  int min_x = x;
  int max_x = x;
  int min_y = y;
  int max_y = y;
  for( int i=0; i<vVotes.size(); i++ ) {
    int pos_x = vPointsInside[vVotes[i].getImgPointId()].x;
    int pos_y = vPointsInside[vVotes[i].getImgPointId()].y;
    
    if( pos_x - nPatchSize < min_x ) min_x = pos_x - nPatchSize;
    if( pos_x + nPatchSize > max_x ) max_x = pos_x + nPatchSize;
    if( pos_y - nPatchSize < min_y ) min_y = pos_y - nPatchSize;
    if( pos_y + nPatchSize > min_y ) max_y = pos_y + nPatchSize;
  }
  x1     = min_x;
  y1     = min_y;
  width  = max_x - min_x;
  height = max_y - min_y;
}


void getHypothesisBoundingBox( VotingSpace &vsHoughVotesPose,
                               PointVector       vPointsInside,
                               int x, int y, int p, 
                               int nImgWidth, int nImgHeight, int nStepSize,
                               int nPatchSize,
                               int &x1, int &y1, 
                               int &width, int &height )
  /*******************************************************************/
  /* Compute the bounding box of a hypothesis (as determined by the  */
  /* patches from its support (not yet from its segmentation!)).     */
  /*******************************************************************/
{
  float dCellSizeX = (float)nImgWidth /(float)(nImgWidth/nStepSize);
  float dCellSizeY = (float)nImgHeight/(float)(nImgHeight/nStepSize);

  FeatureVector fvWindowPos( 3 );
  fvWindowPos.setValue( 0, (x+0.5)*dCellSizeX );
  fvWindowPos.setValue( 1, (y+0.5)*dCellSizeY );
  fvWindowPos.setValue( 2, (float)p );
  
  /* get all contributing votes */
  vector<HoughVote> vVotes = vsHoughVotesPose.getSupportingVotes( fvWindowPos);
  
  /* extract the bounding box */
  int min_x = x;
  int max_x = x;
  int min_y = y;
  int max_y = y;
  for( int i=0; i<vVotes.size(); i++ ) {
    int pos_x = vPointsInside[vVotes[i].getImgPointId()].x;
    int pos_y = vPointsInside[vVotes[i].getImgPointId()].y;
    
    if( pos_x - nPatchSize < min_x ) min_x = pos_x - nPatchSize;
    if( pos_x + nPatchSize > max_x ) max_x = pos_x + nPatchSize;
    if( pos_y - nPatchSize < min_y ) min_y = pos_y - nPatchSize;
    if( pos_y + nPatchSize > min_y ) max_y = pos_y + nPatchSize;
  }
  x1     = min_x;
  y1     = min_y;
  width  = max_x - min_x;
  height = max_y - min_y;
}


OpGrayImage getHypothesisArea( VotingSpace &vsHoughVotesPose,
                               PointVector       vPointsInside,
                               //vector< vector<FeatureVector> > &vvOccMaps,
                               vector<FeatureVector> &vOccMaps,
                               int x, int y, int nPose,
                               int nImgWidth, int nImgHeight, int nStepSize,
                               float dMSMESizeX, float dMSMESizeY, 
                               float dMSMESizeP,
                               int nPatchSize )
  /*******************************************************************/
  /* Compute the area described by a hypothesis (as determined by    */
  /* the patches from its support (not yet from its segmentation!)). */ 
  /*******************************************************************/
{
  float dCellSizeX = (float)nImgWidth /(float)(nImgWidth/nStepSize);
  float dCellSizeY = (float)nImgHeight/(float)(nImgHeight/nStepSize);

  FeatureVector fvWindowPos( 3 );
  fvWindowPos.setValue( 0, (x+0.5)*dCellSizeX );
  fvWindowPos.setValue( 1, (y+0.5)*dCellSizeY );
  fvWindowPos.setValue( 2, nPose );
  
  /* get all contributing votes */
  vector<HoughVote> vVotes = vsHoughVotesPose.getSupportingVotes( fvWindowPos);
  
  /* copy the patches to an image */
  OpGrayImage imgArea( nImgWidth, nImgHeight );
  for( int i=0; i<vVotes.size(); i++ ) {
    int pos_x = vPointsInside[vVotes[i].getImgPointId()].x;
    int pos_y = vPointsInside[vVotes[i].getImgPointId()].y;
    int nClusterId = vVotes[i].getClusterId();
    int nOccNumber = vVotes[i].getOccNumber();
    
    int min_x = max( 0, pos_x - nPatchSize);
    int max_x = min( nImgWidth, pos_x + nPatchSize+1);
    int min_y = max( 0, pos_y - nPatchSize);
    int max_y = min( nImgHeight, pos_y + nPatchSize+1);
//     FeatureVector fvOccMap;
//     if( !vvOccMaps.empty() )
//       fvOccMap = vvOccMaps[nClusterId][nOccNumber];
    
    for( int y=min_y, yy=0; y<max_y; y++, yy++ )
      for( int x=min_x, xx=0; x<max_x; x++, xx++ )
//         if( vvOccMaps.empty() )
          imgArea(x,y) = imgArea(x,y).value() + vVotes[i].getValue();
//         else {
//           int idx = yy*(2*nPatchSize+1) + xx;
//           imgArea(x,y) = ( imgArea(x,y).value() + 
//                            vVotes[i].dValue*fvOccMap.at(idx)/255.0);
//         }
  }

  return imgArea;
}


OpGrayImage getHypothesisArea( vector<HoughVote> &vSupportingVotes,
                               PointVector       vPointsInside,
                               //vector< vector<OpGrayImage> > &vvOccMaps,
                               vector<OpGrayImage> &vOccMaps,
                               int nImgWidth, int nImgHeight, 
                               int nPatchSize )
  /*******************************************************************/
  /* Compute the area described by a hypothesis (as determined by    */
  /* the patches from its support (not yet from its segmentation!)). */ 
  /*******************************************************************/
{
  /* copy the patches to an image */
  OpGrayImage imgArea( nImgWidth, nImgHeight );
  for( int i=0; i<vSupportingVotes.size(); i++ ) {
    int pos_x = vPointsInside[vSupportingVotes[i].getImgPointId()].x;
    int pos_y = vPointsInside[vSupportingVotes[i].getImgPointId()].y;
    int nClusterId = vSupportingVotes[i].getClusterId();
    int nOccNumber = vSupportingVotes[i].getOccNumber();
    
    int min_x = max( 0, pos_x - nPatchSize);
    int max_x = min( nImgWidth, pos_x + nPatchSize+1);
    int min_y = max( 0, pos_y - nPatchSize);
    int max_y = min( nImgHeight, pos_y + nPatchSize+1);
//     OpGrayImage imgOccMap;
//     if( !vvOccMaps.empty() )
//       imgOccMap = vvOccMaps[nClusterId][nOccNumber];
    
    for( int y=min_y, yy=0; y<max_y; y++, yy++ )
      for( int x=min_x, xx=0; x<max_x; x++, xx++ )
//         if( vvOccMaps.empty() )
          imgArea(x,y) = imgArea(x,y).value() + vSupportingVotes[i].getValue();
//         else {
//           int idx = yy*(2*nPatchSize+1) + xx;
//           imgArea(x,y) = ( imgArea(x,y).value() + 
//                            vSupportingVotes[i].dValue*imgOccMap(xx,yy).value()/255.0);
//         }
  }

  return imgArea;
}


float computeBoundingBoxCoverage( Hypothesis h1, Hypothesis h2 )
  /*******************************************************************/
  /* Compute the coverage of bounding box 2 by bbox 1.               */
  /*******************************************************************/
{
  int common_x1 = max( h1.nBoxX1, h2.nBoxX1 );
  int common_y1 = max( h1.nBoxY1, h2.nBoxY1 );
  int common_x2 = min( h1.nBoxX1+h1.nBoxWidth,  h2.nBoxX1+h2.nBoxWidth );
  int common_y2 = min( h1.nBoxY1+h1.nBoxHeight, h2.nBoxY1+h2.nBoxHeight );

  float area1 = (float)h1.nBoxWidth * h1.nBoxHeight;
  float area2 = (float)h2.nBoxWidth * h2.nBoxHeight;
  float common_area;
  if( (common_x2 > common_x1) && (common_y2 > common_y1) )
    common_area = (common_x2-common_x1)*(common_y2-common_y1);
  else
    common_area = 0.0;

  float cover = (common_area/area2);
  return cover;
}


float computeBoundingBoxOverlap( Hypothesis h1, Hypothesis h2 )
  /*******************************************************************/
  /* Compute the overlap between two bounding boxes with the         */
  /* intersection criterion.                                         */
  /*******************************************************************/
{
  int common_x1 = max( h1.nBoxX1, h2.nBoxX1 );
  int common_y1 = max( h1.nBoxY1, h2.nBoxY1 );
  int common_x2 = min( h1.nBoxX1+h1.nBoxWidth,  h2.nBoxX1+h2.nBoxWidth );
  int common_y2 = min( h1.nBoxY1+h1.nBoxHeight, h2.nBoxY1+h2.nBoxHeight );

  float area1 = (float)h1.nBoxWidth * h1.nBoxHeight;
  float area2 = (float)h2.nBoxWidth * h2.nBoxHeight;
  float common_area;
  if( (common_x2 > common_x1) && (common_y2 > common_y1) )
    common_area = (common_x2-common_x1)*(common_y2-common_y1);
  else
    common_area = 0.0;

  float inter = 0.5*(common_area/area1 + common_area/area2);
  return inter;
}


float computeAreaCoverage( OpGrayImage imgHypoArea, OpGrayImage imgModelArea,
                           float dThresh1, float dThresh2 )
  /*******************************************************************/
  /* Compute the coverage of model by hypothesis area1.              */
  /*******************************************************************/
{
  assert( imgHypoArea.width() == imgModelArea.width() );
  assert( imgHypoArea.height() == imgModelArea.height() );

  int nCount1    = 0;
  int nCount2    = 0;
  int nCountBoth = 0;

  for( int y=0; y<imgHypoArea.height(); y++ )
    for( int x=0; x<imgHypoArea.width(); x++ ) {
      if( imgHypoArea(x,y).value() >= dThresh1 )
        nCount1++;
      if( imgModelArea(x,y).value() >= dThresh2 )
        nCount2++;
      if( (imgHypoArea(x,y).value() >= dThresh1) && 
          (imgModelArea(x,y).value() >= dThresh2) )
        nCountBoth++;
    }
  
  float cover = ((float)nCountBoth/(float)nCount2);
  return cover;
}


float computeAreaCoverage( OpGrayImage imgHypoArea, OpGrayImage imgModelArea,
                           int offset_x, int offset_y,
                           float dThresh1, float dThresh2 )
  /*******************************************************************/
  /* Compute the coverage of model by hypothesis area1.              */
  /*******************************************************************/
{
  int nCount1    = 0;
  int nCount2    = 0;
  int nCountBoth = 0;

  OpGrayImage imgModel( imgHypoArea.width(), imgHypoArea.height() );
  imgModel = imgModel.add( imgModelArea, offset_x - imgModelArea.width()/2, 
                           offset_y - imgModelArea.height()/2 );

  for( int y=0; y<imgHypoArea.height(); y++ )
    for( int x=0; x<imgHypoArea.width(); x++ ) {
      if( imgHypoArea(x,y).value() >= dThresh1 )
        nCount1++;
      if( imgModel(x,y).value() >= dThresh2 )
        nCount2++;
      if( (imgHypoArea(x,y).value() >= dThresh1) && 
          (imgModel(x,y).value() >= dThresh2) )
        nCountBoth++;
    }
  
  float cover = ((float)nCountBoth/(float)nCount2);
  return cover;
}


float computeAreaOverlap( OpGrayImage imgHypoArea, OpGrayImage imgModelArea,
                          float dThresh1, float dThresh2 )
  /*******************************************************************/
  /* compute the overlap between two hypothesis areas with the       */
  /* intersection criterion.                                         */
  /*******************************************************************/
{
  assert( imgHypoArea.width() == imgModelArea.width() );
  assert( imgHypoArea.height() == imgModelArea.height() );

  int nCount1    = 0;
  int nCount2    = 0;
  int nCountBoth = 0;

  for( int y=0; y<imgHypoArea.height(); y++ )
    for( int x=0; x<imgHypoArea.width(); x++ ) {
      if( imgHypoArea(x,y).value() >= dThresh1 )
        nCount1++;
      if( imgModelArea(x,y).value() >= dThresh2 )
        nCount2++;
      if( (imgHypoArea(x,y).value() >= dThresh1) && 
          (imgModelArea(x,y).value() >= dThresh2) )
        nCountBoth++;
    }
  
  float inter;
  if( nCount1 == 0 )
    inter = 0;
  else
    inter = 0.5*((float)nCountBoth/(float)nCount1 + 
                 (float)nCountBoth/(float)nCount2 );

  return inter;
}


float computeAreaOverlap( OpGrayImage imgHypoArea, OpGrayImage imgModelArea,
                          int offset_x, int offset_y,
                          float dThresh1, float dThresh2 )
  /*******************************************************************/
  /* Compute the overlap between two hypothesis areas with the       */
  /* intersection criterion.                                         */
  /*******************************************************************/
{
  int nCount1    = 0;
  int nCount2    = 0;
  int nCountBoth = 0;

  OpGrayImage imgModel( imgHypoArea.width(), imgHypoArea.height() );
  imgModel = imgModel.add( imgModelArea, offset_x - imgModelArea.width()/2, 
                           offset_y - imgModelArea.height()/2 );

  for( int y=0; y<imgHypoArea.height(); y++ )
    for( int x=0; x<imgHypoArea.width(); x++ ) {
      if( imgHypoArea(x,y).value() >= dThresh1 )
        nCount1++;
      if( imgModel(x,y).value() >= dThresh2 )
        nCount2++;
      if( (imgHypoArea(x,y).value() >= dThresh1) && 
          (imgModel(x,y).value() >= dThresh2) )
        nCountBoth++;
    }
  
  float inter;
  if( nCount1 == 0 )
    inter = 0;
  else
    inter = 0.5*((float)nCountBoth/(float)nCount1 + 
                     (float)nCountBoth/(float)nCount2 );
  return inter;
}


void printHypothesis( Hypothesis hypo )
  /*******************************************************************/
  /* Print a hypothesis to stdout for debugging purposes.            */
  /*******************************************************************/
{
  cout << setw(5) << setprecision(5) << "  Score=" << hypo.dScore 
       << setw(2) << ": Pos(" << hypo.x << "," << hypo.y 
       << "), Category=" << hypo.nCategory 
       << ", Scale=" << hypo.dScale << ", Pose=" << hypo.nPose << endl;
}


void printHypothesisMDL( Hypothesis hypo )
  /*******************************************************************/
  /* Print a hypothesis to stdout for debugging purposes.            */
  /*******************************************************************/
{
  cout << setw(5) << setprecision(5) << "  Score=" << hypo.dScore 
       << " (MDL=" << hypo.dScoreMDL << ")"
       << setw(2) << ": Pos(" << hypo.x << "," << hypo.y 
       << "), Category=" << hypo.nCategory 
       << ", Scale=" << hypo.dScale << ", Pose=" << hypo.nPose << endl;
}


void getResultRanks( vector<Hypothesis> vRecoResults,
                     vector<Hypothesis> vGroundTruth,
                     vector<float> vTolerances,
                     vector<Result> &vRawResults,
                     vector<Result> &vResultRanks )
  /*******************************************************************/
  /* Compare the recognition results with the ground truth and       */
  /* compute the ranks of the correct hypotheses (or -1 if there was */ 
  /* no correct hypothesis.                                          */
  /*******************************************************************/
{
  /*************************************/
  /*   Initialize the result vectors   */
  /*************************************/
  vRawResults.clear();
  vector<Result> tmp( vRecoResults.size() );
  vRawResults = tmp;

  vResultRanks.clear();
  vector<Result> tmp2( vGroundTruth.size() );
  vResultRanks = tmp2;

  /*******************************/
  /*   Prepare the raw results   */
  /*******************************/
  /* check every recognition hypothesis */
  for( int j=0; j<vRecoResults.size(); j++ ) {
    vRawResults[j].nRank  = -1;
    vRawResults[j].dDistX = -1.0;
    vRawResults[j].dDistY = -1.0;
    vRawResults[j].dDist  = -1.0;
    vRawResults[j].dCover   = -1.0;
    vRawResults[j].dOverlap = -1.0;
    
    /* with every ground truth item */
    float max_overlap = -1.0;
    for( int i=0; i<vGroundTruth.size(); i++ ) {
      float dx   = vRecoResults[j].x - vGroundTruth[i].x;
      float dy   = vRecoResults[j].y - vGroundTruth[i].y;
      float dist = sqrt(dx*dx + dy*dy);
      
      float dCoverage = computeBoundingBoxCoverage(vRecoResults[j], 
                                                   vGroundTruth[i]);
      float dOverlap = computeBoundingBoxOverlap(vRecoResults[j], 
                                                 vGroundTruth[i]);

      /* record the distance to the nearest ground truth item */
      if( dOverlap > max_overlap ) {
        vRawResults[j].dDistX = dx;
        vRawResults[j].dDistY = dy;
        vRawResults[j].dDist  = dist;
        
        vRawResults[j].dCover   = dCoverage;
        vRawResults[j].dOverlap = dOverlap;
        
        max_overlap = dOverlap;
      }
    }
  }
  
  
  /**********************************/
  /*   Start the matching process   */
  /**********************************/
  vector<bool> vUsed ( vRecoResults.size(), false );
  /* for every ground truth item */
    for( int i=0; i<vGroundTruth.size(); i++ ) {
    
    /* check every recognition hypothesis */
    bool found = false;
    int nRank = 1;
    for( int j=0; j<vRecoResults.size() && !found; j++ )
      if( !vUsed[j] ) {
        float dx   = vRecoResults[j].x - vGroundTruth[i].x;
        float dy   = vRecoResults[j].y - vGroundTruth[i].y;
        float dist = sqrt(dx*dx + dy*dy);

        float dCoverage = computeBoundingBoxCoverage(vRecoResults[j], 
                                                     vGroundTruth[i]);
        float dOverlap = computeBoundingBoxOverlap(vRecoResults[j], 
                                                   vGroundTruth[i]);

        if( (fabs((double)vRecoResults[j].x - vGroundTruth[i].x) 
             <= vTolerances[0]) &&
            (fabs((double)vRecoResults[j].y - vGroundTruth[i].y) 
             <= vTolerances[1]) &&
            (dCoverage >= vTolerances[2]) && (dOverlap >= vTolerances[3]) ) {
          vResultRanks[i].nRank = nRank;
          
          vResultRanks[i].dDistX = dx;
          vResultRanks[i].dDistY = dy;
          vResultRanks[i].dDist  = dist;
          
          vResultRanks[i].dCover   = dCoverage;
          vResultRanks[i].dOverlap = dOverlap;

          found = true;
          vUsed[j] = true;
        }
        else
          nRank++;
      }
    
    if( !found ) {
      vResultRanks[i].nRank   = -1;
      vResultRanks[i].dDist   = -1.0;
      vResultRanks[i].dCover  = -1.0;
      vResultRanks[i].dOverlap= -1.0;
    }
  }
}


void writeResultsToDisk( ofstream &ofile, int nImgNumber,
                         vector<Result> vResults )
  /*******************************************************************/
  /* Write the results to disk in a plain ASCII file format.         */
  /*******************************************************************/
{
  if( ofile != 0 )
    for( int i=0; i<vResults.size(); i++ )
      ofile << nImgNumber << '\t' << i+1 << '\t' << vResults[i].dDistX
            << '\t' << vResults[i].dDistY << '\t' << vResults[i].dDist
            << '\t' << vResults[i].dCover << '\t' << vResults[i].dOverlap
            << endl;
}


void writeResultsToDiskUIUC( ofstream &ofile, int nImgNumber,
                             vector<Hypothesis> vHypos )
  /*******************************************************************/
  /* Write the results to disk in the Agarwal & Roth format.         */
  /*******************************************************************/
{
  if( ofile != 0 ) {
    ofile << ' ' << nImgNumber << ":";
    
    for( int i=0; i<vHypos.size(); i++ )
      ofile << " (" << vHypos[i].nBoxY1 << "," << vHypos[i].nBoxX1 << ")";
    ofile << endl;
  }
}


void writeResultsToDiskScore( ofstream &ofile, int nImgNumber,
                              vector<Hypothesis> vHypos )
  /*******************************************************************/
  /* Write the results to disk including the exact value of the      */
  /* hypotheses scores.                                              */
  /*******************************************************************/
{
  if( ofile != 0 ) {
    ofile << nImgNumber << endl;

    ofile << vHypos.size() << endl;
    
    for( int i=0; i<vHypos.size(); i++ )
      ofile << vHypos[i].x << "  " << vHypos[i].y << "  " 
            << vHypos[i].nBoxX1 << "  " << vHypos[i].nBoxY1 << "  "
            << vHypos[i].nBoxWidth << "  " << vHypos[i].nBoxHeight << "  "
            << vHypos[i].dScale << "  " << vHypos[i].dScore << endl;
  }
}
