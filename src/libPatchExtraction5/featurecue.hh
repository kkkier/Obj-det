/*********************************************************************/
/*                                                                   */
/* FILE         featurecue.hh                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Class for feature extraction.                        */
/*                                                                   */
/* BEGIN        Thu Mar 25 2004                                      */
/* LAST CHANGE  Tue Sep 20 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef FEATURECUE_HH
#define FEATURECUE_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>

#include <qwidget.h>
#include <qstring.h>

#include <featurevector.hh>
#include <oprgbimage.hh>
#include <opgrayimage.hh>
#include <opinterestimage.hh>

#include "featuregui.hh"

/*******************/
/*   Definitions   */
/*******************/

const string PATH_MIKO     = "~/code/bin/i686/";
const string PATH_SURF     = "~/code/bin/i686/";
const string PATH_LOWE     = "~/code/bin/i686/";
const string PATH_MATAS    = "~/code/bin/i686/";

const float SCALE_FACTOR_STD    =  3.0;
const float SCALE_FACTOR_HARRIS = 12.0;

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                         Class FeatureCue                          */
/*===================================================================*/

class FeatureCue
{
public:
  FeatureCue();
  
public:
  /*********************/
  /*   GUI Functions   */
  /*********************/
  FeatureGUI* createGUI( QWidget *parent=0, const char* name=0 );
  void        setGUI   ( FeatureGUI* pGUI ) { m_guiParams = pGUI; }
  
public:
  /**************************/
  /*   File I/O Functions   */
  /**************************/
  
public:
  /************************/
  /*   Parameter Access   */
  /************************/
  FeatureGUI* params() const;
  
public:
  /************************************/
  /*   Feature Extraction Functions   */
  /************************************/
  
  /*--------------------*/
  /* Extracting Patches */
  /*--------------------*/
  void processImage         ( const OpGrayImage&    img, 
                              const OpGrayImage&    imgMap, 
                              const QImage&         qimg,
                              int                   &nFeatureType,
                              PointVector           &vPoints,
                              PointVector           &vPointsInside,
                              vector<OpGrayImage>   &vPatches,
                              vector<FeatureVector> &vFeatures,
                              bool bVerbose=true, bool bExtractPatches=false );
  
  void processImage         ( string                sImgName,
                              const OpGrayImage&    img, 
                              const OpGrayImage&    imgMap, 
                              const QImage&         qimg,
                              int                   &nFeatureType,
                              PointVector           &vPoints,
                              PointVector           &vPointsInside,
                              vector<OpGrayImage>   &vPatches,
                              vector<FeatureVector> &vFeatures,
                              bool bVerbose=true, bool bExtractPatches=false );
  
  void getMirroredFeatures  ( const OpGrayImage&           img, 
                              const OpGrayImage&           imgMap, 
                              const PointVector&           vPointsInside,
                              const vector<FeatureVector>& vFeatures,
                              PointVector&                 vPointsLeft,
                              vector<FeatureVector>&       vFeaturesLeft,
                              bool bVerbose=false );
                                    
  void getDescriptors       ( const OpGrayImage&           img, 
                              const OpGrayImage&           imgMap, 
                              const PointVector&           vPoints,
                              PointVector&                 vPointsSampled,
                              vector<int>&                 vPtIdzs,
                              vector<FeatureVector>&       vFeatures,
                              bool bVerbose=false );
                                    

protected:
  /*------------------*/
  /* Interest Regions */
  /*------------------*/
  void collectPatches             ( string sImgName="", bool bVerbose=true,
                                    bool bExtractPatches=false );
  
