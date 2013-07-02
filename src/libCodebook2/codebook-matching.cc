/*********************************************************************/
/*                                                                   */
/* FILE         codebook-matching.cc                                 */
/* AUTHORS      Bastian Leibe, Edgar Seemann                         */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Functions for matching features to codebooks.        */
/*                                                                   */
/* BEGIN        Tue Mar 15 2005                                      */
/* LAST CHANGE  Tue Mar 15 2005                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <math.h>

#include <chamfermatching.h>

#include "codebook.hh"

/*---------------------------------------------------------*/
/*                   Matching to Codebook                  */
/*---------------------------------------------------------*/
void Codebook::matchToCodebook( const vector<FeatureVector> &vFeatures,
                                float dRejectionThresh,
                                int   nFeatureType,
                                vector<int>             &vNearestNeighbor,
                                vector<float>           &vNearestNeighborSim,
                                vector< vector<int> >   &vvAllNeighbors,
                                vector< vector<float> > &vvAllNeighborsSim ) const
{
  //cout << "  Codebook::matchToCodebook() called..." << endl;

  if( m_vClusters.empty() ) {
    cerr << "    Error in Codebook::matchToCodebook(): "
         << "No clusters computed yet!" << endl;
    return;
  }

  if( !m_parMatching.isValid() ) {
    cerr << "    Error in Codebook::matchToCodebook(): "
         << "No parameters defined yet!" << endl;
    return;
  }

  if( m_vFeatures.empty() ) {
    cerr << "    Error in Codebook::matchToCodebook(): "
         << "No features computed yet!" << endl;
    return;
  }

  /*-----------------------*/
  /* Match to the codebook */
  /*-----------------------*/
  switch( nFeatureType ) {
  case FEATURE_PATCH:
  case FEATURE_PATCHMIKO:
    /*-=-=-=-=-=-=-=-=*/
    /* Patch Features */
    /*-=-=-=-=-=-=-=-=*/
    matchToCodebookCorr( vFeatures, dRejectionThresh,
                         vNearestNeighbor, vNearestNeighborSim,
                         vvAllNeighbors, vvAllNeighborsSim );
   break;
    
  case FEATURE_PATCHMIKO2:
  case FEATURE_STEERABLE:
  case FEATURE_SIFT:
  case FEATURE_GLOH:
  case FEATURE_MOMENTS:
  case FEATURE_DIFFINV:
  case FEATURE_COMPLEX:
  case FEATURE_SHAPECONTEXT:
  case FEATURE_SPINIMGS:
  case FEATURE_GRADIENTPCA:
  case FEATURE_SURF64: 
  case FEATURE_SURF128: {
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    /* Mikolajczyk's Features */
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    float dDimFact  = (float)vFeatures.front().numDims();
    float dDistFact = m_parMatching.params()->m_dFeatureSimFact*dDimFact;
    matchToCodebookEuclid( vFeatures, dRejectionThresh, dDistFact,
                           vNearestNeighbor, vNearestNeighborSim,
                           vvAllNeighbors, vvAllNeighborsSim );
    
    break;
  }
    
  default:
    cerr << "    Error in matchToCodebook(): "
         << "Unknown feature type (" << nFeatureType << ")!" << endl;
  }  
}


void Codebook::matchToCodebook( const vector<FeatureVector> &vFeatures,
                                float dRejectionThresh,
                                int   nFeatureType,
                                MatchingInfo &miMatchResult ) const
{
  matchToCodebook( vFeatures, dRejectionThresh, nFeatureType, 
                   miMatchResult.getNN(), miMatchResult.getNNSim(), 
                   miMatchResult.getAllNeighbors(), 
                   miMatchResult.getAllNeighborsSim() );
}


MatchingInfo Codebook::matchToCodebook( const vector<FeatureVector> &vFeatures,
                                        float dRejectionThresh,
                                        int   nFeatureType ) const
{
  vector<int>            vNN;
  vector<float>          vNNSim;
  vector<vector<int> >   vvAllNeighbors;
  vector<vector<float> > vvAllNeighborsSim;
  matchToCodebook( vFeatures, dRejectionThresh, nFeatureType, 
                   vNN, vNNSim, vvAllNeighbors, vvAllNeighborsSim );
  
  return MatchingInfo( vNN, vNNSim, vvAllNeighbors, vvAllNeighborsSim );
}


void Codebook::matchToCodebookCorr( const vector<FeatureVector> &vFeatures,
                                    float dRejectionThresh,
                                    vector<int>           &vNearestNeighbor,
                                    vector<float>         &vNearestNeighborSim,
                                    vector<vector<int> >   &vvAllNeighbors,
                                    vector<vector<float> > &vvAllNeighborsSim) const
