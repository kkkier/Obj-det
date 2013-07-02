/*********************************************************************/
/*                                                                   */
/* FILE         recogui.cc                                           */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      GUI for the recognition parameters.                  */
/*                                                                   */
/* BEGIN        Thu Jan 20 2005                                      */
/* LAST CHANGE  Thu Jan 20 2005                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>

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
#include <qfile.h>

#include <qtmacros.hh>

#include "recogui.hh"

/*===================================================================*/
/*                        Class FeatureGUI                           */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/
RecoGUI::RecoGUI( QWidget *parent, const char *name )
  : QWidget( parent, name )
{
  QVBoxLayout *menurightbox  = new QVBoxLayout( this );
  
  /**********************************/
  /*   Tab Widget for Recognition   */
  /**********************************/
  menurightbox->addSpacing(5);
  QTabWidget *tabReco = new QTabWidget( this, "reco" );

  /* tabs: msme, reco, mdl */
  QWidget     *tabwMSME      = new QWidget( this, "tabwMSME" );
  QWidget     *tabwRecoParam = new QWidget( this, "tabwRecoParam" );
  QWidget     *tabwMDL       = new QWidget( this, "tabwMDL" );
  QWidget     *tabwMisc      = new QWidget( this, "tabwMisc" );
  QWidget     *tabwMisc2     = new QWidget( this, "tabwMisc2" );
  QVBoxLayout *tabpMSME      = new QVBoxLayout( tabwMSME );
  QVBoxLayout *tabpRecoParam = new QVBoxLayout( tabwRecoParam );
  QVBoxLayout *tabpMDL       = new QVBoxLayout( tabwMDL );
  QVBoxLayout *tabpMisc      = new QVBoxLayout( tabwMisc );
  QVBoxLayout *tabpMisc2     = new QVBoxLayout( tabwMisc2 );
  tabpMSME->setSpacing( 0 );
  tabpRecoParam->setSpacing( 0 );
  tabpMDL->setSpacing( 0 );
  tabpMisc->setSpacing( 0 );
  tabpMisc2->setSpacing( 0 );


  /**************************************/
  /*  Group 'Mean Shift Mode Estimator' */
  /**************************************/
  tabpMSME->addSpacing( 5 );
  QButtonGroup *bgMSMEWin= new QButtonGroup( "MSME Window Size:", tabwMSME, 
                                             "bgMSME");
  bgMSMEWin->setColumnLayout( 2, Qt::Horizontal );
  tabpMSME->addWidget( bgMSMEWin );
  
  QLabel      *labSizeX  = new QLabel( "X:", bgMSMEWin );
  QLineEdit   *editMSMEx = new QLineEdit( "10.0", bgMSMEWin, "editMSMEx");
  QLabel      *labSizeY  = new QLabel( "Y:", bgMSMEWin );
  QLineEdit   *editMSMEy = new QLineEdit( "10.0", bgMSMEWin, "editMSMEy");
  QLabel      *labSizeS  = new QLabel( "Scale:", bgMSMEWin );
  QLineEdit   *editMSMEs = new QLineEdit( "0.10", bgMSMEWin, "editMSMEs");
  QLabel      *labSizeR  = new QLabel( "Rot.:", bgMSMEWin );
  QLineEdit   *editMSMEr = new QLineEdit( "10.0", bgMSMEWin, "editMSMEr");
  QLabel      *labSizeA  = new QLabel( "Aspect:", bgMSMEWin );
  QLineEdit   *editMSMEa = new QLineEdit( "10.0", bgMSMEWin, "editMSMEa");
  //QLabel      *labSizeP  = new QLabel( "Pose:", bgMSMEWin );
  //QLineEdit   *editMSMEp = new QLineEdit( "1.0", bgMSMEWin, "editMSMEp");
  
  editMSMEx->setMaximumWidth(50);
  editMSMEy->setMaximumWidth(50);
  editMSMEs->setMaximumWidth(50);
  editMSMEr->setMaximumWidth(50);
  editMSMEa->setMaximumWidth(50);
  //editMSMEp->setMaximumWidth(50);
  m_dMSMESizeX = atof( editMSMEx->text() );
  m_dMSMESizeY = atof( editMSMEy->text() );
  m_dMSMESizeS = atof( editMSMEs->text() );
  m_dMSMESizeR = atof( editMSMEr->text() );
  m_dMSMESizeA = atof( editMSMEa->text() );
  m_dMSMESizeP = 1.0; //atof( editMSMEp->text() );
  
  QT_CONNECT_LINEEDIT( editMSMEx, MSMEx ); 
  QT_CONNECT_LINEEDIT( editMSMEy, MSMEy ); 
  QT_CONNECT_LINEEDIT( editMSMEs, MSMEs ); 
  QT_CONNECT_LINEEDIT( editMSMEr, MSMEr ); 
  QT_CONNECT_LINEEDIT( editMSMEa, MSMEr ); 
  //QT_CONNECT_LINEEDIT( editMSMEp, MSMEp ); 
  
  /*-------------------*/
  /* Kernel parameters */
  /*-------------------*/
  tabpMSME->addSpacing( 5 );
  QButtonGroup *bgKernel= new QButtonGroup( "MSME Kernel:", 
                                            tabwMSME, "bgKernel");
  bgKernel->setColumnLayout( 1, Qt::Horizontal );
  tabpMSME->addWidget( bgKernel );

  selKernelHCube   = new QRadioButton( "cubical", bgKernel,"selHCube" );
  selKernelHSphere = new QRadioButton( "spherical", bgKernel,"selHSphere" );
    
  selKernelHCube->setChecked( true );
  m_nKernelType = KERNEL_HCUBE;
  QT_CONNECT_RADIOBUTTON( bgKernel, KernelType );


  /*---------------------------*/
  /* Checkbox 'Adaptive Scale' */
  /*---------------------------*/
  chkAdaptSc = new QCheckBox( "Use Adaptive Scale", tabwMSME, 
                                             "chkAdaptSc" );
  chkAdaptSc->setChecked( true );
  m_bAdaptiveScale = chkAdaptSc->isChecked();
  QT_CONNECT_CHECKBOX( chkAdaptSc, AdaptiveScale );

  tabpMSME->addWidget( chkAdaptSc );

  QHBox       *hbAdaptMin   = new QHBox( tabwMSME );
  tabpMSME->addWidget( hbAdaptMin );

  QLabel      *labAdaptMin  = new QLabel( "for scales >", hbAdaptMin );
  QLineEdit   *editAdaptMin = new QLineEdit( "1.0", hbAdaptMin, "edAdaptMin");

  editAdaptMin->setMaximumWidth(50);
  m_dAdaptMinScale = atof( editAdaptMin->text() );

  QT_CONNECT_LINEEDIT( editAdaptMin, AdaptMinScale ); 

 /*--------------------------------*/
  /* Checkbox 'Normalize per Patch' */
  /*--------------------------------*/
//   QCheckBox   *chkNormPatch = new QCheckBox( "Normalize per Patch", this, 
//                                              "chkNormPatch" );
//   menurightbox->addWidget( chkNormPatch );
//   chkNormPatch->setChecked( true );
//   m_bNormPerPatch = chkNormPatch->isChecked();
//   QT_CONNECT_CHECKBOX( chkNormPatch, NormPatch );
  m_bNormPerPatch = true;
  
  /*-------------------------------*/
  /* Checkbox 'Normalize per Pose' */
  /*-------------------------------*/
//   QCheckBox   *chkNormPose = new QCheckBox( "Normalize per Pose", tabwMSME, 
//                                             "chkNormPose" );
//   chkNormPose->setChecked( true );
  m_bNormPerPose = true; //chkNormPose->isChecked();
//   QT_CONNECT_CHECKBOX( chkNormPose, NormPose );

//   tabpMSME->addWidget( chkNormPose );
  

  /***********************************/
  /*  Group 'Recognition Parameters' */
  /***********************************/
  tabpRecoParam->addSpacing( 5 );
  QButtonGroup *bgRecoParams= new QButtonGroup( "Reco Parameters:", 
                                                tabwRecoParam, "bgRecoParams");
  bgRecoParams->setColumnLayout( 2, Qt::Horizontal );
  tabpRecoParam->addWidget( bgRecoParams );
  
  QLabel      *labScoreThS     = new QLabel( "Thresh Single:", bgRecoParams );
  QLineEdit   *editScoreThS    = new QLineEdit( "2.00", bgRecoParams, 
                                                "edScoreThS");
  QLabel      *labObjWidth     = new QLabel( "Obj. Width:", bgRecoParams );
  QLineEdit   *editObjWidth    = new QLineEdit( "80", bgRecoParams, "edobw" );
  QLabel      *labObjHeight    = new QLabel( "Obj. Height:", bgRecoParams );
  QLineEdit   *editObjHeight   = new QLineEdit( "200", bgRecoParams, "edobh" );
               chkExtendSearch = new QCheckBox( "Extend Rg.:", 
                                                bgRecoParams, "chkextrg" );
  QLineEdit   *editSearchRange = new QLineEdit( "0.25", bgRecoParams, "edrg" );
  QLabel      *labScaleMin     = new QLabel( "min Scale:", bgRecoParams );
  QLineEdit   *editScaleMin    = new QLineEdit( "0.30", bgRecoParams,"edmins");
  QLabel      *labScaleMax     = new QLabel( "max Scale:", bgRecoParams );
  QLineEdit   *editScaleMax    = new QLineEdit( "1.50", bgRecoParams,"edmaxs");
  QLabel      *labMinWeight    = new QLabel( "min Vote Wt:", bgRecoParams);
  QLineEdit   *editMinWeight   = new QLineEdit( "0.0002", bgRecoParams,
                                                "edminvw");
  QLabel      *labMaxWeight    = new QLabel( "max Vote Wt:", bgRecoParams);
  QLineEdit   *editMaxWeight   = new QLineEdit( "0.05", bgRecoParams,
                                                "edmaxvw");

  editScoreThS->setMaximumWidth(50);
  editObjWidth->setMaximumWidth(50);
  editObjHeight->setMaximumWidth(50);
  editSearchRange->setMaximumWidth(50);
  editScaleMin->setMaximumWidth(50);
  editScaleMax->setMaximumWidth(50);
  editMinWeight->setMaximumWidth(50);
  editMaxWeight->setMaximumWidth(50);
  m_dScoreThreshSingle = atof( editScoreThS->text() );
  m_nObjWidth          = atoi( editObjWidth->text() );
  m_nObjHeight         = atoi( editObjHeight->text() );
  m_dSearchRange       = atof( editSearchRange->text() );
  m_dRecoScaleMin      = atof( editScaleMin->text() );
  m_dRecoScaleMax      = atof( editScaleMax->text() );
  m_dMinVoteWeight     = atof( editMinWeight->text() );
  m_dMaxVoteWeight     = atof( editMaxWeight->text() );

  chkExtendSearch->setChecked( false );
  m_bExtendSearch = chkExtendSearch->isChecked();

  QT_CONNECT_LINEEDIT( editScoreThS, ScoreThreshSingle ); 
  QT_CONNECT_LINEEDIT( editObjWidth, ObjWidth ); 
  QT_CONNECT_LINEEDIT( editObjHeight, ObjHeight ); 
  QT_CONNECT_LINEEDIT( editSearchRange, SearchRange ); 
  QT_CONNECT_LINEEDIT( editScaleMin, ScaleMin ); 
  QT_CONNECT_LINEEDIT( editScaleMax, ScaleMax ); 
  QT_CONNECT_LINEEDIT( editMinWeight, MinVoteWeight ); 
  QT_CONNECT_LINEEDIT( editMaxWeight, MaxVoteWeight ); 

  QT_CONNECT_CHECKBOX( chkExtendSearch, ExtendSearch );


  /************************************/
  /* Group 'MDL' Hypothesis Selection */
  /************************************/
  tabpMDL->addSpacing( 5 );

  /*------------------------------------------*/
  /* Check box to do MDL hypothesis selection */
  /*------------------------------------------*/
  chkDoMDL = new QCheckBox( "Do MDL Selection", tabwMDL, "domdl" );

  chkDoMDL->setChecked( true );
  m_bDoMDL = chkDoMDL->isChecked();
  QT_CONNECT_CHECKBOX( chkDoMDL, DoMDL );

  tabpMDL->addWidget( chkDoMDL );

  /*-----------------------------------------------*/
  /* Parameter fields for MDL hypothesis selection */
  /*-----------------------------------------------*/
  QButtonGroup *bgMDLParams= new QButtonGroup( "Hypothesis Selection:", 
                                               tabwMDL, "bgMDLParams");
  bgMDLParams->setColumnLayout( 2, Qt::Horizontal );
  tabpMDL->addWidget( bgMDLParams );
  
  chkRejectOverlap             = new QCheckBox( "Rej. Overl>", 
                                                bgMDLParams, "chkoverlp" );
  QLineEdit   *editOverlap     = new QLineEdit( "0.20", bgMDLParams,"edovlp");
  chkRejectPFig                = new QCheckBox( "Rej. Savings<", 
                                                bgMDLParams, "chkpfig" );
  QLineEdit   *editMinPFig     = new QLineEdit( "400", bgMDLParams,
                                                "edpfig");
  QLabel      *labWeightPFig   = new QLabel( "    K2/K0", bgMDLParams );
  QLineEdit   *editWeightPFig  = new QLineEdit( "0.95", bgMDLParams,"edwpfig");
  QLabel      *labPFigRef      = new QLabel( "  refined PFig<", bgMDLParams );
  QLineEdit   *editMinPFigRef  = new QLineEdit( "45000", bgMDLParams,
                                                "edpfigref");
  QLabel      *labAdaptMDLSc   = new QLabel( "adapt for scales >",bgMDLParams);
  QLineEdit   *editAdaptMDLSc  = new QLineEdit( "1.0", bgMDLParams,"edadmdls");

  editOverlap->setMaximumWidth(50);
  editMinPFig->setMinimumWidth(40);
  editMinPFig->setMaximumWidth(50);
  editWeightPFig->setMinimumWidth(50);
  editWeightPFig->setMaximumWidth(50);
  editMinPFigRef->setMinimumWidth(50);
  editMinPFigRef->setMaximumWidth(50);
  editAdaptMDLSc->setMinimumWidth(50);
  editAdaptMDLSc->setMaximumWidth(50);
  m_dMaxOverlap        = atof( editOverlap->text() );
  m_dMinPFig           = atof( editMinPFig->text() );
  m_dWeightPFig        = atof( editWeightPFig->text() );
  m_dMinPFigRefined    = atof( editMinPFigRef->text() );
  m_dAdaptMinMDLScale  = atof( editAdaptMDLSc->text() );

  chkRejectOverlap->setChecked( false );
  m_bRejectOverlap = chkRejectOverlap->isChecked();
  chkRejectPFig->setChecked( true );
  m_bRejectPFig = chkRejectPFig->isChecked();

  QT_CONNECT_LINEEDIT( editOverlap, MaxOverlap ); 
  QT_CONNECT_LINEEDIT( editMinPFig, MinPFig ); 
  QT_CONNECT_LINEEDIT( editWeightPFig, WeightPFig ); 
  QT_CONNECT_LINEEDIT( editMinPFigRef, MinPFigRefined ); 
  QT_CONNECT_LINEEDIT( editAdaptMDLSc, AdaptMinMDLScale ); 

  QT_CONNECT_CHECKBOX( chkRejectOverlap, RejectOverlap );
  QT_CONNECT_CHECKBOX( chkRejectPFig, RejectPFig );

  /*----------------------------------*/
  /* Check box to resample hypotheses */
  /*----------------------------------*/
  chkResample = new QCheckBox( "Resample Hypotheses", tabwMDL, "resmpl" );

  chkResample->setChecked( false );
  m_bResampleHypos = chkResample->isChecked();
  QT_CONNECT_CHECKBOX( chkResample, ResampleHypos );

  tabpMDL->addWidget( chkResample );


  /*****************************/
  /*  Group 'Misc. Parameters' */
  /*****************************/
  tabpMisc->addSpacing( 5 );
  QButtonGroup *bgRot= new QButtonGroup( "Rotation:", 
                                                tabwMisc, "bgRot");
  bgRot->setColumnLayout( 2, Qt::Horizontal );
  tabpMisc->addWidget( bgRot );
  
  chkMakeRotInv               = new QCheckBox( "Make Rot.Inv.", 
                                                bgRot, "chkrotinv" );
  QLabel      *lbRotInv       = new QLabel( "", bgRot );
  chkRecoverRot               = new QCheckBox( "Recover Rot.", 
                                                bgRot, "chkrecrot" );
  QLabel      *lbRecoverRot   = new QLabel( "", bgRot );
  QLabel      *lbRotMin       = new QLabel( "Min Rot.:", bgRot );
  QLineEdit   *edRotMin       = new QLineEdit( "-45.0", bgRot, "edRotMin");
  QLabel      *lbRotMax       = new QLabel( "Max Rot.:", bgRot );
  QLineEdit   *edRotMax       = new QLineEdit( " 45.0", bgRot, "edRotMin");

  edRotMin->setMaximumWidth(50);
  edRotMin->setMinimumWidth(40);
  edRotMax->setMaximumWidth(50);
  edRotMax->setMinimumWidth(40);
  m_dRecoRotMin = atof( edRotMin->text() );
  m_dRecoRotMax = atof( edRotMax->text() );

  chkMakeRotInv->setChecked( false );
  m_bRecoRotInv = chkMakeRotInv->isChecked();
  chkRecoverRot->setChecked( false );
  m_bRecoverRotation = chkRecoverRot->isChecked();

  QT_CONNECT_LINEEDIT( edRotMin, RotMin ); 
  QT_CONNECT_LINEEDIT( edRotMax, RotMax ); 

  QT_CONNECT_CHECKBOX( chkMakeRotInv, RecoRotInv );
  QT_CONNECT_CHECKBOX( chkRecoverRot, RecoverRot );

  /*------------------------------*/
  /* Checkbox 'Normalize SumPFig' */
  /*------------------------------*/
  tabpMisc->addSpacing( 5 );
  QButtonGroup *bgScale= new QButtonGroup( "Scale:", 
                                                tabwMisc, "bgScale");
  bgScale->setColumnLayout( 1, Qt::Horizontal );
  tabpMisc->addWidget( bgScale );
  
  chkNormScPFig2 = new QCheckBox( "Normalize PFig Scale^2", bgScale, 
                                  "chkNormSc2" );

  chkNormScPFig2->setChecked( true );
  m_bNormScalePFig2 = chkNormScPFig2->isChecked();

  QT_CONNECT_CHECKBOX( chkNormScPFig2, NormScalePFig2 );

  //tabpMisc->addWidget( chkNormScPFig2 );


  /*---------------------------*/
  /* Checkbox 'Restrict Scale' */
  /*---------------------------*/
  chkRestrSc = new QCheckBox( "Restrict Vote Scales", bgScale, "chkRestrSc" );

  chkRestrSc->setChecked( true );
  m_bRestrictScale = chkRestrSc->isChecked();
  QT_CONNECT_CHECKBOX( chkRestrSc, RestrictScale );

  //tabpMisc->addWidget( chkRestrSc );

   /*-----------------------------------------*/
  /* Checkbox 'Use Fast (approximative) MSME' */
  /*------------------------------------------*/
  chkUseFastMSME = new QCheckBox( "Use fast MSME", tabwMisc, "chkFastMSME" );

  chkUseFastMSME->setChecked( false );
  m_bUseFastMSME = chkUseFastMSME->isChecked();

  QT_CONNECT_CHECKBOX( chkUseFastMSME, UseFastMSME );

  tabpMisc->addWidget( chkUseFastMSME );


  /*****************************/
  /*  Group 'Misc2 Parameters' */
  /*****************************/
  tabpMisc2->addSpacing( 5 );
  QButtonGroup *bgAspect= new QButtonGroup( "Aspect:", 
                                            tabwMisc2, "bgAsp");
  bgAspect->setColumnLayout( 2, Qt::Horizontal );
  tabpMisc2->addWidget( bgAspect );
  
  chkUseAspect                = new QCheckBox( "Use Aspect", 
                                                bgAspect, "chkaspect" );
  QLabel      *lbAspect       = new QLabel( "", bgAspect );
  QLabel      *lbAspMin       = new QLabel( "Min Asp.:", bgAspect );
  QLineEdit   *edAspMin       = new QLineEdit( " 10.0", bgAspect, "edAspMin");
  QLabel      *lbAspMax       = new QLabel( "Max Asp.:", bgAspect );
  QLineEdit   *edAspMax       = new QLineEdit( " 80.0", bgAspect, "edAspMax");

  edAspMin->setMaximumWidth(50);
  edAspMin->setMinimumWidth(40);
  edAspMax->setMaximumWidth(50);
  edAspMax->setMinimumWidth(40);
  m_dRecoAspMin = atof( edAspMin->text() );
  m_dRecoAspMax = atof( edAspMax->text() );

  chkUseAspect->setChecked( false );
  m_bUseAspect = chkUseAspect->isChecked();

  QT_CONNECT_LINEEDIT( edAspMin, AspMin ); 
  QT_CONNECT_LINEEDIT( edAspMax, AspMax ); 

  QT_CONNECT_CHECKBOX( chkUseAspect, UseAspect );

  /*----------------------------------*/
  /* Make a field for the object size */
  /*----------------------------------*/
  QButtonGroup *bgObjSize = new QButtonGroup( "Fix Object Size:", tabwMisc2, 
                                              "bgO" );
  bgObjSize->setColumnLayout( 1, Qt::Horizontal );
  tabpMisc2->addWidget( bgObjSize );
  
  selObjDimW = new QRadioButton( "width", bgObjSize, "selObjW" );
  selObjDimH = new QRadioButton( "height", bgObjSize, "selObjH");
  
  selObjDimW->setChecked( true );
  m_nFixObjDim = OBJDIM_WIDTH;
  
  QT_CONNECT_RADIOBUTTON( bgObjSize, FixObjDim );

  /*-----------------------------------------------*/
  /* Make a field for the match weighting strategy */
  /*-----------------------------------------------*/
  QButtonGroup *bgMatchWt = new QButtonGroup( "Match Weighting:", tabwMisc2, 
                                              "bgMW" );
  bgMatchWt->setColumnLayout( 1, Qt::Horizontal );
  tabpMisc2->addWidget( bgMatchWt );
  
  selMatchWtUni   = new QRadioButton( "uniform", bgMatchWt, "selMWu" );
  selMatchWtGibbs = new QRadioButton( "Gibbs", bgMatchWt, "selMWG" );

  QHBox *hbGibbs = new QHBox( bgMatchWt );
  QLabel      *lbGibbsC = new QLabel( "with const", hbGibbs );
  QLineEdit   *edGibbsC = new QLineEdit( " 2.00", hbGibbs, "edGibbsC");

  selMatchWtUni->setChecked( true );
  m_nMatchWeighting = MATCHWEIGHT_UNIFORM;
  
  edGibbsC->setMaximumWidth(50);
  edGibbsC->setMaximumWidth(50);
  m_dGibbsConst = atof( edGibbsC->text() );

  QT_CONNECT_RADIOBUTTON( bgMatchWt, MatchWeighting );
  QT_CONNECT_LINEEDIT( edGibbsC, GibbsConst ); 


  /*********************************************/
  /*   Finish the tab Widget for Recognition   */
  /*********************************************/
  tabpMSME->addStretch( 50 );
  tabpRecoParam->addStretch( 50 );
  tabpMDL->addStretch( 50 );
  tabpMisc->addStretch( 50 );
  tabpMisc2->addStretch( 50 );

  tabReco->addTab( tabwMSME,      "MSME"  );
  tabReco->addTab( tabwRecoParam, "Reco" );
  tabReco->addTab( tabwMDL,       "MDL" );
  tabReco->addTab( tabwMisc,      "Misc" );
  tabReco->addTab( tabwMisc2,     "Misc2" );

  tabReco->setCurrentPage( 1 );

  menurightbox->addWidget( tabReco );

}


