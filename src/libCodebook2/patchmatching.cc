/*********************************************************************/
/*                                                                   */
/* FILE         patchmatching.cc                                     */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Implements functions for matching image patches      */
/*              using normalized grayscale correlation.              */
/*                                                                   */
/*                                                                   */
/* BEGIN        Tue Mar 18 2003                                      */
/* LAST CHANGE  Tue Mar 18 2003                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <math.h>
#include <algorithm>

#include <pca.hh>

#include "patchmatching.hh"
#include <chamfermatching.h>
#include <eimagebrowser.h>

/********************************/
/*   Function Implementations   */
/********************************/

void compareCodebookNGC( vector<FeatureVector> vSrcImgFeatures,
                         vector<FeatureVector> vClusters,
                         float dRejectionThresh,
                         vector<int>             &vNearestNeighbor,
                         vector<float>           &vNearestNeighborSim,
                         vector< vector<int> >   &vvAllNeighbors,
                         vector< vector<float> > &vvAllNeighborsSim )
/*******************************************************************/
/* Compare all extracted feature vectors from the test image with  */
/* the codebook cluster centers using the NGC measure.             */
/*******************************************************************/
{
  vNearestNeighbor.clear();
  vNearestNeighborSim.clear();

  vvAllNeighbors.clear();
  vector< vector<int> > tmpVec1( vSrcImgFeatures.size() );
  vvAllNeighbors = tmpVec1;

  vvAllNeighborsSim.clear();
  vector< vector<float> > tmpVec2( vSrcImgFeatures.size() );
  vvAllNeighborsSim = tmpVec2;

  /***************************/
  /*   NGC similarity fast   */
  /***************************/
  cout << " computing NGC similarities (directly with cluster centers)..."
  << endl;

  for(unsigned i=0; i < vSrcImgFeatures.size(); i++)
  {
    double currentminsim = 0.0;
    double cursim        = 0.0;
    int clNo = -1;
    for(unsigned k=0; k < vClusters.size(); k++)
    {
      cursim = computeNGC( vSrcImgFeatures[i], vClusters[k] );
      if ( cursim > currentminsim )
      {
        currentminsim = cursim;
        clNo = k;
      }

      if( cursim > dRejectionThresh )
      {
        vvAllNeighbors[i].push_back( k );
        vvAllNeighborsSim[i].push_back( cursim );
      }
    }

    vNearestNeighbor.push_back( clNo );
    vNearestNeighborSim.push_back( currentminsim );
  }
}


void compareCodebookNGCEdges( vector<FeatureVector> vSrcImgFeatures,
                              vector<FeatureVector> vFeatureMaps,
                              vector<FeatureVector> vGradMagMaps,
                              vector<FeatureVector> vClusters,
                              vector<FeatureVector> vAveragedMaps,
                              vector<float>         vAveragedMapsSum,
                              vector<FeatureVector> vTrainGradMagMaps,
                              float dRejectionThresh,
                              vector<int>             &vNearestNeighbor,
                              vector<float>           &vNearestNeighborSim,
                              vector< vector<int> >   &vvAllNeighbors,
                              vector< vector<float> > &vvAllNeighborsSim )
/*******************************************************************/
/* Compare all extracted feature vectors from the test image with  */
/* the codebook cluster centers using the NGC measure.             */
/*******************************************************************/
{
  if ( vFeatureMaps.empty() )
  {
    cerr << "Please ensure having loaded codebooks with segmentation maps!"
    << endl;
    return;
  }

  vNearestNeighbor.clear();
  vNearestNeighborSim.clear();

  vvAllNeighbors.clear();
  vector< vector<int> > tmpVec1( vSrcImgFeatures.size() );
  vvAllNeighbors = tmpVec1;

  vvAllNeighborsSim.clear();
  vector< vector<float> > tmpVec2( vSrcImgFeatures.size() );
  vvAllNeighborsSim = tmpVec2;

  /******************/
  /*   NGC + Edge   */
  /******************/
  cout << "  Computing NGC similarities with edge maps... " << endl;

  cout << "    Size of averaged maps is: " << vAveragedMaps.size() << endl;
  cout << "    Size of vClusters is    : " << vClusters.size() << endl;

  for(unsigned i=0; i < vSrcImgFeatures.size(); i++)
  {
    double currentminsim = 0.0;
    double cursim        = 0.0;
    int    clNo          =  -1;
    for(unsigned k=0; k < vClusters.size(); k++)
    {
      cursim = computeFigureNGC( vSrcImgFeatures[i], vClusters[k],
                                 vAveragedMaps[k], vGradMagMaps[i],
                                 vTrainGradMagMaps[k],
                                 vAveragedMapsSum[k] );
      if ( cursim > currentminsim )
      {
        currentminsim = cursim;
        clNo = k;
      }

      if( cursim > dRejectionThresh )
      {
        vvAllNeighbors[i].push_back( k );
        vvAllNeighborsSim[i].push_back( cursim );
      }
    }

    vNearestNeighbor.push_back( clNo );
    vNearestNeighborSim.push_back( currentminsim );
  }
}


