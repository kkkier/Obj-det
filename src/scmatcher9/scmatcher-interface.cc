/*********************************************************************/
/*                                                                   */
/* FILE         scmatcher-interface.cc                               */
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
/* LAST CHANGE  Wed Feb 05 2003                                      */
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

#include <qtmacros.hh>
#include <qtcoordlabel.hh>
#include <qtimgbrowser.hh>
#include <opgrayimage.hh>
#include <featuregui.hh>
#include <featurecue.hh>
#include <recogui.hh>

//#include "patchmatching.hh"
#include "scmatcher.hh"


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
  , m_qsLastIDL(DIR_IMAGES.c_str())
  , m_sImgName("Img")
  , m_bMapsOn( false )
  , m_bMapsOnManually( false )
  , m_bSilhouettesLoaded( false )
  , m_vvAllNeighbors()
  , m_vvAllNeighborsSim()
  , m_vActiveVotes()
  , m_ismReco( 0 )
  , m_ismVeri( 0 )
{
  /********************************/
  /*   create the custom widget   */
  /********************************/
  QVBox *framebox = new QVBox( this, "framebox" );
  framebox->setFrameStyle( QFrame::WinPanel | QFrame::Sunken );
  
  QVBox *picBox = new QVBox( framebox, "picBox" );
  
  QHBox *imageBox = new QHBox( picBox, "image box");
  QLabel *spacelabel = new QLabel( "", picBox, "labelSpace" );
  rsSourceImg = new QtResizeImg (imageBox, "source image");
  rsResultImg = new QtResizeImg (imageBox, "result image");
  
  
  /*********************************/
  /*   make a "Codebook" display   */
  /*********************************/
  QHBox *hbIconBox = new QHBox( picBox, "image box");
  qClassView = new QtClusterView( hbIconBox, "codebook view" );
  

  /***************************/
  /*   create menu layouts   */
  /***************************/
  QVBoxLayout *menuleftbox  = new QVBoxLayout();
  QVBoxLayout *menurightbox = new QVBoxLayout();
  QVBoxLayout *menulleftbox = new QVBoxLayout();
  menuleftbox->setSpacing( 0 );
  menurightbox->setSpacing( 0 );
  menulleftbox->setSpacing( 0 );
  
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
  
  
  /*************************************/
  /*   make a 'Load Codebook' button   */
  /*************************************/
  //menuleftbox->addSpacing( 10 );
  QPushButton  *clLoad = new QPushButton( "Load Code&book", this, "loadCl" );
  
  connect( clLoad, SIGNAL(clicked()), this, SLOT(loadCodebook()) );
  
  menuleftbox->addWidget( clLoad );
  
  /*-----------------------------------*/
  /* Checkbox 'Histogram Equalization' */
  /*-----------------------------------*/
  QCheckBox   *chkHistEq = new QCheckBox( "Perform Hist. Eq.", this, 
                                          "chkHistEq" );
  menuleftbox->addWidget( chkHistEq );
  chkHistEq->setChecked( false );
  m_bPerformHistEq = chkHistEq->isChecked();
  QT_CONNECT_CHECKBOX( chkHistEq, HistEq );


  /*-------------------------------*/
  /* Check box "Load image patches */
  /*-------------------------------*/
  QCheckBox *chkUsePatches = new QCheckBox( "Load/Use Image Patches", 
                                            this, "usepatches" );
  
  chkUsePatches->setChecked( false );
  m_bUsePatches = chkUsePatches->isChecked();
  
  QT_CONNECT_CHECKBOX( chkUsePatches, UsePatches );
  
  menuleftbox->addWidget( chkUsePatches );
  
  
  /***************************************/
  /*   make a 'Collect patches' button   */
  /***************************************/
  menuleftbox->addSpacing(10);
  QPushButton *colPatches = new QPushButton( "Collect Patches", this, 
                                           "colpatches" );
  menuleftbox->addWidget( colPatches );
  
  connect( colPatches, SIGNAL(clicked()), this, SLOT(collectPatches()) );
  
  
  /*****************************************/
  /*   Tab Widget for Feature Extraction   */
  /*****************************************/
  FeatureGUI *guiCue = m_fcCue.createGUI( this, "cue1" );
  menuleftbox->addWidget( guiCue );

  /* create a second feature extractor for Harris points */
  /* (for an experimental verification stage).           */
  FeatureGUI *guiHarris = m_fcHarris.createGUI( NULL, "cueHarris" );
  m_fcHarris.params()->slotSelectPatchExtMethod( PATCHEXT_HARRIS );

  /*******************************/
  /*   make a 'Compare' button   */
  /*******************************/
  menulleftbox->addSpacing( 10 );
  QPushButton *compare = new QPushButton( "&Compare", this, "compare" );
  menulleftbox->addWidget( compare);
  
  connect( compare, SIGNAL(clicked()), this, SLOT(compareFeatures()) );
  
  
  /***************************************/
  /*   Tab Widget for Patch Comparison   */
  /***************************************/
  menulleftbox->addSpacing(5);
  QTabWidget *tabCompare = new QTabWidget( this, "tabcomp" );

  /* tabs: method, options */
  QWidget     *tabwMethod   = new QWidget( this, "tabwMethod" );
  QWidget     *tabwDrawing  = new QWidget( this, "tabwDrawing" );
  QWidget     *tabwDisplay  = new QWidget( this, "tabwDisplay" );
  QVBoxLayout *tabpMethod   = new QVBoxLayout( tabwMethod );
  QVBoxLayout *tabpDrawing  = new QVBoxLayout( tabwDrawing );
  QVBoxLayout *tabpDisplay  = new QVBoxLayout( tabwDisplay );
  tabpMethod->setSpacing( 0 );
  tabpDrawing->setSpacing( 0 );
  tabpDisplay->setSpacing( 0 );

  /************************************************/
  /*   parameter fields for 'Compare' function    */
  /************************************************/
  tabpMethod->addSpacing( 5 );

  MatchingGUI *guiMatching = m_parMatching.createGUI( tabwMethod, "matching" );
  tabpMethod->addWidget( guiMatching );
  m_cbCodebook.setMatchingParams( m_parMatching );

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
  QCheckBox    *chkMaps = new QCheckBox( "Draw Maps", bgDrawing, "sMaps" );

  chkMaps->setChecked( false );
  m_bMapsOnManually = chkMaps->isChecked();
  QT_CONNECT_CHECKBOX( chkMaps, Maps );
    
  /*---------------------------------*/
  /*   Checkbox "Draw Confidences"   */
  /*---------------------------------*/
  QCheckBox    *chkDrawConf = new QCheckBox( "Draw Confidence", bgDrawing, 
                                             "chkconf" );

  chkDrawConf->setChecked( false );
  m_bDrawConfidence = chkDrawConf->isChecked();
  QT_CONNECT_CHECKBOX( chkDrawConf, DrawConf );
  
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
  QCheckBox    *chkDrawMatchedPs = new QCheckBox( "Display Matched Patches", 
                                                  bgDisplay, "drawmp" );

  chkDrawMatchedPs->setChecked( false );
  m_bDrawMatchedPs = chkDrawMatchedPs->isChecked();
  QT_CONNECT_CHECKBOX( chkDrawMatchedPs, DrawMatchedPs );
  
  /*------------------------------------*/
  /*   Checkbox "Display VotingSpace"   */
  /*------------------------------------*/
  QCheckBox    *chkDispVS = new QCheckBox( "Display Voting Space", bgDisplay,
                                           "chdispvs" );

  chkDispVS->setChecked( false );
  m_bDisplayVS = chkDispVS->isChecked();
  QT_CONNECT_CHECKBOX( chkDispVS, DisplayVS );
  
  /*--------------------------------*/
  /*   Checkbox "Display Support"   */
  /*--------------------------------*/
  QCheckBox    *chkDispSup = new QCheckBox( "Display Support", bgDisplay,
                                           "chdispsup" );

  chkDispSup->setChecked( true );
  m_bDisplaySupport = chkDispSup->isChecked();
  QT_CONNECT_CHECKBOX( chkDispSup, DisplaySupport );
  
  /*--------------------------------------*/
  /*   Checkbox "Display Segmentations"   */
  /*--------------------------------------*/
  QCheckBox    *chkDispSeg = new QCheckBox( "Display Segmentations", bgDisplay,
                                           "chdispseg" );

  chkDispSeg->setChecked( true );
  m_bDisplaySegment = chkDispSeg->isChecked();
  QT_CONNECT_CHECKBOX( chkDispSeg, DisplaySegment );
  
  /*----------------------------------*/
  /*   Checkbox "Draw Tight BBoxes"   */
  /*----------------------------------*/
  QCheckBox    *chkTightBB = new QCheckBox( "Draw Tight BBoxes", bgDisplay, 
                                            "sTightBB" );
  QLabel       *labDummy6= new QLabel( "", bgDisplay );

  chkTightBB->setChecked( false );
  m_bDrawTightBB = chkMaps->isChecked();
  QT_CONNECT_CHECKBOX( chkTightBB, DrawTightBB );
  
  
  /**************************************************/
  /*   Finish the tab Widget for Patch Comparison   */
  /**************************************************/
  tabpMethod->addStretch( 50 );
  tabpDrawing->addStretch( 50 );
  tabpDisplay->addStretch( 50 );

  tabCompare->addTab( tabwMethod,  "Comparison"  );
  tabCompare->addTab( tabwDrawing, "Drawing" );
  tabCompare->addTab( tabwDisplay, "Display" );

  menulleftbox->addWidget( tabCompare );


  /*************************************/
  /*   make a 'Test Matching' button   */
  /*************************************/
  menulleftbox->addSpacing( 5 );
  QPushButton *testMatch = new QPushButton( "Test Matching", this, "tstmat" );
  
  menulleftbox->addWidget( testMatch );

  connect( testMatch, SIGNAL(clicked()), this, SLOT(testMatching()) );
  

  /***********************************/
  /*   make a 'Save Images' button   */
  /***********************************/
  menulleftbox->addSpacing( 5 );
  QPushButton *saveImg = new QPushButton( "Save Images", this, "saveImages" );
  //saveImg->setFont( QFont( "Times", 11, QFont::Normal ) );
  
  menulleftbox->addWidget( saveImg );

  connect( saveImg, SIGNAL(clicked()), this, SLOT(saveImages()) );
  

  /*******************************************/
  /*   make a 'Save Cluster Images' button   */
  /*******************************************/
  QPushButton *saveClImages = new QPushButton( "Save Cluster Images", this, 
                                             "saveClusters" );
  //saveClImages->setFont( QFont( "Times", 10, QFont::Bold ) );
  
  menulleftbox->addWidget( saveClImages );

  connect( saveClImages, SIGNAL(clicked()), this, SLOT(saveClusterImages()) );
  
  /******************************/
  /* Button 'Save Test Results' */
  /******************************/
  menulleftbox->addSpacing( 10 );
  QPushButton *saveTestRes = new QPushButton( "Save Results Matlab", this, 
                                              "saveTestRes" );
  menulleftbox->addWidget( saveTestRes );
  
  connect( saveTestRes, SIGNAL(clicked()), 
           this, SLOT(savePatchActivationsMatlab()) );
  
  /*-----------------------------*/
  /* Button 'Save Segmentations' */
  /*-----------------------------*/
  QPushButton *saveSegment = new QPushButton( "Save Segmentations", this, 
                                              "saveSegment" );
  menulleftbox->addWidget( saveSegment );
  
  connect( saveSegment, SIGNAL(clicked()), 
           this, SLOT(saveSegmentationsMatlab()) );
  
  /***************************************/
  /* Button 'Save FeatureVectors Matlab' */
  /***************************************/
  QPushButton *saveFVMat = new QPushButton( "Save FV Matlab", this, 
                                            "saveFVMa" );
  menulleftbox->addWidget( saveFVMat );
  
  connect( saveFVMat, SIGNAL(clicked()), this, SLOT(saveFVMatlab()));

  /*-------------------------------*/
  /* Button 'Save Clusters Matlab' */
  /*-------------------------------*/
  QPushButton *saveClMat = new QPushButton( "Save Clusters Matlab", this, 
                                            "saveClMat" );
  menulleftbox->addWidget( saveClMat );
  
  connect( saveClMat, SIGNAL(clicked()), this, SLOT(saveClustersMatlab()));

  /*----------------------------------*/
  /* Button 'Write Segm. Texture Map' */
  /*----------------------------------*/
  menulleftbox->addSpacing( 5 );
  QPushButton *pbWrSegMap = new QPushButton( "Write Segm. Texture Map", this, 
                                              "wrSegMap" );
  menulleftbox->addWidget( pbWrSegMap );
  
  connect( pbWrSegMap, SIGNAL(clicked()), this, SLOT(saveTextureMap()));

  /*----------------------------------*/
  /* Button 'Write Segmentation Data' */
  /*----------------------------------*/
  QPushButton *pbWrSegData = new QPushButton( "Write Segmentation Data", this, 
                                              "wrSegDat" );
  menulleftbox->addWidget( pbWrSegData );
  
  connect( pbWrSegData, SIGNAL(clicked()), this, SLOT(saveSegmentationData()));

  
  /*=================================================================*/
  /*            R I G H T   H A N D   S I D E   M E N U              */
  /*=================================================================*/
  
  /**********************************/
  /* Tab Widget for Patches & Parts */
  /**********************************/
  QTabWidget *tabPatchParts = new QTabWidget( this, "tabParts" );

  /* tabs: Patches, Subparts & Parts */
  QWidget     *tabwPatches  = new QWidget( this, "tabwPatches" );
  QVBoxLayout *tabpPatches  = new QVBoxLayout( tabwPatches );
  QWidget     *tabwData     = new QWidget( this, "tabwData" );
  QVBoxLayout *tabpData     = new QVBoxLayout( tabwData );
  tabpPatches->setSpacing( 0 );
  tabpData->setSpacing( 0 );

  /********************************/
  /* Button 'Compute Occurrences' */
  /********************************/
  tabpPatches->addSpacing( 5 );

  QHBox       *hbCompOcc    = new QHBox( tabwPatches );
  QPushButton *procTrain    = new QPushButton( "Compute Occurrences", 
                                               hbCompOcc, "procTrain" );
  QPushButton *procTrainIDL = new QPushButton( "IDL", 
                                               hbCompOcc, "procTrainIDL" );
  tabpPatches->addWidget( hbCompOcc );
  
  connect( procTrain, SIGNAL(clicked()), this, SLOT(computeOccurrences()) );
  connect( procTrainIDL, SIGNAL(clicked()), 
           this, SLOT(computeOccurrencesIDL()) );
  
  /*---------------------------------------*/
  /* Buttons 'load' and 'save' occurrences */
  /*---------------------------------------*/
  QHBox       *boxOccIO = new QHBox( tabwPatches, "occ I/O box");
  QPushButton *loadOcc  = new QPushButton( "load", boxOccIO, 
                                           "loadOcc" );
  QPushButton *saveOcc  = new QPushButton( "save", boxOccIO, 
                                           "saveOcc" );
  tabpPatches->addWidget( boxOccIO );
  
  connect( loadOcc, SIGNAL(clicked()), this, SLOT(loadOccurrences()) );
  connect( saveOcc, SIGNAL(clicked()), this, SLOT(saveOccurrences()) );
  
  /*-----------------------------------*/
  /* Buttons 'save occurrences Matlab' */
  /*-----------------------------------*/
  QPushButton *saveOccMat  = new QPushButton( "save Matlab format",
                                              tabwPatches, 
                                              "saveOccMat" );
  tabpPatches->addWidget( saveOccMat );
  
  connect( saveOccMat, SIGNAL(clicked()), 
           this, SLOT(saveOccurrencesMatlab()) );
  
  
  /*-------------------*/
  /* Group 'Save Info' */
  /*-------------------*/
  QButtonGroup *bgSaveInfo= new QButtonGroup( "Save Info:", 
                                              tabwPatches, "saveInfo");
  bgSaveInfo->setColumnLayout( 2, Qt::Horizontal );
  tabpPatches->addWidget( bgSaveInfo );
  
  QLabel      *labCateg  = new QLabel( "Category:", bgSaveInfo );
  QLineEdit   *editCateg = new QLineEdit( "3", bgSaveInfo, "editCateg");
  QCheckBox   *chkPose   = new QCheckBox( "Pose:", bgSaveInfo, "checkPose" );
  QLineEdit   *editPose  = new QLineEdit( "25", bgSaveInfo, "editPose");
  
  editCateg->setMaximumWidth(50);
  editPose->setMaximumWidth(50);
  chkPose->setChecked( false );
  m_nCateg = atoi( editCateg->text() );
  m_nPose = atoi( editPose->text() );
  m_bSavePose = chkPose->isChecked();
  
  QT_CONNECT_LINEEDIT( editCateg, Categ );
  QT_CONNECT_LINEEDIT( editPose, Pose );
  QT_CONNECT_CHECKBOX( chkPose, SavePose );
  

  /********************************/
  /* Button 'Compute Occurrences' */
  /********************************/
  tabpData->addSpacing( 5 );

  /*----------------------------------*/
  /* Make a field for the object size */
  /*----------------------------------*/
  QButtonGroup *bgObjSize = new QButtonGroup( "Fix Object Size:", tabwData, 
                                              "bgO" );
  bgObjSize->setColumnLayout( 2, Qt::Horizontal );
  tabpData->addWidget( bgObjSize );
  
  QRadioButton *selObjW = new QRadioButton( "width=", bgObjSize, "selObjW" );
  QLineEdit    *edObjW  = new QLineEdit( "200", bgObjSize, "edObjW");
  QRadioButton *selObjH = new QRadioButton( "height=", bgObjSize, "selObjH");
  QLineEdit    *edObjH  = new QLineEdit( "200", bgObjSize, "edObjH");
  
  edObjW->setMaximumWidth(50);
  edObjH->setMaximumWidth(50);
  m_nObjWidth  = atoi( edObjW->text() );
  m_nObjHeight = atoi( edObjH->text() );
  
  selObjW->setChecked( true );
  m_nFixObjDim = OBJDIM_WIDTH;
  
  QT_CONNECT_LINEEDIT( edObjW, ObjWidth ); 
  QT_CONNECT_LINEEDIT( edObjH, ObjHeight ); 
  QT_CONNECT_RADIOBUTTON( bgObjSize, FixObjDim );


  /*********************************************/
  /* Finish the tab Widget for Patches & Parts */
  /*********************************************/
  tabpPatches->addStretch( 50 );
  tabpData->addStretch( 50 );

  tabPatchParts->addTab( tabwPatches,  "Patch"  );
  tabPatchParts->addTab( tabwData,     "Data"  );

  menurightbox->addWidget( tabPatchParts );


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
  
  
  /***********************************/
  /* Button 'Start UIUC Test Series' */
  /***********************************/
  QPushButton *testUIUC = new QPushButton( "Perform UIUC Test", this, 
                                              "startUIUC" );
  menurightbox->addWidget( testUIUC );
  
  connect( testUIUC, SIGNAL(clicked()), 
           this, SLOT(performUIUCTestSeries()) );
  
  /*------------------------------------------------*/
  /* make an edit field to enter the starting image */
  /*------------------------------------------------*/
  QHBox *hbUIUCFrom= new QHBox( this, "hbUIUCfrom");
  menurightbox->addWidget( hbUIUCFrom );
  
  QLabel      *labUIUCFrom = new QLabel( "Start with Img:", hbUIUCFrom );
  QLineEdit   *editUIUCFrom= new QLineEdit( "0", hbUIUCFrom, "editUIUCFrom");
  
  editUIUCFrom->setMaximumWidth(50);
  m_nUIUCFrom = atoi( editUIUCFrom->text() );
  
  QT_CONNECT_LINEEDIT( editUIUCFrom, UIUCFrom ); 


  /*********************************/
  /* Button 'Process Image Series' */
  /*********************************/
  QPushButton *procSeries = new QPushButton( "Process Image Series", this, 
                                              "procSeries" );
  menurightbox->addWidget( procSeries );
  
  connect( procSeries, SIGNAL(clicked()), 
           this, SLOT(processImageSeries()) );
  
  /*--------------------------------------------------*/
  /* Check box to automatically refine the hypothesis */
  /*--------------------------------------------------*/
  QCheckBox   *chkRefineHypo = new QCheckBox( "Refine Hypothesis", 
                                                this, "refinehypo" );
  menurightbox->addWidget( chkRefineHypo );

  chkRefineHypo->setChecked( false );
  m_bRefineHypothesis = chkRefineHypo->isChecked();

  QT_CONNECT_CHECKBOX( chkRefineHypo, RefineHypo );


  /*-----------------------------------*/
  /* Check box to write bounding boxes */
  /*-----------------------------------*/
  QCheckBox   *chkWriteDetect = new QCheckBox( "Write Detections", 
                                                this, "writedetect" );
  menurightbox->addWidget( chkWriteDetect );

  chkWriteDetect->setChecked( false );
  m_bWriteDetections = chkWriteDetect->isChecked();

  QT_CONNECT_CHECKBOX( chkWriteDetect, WriteDetect );


  /*-----------------------------------------*/
  /* Check box to draw "rejected" hypotheses */
  /*-----------------------------------------*/
  QCheckBox   *chkDrawRejected = new QCheckBox( "Draw Rejected Hypos", 
                                                this, "drawreject" );
  menurightbox->addWidget( chkDrawRejected );

  chkDrawRejected->setChecked( false );
  m_bDrawRejectedHypos = chkDrawRejected->isChecked();

  QT_CONNECT_CHECKBOX( chkDrawRejected, DrawRejected );


  /**************************************/
  /*   Tab Widget for Reco Parameters   */
  /**************************************/
  RecoGUI *guiReco = m_parReco.createGUI( this, "reco" );
  menurightbox->addWidget( guiReco );
  m_ismReco.setRecoParams( m_parReco );


  /**********************************************/
  /*   Tab Widget for Verification Parameters   */
  /**********************************************/
  VeriGUI *guiVeri = m_parVeri.createGUI( this, "veri" );
  menurightbox->addWidget( guiVeri );


  /******************************/
  /* Button 'Refine Hypotheses' */
  /******************************/
  menurightbox->addSpacing( 5 );
  QPushButton *refineHypos = new QPushButton( "Refine Hypotheses", this, 
                                              "refhypos" );
  QPushButton *refineHyposMS= new QPushButton( "Refine Hypotheses MS", this, 
                                              "refhypos" );
  menurightbox->addWidget( refineHypos );
  menurightbox->addWidget( refineHyposMS );
  
  connect( refineHypos, SIGNAL(clicked()), 
           this, SLOT(refineHypotheses()));
  connect( refineHyposMS, SIGNAL(clicked()), 
           this, SLOT(refineHypothesesMultiScale()));


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
  
  /****************************/
  /*   do the window layout   */
  /****************************/
  menuleftbox->addStretch( 150 );
  menulleftbox->addStretch( 150 );

  QGridLayout *grid = new QGridLayout( this, 2, 4, 10 );
  grid->addWidget( logo, 0, 0, Qt::AlignHCenter );
  grid->addLayout( menuleftbox, 1, 0 );
  grid->addLayout( menulleftbox, 1, 1);
  grid->addWidget( coordbox, 0, 1, Qt::AlignHCenter );
  grid->addWidget( framebox, 1, 2 ); //framebox contains images
  grid->addLayout( menurightbox, 1, 3 );
  
  grid->setColStretch( 2, 10 );
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
  if( m_fcCue.params() != 0 )
    m_fcCue.params()->saveParams( sFileName.c_str(), bVerbose );
  //if( m_fcHarris.params() != 0 )
  //m_fcHarris.params()->saveParams( sFileName.c_str(), bVerbose );
  //m_parReco.params()->saveParams( sFileName.c_str(), bVerbose );
  if( m_parMatching.params() != 0 )
    m_parMatching.params()->saveParams( sFileName.c_str(), bVerbose );
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
  cout << "  Loading main interface parameters..." << endl;
  if( m_parMatching.params() != 0 ) {
    cout << "    Loading matching parameters..." << endl;
    m_parMatching.params()->loadParams( sFileName.c_str(), bVerbose );
  }
  if( m_fcCue.params() != 0 )
    m_fcCue.params()->loadParams( sFileName.c_str(), bVerbose );
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

QT_IMPLEMENT_LINEEDIT_INT( ISMReco::slot, Categ, m_nCateg )
QT_IMPLEMENT_LINEEDIT_INT( ISMReco::slot, Pose, m_nPose )

QT_IMPLEMENT_LINEEDIT_INT( ISMReco::slot, UIUCFrom, m_nUIUCFrom )

QT_IMPLEMENT_LINEEDIT_INT( ISMReco::slot, ObjWidth, m_nObjWidth )
QT_IMPLEMENT_LINEEDIT_INT( ISMReco::slot, ObjHeight, m_nObjHeight )

QT_IMPLEMENT_RADIOBUTTON( ISMReco::slot, FixObjDim, m_nFixObjDim )

QT_IMPLEMENT_CHECKBOX( ISMReco::slot, HistEq, m_bPerformHistEq )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, UsePatches, m_bUsePatches )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, Maps, m_bMapsOnManually )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DrawConf, m_bDrawConfidence )
//QT_IMPLEMENT_CHECKBOX( ISMReco::slot, MapThresh, m_bUseMapThresh )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DrawMatchedPs, m_bDrawMatchedPs )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DisplayVS, m_bDisplayVS )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DisplaySupport, m_bDisplaySupport )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DisplaySegment, m_bDisplaySegment )
//QT_IMPLEMENT_CHECKBOX( ISMReco::slot, PatchSizeFact, m_bPatchSizeFactor )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, Figure, m_bOnlyUseFigure )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DrawTightBB, m_bDrawTightBB )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, SavePose, m_bSavePose )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, RefineHypo, m_bRefineHypothesis )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, WriteDetect, m_bWriteDetections )
QT_IMPLEMENT_CHECKBOX( ISMReco::slot, DrawRejected, m_bDrawRejectedHypos )


