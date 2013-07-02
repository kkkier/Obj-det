/*********************************************************************/
/*                                                                   */
/* FILE         detector.hh                                          */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@visin.ee.ethz.ch                               */
/*                                                                   */
/* CONTENT      Convenience class for bundeling all information of a */
/*              single viewpoint or category detector (possibly      */
/*              based on multiple cues).                             */
/*                                                                   */
/* BEGIN        Thu Feb 09 2006                                      */
/* LAST CHANGE  Thu Feb 09 2006                                      */
/*                                                                   */
/*********************************************************************/

#ifndef DETECTOR_HH
#define DETECTOR_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <cassert>

#include <opgrayimage.hh>
#include <featurevector.hh>
#include <votingspace.hh>
#include <codebook.hh>
#include <occurrences.hh>
#include <ism.hh>
#include <featurecue.hh>
#include <featurecue.hh>
#include <recogui.hh>
#include <recoparams.hh>
#include <matchinggui.hh>
#include <matchingparams.hh>
#include <matchinginfo.hh>
#include <featurecue.hh>
#include <calibration.hh>

#include "cuewidget.hh"
#include "detectorgui.hh"

/*******************/
/*   Definitions   */
/*******************/
const int CUE_COMBI_SUM  = 0;
const int CUE_COMBI_AVG  = 1;
const int CUE_COMBI_MAX  = 2;


typedef std::map<string,SharedCodebook>     CBTable;
typedef std::map<string,SharedMatchingInfo> MatchTable;


/*************************/
/*   Class Definitions   */
/*************************/
/*===================================================================*/
/*                        Class Detector                             */
/*===================================================================*/
class Detector: public QObject
{
  Q_OBJECT
public:
  Detector();
  Detector( const Detector &other );
  
  Detector& operator=( const Detector &other );

protected:
  void copyFrom( const Detector &other );

public:
  /*********************/
  /*   GUI Functions   */
  /*********************/
  DetectorGUI* createGUI( QWidget *parent=0, const char* name=0 );
  void         setGUI( DetectorGUI* pGUI );

  RecoParams&  getRecoParams() { return m_parReco; }
  void         setRecoParams( RecoParams &parReco );

  void         setActiveCBList   ( CBTable *pmActiveCBs )
  { m_pmActiveCBs = pmActiveCBs; }
  void         setActiveMatchList( MatchTable *pmActiveMatches,
                                   MatchTable *pmActiveMatchesL )
  { m_pmActiveMatches=pmActiveMatches; m_pmActiveMatchesL=pmActiveMatchesL; }

public:
  /*******************************/
  /*   Content Access Operators  */
  /*******************************/
  bool          isValid() const { return (m_guiDetect != 0); }
  DetectorGUI*  params(); 

  void          setDefaultDirs( const string &sDirCodebooks, 
                                const string &sDirResults );

  void         setCalibration  ( Calibration &calCamera );
  void         clearCalibration() { m_bCalibAvailable=false; }
  Calibration& getCalibration  () { return m_calCamera; }

  /**************************/
  /*   Interface Handlers   */
  /**************************/
signals:
  void sigDrawHypothesis           ( const Hypothesis&, const QColor&, 
                                     bool bPermanent );
  void sigRefreshSrcImg            ();
  void sigDisplayResultImg         ( const QImage& );
  void sigDisplayResultImg         ( const QImage&, OpGrayImage );
  
  void sigDetectorChanged          ();
  void sigCuesChanged              ();

public slots:
  /*************************/
  /*   Regular Functions   */
  /*************************/
  /*--------------------------*/
  /* Loading/Saving Detectors */
  /*--------------------------*/
  void clearDetector();
  void loadDetector();
  void loadDetector( string sDetName, bool bVerbose=true );
  void saveDetector();
  void saveDetector( string sDetName, bool bVerbose=true );

  /*--------------*/
  /* Loading Cues */
  /*--------------*/
  void addCue();
  void addCue( string sCBName, string sOccName, bool bVerbose=true );

  /* calling routines from codebook.hh */
  void loadCodebook   ( unsigned nIdx, bool bVerbose=true );
  void loadCodebook   ( unsigned nIdx, string sFileName, bool bLoadCB=true,
                        bool bVerbose=true );

  /* calling routines from ism.hh */
  void loadOccurrences( unsigned nIdx, bool bVerbose=true );
  void loadOccurrences( unsigned nIdx, string sFileName, bool bVerbose=true );

  void propagateDetectorUpdate()               { emit sigDetectorChanged(); }
  void propagateDetectorUpdate(const QString&) { emit sigDetectorChanged(); }
  void propagateCueUpdate     ()               { emit sigCuesChanged(); }

