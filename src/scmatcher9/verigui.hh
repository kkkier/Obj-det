/*********************************************************************/
/*                                                                   */
/* FILE         verigui.hh                                           */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      GUI for the verification stage parameters.           */
/*                                                                   */
/* BEGIN        Wed Mar 23 2005                                      */
/* LAST CHANGE  Wed Mar 23 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef VERIGUI_HH
#define VERIGUI_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <qwidget.h>
#include <qstring.h>
#include <qimage.h>
#include <qradiobutton.h>

/*******************/
/*   Definitions   */
/*******************/

/*************************/
/*   Class Definitions   */
/*************************/
const int VERI_HARRIS   = 0;
const int VERI_CHAMFER  = 1;


/*===================================================================*/
/*                           Class VeriGUI                           */
/*===================================================================*/

class VeriGUI: public QWidget
{
  Q_OBJECT
public:
  VeriGUI( QWidget *parent=0, const char *name=0 );
  
public slots:
  /**************************/
  /*   Interface Handlers   */
  /**************************/
  void slotSetVerThreshSingle   ( const QString &text );
  void slotSetVerMinPFig        ( const QString &text );
  void slotSetVerWeightPFig     ( const QString &text );
  void slotSetVerChAlpha        ( const QString &text );
  void slotSetVerChThresh       ( const QString &text );
  void slotSetTemplateScale     ( const QString &text );
  void slotUpdateVerThreshSingle();
  void slotUpdateVerMinPFig();
  void slotUpdateVerWeightPFig();
  void slotUpdateVerChAlpha();
  void slotUpdateVerChThresh();
  void slotUpdateTemplateScale();

  void slotSelectVerifMethod     ( int   state );

  void slotSetDoVerifOnOff       ( int   state );
  void slotSetVerHistEqOnOff     ( int   state );

signals:
  /**************************/
  /*   Interface Handlers   */
  /**************************/
  void sigVerThreshSingleChanged   ( const QString& );
  void sigVerMinPFigChanged        ( const QString& );
  void sigVerWeightPFigChanged     ( const QString& );
  void sigVerChAlphaChanged        ( const QString& );
  void sigVerChThreshChanged       ( const QString& );
  void sigTemplateScaleChanged     ( const QString& );

public:
  /*******************************/
  /*   Verification Parameters   */
  /*******************************/

  QRadioButton *selHarris;
  QRadioButton *selChamfer;

  /* General parameters */
  bool   m_bDoVerif;
  int    m_nVerifMethod;

  /* Harris verification */
  float  m_dVerThreshSingle;
  float  m_dVerMinPFig;
  float  m_dVerWeightPFig;
  bool   m_bVerHistEq;

  /* Chamfer verification */
  float  m_dVerChAlpha;
  float  m_dVerChThresh;
  float  m_dTemplateScale;
};

#endif
