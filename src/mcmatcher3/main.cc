/*********************************************************************/
/*                                                                   */
/* FILE         main.cc                                              */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      main function - calls up a Qt window                 */
/*                                                                   */
/* BEGIN        Wed Aug 15 2001                                      */
/* LAST CHANGE  Tue Non 05 2002                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <qapplication.h>
#include <qdir.h>

#include <qtimgbrowser.hh>
#include <imgdescrlist.hh>
#include "mcmatcher.hh"

const QString usage =
    "USAGE: mcmatcher [OPTIONS]\n\n \
    -nw       : no gui\n \
    -par FILE : load parameters from FILE\n \
    -t   T    : set recognition threshold to T\n \
    -minsc S  : set min detection scale to S (rel. to training scale)\n \
    -maxsc S  : set max detection scale to S (rel. to training scale)\n \
    -nomdl    : disable MDL hypothesis selection stage\n \
    -imagesc S: image scale factor for ground plane calculation\n \
    -worldsc S: world scale factor for ground plane calculation\n \
    -det FILE : load detector from FILE\n \
                (can occur several times to add more detectors)\n \
    -img FILE : process single image from FILE\n \
    -idl FILE : process a set of test images from IDL FILE\n \
    -out FILE : result IDL file for output\n \
    -odir DIR : result directory for detailed output\n \
    -timings F: enable (F=1) or disable (F=0) timing output\n \
    -q        : quiet mode (no text output)\n \
    -v        : verbose output\n \
    -vv       : very verbose output\n \
    \n";

int main( int argc, char **argv )
{
  QApplication::setColorSpec( QApplication::CustomColor );
  QApplication a( argc, argv );
  
  ISMReco w;
  w.setGeometry( 100, 100, 1000, 800 );

  bool   bDisplayGUI = true;
  string sParamFile  = "default.params";
  float  t           = 400.0;
  bool   bSetScale   = false;
  float  minsc       = 0.3;
  float  maxsc       = 2.5;
  float  imagesc     = 1.0;
  float  worldsc     = 1.0;
  bool   bUseMDL     = true;
  string sInputImg   = "test.png";
  bool   bProcessImg = false;
  string sInputIDL   = "test.idl";
  bool   bProcessIDL = false;
  string sResultIDL  = "result.idl";
  bool   bUseResIDL  = false;
  string sResultDir  = "./results";
  bool   bUseResDir  = false;
  bool   bShowTimings= true;
  bool   bQuiet      = false;
  bool   bVerbose    = false;
  bool   bVeryVerbose= false;

  vector<string> vDetFiles;

  //QtImgBrowser::verbosity =0;
  for(int i=1; i<argc; i++) {
    //printf("argv[%d]: %s\n", i, argv[i]);
    
    //--- Help ---//
    if (strcmp(argv[i],"-h")==0 || strcmp(argv[i],"-help")==0 || 
        strcmp(argv[i], "--help")==0)
      {
        printf("%s v%s (%s)\n", STR_NAME.c_str(), STR_VERSION.c_str(), 
               STR_DATE.c_str());
        printf("%s\n\n", STR_COPYRIGHT.c_str());
        printf("%s\n", STR_LICENSE.c_str());
        printf("\n%s", usage.latin1());
        exit(0);
      }

    //--- GUI? ---//
    if (strcmp(argv[i],"-nw")==0)
      {
        QtImgBrowser::verbosity = 0;
        bDisplayGUI = false;
      }
    
    //--- Load Parameters ---//
    if (strcmp(argv[i],"-par")==0 && argc>i+1)
      {
        //w.loadParams( string(argv[i+1]) );
        cerr << "WARNING: Parameter file not implemented yet => ignoring..."
             << endl;
        i++;
      }

    //--- Set Reco Threshold ---//
    if (strcmp(argv[i],"-t")==0 && argc>i+1)
      {
        t = atof( argv[i+1] );
        i++;
      }

    //--- Set Min detection scale ---//
    if (strcmp(argv[i],"-minsc")==0 && argc>i+1)
      {
        minsc = atof( argv[i+1] );
        bSetScale = true;
        i++;
      }

    //--- Set Max detection scale ---//
    if (strcmp(argv[i],"-maxsc")==0 && argc>i+1)
      {
        maxsc = atof( argv[i+1] );
        bSetScale = true;
        i++;
      }

    //--- Set GP image scale ---//
    if (strcmp(argv[i],"-imagesc")==0 && argc>i+1)
      {
        imagesc = atof( argv[i+1] );
        i++;
      }

    //--- Set GP world scale ---//
    if (strcmp(argv[i],"-worldsc")==0 && argc>i+1)
      {
        worldsc = atof( argv[i+1] );
        i++;
      }

    //--- Disable MDL ---//
    if (strcmp(argv[i],"-nomdl")==0)
      {
        bUseMDL = false;
      }

    //--- Add Detector ---//
    if (strcmp(argv[i],"-det")==0 && argc>i+1)
      {
        vDetFiles.push_back( string(argv[i+1]) );
        i++;
      }

    //--- Process Single Image ---//
    if (strcmp(argv[i],"-img")==0 && argc>i+1)
      {
        sInputImg = string(argv[i+1]);
        bProcessImg = true;
        i++;
      }

    //--- Process IDL Image Set ---//
    if (strcmp(argv[i],"-idl")==0 && argc>i+1)
      {
        sInputIDL = string(argv[i+1]);
        bProcessIDL = true;
        i++;
      }

    //--- Set Result File ---//
    if (strcmp(argv[i],"-out")==0 && argc>i+1)
      {
        sResultIDL = string(argv[i+1]);
        bUseResIDL = true;
        i++;
      }

    //--- Set Result Directory ---//
    if (strcmp(argv[i],"-odir")==0 && argc>i+1)
      {
        sResultDir = string(argv[i+1]);
        bUseResDir = true;
        i++;
      }

    //--- Verbose ---//
    if (strcmp(argv[i],"-timings")==0 && argc>i+1)
      {
        int val = atoi(argv[i+1]);
        if( val==0 )
          bShowTimings = false;
        else
          bShowTimings = true;
        i++;
      }

    if (strcmp(argv[i],"-q")==0)
      {
        bQuiet       = true;
        bVerbose     = false;
        bVeryVerbose = false;
      }

    if (strcmp(argv[i],"-v")==0)
      {
        bVerbose = true;
      }

    //--- Very Verbose ---//
    if (strcmp(argv[i],"-vv")==0)
      {
        bVerbose     = true;
        bVeryVerbose = true;
      }

  }

  /**********************************/
  /* Check for allowed combinations */
  /**********************************/
  if( bProcessImg && bProcessIDL ) {
    cerr << "ERROR: Please specify either an input image, "
         << "or an input IDL file, but not both!" << endl;
    exit(-1);
  }

  if( !bProcessImg && !bProcessIDL && !bDisplayGUI ) {
    cerr << "ERROR: In order to execute without GUI, you need to specify " 
         << "either an input image, or an input IDL file!" << endl;
    exit(-1);
  }

  if( bProcessIDL && !bUseResIDL ) {
    cerr << "No result file specified. Writing results to '" 
         << sResultIDL << "'..." << endl; 
    bUseResIDL = true;
  }

  if( bProcessImg && !bUseResIDL && bUseResDir ) {
    cerr << "Specified result dir, but no result file. Writing results to '" 
         << sResultIDL << "'..." << endl; 
    bUseResIDL = true;
  }

  if( bUseResDir ) {
    // make sure the result directory exists
    QDir qdResDir( "." );

    if( !qdResDir.exists( sResultDir.c_str() ) ) {
      cerr << "Creating result directory '" << sResultDir << "'..." << endl;
      if( !qdResDir.mkdir( sResultDir.c_str() ) ) {
        cerr << "ERROR: Couldn't create result directory!" << endl;
        exit(-1);
      }
    }
  }

  // Display the GUI (if desired)
  if( bDisplayGUI )
    w.show();
  w.setShowGUI( bDisplayGUI );

  /**************************/
  /* Set the GUI parameters */
  /**************************/
  w.slotSetMinPFig( QString::number(t) );
  w.slotUpdateMinPFig();
  w.chkDoMDL->setChecked( bUseMDL );
  w.slotSetDoMDLOnOff( bUseMDL );
  w.slotSetImgScale( QString::number(imagesc) );
  w.slotUpdateImgScale();
  w.slotSetWorldScale( QString::number(worldsc) );
  w.slotUpdateWorldScale();

  if( bUseResDir ) {
    w.chkWriteResult->setChecked( true );
    w.slotSetWriteResultsOnOff( true );
    w.chkWriteSeg->setChecked( true );
    w.slotSetWriteSegsOnOff( true );
  }

  /* Disable visual output if there is no GUI */
  w.chkShowIntPts->setChecked( false );
  w.slotSetShowIntPtsOnOff( false );

  /* Enable/disable timing output */
  if( bShowTimings ) {
    w.chkShowTimings->setChecked( true );
    w.slotSetShowTimingsOnOff( true );
  } else {
    w.chkShowTimings->setChecked( false );
    w.slotSetShowTimingsOnOff( false );
  }

  /* Set verbosity level */
  if( bQuiet ) {
    w.chkShowTxtSteps->setChecked( false );
    w.slotSetShowTxtStepsOnOff( false ); 
    w.chkShowTxtDet->setChecked( false );
    w.slotSetShowTxtDetailsOnOff( false ); 
    w.chkShowTxtVot->setChecked( false );
    w.slotSetShowTxtVotingOnOff( false ); 
    w.chkShowTxtMDL->setChecked( false );
    w.slotSetShowTxtMDLOnOff( false ); 
  } else if( bVerbose ) {
    w.chkShowTxtSteps->setChecked( true );
    w.slotSetShowTxtStepsOnOff( true ); 
    w.chkShowTxtVot->setChecked( true );
    w.slotSetShowTxtVotingOnOff( true ); 
    w.chkShowTxtMDL->setChecked( true );
    w.slotSetShowTxtMDLOnOff( true ); 
  } else {
    //w.chkShowTxtSteps->setChecked( false );
    //w.slotSetShowTxtStepsOnOff( false ); 
    w.chkShowTxtVot->setChecked( false );
    w.slotSetShowTxtVotingOnOff( false ); 
    w.chkShowTxtMDL->setChecked( false );
    w.slotSetShowTxtMDLOnOff( false ); 
  }
  if( bVeryVerbose && !bQuiet ) {
    w.chkShowTxtDet->setChecked( true );
    w.slotSetShowTxtDetailsOnOff( true ); 
  } else {
    w.chkShowTxtDet->setChecked( false );
    w.slotSetShowTxtDetailsOnOff( false ); 
  }
  /**********************/
  /* Load the detectors */
  /**********************/
  for( unsigned i=0; i<vDetFiles.size(); i++ ) {
    if( !bQuiet )
      cout << endl
           << "Loading detector " << i << ": '" << vDetFiles[i] << "'..."
           << endl;
    w.addDetector( vDetFiles[i], bVerbose && !bQuiet );

    // set the detector parameters
    if( bSetScale ) {
      w.getRecoParams(i).params()->slotSetScaleMin( QString::number(minsc) );
      w.getRecoParams(i).params()->slotUpdateScaleMin();
      w.getRecoParams(i).params()->slotSetScaleMax( QString::number(maxsc) );
      w.getRecoParams(i).params()->slotUpdateScaleMax();
    }
  }

  /********************/
  /* Process the data */ 
  /********************/
  /* do everything automatically */
  if( bProcessImg ) {

    if( !bUseResIDL )
      w.processTestImgStd( QString( sInputImg.c_str()) );
    else {
      // create an ad-hoc 1-entry IDL file
      ImgDescrList idlExperiment;
      idlExperiment.findOrCreate( sInputImg );

      if( !bUseResDir )
        w.performIDLTestSeries( idlExperiment, QString(""),
                                QString( sInputIDL.c_str()), 
                                QString( sResultIDL.c_str()) );
      else
        w.performIDLTestSeries( idlExperiment, QString(""),
                                QString( sResultIDL.c_str()),
                                QString( sResultDir.c_str()),
                                QString( (sResultDir+"/results.txt").c_str()));
    }

    if( !bDisplayGUI )
      exit(0);

  } else 
    if( bProcessIDL ) {
      if( !bUseResDir )
        w.performIDLTestSeries( QString( sInputIDL.c_str()), 
                                QString( sResultIDL.c_str()) );
      else
        w.performIDLTestSeries( QString( sInputIDL.c_str()), 
                                QString( sResultIDL.c_str()),
                                QString( sResultDir.c_str()),
                                QString( (sResultDir+"/results.txt").c_str()));
      
      if( !bDisplayGUI )
        exit(0);
    }
  
  a.setMainWidget( &w );
  return a.exec();
}

