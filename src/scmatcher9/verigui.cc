/*********************************************************************/
/*                                                                   */
/* FILE         verigui.cc                                           */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      GUI for the verification stage parameters.           */
/*                                                                   */
/* BEGIN        Wed Mar 23 2005                                      */
/* LAST CHANGE  Wed Mar 23 2005                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <stdlib.h>

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

#include <qtmacros.hh>

#include "verigui.hh"

/*===================================================================*/
/*                        Class VeriGUI                              */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/
VeriGUI::VeriGUI( QWidget *parent, const char *name )
  : QWidget( parent, name )
{
  QVBoxLayout *menurightbox  = new QVBoxLayout( this );
  
  /***********************************/
  /*   Tab Widget for Verification   */
  /***********************************/
  menurightbox->addSpacing(5);
  QTabWidget *tabVeri = new QTabWidget( this, "veri" );

  /* tabs: general, harris, chamfer */
  QWidget     *tabwVerif     = new QWidget( this, "tabwVerif" );
  QWidget     *tabwHarris    = new QWidget( this, "tabwHarris" );
  QWidget     *tabwChamfer   = new QWidget( this, "tabwChamfer" );
  QVBoxLayout *tabpVerif     = new QVBoxLayout( tabwVerif );
  QVBoxLayout *tabpHarris    = new QVBoxLayout( tabwHarris );
  QVBoxLayout *tabpChamfer   = new QVBoxLayout( tabwChamfer );
  tabpVerif->setSpacing( 0 );
  tabpHarris->setSpacing( 0 );
  tabpChamfer->setSpacing( 0 );

  /************************/
  /* Group 'Verification' */
  /************************/
  tabpVerif->addSpacing( 5 );

  /*------------------------------*/
  /* Check box to do verification */
  /*------------------------------*/
  QCheckBox   *chkDoVerif = new QCheckBox( "Do Verification", 
                                         tabwVerif, "doverif" );
  chkDoVerif->setChecked( false );
  m_bDoVerif = chkDoVerif->isChecked();
  QT_CONNECT_CHECKBOX( chkDoVerif, DoVerif );

  tabpVerif->addWidget( chkDoVerif );

  /*---------------------*/
  /* Verification method */
  /*---------------------*/
  QButtonGroup *bgVerif = new QButtonGroup( "Verification Strategy:", 
                                            tabwVerif, "bgVerif");
  bgVerif->setColumnLayout( 1, Qt::Horizontal );
  tabpVerif->addWidget( bgVerif );

  selHarris    = new QRadioButton( "Harris", bgVerif, "selHarris" );
  selChamfer   = new QRadioButton( "Chamfer", bgVerif, "selChamfer" );

  selChamfer->setChecked( true );
  m_nVerifMethod = VERI_CHAMFER;
  QT_CONNECT_RADIOBUTTON( bgVerif, VerifMethod );


  /*************************/
  /* Group 'Harris Verif.' */
  /*************************/
  tabpHarris->addSpacing( 5 );

  /*---------------------------*/
  /* Load Codebook/Occurrences */
  /*---------------------------*/
  //QHBox *hbVeri = new QHBox( tabwVerif );
  //QPushButton *loadVCB  = new QPushButton( "Load CB", hbVeri, "ldvcb" );
  //QPushButton *loadVOcc = new QPushButton( "Load Occ", hbVeri, "ldvocc" );

  //tabpVerif->addWidget( hbVeri );
  
  //connect( loadVCB, SIGNAL(clicked()), this, SLOT(loadVerifCodebook()));
  //connect( loadVOcc, SIGNAL(clicked()), this, SLOT(loadVerifOccurrences()));

  /*-----------------*/
  /* Reco Parameters */
  /*-----------------*/
  QButtonGroup *bgHarris = new QButtonGroup( "Harris Verif.:", 
                                            tabwHarris, "bgHarris");
  bgHarris->setColumnLayout( 2, Qt::Horizontal );
  tabpHarris->addWidget( bgHarris );

  QLabel      *labVerThS        = new QLabel( "Thresh Single:", bgHarris );
  QLineEdit   *edVerThS         = new QLineEdit( "2.00", bgHarris, "edVerThS");
  QLabel      *labVerMinPFig    = new QLabel( "Reject Savings<", bgHarris );
  QLineEdit   *edVerMinPFig     = new QLineEdit( "80", bgHarris, "edverpfig");
  QLabel      *labVerWeightPFig = new QLabel( "    K2/K0", bgHarris );
  QLineEdit   *edVerWeightPFig  = new QLineEdit( "1.0", bgHarris,"edverwpfig");

  edVerThS->setMaximumWidth(50);
  edVerMinPFig->setMinimumWidth(40);
  edVerMinPFig->setMaximumWidth(50);
  edVerWeightPFig->setMinimumWidth(50);
  edVerWeightPFig->setMaximumWidth(50);
  m_dVerThreshSingle = atof( edVerThS->text() );
  m_dVerMinPFig      = atof( edVerMinPFig->text() );
  m_dVerWeightPFig   = atof( edVerWeightPFig->text() );

  QT_CONNECT_LINEEDIT( edVerThS, VerThreshSingle ); 
  QT_CONNECT_LINEEDIT( edVerMinPFig, VerMinPFig ); 
  QT_CONNECT_LINEEDIT( edVerWeightPFig, VerWeightPFig ); 

  /*--------------------------*/
  /* Check box to do hist. eq */
  /*--------------------------*/
  QCheckBox   *chkVerHistEq = new QCheckBox( "Hist. Eq.", bgHarris, "verhq" );
  QLabel      *labVerHistEq = new QLabel( "", bgHarris );

  chkVerHistEq->setChecked( false );
  m_bVerHistEq = chkVerHistEq->isChecked();
  QT_CONNECT_CHECKBOX( chkVerHistEq, VerHistEq );


  /**************************/
  /* Group 'Chamfer Verif.' */
  /**************************/
  tabpChamfer->setSpacing( 5 );

  /*--------------------*/
  /* Chamfer Parameters */
  /*--------------------*/
  QButtonGroup *bgVerChamf = new QButtonGroup( "Chamfer Verif.:", 
                                            tabwChamfer, "bgChamfer");
  bgVerChamf->setColumnLayout( 2, Qt::Horizontal );
  tabpChamfer->addWidget( bgVerChamf );

  QLabel      *labVerChAlpha    = new QLabel( "Comb. Alpha:", bgVerChamf );
  QLineEdit   *edVerChAlpha     = new QLineEdit( "0.45", bgVerChamf, "edVCAl");
  QLabel      *labVerChThresh   = new QLabel( "Min. Score", bgVerChamf );
  QLineEdit   *edVerChThresh    = new QLineEdit( "65.0", bgVerChamf, "edVCTh");

  edVerChAlpha->setMaximumWidth(50);
  edVerChThresh->setMinimumWidth(40);
  edVerChThresh->setMaximumWidth(50);
  m_dVerChAlpha  = atof( edVerChAlpha->text() );
  m_dVerChThresh = atof( edVerChThresh->text() );

  QT_CONNECT_LINEEDIT( edVerChAlpha, VerChAlpha ); 
  QT_CONNECT_LINEEDIT( edVerChThresh, VerChThresh ); 

  /*-------------------------*/
  /* Template rescale factor */
  /*-------------------------*/
  QLabel      *labVCTemplSc  = new QLabel( "Rescale Silh.:", bgVerChamf);
  QLineEdit   *edVCTemplSc   = new QLineEdit( "0.50", bgVerChamf, "edvcresc");

  edVCTemplSc->setMaximumWidth(50);
  m_dTemplateScale = atof( edVCTemplSc->text() );

  QT_CONNECT_LINEEDIT( edVCTemplSc, TemplateScale ); 


  /*********************************************/
  /*   Finish the tab Widget for Recognition   */
  /*********************************************/
  tabpVerif->addStretch( 50 );
  tabpHarris->addStretch( 50 );
  tabpChamfer->addStretch( 50 );

  tabVeri->addTab( tabwVerif,   "Verif."  );
  tabVeri->addTab( tabwHarris,  "Harris" );
  tabVeri->addTab( tabwChamfer, "Chamfer" );

  tabVeri->setCurrentPage( 0 );

  menurightbox->addWidget( tabVeri );

}

QT_IMPLEMENT_CHECKBOX( VeriGUI::slot, DoVerif, m_bDoVerif )
QT_IMPLEMENT_RADIOBUTTON( VeriGUI::slot, VerifMethod, m_nVerifMethod )

QT_IMPLEMENT_LINEEDIT_FLOAT( VeriGUI::slot, VerThreshSingle, m_dVerThreshSingle, 2 )
QT_IMPLEMENT_LINEEDIT_FLOAT( VeriGUI::slot, VerMinPFig, m_dVerMinPFig, 0 )
QT_IMPLEMENT_LINEEDIT_FLOAT( VeriGUI::slot, VerWeightPFig, m_dVerWeightPFig, 2 )
QT_IMPLEMENT_CHECKBOX( VeriGUI::slot, VerHistEq, m_bVerHistEq )

QT_IMPLEMENT_LINEEDIT_FLOAT( VeriGUI::slot, VerChAlpha, m_dVerChAlpha, 1 )
QT_IMPLEMENT_LINEEDIT_FLOAT( VeriGUI::slot, VerChThresh, m_dVerChThresh, 2 )

QT_IMPLEMENT_LINEEDIT_FLOAT( VeriGUI::slot, TemplateScale, m_dTemplateScale, 2 )
