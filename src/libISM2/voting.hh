/*********************************************************************/
/*                                                                   */
/* FILE         voting.hh                                            */
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

#ifndef VOTING_HH
#define VOTING_HH

using namespace std;
    
/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>

#include <featurevector.hh>
#include <opgrayimage.hh>
#include <opinterestimage.hh>

#include "occurrences.hh"
#include "parts.hh"
//#include "houghtransform.hh"
#include "votingspace.hh"

/*******************/
/*   Definitions   */
/*******************/
const int NUM_POSES = 16;
const string POSE_NAMES[]  = { "090-000", "090-022", "090-045", "090-068", 
                               "090-090", "090-112", "090-135", "090-158", 
                               "090-180", "090-202", "090-225", "090-248", 
                               "090-270", "090-292", "090-315", "090-338" };

const float MIN_VOTE_WEIGHT   = 0.0002;

/*------------*/
/* Hypothesis */
/*------------*/
typedef struct _Hypothesis
{
public:
  int   x;
  int   y;
  int   nBoxX1;
  int   nBoxY1;
  int   nBoxWidth;
  int   nBoxHeight;
  float dScore;
  float dScoreMDL;
  int   nCategory;
  float dScale;
  int   nPose;
} Hypothesis;


/*--------*/
/* Result */
/*--------*/
typedef struct _Result
{
public:
  int   nRank;
  float dDistX;
  float dDistY;
  float dDist;
  float dCover;
  float dOverlap;
} Result;


/***************************/
/*   Function Prototypes   */
/***************************/

vector< vector<float> > getOccurrenceWeights( VecVecOccurrence vvOccurrences,
                                              bool bNormPerPose );

void doPatchVoting     ( const VecVecOccurrence        &vvOccurrences, 
                         const vector<int>             &vNegOccs,
                         const PointVector             &vPointsInside,
                         const vector<int>             &vNearestNeighbor, 
                         const vector<float>           &vNearestNeighborSim,
                         const vector< vector<int> >   &vvAllNeighbors, 
                         const vector< vector<float> > &vvAllNeighborsSim,
                         float dRejectionThresh,
                         int nImgWidth, int nImgHeight, int nStepSize,
                         bool bNormPerPatch, bool bNormPerPose, 
                         VotingSpace            &vsHoughVotesGlobal,
                         VotingSpace            &vsHoughVotesPose );

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
                         VotingSpace    &vsHoughVotesGlobal,
                         VotingSpace    &vsHoughVotesScale );

vector<Hypothesis> getPatchHypotheses  ( VotingSpace   &vsHoughVotesGlobal,
                                         PointVector       vPointsInside,
                                         int nImgWidth, int nImgHeight, 
                                         int nStepSize,
                                         int nCateg,
                                         float dMSMESizeX, float dMSMESizeY,
                                         //float dWindowSizeX, 
                                         //float dWindowSizeY,
                                         int   nPatchSize,
                                         OpGrayImage &imgPatchVotesGlob,
                                         OpGrayImage &imgPatchMaxima );

vector<Hypothesis> getPatchHypothesesScale( VotingSpace &vsHoughVotesScale,
                                            PointVector vPointsInside,
                                            int nImgWidth, int nImgHeight, 
                                            int nStepSize, 
                                            float dScaleMin, float dScaleMax,
                                            int nScaleSteps,
                                            int nCateg,
                                            float dMSMESizeX, float dMSMESizeY,
                                            float dMSMESizeS, 
                                            //float dWindowSizeX, 
                                            //float dWindowSizeY,
                                            //float dWindowSizeS,
                                            int   nPatchSize,
                                            vector<OpGrayImage> &vImgPatchVotesScale,
                                            vector<OpGrayImage> &vImgPatchMaxima );