  void collectUniformPoints       ();
  void collectUniformPoints       ( int x1, int y1, int x2, int y2 );
  void applyHarris                ( bool bVerbose=true );
  void applyMikolajczyk           ( int nDetector, bool bVerbose=true );
  void applyMikolajczykNew        ( int nDetector, bool bVerbose=true );
  void applyLowe                  ( bool bVerbose=true );
  void applyExactDoG              ( bool bVerbose=true );
  void applyMatas                 ( bool bVerbose=true );
  void applyMatasUnaffine         ( bool bVerbose=true );
  //void applyEdgeScaleSpace        ( bool bVerbose=true );
  //void applyContour               ( bool bVerbose=true );
  //void applyKadir                 ( bool bVerbose=true );
  void applySURF                  ( int nFeatureType,
                                    PointVector           &vPoints,
                                    vector<FeatureVector> &vResults,
                                    bool bVerbose=true );

  /*-------------*/
  /* Descriptors */
  /*-------------*/
  void extractAllPatches        ( bool bFeaturesExtracted=false,
                                  bool bVerbose=true,
                                  bool bExtractPatches=false );

  void applyMikoBoth            ( string sImgName, 
                                  int nDetector, int nFeatureType,
                                  PointVector           &vPoints,
                                  vector<FeatureVector> &vResults,
                                  bool bVerbose=true );
  void applyMikoBothNew         ( string sImgName, 
                                  int nDetector, int nFeatureType,
                                  PointVector           &vPoints,
                                  vector<FeatureVector> &vResults,
                                  bool bVerbose=true );
  void applyMikoDescriptor      ( int nFeatureType, 
                                  const PointVector     &vPoints,
                                  vector<FeatureVector> &vResults,
                                  bool bVerbose=true );
  void applyMikoDescriptorNew   ( int nFeatureType, 
                                  const PointVector     &vPoints,
                                  vector<FeatureVector> &vResults,
                                  bool bVerbose=true );
  void applySURFDescriptor      ( int nFeatureType,
                                  /*const*/ PointVector     &vPoints,
                                  vector<FeatureVector> &vResults,
                                  bool bVerbose=true );

  /*-------------------*/
  /* Service Functions */
  /*-------------------*/
  PointVector loadInterestFileMiko     ( string sFileName, bool bVerbose=true);
  PointVector loadInterestFileEllipse  ( string sFileName, 
                                         float dRescaleFact=1.0,
                                         bool bVerbose=true );
  PointVector loadInterestFileLowe     ( string sFileName,
                                         bool bVerbose=true );

  bool writeInterestFileEllipse ( string sFileName, 
                                  const PointVector &vPoints,
                                  bool bVerbose=true );

  void loadDescriptorFileEllipse( string sFileName,
                                  vector<FeatureVector> &vResults,
                                  bool bVerbose=true );
  void loadDescriptorFileEllipse( string sFileName,
                                  PointVector           &vPoints,
                                  vector<FeatureVector> &vResults,
                                  float dRescaleFact=1.0,
                                  bool bVerbose=true );
  void loadDescriptorFileSURF   ( string sFileName,
                                  vector<FeatureVector> &vResults,
                                  float dRescaleFact=1.0,
                                  bool bVerbose=true );
  void loadDescriptorFileSURF   ( string sFileName,
                                  PointVector           &vPoints,
                                  vector<FeatureVector> &vResults,
                                  float dRescaleFact=1.0,
                                  bool bVerbose=true );

public:
  void extractAffineRegion      ( OpGrayImage img, InterestPoint pt,
                                  int nPatchSize,
                                  //OpGrayImage &imgBig,
                                  //OpGrayImage &imgRot,
                                  //OpGrayImage &imgRegion,
                                  OpGrayImage &imgResult );

protected:
  
  int         m_nFeatureType;
  
  OpGrayImage m_imgSrc;
  OpGrayImage m_imgSrcMap;
  QImage      m_qimgSrc;
  
  PointVector           m_vPoints;
  PointVector           m_vPointsInside;
  vector<OpGrayImage>   m_vPatches;
  vector<FeatureVector> m_vFeatures;
  vector<int>           m_vPtIdzs;
  
public:
  FeatureGUI* m_guiParams;
};

#endif
