//
// C++ Interface: patchmatching-gui
//
// Description: 
//
//
// Author: Edgar Seemann,-,-,- <edi@tahiti>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef PATCHMATCHINGGUI_H
#define PATCHMATCHINGGUI_H

#include <qwidget.h>
#include <qcheckbox.h>
#include <qradiobutton.h>

const int CMP_CORRELATION     = 0;
const int CMP_EUKLIDEAN       = 1;
const int CMP_NNSEARCH        = 2;
const int CMP_EV_DIST         = 3;
const int CMP_CHAMFER         = 4;
const int CMP_CHAMFERPLANES   = 5;

class PatchMatchingGUI : public QWidget
{
  Q_OBJECT
public:
  PatchMatchingGUI( QWidget *parent=0, const char *name=0 );
  
public slots:
  /**************************/
  /*   Interface Handlers   */
  /**************************/
  void slotSetRejectionThreshold( const QString &text );
  void slotSetEVEnergy          ( const QString &text );
  void slotSetMapThresh         ( const QString &text );
  void slotUpdateRejectionThreshold();
  void slotUpdateEVEnergy();
  void slotUpdateMapThresh();

  void slotSelectCompMethod      ( int   id );

  void slotSetChamferSymOnOff    ( int   state );
  void slotSetMapsOnOff          ( int   state );
  void slotSetDrawConfOnOff      ( int   state );
  void slotSetMapThreshOnOff     ( int   state );
  void slotSetDrawTightBBOnOff   ( int   state );
  void slotSetPatchSizeFactOnOff ( int   state );

  void saveParams(const QString& filename);
  void loadParams(const QString& filename);
  /**************************/
  /*   Interface Handlers   */
  /**************************/
signals:
  void sigRejectionThresholdChanged( const QString& );
  void sigEVEnergyChanged          ( const QString& );
  void sigMapThreshChanged         ( const QString& );

private:
  QCheckBox* chkMaps;
  QCheckBox* chkDrawConf;
  QCheckBox* chkMapThresh;
  QCheckBox* chkPSFact;
  QCheckBox* chkTightBB;

  QRadioButton *selCorrel;
  QRadioButton *selEuklid;
  QRadioButton *selNNSearch;
  QRadioButton *selEVMatch;
  QRadioButton *selChamfer;
  QRadioButton *selChamferOrientation;
    
public:
  /* Feature comparison parameters */
  int    m_nCompareSelect;
  bool   m_bChamferSymDist;
  float  m_dEVEnergy;
  float  m_dRejectionThresh;
  
  /* Segmentation parameters */
  bool   m_bMapsOnManually;
  bool   m_bDrawConfidence;
  bool   m_bUseMapThresh;
  float  m_dMapThresh;
  bool   m_bPatchSizeFactor;
  bool   m_bDrawTightBB;
};

#endif