void RecoGUI::saveParams( string sFileName, bool bVerbose )
{
  QFile qfile( sFileName );
  if ( qfile.open(IO_WriteOnly | IO_Append ) )
    {
      QTextStream stream( &qfile );
      stream << "\n*** Section RecoGUI ***\n"
        //-- Voting parameters --//
             << "m_bRestrictScale: " << m_bRestrictScale << "\n"
             << "m_nMatchWeighting: " << m_nMatchWeighting << "\n"
             << "m_dGibbsConst: " << m_dGibbsConst << "\n"
        //-- MSME parameters --//
             << "m_nKernelType: " << m_nKernelType << "\n"
             << "m_dMSMESizeX: " << m_dMSMESizeX << "\n"
             << "m_dMSMESizeY: " << m_dMSMESizeY << "\n"
             << "m_dMSMESizeS: " << m_dMSMESizeS << "\n"
             << "m_dMSMESizeR: " << m_dMSMESizeR << "\n"
             << "m_dMSMESizeA: " << m_dMSMESizeA << "\n"
             << "m_dMSMESizeP: " << m_dMSMESizeP << "\n"
             << "m_bAdaptiveScale: " << m_bAdaptiveScale << "\n"
             << "m_dAdaptMinScale: " << m_dAdaptMinScale << "\n"
             << "m_bNormScalePFig2: " << m_bNormScalePFig2 << "\n"
             << "m_dMinVoteWeight: " << m_dMinVoteWeight << "\n"
             << "m_dMaxVoteWeight: " << m_dMaxVoteWeight << "\n"
             << "m_bUseFastMSME: " << m_bUseFastMSME << "\n"
        //-- Recognition parameters --//
             << "m_dScoreThreshSingle: " << m_dScoreThreshSingle << "\n"
             << "m_nObjWidth: "  << m_nObjWidth << "\n"
             << "m_nObjHeight: " << m_nObjHeight << "\n"
             << "m_bExtendSearch: " << m_bExtendSearch << "\n"
             << "m_dSearchRange: " << m_dSearchRange << "\n"
             << "m_dRecoScaleMin: " << m_dRecoScaleMin << "\n"
             << "m_dRecoScaleMax: " << m_dRecoScaleMax << "\n"
        //-- Rotation parameters --//
             << "m_bRecoRotInv: " << m_bRecoRotInv << "\n"
             << "m_bRecoverRotation: " << m_bRecoverRotation << "\n"
             << "m_dRecoRotMin: " << m_dRecoRotMin << "\n"
             << "m_dRecoRotMax: " << m_dRecoRotMax << "\n"
        //-- Aspect parameters --//
             << "m_bUseAspect: " << m_bUseAspect << "\n"
             << "m_dRecoAspMin: " << m_dRecoAspMin << "\n"
             << "m_dRecoAspMax: " << m_dRecoAspMax << "\n"
             << "m_nFixObjDim: " << m_nFixObjDim << "\n"
        //-- MDL parameters --//
             << "m_bDoMDL: " << m_bDoMDL << "\n"
             << "m_bRejectOverlap: " << m_bRejectOverlap << "\n"
             << "m_dMaxOverlap: " << m_dMaxOverlap << "\n"
             << "m_bRejectPFig: " << m_bRejectPFig << "\n"
             << "m_dMinPFig: " << m_dMinPFig << "\n"
             << "m_dMinPFigRefined: " << m_dMinPFigRefined << "\n"
             << "m_dWeightPFig: " << m_dWeightPFig << "\n"
             << "m_dAdaptMinMDLScale: " << m_dAdaptMinMDLScale << "\n"
             << "m_bResampleHypos: " << m_bResampleHypos << "\n";
      qfile.close();
    }
}


