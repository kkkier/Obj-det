/*********************************************************************/
/*                                                                   */
/* FILE         cuewidget.cc                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      GUI for the feature extraction and matching parame-  */
/*              ters of one cue.                                     */
/*                                                                   */
/* BEGIN        Tue Oct 11 2005                                      */
/* LAST CHANGE  Tue Oct 11 2005                                      */
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

#include <qtmacros.hh>

#include "cuewidget.hh"

/*===================================================================*/
/*                          Class CueWidget                          */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/
CueWidget::CueWidget( FeatureGUI* &guiFeatures, MatchingGUI* &guiMatching,
                       QWidget *parent, const char *name )
  : QWidget( parent, name )
{
  QVBoxLayout *vblContent  = new QVBoxLayout( this );
  vblContent->setSpacing(0);

  /*********************************/
  /*  GUI for Feature Extraction   */
  /*********************************/
  guiFeatures = new FeatureGUI( this, "features" );
  vblContent->addWidget( guiFeatures );

  /* propagate cue updates */
  connect( guiFeatures, SIGNAL(sigMinScaleChanged(const QString&)),
           this, SLOT(propagateCueUpdate(const QString&)) );
  connect( guiFeatures, SIGNAL(sigMaxScaleChanged(const QString&)),
           this, SLOT(propagateCueUpdate(const QString&)) );

  /***********************/
  /*  GUI for Matching   */
  /***********************/
  vblContent->addSpacing(10);
  QButtonGroup *bgMatching = new QButtonGroup( "Matching:", this, "bgmatch" );
  bgMatching->setColumnLayout( 1, Qt::Horizontal );
  guiMatching = new MatchingGUI( bgMatching, "matching" );

  //vblContent->addWidget( guiMatching );
  vblContent->addWidget( bgMatching );
    
  vblContent->addStretch( 50 );
}
