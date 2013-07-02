/*********************************************************************/
/*                                                                   */
/* FILE         featuregui.cc                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      GUI for the feature extraction parameters.           */
/*                                                                   */
/* BEGIN        Thu Mar 25 2004                                      */
/* LAST CHANGE  Tue Feb 20 2013                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <qwidget.h>
#include <qpushbutton.h>
#include <q3vbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qcolor.h>
#include <q3buttongroup.h>
#include <qtabwidget.h>
#include <qfile.h>

#include <qtmacros.hh>
//Added by qt3to4:
#include <Q3TextStream>
#include <Q3VBoxLayout>

#include "featuregui.hh"



/*===================================================================*/
/*                        Class FeatureGUI                           */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

FeatureGUI::FeatureGUI( QWidget *parent, const char *name )
        : QWidget( parent, name )
{
    Q3VBoxLayout *menuleftbox  = new Q3VBoxLayout( this );

    /***************************************/
    /*   Tab Widget for Patch Extraction   */
    /***************************************/
    menuleftbox->addSpacing(5);
    QTabWidget *tabIntPts = new QTabWidget( this, "intpts" );

    /* tabs: detector, pHarris */
    QWidget     *tabwExtract   = new QWidget( this, "tabwExtract" );
    QWidget     *tabwFeatures  = new QWidget( this, "tabwFeatures" );
    QWidget     *tabwHarParam  = new QWidget( this, "tabwHarParam" );
    QWidget     *tabwScale     = new QWidget( this, "tabwScale" );
    Q3VBoxLayout *tabpExtract   = new Q3VBoxLayout( tabwExtract );
    Q3VBoxLayout *tabpFeatures  = new Q3VBoxLayout( tabwFeatures );
    Q3VBoxLayout *tabpHarParam  = new Q3VBoxLayout( tabwHarParam );
    Q3VBoxLayout *tabpScale     = new Q3VBoxLayout( tabwScale );
    tabpExtract->setSpacing( 0 );
    tabpFeatures->setSpacing( 0 );
    tabpHarParam->setSpacing( 0 );
    tabpScale->setSpacing( 0 );


    /****************************************************/
    /*   make parameter fields for 'Patch extraction'   */
    /****************************************************/
    tabpExtract->addSpacing( 5 );
    Q3ButtonGroup *bgExtract = new Q3ButtonGroup("Patch Extraction:",
                              tabwExtract, "bgExtract");
    bgExtract->setColumnLayout( 1, Qt::Horizontal );
    tabpExtract->addWidget( bgExtract );

    /* make an option 'uniform with step size' */
    selUniform      = new QRadioButton( "uniform", bgExtract,
                                        "selUniform" );
    Q3HBox        *hbStepSize  = new Q3HBox( bgExtract, "hbStepSz" );
    QLabel       *labStepSize = new QLabel( "Step Size:", hbStepSize );
    QLineEdit    *editStepSize = new QLineEdit( "2", hbStepSize,
                                 "editStepSize" );
    editStepSize->setMaximumWidth(50);
    m_nStepSize = atoi( editStepSize->text() );
    QT_CONNECT_LINEEDIT( editStepSize, StepSize );


    selHarris       = new QRadioButton( "Harris", bgExtract,
                                        "selHarris" );
    selHarLap       = new QRadioButton( "Harris-Laplace",
                                        bgExtract, "selHarLap" );
    selHesLap       = new QRadioButton( "Hessian-Laplace",
                                        bgExtract, "selHesLap");
    selLDoG         = new QRadioButton( "Lowe's DoG", bgExtract,
                                        "selLDoG" );
    selExactDoG     = new QRadioButton( "Exact DoG", bgExtract,
                                        "selExDoG" );
    selHarAff       = new QRadioButton( "Harris-Affine",
                                        bgExtract, "selHarAff" );
    selHesAff       = new QRadioButton( "Hessian-Affine",
                                        bgExtract, "selHesAff");
    selMSER         = new QRadioButton( "MSER",
                                        bgExtract, "selMSER" );
    selHarHesLap    = new QRadioButton( "Harris-Laplace + Hessian-Laplace",
                                        bgExtract, "selHarHesLap" );
    selMSER2        = new QRadioButton( "MSER un-affine", bgExtract,
                                         "selMSER2" );
    selSURF         = new QRadioButton( "SURF", bgExtract,
                                         "selSURF" );
    selEdgeLap      = new QRadioButton( "EdgeLap", bgExtract,
                                         "selEdgeLap" );
	selRandom       = new QRadioButton( "Random", bgExtract,
										"selRandom");
	//selRandom->setChecked( true );
    //selExactDoG->setChecked( true );
	selHesLap->setChecked( true );
    m_nPatchExtMethod = PATCHEXT_HESLAP;

    QT_CONNECT_RADIOBUTTON( bgExtract, PatchExtMethod );


    /*-------------------------------------*/
    /*   add checkboxes for figure pixels  */
    /*-------------------------------------*/
    chkUseFigureOnly = new QCheckBox( "Use only figure area",
                                      tabwExtract, "chkfigureonly" );
    chkUseFigureOnly->setChecked(false);
    m_bUseFigureOnly = chkUseFigureOnly->isChecked();

    tabpExtract->addWidget( chkUseFigureOnly );

    QT_CONNECT_CHECKBOX( chkUseFigureOnly, UseFigureOnly );

    chkExtractFromWholeImage = new QCheckBox( "Extract from whole image",
											  tabwExtract, "chkextractfromwholeimage");
	chkExtractFromWholeImage->setChecked(false);
	m_bExtractFromWholeImage = chkExtractFromWholeImage->isChecked();
	tabpExtract->addWidget( chkExtractFromWholeImage );
	QT_CONNECT_CHECKBOX( chkExtractFromWholeImage, ExtractFromWholeImage);


    /*****************************************/
    /*   make parameter fields for Features  */
    /*****************************************/
    tabpFeatures->addSpacing( 5 );
    Q3ButtonGroup *bgFeature = new Q3ButtonGroup("Feature Type:",
                              tabwFeatures, "bgFeature");
    bgFeature->setColumnLayout( 1, Qt::Horizontal );
    tabpFeatures->addWidget( bgFeature );

    selPatches          = new QRadioButton( "Patches",
                                            bgFeature, "chkpat" );
    selPatchMiko        = new QRadioButton( "Sampled Patches [Mikolajczyk]",
                                            bgFeature, "chkpatm" );
    selSteerable        = new QRadioButton( "Steerable Filters",
                                            bgFeature, "chksf" );
    selSIFT             = new QRadioButton( "SIFT [Lowe]",
                                            bgFeature, "chksift" );
    selGLOH             = new QRadioButton( "GLOH [Mikolajczyk]",
                                            bgFeature, "chkgloh" );
    selMoments          = new QRadioButton( "Moments",
                                            bgFeature, "chkmom" );
    selDiffInv          = new QRadioButton( "Differential Invariants",
                                            bgFeature, "chkdiffinv" );
    selComplex          = new QRadioButton( "Complex Filters [Schaffalitzky]",
                                            bgFeature, "chkcf" );
    selShapeContext     = new QRadioButton( "Shape Context",
                                            bgFeature, "chksc" );
    selSpinImgs         = new QRadioButton( "Spin Images",
                                            bgFeature, "chkspin" );
    selGradPCA          = new QRadioButton( "Gradient PCA [Ke]",
                                            bgFeature, "chkgpca" );
    selPatchMiko2       = new QRadioButton( "Sampled Patches2 [Mikolajczyk]",
                                            bgFeature, "chkpatm2" );
    selSURF64           = new QRadioButton( "SURF-64  [Bay]",
                                            bgFeature, "chksurf64" );
    selSURF128          = new QRadioButton( "SURF-128 [Bay]",
                                            bgFeature, "chksurf128" );
	selSIFT->setChecked( true );
    //selPatches->setChecked( true );
    m_nFeatureType = FEATURE_SIFT;
    QT_CONNECT_RADIOBUTTON( bgFeature, FeatureType );


    /*-----------------------------------------*/
    /*   add checkbox for rotation invariance  */
    /*-----------------------------------------*/
    chkRotInv  = new QCheckBox( "Make Rotation Invariant",
                                tabwFeatures, "chkrotinv" );

    chkRotInv->setChecked(false);
    m_bMakeRotInv = chkRotInv->isChecked();

    tabpFeatures->addWidget( chkRotInv );

    QT_CONNECT_CHECKBOX( chkRotInv, MakeRotInv );


    /***************************************************/
    /*   make parameter fields for 'Harris Detector'   */
    /***************************************************/
    tabpHarParam->addSpacing( 5 );
    Q3ButtonGroup *bgHarParam = new Q3ButtonGroup("Harris:",
                               tabwHarParam, "bgHarParams");
    bgHarParam->setColumnLayout( 2, Qt::Horizontal );
    tabpHarParam->addWidget( bgHarParam );

    QLabel      *labSigma1     = new QLabel( "Sigma1:", bgHarParam );
    QLineEdit   *editSigma1    = new QLineEdit( "3.00", bgHarParam, "edSigma1");
    QLabel      *labSigma2     = new QLabel( "Sigma2:", bgHarParam );
    QLineEdit   *editSigma2    = new QLineEdit( "2.00", bgHarParam, "edSigma2");
    QLabel      *labAlpha      = new QLabel( "Alpha:", bgHarParam );
    QLineEdit   *editAlpha     = new QLineEdit( "0.06", bgHarParam, "edAlpha");
    QLabel      *labThreshHar  = new QLabel( "Thresh:", bgHarParam );
    QLineEdit   *editThreshHar = new QLineEdit( "100.0", bgHarParam, "edThresh");

    editSigma1->setMaximumWidth(50);
    editSigma2->setMaximumWidth(50);
    editAlpha->setMaximumWidth(50);
    editThreshHar->setMaximumWidth(50);
    m_dSigma1    = atof ( editSigma1->text() );
    m_dSigma2    = atof ( editSigma2->text() );
    m_dAlpha     = atof ( editAlpha->text() );
    m_dThreshHar = atof ( editThreshHar->text() );

    QT_CONNECT_LINEEDIT( editSigma1, Sigma1 );
    QT_CONNECT_LINEEDIT( editSigma2, Sigma2 );
    QT_CONNECT_LINEEDIT( editAlpha, Alpha );
    QT_CONNECT_LINEEDIT( editThreshHar, ThreshHar );


    /*----------------------------------------------*/
    /*   make parameter fields for 'DoG Detector'   */
    /*----------------------------------------------*/
    tabpHarParam->addSpacing( 5 );
    Q3ButtonGroup *bgDoGParam = new Q3ButtonGroup("Exact DoG:", tabwHarParam, "bgDoGParams");
    bgDoGParam->setColumnLayout( 2, Qt::Horizontal );
    tabpHarParam->addWidget( bgDoGParam );

    QLabel      *labOctaves    = new QLabel( "Scale Octaves: ", bgDoGParam );
    QLineEdit   *edOctaves     = new QLineEdit( "5", bgDoGParam, "edoctaves" );
    QLabel      *labLevPerOct  = new QLabel( "Levels/Octave: ", bgDoGParam );
    QLineEdit   *edLevPerOct   = new QLineEdit( "3", bgDoGParam, "edlevoct" );
    QLabel      *labScSigma0   = new QLabel( "Sigma0: ", bgDoGParam );
    QLineEdit   *edScSigma0    = new QLineEdit( "1.00", bgDoGParam, "edscsig0" );
    QLabel      *labThreshEdog = new QLabel( "Threshold: ", bgDoGParam );
    QLineEdit   *edThreshEdog  = new QLineEdit( "10.0", bgDoGParam, "edthresh" );

    edOctaves->setMaximumWidth(50);
    edLevPerOct->setMaximumWidth(50);
    edScSigma0->setMaximumWidth(50);
    edThreshEdog->setMaximumWidth(50);
    m_nScaleOctaves  = atoi ( edOctaves->text() );
    m_nLevsPerOctave = atoi ( edLevPerOct->text() );
    m_dScaleSigma0   = atof ( edScSigma0->text() );
    m_dThreshEdog    = atof ( edThreshEdog->text() );

    QT_CONNECT_LINEEDIT( edOctaves, ScaleOctaves );
    QT_CONNECT_LINEEDIT( edLevPerOct, LevsPerOctave );
    QT_CONNECT_LINEEDIT( edScSigma0, ScaleSigma0 );
    QT_CONNECT_LINEEDIT( edThreshEdog, ThreshEdog );

    /*--------------------------------------*/
    /*   make parameter fields for 'SURF'   */
    /*--------------------------------------*/
    Q3ButtonGroup *bgSURFParams = new Q3ButtonGroup("SURF:", tabwHarParam, 
                                                  "surfParams");
    bgSURFParams->setColumnLayout( 2, Qt::Horizontal );
    tabpHarParam->addWidget( bgSURFParams );
    QLabel      *lbThreshSURF = new QLabel( "Threshold: ", bgSURFParams );
    QLineEdit   *edThreshSURF = new QLineEdit( "1000", bgSURFParams, 
                                               "tsurf" );

    edThreshSURF->setMaximumWidth(50);
    m_dThreshSURF = atof( edThreshSURF->text() );
    QT_CONNECT_LINEEDIT( edThreshSURF, ThreshSURF );

    /*--------------------------------------*/
    /*   make parameter fields for 'Misc'   */
    /*--------------------------------------*/
    Q3ButtonGroup *bgMiscParams = new Q3ButtonGroup("Misc:", tabwHarParam, 
                                                  "miscParams");
    bgMiscParams->setColumnLayout( 2, Qt::Horizontal );
    tabpHarParam->addWidget( bgMiscParams );
    QLabel      *labMinFigurePixels = new QLabel( "min. figure pixels: ", bgMiscParams );
    QLineEdit   *edMinFigurePixels = new QLineEdit( "50", bgMiscParams, 
                                                       "minfigpixels" );
    QLabel      *labFigureThresh = new QLabel( "Fig. Thresh.: ", bgMiscParams);
    QLineEdit   *edFigureThresh  = new QLineEdit( "220", bgMiscParams, 
                                                  "edfigth");

    edMinFigurePixels->setMaximumWidth(50);
    edFigureThresh->setMaximumWidth(50);
    m_nMinFigurePixels = atoi( edMinFigurePixels->text() );
    m_nFigureThresh      = atoi( edFigureThresh->text() );
    QT_CONNECT_LINEEDIT( edMinFigurePixels, MinFigurePixels );
    QT_CONNECT_LINEEDIT( edFigureThresh, FigureThresh );

    /*******************************************/
    /*   make parameter fields for 'Patches'   */
    /*******************************************/
    tabpScale->addSpacing( 5 );
    Q3ButtonGroup *bgScale = new Q3ButtonGroup("Parameters:",
                            tabwScale, "bgScale");
    bgScale->setColumnLayout( 2, Qt::Horizontal );
    tabpScale->addWidget( bgScale );

    QLabel      *labPatchSize  = new QLabel( "Patch Size:", bgScale );
    QLineEdit   *editPatchSize = new QLineEdit( "8", bgScale, "edPatchSize");
    QLabel      *labMinScale   = new QLabel( "Min. Scale:", bgScale );
    QLineEdit   *editMinScale  = new QLineEdit( "1.9", bgScale, "edMinScale");
    QLabel      *labMaxScale   = new QLabel( "Max. Scale:", bgScale );
    QLineEdit   *editMaxScale  = new QLineEdit( "16.0", bgScale, "edMaxScale");

    editPatchSize->setMaximumWidth(50);
    editMinScale->setMaximumWidth(50);
    editMaxScale->setMaximumWidth(50);
    m_nPatchSize   = atoi ( editPatchSize->text() );
    m_dMinScale    = atof ( editMinScale->text() );
    m_dMaxScale    = atof ( editMaxScale->text() );
    m_dScaleFactor = 3.0;

    QT_CONNECT_LINEEDIT( editPatchSize, PatchSize );
    QT_CONNECT_LINEEDIT( editMinScale, MinScale );
    QT_CONNECT_LINEEDIT( editMaxScale, MaxScale );


    /*-----------------------------------------*/
    /*   make checkbox for 'Smoothe Patches'   */
    /*-----------------------------------------*/
    chkFilterPatches = new QCheckBox( "Filter Patches w/ Gauss 1.0",
                                      tabwScale, "chkfiltpat" );
    tabpScale->addWidget( chkFilterPatches );

    chkFilterPatches->setChecked( true );
    m_bFilterPatches = chkFilterPatches->isChecked();

    QT_CONNECT_CHECKBOX( chkFilterPatches, FilterPatches );


    /*************************************************/
    /*   make parameter fields for 'Normalization'   */
    /*************************************************/
    //menuleftbox->addSpacing( 10 );
    //   QButtonGroup *bgNorm = new QButtonGroup( "Normalization:", this, "bgNorm" );
    //   bgNorm->setColumnLayout( 1, Qt::Horizontal );

    //   QRadioButton *selNormNone = new QRadioButton( "none",
    //                                                 bgNorm,"selNormNone" );
    //   QRadioButton *selNormStdDev= new QRadioButton( "by std dev.",
    //                      1                           bgNorm,"selNormStdDev" );
    //   QRadioButton *selNormVar  = new QRadioButton( "by variance",
    //                                                 bgNorm,"selNormVar" );
    //   QRadioButton *selNormEnerg= new QRadioButton( "by energy",
    //                                                 bgNorm,"selNormEnerg" );

    //   selNormStdDev->setChecked( true );
    m_nNormalizeMethod = NORM_STDDEV;
    //   QT_CONNECT_RADIOBUTTON( bgNorm, NormalizeMethod );

    //   menuleftbox->addWidget( bgNorm );


    /*************************************************/
    /*   Finish the tab Widget for PatchExtraction   */
    /*************************************************/
    tabpExtract->addStretch( 200 );
    tabpFeatures->addStretch( 200 );
    tabpHarParam->addStretch( 50 );
    tabpScale->addStretch( 50 );

    tabIntPts->addTab( tabwExtract,  "Detector"  );
    tabIntPts->addTab( tabwFeatures,  "Features"  );
    tabIntPts->addTab( tabwHarParam, "Param." );
    tabIntPts->addTab( tabwScale,    "Scale"  );

    menuleftbox->addWidget( tabIntPts );


}