void RecoGUI::loadParams( string sFileName, bool bVerbose )
{
  QFile qfile( sFileName.c_str() );
  if ( qfile.open( IO_ReadOnly ) ) {
    QTextStream stream( &qfile );
    QString line;
    bool started=false;
    
    while(!(line = stream.readLine()).isNull())
      {
        if (!started || line.isEmpty())
          {
            if(line.compare("*** Section RecoGUI ***")==0)
              started=true;
            continue;
          }
        else if(line.left(1).compare("*")==0)
          {
            started=false;//stop if section is over
            break;
          }
        
        int len = line.length();
        int pos = line.find(':');
        if( bVerbose )
          printf("line: %s\n",line.latin1());
        
        QString name = line.left(pos);
        QString val = line.right(len-pos-2);
        
        //-- Voting parameters --//
        if (name.compare("m_bRestrictScale")==0) {
          chkRestrSc->setChecked((bool)val.toInt());
          slotSetRestrictScaleOnOff(val.toInt());
        } 
        else if (name.compare("m_nMatchWeighting")==0) {
          m_nMatchWeighting = val.toInt();
          switch (m_nMatchWeighting) {
          case MATCHWEIGHT_UNIFORM:
            selMatchWtUni->setChecked(true);
            break;
          case MATCHWEIGHT_GIBBS:
            selMatchWtGibbs->setChecked(true);
            break;
          default:
            cout << "*********   ERROR: "
                 << "Match weighting strategy unknown !!!    *********" 
                 << endl;
          }
        } 
        else if (name.compare("m_dGibbsConst")==0)
          emit sigGibbsConstChanged(val);
        //-- MSME parameters --//
        else if (name.compare("m_nKernelType")==0) {
          m_nKernelType = val.toInt();
          switch (m_nKernelType) {
          case KERNEL_HCUBE:
            selKernelHCube->setChecked(true);
            break;
          case KERNEL_HSPHERE:
            selKernelHSphere->setChecked(true);
            break;
          default:
            cout << "*********   ERROR: "
                 << "Kernel Type unknown !!!    *********" << endl;
          }
          if( bVerbose )
            cerr << "   val: " << val.latin1() << " -> setting " 
                 << val.toInt() << endl;
        } 
        else if (name.compare("m_dMSMESizeX")==0)
          emit sigMSMExChanged(val);
        else if (name.compare("m_dMSMESizeY")==0)
          emit sigMSMEyChanged(val);
        else if (name.compare("m_dMSMESizeS")==0)
          emit sigMSMEsChanged(val);
        else if (name.compare("m_dMSMESizeR")==0)
          emit sigMSMErChanged(val);
        else if (name.compare("m_dMSMESizeA")==0)
          emit sigMSMEaChanged(val);
        else if (name.compare("m_dMSMESizeP")==0)
          emit sigMSMEpChanged(val);
        else if (name.compare("m_bAdaptiveScale")==0) {
          chkAdaptSc->setChecked((bool)val.toInt());
          slotSetAdaptiveScaleOnOff(val.toInt());
        }
        else if (name.compare("m_dAdaptMinScale")==0)
          emit sigAdaptMinScaleChanged(val);
        else if (name.compare("m_bNormScalePFig2")==0) {
          chkNormScPFig2->setChecked((bool)val.toInt());
          slotSetNormScalePFig2OnOff(val.toInt());
        }
        else if (name.compare("m_dMinVoteWeight")==0)
          emit sigMinVoteWeightChanged(val);
        else if (name.compare("m_dMaxVoteWeight")==0)
          emit sigMaxVoteWeightChanged(val);
        else if (name.compare("m_bUseFastMSME")==0) {
          chkUseFastMSME->setChecked((bool)val.toInt());
          slotSetUseFastMSMEOnOff(val.toInt());
        }
        //-- Recognition parameters  --//
        else if (name.compare("m_dScoreThreshSingle")==0)
          emit sigScoreThreshSingleChanged(val);
        else if (name.compare("m_nObjWidth")==0)
          emit sigObjWidthChanged(val);
        else if (name.compare("m_nObjHeight")==0)
          emit sigObjHeightChanged(val);
        else if (name.compare("m_bExtendSearch")==0) {
          chkExtendSearch->setChecked((bool)val.toInt());
          slotSetExtendSearchOnOff(val.toInt());
        }
        else if (name.compare("m_dSearchRange")==0)
          emit sigSearchRangeChanged(val);
        else if (name.compare("m_dRecoScaleMin")==0)
          emit sigScaleMinChanged(val);
        else if (name.compare("m_dRecoScaleMax")==0)
          emit sigScaleMaxChanged(val);
        //-- Rotation parameters  --//
        else if (name.compare("m_bRecoRotInv")==0) {
          chkMakeRotInv->setChecked((bool)val.toInt());
          slotSetRecoRotInvOnOff(val.toInt());
        }
        else if (name.compare("m_bRecoverRotation")==0) {
          chkRecoverRot->setChecked((bool)val.toInt());
          slotSetRecoverRotOnOff(val.toInt());
        }
        else if (name.compare("m_dRecoRotMin")==0)
          emit sigRotMinChanged(val);
        else if (name.compare("m_dRecoRotMax")==0)
          emit sigRotMaxChanged(val);
        //-- Aspect parameters  --//
        else if (name.compare("m_bUseAspect")==0) {
          chkUseAspect->setChecked((bool)val.toInt());
          slotSetUseAspectOnOff(val.toInt());
        }
        else if (name.compare("m_dRecoAspMin")==0)
          emit sigAspMinChanged(val);
        else if (name.compare("m_dRecoAspMax")==0)
          emit sigAspMaxChanged(val);
        else if (name.compare("m_nFixObjDim")==0) {
          m_nFixObjDim = val.toInt();
          switch (m_nFixObjDim) {
          case OBJDIM_WIDTH:
            selObjDimW->setChecked(true);
            break;
          case OBJDIM_HEIGHT:
            selObjDimH->setChecked(true);
            break;
          default:
            cout << "*********   ERROR: "
                 << "Object dimension unknown !!!    *********" << endl;
          }
          if( bVerbose )
            cerr << "   val: " << val.latin1() << " -> setting " 
                 << val.toInt() << endl;
        }
        //-- MDL parameters  --//
        else if (name.compare("m_bDoMDL")==0) {
          chkDoMDL->setChecked((bool)val.toInt());
          slotSetDoMDLOnOff(val.toInt());
        }
         else if (name.compare("m_bRejectOverlap")==0) {
          chkRejectOverlap->setChecked((bool)val.toInt());
          slotSetRejectOverlapOnOff(val.toInt());
        }
        else if (name.compare("m_dMaxOverlap")==0)
          emit sigMaxOverlapChanged(val);
         else if (name.compare("m_bRejectPFig")==0) {
          chkRejectPFig->setChecked((bool)val.toInt());
          slotSetRejectPFigOnOff(val.toInt());
        }
        else if (name.compare("m_dMinPFig")==0)
          emit sigMinPFigChanged(val);
        else if (name.compare("m_dMinPFigRefined")==0)
          emit sigMinPFigRefinedChanged(val);
        else if (name.compare("m_dWeightPFig")==0)
          emit sigWeightPFigChanged(val);
        else if (name.compare("m_dAdaptMinMDLScale")==0)
         emit sigAdaptMinMDLScaleChanged(val);
        else if (name.compare("m_bResampleHypos")==0) {
          chkResample->setChecked((bool)val.toInt());
          slotSetResampleHyposOnOff(val.toInt());
        }
        else
          cerr << "XXXXXXXXX     WARNING: variable " << name.latin1()
               << " unknown !!!     XXXXXXXXXXXX" << endl;
      }
    qfile.close();
  }
}

