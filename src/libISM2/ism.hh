/*********************************************************************/
/*                                                                   */
/* FILE         ism.hh                                               */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Interleaved Object Categorization and Segmentation   */
/*              with an Implicit Shape Model.                        */
/*                                                                   */
/*              The class contains methods to perform simultaneous   */
/*              recognition and segmentation of canonical views of   */
/*              categorical objects, such as sideviews of cars or    */
/*              cows. Prior to recognition, a codebook of local ap-  */
/*              pearances (generated by the 'clusterer' program)     */
/*              must be loaded. Extracted image features are then    */
/*              matched to this codebook, thereby activating various */
/*              codebook entries. The list of activated codebook     */
/*              entries is passed as input to the ISM class.         */
/*                                                                   */
/*              In addition, the ISM class needs a file of 'occur-   */
/*              rences', i.e. of stored locations where codebook     */
/*              entries have been observed on the training objects,  */
/*              and of 'occurrence maps', i.e. the stored patch seg- */
/*              mentation masks for those locations, as extracted    */
/*              from reference segmentations for the training ima-   */
/*              ges. From this information, it generates first ini-  */
/*              tial hypotheses for possible object locations (and   */
/*              scales) in the image. For each of those hypotheses,  */
/*              it then computes an automatic top-down segmentation  */
/*              and uses this segmentation to again improve the      */
/*              hypotheses and resolve ambiguities between overlap-  */
/*              ping objects.                                        */
/*                                                                   */
/*              An explanation of the algorithm can be found in the  */
/*              following papers:                                    */
/*                                                                   */
/*              Bastian Leibe and Bernt Schiele,                     */
/*              Interleaved Object Categorization and Segmentation.  */
/*              In Proc. British Machine Vision Conference (BMVC'03) */
/*              Norwich, GB, Sept. 9-11, 2003.                       */
/*                                                                   */
/*              Bastian Leibe, Ales Leonardis, and Bernt Schiele,    */
/*              Combined Object Categorization and Segmentation with */
/*              an Implicit Shape Model.                             */
/*              in ECCV'04 Workshop on Statistical Learning in       */
/*              Computer Vision, Prague, May 2004.                   */
/*                                                                   */
/*              Bastian Leibe and Bernt Schiele,                     */
/*              Scale-Invariant Object Categorization using a Scale- */
/*              Adaptive Mean-Shift Search.                          */
/*              In Proc. DAGM'04 Pattern Recognition Symposium,      */
/*              Tuebingen, Germany, Aug. 2004. Springer LNCS,        */
/*              Vol. 3175, pp. 145--153, 2004.                       */
/*                                                                   */
/* BEGIN        Tue Nov 05 2002                                      */
/* LAST CHANGE  Thu Jan 20 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef ISM_HH
#define ISM_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <cassert>

#include <featurevector.hh>
#include <votingspace.hh>
#include <opinterestimage.hh>
#include <featurecue.hh>
#include <matchinginfo.hh>

#include "recoparams.hh"
#include "occurrences.hh"
#include "segmentation.hh"

/*******************/
/*   Definitions   */
/*******************/
// const int CMP_CORRELATION     = 0;
// const int CMP_EUKLIDEAN       = 1;
// const int CMP_NNSEARCH        = 2;
// const int CMP_EV_DIST         = 3;

const float MIN_VOTE_WEIGHT   = 0.0002;


/*************************/
/*   Class Definitions   */
/*************************/

/*------------*/
/* Hypothesis */
/*------------*/
typedef struct _Hypothesis
{
public:
  //void print();

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
  float dAngle;
  int   nPose;
  bool  bPoseFlipped;
  int   nTemplateId;
  float dAreaFactor;
  float dAspect;
  float dRealDist;
  float dRealSize;
} Hypothesis;

void printHypothesis            ( Hypothesis hypo );
void printHypothesisMDL         ( Hypothesis hypo );

typedef vector<Hypothesis> HypoVec;

