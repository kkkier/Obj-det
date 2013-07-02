/*********************************************************************/
/*                                                                   */
/* FILE         featuregui.hh                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      GUI for the feature extraction parameters.           */
/*                                                                   */
/* BEGIN        Thu Mar 25 2004                                      */
/* LAST CHANGE  Tue Sep 20 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef FEATUREGUI_HH
#define FEATUREGUI_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <string>

#include <qwidget.h>
#include <qstring.h>
#include <qimage.h>
#include <qradiobutton.h>
#include <qcheckbox.h>

//#include <patchmatching.hh>

/*******************/
/*   Definitions   */
/*******************/
const int PATCHEXT_UNIFORM    = 0;
const int PATCHEXT_HARRIS     = 1;
const int PATCHEXT_HARLAP     = 2;
const int PATCHEXT_HESLAP     = 3;
const int PATCHEXT_LDOG       = 4;
const int PATCHEXT_EXACTDOG   = 5;
const int PATCHEXT_HARAFF     = 6;
const int PATCHEXT_HESAFF     = 7;
const int PATCHEXT_MSER       = 8;
const int PATCHEXT_HARHESLAP  = 9;
const int PATCHEXT_MSER2      = 10;
const int PATCHEXT_SURF       = 11;
const int PATCHEXT_EDGELAP    = 12;

const string NAMES_PATCHEXT[] = { "uniform", "Harris", "HarLap", "HesLap",
                                  "LDoG", "eDoG", "HarAff", "HesAff",
                                  "MSER", "HarHesLap", "MSER2", "SURF",
                                  "EdgeLap" };

const int FEATURE_PATCH       = 0;
const int FEATURE_PATCHMIKO   = 1;
const int FEATURE_STEERABLE   = 2;
const int FEATURE_SIFT        = 3;
const int FEATURE_GLOH        = 4;
const int FEATURE_MOMENTS     = 5;
const int FEATURE_DIFFINV     = 6;
const int FEATURE_COMPLEX     = 7;
const int FEATURE_SHAPECONTEXT= 8;
const int FEATURE_SPINIMGS    = 9;
const int FEATURE_GRADIENTPCA = 10;
const int FEATURE_PATCHMIKO2  = 11;
const int FEATURE_SURF64      = 12;
const int FEATURE_SURF128     = 13;

const string NAMES_FEATURE[]  = { "Patch", "Patch2", "SF", "SIFT", "GLOH",
                                  "Mom", "DI", "CF", "SC", "Spin", "GPCA",
                                  "Patch3", "SURF64", "SURF128" };

const int NORM_NONE           = 0;
const int NORM_STDDEV         = 1;
const int NORM_VARIANCE       = 2;
const int NORM_ENERGY         = 3;


class FeatureGUI: public QWidget
{
  Q_OBJECT
public:
  FeatureGUI( QWidget *parent=0, const char *name=0 );
  
public slots:
  /**************************/
  /*   Interface Handlers   */
  /**************************/
  void slotSetSigma1            ( const QString &text );
  void slotSetSigma2            ( const QString &text );
  void slotSetAlpha             ( const QString &text );
  void slotSetThreshHar         ( const QString &text );
  void slotSetScaleOctaves      ( const QString &text );
  void slotSetLevsPerOctave     ( const QString &text );
  void slotSetScaleSigma0       ( const QString &text );
  void slotSetThreshEdog        ( const QString &text );
  void slotSetThreshSURF        ( const QString &text );
  void slotSetPatchSize         ( const QString &text );
  void slotSetMinScale          ( const QString &text );
  void slotSetMaxScale          ( const QString &text );
  //void slotSetScaleFact         ( const QString &text );
  void slotSetStepSize          ( const QString &text );
  void slotSetMinFigurePixels   ( const QString &text );
  void slotSetFigureThresh      ( const QString &text );
  void slotUpdateSigma1();
  void slotUpdateSigma2();
  void slotUpdateAlpha();
  void slotUpdateThreshHar();
  void slotUpdateScaleOctaves();
  void slotUpdateLevsPerOctave();
  void slotUpdateScaleSigma0();
  void slotUpdateThreshEdog();
  void slotUpdateThreshSURF();
  void slotUpdatePatchSize();
  void slotUpdateMinScale();
  void slotUpdateMaxScale();
  //void slotUpdateScaleFact();
  void slotUpdateStepSize();
  void slotUpdateMinFigurePixels();
  void slotUpdateFigureThresh();
  