void compareCodebookCorrelation( const vector<FeatureVector> &vSrcImgFeatures,
                                 /*const*/ vector<FeatureVector> &vClusters,
                                 float dRejectionThresh,
                                 int   nNormalizeMethod,
                                 vector<int>             &vNearestNeighbor,
                                 vector<float>           &vNearestNeighborSim,
                                 vector< vector<int> >   &vvAllNeighbors,
                                 vector< vector<float> > &vvAllNeighborsSim )
/*******************************************************************/
/* Compare all extracted feature vectors from the test image with  */
/* the codebook cluster centers using the NGC measure.             */
/*******************************************************************/
{
  vNearestNeighbor.clear();
  vNearestNeighborSim.clear();

  vvAllNeighbors.clear();
  vector< vector<int> > tmpVec1( vSrcImgFeatures.size() );
  vvAllNeighbors = tmpVec1;

  vvAllNeighborsSim.clear();
  vector< vector<float> > tmpVec2( vSrcImgFeatures.size() );
  vvAllNeighborsSim = tmpVec2;

  /**************************/
  /*   simple Correlation   */
  /**************************/
  cout << "      computing correlation similarities..."
  << endl;

  vector<FeatureVector> vFvMatch = vSrcImgFeatures;

  switch( nNormalizeMethod )
  {
  case NORM_NONE:
    break;

  case NORM_STDDEV:
    for(unsigned i=0; i<vFvMatch.size(); i++ )
      vFvMatch[i].normalizeZeroMeanUnitStdDev2();
    break;

  case NORM_VARIANCE:
    for(unsigned i=0; i<vFvMatch.size(); i++ )
      vFvMatch[i].normalizeZeroMeanUnitVar();
    break;

  case NORM_ENERGY:
    for(unsigned i=0; i<vFvMatch.size(); i++ )
    {
      vFvMatch[i].normalizeEnergy2();
    }
    break;

  default:
    cerr << "  ERROR: Unknown normalization method (" << nNormalizeMethod
    << ")!" << endl;
    return;
  }

  for(unsigned i=0; i < vFvMatch.size(); i++)
  {
    double currentminsim = 0.0;
    double cursim        = 0.0;
    int clNo = -1;
    for(unsigned k=0; k < vClusters.size(); k++)
    {
      cursim = vFvMatch[i].compCorrelation( vClusters[k] );
      if ( cursim > currentminsim )
      {
        currentminsim = cursim;
        clNo = k;
      }

      if( cursim > dRejectionThresh )
      {
        vvAllNeighbors[i].push_back( k );
        vvAllNeighborsSim[i].push_back( cursim );
      }
    }

    vNearestNeighbor.push_back( clNo );
    vNearestNeighborSim.push_back( currentminsim );
  }
  cout << "      done." << endl;
}


void compareCodebookEuklidean( const vector<FeatureVector> &vSrcImgFeatures,
                               /*const*/ vector<FeatureVector> &vClusters,
                               float dRejectionThresh,
                               float dDistFactor,
                               vector<int>             &vNearestNeighbor,
                               vector<float>           &vNearestNeighborSim,
                               vector< vector<int> >   &vvAllNeighbors,
                               vector< vector<float> > &vvAllNeighborsSim )
