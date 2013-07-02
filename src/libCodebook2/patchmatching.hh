/*********************************************************************/
/*                                                                   */
/* FILE         patchmatching.hh                                     */
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

#ifndef PATCHMATCHING_HH
#define PATCHMATCHING_HH


/****************/
/*   Includes   */
/****************/
#include <vector>
#include <stdlib.h>
#include <fstream>

#include <featurevector.hh>
#include <opgrayimage.hh>
#include <opinterestimage.hh>

/*******************/
/*   Definitions   */
/*******************/
const int NORM_NONE        = 0;
const int NORM_STDDEV      = 1;
const int NORM_VARIANCE    = 2;
const int NORM_ENERGY      = 3;

/***************************/
/*   Function Prototypes   */
/***************************/

void compareCodebookNGC        ( vector<FeatureVector> vSrcImgFeatures,
                                 vector<FeatureVector> vClusters,
                                 float dRejectionThresh,
                                 vector<int>             &vNearestNeighbor,
                                 vector<float>           &vNearestNeighborSim,
                                 vector< vector<int> >   &vvAllNeighbors,
                                 vector< vector<float> > &vvAllNeighborsSim );

void compareCodebookNGCEdges   ( vector<FeatureVector> vSrcImgFeatures,
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
                                 vector< vector<float> > &vvAllNeighborsSim );

void compareCodebookCorrelation( const vector<FeatureVector> &vSrcImgFeatures,
                                 /*const*/ vector<FeatureVector> &vClusters,
                                 float dRejectionThresh,
                                 int   nNormalizeMethod,
                                 vector<int>             &vNearestNeighbor,
                                 vector<float>           &vNearestNeighborSim,
                                 vector< vector<int> >   &vvAllNeighbors,
                                 vector< vector<float> > &vvAllNeighborsSim );

void compareCodebookEuklidean( const vector<FeatureVector> &vSrcImgFeatures,
                               /*const*/ vector<FeatureVector> &vClusters,
                               float dRejectionThresh,
                               float dDistFactor,
                               vector<int>             &vNearestNeighbor,
                               vector<float>           &vNearestNeighborSim,
                               vector< vector<int> >   &vvAllNeighbors,
                               vector< vector<float> > &vvAllNeighborsSim );

void compareCodebookES         ( vector<FeatureVector> vSrcImgFeatures,
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
                                 OpGrayImage &imgDffsError,
                                 OpGrayImage &imgDifsError );

void compareCodebookChamfer( const vector<FeatureVector> &vSrcImgFeatures,
                             const vector<FeatureVector> &vFeatures,
                             const vector<int> &vPrototypes,
                             float dRejectionThresh,
                             int   nNormalizeMethod,
                             vector<int>             &vNearestNeighbor,
                             vector<float>           &vNearestNeighborSim,
                             vector< vector<int> >   &vvAllNeighbors,
                             vector< vector<float> > &vvAllNeighborsSim, bool symDist=false );

void compareCodebookChamferOrientation( const vector<FeatureVector> &vSrcImgFeatures,
                             const vector<FeatureVector> &vFeatures,
                             const vector<int> &vPrototypes,
                             float dRejectionThresh,
                             int   nNormalizeMethod,
                             vector<int>             &vNearestNeighbor,
                             vector<float>           &vNearestNeighborSim,
                             vector< vector<int> >   &vvAllNeighbors,
                             vector< vector<float> > &vvAllNeighborsSim, bool symDist=false );

double computeNGC      ( /*const*/ FeatureVector &p1,
                                   /*const*/ FeatureVector &p2 );
double computeFigureNGC( /*const*/ FeatureVector &p1,
                                   /*const*/ FeatureVector &p2,
                                   /*const*/ FeatureVector &seg,
                                   /*const*/ FeatureVector &gradMagMap,
                                   /*const*/ FeatureVector &trainGradMagMap,
                                   float sum  );

#endif