/***********************************************************/
/*                   Interface Handlers                    */
/***********************************************************/

QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, MSMEx, m_dMSMESizeX, 1 )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, MSMEy, m_dMSMESizeY, 1 )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, MSMEs, m_dMSMESizeS, 2 )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, MSMEr, m_dMSMESizeR, 1 )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, MSMEa, m_dMSMESizeA, 1 )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, MSMEp, m_dMSMESizeP, 1 )

QT_IMPLEMENT_RADIOBUTTON( RecoGUI::slot, KernelType, m_nKernelType )

QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, AdaptMinScale, m_dAdaptMinScale, 1 )

QT_IMPLEMENT_CHECKBOX( RecoGUI::slot, NormScalePFig2, m_bNormScalePFig2 )
QT_IMPLEMENT_CHECKBOX( RecoGUI::slot, RestrictScale, m_bRestrictScale )
QT_IMPLEMENT_CHECKBOX( RecoGUI::slot, UseFastMSME, m_bUseFastMSME )

QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, ScoreThreshSingle, m_dScoreThreshSingle, 2 )
QT_IMPLEMENT_LINEEDIT_INT( RecoGUI::slot, ObjWidth, m_nObjWidth )
QT_IMPLEMENT_LINEEDIT_INT( RecoGUI::slot, ObjHeight, m_nObjHeight )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, SearchRange, m_dSearchRange, 2 )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, ScaleMin, m_dRecoScaleMin, 2 )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, ScaleMax, m_dRecoScaleMax, 2 )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, MinVoteWeight, m_dMinVoteWeight, 5 )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, MaxVoteWeight, m_dMaxVoteWeight, 5 )
QT_IMPLEMENT_CHECKBOX( RecoGUI::slot, ExtendSearch, m_bExtendSearch )