/*******************************************************************/
/* Compare all extracted feature vectors from the test image with  */
/* the codebook cluster centers using a Euklidean distance.        */
/*******************************************************************/
{
  vNearestNeighbor.clear();
  vNearestNeighborSim.clear();

  vvAllNeighbors.clear();
  vector< vector<int> > tmpVec1( vSrcImgFeatures.size() );
  vvAllNeighbors = tmpVec1;

  vvAllNeighborsSim.clear();
  vector< vector<float> > tmpVec2( vSrcImgFeatures.size() );
  vvAllNeighborsSim = tmpVec2;

  /**************************/
  /*   simple Correlation   */
  /**************************/
  cout << "      computing correlation similarities..."
  << endl;

  vector<FeatureVector> vFvMatch = vSrcImgFeatures;

  for(unsigned i=0; i < vFvMatch.size(); i++)
  {
    double currentminsim = 0.0;
    double cursim        = 0.0;
    int clNo = -1;
    for(unsigned k=0; k < vClusters.size(); k++)
    {
      float dist = vFvMatch[i].compSSD( vClusters[k] );

      cursim = exp( -dist/dDistFactor );
      if ( cursim > currentminsim )
      {
        currentminsim = cursim;
        clNo = k;
      }

      if( cursim > dRejectionThresh )
      {
        vvAllNeighbors[i].push_back( k );
        vvAllNeighborsSim[i].push_back( cursim );
      }
    }

    vNearestNeighbor.push_back( clNo );
    vNearestNeighborSim.push_back( currentminsim );
  }
  cout << "      done." << endl;
}


void compareCodebookES( vector<FeatureVector> vSrcImgFeatures,
                        vector<FeatureVector> vClusters,
                        float dRejectionThresh,
                        vector<FeatureVector> vEigenVectors,
                        FeatureVector fvEigenValues,
                        FeatureVector fvAvgImage,
                        float dEVEnergy,
                        PointVector vPointsInside,
                        int nWidth, int nHeight,
                        int nStepSize,
                        vector<int>             &vNearestNeighbor,
                        vector<float>           &vNearestNeighborSim,
                        vector< vector<int> >   &vvAllNeighbors,
                        vector< vector<float> > &vvAllNeighborsSim,
                        OpGrayImage &imgDffsError, OpGrayImage &imgDifsError )
/*******************************************************************/
/* Compare all extracted feature vectors from the test image with  */
/* the codebook cluster centers using Euklidean distance in a      */
/* truncated eigenspace.                                           */
/*******************************************************************/
{
  vNearestNeighbor.clear();
  vNearestNeighborSim.clear();

  vvAllNeighbors.clear();
  vector< vector<int> > tmpVec1( vSrcImgFeatures.size() );
  vvAllNeighbors = tmpVec1;

  vvAllNeighborsSim.clear();
  vector< vector<float> > tmpVec2( vSrcImgFeatures.size() );
  vvAllNeighborsSim = tmpVec2;

  /******************************/
  /*   Matching in Eigenspace   */
  /******************************/
  cout << "  Computing distances in Eigenspace..."
  << endl;

  vector<FeatureVector> vComponents;
  getComponentsEnergy( vEigenVectors, fvEigenValues, dEVEnergy,
                       vComponents );
  cout << "    EV energy of " << dEVEnergy << " corresponds to "
  << vComponents.size() << " components." << endl;

  cout << "    Projecting the clusters into the eigenspace..." << endl;
  vector<FeatureVector> vProjClusters;
  cout << "      (using all clusters)" << endl;
  for(unsigned i=0; i<vClusters.size(); i++ )
  {
    FeatureVector fvProjected;
    applyEigenVectors( vComponents, fvAvgImage, vClusters[i],
                       fvProjected );
    vProjClusters.push_back( fvProjected );
  }

  cout << "    Projecting the patches into the eigenspace..." << endl;
  vector<FeatureVector> vProjFeatures;
  OpGrayImage imgTmp1( nWidth/nStepSize, nHeight/nStepSize );
  imgDffsError = imgTmp1;
  for(unsigned i=0; i<vSrcImgFeatures.size(); i++ )
  {
    FeatureVector fvProjected;
    applyEigenVectors( vComponents, fvAvgImage, vSrcImgFeatures[i],
                       fvProjected );
    vProjFeatures.push_back( fvProjected );

    /* calculate the reco error */
    imgDffsError( vPointsInside[i].x/nStepSize,
                  vPointsInside[i].y/nStepSize ) =
                    calcRecoError( vComponents, fvAvgImage, vSrcImgFeatures[i],
                                   fvProjected );
  }

  cout << "    Matching in the eigenspace..." << endl;
  OpGrayImage imgTmp2( nWidth/nStepSize, nHeight/nStepSize );
  imgDifsError = imgTmp2;
  for(unsigned i=0; i < vProjFeatures.size(); i++)
  {
    double currentminsim = 999999999999999999.0;
    double cursim        = 0.0;
    int clNo = -1;
    for(unsigned k=0; k < vProjClusters.size(); k++)
    {
      //cursim = computeNGC( m_vSrcImgFeatures[i], m_vClusters[k] );
      //cursim = -vProjFeatures[i].compSSD( vProjClusters[k] );
      cursim = 0.0;
      for( int m=0; m<vProjFeatures[i].numDims(); m++ )
        cursim += vProjFeatures[i].at(m) * vProjClusters[k].at(m);

      if ( cursim > currentminsim )
      {
        currentminsim = cursim;
        clNo = k;
      }

      if( cursim > dRejectionThresh )
      {
        vvAllNeighbors[i].push_back( k );
        vvAllNeighborsSim[i].push_back( cursim );
      }

    }

    vNearestNeighbor.push_back( clNo );
    vNearestNeighborSim.push_back( currentminsim );

    /* visualize the difs error */
    imgDifsError( vPointsInside[i].x/nStepSize,
                  vPointsInside[i].y/nStepSize) = currentminsim;
  }
  cout << "  done." << endl;
}



