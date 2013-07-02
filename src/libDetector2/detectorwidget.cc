/*********************************************************************/
/*                                                                   */
/* FILE         detectorwidget.cc                                    */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@vision.ee.ethz.ch                              */
/*                                                                   */
/* CONTENT      GUI for the detector and its recognition parameters. */
/*                                                                   */
/* BEGIN        Fri Feb 10 2006                                      */
/* LAST CHANGE  Fri Feb 10 2006                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <qwidget.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qcolor.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qhbox.h>

#include <qtmacros.hh>

#include "detectorwidget.hh"

/*===================================================================*/
/*                       Class DetectorWidget                        */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/
DetectorWidget::DetectorWidget( DetectorGUI* &guiDetect, RecoGUI* &guiReco,
                                QWidget *parent, const char *name )
  : QWidget( parent, name )
{
  QVBoxLayout *vblContent  = new QVBoxLayout( this );
  vblContent->setSpacing(0);

  /***********************/
  /*  GUI for Detector   */
  /***********************/
  guiDetect = new DetectorGUI( this, "detector" );
  vblContent->addWidget( guiDetect );
  m_guiDetect = guiDetect;

  /**************************/
  /*  GUI for Recognition   */
  /**************************/
  vblContent->addSpacing(10);
  guiReco = new RecoGUI( this, "reco" );
  vblContent->addWidget( guiReco );
  m_guiReco = guiReco;

  /*-------------------------------------*/
  /*   Make a button 'Load/Save/Clear'   */
  /*-------------------------------------*/
  vblContent->addSpacing(10);
  QHBox *hbFileIO = new QHBox( this, "hbFileIO" );
  vblContent->addWidget( hbFileIO );

  QPushButton *pbLoad  = new QPushButton( "Load", hbFileIO, "load" );
  QPushButton *pbSave  = new QPushButton( "Save", hbFileIO, "save" );
  QPushButton *pbClear = new QPushButton( "Clear", hbFileIO, "clear" );

  connect( pbLoad, SIGNAL(clicked()), this, SLOT(loadDetector()) );
  connect( pbSave, SIGNAL(clicked()), this, SLOT(saveDetector()) );
  connect( pbClear, SIGNAL(clicked()), this, SLOT(clearDetector()) );

  vblContent->addStretch( 50 );
}


void DetectorWidget::hideEvent( QHideEvent *e )
{
  emit sigDetectorChanged();
}


void DetectorWidget::closeEvent( QCloseEvent *e )
{
  emit sigDetectorChanged();
  e->accept();
}


void DetectorWidget::loadDetector()
{
  emit sigLoadDetectorClicked();
}


void DetectorWidget::saveDetector()
{
  emit sigSaveDetectorClicked();
}


void DetectorWidget::clearDetector()
{
  emit sigClearDetectorClicked();
}

