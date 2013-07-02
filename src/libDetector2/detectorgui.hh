/*********************************************************************/
/*                                                                   */
/* FILE         detectorgui.hh                                       */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@vision.ee.ethz.ch                              */
/*                                                                   */
/* CONTENT      GUI for the detector parameters.                     */
/*                                                                   */
/* BEGIN        Thu Feb 09 2006                                      */
/* LAST CHANGE  Thu Feb 09 2006                                      */
/*                                                                   */
/*********************************************************************/

#ifndef DETECTORGUI_HH
#define DETECTORGUI_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

#include <qwidget.h>
#include <qstring.h>
#include <qimage.h>
#include <qradiobutton.h>
#include <qtable.h>
#include <qcolor.h>

#include "cuewidget.hh"

/*******************/
/*   Definitions   */
/*******************/

const int    NUM_COLORS      = 14;
const QColor COL_DETECTION[] = { Qt::green, Qt::yellow, Qt::blue,
                                 Qt::red, Qt::cyan, Qt::magenta, 
                                 Qt::white, Qt::darkGreen, Qt::darkYellow,
                                 Qt::darkBlue, Qt::darkRed, Qt::darkCyan, 
                                 Qt::darkMagenta, Qt::gray };
const QString COL_COLORNAME[]= { "green", "yellow", "blue", "red", "cyan", 
                                 "magenta", "white", "dk grn", "dk ylw", 
                                 "dk blue", "dk red", "dk cn", "dk mag", 
                                 "gray" };

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                           Class DetectorGUI                       */
/*===================================================================*/

class DetectorGUI: public QWidget
{
  Q_OBJECT
public:
  DetectorGUI( QWidget *parent=0, const char *name=0 );
  
public slots:
  /**************************/
  /*   Interface Handlers   */
  /**************************/
  void slotSetCategName           ( const QString &text );
  void slotSetPoseName            ( const QString &text );
  void slotSetAreaFactor          ( const QString &text );
  void slotSetRealHeight          ( const QString &text );
  void slotSetHeightVar           ( const QString &text );
  void slotSetDistCenter          ( const QString &text );
  void slotUpdateCategName();
  void slotUpdatePoseName();
  void slotUpdateAreaFactor();
  void slotUpdateRealHeight();
  void slotUpdateHeightVar();
  void slotUpdateDistCenter();

  void slotSetUseHeightVarOnOff   ( int   state );
  void slotSetBothDirOnOff        ( int   state );
  void slotSetApplyGPOnOff        ( int   state );

signals:
  /**************************/
  /*   Interface Handlers   */
  /**************************/
  void sigCategNameChanged        ( const QString& );
  void sigPoseNameChanged         ( const QString& );
  void sigAreaFactorChanged       ( const QString& );
  void sigRealHeightChanged       ( const QString& );
  void sigHeightVarChanged        ( const QString& );
  void sigDistCenterChanged       ( const QString& );
  void sigAddCueClicked           ();
  void sigAddCueClicked           ( string, string, bool );

public:
  /*******************************/
  /*   Content Access Operators  */
  /*******************************/
  void setDefaultDir( const string &sDirCodebooks );

public slots:
  /*************************/
  /*   Regular Functions   */
  /*************************/
  void clear();
  void saveParams( string sFileName );
  void loadParams( string sFileName, bool bVerbose=true );

  /*--------------*/
  /* Loading Cues */
  /*--------------*/
  void addCue();

  void cueTableClicked ( int nRow, int nCol, int button, 
                         const QPoint& mousePos );
  void showCueWindow   ( int nIdx );
  void addCueTableEntry( string sDetector, string sFeature, 
                         string sNumCl, string sNumOcc );
public:
  /***************************/
  /*   Detector Parameters   */
  /***************************/
  QCheckBox   *chkApplyGP;
  QCheckBox   *chkBothDir;
  QCheckBox   *chkHeightVar;

  /* Detector type */
  int     m_nCateg;
  QString m_qsCategName;
  int     m_nPose;
  QString m_qsPoseName;

  float   m_dAreaFactor;
  float   m_dRealHeight;
  bool    m_bUseHeightVar;
  float   m_dHeightVar;
  float   m_dDistCenter;
  bool    m_bApplyGPFilter;

  QColor  m_qcColor;
  QString m_qsColorName;
  
  /* cue table */
  QTable             *m_tblCues;
  vector<CueWidget*>  m_vpCueWindows;

  bool   m_bProcessBothDir;

  /* Default directories */
  QString m_qsDirCodebooks;

  /* Codebook/occurrence file names */
  vector<string>      m_vCBFiles;
  vector<string>      m_vOccFiles;
};

#endif