/*===================================================================*/
/*                            Class ISM                              */
/*===================================================================*/
class ISM
{
public:
  ISM( int nCategory, int nCue=0 );

public:
  /*******************************/
  /*   Content Access Operators  */
  /*******************************/
  VotingSpace& getVotingSpace() { return m_vsHoughVotes; }

public:
  /******************/
  /*   Parameters   */
  /******************/
  void setRecoParams( RecoParams parReco );

public:
  /*******************/
  /*   Occurrences   */
  /*******************/
  void initOccurrences  ( int nClusters );
  void addOccurrences   ( const VecVecOccurrence    &vvOccurrences,
                          const vector<OpGrayImage> &vOccMaps,
                          int &nOccMapIdx );
  void finishOccurrences();

  void loadOccurrences      ( string sFileName, int nNumClusters, 
                              bool bVerbose=true );
  void saveOccurrences      ( string sFileName, bool bVerbose=true );
  void saveOccurrencesMatlab( string sFileName, bool bVerbose=true );

  unsigned            getNumOccs()     const { return m_nNumOccs; }
  VecVecOccurrence    getOccurrences() const { return m_vvOccurrences; }
  vector<float>       getOccWeights()  const { return m_vOccSumWeights; }
  vector<OpGrayImage> getOccMaps()     const { return m_vOccMaps;      }

  void setOccurrences( const VecVecOccurrence & vvOcc ) 
                     { m_vvOccurrences = vvOcc; finishOccurrences(); }
  void setOccWeights ( const vector<float> &vOccWeights )
                     { m_vOccSumWeights = vOccWeights; }
  void setOccMaps    ( const vector<OpGrayImage> &vOccMaps )
                     { m_vOccMaps = vOccMaps; }

  void removeOccurrences( const vector<bool> &vIdzs );

public:
  /**********************************/
  /*   VotingSpace Initialization   */
  /**********************************/
  void clearVotingSpace   ();
  void createVotingSpace  ( int nImgWidth, int nImgHeight, int nStepSize,
                            bool bVerbose=false );
  void createVotingSpace  ( int nImgWidth, int nImgHeight, int nStepSize,
                            float dScaleMin, float dScaleMax, int nScaleSteps,
                            bool bVerbose=false );

public:
  /**********************/
  /*   Initial Voting   */
  /**********************/
  void doPatchVoting  ( const PointVector             &vPoints,
                        const vector< vector<int> >   &vvAllNeighbors, 
                        const vector< vector<float> > &vvAllNeighborsSim,
                        float dRejectionThresh,
                        bool bVerbose=false );
  void doPatchVoting  ( const PointVector             &vPoints,
                        MatchingInfo                  &miMatching, 
                        float dRejectionThresh,
                        bool bVerbose=false );

  void doPatchVoting  ( VotingSpace                   &vsHoughVotes,
                        const PointVector             &vPoints,
                        const vector< vector<int> >   &vvAllNeighbors, 
                        const vector< vector<float> > &vvAllNeighborsSim,
                        float dPrior, float dRejectionThresh, 
                        bool bVerbose=false );
  void doPatchVoting  ( VotingSpace                   &vsHoughVotes,
                        const PointVector             &vPoints,
                        MatchingInfo                  &miMatching, 
                        float dPrior, float dRejectionThresh, 
                        bool bVerbose=false );
  
protected:
  void doPatchVoting2D( VotingSpace                   &vsHoughVotes,
                        const PointVector             &vPoints,
                        const vector< vector<int> >   &vvAllNeighbors, 
                        const vector< vector<float> > &vvAllNeighborsSim,
                        float dPrior, float dRejectionThresh,
                        bool bVerbose=false );
  void doPatchVoting3D( VotingSpace                   &vsHoughVotes, 
                        const PointVector             &vPoints,
                        const vector< vector<int> >   &vvAllNeighbors, 
                        const vector< vector<float> > &vvAllNeighborsSim,
                        float dPrior, float dRejectionThresh,
                        bool bVerbose=false );

public:
  /*****************************/
  /*   Hypothesis Extraction   */
  /*****************************/
  HypoVec getPatchHypotheses  ( const PointVector &vPoints,
                                int   nStepSize,
                                vector<OpGrayImage> &vImgVotes,
                                bool  bVerbose=false );  

protected:
  HypoVec getPatchHypotheses2D( const PointVector &vPoints,
                                int   nStepSize,
                                bool  bExtendSearch, 
                                int   nAddedRangeX, int   nAddedRangeY,
                                vector<OpGrayImage> &vImgVotes,
                                bool  bVerbose=false );
  