/*******************************************************************/
/* Compare all extracted feature vectors from a test image with    */
/* the codebook cluster centers using the NGC measure.             */
/*******************************************************************/
{
  /*-------------------------------*/
  /* Initialize the result vectors */
  /*-------------------------------*/
  vNearestNeighbor.clear();
  vNearestNeighborSim.clear();

  vvAllNeighbors.clear();
  vector< vector<int> > tmpVec1( vFeatures.size() );
  vvAllNeighbors = tmpVec1;

  vvAllNeighborsSim.clear();
  vector< vector<float> > tmpVec2( vFeatures.size() );
  vvAllNeighborsSim = tmpVec2;

  /*------------------------*/
  /*   simple Correlation   */
  /*------------------------*/
  //cout << "      Computing correlation similarities..." << endl;
  for( int i=0; i<(int)vFeatures.size(); i++ ) {
    float dSim    = 0.0;
    float dMaxSim = 0.0;
    int   nMaxIdx = -1;
    for( int k=0; k<(int)m_vClusters.size(); k++) {
      dSim = vFeatures[i].compCorrelation( m_vClusters[k] );
      if ( dSim > dMaxSim ) {
        dMaxSim = dSim;
        nMaxIdx = k;
      }

      if( dSim > dRejectionThresh ) {
        vvAllNeighbors[i].push_back( k );
        vvAllNeighborsSim[i].push_back( dSim );
      }
    }

    vNearestNeighbor.push_back( nMaxIdx );
    vNearestNeighborSim.push_back( dMaxSim );
  }
  //cout << "      done." << endl;
}


void Codebook::matchToCodebookEuclid( const vector<FeatureVector> &vFeatures,
                                      float dRejectionThresh,
                                      float dDistFactor,
                                      vector<int>    &vNearestNeighbor,
                                      vector<float>  &vNearestNeighborSim,
                                      vector<vector<int> >   &vvAllNeighbors,
                                      vector<vector<float> > &vvAllNeighborsSim) const
/*******************************************************************/
/* Compare all extracted feature vectors from a test image with    */
/* the codebook cluster centers using Euclidean distances.         */
/*******************************************************************/
{
  /*-------------------------------*/
  /* Initialize the result vectors */
  /*-------------------------------*/
  vNearestNeighbor.clear();
  vNearestNeighborSim.clear();

  vvAllNeighbors.clear();
  vector< vector<int> > tmpVec1( vFeatures.size() );
  vvAllNeighbors = tmpVec1;

  vvAllNeighborsSim.clear();
  vector< vector<float> > tmpVec2( vFeatures.size() );
  vvAllNeighborsSim = tmpVec2;

  /**************************/
  /*   simple Correlation   */
  /**************************/
  //cout << "      Computing Euclidean distances..." << endl;

  for( int i=0; i<(int)vFeatures.size(); i++ ) {
    float dSim    = 0.0;
    float dMaxSim = 0.0;
    int   nMaxIdx = -1;
    for( int k=0; k<(int)m_vClusters.size(); k++ ) {
      float dDist = vFeatures[i].compSSD( m_vClusters[k] );

      dSim = exp( -dDist/dDistFactor );
      if ( dSim > dMaxSim ) {
        dMaxSim = dSim;
        nMaxIdx = k;
      }

      if( dSim > dRejectionThresh ) {
        vvAllNeighbors[i].push_back( k );
        vvAllNeighborsSim[i].push_back( dSim );
      }
    }

    vNearestNeighbor.push_back( nMaxIdx );
    vNearestNeighborSim.push_back( dMaxSim );
  }
  //cout << "      done." << endl;
}


void Codebook::matchToCodebookChamfer( const vector<FeatureVector> &vFeatures,
                                       float dRejectionThresh,
                                       vector<int>   &vNearestNeighbor,
                                       vector<float> &vNearestNeighborSim,
                                       vector<vector<int> >   &vvAllNeighbors,
                                       vector<vector<float> > &vvAllNeighborsSim, 
                                       bool bSymDist ) const
