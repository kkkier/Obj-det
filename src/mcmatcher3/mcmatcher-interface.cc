/*********************************************************************/
/*                                                                   */
/* FILE         mcmatcher-interface.cc                               */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Basic framework to test image patches received from  */
/*              a test image with a loaded cluster codebook.         */
/*              (extension of clmanager).                            */
/*              This version also allows tests with a uniform sam-   */
/*              pling, and matching in an eigenspace.                */
/*                                                                   */
/* BEGIN        Tue Nov 05 2002                                      */
/* LAST CHANGE  Tue Oct 11 2005                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <string>
#include <algorithm>

#include <qapplication.h>
#include <qwidget.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qcolor.h>
#include <qpainter.h>
#include <qstringlist.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qinputdialog.h>
#include <qcombobox.h>
#include <qtabwidget.h>
#include <qlistbox.h>
#include <qtable.h>

#include <qtmacros.hh>
#include <qtcoordlabel.hh>
#include <qtimgbrowser.hh>
#include <opgrayimage.hh>
#include <featuregui.hh>
#include <featurecue.hh>
#include <recogui.hh>
#include <cuewidget.hh>
#include <detector.hh>

#include "mcmatcher.hh"


/*******************/
/*   Definitions   */
/*******************/

/*===================================================================*/
/*                         Class ISMReco                           */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

