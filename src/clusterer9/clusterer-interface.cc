/*********************************************************************/
/*                                                                   */
/* FILE         clusterer-interface.cc                               */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Main framework to test different clustering methods  */
/*              and to create codebooks used for object              */
/*              classification (in clmanager).                       */
/*              This program uses fast codebook generation (more     */
/*              memory).                                             */
/*                                                                   */
/* BEGIN        Fri Sep 27 2002                                      */
/* LAST CHANGE  Fri Mar 19 2004                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
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
#include <qtabwidget.h>

#include <string>
#include <math.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

#include <qtcoordlabel.hh>
#include <qtimgbrowser.hh>
#include <qtclusterview.hh>
#include <opgrayimage.hh>

#include "clusterer.hh"


/*===================================================================*/
/*                         Class MyWidget                            */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

Clusterer::Clusterer( QWidget *parent, const char *name )
        : QWidget( parent, name )
        , m_grayImg()
        , m_qsLastImage(DIR_IMAGES)
        , m_qsLastIDL(DIR_IMAGES)
        , m_sImgName("Img")
        , m_bPicLoaded(false)
        , m_bTestRoutineOn(false)
        , m_bMapsOn( false )
        , m_bCodebook( false )
{
    /********************************/
    /*   create the custom widget   */
    /********************************/
    QVBox *framebox = new QVBox( this, "framebox" );
    framebox->setFrameStyle( QFrame::WinPanel | QFrame::Sunken );

    QVBox *picBox = new QVBox( framebox, "picBox" );

    QHBox *hbImageBox = new QHBox( picBox, "image box");
    QLabel *spacelabel = new QLabel( "", picBox, "spacelabel" );
    rsSourceImg = new QtResizeImg (hbImageBox, "source image");
    rsResultImg = new QtResizeImg (hbImageBox, "result image");


    /****************************************/
    /*   make a "Cluster Centers" display   */
    /****************************************/
    qClassView = new QtClusterView( picBox, "codebook view" );


    /***************************/
    /*   create menu layouts   */
    /***************************/
    QVBoxLayout *menuleftbox  = new QVBoxLayout();
    QVBoxLayout *menurightbox = new QVBoxLayout();
    QVBoxLayout *menulleftbox = new QVBoxLayout();
    menuleftbox->setSpacing( 0 );
    menurightbox->setSpacing( 0 );
    menulleftbox->setSpacing(0);


    /*******************************/
    /*   make a 'Clusterer' Logo   */
    /*******************************/
    QLabel *logo = new QLabel( string(STR_NAME + " " + STR_VERSION).c_str(),
                               this);
    logo->setFont( QFont( "Times", 14, QFont::Bold ) );


    /*=================================================================*/
    /*             L E F T   H A N D   S I D E   M E N U               */
    /*=================================================================*/

    /**********************************/
    /*   make a 'Load Image' button   */
    /**********************************/
    QPushButton *load = new QPushButton( "Load Image", this, "load" );

    menuleftbox->addWidget( load );

    connect( load, SIGNAL(clicked()), this, SLOT(loadImage()) );


    /**************************************************/
    /*   make a 'Load Images For Processing' button   */
    /**************************************************/
    QPushButton *loadmore = new QPushButton( "Load Images For Processing",
                            this, "loadmore" );
    menuleftbox->addWidget( loadmore );

    connect( loadmore, SIGNAL(clicked()), this, SLOT(loadImageSet()) );


     /***************************************/
    /*   make a 'Process IDL File' button   */
    /****************************************/
    QPushButton *loadidl = new QPushButton( "Process IDL File",
                            this, "loadidl" );
    menuleftbox->addWidget( loadidl );

    connect( loadidl, SIGNAL(clicked()), this, SLOT(loadIDLImageSet()) );


   /***************************************/
    /*   make a button 'Collect Patches'   */
    /***************************************/
   // menuleftbox->addSpacing(10);
    QPushButton *colPatches = new QPushButton( "Collect Patches", this,
                              "colpatches" );
    menuleftbox->addWidget( colPatches );

    connect( colPatches, SIGNAL(clicked()), this, SLOT(collectPatches()) );


    /**********************************/
    /* Tab Widget for Interest Points */
    /**********************************/
    FeatureGUI *guiParams = m_fcCue.createGUI( this, "cue1" );
    menuleftbox->addWidget( guiParams );
    //TODO: make connections


    /*****************************************/
    /* Button for removing cluttered patches */
    /*****************************************/
//     menuleftbox->addSpacing(10);
//     QPushButton *pbRemCluttPats = new QPushButton( "Remove Cluttered Patches",
//                                                    this, "remCluttered" );
//     menuleftbox->addWidget( pbRemCluttPats );
//     connect( pbRemCluttPats, SIGNAL(clicked()), 
//              this, SLOT(removeClutteredPatches()) );

//     QHBox* clutterBox = new QHBox(this, "clutterBox");
//     QLabel       *lbClutterThresh  = new QLabel( "Clutter Thresh:", 
//                                                  clutterBox );
//     QLineEdit    *leClutterThresh     = new QLineEdit( "450", clutterBox, 
//                                                        "clutterThresh" );
//     leClutterThresh->setMaximumWidth(50);
//     m_nClutterThresh = atoi( leClutterThresh->text() );
//     QT_CONNECT_LINEEDIT( leClutterThresh, ClutterThresh );

//     menuleftbox->addWidget( clutterBox );

    /***************************************/
    /*   make a 'Cluster Patches' button   */
    /***************************************/
   // menuleftbox->addSpacing(15);
    QPushButton *cluster = new QPushButton( "Cluster Patches", this, "cluster" );

    menuleftbox->addWidget( cluster );

    connect( cluster, SIGNAL(clicked()), this, SLOT(clusterPatches()) );


    /*************************************************************/
    /*   make a 'Load Clustering' and 'Save Clustering' button   */
    /*************************************************************/
    QHBox        *clLoadSave = new QHBox( this, "clLoadSave" );
    QPushButton  *clLoad   = new QPushButton( "&Load", clLoadSave, "loadCl" );
    QPushButton  *clAppend = new QPushButton( "&Append", clLoadSave, "appCl" );
    QPushButton  *clSave   = new QPushButton( "&Save", clLoadSave, "saveCl" );

    //menuleftbox->addWidget( clContinue );
    menuleftbox->addWidget( clLoadSave );

    connect( clLoad, SIGNAL(clicked()), this, SLOT(loadCodebook()) );
    connect( clAppend, SIGNAL(clicked()), this, SLOT(appendCodebook()) );
    connect( clSave, SIGNAL(clicked()), this, SLOT(saveCodebook()) );
    //connect( clContinue, SIGNAL(clicked()), this, SLOT(continueClustering()) );

    /***********************************************/
    /*   make a 'Reconstitute Clustering' button   */
    /***********************************************/
    QPushButton *pbReCluster = new QPushButton( "Recreate Clustering", 
                                                this, "recluster" );

    menulleftbox->addWidget( pbReCluster );

    connect( pbReCluster, SIGNAL(clicked()), 
             this, SLOT(recreateCodebookFromTrace()) );


    /*-------------------------------*/
    /* Check box "Load image patches */
    /*-------------------------------*/
    QCheckBox *chkUsePatches = new QCheckBox( "Load/Use Image Patches", 
                                              this, "usepatches" );
    
    chkUsePatches->setChecked( false );
    m_bUsePatches = chkUsePatches->isChecked();
    
    QT_CONNECT_CHECKBOX( chkUsePatches, UsePatches );
    
    menulleftbox->addWidget( chkUsePatches );


    /*****************************/
    /* Tab Widget for Clustering */
    /*****************************/
    ClusterGUI *guiCluster = m_parCluster.createGUI( this, "cluster" );
    menulleftbox->addWidget( guiCluster );

    m_cbCodebook.setClusterParams( m_parCluster );

    /* also create a matching gui, but don't display it */
    m_parMatching.createGUI( NULL, "matching" );
    
    m_cbCodebook.setMatchingParams( m_parMatching );

    /*=================================================================*/
    /*            R I G H T   H A N D   S I D E   M E N U              */
    /*=================================================================*/

    /**********************************/
    /*   make a 'Remove 1pC' button   */
    /**********************************/
    QPushButton *rem1pc = new QPushButton( "Remove 1P Cls", this, "rem1pc" );
    menurightbox->addWidget( rem1pc );
    connect( rem1pc, SIGNAL(clicked()), this, SLOT(remove1PatchClusters()) );



    /*********************************************/
    /*   make a 'Compute FV Eigenspace' button   */
    /*********************************************/
    menurightbox->addSpacing( 15 );
    QPushButton *compFVES = new QPushButton( "Compute FV Eigenspace",
                            this, "compFVES" );
    menurightbox->addWidget( compFVES );

    connect( compFVES, SIGNAL(clicked()), this, SLOT(computeFVEigenspace()) );


    /*-------------------------------------*/
    /*   make a 'Load Eigenspace' button   */
    /*-------------------------------------*/
    QPushButton *loadES = new QPushButton( "Load Eigenspace Matlab",
                                           this, "loadES" );
    menurightbox->addWidget( loadES );

    connect( loadES, SIGNAL(clicked()), this, SLOT(loadESMatlab()) );


    /***************************************/
    /* Button 'Save FeatureVectors Matlab' */
    /***************************************/
    menurightbox->addSpacing( 15 );
    QPushButton *saveFVMat    = new QPushButton( "Save FV Matlab", this,
                                                 "saveFVMa" );
    QPushButton *savePatchMat = new QPushButton( "Save Patches Matlab", this,
                                                 "savePMa" );
    QPushButton *saveFeatLoc  = new QPushButton( "Save Feature Locations", 
                                                 this, "saveFLoc" );
    QPushButton *saveClassAss = new QPushButton( "Save Class Assignments", 
                                                 this, "saveClass" );
    QPushButton *saveClTrace  = new QPushButton( "Save Cluster Trace", 
                                                 this, "saveClTrace" );

    menurightbox->addWidget( saveFVMat );
    menurightbox->addWidget( savePatchMat );
    menurightbox->addWidget( saveFeatLoc );
    menurightbox->addWidget( saveClassAss );
    menurightbox->addWidget( saveClTrace );

    connect( saveFVMat, SIGNAL(clicked()), this, SLOT(saveFeaturesMatlab()));
    connect( savePatchMat, SIGNAL(clicked()), this, SLOT(savePatchesMatlab()));
    connect( saveFeatLoc, SIGNAL(clicked()), 
             this, SLOT(saveFeatureLocations()));
    connect( saveClassAss, SIGNAL(clicked()), 
             this, SLOT(saveFeatureClasses()));
    connect( saveClTrace, SIGNAL(clicked()), 
             this, SLOT(saveClusterTrace()));

    /*-------------------------------*/
    /* Button 'Save Clusters Matlab' */
    /*-------------------------------*/
    //   QPushButton *saveClMat = new QPushButton( "Save Clusters Matlab", this,
    //                                             "saveClMat" );
    //   menurightbox->addWidget( saveClMat );

    //   connect( saveClMat, SIGNAL(clicked()), this, SLOT(saveClustersMatlab()));

    /*************************************/
    /* Button 'Load Parameters' */
    /*************************************/
    menurightbox->addSpacing( 15 );
    QPushButton *pbLoadParams = new QPushButton( "Load Parameters", this,
                                "loadparams" );
    QPushButton *pbSaveParams = new QPushButton( "Save Parameters", this,
                                "saveparams" );
    menurightbox->addWidget( pbLoadParams );
    menurightbox->addWidget( pbSaveParams );

    connect( pbLoadParams, SIGNAL(clicked()), this, SLOT(loadParams()));
    connect( pbSaveParams, SIGNAL(clicked()), this, SLOT(saveParams()));


    /************************************/
    /* Make a field for the object size */
    /************************************/
    menurightbox->addSpacing( 20 );
    QButtonGroup *bgObjSize = new QButtonGroup( "Fix Object Size:", this, 
                                                "bgO" );
    bgObjSize->setColumnLayout( 2, Qt::Horizontal );
    menurightbox->addWidget( bgObjSize );

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


    /***********************************/
    /*   make a 'Save Images' button   */
    /***********************************/
    menurightbox->addStretch( 630 );
    QPushButton *saveImages = new QPushButton( "Save &Images", this, "quit" );

    menurightbox->addWidget( saveImages );

    connect( saveImages, SIGNAL(clicked()), this, SLOT(saveClusteredImages()) );


    /****************************/
    /*   make a 'Quit' button   */
    /****************************/
    menurightbox->addSpacing( 10 );
    QPushButton *quit = new QPushButton( "&Quit", this, "quit" );
    quit->setFont( QFont( "Times", 10, QFont::Bold ) );

    menurightbox->addWidget( quit );

    connect( quit, SIGNAL(clicked()), qApp, SLOT(quit()) );



    /*=================================================================*/
    /*                     O T H E R   S T U F F                       */
    /*=================================================================*/

    /***********************************/
    /*   make a "coordinate display"   */
    /***********************************/
    QHBox        *hbox = new QHBox(this, "hbox");
    QtCoordLabel *coordPos = new QtCoordLabel( "Pos:", hbox, "coord_pos" );

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
    menuleftbox->addStretch(500);

    QGridLayout *grid = new QGridLayout( this, 2, 4, 10 );
    grid->addWidget( logo, 0, 0, Qt::AlignHCenter );
    grid->addLayout( menuleftbox, 1, 0 );
    grid->addWidget( hbox, 0, 1, Qt::AlignHCenter );
    //framebox contains image
    grid->addLayout( menulleftbox, 1, 3);
    grid->addWidget( framebox, 1, 1 );
    grid->addLayout( menurightbox, 1, 2 );
    grid->setColStretch( 1, 10 );
}