  HypoVec getPatchHypotheses3D( const PointVector &vPoints,
                                int   nStepSize,
                                bool  bExtendSearch, 
                                int   nAddedRangeX, int   nAddedRangeY,
                                vector<OpGrayImage> &vImgVotes,
                                bool  bVerbose=false );
  
public:
  /*****************************/
  /*   Hypothesis Refinement   */
  /*****************************/
  HypoVec refinePatchHypotheses  ( const HypoVec     &vInitialHypos,
                                   const PointVector &vPoints,
                                   bool  bVerbose=false );  

protected:
  HypoVec refinePatchHypotheses2D( const HypoVec     &vInitialHypos,
                                   const PointVector &vPoints,
                                   bool  bVerbose=false );
  
  HypoVec refinePatchHypotheses3D( const HypoVec     &vInitialHypos,
                                   const PointVector &vPoints,
                                   bool  bVerbose=false );
  
public:
  /************************/
  /*   Supporting Votes   */
  /************************/
  
  list<HoughVote> getSupportingVotes( float x, float y );
  list<HoughVote> getSupportingVotes( float x, float y, float s );
  list<HoughVote> getSupportingVotes( FeatureVector fvWindowPos );
  list<HoughVote> getSupportingVotes( FeatureVector fvWindowPos,
                                        FeatureVector fvWindowSize );
  /*****************************************/
  /* Supporting Votes, added by yuren zhang*/
  /*****************************************/
  vector<HoughVote> getSupportingVotes_v(float x, float y);
  vector<HoughVote> getSupportingVotes_v( float x, float y, float s );
  vector<HoughVote> getSupportingVotes_v( FeatureVector fvWindowPos );
  vector<HoughVote> getSupportingVotes_v( FeatureVector fvWindowPos,
                                        FeatureVector fvWindowSize );
 
public:
  /**********************************/
  /*   Draw PatchMatching Results   */
  /**********************************/
  static const int   DRAW_SUPPORT = 0;
  static const int   DRAW_SEGMENT = 1;
  static const int   DRAW_PFIG    = 2;
  static const int   DRAW_PGND    = 3;
  OpGrayImage drawVotePatches   ( /*const*/ vector<HoughVote> &vVotes, 
                                  const PointVector           &vPoints,
                                  const vector<OpGrayImage>   &vClusterPatches,
                                  const FeatureCue            &parFeatures,
                                  int nDrawStyle=DRAW_SUPPORT,
                                  bool bBackgroundZero=false,
                                  bool bVerbose=false );