vector<Hypothesis> getPatchHypothesesMSME( VotingSpace &vsHoughVotesGlobal,
                                           PointVector       vPointsInside,
                                           int   nImgWidth, int nImgHeight, 
                                           int   nStepSize, 
                                           int   nCateg,
                                           float dMSMESizeX, float dMSMESizeY,
                                           int   nPatchSize,
                                           bool  bExtendSearch, 
                                           int   nAddedRangeX,
                                           int   nAddedRangeY,
                                           OpGrayImage &imgPatchVotesGlob,
                                           OpGrayImage &imgPatchMaxima );

vector<Hypothesis> getPatchHypothesesScaleMSME(VotingSpace &vsHoughVotesGlobal,
                                              const PointVector &vPointsInside,
                                               int   nImgWidth, 
                                               int nImgHeight, 
                                               int   nStepSize, 
                                               float dScaleMin, 
                                               float dScaleMax,
                                               int nScaleSteps,
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
                                               vector<OpGrayImage> &vImgPatchVotesGlob,
                                               vector<OpGrayImage> &vImgPatchMaxima );


vector<Hypothesis> getPatchHypothesesPose( VotingSpace &vsHoughVotesPose,
                                           PointVector       vPointsInside,
                                           vector<OpGrayImage> &vAvgImages,
                                           //vector< vector<OpGrayImage> > &vvOccMaps,
                                           vector<OpGrayImage> &vOccMaps,
                                           int nImgWidth, int nImgHeight, 
                                           int nStepSize, 
                                           int nCateg,
                                           float dMSMESizeX, float dMSMESizeY,
                                           float dMSMESizeP,
                                           float dWindowSizeX, 
                                           float dWindowSizeY,
                                           int   nPatchSize,
                                           vector<OpGrayImage> &imgPatchVotesPose,
                                           vector<OpGrayImage> &imgPatchMaximaPose );


vector<Hypothesis> extractHypotheses2D  ( OpGrayImage imgVotes, 
                                          VotingSpace &vsHoughVotesGlobal,
                                          PointVector       vPointsInside,
                                          int nImgWidth, int nImgHeight, 
                                          int nStepSize,
                                          float dMSMESizeX, float dMSMESizeY,
                                          int nPatchSize,
                                          int nCategory, int nPose );

vector<Hypothesis> extractHypotheses2DMSME( OpGrayImage imgVotes, 
                                            VotingSpace &vsHoughVotesGlobal,
                                            PointVector       vPointsInside,
                                            int nImgWidth, int nImgHeight, 
                                            int nStepSize,
                                            int nAddedRangeX,
                                            int nAddedRangeY,
                                            float dMSMESizeX, float dMSMESizeY,
                                            int nPatchSize,
                                            int nCategory, int nPose );

vector<Hypothesis> refineHypotheses2DMSME( vector<Hypothesis> &vInitialHypos, 
                                           VotingSpace &vsHoughVotesGlobal,
                                           PointVector       vPointsInside,
                                           int nImgWidth, int nImgHeight, 
                                           int nStepSize,
                                           float dMSMESizeX, float dMSMESizeY,
                                           int nPatchSize,
                                           int nCategory, int nPose );

vector<Hypothesis> refineHypotheses3DMSME( vector<Hypothesis> &vInitialHypos, 
                                           VotingSpace &vsHoughVotesScale,
                                           PointVector       vPointsInside,
                                           int nImgWidth, int nImgHeight, 
                                           int nStepSize,
                                           float dMSMESizeX, float dMSMESizeY,
                                           float dMSMESizeS,
                                           int nPatchSize,
                                           int nCategory, int nPose );

vector<Hypothesis> extractHypotheses3D  ( vector<OpGrayImage> vImgVotes, 
                                          VotingSpace &vsHoughVotesPose,
                                          PointVector       vPointsInside,
                                          int nImgWidth, int nImgHeight, 
                                          int nStepSize,
                                          float dMSMESizeX, float dMSMESizeY,
                                          float dMSMESizeP,
                                          int nPatchSize,
                                          int nCategory );