void Clusterer::saveParams( bool bVerbose )
{
  QString qsFileName = QFileDialog::getSaveFileName( DIR_CB_SAVED, 
                                       "Parameter file (*.params)", this);
  if ( !qsFileName.isEmpty() )
    saveParams( qsFileName, bVerbose );
}

void Clusterer::saveParams( string sFileName, bool bVerbose )
{
  QFile qfile( sFileName );
  if ( qfile.open(IO_WriteOnly) ) // | IO_Append ) ) don't append
    {
      QTextStream stream( &qfile );
      stream << "\n*** Section clusterer-interface ***\n";
      //<< "m_nClutterThresh: " << m_nClutterThresh << "\n";

      /* also save the relevant information for scmatcher */
      stream << "\n*** Section scmatcher-interface ***\n"
        //-- comparison --//
             << "m_dRejectionThresh: " 
             << m_parCluster.params()->m_dSimilarity << "\n"
             << "m_dFeatureSimFact: " 
             << m_parCluster.params()->m_dFeatureSimFact << "\n";
      qfile.close();
    }
  m_fcCue.params()->saveParams( sFileName.c_str(), bVerbose );
  m_parCluster.params()->saveParams( sFileName.c_str(), bVerbose );
  m_parMatching.params()->saveParams( sFileName.c_str(), bVerbose );
}