  OpGrayImage drawVotePatches   ( /*const*/ list<HoughVote> &vVotes, 
                                  const PointVector           &vPoints,
                                  const vector<OpGrayImage>   &vClusterPatches,
                                  const FeatureCue            &parFeatures,
                                  int nDrawStyle=DRAW_SUPPORT,
                                  bool bBackgroundZero=false,
                                  bool bVerbose=false );

public:
  /*****************************/
  /*   Top-Down Segmentation   */
  /*****************************/
   void drawSegmentation          ( const vector<HoughVote>     &vVotes,
                                   const PointVector         &vPoints,
                                   const FeatureCue          &parFeatures,
                                   OpGrayImage &imgPFig, 
                                   OpGrayImage &imgPGnd,
                                   OpGrayImage &imgSeg,
                                   bool  bBackgroundZero=false,
                                   float dRescaleBy=1.0,
                                   bool  bVerbose=false );
void drawSegmentation          ( const list<HoughVote>     &vVotes,
                                   const PointVector         &vPoints,
                                   const FeatureCue          &parFeatures,
                                   OpGrayImage &imgPFig, 
                                   OpGrayImage &imgPGnd,
                                   OpGrayImage &imgSeg,
                                   bool  bBackgroundZero=false,
                                   float dRescaleBy=1.0,
                                   bool  bVerbose=false );
  void drawSegmentation          ( const list<HoughVote>     &vVotes,
                                   const PointVector         &vPoints,
                                   float dPatchScaleFactor,
                                   OpGrayImage &imgPFig, 
                                   OpGrayImage &imgPGnd,
                                   OpGrayImage &imgSeg,
                                   bool  bBackgroundZero=false,
                                   float dRescaleBy=1.0,
                                   bool  bVerbose=false );
   void drawSegmentationOffset    ( const vector<HoughVote>   &vVotes,
                                   const PointVector       &vPoints,
                                   const FeatureCue        &parFeatures,
                                   int nOffX, int nOffY, float dHypoScale,
                                   OpGrayImage &imgPFig, 
                                   OpGrayImage &imgPGnd,
                                   OpGrayImage &imgSeg,
                                   bool  bBackgroundZero,
                                   float dRescaleBy=1.0,
                                   int   nSegWidth=-1, int nSegHeight=-1,
                                   bool  bVerbose=false );

  void drawSegmentationOffset    ( const list<HoughVote>   &vVotes,
                                   const PointVector       &vPoints,
                                   const FeatureCue        &parFeatures,
                                   int nOffX, int nOffY, float dHypoScale,
                                   OpGrayImage &imgPFig, 
                                   OpGrayImage &imgPGnd,
                                   OpGrayImage &imgSeg,
                                   bool  bBackgroundZero,
                                   float dRescaleBy=1.0,
                                   int   nSegWidth=-1, int nSegHeight=-1,
                                   bool  bVerbose=false );
  void drawSegmentationOffset    ( const list<HoughVote>   &vVotes,
                                   const PointVector       &vPoints,
                                   float dPatchScaleFactor,
                                   int nOffX, int nOffY, float dHypoScale,
                                   OpGrayImage &imgPFig, 
                                   OpGrayImage &imgPGnd,
                                   OpGrayImage &imgSeg,
                                   bool bBackgroundZero,
                                   float dRescaleBy,
                                   int nSegWidth, int nSegHeight,
                                   bool bVerbose );

  Segmentation drawSegmentation      ( const list<HoughVote>   &vVotes,
                                       const PointVector         &vPoints,
                                       const FeatureCue          &parFeatures,
                                       bool  bBackgroundZero=false,
                                       float dRescaleBy=1.0,
                                       bool  bVerbose=false );
  Segmentation drawSegmentation      ( const list<HoughVote>   &vVotes,
                                       const PointVector         &vPoints,
                                       float dPatchScaleFactor,
                                       bool  bBackgroundZero=false,
                                       float dRescaleBy=1.0,
                                       bool  bVerbose=false );

 Segmentation drawSegmentationOffset( const vector<HoughVote> &vVotes,
                                       const PointVector       &vPoints,
                                       const FeatureCue        &parFeatures,
                                       int nOffX, int nOffY, float dHypoScale,
                                       int nSegWidth, int nSegHeight,
                                       bool  bBackgroundZero,
                                       float dRescaleBy=1.0,
                                       bool  bVerbose=false );

