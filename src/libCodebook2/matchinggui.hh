/*********************************************************************/
/*                                                                   */
/* FILE         matchinggui.hh                                       */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      GUI for the codebook matching parameters.            */
/*                                                                   */
/* BEGIN        Tue Mar 22 2005                                      */
/* LAST CHANGE  Tue Mar 22 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef MATCHINGGUI_HH
#define MATCHINGGUI_HH

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
const int CMP_CORRELATION = 0;
const int CMP_EUKLIDEAN   = 1;

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                           Class MatchingGUI                       */
/*===================================================================*/

class MatchingGUI: public QWidget
{
  Q_OBJECT
public:
  MatchingGUI( QWidget *parent=0, const char *name=0 );
  
public slots:
  /**************************/
  /*   Interface Handlers   */
  /**************************/
  void slotSetRejectionThreshold( const QString &text );
  void slotSetFeatureSimFact    ( const QString &text );
  void slotUpdateRejectionThreshold();
  void slotUpdateFeatureSimFact();

  void slotSelectCompMethod      ( int   id );

signals:
  /**************************/
  /*   Interface Handlers   */
  /**************************/
  void sigRejectionThresholdChanged( const QString& );
  void sigFeatureSimFactChanged    ( const QString& );

public:
  /*********************/
  /*   Parameter I/O   */
  /*********************/
  void saveParams( string sFileName, bool bVerbose=false );
  void loadParams( string sFileName, bool bVerbose=false );

public:
  /************************************/
  /*   Codebook Matching Parameters   */
  /************************************/
  QRadioButton *selCorrel;
  QRadioButton *selEuclid;
  
  /* Feature comparison parameters */
  int    m_nCompareSelect;
  float  m_dRejectionThresh;
  float  m_dFeatureSimFact;
};

#endif