void FeatureGUI::saveParams( string sFileName, bool bVerbose )
{
  if( bVerbose )
    cout << "    FeatureGUI::saveParams() called..." << endl;
  
  QFile qfile( sFileName.c_str() );
  if ( qfile.open( QIODevice::WriteOnly | QIODevice::Append ) )
    {
      Q3TextStream stream( &qfile );
      
      stream << "\n*** Section FeatureGUI ***\n"
        //--- Detector tab ---//
             << "m_nPatchExtMethod: " << m_nPatchExtMethod << "\n"
             << "m_nStepSize: " << m_nStepSize << "\n"
             << "m_bUseFigureOnly: " << m_bUseFigureOnly << "\n"
        //--- Features tab ---//
             << "m_nFeatureType: " << m_nFeatureType << "\n"
             << "m_bMakeRotInv: " << m_bMakeRotInv << "\n"
        //--- Param tab ---//
             << "m_dSigma1: " << m_dSigma1 << "\n"
             << "m_dSigma2: " << m_dSigma2 << "\n"
             << "m_dAlpha: " << m_dAlpha << "\n"
             << "m_dThreshHar: " << m_dThreshHar << "\n"
             << "m_nScaleOctaves: " << m_nScaleOctaves << "\n"
             << "m_nLevsPerOctave: " << m_nLevsPerOctave << "\n"
             << "m_dScaleSigma0: " << m_dScaleSigma0 << "\n"
             << "m_dThreshEdog: " << m_dThreshEdog << "\n"
             << "m_dThreshSURF: " << m_dThreshSURF << "\n"
             << "m_nMinFigurePixels: " << m_nMinFigurePixels << "\n"
             << "m_nFigureThresh: " << m_nFigureThresh << "\n"
        //--- Scale tab ---//
             << "m_nPatchSize: " << m_nPatchSize << "\n"
             << "m_dMinScale: " << m_dMinScale << "\n"
             << "m_dMaxScale: " << m_dMaxScale << "\n"
             << "m_bFilterPatches: " << m_bFilterPatches << "\n";
      //<< "m_nNormalizeMethod: " << m_nNormalizeMethod << "\n";
      //<< "m_nPatchResolution: " << m_nPatchResolution << "\n"
      qfile.close();
    }
}