ISMReco::ISMReco( QWidget *parent, const char *name )
  : QWidget( parent, name )
  , m_grayImg()
  , m_qsLastImage(DIR_IMAGES.c_str())
  , m_calCamera()
  , m_cbCodebook()
  , m_ismMDL( 0 )
  , m_bShowGUI( true )
  , m_sImgName("Img")
  , m_bMapsOn( false )
  , m_bMapsOnManually( false )
  , m_bSilhouettesLoaded( false )
  , m_vActiveVotes()
{
  /********************************/
  /*   create the custom widget   */
  /********************************/
  QVBox *framebox = new QVBox( this, "framebox" );
  framebox->setFrameStyle( QFrame::WinPanel | QFrame::Sunken );
  
  QVBox *picBox = new QVBox( framebox, "picBox" );
  rsSourceImg = new QtResizeImg (picBox, "source image");

  QHBox *imageBox = new QHBox( picBox, "image box");
  //QLabel *labSpace = new QLabel( "", picBox, "labSpace" );
  //labSpace->setText("");
  rsResultImg  = new QtResizeImg (imageBox, "result image");
  rsResultImg2 = new QtResizeImg (imageBox, "result image2");
  
  /**********************************************/
  /*   make an "Intermediate Results" display   */
  /**********************************************/
  //QHBox *hbIconBox = new QHBox( picBox, "image box");
  //qClassView = new QtClusterView( hbIconBox, "codebook view" );
  
  m_qcvResultView = new QtClusterView( picBox, "result view" );
  m_qcvResultView->setMaximumHeight( 200 );

  /***************************/
  /*   create menu layouts   */
  /***************************/
  QVBoxLayout *menuleftbox  = new QVBoxLayout();
  QVBoxLayout *menurightbox = new QVBoxLayout();
  menuleftbox->setSpacing( 0 );
  menurightbox->setSpacing( 0 );
  
  /**********************************/
  /*   make a 'ScaleMatcher' Logo   */
  /**********************************/
  QLabel *logo = new QLabel( (STR_NAME + " " + STR_VERSION).c_str(), 
                             this, "labelLogo" );
  logo->setFont( QFont( "Times", 14, QFont::Bold ) );
  
  
  /*=================================================================*/
  /*             L E F T   H A N D   S I D E   M E N U               */
  /*=================================================================*/
  
  /***************************************/
  /*   make a 'Load Test Image' button   */
  /***************************************/
  QPushButton *load = new QPushButton( "&Load Test Image", this, "load" );
  menuleftbox->addWidget( load );
  
  connect( load, SIGNAL(clicked()), this, SLOT(loadImage()) );
  
  
  /*-----------------------------------*/
  /* Checkbox 'Better Gray Conversion' */
  /*-----------------------------------*/
  chkGrayConv = new QCheckBox( "Better RGB->Gray Conversion", 
                                             this,  "chkGrayConv" );
  menuleftbox->addWidget( chkGrayConv );
  chkGrayConv->setChecked( true );
  m_bBetterGrayConv = chkGrayConv->isChecked();
  QT_CONNECT_CHECKBOX( chkGrayConv, GrayConv );

  /*-----------------------------------*/
  /* Checkbox 'Histogram Equalization' */
  /*-----------------------------------*/
  chkGammaNorm = new QCheckBox( "Perform Gamma Normalization", 
                                             this,  "chkGamNorm" );
  menuleftbox->addWidget( chkGammaNorm );
  chkGammaNorm->setChecked( false );
  m_bPerformGammaNorm = chkGammaNorm->isChecked();
  QT_CONNECT_CHECKBOX( chkGammaNorm, GammaNorm );

  /*-----------------------------------*/
  /* Checkbox 'Histogram Equalization' */
  /*-----------------------------------*/
//   QCheckBox   *chkUse3DCtxt = new QCheckBox( "Use 3D Context", 
//                                              this,  "chkUse3DCtxt" );
//   menuleftbox->addWidget( chkUse3DCtxt );
//   chkUse3DCtxt->setChecked( false );
  m_bUse3DContext = false; //chkUse3DCtxt->isChecked();
//   QT_CONNECT_CHECKBOX( chkUse3DCtxt, Use3DContext );


//   /*----------------------------------*/
//   /* Checkbox 'Process Flipped Image' */
//   /*----------------------------------*/
//   QCheckBox   *chkBothDir = new QCheckBox( "Process Flipped Image", this, 
//                                            "chkBothDir" );
//   menuleftbox->addWidget( chkBothDir );
//   chkBothDir->setChecked( false );
//   m_bProcessBothDir = chkBothDir->isChecked();
//   QT_CONNECT_CHECKBOX( chkBothDir, BothDir );


  /*-------------------------------*/
  /* Check box "Load image patches */
  /*-------------------------------*/
  //QCheckBox *chkUsePatches = new QCheckBox( "Load/Use Image Patches", 
  //                                          this, "usepatches" );
  
  //chkUsePatches->setChecked( false );
  m_bUsePatches = false; //chkUsePatches->isChecked();
  
  //QT_CONNECT_CHECKBOX( chkUsePatches, UsePatches );
  
  //menuleftbox->addWidget( chkUsePatches );
  
  /********************************/
  /*   Make a Group 'Detectors'   */
  /********************************/
  menuleftbox->addSpacing( 10 );
  QButtonGroup *bgDetect = new QButtonGroup( "Detectors:", this, "detectors" );
  bgDetect->setColumnLayout( 1, Qt::Horizontal );
  bgDetect->setInsideSpacing( 0 );

  /*----------------------------------*/
  /*   Make a button 'Add Detector'   */
  /*----------------------------------*/
  QPushButton  *addDetect = new QPushButton( "Add Detector", bgDetect, 
                                             "addDet" );
  
  connect( addDetect, SIGNAL(clicked()), this, SLOT(addDetector()) );
    
  /*------------------------------------*/
  /*   Make a table for all detectors   */
  /*------------------------------------*/
  bgDetect->addSpace( 5 );
  m_tblDetectors = new QTable( 1, 5, bgDetect, "DetTable" );
  m_tblDetectors->setSelectionMode( QTable::SingleRow );

  m_tblDetectors->horizontalHeader()->setLabel( 0, "Category" );
  m_tblDetectors->horizontalHeader()->setLabel( 1, "Pose" );
  m_tblDetectors->horizontalHeader()->setLabel( 2, "X" );
  m_tblDetectors->horizontalHeader()->setLabel( 3, "t_S" );
  m_tblDetectors->horizontalHeader()->setLabel( 4, "Color" );

  m_tblDetectors->setText( 0, 0, "people" );
  m_tblDetectors->setText( 0, 1, "az000deg" );
  m_tblDetectors->setText( 0, 2, "" );
  m_tblDetectors->setText( 0, 3, "2.50" );
  m_tblDetectors->setText( 0, 4, "yellow" );
  m_tblDetectors->setMaximumWidth(270);
  for( int i=0; i<4; i++ )
    m_tblDetectors->adjustColumn( i );
  m_tblDetectors->setColumnStretchable(4, true);
  m_tblDetectors->setNumRows( 0 );

  connect( m_tblDetectors, SIGNAL(doubleClicked(int,int,int,const QPoint&)),
           this, SLOT(detectorTableClicked(int,int,int,const QPoint&)) );

  menuleftbox->addWidget( bgDetect );

  /*------------------------------*/
  /*   Initialize the detectors   */
  /*------------------------------*/
  m_nNumDetectors = 0;
  m_vDetectors.clear();
//   Detector detNew;
//   m_vDetectors.push_back( detNew );

//   DetectorGUI* guiDetector = m_vDetectors[0].createGUI( this, "guiDetect" );
//   menuleftbox->addWidget( guiDetector );

  /***************************/
  /*   Make a Group 'Cues'   */
  /***************************/
  menuleftbox->addSpacing( 10 );
  QButtonGroup *bgCues = new QButtonGroup( "Cues:", this, "cues" );
  bgCues->setColumnLayout( 1, Qt::Horizontal );
  bgCues->setInsideSpacing( 0 );
 
  /*-----------------------------*/
  /*   Make a button 'Add Cue'   */
  /*-----------------------------*/
  //QPushButton  *addCue = new QPushButton( "Add Cue", bgCues, "loadCl" );
  
  //connect( addCue, SIGNAL(clicked()), this, SLOT(addCue()) );
    
  /*-------------------------------*/
  /*   Make a table for all cues   */
  /*-------------------------------*/
  bgCues->addSpace( 5 );
  m_tblCues = new QTable( 1, 5, bgCues, "CueTable" );
  m_tblCues->setSelectionMode( QTable::SingleRow );

  m_tblCues->horizontalHeader()->setLabel( 0, "Det." );
  m_tblCues->horizontalHeader()->setLabel( 1, "Feat." );
  m_tblCues->horizontalHeader()->setLabel( 2, "X" );
  m_tblCues->horizontalHeader()->setLabel( 3, "MinSc." );
  m_tblCues->horizontalHeader()->setLabel( 4, "MaxSc." );

  m_tblCues->setText( 0, 0, "Salient" );
  m_tblCues->setText( 0, 1, "SURF128" );
  m_tblCues->setText( 0, 2, "" );
  m_tblCues->setText( 0, 3, "1.0" );
  m_tblCues->setText( 0, 4, "64.0" );
  m_tblCues->setMaximumWidth(270);
  for( int i=0; i<4; i++ )
    m_tblCues->adjustColumn( i );
  m_tblCues->setColumnStretchable(4, true);
  m_tblCues->setNumRows( 0 );

  connect( m_tblCues, SIGNAL(doubleClicked(int,int,int,const QPoint&)),
           this, SLOT(cueTableClicked(int,int,int,const QPoint&)) );

  menuleftbox->addWidget( bgCues );

   
  /* reserve some space for future additions */
  menuleftbox->addStretch(100);


  /*****************************************/
  /*   Tab Widget for Displaying Options   */
  /*****************************************/
  menuleftbox->addSpacing(10);
  QTabWidget *tabCompare = new QTabWidget( this, "tabcomp" );

  /* tabs: method, options */
  QWidget     *tabwDrawing  = new QWidget( this, "tabwDrawing" );
  QWidget     *tabwOutput   = new QWidget( this, "tabwOutput" );
  QWidget     *tabwDisplay  = new QWidget( this, "tabwDisplay" );
  QVBoxLayout *tabpDrawing  = new QVBoxLayout( tabwDrawing );
  QVBoxLayout *tabpOutput   = new QVBoxLayout( tabwOutput );
  QVBoxLayout *tabpDisplay  = new QVBoxLayout( tabwDisplay );
  tabpDrawing->setSpacing( 0 );
  tabpOutput->setSpacing( 0 );
  tabpDisplay->setSpacing( 0 );


  /***********************/
  /*   Drawing Options   */
  /***********************/
  tabpDrawing->addSpacing( 5 );
  QButtonGroup *bgDrawing = new QButtonGroup("Drawing Options:", 
                                              tabwDrawing, "bgDrawing");
  bgDrawing->setColumnLayout( 1, Qt::Horizontal );
  tabpDrawing->addWidget( bgDrawing );
  
  /*--------------------------*/
  /*   Checkbox "Draw Maps"   */
  /*--------------------------*/
  chkDrawMaps = new QCheckBox( "Draw Maps", bgDrawing, "sMaps" );

  chkDrawMaps->setChecked( false );
  m_bMapsOnManually = chkDrawMaps->isChecked();
  QT_CONNECT_CHECKBOX( chkDrawMaps, DrawMaps );
    
  /*---------------------------------*/
  /*   Checkbox "Draw Confidences"   */
  /*---------------------------------*/
  chkDrawConf = new QCheckBox( "Draw Confidence", bgDrawing, 
                                             "chkconf" );

  chkDrawConf->setChecked( true );
  m_bDrawConfidence = chkDrawConf->isChecked();
  QT_CONNECT_CHECKBOX( chkDrawConf, DrawConf );
  
  /*----------------------------------*/
  /*   Checkbox "Draw Tight BBoxes"   */
  /*----------------------------------*/
  chkTightBB = new QCheckBox( "Draw Tight BBoxes", bgDrawing, 
                                            "sTightBB" );

  chkTightBB->setChecked( false );
  m_bDrawTightBB = chkTightBB->isChecked();
  QT_CONNECT_CHECKBOX( chkTightBB, DrawTightBB );
  
  /*--------------------------------------*/
  /*   Checkbox "Eval. Unique Contrib."   */
  /*--------------------------------------*/
  chkEvalUnique = new QCheckBox( "Eval. Unique Contrib.", bgDrawing, 
                                             "chkunique" );

  chkEvalUnique->setChecked( false );
  m_bEvalUniqueContrib = chkEvalUnique->isChecked();
  QT_CONNECT_CHECKBOX( chkEvalUnique, EvalUnique );
  
  /*------------------------------------*/
  /*   Checkbox "Draw Maps > x votes"   */
  /*------------------------------------*/
  //QCheckBox    *chkMapThresh = new QCheckBox( "Only Draw Maps > ", 
  //                                           bgOptions, "chkmapthresh" );
  //QLineEdit    *editMapThresh = new QLineEdit( "5.0", bgOptions, 
  //                                            "editMapThresh" );
  //chkMapThresh->setChecked( false );
  m_bUseMapThresh = false; //chkMapThresh->isChecked();
  //QT_CONNECT_CHECKBOX( chkMapThresh, MapThresh );

  //editMapThresh->setMinimumWidth(30);
  //editMapThresh->setMaximumWidth(40);
  m_dMapThresh = 5.0; //atof( editMapThresh->text() );
  //QT_CONNECT_LINEEDIT( editMapThresh, MapThresh );
  
  /*--------------------------------------*/
  /*   Checkbox "Use Patch Size Factor"   */
  /*--------------------------------------*/
  //QCheckBox    *chkPSFact = new QCheckBox( "Use Patch Size Factor", 
  //                                         bgOptions, "chkpsf" );
  //QLabel       *labDummy5= new QLabel( "", bgOptions );

  //chkPSFact->setChecked( false );
  m_bPatchSizeFactor = false; //chkPSFact->isChecked();
  //QT_CONNECT_CHECKBOX( chkPSFact, PatchSizeFact );
  

  /***********************/
  /*   Output Options   */
  /***********************/
  tabpOutput->addSpacing( 5 );
  QButtonGroup *bgOutput = new QButtonGroup("Output:", 
                                             tabwOutput, "bgOutput");
  bgOutput->setColumnLayout( 1, Qt::Horizontal );
  tabpOutput->addWidget( bgOutput );
  
  /*-----------------------------------*/
  /*   Checkbox "Text Output: Steps"   */
  /*-----------------------------------*/
  chkShowTxtSteps = new QCheckBox( "Text Output: Main steps", 
                                   bgOutput, "showtxts" );

  chkShowTxtSteps->setChecked( true );
  m_bShowTxtSteps = chkShowTxtSteps->isChecked();
  QT_CONNECT_CHECKBOX( chkShowTxtSteps, ShowTxtSteps );
  
  /*-------------------------------------*/
  /*   Checkbox "Text Output: Details"   */
  /*-------------------------------------*/
  chkShowTxtDet = new QCheckBox( "Text Output: Details", 
                                 bgOutput, "showtxtd" );

  chkShowTxtDet->setChecked( true );
  m_bShowTxtDetails = chkShowTxtDet->isChecked();
  QT_CONNECT_CHECKBOX( chkShowTxtDet, ShowTxtDetails );
  
  /*-------------------------------------*/
  /*   Checkbox "Text Output: Voting"   */
  /*-------------------------------------*/
  chkShowTxtVot = new QCheckBox( "Text Output: Voting Results", 
                                 bgOutput, "showtxtd" );

  chkShowTxtVot->setChecked( true );
  m_bShowTxtVoting = chkShowTxtVot->isChecked();
  QT_CONNECT_CHECKBOX( chkShowTxtVot, ShowTxtVoting );
  
  /*-------------------------------------*/
  /*   Checkbox "Text Output: MDL"   */
  /*-------------------------------------*/
  chkShowTxtMDL = new QCheckBox( "Text Output: MDL Results", 
                                 bgOutput, "showtxtd" );

  chkShowTxtMDL->setChecked( true );
  m_bShowTxtMDL = chkShowTxtMDL->isChecked();
  QT_CONNECT_CHECKBOX( chkShowTxtMDL, ShowTxtMDL );
  
  /*-----------------------------*/
  /*   Checkbox "Show Timings"   */
  /*-----------------------------*/
  chkShowTimings = new QCheckBox( "Show Timings", 
                                 bgOutput, "showtim" );

  chkShowTimings->setChecked( true );
  m_bShowTimings = chkShowTimings->isChecked();
  QT_CONNECT_CHECKBOX( chkShowTimings, ShowTimings );
  
  /***********************/
  /*   Display Options   */
  /***********************/
  tabpDisplay->addSpacing( 5 );
  QButtonGroup *bgDisplay = new QButtonGroup("Display:", 
                                             tabwDisplay, "bgDisplay");
  bgDisplay->setColumnLayout( 1, Qt::Horizontal );
  tabpDisplay->addWidget( bgDisplay );
  
  /*-------------------------------------*/
  /*   Checkbox "Display Matched Patches"   */
  /*-------------------------------------*/
//   QCheckBox    *chkDrawMatchedPs = new QCheckBox( "Display Matched Patches", 
//                                                   bgDisplay, "drawmp" );

//   chkDrawMatchedPs->setChecked( false );
//   m_bDrawMatchedPs = chkDrawMatchedPs->isChecked();
//   QT_CONNECT_CHECKBOX( chkDrawMatchedPs, DrawMatchedPs );
  
  /*----------------------------------------*/
  /*   Checkbox "Display Interest Points"   */
  /*----------------------------------------*/
  chkShowIntPts = new QCheckBox( "Display Interest Points", 
                                 bgDisplay, "showip" );

  chkShowIntPts->setChecked( true );
  m_bShowIntPts = chkShowIntPts->isChecked();
  QT_CONNECT_CHECKBOX( chkShowIntPts, ShowIntPts );
  
  /*------------------------------------*/
  /*   Checkbox "Display VotingSpace"   */
  /*------------------------------------*/
  chkDispVS = new QCheckBox( "Display Voting Space", bgDisplay,"chdispvs" );

  chkDispVS->setChecked( false );
  m_bDisplayVS = chkDispVS->isChecked();
  QT_CONNECT_CHECKBOX( chkDispVS, DisplayVS );
  
  /*--------------------------------*/
  /*   Checkbox "Display Support"   */
  /*--------------------------------*/
  chkDispSup = new QCheckBox( "Display Support", bgDisplay,"chdispsup" );

  chkDispSup->setChecked( false );
  m_bDisplaySupport = chkDispSup->isChecked();
  QT_CONNECT_CHECKBOX( chkDispSup, DisplaySupport );
  
  /*--------------------------------------*/
  /*   Checkbox "Display Segmentations"   */
  /*--------------------------------------*/
  chkDispSeg = new QCheckBox( "Display Segmentations", bgDisplay,"chdispseg" );

  chkDispSeg->setChecked( true );
  m_bDisplaySegment = chkDispSeg->isChecked();
  QT_CONNECT_CHECKBOX( chkDispSeg, DisplaySegment );
  
  /*------------------------------------------*/
  /*   Checkbox "Display Multi-cue Results"   */
  /*------------------------------------------*/
  chkDispMC = new QCheckBox( "Display Result List", bgDisplay, "chdispmc" );

  chkDispMC->setChecked( false );
  m_bDisplayMultiCue = chkDispMC->isChecked();
  QT_CONNECT_CHECKBOX( chkDispMC, DisplayMultiCue );
  
  
  /**************************************************/
  /*   Finish the tab Widget for Patch Comparison   */
  /**************************************************/
  tabpDrawing->addStretch( 50 );
  tabpOutput->addStretch( 50 );
  tabpDisplay->addStretch( 50 );

  tabCompare->addTab( tabwDrawing, "Drawing" );
  tabCompare->addTab( tabwOutput, "Output" );
  tabCompare->addTab( tabwDisplay, "Display" );

  tabCompare->setCurrentPage( 1 );

  menuleftbox->addWidget( tabCompare );


  /*************************************/
  /*   make a 'Test Matching' button   */
  /*************************************/
//   menuleftbox->addSpacing( 5 );
//   QPushButton *testMatch = new QPushButton( "Test Matching", this, "tstmat" );
  
//   menuleftbox->addWidget( testMatch );

//   connect( testMatch, SIGNAL(clicked()), this, SLOT(testMatching()) );
  

  /***********************************/
  /*   make a 'Save Images' button   */
  /***********************************/
  menuleftbox->addSpacing( 5 );
  QPushButton *saveImg = new QPushButton( "Save Images", this, "saveImages" );
  //saveImg->setFont( QFont( "Times", 11, QFont::Normal ) );
  
  menuleftbox->addWidget( saveImg );

  connect( saveImg, SIGNAL(clicked()), this, SLOT(saveImages()) );
  

  /*******************************************/
  /*   make a 'Save Cluster Images' button   */
  /*******************************************/
//   QPushButton *saveClImages = new QPushButton( "Save Cluster Images", this, 
//                                              "saveClusters" );
//   //saveClImages->setFont( QFont( "Times", 10, QFont::Bold ) );
  
//   menuleftbox->addWidget( saveClImages );

//   connect( saveClImages, SIGNAL(clicked()), this, SLOT(saveClusterImages()) );
  
  /******************************/
  /* Button 'Save Test Results' */
  /******************************/
  menuleftbox->addSpacing( 10 );
//   QPushButton *saveTestRes = new QPushButton( "Save Results Matlab", this, 
//                                               "saveTestRes" );
//   menuleftbox->addWidget( saveTestRes );
  
//   connect( saveTestRes, SIGNAL(clicked()), 
//            this, SLOT(savePatchActivationsMatlab()) );
  
  /*-----------------------------*/
  /* Button 'Save Segmentations' */
  /*-----------------------------*/
  QPushButton *saveSegment = new QPushButton( "Save Segmentations", this, 
                                              "saveSegment" );
  menuleftbox->addWidget( saveSegment );
  
  connect( saveSegment, SIGNAL(clicked()), 
           this, SLOT(saveSegmentationsMatlab()) );
  
  /***************************************/
  /* Button 'Save FeatureVectors Matlab' */
  /***************************************/
//   QPushButton *saveFVMat = new QPushButton( "Save FV Matlab", this, 
//                                             "saveFVMa" );
//   menuleftbox->addWidget( saveFVMat );
  
//   connect( saveFVMat, SIGNAL(clicked()), this, SLOT(saveFVMatlab()));

  /*-------------------------------*/
  /* Button 'Save Clusters Matlab' */
  /*-------------------------------*/
//   QPushButton *saveClMat = new QPushButton( "Save Clusters Matlab", this, 
//                                             "saveClMat" );
//   menuleftbox->addWidget( saveClMat );
  
//   connect( saveClMat, SIGNAL(clicked()), this, SLOT(saveClustersMatlab()));

 
  /*************************************/
  /*   make a 'Test Matching' button   */
  /*************************************/
//   menuleftbox->addSpacing( 5 );
//   QPushButton *testMatch = new QPushButton( "Test Matching", this, "tstmat" );
  
//   menuleftbox->addWidget( testMatch );

//   connect( testMatch, SIGNAL(clicked()), this, SLOT(testMatching()) );
  

  /*=================================================================*/
  /*            R I G H T   H A N D   S I D E   M E N U              */
  /*=================================================================*/
  
  /**********************************/
  /* Tab Widget for Patches & Parts */
  /**********************************/
//   QTabWidget *tabPatchParts = new QTabWidget( this, "tabParts" );

//   /* tabs: Patches, Subparts & Parts */
//   QWidget     *tabwPatches  = new QWidget( this, "tabwPatches" );
//   QVBoxLayout *tabpPatches  = new QVBoxLayout( tabwPatches );
//   tabpPatches->setSpacing( 0 );

  /********************************/
  /* Button 'Compute Occurrences' */
  /********************************/
//   tabpPatches->addSpacing( 5 );

//   QPushButton *procTrain = new QPushButton( "Compute Occurrences", 
//                                             tabwPatches, 
//                                             "procTrain" );
//   tabpPatches->addWidget( procTrain );
  
//   connect( procTrain, SIGNAL(clicked()), this, SLOT(computeOccurrences()) );
  
  /*---------------------------------------*/
  /* Buttons 'load' and 'save' occurrences */
  /*---------------------------------------*/
//   QHBox       *boxOccIO = new QHBox( tabwPatches, "occ I/O box");
//   QPushButton *loadOcc  = new QPushButton( "load", boxOccIO, 
//                                            "loadOcc" );
//   QPushButton *saveOcc  = new QPushButton( "save", boxOccIO, 
//                                            "saveOcc" );
//   tabpPatches->addWidget( boxOccIO );
  
//   connect( loadOcc, SIGNAL(clicked()), this, SLOT(loadOccurrences()) );
//   connect( saveOcc, SIGNAL(clicked()), this, SLOT(saveOccurrences()) );
  
  /*-----------------------------------*/
  /* Buttons 'save occurrences Matlab' */
  /*-----------------------------------*/
//   QPushButton *saveOccMat  = new QPushButton( "save Matlab format",
//                                               tabwPatches, 
//                                               "saveOccMat" );
//   tabpPatches->addWidget( saveOccMat );
  
//   connect( saveOccMat, SIGNAL(clicked()), 
//            this, SLOT(saveOccurrencesMatlab()) );
  
  
  /*-------------------*/
  /* Group 'Save Info' */
  /*-------------------*/
//   QButtonGroup *bgSaveInfo= new QButtonGroup( "Save Info:", 
//                                               tabwPatches, "saveInfo");
//   bgSaveInfo->setColumnLayout( 2, Qt::Horizontal );
//   tabpPatches->addWidget( bgSaveInfo );
  
//   QLabel      *labCateg  = new QLabel( "Category:", bgSaveInfo );
//   QLineEdit   *editCateg = new QLineEdit( "3", bgSaveInfo, "editCateg");
//   QCheckBox   *chkPose   = new QCheckBox( "Pose:", bgSaveInfo, "checkPose" );
//   QLineEdit   *editPose  = new QLineEdit( "25", bgSaveInfo, "editPose");
  
//   editCateg->setMaximumWidth(50);
//   editPose->setMaximumWidth(50);
//   chkPose->setChecked( false );
//   m_nCateg = atoi( editCateg->text() );
//   m_nPose = atoi( editPose->text() );
//   m_bSavePose = chkPose->isChecked();
  
//   QT_CONNECT_LINEEDIT( editCateg, Categ );
//   QT_CONNECT_LINEEDIT( editPose, Pose );
//   QT_CONNECT_CHECKBOX( chkPose, SavePose );
  

  /*********************************************/
  /* Finish the tab Widget for Patches & Parts */
  /*********************************************/
//   tabpPatches->addStretch( 50 );

//   tabPatchParts->addTab( tabwPatches,  "Patch"  );

//   menurightbox->addWidget( tabPatchParts );


  /*******************************/
  /* Button 'Process Test Image' */
  /*******************************/
  menurightbox->addSpacing( 10 );
  QPushButton *procTest = new QPushButton( "Process Test Image", this, 
                                           "procTest" );
  menurightbox->addWidget( procTest );
  
  connect( procTest, SIGNAL(clicked()), this, SLOT(processTestImgStd()) );
  
  /*------------------------------*/
  /* Button 'Perform Test Series' */
  /*------------------------------*/
  QPushButton *testSeries = new QPushButton( "Perform IDL Test", this, 
                                              "testSeries" );
  menurightbox->addWidget( testSeries );
  
  connect( testSeries, SIGNAL(clicked()), 
           this, SLOT(performIDLTestSeries()) );
  
  
  /*********************************/
  /* Button 'Process Image Series' */
  /*********************************/
//   QPushButton *procSeries = new QPushButton( "Process Image Series", this, 
//                                               "procSeries" );
//   menurightbox->addWidget( procSeries );
  
//   connect( procSeries, SIGNAL(clicked()), 
//            this, SLOT(processImageSeries()) );
  
  /************************/
  /* Button 'Open Socket' */
  /************************/
//   QPushButton *pbSocket = new QPushButton( "Listen to Socket", this, 
//                                               "opsocket" );
//   menurightbox->addWidget( pbSocket );
  
//   connect( pbSocket, SIGNAL(clicked()), 
//            this, SLOT(listenToSocket()) );
  
  /*--------------------------------------------------*/
  /* Check box to automatically refine the hypothesis */
  /*--------------------------------------------------*/
//   chkRefineHypo = new QCheckBox( "Resample Hypotheses", 
//                                  this, "refinehypo" );
//   menurightbox->addWidget( chkRefineHypo );

//   chkRefineHypo->setChecked( false );
  m_bRefineHypothesis = false; //chkRefineHypo->isChecked();

//   QT_CONNECT_CHECKBOX( chkRefineHypo, RefineHypo );


  /*-----------------------------------*/
  /* Check box to write bounding boxes */
  /*-----------------------------------*/
//   QCheckBox   *chkWriteDetect = new QCheckBox( "Write Detections", 
//                                                 this, "writedetect" );
//   menurightbox->addWidget( chkWriteDetect );

//   chkWriteDetect->setChecked( false );
  m_bWriteDetections = false; //chkWriteDetect->isChecked();

//   QT_CONNECT_CHECKBOX( chkWriteDetect, WriteDetect );


  /*----------------------------------*/
  /* Check box to write result images */
  /*----------------------------------*/
  chkWriteResult = new QCheckBox( "Write Result Images", 
                                  this, "writeresult" );
  menurightbox->addWidget( chkWriteResult );

  chkWriteResult->setChecked( false );
  m_bWriteResults = chkWriteResult->isChecked();

  QT_CONNECT_CHECKBOX( chkWriteResult, WriteResults );


  /*----------------------------------------*/
  /* Check box to write segmentation images */
  /*----------------------------------------*/
  chkWriteSeg = new QCheckBox( "Write Segmentations", 
                               this, "writeseg" );
  menurightbox->addWidget( chkWriteSeg );

  chkWriteSeg->setChecked( false );
  m_bWriteSegs = chkWriteSeg->isChecked();

  QT_CONNECT_CHECKBOX( chkWriteSeg, WriteSegs );


  /*-----------------------------------------*/
  /* Check box to draw "rejected" hypotheses */
  /*-----------------------------------------*/
//   QCheckBox   *chkDrawRejected = new QCheckBox( "Draw Rejected Hypos", 
//                                                 this, "drawreject" );
//   menurightbox->addWidget( chkDrawRejected );

//   chkDrawRejected->setChecked( false );
  m_bDrawRejectedHypos = false; //chkDrawRejected->isChecked();

//   QT_CONNECT_CHECKBOX( chkDrawRejected, DrawRejected );


  /**************************************/
  /*   Tab Widget for Reco Parameters   */
  /**************************************/
//   RecoGUI *guiReco = m_parReco.createGUI( this, "reco" );
//   menurightbox->addWidget( guiReco );
  //m_ismReco.setRecoParams( m_parReco );

  menurightbox->addSpacing(5);
  QTabWidget *tabReco = new QTabWidget( this, "reco" );

  /* tabs: msme, reco, mdl */
  QWidget     *tabwMDL       = new QWidget( this, "tabwMDL" );
  QVBoxLayout *tabpMDL       = new QVBoxLayout( tabwMDL );
  tabpMDL->setSpacing( 0 );
  
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
//   QLabel      *labPFigRef      = new QLabel( "  refined PFig<", bgMDLParams );
//   QLineEdit   *editMinPFigRef  = new QLineEdit( "45000", bgMDLParams,
//                                                 "edpfigref");

  editOverlap->setMaximumWidth(50);
  editMinPFig->setMinimumWidth(40);
  editMinPFig->setMaximumWidth(50);
  editWeightPFig->setMinimumWidth(50);
  editWeightPFig->setMaximumWidth(50);
//   editMinPFigRef->setMinimumWidth(50);
//   editMinPFigRef->setMaximumWidth(50);
  m_dMaxOverlap        = atof( editOverlap->text() );
  m_dMinPFig           = atof( editMinPFig->text() );
  m_dWeightPFig        = atof( editWeightPFig->text() );
  m_dMinPFigRefined    = 45000; //atof( editMinPFigRef->text() );

  chkRejectOverlap->setChecked( false );
  m_bRejectOverlap = chkRejectOverlap->isChecked();
  chkRejectPFig->setChecked( true );
  m_bRejectPFig = chkRejectPFig->isChecked();

  QT_CONNECT_LINEEDIT( editOverlap, MaxOverlap ); 
  QT_CONNECT_LINEEDIT( editMinPFig, MinPFig ); 
  QT_CONNECT_LINEEDIT( editWeightPFig, WeightPFig ); 
//   QT_CONNECT_LINEEDIT( editMinPFigRef, MinPFigRefined ); 

  QT_CONNECT_CHECKBOX( chkRejectOverlap, RejectOverlap );
  QT_CONNECT_CHECKBOX( chkRejectPFig, RejectPFig );

  /*----------------------------------*/
  /* Check box to resample hypotheses */
  /*----------------------------------*/
//   chkResample = new QCheckBox( "Resample Hypotheses", tabwMDL, 
//                                           "resmpl" );

//   chkResample->setChecked( false );
  m_bResampleHypos = false; //chkResample->isChecked();
//   QT_CONNECT_CHECKBOX( chkResample, ResampleHypos );

//   tabpMDL->addWidget( chkResample );

  /*********************************************/
  /*   Finish the tab Widget for Recognition   */
  /*********************************************/
  tabpMDL->addStretch( 50 );

  tabReco->addTab( tabwMDL,       "Hypothesis Selection" );
  tabReco->setCurrentPage( 0 );

  menurightbox->addWidget( tabReco );


  /*************************************/
  /*   Tab Widget for New Components   */
  /*************************************/
  menurightbox->addSpacing(5);
  QTabWidget *tabNew = new QTabWidget( this, "newstuff" );

  /* tabs: multicue, ground plane */
//   QWidget     *tabwCue     = new QWidget( this, "tabwCue" );
  QWidget     *tabwGP      = new QWidget( this, "tabwGP" );
//   QVBoxLayout *tabpCue     = new QVBoxLayout( tabwCue );
  QVBoxLayout *tabpGP      = new QVBoxLayout( tabwGP );
//   tabpCue->setSpacing( 0 );
  tabpGP->setSpacing( 0 );

  /***************/
  /* Group 'Cue' */
  /***************/
//   tabpCue->addSpacing( 5 );

  /*---------------------*/
  /* Verification method */
  /*---------------------*/
//   QButtonGroup *bgCombi = new QButtonGroup( "Combination Strategy:", 
//                                             tabwCue, "bgCombi");
//   bgCombi->setColumnLayout( 1, Qt::Horizontal );
//   tabpCue->addWidget( bgCombi );

//   QRadioButton *selSum   = new QRadioButton( "sum", bgCombi, "selSum" );
//   QRadioButton *selAvg   = new QRadioButton( "avg", bgCombi, "selAvg" );
//   QRadioButton *selMax   = new QRadioButton( "max", bgCombi, "selMax" );

//   selAvg->setChecked( true );
  m_nCombiMethod = true ; //CUE_COMBI_AVG;
//   QT_CONNECT_RADIOBUTTON( bgCombi, CombiMethod );


  /************************/
  /* Group 'Ground Plane' */
  /************************/
  tabpGP->addSpacing( 5 );

  /*---------------------*/
  /* Verification method */
  /*---------------------*/
  QButtonGroup *bgGP = new QButtonGroup( "Ground Plane:", 
                                         tabwGP, "bgGP");
  bgGP->setColumnLayout( 2, Qt::Horizontal );
  tabpGP->addWidget( bgGP );

  QRadioButton *selGPCur = new QRadioButton( "current", bgGP, 
                                             "selGPCur" );
  QLabel       *lbEmpty1 = new QLabel( "", bgGP );
  QRadioButton *selGPPre = new QRadioButton( "previous", bgGP, 
                                             "selGPPre" );
  QLabel       *lbEmpty2 = new QLabel( "", bgGP );
  QLabel      *lbImgSc   = new QLabel( "Image scale:", bgGP );
  //QLineEdit   *edImgSc   = new QLineEdit( "0.5", bgGP, "edImgSc" );
  QLineEdit   *edImgSc   = new QLineEdit( "1.0", bgGP, "edImgSc" );
  QLabel      *lbWorldSc = new QLabel( "World scale:", bgGP );
  //QLineEdit   *edWorldSc = new QLineEdit( "0.049603", bgGP, "edWorldSc" );
  QLineEdit   *edWorldSc = new QLineEdit( "1.0", bgGP, "edWorldSc" );
 
  selGPCur->setChecked( true );
  m_nGPFrame = GP_CURRENT;
  QT_CONNECT_RADIOBUTTON( bgGP, GPFrame );

  edImgSc->setMaximumWidth(80);
  edImgSc->setMinimumWidth(80);
  edWorldSc->setMaximumWidth(100);
  edWorldSc->setMinimumWidth(100);
  m_dImgScale   = atof(edImgSc->text());
  m_dWorldScale = atof(edWorldSc->text());

  QT_CONNECT_LINEEDIT( edImgSc, ImgScale );
  QT_CONNECT_LINEEDIT( edWorldSc, WorldScale );

  /************************************************/
  /*   Finish the tab Widget for new components   */
  /************************************************/
//   tabpCue->addStretch( 50 );
  tabpGP->addStretch( 50 );

//   tabNew->addTab( tabwCue,  "Cue"  );
  tabNew->addTab( tabwGP,   "Ground Plane"  );

//   tabNew->setCurrentPage( 1 );

  menurightbox->addWidget( tabNew );

  /**********************************************/
  /*   Tab Widget for Verification Parameters   */
  /**********************************************/
  VeriGUI *guiVeri = m_parVeri.createGUI( this, "veri" );
  menurightbox->addWidget( guiVeri );


  /******************************/
  /* Button 'Refine Hypotheses' */
  /******************************/
//   menurightbox->addSpacing( 5 );
//   QPushButton *refineHypos = new QPushButton( "Refine Hypotheses", this, 
//                                               "refhypos" );
//   QPushButton *refineHyposMS= new QPushButton( "Refine Hypotheses MS", this, 
//                                               "refhypos" );
//   menurightbox->addWidget( refineHypos );
//   menurightbox->addWidget( refineHyposMS );
  
//   connect( refineHypos, SIGNAL(clicked()), 
//            this, SLOT(refineHypotheses()));
//   connect( refineHyposMS, SIGNAL(clicked()), 
//            this, SLOT(refineHypothesesMultiScale()));


  /*************************************/
  /* Button 'Display Scale Footprints' */
  /*************************************/
  menurightbox->addSpacing( 5 );
  QPushButton *scFootPr = new QPushButton( "Display Scale Footprint", this, 
                                           "scfps" );
  menurightbox->addWidget( scFootPr );
  
  connect( scFootPr, SIGNAL(clicked()), this, SLOT(displayScaleFootprint()));


  /*****************************/
  /* Button 'Load Silhouettes' */
  /*****************************/
  menurightbox->addSpacing( 5 );
  QPushButton *loadSilh = new QPushButton( "Load Silhouettes", this, 
                                           "ldsilh" );
  menurightbox->addWidget( loadSilh );
  
  connect( loadSilh, SIGNAL(clicked()), this, SLOT(loadSilhouettes()));

  /****************************/
  /*   make a 'Quit' button   */
  /****************************/
  menurightbox->addStretch( 150 );
  QPushButton *quit = new QPushButton( "&Quit", this, "quit" );
  quit->setFont( QFont( "Times", 11, QFont::Bold ) );
  
  menurightbox->addWidget( quit );
  
  connect( quit, SIGNAL(clicked()), qApp, SLOT(quit()) );
  
  /*=================================================================*/
  /*                     O T H E R   S T U F F                       */
  /*=================================================================*/
  
  /***********************************/
  /*   make a "coordinate display"   */
  /***********************************/
  QHBox        *coordbox = new QHBox(this, "boxCoord");
  QtCoordLabel *coordPos = new QtCoordLabel( "Position:", coordbox, "labelCoord" );
  
  // display the coordinates when the mouse moves over the image
  rsSourceImg->setMouseTracking( true );
  connect ( rsSourceImg, SIGNAL(mouseMovedOnPixel(int, int, QRgb)), 
            coordPos, SLOT(setCoordValue( int, int, QRgb )) );
  
  rsResultImg->setMouseTracking( true );
  connect ( rsResultImg, SIGNAL(mouseMovedOnPixel(int, int, QRgb, float)), 
            coordPos, SLOT(setCoordValue( int, int, QRgb, float )) );
  
  // display the result images in a separate window when clicked at
  connect( rsSourceImg, SIGNAL(leftClickOnPixel(int,int)), 
           this, SLOT(showResultImg(int,int)) );
  
  /****************************/
  /*   do the window layout   */
  /****************************/
  menuleftbox->addStretch( 150 );

  QGridLayout *grid = new QGridLayout( this, 2, 3, 10 );
  grid->addWidget( logo, 0, 0, Qt::AlignHCenter );
  grid->addLayout( menuleftbox, 1, 0 );
  grid->addWidget( coordbox, 0, 1, Qt::AlignHCenter );
  grid->addWidget( framebox, 1, 1 ); //framebox contains images
  grid->addLayout( menurightbox, 1, 2 );
  
  grid->setColStretch( 1, 10 );
}