void compareCodebookChamfer( const vector<FeatureVector> &vSrcImgFeatures,
                             const vector<FeatureVector> &vFeatures,
                             const vector<int> &vPrototypes,
                             float dRejectionThresh,
                             int   nNormalizeMethod,
                             vector<int>             &vNearestNeighbor,
                             vector<float>           &vNearestNeighborSim,
                             vector< vector<int> >   &vvAllNeighbors,
                             vector< vector<float> > &vvAllNeighborsSim,
                             bool symDist)
/*******************************************************************/
/* Compare all extracted feature vectors from the test image with  */
/* the codebook cluster centers using the chamfer distance         */
/*******************************************************************/
{

  //EImageBrowser* b = new EImageBrowser();
  //b->show();


  vNearestNeighbor.clear();
  vNearestNeighborSim.clear();

  vvAllNeighbors.clear();
  vector< vector<int> > tmpVec1( vSrcImgFeatures.size() );
  vvAllNeighbors = tmpVec1;

  vvAllNeighborsSim.clear();
  vector< vector<float> > tmpVec2( vSrcImgFeatures.size() );
  vvAllNeighborsSim = tmpVec2;

  /**************************/
  /*   Chamfer distance     */
  /**************************/
  cout << "      computing chamfer distances..."
  << endl;

  //vector<FeatureVector>& vFvMatch = vSrcImgFeatures;

  //-- store codebook in ChamferMatching instance --//

  ChamferMatching chamfer;
  vector<OpGrayImage> codebook;

  int s = vFeatures[vPrototypes[0]].numDims();
  int ss = (int) ceilf(sqrt((float) s));

  for(unsigned i=0; i<vPrototypes.size(); i++)
  {
    OpGrayImage tmp;
    assert(vFeatures[vPrototypes[i]].numDims()==s);
    tmp.loadFromData(ss, ss, vFeatures[vPrototypes[i]].getData());
    codebook.push_back(tmp);
    //b->pushImage(tmp.getQtImage(), "");
  }
  chamfer.setCodebook(codebook);

  //-- calculate best matches --//
  for(unsigned i=0; i<vSrcImgFeatures.size(); i++)
  {
    float currentminsim = 0.0;
    float cursim        = 0.0;
    int clNo = -1;

    OpGrayImage tmp;
    assert(vSrcImgFeatures[i].numDims()==s);
    tmp.loadFromData(ss,ss, vSrcImgFeatures[i].getData());
    vector<float> sims = chamfer.getDistancesToCodebook(tmp, false, symDist);

    for(unsigned k=0; k<sims.size(); k++)
    {
      //cursim = 1.0 - min(1.0f, sims[k]/100.0f);
      cursim =sims[k];
      if ( cursim > currentminsim )
      {
        currentminsim = cursim;
        clNo = k;
      }

      if( cursim > dRejectionThresh )
      {
        vvAllNeighbors[i].push_back( k );
        vvAllNeighborsSim[i].push_back( cursim );
      }

    }
    vNearestNeighbor.push_back( clNo );
    vNearestNeighborSim.push_back( currentminsim );

  }

  //-- calculate best matches --//
  //   for(unsigned i=0; i < vFvMatch.size(); i++) {
  //     float currentminsim = 0.0;
  //     float cursim        = 0.0;
  //     int clNo = -1;
  //     for(unsigned  k=0; k < vPrototypes.size(); k++) {
  //
  // 	  //-- DEBUG (2 lines --//
  // 	  if (vFeatures.size()<=vPrototypes[k]) printf("size: %d req: %d\n",vFeatures.size(),vPrototypes[k]);
  // 	  assert(vFeatures.size()>vPrototypes[k]);
  //
  //       OpGrayImage i1, i2;
  // 	  int s1 = (int) ceilf(sqrt((float) vFvMatch[i].numDims()));
  // 	  int s2 = (int) ceilf(sqrt((float) vFeatures[vPrototypes[k]].numDims()));
  //       i1.loadFromData(s1, s1,vFvMatch[i].getData());
  //       i2.loadFromData(s2, s2, vFeatures[vPrototypes[k]].getData());
  //       cursim = ChamferMatching::getDistance(i1, i2 );
  //
  // 	  //char tmp[32];sprintf(tmp, "%d-%d: %.4f", i, k, cursim);
  // 	  //b->pushImage(i1.getQtImage(),tmp);
  //       //b->pushImage(i2.getQtImage(),tmp);
  //       //printf("%d. cluster: %f ", k, cursim);
  //       cursim = 1.0 - min(1.0f, cursim/100.0f);
  //       //printf("-> norm: %f\n", cursim);
  //       if ( cursim > currentminsim ) {
  //         currentminsim = cursim;
  //         clNo = k;
  //       }
  //
  //       if( cursim > dRejectionThresh ) {
  //         vvAllNeighbors[i].push_back( k );
  //         vvAllNeighborsSim[i].push_back( cursim );
  //       }
  //     }
  //
  //     vNearestNeighbor.push_back( clNo );
  //     vNearestNeighborSim.push_back( currentminsim );
  //   }
  //   cout << "      done." << endl;
}