void FeatureGUI::loadParams( string sFileName, bool bVerbose )
{
  if( bVerbose )
    cout << "    FeatureGUI::loadParams() called..." << endl;
  
  QFile qfile( sFileName.c_str() );
  if ( qfile.open( QIODevice::ReadOnly ) ) {
    Q3TextStream stream( &qfile );
    QString line;
    bool started = false;
    
    while(!(line = stream.readLine()).isNull())
      {
        if (!started || line.isEmpty())
          {
            if(line.compare("*** Section FeatureGUI ***")==0)
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
          printf("  line: %s\n",line.latin1());
        
        QString name = line.left(pos);
        QString val = line.right(len-pos-2);
        
        //--------------------//
        //--- Detector tab ---//
        //--------------------//
        if (name.compare("m_nPatchExtMethod")==0)
          {
            m_nPatchExtMethod = val.toInt();
            switch (m_nPatchExtMethod)
              {
              case PATCHEXT_UNIFORM:
                selUniform->setChecked(true);
                break;
              case PATCHEXT_HARRIS:
                selHarris->setChecked(true);
                break;
              case PATCHEXT_HARLAP:
                selHarLap->setChecked(true);
                break;
              case PATCHEXT_HESLAP:
                selHesLap->setChecked(true);
                break;
              case PATCHEXT_LDOG:
                selLDoG->setChecked(true);
                break;
              case PATCHEXT_EXACTDOG:
                selExactDoG->setChecked(true);
                break;
              case PATCHEXT_HARAFF:
                selHarAff->setChecked(true);
                break;
              case PATCHEXT_HESAFF:
                selHesAff->setChecked(true);
                break;
              case PATCHEXT_MSER:
                selMSER->setChecked(true);
                break;
              case PATCHEXT_HARHESLAP:
                selHarHesLap->setChecked(true);
                break;
              case PATCHEXT_MSER2:
                selMSER2->setChecked(true);
                break;
              case PATCHEXT_SURF:
                selSURF->setChecked(true);
                break;
              case PATCHEXT_EDGELAP:
                selEdgeLap->setChecked(true);
                break;
              case PATCHEXT_RANDOM:
				selRandom->setChecked(true);
				break;
              default:
                cerr << "*********   ERROR: Patch Extraction Method unknown: "
                     << m_nPatchExtMethod << "!!!    *********" << endl;
              }
            if( bVerbose )
              printf("   val: %s -> setting %d\n", val.latin1(), val.toInt());
          }
        else if (name.compare("m_nStepSize")==0)
          emit sigStepSizeChanged(val);
        else if (name.compare("m_bUseFigureOnly")==0)
          {
            chkUseFigureOnly->setChecked((bool)val.toInt());
            slotSetUseFigureOnlyOnOff(val.toInt());
          }
        //--------------------//
        //--- Features tab ---//
        //--------------------//
        else if (name.compare("m_nFeatureType")==0)
          {
            m_nFeatureType = val.toInt();
            switch (m_nFeatureType)
              {
              case FEATURE_PATCH:
                selPatches->setChecked(true);
                break;
              case FEATURE_PATCHMIKO:
                selPatchMiko->setChecked(true);
                break;
              case FEATURE_STEERABLE:
                selSteerable->setChecked(true);
                break;
              case FEATURE_SIFT:
                selSIFT->setChecked(true);
                break;
              case FEATURE_GLOH:
                selGLOH->setChecked(true);
                break;
              case FEATURE_MOMENTS:
                selMoments->setChecked(true);
                break;
              case FEATURE_DIFFINV:
                selDiffInv->setChecked(true);
                break;
              case FEATURE_COMPLEX:
                selComplex->setChecked(true);
                break;
              case FEATURE_SHAPECONTEXT:
                selShapeContext->setChecked(true);
                break;
              case FEATURE_SPINIMGS:
                selSpinImgs->setChecked(true);
                break;
              case FEATURE_GRADIENTPCA:
                selGradPCA->setChecked(true);
                break;
              case FEATURE_PATCHMIKO2:
                selPatchMiko2->setChecked(true);
                break;
               case FEATURE_SURF64:
                selSURF64->setChecked(true);
                break;
               case FEATURE_SURF128:
                selSURF128->setChecked(true);
                break;
             default:
                cerr << "*********   ERROR: Feature Type unknown: "
                     << m_nFeatureType << "!!!    *********\n" << endl;
              }
            if( bVerbose )
              printf("   val: %s -> setting %d\n", val.latin1(), val.toInt());
          }
         else if (name.compare("m_bMakeRotInv")==0)
          {
            chkRotInv->setChecked((bool)val.toInt());
            slotSetMakeRotInvOnOff(val.toInt());
          }
        //-----------------//
        //--- Param tab ---//
        //-----------------//
        else if (name.compare("m_dSigma1")==0)
          emit sigSigma1Changed(val);
        else if (name.compare("m_dSigma2")==0)
          emit sigSigma2Changed(val);
        else if (name.compare("m_dAlpha")==0)
          emit sigAlphaChanged(val);
        else if (name.compare("m_dThreshHar")==0)
          emit sigThreshHarChanged(val);
        else if (name.compare("m_nScaleOctaves")==0)
          emit sigScaleOctavesChanged(val);
        else if (name.compare("m_nLevsPerOctave")==0)
          emit sigLevsPerOctaveChanged(val);
        else if (name.compare("m_dScaleSigma0")==0)
          emit sigScaleSigma0Changed(val);
        else if (name.compare("m_nLevsPerOctave")==0)
          emit sigLevsPerOctaveChanged(val);
        else if (name.compare("m_dThreshEdog")==0)
          emit sigThreshEdogChanged(val);
         else if (name.compare("m_dThreshSURF")==0)
          emit sigThreshSURFChanged(val);
       else if (name.compare("m_nMinFigurePixels")==0)
          emit sigMinFigurePixelsChanged(val);
        else if (name.compare("m_nFigureThresh")==0)
          emit sigFigureThreshChanged(val);
				
        //-----------------//
        //--- Scale tab ---//
        //-----------------//
        else if (name.compare("m_nPatchSize")==0)
          emit sigPatchSizeChanged(val);
        //else if (name.compare("m_nPatchResolution")==0)
        //	emit sig
        else if (name.compare("m_dMinScale")==0)
          emit sigMinScaleChanged(val);
        else if (name.compare("m_dMaxScale")==0)
          emit sigMaxScaleChanged(val);
        else if (name.compare("m_bFilterPatches")==0)
          {
            chkFilterPatches->setChecked((bool)val.toInt());
            emit slotSetFilterPatchesOnOff(val.toInt());
          }
        
        else
          cerr << "XXXXXXXXXXXXXXX     WARNING: variable " << name.latin1() 
               << " unknown !!!     XXXXXXXXXXXXXXXXXX" << endl;
      }
    qfile.close();
  }
}

QT_IMPLEMENT_LINEEDIT_FLOAT( FeatureGUI::slot, Sigma1, m_dSigma1, 3 )
QT_IMPLEMENT_LINEEDIT_FLOAT( FeatureGUI::slot, Sigma2, m_dSigma2, 3 )
QT_IMPLEMENT_LINEEDIT_FLOAT( FeatureGUI::slot, Alpha, m_dAlpha, 2 )
QT_IMPLEMENT_LINEEDIT_FLOAT( FeatureGUI::slot, ThreshHar, m_dThreshHar, 1 )

QT_IMPLEMENT_LINEEDIT_INT( FeatureGUI::slot, ScaleOctaves, m_nScaleOctaves )
QT_IMPLEMENT_LINEEDIT_INT( FeatureGUI::slot, LevsPerOctave, m_nLevsPerOctave )
QT_IMPLEMENT_LINEEDIT_FLOAT( FeatureGUI::slot, ScaleSigma0, m_dScaleSigma0, 1 )
QT_IMPLEMENT_LINEEDIT_FLOAT( FeatureGUI::slot, ThreshEdog, m_dThreshEdog, 1 )
QT_IMPLEMENT_LINEEDIT_FLOAT( FeatureGUI::slot, ThreshSURF, m_dThreshSURF, 1 )
QT_IMPLEMENT_LINEEDIT_INT( FeatureGUI::slot, MinFigurePixels, m_nMinFigurePixels )
QT_IMPLEMENT_LINEEDIT_INT( FeatureGUI::slot, FigureThresh, m_nFigureThresh )

QT_IMPLEMENT_LINEEDIT_INT( FeatureGUI::slot, PatchSize, m_nPatchSize )
QT_IMPLEMENT_LINEEDIT_INT( FeatureGUI::slot, StepSize, m_nStepSize )
QT_IMPLEMENT_LINEEDIT_FLOAT( FeatureGUI::slot, MinScale, m_dMinScale, 1 )
QT_IMPLEMENT_LINEEDIT_FLOAT( FeatureGUI::slot, MaxScale, m_dMaxScale, 1 )
QT_IMPLEMENT_CHECKBOX( FeatureGUI::slot, FilterPatches, m_bFilterPatches )
QT_IMPLEMENT_CHECKBOX( FeatureGUI::slot, MakeRotInv, m_bMakeRotInv )

QT_IMPLEMENT_RADIOBUTTON( FeatureGUI::slot, PatchExtMethod, m_nPatchExtMethod )
QT_IMPLEMENT_RADIOBUTTON( FeatureGUI::slot, FeatureType, m_nFeatureType )
QT_IMPLEMENT_CHECKBOX( FeatureGUI::slot, UseFigureOnly, m_bUseFigureOnly )
QT_IMPLEMENT_CHECKBOX( FeatureGUI::slot, ExtractFromWholeImage, m_bExtractFromWholeImage )
QT_IMPLEMENT_RADIOBUTTON( FeatureGUI::slot, NormalizeMethod, m_nNormalizeMethod)