void ISMReco::saveParams( bool bVerbose )
{
  QString qsFileName = QFileDialog::getSaveFileName( DIR_CODEBOOKS, 
                                       "Parameter file (*.params)", this);
  if ( !qsFileName.isEmpty() )
    saveParams( qsFileName, bVerbose );
}


void ISMReco::saveParams( string sFileName, bool bVerbose )
{
  QFile qfile( sFileName );
  if ( qfile.open(IO_WriteOnly) ) // | IO_Append ) ) don't append
    {
      QTextStream stream( &qfile );
      stream << "\n*** Section scmatcher-interface ***\n";
      qfile.close();
    }
  //if( m_fcCue.params() != 0 )
  //  m_fcCue.params()->saveParams( sFileName.c_str(), bVerbose );
  //if( m_parMatching.params() != 0 )
  //  m_parMatching.params()->saveParams( sFileName.c_str(), bVerbose );

  //if( m_fcHarris.params() != 0 )
  //m_fcHarris.params()->saveParams( sFileName.c_str(), bVerbose );
  //m_parReco.params()->saveParams( sFileName.c_str(), bVerbose );
}


void ISMReco::loadParams( bool bVerbose )
{
  QString qsFileName = QFileDialog::getOpenFileName( DIR_CODEBOOKS, 
                                       "Parameter file (*.params)", this);
  if (!qsFileName.isEmpty())
    loadParams( qsFileName.latin1(), bVerbose );
}


