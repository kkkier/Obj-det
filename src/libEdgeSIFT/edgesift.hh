/*********************************************************************/
/*                                                                   */
/* FILE         edgesift.hh                                          */
/* AUTHORS      Oliver Bay, Bastian Leibe                            */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Implementation of an edge-based feature descriptor   */
/*              based on the paper:                                  */
/*                                                                   */
/*              K. Mikolajczyk, A. Zisserman, and C. Schmid          */
/*              Shape Recognition with Edge-Based Features.          */
/*              In Proc. British Machine Vision Conference (BMVC'03) */
/*              Norwich, GB, Sept. 9-11, 2003.                       */
/*                                                                   */
/* BEGIN        Fri Mar 05 2004                                      */
/* LAST CHANGE  Fri Mar 05 2004                                      */
/*                                                                   */
/*********************************************************************/

#ifndef EDGESIFT_HH
#define EDGESIFT_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include <opgrayimage.hh>
#include <opinterestimage.hh>
#include <featurevector.hh>


/*******************/
/*   Definitions   */
/*******************/
const float THRESH_ROT_EDGE = 180.0;

void applyEdgeScaleSpace( OpGrayImage img, 
                          int nNumOctaves, int nLevelsPerOctave, 
                          float dScaleSigma0, bool bInterpolate, 
                          float dEdgeMinSigma, float dEdgeMaxSigma, 
                          float dEdgeIncSigma,
                          float dThreshCannyLo, float dThreshCannyHi,
                          int nthEdgePixel, float dEdgeThresh,
                          float dScaleFactor, bool bNormOri,
                          PointVector &vPoints,
                          vector<OpGrayImage> &vEdgePatches,
                          vector<OpGrayImage> &vDirPatches );


void createEdgeFeatures( /*const*/ PointVector &vPoints,
                         /*const*/ vector<OpGrayImage> &vEdgePatches,
                         /*const*/ vector<OpGrayImage> &vDirPatches,
                         vector<FeatureVector> &vFeatures16d,
                         vector<FeatureVector> &vFeatures128d );


OpGrayImage drawEdgeFeature    ( FeatureVector fvFeature );
OpGrayImage drawEdgeFeature16d ( FeatureVector fvFeature );
OpGrayImage drawEdgeFeature128d( FeatureVector fvFeature );

OpGrayImage drawEdgeFeature16d_2 ( FeatureVector fvFeature );
OpGrayImage drawEdgeFeature128d_2( FeatureVector fvFeature );

OpGrayImage drawEdgeFeatureBoth( FeatureVector fv16, FeatureVector fv128 );



#endif
