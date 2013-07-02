/*********************************************************************/
/*                                                                   */
/* FILE         recogui.hh                                           */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      GUI for the recognition parameters.                  */
/*                                                                   */
/* BEGIN        Thu Jan 20 2005                                      */
/* LAST CHANGE  Thu Jan 20 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef RECOGUI_HH
#define RECOGUI_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <qwidget.h>
#include <qstring.h>
#include <qimage.h>
#include <qapplication.h>
#include <qradiobutton.h>
#include <qcheckbox.h>

/*******************/
/*   Definitions   */
/*******************/
#include <votingspace.hh>

/*************************/
/*   Class Definitions   */
/*************************/

const int OBJDIM_WIDTH        = 0;
const int OBJDIM_HEIGHT       = 1;

const int MATCHWEIGHT_UNIFORM = 0;
const int MATCHWEIGHT_GIBBS   = 1;

/*===================================================================*/
/*                           Class RecoGUI                           */
/*===================================================================*/

class RecoGUI: public QWidget
{
  Q_OBJECT
public:
  RecoGUI( QWidget *parent=0, const char *name=0 );
  
public slots:
  /**************************/
  /*   Interface Handlers   */
  /**************************/
  void slotSetMSMEx             ( const QString &text );
  void slotSetMSMEy             ( const QString &text );
  void slotSetMSMEs             ( const QString &text );
  void slotSetMSMEr             ( const QString &text );
  void slotSetMSMEa             ( const QString &text );
  void slotSetMSMEp             ( const QString &text );
  void slotSetAdaptMinScale     ( const QString &text );
  void slotSetScoreThreshSingle ( const QString &text );
  void slotSetObjWidth          ( const QString &text );
  void slotSetObjHeight         ( const QString &text );
  void slotSetSearchRange       ( const QString &text );
  void slotSetScaleMin          ( const QString &text );
  void slotSetScaleMax          ( const QString &text );
  void slotSetMinVoteWeight     ( const QString &text );
  void slotSetMaxVoteWeight     ( const QString &text );
  void slotSetMaxOverlap        ( const QString &text );
  void slotSetMinPFig           ( const QString &text );
  void slotSetWeightPFig        ( const QString &text );
  void slotSetMinPFigRefined    ( const QString &text );
  void slotSetAdaptMinMDLScale  ( const QString &text );
  void slotSetRotMin            ( const QString &text );
  void slotSetRotMax            ( const QString &text );
  void slotSetAspMin            ( const QString &text );
  void slotSetAspMax            ( const QString &text );
  void slotSetGibbsConst        ( const QString &text );
  void slotUpdateMSMEx();
  void slotUpdateMSMEy();
  void slotUpdateMSMEs();
  void slotUpdateMSMEr();
  void slotUpdateMSMEa();
  void slotUpdateMSMEp();
  void slotUpdateAdaptMinScale();
  void slotUpdateScoreThreshSingle();
  void slotUpdateObjWidth();
  void slotUpdateObjHeight();
  void slotUpdateSearchRange();
  void slotUpdateScaleMin();
  void slotUpdateScaleMax();
  void slotUpdateMinVoteWeight();
  void slotUpdateMaxVoteWeight();
  void slotUpdateMaxOverlap();
  void slotUpdateMinPFig();
  void slotUpdateWeightPFig();
  void slotUpdateMinPFigRefined();
  void slotUpdateAdaptMinMDLScale();
  void slotUpdateRotMin();
  void slotUpdateRotMax();
  void slotUpdateAspMin();
  void slotUpdateAspMax();
  void slotUpdateGibbsConst();

  void slotSelectKernelType      ( int   id );
  void slotSelectFixObjDim       ( int   id );
  void slotSelectMatchWeighting  ( int   id );
  