  void slotSelectPatchExtMethod  ( int   id );
  void slotSelectFeatureType     ( int   id );
  void slotSelectNormalizeMethod ( int   id );

  void slotSetUseFigureOnlyOnOff    ( int   state );
  void slotSetFilterPatchesOnOff    ( int   state );
  void slotSetMakeRotInvOnOff       ( int   state );

  void saveParams( string sFileName, bool bVerbose=false );
  void loadParams( string sFileName, bool bVerbose=false );
  
signals:
  /**************************/
  /*   Interface Handlers   */
  /**************************/
  void sigSigma1Changed            ( const QString& );
  void sigSigma2Changed            ( const QString& );
  void sigAlphaChanged             ( const QString& );
  void sigThreshHarChanged         ( const QString& );
  void sigScaleOctavesChanged      ( const QString& );
  void sigLevsPerOctaveChanged     ( const QString& );
  void sigScaleSigma0Changed       ( const QString& );
  void sigThreshEdogChanged        ( const QString& );
  void sigThreshSURFChanged        ( const QString& );
  void sigPatchSizeChanged         ( const QString& );
  void sigMinScaleChanged          ( const QString& );
  void sigMaxScaleChanged          ( const QString& );
  //void sigScaleFactChanged         ( const QString& );
  void sigStepSizeChanged          ( const QString& );
  void sigMinFigurePixelsChanged   ( const QString& );
  void sigFigureThreshChanged      ( const QString& );

private:
	QRadioButton* selPatches;
	QRadioButton* selPatchMiko;
	QRadioButton* selSteerable;
	QRadioButton* selSIFT;
	QRadioButton* selGLOH;
	QRadioButton* selMoments;
	QRadioButton* selDiffInv;
	QRadioButton* selComplex;
	QRadioButton* selShapeContext;
	QRadioButton* selSpinImgs;
	QRadioButton* selGradPCA;
	QRadioButton* selPatchMiko2;
	QRadioButton* selSURF64;
  QRadioButton* selSURF128;

	QCheckBox*    chkUseFigureOnly;
  QCheckBox*    chkFilterPatches;
	QCheckBox*    chkRotInv;
	QCheckBox*    chkColorCanny;
	QCheckBox*    chkNormOri;
	QCheckBox*    chkUse16dEdge;
	
	QRadioButton* selUniform;
	QRadioButton* selHarris;
  QRadioButton* selHarLap;
  QRadioButton* selHesLap;
  QRadioButton* selHarHesLap;
  QRadioButton* selLDoG;
  QRadioButton* selExactDoG;
  QRadioButton* selHarAff;
  QRadioButton* selHesAff;
  QRadioButton* selMSER;
  QRadioButton* selMSER2;
  QRadioButton* selSURF;
  QRadioButton* selEdgeLap;
	
public:
  /*************************************/
  /*   Feature Extraction Parameters   */
  /*************************************/
  int    m_nPatchExtMethod;
  int    m_nFeatureType;
  
  /* interest point parameters */
  float  m_dSigma1;
  float  m_dSigma2;
  float  m_dAlpha;
  float  m_dThreshHar;
  int    m_nScaleOctaves;
  int    m_nLevsPerOctave;
  float  m_dScaleSigma0;
  float  m_dThreshEdog;
  float  m_dThreshSURF;

  /* patch extraction parameters */
  int    m_nPatchSize;
  int    m_nPatchResolution;
  float  m_dMinScale;
  float  m_dMaxScale;
  float  m_dScaleFactor;
  int    m_nStepSize;
  bool   m_bFilterPatches;
  
  bool   m_bUseFigureOnly;
  bool   m_bMakeRotInv;

  int    m_nMinFigurePixels;
  int    m_nFigureThresh;

  int    m_nNormalizeMethod;
};

#endif 