  Segmentation drawSegmentationOffset( const list<HoughVote> &vVotes,
                                       const PointVector       &vPoints,
                                       const FeatureCue        &parFeatures,
                                       int nOffX, int nOffY, float dHypoScale,
                                       int nSegWidth, int nSegHeight,
                                       bool  bBackgroundZero,
                                       float dRescaleBy=1.0,
                                       bool  bVerbose=false );
  Segmentation drawSegmentationOffset( const list<HoughVote> &vVotes,
                                       const PointVector       &vPoints,
                                       float dPatchScaleFactor,
                                       int nOffX, int nOffY, float dHypoScale,
                                       int nSegWidth, int nSegHeight,
                                       bool  bBackgroundZero,
                                       float dRescaleBy=1.0,
                                       bool  bVerbose=false );

public:
  OpGrayImage createAffinePatch ( OpGrayImage img, InterestPoint pt,
                                  const FeatureCue &parFeatures );
  OpGrayImage createAffinePatch ( OpGrayImage img, InterestPoint pt,
                                  float dScaleFactor );


public:
  /************************/
  /*   MDL Verification   */
  /************************/
  vector<Hypothesis> doMDLSelection( HypoVec             &vHypos,
                                     vector<OpGrayImage> &vImgSegment,
                                     vector<OpGrayImage> &vImgPFig,
                                     vector<OpGrayImage> &vImgPGnd, 
                                     vector<float>       &vSumPFig,
                                     vector<int>         &vRanks,
                                     bool                bAlwaysAdapt=false,
                                     bool                bVerbose=false );
  vector<Hypothesis> doMDLSelection( HypoVec             &vHypos,
                                     vector<OpGrayImage> &vImgSegment,
                                     vector<OpGrayImage> &vImgPFig,
                                     vector<OpGrayImage> &vImgPGnd, 
                                     vector<float>       &vSumPFig,
                                     float dThreshold, float dWeightPFig,
                                     vector<int>         &vRanks,
                                     bool                bAlwaysAdapt=false,
                                     bool                bVerbose=false );

  vector<Hypothesis> doMDLSelection( HypoVec              &vHypos,
                                     vector<Segmentation> &vSegmentations,
                                     vector<int>          &vRanks,
                                     bool                 bAlwaysAdapt=false,
                                     bool                 bVerbose=false );
  vector<Hypothesis> doMDLSelection( HypoVec              &vHypos,
                                     vector<Segmentation> &vSegmentations,
                                     float dThreshold, float dWeightPFig,
                                     vector<int>          &vRanks,
                                     bool                 bAlwaysAdapt=false,
                                     bool                 bVerbose=false );

  vector<Hypothesis> doMDLSelExact ( HypoVec             &vHypos,
                                     vector<OpGrayImage> &vImgSegment,
                                     vector<OpGrayImage> &vImgPFig,
                                     vector<OpGrayImage> &vImgPGnd, 
                                     vector<float>       &vSumPFig,
                                     float Threshold,
                                     vector<int>         &vRanks,
                                     bool                bAlwaysAdapt=false,
                                     bool                bVerbose=false );

  float getMDLScore  ( float dSumPFig, float dFigArea, float dScale );
  bool  checkOverlap ( int  Px1, int  Py1, int  Px2, int  Py2,
                       int  Qx1, int  Qy1, int  Qx2, int  Qy2,
                       int &Rx1, int &Ry1, int &Rx2, int &Ry2 );

public:
  /*************************/
  /*   Service Functions   */
  /*************************/
  FeatureVector      getAdaptiveWinSize( FeatureVector fvWindowPos );

  void writeResultsToDiskUIUC ( ofstream &ofile, int nImgNumber,
                                const HypoVec &vHypos );
  
  void writeResultsToDiskScore( ofstream &ofile, int nImgNumber,
                                const HypoVec &vHypos );
  

private:
  RecoParams m_parReco;

  unsigned               m_nNumOccs;
  VecVecOccurrence       m_vvOccurrences;
  vector<float>          m_vOccSumWeights;
  vector<OpGrayImage>    m_vOccMaps;
  vector<int>            m_vNegOccs;

  VotingSpace            m_vsHoughVotes;

  int                    m_nCategory;
  int                    m_nCue;
};


#endif