  void slotSetAdaptiveScaleOnOff ( int   state );
  void slotSetNormScalePFig2OnOff( int   state );
  void slotSetRestrictScaleOnOff ( int   state );
  void slotSetUseFastMSMEOnOff   ( int   state );
  void slotSetExtendSearchOnOff  ( int   state );
  void slotSetNormPatchOnOff     ( int   state );
  void slotSetNormPoseOnOff      ( int   state );
  void slotSetDoMDLOnOff         ( int   state );
  void slotSetRejectOverlapOnOff ( int   state );
  void slotSetRejectPFigOnOff    ( int   state );
  void slotSetResampleHyposOnOff ( int   state );
  void slotSetRecoRotInvOnOff    ( int   state );
  void slotSetRecoverRotOnOff    ( int   state );
  void slotSetUseAspectOnOff     ( int   state );

signals:
  /**************************/
  /*   Interface Handlers   */
  /**************************/
  void sigMSMExChanged             ( const QString& );
  void sigMSMEyChanged             ( const QString& );
  void sigMSMEsChanged             ( const QString& );
  void sigMSMErChanged             ( const QString& );
  void sigMSMEaChanged             ( const QString& );
  void sigMSMEpChanged             ( const QString& );
  void sigAdaptMinScaleChanged     ( const QString& );
  void sigScoreThreshSingleChanged ( const QString& );
  void sigObjWidthChanged          ( const QString& );
  void sigObjHeightChanged         ( const QString& );
  void sigSearchRangeChanged       ( const QString& );
  void sigScaleMinChanged          ( const QString& );
  void sigScaleMaxChanged          ( const QString& );
  void sigMinVoteWeightChanged     ( const QString& );
  void sigMaxVoteWeightChanged     ( const QString& );
  void sigMaxOverlapChanged        ( const QString& );
  void sigMinPFigChanged           ( const QString& );
  void sigWeightPFigChanged        ( const QString& );
  void sigMinPFigRefinedChanged    ( const QString& );
  void sigAdaptMinMDLScaleChanged  ( const QString& );
  void sigRotMinChanged            ( const QString& );
  void sigRotMaxChanged            ( const QString& );
  void sigAspMinChanged            ( const QString& );
  void sigAspMaxChanged            ( const QString& );
  void sigGibbsConstChanged        ( const QString& );

public:
  void processEvents() { qApp->processEvents(); }

public:
  /*********************/
  /*   Parameter I/O   */
  /*********************/
  void saveParams( string sFileName, bool bVerbose=false );
  void loadParams( string sFileName, bool bVerbose=false );

public:
  /******************************/
  /*   Recognition Parameters   */
  /******************************/
  QRadioButton *selKernelHCube;
  QRadioButton *selKernelHSphere;
  QRadioButton *selObjDimW;
  QRadioButton *selObjDimH;
  QRadioButton *selMatchWtUni;
  QRadioButton *selMatchWtGibbs;
  QCheckBox    *chkRestrSc;
  QCheckBox    *chkAdaptSc;
  QCheckBox    *chkNormScPFig2;
  QCheckBox    *chkUseFastMSME;
  QCheckBox    *chkExtendSearch;
  QCheckBox    *chkMakeRotInv;
  QCheckBox    *chkRecoverRot;
  QCheckBox    *chkUseAspect;
  QCheckBox    *chkDoMDL;
  QCheckBox    *chkRejectOverlap;
  QCheckBox    *chkRejectPFig;
  QCheckBox    *chkResample;

  /* Voting parameters */
  bool   m_bEstimatePose;
  bool   m_bNormPerPatch;
  bool   m_bNormPerPose;
  bool   m_bRestrictScale;
  bool   m_bDrawAvgPatches;
  float  m_dMinVoteWeight;
  float  m_dMaxVoteWeight;
  int    m_nMatchWeighting;
  float  m_dGibbsConst;
  
  /* MSME parameters */
  int    m_nKernelType;
  float  m_dMSMESizeX;
  float  m_dMSMESizeY;
  float  m_dMSMESizeS;
  float  m_dMSMESizeR;
  float  m_dMSMESizeA;
  float  m_dMSMESizeP;
  bool   m_bAdaptiveScale;
  float  m_dAdaptMinScale;
  bool   m_bNormScalePFig;
  bool   m_bNormScalePFig2;
  bool   m_bUseFastMSME;

  /* Recognition parameters */
  float  m_dScoreThreshSingle;
  int    m_nObjWidth;
  int    m_nObjHeight;
  bool   m_bExtendSearch;
  float  m_dSearchRange;
  float  m_dRecoScaleMin;
  float  m_dRecoScaleMax;

  /* Rotation parameters */
  bool   m_bRecoRotInv;
  bool   m_bRecoverRotation;
  float  m_dRecoRotMin;
  float  m_dRecoRotMax;

  /* Aspect parameters */
  bool   m_bUseAspect;
  float  m_dRecoAspMin;
  float  m_dRecoAspMax;
  int    m_nFixObjDim;

  /* MDL parameters */
  bool   m_bDoMDL;
  bool   m_bRejectOverlap;
  float  m_dMaxOverlap;
  bool   m_bRejectPFig;
  float  m_dMinPFig;
  float  m_dMinPFigRefined;
  float  m_dWeightPFig;
  float  m_dAdaptMinMDLScale;
  bool   m_bResampleHypos;
};

#endif