QT_IMPLEMENT_CHECKBOX( RecoGUI::slot, NormPatch, m_bNormPerPatch )
QT_IMPLEMENT_CHECKBOX( RecoGUI::slot, NormPose, m_bNormPerPose )


QT_IMPLEMENT_CHECKBOX( RecoGUI::slot, DoMDL, m_bDoMDL )
QT_IMPLEMENT_CHECKBOX( RecoGUI::slot, RejectOverlap, m_bRejectOverlap )
QT_IMPLEMENT_CHECKBOX( RecoGUI::slot, RejectPFig, m_bRejectPFig )
QT_IMPLEMENT_CHECKBOX( RecoGUI::slot, ResampleHypos, m_bResampleHypos )

QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, MaxOverlap, m_dMaxOverlap, 2 )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, MinPFig, m_dMinPFig, 0 )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, WeightPFig, m_dWeightPFig, 2 )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, MinPFigRefined, m_dMinPFigRefined, 0 )

QT_IMPLEMENT_CHECKBOX( RecoGUI::slot, AdaptiveScale, m_bAdaptiveScale )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, AdaptMinMDLScale, m_dAdaptMinMDLScale, 1 )

QT_IMPLEMENT_CHECKBOX( RecoGUI::slot, RecoRotInv, m_bRecoRotInv )
QT_IMPLEMENT_CHECKBOX( RecoGUI::slot, RecoverRot, m_bRecoverRotation )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, RotMin, m_dRecoRotMin, 1 )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, RotMax, m_dRecoRotMax, 1 )

QT_IMPLEMENT_CHECKBOX( RecoGUI::slot, UseAspect, m_bUseAspect )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, AspMin, m_dRecoAspMin, 1 )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, AspMax, m_dRecoAspMax, 1 )
QT_IMPLEMENT_RADIOBUTTON( RecoGUI::slot, FixObjDim, m_nFixObjDim )
QT_IMPLEMENT_RADIOBUTTON( RecoGUI::slot, MatchWeighting, m_nMatchWeighting )
QT_IMPLEMENT_LINEEDIT_FLOAT( RecoGUI::slot, GibbsConst, m_dGibbsConst, 2 )