void compareCodebookChamferOrientation( const vector<FeatureVector> &vSrcImgFeatures,
                                        const vector<FeatureVector> &vFeatures,
                                        const vector<int> &vPrototypes,
                                        float dRejectionThresh,
                                        int   nNormalizeMethod,
                                        vector<int>             &vNearestNeighbor,
                                        vector<float>           &vNearestNeighborSim,
                                        vector< vector<int> >   &vvAllNeighbors,
                                        vector< vector<float> > &vvAllNeighborsSim,
                                        bool symDist )
/*******************************************************************/
/* Compare all extracted feature vectors from the test image with  */
/* the codebook cluster centers using the chamfer distance         */
/*******************************************************************/
{

  //EImageBrowser* b = new EImageBrowser();
  //b->show();

  vNearestNeighbor.clear();
  vNearestNeighborSim.clear();

  vvAllNeighbors.clear();
  vector< vector<int> > tmpVec1( vSrcImgFeatures.size() );
  vvAllNeighbors = tmpVec1;

  vvAllNeighborsSim.clear();
  vector< vector<float> > tmpVec2( vSrcImgFeatures.size() );
  vvAllNeighborsSim = tmpVec2;

  /**************************/
  /*   Chamfer distance     */
  /**************************/
  cout << "      computing chamfer distances..."
  << endl;

  //vector<FeatureVector>& vFvMatch = vSrcImgFeatures;

  //-- store codebook in ChamferMatching instance --//

  ChamferMatching chamfer;
  vector<OpGrayImage> codebook;

  assert(vPrototypes.size()>0);
  int s = vFeatures[vPrototypes[0]].numDims();
  printf("number of dimensions in prototype: %d\n", s);
  int ss = (int) ceilf(sqrt((float) s));

  for(unsigned i=0; i<vPrototypes.size(); i++)
  {
    OpGrayImage tmp;
    assert(vFeatures[vPrototypes[i]].numDims()==s);
    tmp.loadFromData(ss, ss, vFeatures[vPrototypes[i]].getData());
    codebook.push_back(tmp);
    //b->pushImage(tmp.getQtImage(), "");
  }
  chamfer.setCodebook(codebook, true);

  //-- calculate best matches --//
  for(unsigned i=0; i<vSrcImgFeatures.size(); i++)
  {
    float currentminsim = 0.0;
    float cursim        = 0.0;
    int clNo = -1;

    OpGrayImage tmp;
    assert(vSrcImgFeatures[i].numDims()==s);
    tmp.loadFromData(ss,ss, vSrcImgFeatures[i].getData());
    vector<float> sims = chamfer.getDistancesToCodebook(tmp, true, symDist);
    //printf(" simssize: %d\n", sims.size());

    for(unsigned k=0; k<sims.size(); k++)
    {
      //cursim = 1.0 - min(1.0f, sims[k]/100.0f);
      cursim = sims[k];
      if ( cursim > currentminsim )
      {
        currentminsim = cursim;
        clNo = k;
      }

      if( cursim > dRejectionThresh )
      {
        vvAllNeighbors[i].push_back( k );
        vvAllNeighborsSim[i].push_back( cursim );
      }

    }
    vNearestNeighbor.push_back( clNo );
    vNearestNeighborSim.push_back( currentminsim );

  }

  //-- calculate best matches --//
  //   for(unsigned i=0; i < vFvMatch.size(); i++) {
  //     float currentminsim = 0.0;
  //     float cursim        = 0.0;
  //     int clNo = -1;
  //     for(unsigned  k=0; k < vPrototypes.size(); k++) {
  //
  // 	  //-- DEBUG (2 lines --//
  // 	  if (vFeatures.size()<=vPrototypes[k]) printf("size: %d req: %d\n",vFeatures.size(),vPrototypes[k]);
  // 	  assert(vFeatures.size()>vPrototypes[k]);
  //
  //       OpGrayImage i1, i2;
  // 	  int s1 = (int) ceilf(sqrt((float) vFvMatch[i].numDims()));
  // 	  int s2 = (int) ceilf(sqrt((float) vFeatures[vPrototypes[k]].numDims()));
  //       i1.loadFromData(s1, s1,vFvMatch[i].getData());
  //       i2.loadFromData(s2, s2, vFeatures[vPrototypes[k]].getData());
  //       cursim = ChamferMatching::getDistance(i1, i2 );
  //
  // 	  //char tmp[32];sprintf(tmp, "%d-%d: %.4f", i, k, cursim);
  // 	  //b->pushImage(i1.getQtImage(),tmp);
  //       //b->pushImage(i2.getQtImage(),tmp);
  //       //printf("%d. cluster: %f ", k, cursim);
  //       cursim = 1.0 - min(1.0f, cursim/100.0f);
  //       //printf("-> norm: %f\n", cursim);
  //       if ( cursim > currentminsim ) {
  //         currentminsim = cursim;
  //         clNo = k;
  //       }
  //
  //       if( cursim > dRejectionThresh ) {
  //         vvAllNeighbors[i].push_back( k );
  //         vvAllNeighborsSim[i].push_back( cursim );
  //       }
  //     }
  //
  //     vNearestNeighbor.push_back( clNo );
  //     vNearestNeighborSim.push_back( currentminsim );
  //   }
  //   cout << "      done." << endl;
}