void ISMReco::loadParams( string sFileName, bool bVerbose )
{
  cout << "  Loading no main interface parameters..." << endl;
  cout << "    (file was '" << sFileName << "')" << endl;
//   if( m_parMatching.params() != 0 ) {
//     cout << "    Loading matching parameters..." << endl;
//     m_parMatching.params()->loadParams( sFileName.c_str(), bVerbose );
//   }
//   if( m_fcCue.params() != 0 )
//     m_fcCue.params()->loadParams( sFileName.c_str(), bVerbose );

  //if( m_fcHarris.params() != 0 )
  //m_fcHarris.params()->loadParams( sFileName.c_str(), bVerbose );
  //m_parReco.params()->loadParams( sFileName.c_str(), bVerbose );

//   QFile qfile( sFileName.c_str() );
//   if ( qfile.open( IO_ReadOnly ) ) {
//     QTextStream stream( &qfile );
//     QString line;
//     bool started=false;
    
//     while(!(line = stream.readLine()).isNull())
//       {
//         if (!started || line.isEmpty())
//           {
//             if( (line.compare("*** Section scmatcher-interface ***")==0) )
//               started=true;
//             continue;
//           }
//         else if(line.left(1).compare("*")==0)
//           {
//             started=false;//stop if section is over
//             break;
//           }
        
//         int len = line.length();
//         int pos = line.find(':');
//         if( bVerbose )
//           printf("line: %s\n",line.latin1());
        
//         QString name = line.left(pos);
//         QString val = line.right(len-pos-2);
        
//         if (name.compare("m_dRejectionThresh")==0) {
//           m_parMatching.params()->slotSetRejectionThreshold(val);
//           cout << "    Setting m_dRejectionThresh=" << val << endl;
//         } else if (name.compare("m_dSimilarity")==0)
//           m_parMatching.params()->slotSetRejectionThreshold(val);
//         else if (name.compare("m_dFeatureSimFact")==0)
//           m_parMatching.params()->slotSetFeatureSimFact(val);
//          else
//           cerr << "XXXXXXXXX     WARNING: variable " << name.latin1()
//                << " unknown !!!     XXXXXXXXXXXX" << endl;
//       }
//     qfile.close();
// }
}