  /*--------------------------------------*/
  /* Comparing Features with the Codebook */
  /*--------------------------------------*/
  void compareFeatures    ( vector<vector<FeatureVector> > &vvFeatures,
                            vector<vector<FeatureVector> > &vvFeaturesLeft,
                            bool bVerbose=false );
  void compareFeatures    ( unsigned nIdx, vector<FeatureVector> &vFeatures,
                            bool bVerbose=false );
  void compareFeaturesLeft( unsigned nIdx, vector<FeatureVector> &vFeatures,
                            bool bVerbose=false );

  //void testMatching( unsigned nIdx=0 );

  /*-------------------------*/
  /* Processing a Test Image */
  /*-------------------------*/
  /* calling routines from ism.hh */
//   void processTestImg       ( int nImgWidth, int nImgHeight, 
//                               int nVotingBinSize,
//                               vector<Hypothesis>   &vResultHypos,
//                               vector<Segmentation> &vSegmentations,
//                               bool bDisplayVS, bool bDisplayResults );
  void processTestImg ( int nImgWidth, int nImgHeight, 
                        int nVotingBinSize,
                        vector<Hypothesis>   &vResultHypos,
                        vector<Segmentation> &vSegmentations,
                        bool bDisplayVS, bool bDisplayResults,
                        bool bVerbose=false, bool bPrintTiming=true );

  /* calling routines from ism.hh */
  vector<Hypothesis> getPatchHypotheses     ( bool bDisplayResults=true,
                                              bool bVerbose=true );  

  vector<Hypothesis> adjustHypoScoresGP     (const vector<Hypothesis> &vHypos);
  vector<Hypothesis> removeDuplicateHypos   (const vector<Hypothesis> &vHypos,
                                             bool bVerbose=false );
  vector<Hypothesis> setHypoBBox            ( const vector<Hypothesis> &vHypos,
                                              int nObjWidth, int nObjHeight );
  Hypothesis         flipHypothesis         ( const Hypothesis &hypo, 
                                              int nImgWidth );

  Segmentation       getHypoSegmentation    ( Hypothesis hypo,
                                              int nObjWidth, int nObjHeight,
                                              int nImgWidth, int nImgHeight );
  Segmentation       getHypoSegmentationLeft( Hypothesis hypo,
                                              int nObjWidth, int nObjHeight,
                                              int nImgWidth, int nImgHeight );

  /*-------------------------------*/
  /* Refinement (Uniform Sampling) */
  /*-------------------------------*/

  //void refineHypotheses          ( bool bDisplayResults=true );
  //void refineHypothesesMultiScale( bool bDisplayResults=true );

  /*----------------------*/
  /* Supporting Functions */
  /*----------------------*/
  vector<list<HoughVote> > splitUpCueVotes( const list<HoughVote> &vVotes);

  void showSupportingPatches     ( int page, int x, int y );
  void showSupportingPatches     ( unsigned nIdx, FeatureVector fvWindowPos, 
                                   bool bDrawMaps=false );

  void displayOccScaleFootprint  ();

  /*----------*/
  /* File I/O */
  /*----------*/
  /* routines from detector-fileio.cc */
  void saveFVMatlab              ( unsigned nIdx=0 );
  void saveClustersMatlab        ( unsigned nIdx=0 );
  void savePatchActivationsMatlab( unsigned nIdx=0 );
  void saveClusterImages         ( unsigned nIdx=0 );

public:
  /* Image information */
  int                 m_nImgWidth;
  int                 m_nImgHeight;
  int                 m_nVotingBinSize;

  unsigned               m_nNumCues;
  vector<string>         m_vCBNames;
  vector<FeatureCue>     m_vCues;
  vector<SharedCodebook> m_vCodebooks;
  vector<ISM>            m_vISMReco;
  ISM                    m_ismMultiCue;
  ISM                    m_ismMultiCueLeft;

  vector<PointVector>            m_vvPointsInside;
  //vector<vector<FeatureVector> > m_vvFeatures;
  vector<SharedMatchingInfo>     m_vMatchResults;

  vector<PointVector>            m_vvPointsInsideLeft;
  //vector<vector<FeatureVector> > m_vvFeaturesLeft;
  vector<SharedMatchingInfo>     m_vMatchResultsLeft;

  vector<MatchingParams>  m_vParMatching;

  RecoParams   m_parReco;

  int    m_nFeatureType;

  /* Multi-cue combination */
  int    m_nCombiMethod;

  /* Camera calibration data */
  Calibration m_calCamera;
  bool        m_bCalibAvailable;

  /* Default directories */
  QString m_qsDirCodebooks;
  QString m_qsDirResults;

protected:
  DetectorGUI* m_guiDetect;

  CBTable*    m_pmActiveCBs;
  MatchTable* m_pmActiveMatches;
  MatchTable* m_pmActiveMatchesL;
};


/****************************/
/*   Associated Functions   */
/****************************/
void getRealObjSize( Calibration &calCamera, const Hypothesis& hypo,
                     float &dDist, float &dSize );
 
void getRealObjSize( Calibration &calCamera, float x, float y1, float y2,
                     float &dDist, float &dSize );

#endif