/*******************************************************************/
/* Compare all extracted feature vectors from a test image with    */
/* the codebook cluster centers using the Chamfer distance         */
/*******************************************************************/
{
  /*-------------------------------*/
  /* Initialize the result vectors */
  /*-------------------------------*/
  vNearestNeighbor.clear();
  vNearestNeighborSim.clear();

  vvAllNeighbors.clear();
  vector< vector<int> > tmpVec1( vFeatures.size() );
  vvAllNeighbors = tmpVec1;

  vvAllNeighborsSim.clear();
  vector< vector<float> > tmpVec2( vFeatures.size() );
  vvAllNeighborsSim = tmpVec2;

  /*------------------------*/
  /*   Chamfer distance     */
  /*------------------------*/
  //cout << "      Computing Chamfer distances..." << endl;

  //-- store codebook in ChamferMatching instance --//

  ChamferMatching chamfer;
  vector<OpGrayImage> vCodebook;

  int nDims   = m_vFeatures[m_vPrototypes[0]].numDims();
  int imgsize = (int) ceilf(sqrt((float) nDims));

  for( int i=0; i<(int)m_vPrototypes.size(); i++) {
    OpGrayImage tmp;
    assert( m_vFeatures[m_vPrototypes[i]].numDims()==nDims );
    tmp.loadFromData( imgsize, imgsize, 
                      m_vFeatures[m_vPrototypes[i]].getData());
    vCodebook.push_back(tmp);
  }
  chamfer.setCodebook(vCodebook);

  //-- calculate best matches --//
  for( int i=0; i<(int)vFeatures.size(); i++ ) {
    float dSim    = 0.0;
    float dMaxSim = 0.0;
    int   nMaxIdx = -1;

    assert( vFeatures[i].numDims()==nDims );
    OpGrayImage tmp;
    tmp.loadFromData( imgsize,imgsize, vFeatures[i].getData());
    vector<float> vSims = chamfer.getDistancesToCodebook(tmp, false, bSymDist);

    for( int k=0; k<(int)vSims.size(); k++ ) {
      dSim = vSims[k];
      if ( dSim > dMaxSim ) {
        dMaxSim = dSim;
        nMaxIdx = k;
      }

      if( dSim > dRejectionThresh ) {
        vvAllNeighbors[i].push_back( k );
        vvAllNeighborsSim[i].push_back( dSim );
      }
    }

    vNearestNeighbor.push_back( nMaxIdx );
    vNearestNeighborSim.push_back( dMaxSim );
  }
}
 

void Codebook::matchToCodebookChamferOri( const vector<FeatureVector> &vFeatures,
                                          float dRejectionThresh,
                                          vector<int>   &vNearestNeighbor,
                                          vector<float> &vNearestNeighborSim,
                                          vector<vector<int> >   &vvAllNeighbors,
                                          vector<vector<float> > &vvAllNeighborsSim, 
                                          bool bSymDist ) const
/*******************************************************************/
/* Compare all extracted feature vectors from a test image with    */
/* the codebook cluster centers using the Chamfer distance with    */
/* multiple orientation planes.                                    */
/*******************************************************************/
{
  /*-------------------------------*/
  /* Initialize the result vectors */
  /*-------------------------------*/
  vNearestNeighbor.clear();
  vNearestNeighborSim.clear();

  vvAllNeighbors.clear();
  vector< vector<int> > tmpVec1( vFeatures.size() );
  vvAllNeighbors = tmpVec1;

  vvAllNeighborsSim.clear();
  vector< vector<float> > tmpVec2( vFeatures.size() );
  vvAllNeighborsSim = tmpVec2;

  /*------------------------*/
  /*   Chamfer distance     */
  /*------------------------*/
  //cout << "      Computing Chamfer distances with orientation planes..."
  //     << endl;

  //-- store codebook in ChamferMatching instance --//

  ChamferMatching     chamfer;
  vector<OpGrayImage> vCodebook;

  assert(m_vPrototypes.size()>0);
  int nDims   = m_vFeatures[m_vPrototypes[0]].numDims();
  //printf("number of dimensions in prototype: %d\n", nDims);
  int imgsize = (int) ceilf(sqrt((float) nDims));

  for( int i=0; i<(int)m_vPrototypes.size(); i++ ) {
    OpGrayImage tmp;
    assert( m_vFeatures[m_vPrototypes[i]].numDims()==nDims );
    tmp.loadFromData( imgsize, imgsize, 
                      m_vFeatures[m_vPrototypes[i]].getData());
    vCodebook.push_back(tmp);
  }
  chamfer.setCodebook( vCodebook, true );

  //-- calculate best matches --//
  for( int i=0; i<(int)vFeatures.size(); i++ ) {
    float dSim    = 0.0;
    float dMaxSim = 0.0;
    int   nMaxIdx = -1;

    assert( vFeatures[i].numDims()==nDims );
    OpGrayImage tmp;
    tmp.loadFromData( imgsize, imgsize, vFeatures[i].getData());
    vector<float> vSims = chamfer.getDistancesToCodebook(tmp, true, bSymDist);

    for( int k=0; k<(int)vSims.size(); k++ ) {
      dSim = vSims[k];
      if ( dSim > dMaxSim ) {
        dMaxSim = dSim;
        nMaxIdx = k;
      }

      if( dSim > dRejectionThresh ) {
        vvAllNeighbors[i].push_back( k );
        vvAllNeighborsSim[i].push_back( dSim );
      }
    }

    vNearestNeighbor.push_back( nMaxIdx );
    vNearestNeighborSim.push_back( dMaxSim );
  }
}