vector<Hypothesis> extractHypotheses3DMSME( /*const*/ vector<OpGrayImage> &vImgVotes, 
                                            VotingSpace &vsHoughVotesGlobal,
                                            const PointVector &vPointsInside,
                                            int nImgWidth, int nImgHeight, 
                                            int nStepSize,
                                            float dScaleMin, float dScaleMax,
                                            int nAddedRangeX,
                                            int nAddedRangeY,
                                            float dMSMESizeX, float dMSMESizeY,
                                            float dMSMESizeS,
                                            int nPatchSize,
                                            float dThresh,
                                            int nCategory,
                                            bool bAdaptiveScale=false,
                                            float dAdaptMinScale=1.0 );

void getHypothesisBoundingBox   ( VotingSpace &vsHoughVotesGlobal,
                                  PointVector       vPointsInside,
                                  int x, int y, 
                                  int nImgWidth, int nImgHeight, int nStepSize,
                                  int nPatchSize,
                                  int &x1, int &y1, 
                                  int &width, int &height );

void getHypothesisBoundingBox   ( VotingSpace &vsHoughVotesPose,
                                  PointVector       vPointsInside,
                                  int x, int y, int p,
                                  int nImgWidth, int nImgHeight, int nStepSize,
                                  int nPatchSize,
                                  int &x1, int &y1, 
                                  int &width, int &height );

OpGrayImage getHypothesisArea   ( VotingSpace &vsHoughVotesPose,
                                  PointVector       vPointsInside,
                                  //vector< vector<FeatureVector> > &vvOccMaps,
                                  vector<FeatureVector> &vOccMaps,
                                  int x, int y, int nPose,
                                  int nImgWidth, int nImgHeight, int nStepSize,
                                  float dMSMESizeX, float dMSMESizeY,
                                  float dMSMESizeP,
                                  int nPatchSize );

OpGrayImage getHypothesisArea   ( vector<HoughVote> &vSupportingVotes,
                                  PointVector       vPointsInside,
                                  //vector< vector<OpGrayImage> > &vvOccMaps,
                                  vector<OpGrayImage> &vOccMaps,
                                 int nImgWidth, int nImgHeight, 
                                  int nPatchSize );

float computeBoundingBoxCoverage( Hypothesis h1, Hypothesis h2 );
float computeBoundingBoxOverlap ( Hypothesis h1, Hypothesis h2 );

float computeAreaCoverage       ( OpGrayImage imgHypoArea, 
                                  OpGrayImage imgModelArea,
                                  float dThresh1, float dThresh2 );
float computeAreaCoverage       ( OpGrayImage imgHypoArea, 
                                  OpGrayImage imgModelArea,
                                  int offset_x, int offset_y,
                                  float dThresh1, float dThresh2 );
float computeAreaOverlap        ( OpGrayImage imgHypoArea, 
                                  OpGrayImage imgModelArea,
                                  float dThresh1, float dThresh2 );
float computeAreaOverlap        ( OpGrayImage imgHypoArea, 
                                  OpGrayImage imgModelArea,
                                  int offset_x, int offset_y,
                                  float dThresh1, float dThresh2 );

void printHypothesis            ( Hypothesis hypo );
void printHypothesisMDL         ( Hypothesis hypo );



void getResultRanks    ( vector<Hypothesis> vRecoResults,
                         vector<Hypothesis> vGroundTruth,
                         vector<float> vTolerances,
                         vector<Result> &vRawResults,
                         vector<Result> &vResultRanks );

void writeResultsToDisk( ofstream &ofile, int nImgNumber,
                         vector<Result> vResults );

void writeResultsToDiskUIUC( ofstream &ofile, int nImgNumber,
                             vector<Hypothesis> vHypos );

void writeResultsToDiskScore( ofstream &ofile, int nImgNumber,
                              vector<Hypothesis> vHypos );

#endif
