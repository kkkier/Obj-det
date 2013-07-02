/*********************************************************************/
/*                                                                   */
/* FILE         mcmatcher-experiments.cc                             */
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

#include "mcmatcher.hh"


void ISMReco::resampleHypothesis( Hypothesis &hypo, Segmentation &segResult )
  // resample the interest points for the given hypothesis
{
  int   nImgWidth     = m_grayImg.width();
  int   nImgHeight    = m_grayImg.height();

  unsigned   nDetId = hypo.nCategory;
  Detector   &det   = m_vDetectors[nDetId];

  if( m_vParReco[nDetId].params()->m_bResampleHypos ) {
    
    /*-----------------------------*/
    /* Process each cue separately */
    /*-----------------------------*/
    vector<Segmentation> vCueSegs;
    float dScore = 0.0;
    for(unsigned nIdx=0; nIdx<det.m_nNumCues; nIdx++ ) {
      cout << "    Processing cue " << nIdx << "..." << endl;
      
      /*-----------------------------------------------*/
      /* Invert the occurrence file to get new samples */
      /*-----------------------------------------------*/
      cout << "      Inverting the occurrence file..." << endl;
      PointVector       vNewSamples;
      vector<unsigned>  vCandClusters;
      vector<unsigned>  vCandOccs;
      VecVecOccurrence  vvOcc      = det.m_vISMReco[nIdx].getOccurrences();
      vector<float>     vOccSumWts = det.m_vISMReco[nIdx].getOccWeights(); 
      for(unsigned j=0; j<vvOcc.size(); j++ )
        for(unsigned k=0; k<vvOcc[j].size(); k++ ) {
          InterestPoint pt;
          pt.x     = (int)round(hypo.x + vvOcc[j][k].dPosX*hypo.dScale);
          pt.y     = (int)round(hypo.y + vvOcc[j][k].dPosY*hypo.dScale);
          pt.scale = hypo.dScale * vvOcc[j][k].dScale;
          pt.value = 1.0;
          pt.angle = 0.0;
          pt.l1    = pt.scale;
          pt.l2    = pt.scale;
          
          // check if the point lies inside the image
          float dScaleFactor = det.m_vCues[nIdx].params()->m_dScaleFactor;
          int nPatchSize = (int)round( pt.scale*dScaleFactor );
          if( (pt.x - nPatchSize >= 0) &&
              (pt.y - nPatchSize >= 0) &&
              (pt.x + nPatchSize < m_grayImg.width()) &&
              (pt.y + nPatchSize < m_grayImg.height()) ) { 
            vNewSamples.push_back( pt );
            vCandClusters.push_back( j );
            vCandOccs.push_back( k );
          }
        }
//         cout << "        created " << vNewSamples.size() << " samples." 
//              << endl;
//         cout << "                " << vCandClusters.size() << " candidates,"
//              << endl
//              << "            and " << vCandOccs.size() << " occ entries." 
//              << endl;

      /*-------------------------*/
      /* Sample the new features */
      /*-------------------------*/
      cout << "      Resampling " << vNewSamples.size() 
           << " regions... " << flush;
      vector<FeatureVector> vNewFeatures;
      vector<int>           vSampleIdzs;
//         for(unsigned j=0; j<vNewSamples.size(); j++ )
//           if( j%10000==0 )
//             cout << "  Region  " << j << ": (" << vNewSamples[j].x 
//                  << "," << vNewSamples[j].y << "," << vNewSamples[j].scale
//                  << ")" << endl;
      det.m_vCues[nIdx].getDescriptors( m_grayImg, m_grayImgMap, 
                                        vNewSamples, 
                                        vNewSamples, vSampleIdzs,
                                        vNewFeatures, false );
      cout << "sampled " << vNewFeatures.size() << " features." 
           << endl;
//         for(unsigned j=0; j<vNewFeatures.size(); j++ )
//           if( j%10000==0 )
//             cout << "  Region  " << j << ": (" << vNewSamples[j].x 
//                  << "," << vNewSamples[j].y << "," << vNewSamples[j].scale
//                  << ")" << endl
//                  << "  Feature " << j << ": " << vNewFeatures[j] << endl;

//         cout << "        (with " << vSampleIdzs.size() << " index entries.)"
//              << endl;

      /*------------------------------------*/
      /* Match them to the codebook entries */
      /*------------------------------------*/
      cout << "      Matching to the codebook... " << flush;
      list<HoughVote> vNewVotes;
      if( vNewFeatures.size()>0 ) {
        vector<FeatureVector> vClusters = 
          (*det.m_vCodebooks[nIdx]).getClusters();
        float dDimFact  = (float)vNewFeatures.front().numDims();
        float dDistFact = 
          det.m_vParMatching[nIdx].params()->m_dFeatureSimFact*dDimFact;
        float dRejThresh = 
          det.m_vParMatching[nIdx].params()->m_dRejectionThresh;
        float dGibbsConst = -1.0 / (det.m_parReco.params()->m_dGibbsConst*
                                    log(dRejThresh) );
//           cout << "        Codebook size: " << vClusters.size() << endl;
//           cout << "        dDimFact   : " << dDimFact << endl
//                << "        dDistFact  : " << dDistFact << endl
//                << "        dRejThresh : " << dRejThresh << endl
//                << "        dGibbsConst: " << dGibbsConst << endl;
          //cout << "        Matching... " << flush;
        for(unsigned j=0; j<vNewFeatures.size(); j++ ) {
          unsigned nMatchIdx  = vCandClusters[ vSampleIdzs[j] ];
          unsigned nOccIdx    = vCandOccs[ vSampleIdzs[j] ];
          unsigned nOccMapIdx = vvOcc[nMatchIdx][nOccIdx].nOccMapIdx;
//             if( j%1000==0 )
//               cout << "        Matching feature " << j 
//                    << " to codebook entry #" << nMatchIdx << "... " << flush;

          float dSim;
          if( det.m_vCues[nIdx].params()->m_nFeatureType==FEATURE_PATCH )
            dSim = vNewFeatures[j].compCorrelation( vClusters[nMatchIdx] );
          else
            dSim = exp( -vNewFeatures[j].compSSD( vClusters[nMatchIdx] ) /
                        dDistFact );

//             if( j%1000==0 )
//               cout << " sim=" << dSim << endl;
            
//             if( j%10000==0 )
//               cout << "  Feature " << j << ": " << vNewFeatures[j] << endl
//                    << "  Cluster " << nMatchIdx << ": " 
//                    << vClusters[nMatchIdx] << endl;

          // check if the feature matches
          if( dSim >= dRejThresh ) {
            // compute Gibbs-like match weights
            float dMatchWt   = pow( dSim, dGibbsConst );
            float dOccWt     = vvOcc[nMatchIdx][nOccIdx].dWeight;
            float dOccWtNorm = 1.0 / vOccSumWts[nMatchIdx];
            
            // create a (resampled) vote for the current hypothesis
            float dWeight = dMatchWt*dOccWt*dOccWtNorm;
            
            // if vote weight is too large, clip it
            if( dWeight > det.m_parReco.params()->m_dMaxVoteWeight )
              dWeight = det.m_parReco.params()->m_dMaxVoteWeight;
            
            HoughVote vote( (float)hypo.x, (float)hypo.y, (float)hypo.dScale,
                            (float)dWeight, 1.0f, (int)j, nMatchIdx, nOccIdx,
                            nOccMapIdx, nIdx );
            vNewVotes.push_back( vote );
            dScore += dWeight;
          } // end if(matches to codebook)
        } // end forall resampled features
      } // end if(resampled features non-empty)
      cout << " found " << vNewVotes.size() << " matches." << endl;
      
      /*---------------------------------*/
      /* Compute a top-down segmentation */
      /*---------------------------------*/
      cout << "      Computing a top-down segmentation..." << endl;
      int nObjWidth   = det.m_parReco.params()->m_nObjWidth;
      int nObjHeight  = det.m_parReco.params()->m_nObjHeight;        
      int nHypoWidth  = (int)floor(nObjWidth*hypo.dScale*1.75 + 0.5);
      int nHypoHeight = (int)floor(nObjHeight*hypo.dScale*1.75+ 0.5);
      int nSegOffX    = max( 0, hypo.x - nHypoWidth/2 );
      int nSegOffY    = max( 0, hypo.y - nHypoHeight/2 );
      int nSegWidth   = min( nHypoWidth, nImgWidth-nSegOffX );
      int nSegHeight  = min( nHypoHeight, nImgHeight-nSegOffY );
      
      Segmentation segNew = 
        det.m_vISMReco[nIdx].drawSegmentationOffset( vNewVotes, 
                                                     vNewSamples, 
                                                     det.m_vCues[nIdx],
                                                     nSegOffX, nSegOffY, 
                                                     1.0, 
                                                     nSegWidth, nSegHeight,
                                                     true );
      vCueSegs.push_back( segNew );
    } // end forall cues
    
    /*----------------------------*/
    /* Combine the different cues */
    /*----------------------------*/
    cout << "    Combining the cue contributions..." << endl;
    Segmentation segCombined;
    switch( det.m_nCombiMethod ) {
    case CUE_COMBI_SUM: segCombined = add( vCueSegs ); break;
    case CUE_COMBI_AVG: segCombined = avg( vCueSegs ); break;
    case CUE_COMBI_MAX: segCombined = max( vCueSegs ); break;
      
    default:
      cerr << "Error in ISMReco::processTestImgBothDir(): "
           << "Invalid cue combination method (" << det.m_nCombiMethod 
           << ")!" << endl;
    }

    float dSumPFig  = segCombined.getSumPFig();
    float dFigArea  = segCombined.getSumSegArea();
    float dScoreMDL = det.m_ismMultiCue.getMDLScore( dSumPFig, dFigArea, 
                                                     hypo.dScale );
    
    /*-------------------------------------------------*/
    /* Update the hypothesis with the new segmentation */
    /*-------------------------------------------------*/
    cout << "    Updating the hypothesis..." << endl;
    hypo.dScore       = dScore;
    hypo.dScoreMDL    = dScoreMDL;
    segResult         = segCombined;

  } // end if(resample hypothesis)
}