double computeNGC( FeatureVector &p1, FeatureVector &p2 )
/*******************************************************************/
/* Compute the normalized grayscale correlation (NGC) between two  */
/* feature vectors.                                                */
/*******************************************************************/
{
  /* check if both feature vectors have the same size */
  if ( p1.numDims() != p2.numDims() )
  {
    cerr << "computeNGC(): vectors have different size!" << endl;
    exit(0);
  }

  /* compute the normalization */
  double norm_p1 = (double)p1.getSum();
  double norm_p2 = (double)p2.getSum();
  double sum1     = 0.0;
  double sum2     = 0.0;
  double dividend = 0.0;
  norm_p1 /= (double)p1.numDims();
  norm_p2 /= (double)p2.numDims();

  for(int i=0; i < p1.numDims(); i++)
  {
    double pos1 = p1.at(i) - norm_p1;
    double pos2 = p2.at(i) - norm_p2;
    dividend += ( pos1 * pos2 );
    sum1 += pos1 * pos1;
    sum2 += pos2 * pos2;
  }

  return dividend / ( sqrt( sum1 * sum2 ) );
}


double computeFigureNGC( /*const*/ FeatureVector &p1,
                                   /*const*/ FeatureVector &p2,
                                   /*const*/ FeatureVector &seg,
                                   /*const*/ FeatureVector &gradMagMap,
                                   /*const*/ FeatureVector &trainGradMagMap,
                                   float sum  )