/***********************************************************/
/*                   Interface Handlers                    */
/***********************************************************/

QT_IMPLEMENT_LINEEDIT_FLOAT( ISMReco::slot, MapThresh, m_dMapThresh, 1)
QT_IMPLEMENT_LINEEDIT_FLOAT( ISMReco::slot, ImgScale, m_dImgScale, 2)
QT_IMPLEMENT_LINEEDIT_FLOAT( ISMReco::slot, WorldScale, m_dWorldScale, 6)

  //QT_IMPLEMENT_RADIOBUTTON( ISMReco::slot, CombiMethod, m_nCombiMethod )
QT_IMPLEMENT_RADIOBUTTON( ISMReco::slot, GPFrame, m_nGPFrame )

QT_IMPLEMENT_CHECKBOX( ISMReco::slot, GammaNorm, m_bPerformGammaNorm )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, GrayConv, m_bBetterGrayConv )
  //QT_IMPLEMENT_CHECKBOX( ISMReco::slot, Use3DContext, m_bUse3DContext )
  //QT_IMPLEMENT_CHECKBOX( ISMReco::slot, BothDir, m_bProcessBothDir )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, UsePatches, m_bUsePatches )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DrawMaps, m_bMapsOnManually )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DrawConf, m_bDrawConfidence )
  //QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DrawMatchedPs, m_bDrawMatchedPs )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, ShowTxtSteps, m_bShowTxtSteps )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, ShowTxtDetails, m_bShowTxtDetails )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, ShowTxtVoting, m_bShowTxtVoting )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, ShowTxtMDL, m_bShowTxtMDL )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, ShowTimings, m_bShowTimings )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, ShowIntPts, m_bShowIntPts )

QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DisplayVS, m_bDisplayVS )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DisplaySupport, m_bDisplaySupport )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DisplaySegment, m_bDisplaySegment )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DisplayMultiCue, m_bDisplayMultiCue )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, Figure, m_bOnlyUseFigure )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DrawTightBB, m_bDrawTightBB )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, EvalUnique, m_bEvalUniqueContrib )
  //QT_IMPLEMENT_CHECKBOX( ISMReco::slot, RefineHypo, m_bRefineHypothesis )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, WriteResults, m_bWriteResults )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, WriteSegs, m_bWriteSegs )
//QT_IMPLEMENT_CHECKBOX( ISMReco::slot, WriteDetect, m_bWriteDetections )
//QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DrawRejected, m_bDrawRejectedHypos )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DoMDL, m_bDoMDL )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, RejectOverlap, m_bRejectOverlap )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, RejectPFig, m_bRejectPFig )
  //QT_IMPLEMENT_CHECKBOX( ISMReco::slot, ResampleHypos, m_bResampleHypos )

QT_IMPLEMENT_LINEEDIT_FLOAT( ISMReco::slot, MaxOverlap, m_dMaxOverlap, 2 )
QT_IMPLEMENT_LINEEDIT_FLOAT( ISMReco::slot, MinPFig, m_dMinPFig, 0 )
QT_IMPLEMENT_LINEEDIT_FLOAT( ISMReco::slot, WeightPFig, m_dWeightPFig, 2 )
  //QT_IMPLEMENT_LINEEDIT_FLOAT( ISMReco::slot, MinPFigRefined, m_dMinPFigRefined, 0 )