void Clusterer::loadParams( bool bVerbose )
{
  QString qsFileName = QFileDialog::getOpenFileName( DIR_CB_SAVED, 
                                       "Parameter file (*.params)", this);
  if (!qsFileName.isEmpty())
    loadParams( qsFileName.latin1(), bVerbose );
}


void Clusterer::loadParams( string sFileName, bool bVerbose )
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
            if(line.compare("*** Section clusterer-interface ***")==0)
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
        
//         if (name.compare("m_nClutterThresh")==0)
//           emit sigClutterThreshChanged(val);
//         else
//           cerr << "XXXXXXXXXXXXXXX     WARNING: variable " << name.latin1()
//                << " unknown !!!     XXXXXXXXXXXXXXXXXX" << endl;
      }
    qfile.close();
  }
  m_fcCue.params()->loadParams( sFileName.c_str(), bVerbose );
  m_parCluster.params()->loadParams( sFileName.c_str(), bVerbose );
  m_parMatching.params()->loadParams( sFileName.c_str(), bVerbose );
}


/***********************************************************/
/*                   Interface Handlers                    */
/***********************************************************/

//QT_IMPLEMENT_LINEEDIT_INT( Clusterer::slot, ClutterThresh, m_nClutterThresh )
QT_IMPLEMENT_LINEEDIT_INT( Clusterer::slot, ObjWidth, m_nObjWidth )
QT_IMPLEMENT_LINEEDIT_INT( Clusterer::slot, ObjHeight, m_nObjHeight )

QT_IMPLEMENT_RADIOBUTTON( Clusterer::slot, FixObjDim, m_nFixObjDim )

QT_IMPLEMENT_CHECKBOX( Clusterer::slot, UsePatches, m_bUsePatches )