/*******************************************************************/
/* Compute the normalized grayscale correlation (NGC) between two  */
/* feature vectors only over the figure part of the image.         */
/* gradMagMap is used for ensuring edge compatibility (from test   */
/* image)                                                          */
/*******************************************************************/
{
  /* check if all feature vectors have the same size */
  if ( !( (p1.numDims() == p2.numDims()) &&
          ( p1.numDims() == seg.numDims()) ) )
  {
    cerr << "ClManager::computeFigureNGC(): vectors have different size!"
    << endl;
    exit(0);
  }

  double resMap;   //similarity over object pixels
  double resEdge;  //edge response

  /*******************************************************/
  /*   Normalize both patches (over the "figure" area)   */
  /*******************************************************/
  double norm_p1 = (double)p1.getSum();
  double norm_p2 = (double)p2.getSum();
  double sum1     = 0.0;
  double sum2     = 0.0;
  double dividend = 0.0;
  norm_p1 /= (double)p1.numDims();
  norm_p2 /= (double)p2.numDims();

  int seg_over = 0;
  for(int i=0; i < p1.numDims(); i++)
  {
    double pos1 = p1.at(i) - norm_p1;
    double pos2 = p2.at(i) - norm_p2;
    /* if the segmentation map shows "figure" */
    //if ( seg.at(i) > (0.7 * 255.0/*max*/) ) {
    //hint: approximately (1.0 - minIntensity) * 255 [=77]
    seg_over++;
    dividend += ( pos1 * pos2 );
    sum1 += pos1 * pos1;
    sum2 += pos2 * pos2;
    //}
  }

  /* The greyscale similarity is the NGC over all figure pixels: */
  resMap =  dividend / ( sqrt( sum1 * sum2 ) );
  //resMap += 1.0;
  //resMap /= 2.0;//normalize to range [0..1]


  /****************************************************/
  /*   Compute a similarity value considering edges   */
  /****************************************************/
  // compute the normalized correlation (NC), range [0..1]
  // between: gradMagTrainMap (FV) and gradMagMap (FV, from whole image)
  if ( trainGradMagMap.numDims() != gradMagMap.numDims() )
  {
    cerr << "ERROR: #dimensions of gradient magnitude patches is different!"
    << endl;
    return -1.0;
  }

  resEdge = trainGradMagMap.compIntersection( gradMagMap );
  if ( sum == 1.0 )
    resEdge = 0.0; //avoid border effects in opFastGauss2()
  if ( resEdge > 1.0 )
    cout << " resEdge out of range!!!!!!!" << endl;//TTT
  if ( resEdge > 0.95)
    cout << " large resEdge (intersection) (>0.95)..." << resEdge
    << " / " << resMap << " / sum: " << sum << endl;


  /****************************************/
  /*   Compute the resulting similarity   */
  /****************************************/
  /* The inspiration for this computation has been taken from the    */
  /* formula of                                                      */
  /*   E. Borenstein and S. Ullman,                                  */
  /*   Class-Specific, Top-Down Segmentation.                        */
  /*   Proc. ECCV'02, p. 115, 2002.                                  */
  /* Our addition is the weighting factor according to the number of */
  /* figure pixels.                                                  */
  if ( sum > 1.0001 )
  {
    cout << "WARNING: Normalized sum is out of range [0..1]! (" << sum << ")"
    << endl;
  }

  //double w = 0.5;
  //return sum * w * resMap + ( 1.0 - w ) * resEdge;
  //return (resMap + resEdge) / 2.0;
  //return resMap + resEdge;
  //return sum * w * resMap     +    (1.0 - sum) * (1.0 - w) * resEdge;

  //if ( resEdge < 0.1 ) {
  //  return -1.0;
  //}

  return sum * resMap  +  1.2 * (1.0 - sum) * resEdge;//1.5 maximal! 1.2 ok!
}


