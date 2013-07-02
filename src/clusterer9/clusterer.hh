/*********************************************************************/
/*                                                                   */
/* FILE         clusterer.hh                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT                                                           */
/*                                                                   */
/* BEGIN        Fri Sep 27 2002                                      */
/* LAST CHANGE  Tue Jul 15 2003                                      */
/*                                                                   */
/*********************************************************************/

#ifndef CLUSTERER_HH
#define CLUSTERER_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <qwidget.h>
#include <qstring.h>
#include <qimage.h>
#include <qiconview.h>

#include <qtmacros.hh>
#include <qtresizeimg.hh>
#include <qtclusterview.hh>
#include <opgrayimage.hh>
#include <opinterestimage.hh>
#include <featurevector.hh>
#include <cluster.hh>
#include <clrnncagglo.hh>
#include <featurecue.hh>
#include <featuregui.hh>
#include <clusterparams.hh>
#include <codebook.hh>
#include <imgdescr.hh>

/*******************/
/*   Definitions   */
/*******************/
const string STR_NAME    = "Clusterer";
const string STR_VERSION = "3.5";

//const float FACTOR_SIM_EDGESIFT = 800.0;  
// chosen s.t. the similarity threshold for clustering can be left at 0.7



/* specially adapted for the test experiment */
// #define DIR_IMAGES         "/home/edi/IMAGES"
// #define DIR_CL_RESULTS     "/home/edi/DATA"
// #define DIR_CB_SAVED       "/home/edi/DATA"
// #define DIR_CL_FVECS       "/home/edi/DATA"
// #define DIR_SAVED_IMAGES   "/home/edi/DATA"
#define DIR_IMAGES         "/root/Download/VOCdevkit"
#define DIR_CL_RESULTS     "/home/bleibe/projects/patches/experiments-cvpr07/codebooks"
#define DIR_CB_SAVED       "/home/bleibe/projects/patches/experiments-cvpr07/codebooks"
#define DIR_CL_FVECS       "/home/bleibe/projects/patches/experiments-cvpr07/codebooks"
#define DIR_SAVED_IMAGES   "/home/bleibe/projects/patches/experiments-cvpr07/results"
#define SSLEVEL            4
#define SSMINSIZE          9 // minimal size of image patch

const int OBJDIM_WIDTH  = 0;
const int OBJDIM_HEIGHT = 1;

class Clusterer: public QWidget
{
  Q_OBJECT
public:
  Clusterer( QWidget *parent=0, const char *name=0 );
  
public slots:
  /**************************/
  /*   Interface Handlers   */
  /**************************/
  //void slotSetClutterThresh     ( const QString &text );
  void slotSetObjWidth          ( const QString &text );
  void slotSetObjHeight         ( const QString &text );
  //void slotUpdateClutterThresh();
  void slotUpdateObjWidth();
  void slotUpdateObjHeight();

  void slotSelectFixObjDim      ( int   id );

  void slotSetUsePatchesOnOff   ( int   state );

  /*************************/
  /*   Regular Functions   */
  /*************************/
  /*----------------*/
  /* Loading Images */
  /*----------------*/
  void loadImage      (); 
  void loadImage      ( QString name );
  void loadImageBBox  ( QString qsName, const Rect &rBBox );
  void loadImageSet   ();
  void loadIDLImageSet();
  void loadIDLImageSet( const QString& qsIdlFile );
  
  /*--------------------*/
  /* Extracting Patches */
  /*--------------------*/
  void processImage               ( QString qstr );
  void processImageBBox           ( QString qstr, const Rect &rBBox );
  void collectPatches             ( bool process=false );

  void drawInterestPoints         ();
  void drawInterestPointsEllipse  ();
  void displayPatchesForBrowsing  ( vector<OpGrayImage> &vPatches );

  /*------------*/
  /* Clustering */
  /*------------*/
  void clusterPatches();
  void remove1PatchClusters();
  void removeClutteredPatches();
  
  void recreateCodebookFromTrace();

  void loadCodebook  ();
  void loadCodebook  ( string sFileName );
  void appendCodebook();
  void appendCodebook( string sFileName );
  void saveCodebook  ();
  void saveCodebook  ( string sFileName );
  
  /*----------------------*/
  /* Eigenspace functions */
  /*----------------------*/
  void computeFVEigenspace     ();
  void loadESMatlab            ();
  void displayEigenspace       ( const vector<FeatureVector> &vEigenVectors,
                                 const FeatureVector &fvEigenValues,
                                 const FeatureVector &fvAvgImage );

  /*----------*/
  /* File I/O */
  /*----------*/
  void savePatchesMatlab   ();
  void saveFeaturesMatlab  ();
  void saveClustersMatlab ();

  void saveFeatureLocations();
  void saveFeatureLocations( string sFileName );
  void saveFeatureClasses  ();
  void saveFeatureClasses  ( string sFileName );
  void loadFeatureClasses  ( string sFileName, 
                             vector<int> &vFeatureClass );
  void saveClusterTrace    ();

  void saveClusteredImages();

  bool readVector( string sFileName, FeatureVector &fvVector );
  bool readMatrix( string sFileName, vector<FeatureVector> &mMatrix );

  /*------------*/
  /* Parameters */
  /*------------*/
  void loadParams( bool bVerbose=false );
  void loadParams( string sFileName, bool bVerbose=false );
  void saveParams( bool bVerbose=false );
  void saveParams( string sFileName, bool bVerbose=false );
  
signals:
  //void sigClutterThreshChanged     ( const QString& );
  void sigObjWidthChanged          ( const QString& );
  void sigObjHeightChanged         ( const QString& );

  //void sigPatchResolutionChanged   ( int            );

private:

	QCheckBox    *chkMaps;
	
  QtResizeImg *rsSourceImg;
  QtResizeImg *rsResultImg;

  OpGrayImage m_grayImg;
  OpGrayImage m_grayImgMap;
  QString     m_qsLastImage;
  QString     m_qsLastIDL;
  QImage      m_img;

  vector<FeatureVector>     m_vEigenVectors;
  FeatureVector             m_fvEigenValues;
  FeatureVector             m_fvAvgImage;

  PointVector               m_vPoints;
  PointVector               m_vPointsInside;
    
  vector<int>               m_vImageClass;
  vector<int>               m_vFeatureClass;
  PointVector               m_vFeatureLoc;

  string  m_sImgNameShort;
  string  m_sImgName;
  string  m_sImgPath;
  string  m_sImgFullName;
  string  m_sFileName;
  bool    m_bPicLoaded;
  bool    m_bTestRoutineOn;
  bool    m_bMapsOn;
  bool    m_bCodebook;
  int     m_nFeatureType;
  
  int     m_nClutterThresh;
  bool    m_bUsePatches;
  
  int     m_nFixObjDim;
  int     m_nObjWidth;
  int     m_nObjHeight;

  Codebook       m_cbCodebook;
  ClusterParams  m_parCluster;
  MatchingParams m_parMatching;
  FeatureCue     m_fcCue;

  QtClusterView *qClassView;
};

 

#endif
