/*********************************************************************/
/*                                                                   */
/* FILE         detector.cc                                          */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@visin.ee.ethz.ch                               */
/*                                                                   */
/* CONTENT      Convenience class for bundeling all information of a */
/*              single viewpoint or category detector (possibly      */
/*              based on multiple cues).                             */
/*                                                                   */
/* BEGIN        Thu Feb 09 2006                                      */
/* LAST CHANGE  Thu Feb 09 2006                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <algorithm>

#include <qapplication.h>
#include <qfiledialog.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qfileinfo.h>
#include <qcolor.h>

#include <resources.hh>
#include <featurevector.hh>
#include <visualhistogram.hh>
#include <qtimgbrowser.hh>

#include "detector.hh"

/*******************/
/*   Definitions   */
/*******************/

/*===================================================================*/
/*                         Class Detector                            */
/*===================================================================*/

Detector::Detector()
  : QObject()
  , m_ismMultiCue( 0 )
  , m_ismMultiCueLeft( 0 )
  , m_calCamera()
  , m_qsDirCodebooks( "~/" )
  , m_qsDirResults( "~/" )
{
  m_guiDetect        = 0;
  m_pmActiveCBs      = 0;
  m_pmActiveMatches  = 0;
  m_pmActiveMatchesL = 0;

  m_nNumCues     = 0;
  m_nFeatureType =-1;
  m_nCombiMethod = CUE_COMBI_AVG;
  
  m_bCalibAvailable = false;
}


Detector::Detector( const Detector &other )
  : QObject()
  , m_ismMultiCue( 0 )
  , m_ismMultiCueLeft( 0 )
  , m_calCamera()
  , m_qsDirCodebooks( "~/" )
  , m_qsDirResults( "~/" )
/* copy constructor */
{
  copyFrom( other );
}


Detector& Detector::operator=( const Detector &other )
  /* assignment operator */
{
  copyFrom( other );
  return *this;
}


void Detector::copyFrom( const Detector& other )
  /* copy operator */
{
  m_nImgWidth          = other.m_nImgWidth;
  m_nImgHeight         = other.m_nImgHeight;
  m_nVotingBinSize     = other.m_nVotingBinSize;

  m_nNumCues           = other.m_nNumCues;
  m_vCBNames           = other.m_vCBNames;
  m_vCues              = other.m_vCues;
  m_vCodebooks         = other.m_vCodebooks;
  m_vISMReco           = other.m_vISMReco;
  m_ismMultiCue        = other.m_ismMultiCue;
  m_ismMultiCueLeft    = other.m_ismMultiCueLeft;

  m_vvPointsInside     = other.m_vvPointsInside;
  m_vMatchResults      = other.m_vMatchResults;
  m_vvPointsInsideLeft = other.m_vvPointsInsideLeft;
  m_vMatchResultsLeft  = other.m_vMatchResultsLeft;

  m_vParMatching       = other.m_vParMatching;
  m_parReco            = other.m_parReco;

  m_nFeatureType       = other.m_nFeatureType;
  m_nCombiMethod       = other.m_nCombiMethod;

  m_qsDirCodebooks     = other.m_qsDirCodebooks;
  m_qsDirResults       = other.m_qsDirResults;

  m_calCamera          = other.m_calCamera;
  m_bCalibAvailable    = other.m_bCalibAvailable;

  m_guiDetect          = other.m_guiDetect;
  m_pmActiveCBs        = other.m_pmActiveCBs;
  m_pmActiveMatches    = other.m_pmActiveMatches;
  m_pmActiveMatchesL   = other.m_pmActiveMatchesL;
}


DetectorGUI* Detector::createGUI( QWidget *parent, const char* name )
{
  if( m_guiDetect != 0 ) {
    cerr << "  Error in Detector::createGUI(): "
         << "Tried to initialize the GUI twice!" << endl;
    return 0;

  } else {
    cout << "  Initializing detector GUI for window '" << name << "'..." 
         << endl;
    m_guiDetect = new DetectorGUI( parent, name );
  }

  /* connect the addCue signal to the own member function */
  connect( m_guiDetect, SIGNAL(sigAddCueClicked()),
           this, SLOT(addCue()) );
  connect( m_guiDetect, SIGNAL(sigAddCueClicked(string,string,bool)),
           this, SLOT(addCue(string,string,bool)) );

  return m_guiDetect;
}


void Detector::setGUI( DetectorGUI* pGUI ) 
{ 
  m_guiDetect = pGUI; 

  /* connect the addCue signal to the own member function */
  connect( m_guiDetect, SIGNAL(sigAddCueClicked()),
           this, SLOT(addCue()) );
  connect( m_guiDetect, SIGNAL(sigAddCueClicked(string,string,bool)),
           this, SLOT(addCue(string,string,bool)) );
}


void Detector::setRecoParams( RecoParams &parReco )
{
  m_parReco = parReco;
}


DetectorGUI* Detector::params()
{
  assert( m_guiDetect != 0 );

  return m_guiDetect;
}


void Detector::setDefaultDirs( const string &sDirCodebooks, 
                               const string &sDirResults )
{
  m_qsDirCodebooks = sDirCodebooks.c_str();
  m_qsDirResults   = sDirResults.c_str();
}


void Detector::setCalibration( Calibration &calCamera ) 
{ 
  m_calCamera = calCamera; 
  m_bCalibAvailable = true;
}


/***********************************************************/
/*                          Slots                          */
/***********************************************************/

/*---------------------------------------------------------*/
/*                Loading/Saving Detectors                 */
/*---------------------------------------------------------*/

void Detector::clearDetector()
{
  /* delete the cue windows */
  for(unsigned j=0; j<m_guiDetect->m_vpCueWindows.size(); j++ ) {
    m_guiDetect->m_vpCueWindows[j]->close();
    m_guiDetect->m_vpCueWindows[j] = 0;
  }
  m_guiDetect->m_vpCueWindows.clear();

  /* update the member variables */
  for(unsigned k=0; k<m_nNumCues; k++ ) {
    m_vvPointsInside[k].clear();
    m_vvPointsInsideLeft[k].clear();
  }
  m_vvPointsInside.clear();
  m_vvPointsInsideLeft.clear();
  
  m_vMatchResults.clear();
  m_vMatchResultsLeft.clear();

  /* delete the cue objects */
  m_nNumCues = 0;
  m_vCues.clear();
  m_vParMatching.clear();
  m_vCodebooks.clear();
  m_vISMReco.clear();
  //m_vCBFiles.clear();
  //m_vOccFiles.clear();

  /* clear the cue table */
  m_guiDetect->clear();
}


void Detector::loadDetector()
{
  /*--------------------------------------*/
  /* Ask for a file name for the codebook */
  /*--------------------------------------*/
  QString qsStartDir( m_qsDirCodebooks );
  QString qsDetName = QFileDialog::getOpenFileName( qsStartDir,
                                    "Detectors (*.det);;All files (*.*)",
                                                    this->params(), 
                                                    "LoadDetector",
                                                    "Select a detector" );
  if ( qsDetName.isEmpty() )
    return;

  loadDetector( qsDetName.latin1() );
  m_qsDirCodebooks = QFileInfo( qsDetName ).dirPath();
}


void Detector::loadDetector( string sDetName, bool bVerbose )
{
  /* clear the old detector */
  clearDetector();

  /*------------------------*/
  /* Load the detector file */
  /*------------------------*/
  m_guiDetect->loadParams( sDetName, bVerbose );
  
  /*--------------------------*/
  /* Load the reco parameters */
  /*--------------------------*/
  m_parReco.params()->loadParams( sDetName );
  qApp->processEvents();

  emit sigDetectorChanged();
  qApp->processEvents();
}


void Detector::saveDetector()
{
  /*--------------------------------------*/
  /* Ask for a file name for the codeb:ook */
  /*--------------------------------------*/
  QString qsStartDir( m_qsDirCodebooks );
  QString qsDetName = QFileDialog::getSaveFileName( qsStartDir,
                                    "Detectors (*.det);;All files (*.*)",
                                                    this->params(), 
                                                    "SaveDetector",
                                                    "Saving the detector" );
  if ( qsDetName.isEmpty() )
    return;

  saveDetector( qsDetName.latin1() );
  m_qsDirCodebooks = QFileInfo( qsDetName ).dirPath();
}


void Detector::saveDetector( string sDetName, bool bVerbose )
{
  /*------------------------*/
  /* Load the detector file */
  /*------------------------*/
  m_guiDetect->saveParams( sDetName );
  
  /*--------------------------*/
  /* Load the reco parameters */
  /*--------------------------*/
  m_parReco.params()->saveParams( sDetName );
}


/*---------------------------------------------------------*/
/*                   Loading/Adding Cues                   */
/*---------------------------------------------------------*/

void Detector::addCue()
{
  /*--------------------------------------*/
  /* Ask for a file name for the codebook */
  /*--------------------------------------*/
  QString qsStartDir( m_qsDirCodebooks );
  QString qsCBName = QFileDialog::getOpenFileName( qsStartDir,
                                      "Vectors (*.fls *.flz);;All files (*.*)",
                                                   this->params(), 
                                                   "LoadCodebook",
                                                   "Select a codebook" );
  if ( qsCBName.isEmpty() )
    return;

  /*-----------------------------------------*/
  /* Ask for a file name for the occurrences */
  /*-----------------------------------------*/
  QString qsOccName = QFileDialog::getOpenFileName( qsCBName,
                                      "Vectors (*.fls *.flz);;All files (*.*)",
                                                    this->params(), 
                                                    "LoadOccurrences",
                                                 "Select an occurrence file");
  if ( qsOccName.isEmpty() )
    return;

  addCue( qsCBName.latin1(), qsOccName.latin1() );
  m_qsDirCodebooks = QFileInfo( qsCBName ).dirPath();
}


void Detector::addCue( string sCBName, string sOccName, bool bVerbose )
{
  assert( isValid() );

  unsigned nNewIdx = m_guiDetect->m_tblCues->numRows();

  assert( m_vCBNames.size()==nNewIdx );
  assert( m_vCues.size()==nNewIdx );
  assert( m_vParMatching.size()==nNewIdx );
  assert( m_vCodebooks.size()==nNewIdx );
  //assert( m_vCBFiles.size()==nNewIdx );
  //assert( m_vOccFiles.size()==nNewIdx );

  /*----------------------------------*/
  /* Prepare a widget for the new cue */
  /*----------------------------------*/
  FeatureGUI  *guiCue      = NULL;
  MatchingGUI *guiMatching = NULL;
  CueWidget   *cwCue = new CueWidget( guiCue, guiMatching, NULL, "cue1" );

  cwCue->setCaption( "Cue " + QString::number(nNewIdx+1) );
  cwCue->hide();

  /* add a new gui window */
  m_guiDetect->m_vpCueWindows.push_back( cwCue );

  /* propagate update events from the cue gui */
  connect( cwCue, SIGNAL(sigCuesChanged()), this, SLOT(propagateCueUpdate()) );
  connect( m_guiDetect, SIGNAL(sigCategNameChanged(const QString&)),
           this, SLOT(propagateDetectorUpdate(const QString&)) );
  connect( m_guiDetect, SIGNAL(sigPoseNameChanged(const QString&)),
           this, SLOT(propagateDetectorUpdate(const QString&)) );
  connect( m_parReco.params(), 
           SIGNAL(sigScoreThreshSingleChanged(const QString&)),
           this, SLOT(propagateDetectorUpdate(const QString&)) );

  /*-----------------------------------------------*/
  /* Check if the Codebook has already been loaded */
  /*-----------------------------------------------*/
  SharedCodebook     cbNew;
  bool bLoadCB = true;
  // look up the Codebook in the 'active' table 
  if( m_pmActiveCBs!=NULL ) {
    CBTable::iterator res = m_pmActiveCBs->find( sCBName );
    if( res!=m_pmActiveCBs->end() ) {
      // existing codebook => look up old entry
      cout << "  The requested codebook is already loaded and will be reused."
           << endl;
      cbNew = res->second;
      bLoadCB = false;

    } else {
      // new codebook => create new entry
      cbNew.create();

      // and add it to the list
      (*m_pmActiveCBs)[sCBName] = cbNew;
    }
  }


 /*-----------------*/
  /* Add new objects */
  /*-----------------*/
  /* add the codebook name */
  m_vCBNames.push_back( sCBName );

  /* add a new FeatureCue object */
  m_vCues.push_back( FeatureCue() );
  m_vCues[nNewIdx].setGUI( guiCue );

  /* add the new matching parameters */
  m_vParMatching.push_back( MatchingParams() );
  m_vParMatching[nNewIdx].setGUI( guiMatching );

  /* add a new Codebook object */
  m_vCodebooks.push_back( cbNew );
  // CAVEAT: This modifies the stored codebook, so that only changes in
  // the last parameter window have an effect. However, shared codebooks
  // shouldn't be used with different parameter settings anyway.
  (*m_vCodebooks[nNewIdx]).setMatchingParams( m_vParMatching[nNewIdx] );

  /* add a new ISM object */
  m_vISMReco.push_back( ISM(0, nNewIdx) );
  m_vISMReco[nNewIdx].setRecoParams( m_parReco );

  /* save the file names */
  //m_vCBFiles.push_back( sCBName );
  //m_vOccFiles.push_back( sOccName );

  /*-------------------*/
  /* Load the codebook */
  /*-------------------*/
  loadCodebook( nNewIdx, sCBName, bLoadCB, bVerbose );

  /*----------------------*/
  /* Load the occurrences */
  /*----------------------*/
  loadOccurrences( nNewIdx, sOccName, bVerbose );

  /*-------------------------*/
  /* Extract cue information */
  /*-------------------------*/
  string sDetector= NAMES_PATCHEXT[guiCue->m_nPatchExtMethod];
  string sFeature = NAMES_FEATURE [guiCue->m_nFeatureType];
  string sNumCl   = QString::number((*m_vCodebooks[nNewIdx]).getNumClusters()).latin1();
  string sNumOcc  = QString::number(m_vISMReco[nNewIdx].getNumOccs()).latin1();

  /*-------------------------------*/
  /* Add an entry to the cue table */
  /*-------------------------------*/
  m_guiDetect->addCueTableEntry( sDetector, sFeature, sNumCl, sNumOcc );
  m_guiDetect->adjustSize();

  /*-----------------------------*/
  /* Update the member variables */
  /*-----------------------------*/
  m_nNumCues = nNewIdx+1;

  PointVector vTmp;
  m_vvPointsInside.push_back( vTmp );
  m_vvPointsInsideLeft.push_back( vTmp );

  //vector<FeatureVector> vTmp2;
  //m_vvFeatures.push_back( vTmp2 );
  //m_vvFeaturesLeft.push_back( vTmp2 );

  SharedMatchingInfo miNew;
  SharedMatchingInfo miNewL;
  miNew.create();
  miNewL.create();
  m_vMatchResults.push_back( miNew );
  m_vMatchResultsLeft.push_back( miNewL );

  for(unsigned k=0; k<m_nNumCues; k++ ) {
    m_vvPointsInside[k].clear();
    //m_vvFeatures[k].clear();

    m_vvPointsInsideLeft[k].clear();
    //m_vvFeaturesLeft[k].clear();
 }

  /*---------------------*/
  /* Show the cue window */
  /*---------------------*/
  if( false ) //bShowCue )
    m_guiDetect->m_vpCueWindows[nNewIdx]->show();

  /*-------------------------*/
  /* Notify the main program */
  /*-------------------------*/
  emit sigCuesChanged();
}


void Detector::loadCodebook( unsigned nIdx, bool bVerbose )
/*******************************************************************/
/* Load a stored codebook and display it in the IconViewer window. */
/*******************************************************************/
{
  /*---------------------*/
  /* Ask for a file name */
  /*---------------------*/
  QString qsStartDir( m_qsDirCodebooks );
  QString qsFileName = QFileDialog::getOpenFileName( qsStartDir,
                   "Vectors (*.fls *.flz);;All files (*.*)",
                                                     this->params() );
  if ( qsFileName.isEmpty() )
    return;

  int pos1 = qsFileName.findRev( ".fls" );
  int pos2 = qsFileName.findRev( ".flz" );
  if ( (pos1 == -1) && (pos2 == -1) ) {
    cout << "No valid file (*.fls or *.flz) selected." << endl;
    return;
  }

  loadCodebook( nIdx, qsFileName.latin1(), true, bVerbose );
  m_qsDirCodebooks = QFileInfo( qsFileName ).dirPath( true );
}


void Detector::loadCodebook( unsigned nIdx, string sFileName, 
                             bool bLoadCB, bool bVerbose )
  /*******************************************************************/
  /* Load a stored codebook and display it in the IconViewer window. */
  /*******************************************************************/
{
  assert( nIdx < m_vCues.size() );
  assert( nIdx < m_vCodebooks.size() );

  if( bVerbose )
    cout << "loadCodebook() called..." << endl;
  
  /* set the cursor to an hourglass */
  params()->setCursor( waitCursor );
  
  /*-----------------------------------------------*/
  /* Prepare the file name and erase the extension */
  /*-----------------------------------------------*/
  string sRawName( sFileName );
  int pos = sRawName.rfind( "." );
  sRawName.erase( pos );

  /*-------------------------*/
  /* Load the parameter file */
  /*-------------------------*/
  //cout << "  Loading parameters ..." << endl;
  string sParamName( sRawName + ".params" );
  //loadParams( sParamName );
  if( m_vParMatching[nIdx].params() )
    m_vParMatching[nIdx].params()->loadParams( sParamName.c_str() );
  if( m_vCues[nIdx].params() )
    m_vCues[nIdx].params()->loadParams( sParamName.c_str() );
  //cout << "  done." << endl;

  /*------------------------*/
  /* Load the main codebook */
  /*------------------------*/
  if( bLoadCB )
    // load the full codebook
    (*m_vCodebooks[nIdx]).loadCodebook( sFileName, m_vCues[nIdx], false, 
                                        false );
//   else
//     // only load the parameter file
//     if( m_vCues[nIdx].params() )
//       m_vCues[nIdx].params()->loadParams( sParamName );

  /*------------------------*/
  /* Normalize the clusters */
  /*------------------------*/
  if( bLoadCB )
    (*m_vCodebooks[nIdx]).normalizeClusters( m_vCues[nIdx].params()->m_nFeatureType );

  /* reset the cursor back to normal */
  params()->setCursor( arrowCursor );
  qApp->processEvents();

  if( bVerbose )
    cout << "loadCodebook() done." << endl;
}


void Detector::loadOccurrences( unsigned nIdx, bool bVerbose )
  /*******************************************************************/
  /* Load a new set of occurrences from disk.                        */
  /*******************************************************************/
{ 
  QString qsFileName = 
    QFileDialog::getOpenFileName( m_qsDirCodebooks, 
                                  "Vectors (*.fls *.flz);;All files (*.*)",
                                  this->params() );
  if ( qsFileName.isEmpty() )
    return;

  loadOccurrences( nIdx, qsFileName.latin1(), bVerbose );
  m_qsDirCodebooks = QFileInfo( qsFileName ).dirPath( true );
}


void Detector::loadOccurrences( unsigned nIdx, string sFileName, 
                                bool bVerbose )
  /*******************************************************************/
  /* Load a new set of occurrences from disk.                        */
  /*******************************************************************/
{ 
  assert( nIdx<m_vISMReco.size() );
  
  /* add a new ISM to the list */
  //m_vISMReco.push_back( ISM(0) );

  m_vISMReco[nIdx].loadOccurrences( sFileName, 
                                    (*m_vCodebooks[nIdx]).getNumClusters(),
                                    bVerbose );
}


/*---------------------------------------------------------*/
/*                    Extracting Patches                   */
/*---------------------------------------------------------*/

/*---------------------------------------------------------*/
/*           Comparing Features with the Codebook          */
/*---------------------------------------------------------*/

void Detector::compareFeatures( vector<vector<FeatureVector> > &vvFeatures,
                                vector<vector<FeatureVector> > &vvFeaturesLeft,
                                bool bVerbose )
  /*******************************************************************/
  /* Compare the extracted image patches with the current codebook   */
  /* and display the matching entries.                               */
  /*******************************************************************/
{
  for(unsigned k=0; k<m_nNumCues; k++ ) {
    compareFeatures( k, vvFeatures[k], bVerbose );
    if( params()->m_bProcessBothDir )
      compareFeaturesLeft( k, vvFeaturesLeft[k], bVerbose );      
  }
}


void Detector::compareFeatures( unsigned nIdx, 
                                vector<FeatureVector> &vFeatures,
                                bool bVerbose )
  /*******************************************************************/
  /* Compare the extracted image patches with the current codebook   */
  /* and display the matching entries.                               */
  /*******************************************************************/
{
  if ( m_vvPointsInside[nIdx].empty() ) {
    cout << "Detector::compareFeatures(): No interest points computed yet." 
         << endl;
    return;
  }
  //if ( m_vvFeatures[nIdx].empty() ) {
  if ( vFeatures.empty() ) {
    cout << "Detector::compareFeatures(): No features computed yet." << endl;
    return;
  }
  if ( (*m_vCodebooks[nIdx]).getNumClusters()<=0 ) {
    cout << "Detector::compareFeatures(): No codebook loaded." << endl;
    return;
  }
  
  /*--------------------------------------------------*/
  /* Compare the features to the codebook             */
  /*--------------------------------------------------*/
  /* Check if the matches have already been computed  */
  /* by a shared codebook and can therefore be reused */
  /*--------------------------------------------------*/
  // Look up the MatchingInfo in the 'active' table 
  if( m_pmActiveMatches!=NULL ) {
    MatchTable::iterator res = m_pmActiveMatches->find( m_vCBNames[nIdx] );
    if( res!=m_pmActiveMatches->end() ) {
      // existing => look up old entry
      if( bVerbose )
        cout << "    Reusing the existing codebook activations..." << endl;
      m_vMatchResults[nIdx] = res->second;

    } else {
      // new => create new entry
      if( bVerbose )
        cout << "    Comparing features with codebook..." << endl;
      float dRejectionThresh = 
        m_vParMatching[nIdx].params()->m_dRejectionThresh;
      (*m_vCodebooks[nIdx]).setMatchingParams( m_vParMatching[nIdx] );
      (*m_vCodebooks[nIdx]).matchToCodebook( vFeatures, 
                                             dRejectionThresh,
                                             m_vCues[nIdx].params()->m_nFeatureType,
                                           (*m_vMatchResults[nIdx]) );

      // and add the matching info to the 'active' table
      (*m_pmActiveMatches)[m_vCBNames[nIdx]] = m_vMatchResults[nIdx];
    }
  }
  
  if( bVerbose )
    cout << "    done." << endl;
}


void Detector::compareFeaturesLeft( unsigned nIdx,
                                    vector<FeatureVector> &vFeatures,
                                    bool bVerbose )
  /*******************************************************************/
  /* Compare the extracted image patches with the current codebook   */
  /* and display the matching entries.                               */
  /*******************************************************************/
{
  if ( m_vvPointsInsideLeft[nIdx].empty() ) {
    cout << "Detector::compareFeaturesLeft(): "
         << "No interest points computed yet." 
         << endl;
    return;
  }
  //if ( m_vvFeaturesLeft[nIdx].empty() ) {
  if ( vFeatures.empty() ) {
    cout << "Detector::compareFeaturesLeft(): No features computed yet." 
         << endl;
    return;
  }
  if ( (*m_vCodebooks[nIdx]).getNumClusters()<=0 ) {
    cout << "Detector::compareFeaturesLeft(): No codebook loaded." << endl;
    return;
  }
  
  /*--------------------------------------------------*/
  /* Compare the features to the codebook             */
  /*--------------------------------------------------*/
  /* Check if the matches have already been computed  */
  /* by a shared codebook and can therefore be reused */
  /*--------------------------------------------------*/
  // Look up the MatchingInfo in the 'active' table 
  // (for the mirrored version)
  if( m_pmActiveMatchesL!=NULL ) {
    MatchTable::iterator res = m_pmActiveMatchesL->find( m_vCBNames[nIdx] );
    if( res!=m_pmActiveMatchesL->end() ) {
      // existing => look up old entry
      if( bVerbose )
        cout << "    Reusing the existing codebook activations..." << endl;
      m_vMatchResultsLeft[nIdx] = res->second;

    } else {
      // new => create new entry
      if( bVerbose )
        cout << "    Comparing features with codebook..." << endl;
      float dRejectionThresh = 
        m_vParMatching[nIdx].params()->m_dRejectionThresh;
      (*m_vCodebooks[nIdx]).setMatchingParams( m_vParMatching[nIdx] );
      (*m_vCodebooks[nIdx]).matchToCodebook( vFeatures, 
                                             dRejectionThresh,
                                             m_vCues[nIdx].params()->m_nFeatureType,
                                             (*m_vMatchResultsLeft[nIdx]) );

      // and add the matching info to the 'active' table
      (*m_pmActiveMatchesL)[m_vCBNames[nIdx]] = m_vMatchResultsLeft[nIdx];
    }
  }

  if( bVerbose )
    cout << "    done." << endl;
}


/*---------------------------------------------------------*/
/*                  Processing a Test Image                */
/*---------------------------------------------------------*/

// void Detector::processTestImg( int nImgWidth, int nImgHeight, 
//                                int nVotingBinSize,
//                                vector<Hypothesis>   &vResultHypos,
//                                vector<Segmentation> &vSegmentations,
//                                bool bDisplayVS, bool bDisplayResults )
// {
//   /* save the settings */
//   m_nImgWidth         = nImgWidth;
//   m_nImgHeight        = nImgHeight;
//   m_nVotingBinSize    = nVotingBinSize;

//   /***********************************************/
//   /*   Apply the voting procedure for each cue   */
//   /***********************************************/
//   vResultHypos.clear();
//   vSegmentations.clear();

//   float dMSMESizeS    = m_parReco.params()->m_dMSMESizeS;
//   int   nObjWidth     = m_parReco.params()->m_nObjWidth;
//   int   nObjHeight    = m_parReco.params()->m_nObjHeight;
//   float dRecoScaleMin = m_parReco.params()->m_dRecoScaleMin;
//   float dRecoScaleMax = m_parReco.params()->m_dRecoScaleMax;

//   float dMaxScaleFactor = nImgWidth / ((float) nObjWidth);
//   float dMaxScaleLevel  = ( (floor(dMaxScaleFactor / dMSMESizeS)+1.0)*
//                             dMSMESizeS );
//   cout << "  Image size: (" << nImgWidth << "," << nImgHeight
//        << ")" << endl;
//   cout << "    => max. scale factor: " << dMaxScaleFactor << endl;
//   cout << "    => max. scale level : " << dMaxScaleLevel << endl;
//   cout << endl;

//   /*--------------------------------*/
//   /*   Initialize the VotingSpace   */
//   /*--------------------------------*/
//   float dScaleMin = dRecoScaleMin - dMSMESizeS/2.0;
//   float dScaleMax = min( dRecoScaleMax, dMaxScaleLevel) + dMSMESizeS/2.0;
//   int   nScaleSteps = (int)floor((dScaleMax - dScaleMin)/
//                                  dMSMESizeS +0.5) + 1;

//   /* prepare a common voting space */
//   m_ismMultiCue.setRecoParams    ( m_parReco );
//   m_ismMultiCue.createVotingSpace( nImgWidth, nImgHeight, nVotingBinSize,
//                                    dScaleMin, dScaleMax, nScaleSteps, 
//                                    true );

//   for(unsigned nIdx=0; nIdx<m_nNumCues; nIdx++ ) {
//     m_vISMReco[nIdx].setRecoParams    ( m_parReco );
//     m_vISMReco[nIdx].createVotingSpace( nImgWidth, nImgHeight, nVotingBinSize,
//                                         dScaleMin, dScaleMax, nScaleSteps, 
//                                         false );
    
//     /*==========================*/
//     /* Apply patch-based voting */
//     /*==========================*/
//     m_vISMReco[nIdx].doPatchVoting( m_ismMultiCue.getVotingSpace(),
//                                     m_vvPointsInside[nIdx], 
//                                     m_vMatchResults[nIdx],
//                                     1.0, 
//                                     m_vParMatching[nIdx].params()->m_dRejectionThresh,
//                                     true );
//   }
//   cout << "================================" << endl;
  
  
//   /*-----------------------------*/
//   /* Retrieve initial hypotheses */
//   /*-----------------------------*/
//   vResultHypos = getPatchHypotheses( bDisplayVS );
  
  
//   /*-----------------------------*/
//   /* Remove duplicate hypotheses */
//   /*-----------------------------*/
//   vResultHypos = removeDuplicateHypos( vResultHypos );
  

//   /*------------------------------------*/
//   /* Convert to fixed bounding box size */
//   /*------------------------------------*/
//   vResultHypos = setHypoBBox( vResultHypos, nObjWidth, nObjHeight );


//   /*==================================*/
//   /* Prepare MDL hypothesis selection */
//   /*==================================*/
//   vector<Hypothesis> vHyposMDL;
//   if( m_parReco.params()->m_bDoMDL ) {
//     vector<Segmentation> vSegmentations;

//     /*------------------------------------------*/
//     /* Resample hypotheses before applying MDL? */
//     /*------------------------------------------*/
//     if( m_bRefineHypothesis ) {
//       m_parReco.params()->m_bDoMDL = false;

//       m_vHyposSingle = vResultHypos;
//       cout << "  Refining the top " << m_vHyposSingle.size() 
//            << " hypotheses..." << endl;
//       refineHypotheses( bDisplayResults );
//       vResultHypos = m_vHyposSingle;

//       m_parReco.params()->m_bDoMDL = true;
//     }
//     vHyposMDL = vResultHypos;

//     /*-------------------------------*/
//     /* Compute the hypothesis images */
//     /*-------------------------------*/
//     vector<Hypothesis> vHyposSegmented;
//     float dMinPFig        = m_parReco.params()->m_dMinPFig;
//     float dMinPFigRefined = m_parReco.params()->m_dMinPFigRefined;
//     float dWeightPFig     = m_parReco.params()->m_dWeightPFig;
//     vector<float> vTmpUC( m_nNumCues, 0.0 );
//     vector<vector<float> > vvUniqueMax( vHyposMDL.size(), vTmpUC );
//     vector<vector<float> > vvUniqueAvg( vHyposMDL.size(), vTmpUC );
//     cout << "  Computing top-down segmentations..." << endl;
//     for( int i=0; i<(int)vHyposMDL.size(); i++ ) {
//       //cout << "    for hypothesis " << i << "..." << endl;
//       /* extract the hypothesis support */
//       FeatureVector fvWindowPos( 3 );
//       fvWindowPos.setValue( 0, vHyposMDL[i].x );
//       fvWindowPos.setValue( 1, vHyposMDL[i].y );
//       fvWindowPos.setValue( 2, vHyposMDL[i].dScale );
      
//       /* get the supporting votes */
//       vector<HoughVote> vSupporting;
//       vSupporting = m_ismMultiCue.getSupportingVotes( fvWindowPos );
//       //cout << "      #votes: " << vSupporting.size() << endl;
      
//       vector<vector<HoughVote> > vvCueSupport;
//       vvCueSupport = splitUpCueVotes( vSupporting );

//       int nHypoWidth  = (int)floor(nObjWidth*vHyposMDL[i].dScale*1.75 + 0.5);
//       int nHypoHeight = (int)floor(nObjHeight*vHyposMDL[i].dScale*1.75+ 0.5);
//       int nSegOffX    = max( 0, vHyposMDL[i].x - nHypoWidth/2 );
//       int nSegOffY    = max( 0, vHyposMDL[i].y - nHypoHeight/2 );
//       int nSegWidth   = min( nHypoWidth, nImgWidth-nSegOffX );
//       int nSegHeight  = min( nHypoHeight, nImgHeight-nSegOffY );

//       vector<Segmentation> vSeg( m_nNumCues );
//       for(unsigned k=0; k<m_nNumCues; k++ ) {
//         vSeg[k] = m_vISMReco[k].drawSegmentationOffset( vvCueSupport[k], 
//                                                         m_vvPointsInside[k], 
//                                                         m_vCues[k],
//                                                         nSegOffX, nSegOffY, 
//                                                         1.0, 
//                                                         nSegWidth, nSegHeight,
//                                                         true );
//       }
  
//       Segmentation segAdd;
//       Segmentation segAvg;
//       Segmentation segMax;
//       Segmentation segArgMax;

//       Segmentation segNew;
//       switch( m_nCombiMethod ) {
//       case CUE_COMBI_SUM:
//         segNew = add( vSeg );
//         break;
        
//       case CUE_COMBI_AVG:
//         segNew = avg( vSeg );
//         break;
        
//       case CUE_COMBI_MAX:
//         segNew = max( vSeg );
//         break;
        
//       default:
//         cerr << "Error in Detector::processTestImg(): "
//              << "Invalid cue combination method (" << m_nCombiMethod << ")!"
//              << endl;
//         return;
//       }

//       float dSumPFig = segNew.getSumPFig();
//       float dFigArea = segNew.getSumSegArea();
//       float dScore   = m_ismMultiCue.getMDLScore( dSumPFig, dFigArea, 
//                                                   vHyposMDL[i].dScale );

//       if( (m_bRefineHypothesis  && (dSumPFig >= dMinPFigRefined)) ||
//           (!m_bRefineHypothesis && (dScore >= dMinPFig)) ) {
//         vSegmentations.push_back( segNew );
//         vHyposSegmented.push_back( vHyposMDL[i] );
//       }

//       vSupporting.clear();
//     }
//     vHyposMDL = vHyposSegmented;
//   }

//  vResultHypos = vHyposMDL;
// }


void Detector::processTestImg( int nImgWidth, int nImgHeight, 
                               int nVotingBinSize,
                               vector<Hypothesis>   &vResultHypos,
                               vector<Segmentation> &vSegmentations,
                               bool bDisplayVS, bool bDisplayResults,
                               bool bVerbose, bool bPrintTiming )
{
  /* save the settings */
  m_nImgWidth         = nImgWidth;
  m_nImgHeight        = nImgHeight;
  m_nVotingBinSize    = nVotingBinSize;

  // TIMING CODE
  time_t  tu1, tu2, tu3, tu4, tu5;
  double  tc1, tc2, tc3, tc4, tc5;
  time(&tu1);
  tc1 = CPUTIME();

  /***********************************************/
  /*   Apply the voting procedure for each cue   */
  /***********************************************/
  vResultHypos.clear();
  vSegmentations.clear();

  float dMSMESizeS    = m_parReco.params()->m_dMSMESizeS;
  int   nObjWidth     = m_parReco.params()->m_nObjWidth;
  int   nObjHeight    = m_parReco.params()->m_nObjHeight;
  float dRecoScaleMin = m_parReco.params()->m_dRecoScaleMin;
  float dRecoScaleMax = m_parReco.params()->m_dRecoScaleMax;

  float dMaxScaleFactor = nImgWidth / ((float) nObjWidth);
  float dMaxScaleLevel  = ( (floor(dMaxScaleFactor / dMSMESizeS)+1.0)*
                            dMSMESizeS );
//   if( bVerbose ) {
//     cout << "  Image size: (" << nImgWidth << "," << nImgHeight
//          << ")" << endl;
//     cout << "    => max. scale factor: " << dMaxScaleFactor << endl;
//     cout << "    => max. scale level : " << dMaxScaleLevel << endl;
//     cout << endl;
//   }

  /*--------------------------------*/
  /*   Initialize the VotingSpace   */
  /*--------------------------------*/
  float dScaleMin = dRecoScaleMin - dMSMESizeS/2.0;
  float dScaleMax = min( dRecoScaleMax, dMaxScaleLevel) + dMSMESizeS/2.0;
  int   nScaleSteps = (int)floor((dScaleMax - dScaleMin)/
                                 dMSMESizeS +0.5) + 1;

  /* prepare a common voting space */
  m_ismMultiCue.setRecoParams    ( m_parReco );
  m_ismMultiCue.createVotingSpace( nImgWidth, nImgHeight, nVotingBinSize,
                                   dScaleMin, dScaleMax, nScaleSteps, 
                                   false );

  if( params()->m_bProcessBothDir ) {
    m_ismMultiCueLeft.setRecoParams    ( m_parReco );
    m_ismMultiCueLeft.createVotingSpace( nImgWidth, nImgHeight, 
                                         nVotingBinSize, 
                                         dScaleMin, dScaleMax, nScaleSteps, 
                                         false );
  }

  for(unsigned nIdx=0; nIdx<m_nNumCues; nIdx++ ) {
    m_vISMReco[nIdx].setRecoParams    ( m_parReco );
    m_vISMReco[nIdx].createVotingSpace( nImgWidth, nImgHeight, nVotingBinSize,
                                        dScaleMin, dScaleMax, nScaleSteps, 
                                        false );
    
    /*==========================*/
    /* Apply patch-based voting */
    /*==========================*/
    m_vISMReco[nIdx].doPatchVoting( m_ismMultiCue.getVotingSpace(),
                                    m_vvPointsInside[nIdx], 
                                    (*m_vMatchResults[nIdx]),
                                    1.0, 
                                    m_vParMatching[nIdx].params()->m_dRejectionThresh,
                                    bVerbose );

    if( params()->m_bProcessBothDir )
      m_vISMReco[nIdx].doPatchVoting( m_ismMultiCueLeft.getVotingSpace(),
                                      m_vvPointsInsideLeft[nIdx], 
                                      (*m_vMatchResultsLeft[nIdx]),
                                      1.0, 
                                      m_vParMatching[nIdx].params()->m_dRejectionThresh,
                                      bVerbose );
  }
  if( bVerbose )
    cout << "================================" << endl;
  
  
  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();

  /*-----------------------------*/
  /* Retrieve initial hypotheses */
  /*-----------------------------*/
  vResultHypos = getPatchHypotheses( bDisplayVS, bVerbose );
  
  vector<Hypothesis> vResultHyposLeft;
  if( params()->m_bProcessBothDir ) {
    vector<OpGrayImage> vImgPatchVotes;
    // !!!HACK: FIX THIS. Should pass correct point vector. !!!
    vResultHyposLeft = 
      m_ismMultiCueLeft.getPatchHypotheses ( m_vvPointsInsideLeft[0], 
                                             nVotingBinSize,
                                             vImgPatchVotes, bVerbose );
  }
  
  // TIMING CODE
  time(&tu3);
  tc3 = CPUTIME();

  /*------------------------------------*/
  /* Convert to fixed bounding box size */
  /*------------------------------------*/
  vResultHypos = setHypoBBox( vResultHypos, nObjWidth, nObjHeight );

  if( params()->m_bProcessBothDir )
    vResultHyposLeft = setHypoBBox( vResultHyposLeft, nObjWidth, nObjHeight );

  /*--------------------------------------------*/
  /* Adjust the scores based on the groundplane */
  /*--------------------------------------------*/
  if( m_bCalibAvailable && params()->m_bApplyGPFilter && 
      params()->m_bUseHeightVar ) {
    vResultHypos = adjustHypoScoresGP( vResultHypos );

  if( params()->m_bProcessBothDir )
    vResultHyposLeft = adjustHypoScoresGP( vResultHyposLeft );    
  }

  /*-----------------------------*/
  /* Remove duplicate hypotheses */
  /*-----------------------------*/
  vResultHypos = removeDuplicateHypos( vResultHypos );
  
  if( params()->m_bProcessBothDir )
    vResultHyposLeft = removeDuplicateHypos( vResultHyposLeft );

  /*---------------------------*/
  /* Apply ground plane filter */
  /*---------------------------*/
  if( m_bCalibAvailable && params()->m_bApplyGPFilter ) {
    float dStdHeight = params()->m_dRealHeight;
    float dHeightVar = params()->m_dHeightVar;

    if( bVerbose )
      cout << "  Applying ground plane filter..." << endl;
    vector<Hypothesis> vHyposGP;
    for(unsigned i=0; i<vResultHypos.size(); i++ ) {
      float dDist = vResultHypos[i].dRealDist;
      float dSize = vResultHypos[i].dRealSize;
      //getRealObjSize( m_calCamera, vResultHypos[i], dDist, dSize );
      float dDev  = dSize - dStdHeight;
      if( (dDist>0.0) && (dDist<=60.0) && (dSize>0.0) &&
          (dSize>=dStdHeight*0.70) && (dSize<=dStdHeight*1.30) ) {
        if( params()->m_bUseHeightVar ) {
          /* weight with Gaussian */
          vResultHypos[i].dAreaFactor /= exp( -(dDev*dDev)/(2.0*dHeightVar) );
        }

        vHyposGP.push_back( vResultHypos[i] );
        //cout << "OK" << flush;
      }
      //cout << endl;
    }
    if( bVerbose )
      cout << "    reducing #hypos from " << vResultHypos.size() << flush;
    vResultHypos = vHyposGP;
    if( bVerbose )
      cout << " to " << vResultHypos.size() << endl;

    if( params()->m_bProcessBothDir ) {
      vector<Hypothesis> vHyposGPLeft;
      for(unsigned i=0; i<vResultHyposLeft.size(); i++ ) {
        float dDist = vResultHyposLeft[i].dRealDist;
        float dSize = vResultHyposLeft[i].dRealSize;
        //getRealObjSize( m_calCamera, vResultHyposLeft[i], dDist, dSize );
        if( (dDist>0.0) && (dDist<=60.0) && (dSize>0.0) &&
            (dSize>=dStdHeight*0.70) && (dSize<=dStdHeight*1.30) ) {
          if( params()->m_bUseHeightVar ) {
            /* weight with Gaussian */
            float dDev = dSize - dStdHeight;
            vResultHyposLeft[i].dAreaFactor /= exp( -(dDev*dDev)/
                                                    (2.0*dHeightVar) );
          }

          vHyposGPLeft.push_back( vResultHyposLeft[i] );
          //cout << "OK" << flush;
        }
        //cout << endl;
      }
      if( bVerbose )
        cout << "    reducing #hypos from " << vResultHyposLeft.size() 
             << flush;
      vResultHyposLeft = vHyposGPLeft;
      if( bVerbose )
        cout << " to " << vResultHyposLeft.size() << endl;
    }
  }

  
  // TIMING CODE
  time(&tu4);
  tc4 = CPUTIME();

  /*==================================*/
  /* Prepare MDL hypothesis selection */
  /*==================================*/
  vector<Hypothesis> vHyposMDL;
  vector<Hypothesis> vHyposMDLLeft;
  if( true ) { //m_parReco.params()->m_bDoMDL ) {
    //vector<Segmentation> vSegmentations;
    vHyposMDL     = vResultHypos;
    vHyposMDLLeft = vResultHyposLeft;

    /*---------------------------------*/
    /* Compute the segmentation images */
    /*---------------------------------*/
    vector<Hypothesis> vHyposSegmented;
    float dMinPFig        = m_parReco.params()->m_dMinPFig;
    //float dWeightPFig     = m_parReco.params()->m_dWeightPFig;

    if( bVerbose )
      cout << "  Computing top-down segmentations..." << flush;
    for( int i=0; i<(int)vHyposMDL.size(); i++ ) {
      if( bVerbose )
        cout << "\r  Computing top-down segmentations... " << i+1 
             << "/" << vHyposMDL.size() << flush;
      
      Segmentation segNew = getHypoSegmentation( vHyposMDL[i],
                                                 nObjWidth, nObjHeight,
                                                 nImgWidth, nImgHeight );
  
      float dSumPFig = segNew.getSumPFig();
      float dFigArea = segNew.getSumSegArea();
      float dScore   = m_ismMultiCue.getMDLScore( dSumPFig, dFigArea, 
                                                  vHyposMDL[i].dScale );
      vHyposMDL[i].dScoreMDL = dScore;

      if( !m_parReco.params()->m_bRejectPFig || (dScore >= dMinPFig) ) {
        vSegmentations.push_back( segNew );
        vHyposSegmented.push_back( vHyposMDL[i] );
      }
    }
    if( bVerbose )
      cout << endl;

    /* process the mirrored hypotheses (if desired) */
    if( params()->m_bProcessBothDir )
      for( int i=0; i<(int)vHyposMDLLeft.size(); i++ ) {
        if( bVerbose )
          cout << "\r  Computing top-down segmentations... " << i+1 
               << "/" << vHyposMDLLeft.size() << flush;

        Segmentation segNew = getHypoSegmentationLeft( vHyposMDLLeft[i],
                                                       nObjWidth, nObjHeight,
                                                       nImgWidth, nImgHeight );
        
        float dSumPFig = segNew.getSumPFig();
        float dFigArea = segNew.getSumSegArea();
        float dScore   = m_ismMultiCueLeft.getMDLScore( dSumPFig, dFigArea, 
                                                        vHyposMDLLeft[i].dScale );
        vHyposMDLLeft[i].dScoreMDL = dScore;
        
        if( !m_parReco.params()->m_bRejectPFig || (dScore >= dMinPFig) ) {
          vSegmentations.push_back( segNew );
          vHyposMDLLeft[i].bPoseFlipped = true;
          vHyposSegmented.push_back( flipHypothesis(vHyposMDLLeft[i],
                                                    nImgWidth)  );
        }
      }
    if( bVerbose )
      cout << endl;
    vHyposMDL = vHyposSegmented;
    vResultHypos = vHyposMDL;
  }

  //vResultHypos = vHyposMDL;

  // TIMING CODE
  time(&tu5);
  tc5 = CPUTIME();

  /*---------------------------*/
  /* Clean up the memory again */
  /*---------------------------*/
  m_ismMultiCue.clearVotingSpace();
  for(unsigned nIdx=0; nIdx<m_nNumCues; nIdx++ )
    m_vISMReco[nIdx].clearVotingSpace();

  /*----------------------*/
  /* Print timing results */
  /*----------------------*/
  if( bPrintTiming ) {
    cout << "  ----------------------" << endl;
    cout << "  Time spent for..." << endl;
    cout << "    Voting           : " << setw(12)
         << tc2-tc1 << "s (system), "
         << tu2-tu1 << "s (user)" << endl;
    cout << "    Maxima Search    : " << setw(12)
         << tc3-tc2 << "s (system), "
         << tu3-tu2 << "s (user)" << endl;
    cout << "    Dupl.+GP Filter  : " << setw(12)
         << tc4-tc3 << "s (system), "
         << tu4-tu3 << "s (user)" << endl;
    cout << "    Top-down Segment.: " << setw(12)
         << tc5-tc4 << "s (system), "
         << tu5-tu4 << "s (user)" << endl;
    cout << "    TOTAL            : " << setw(12)
         << tc5-tc1 << "s (system), "
         << tu5-tu1 << "s (user)" << endl;
    //cout << endl;
    cout << "  ----------------------" << endl;
    //cout << endl;
  }
}


vector<Hypothesis> Detector::getPatchHypotheses( bool bDisplayResults,
                                                 bool bVerbose )
  /*******************************************************************/
  /* Extract the object hypotheses as maxima from the Hough voting   */
  /* space and refine them using MSME (version for scale votes).     */
  /*******************************************************************/
{
  /******************************************/
  /*   Find maxima of patch voting scores   */
  /******************************************/
  vector<OpGrayImage> vImgPatchVotes;
  // !!!HACK: FIX THIS. Should pass correct point vector. !!!
  vector<Hypothesis> vPatchHypos = 
    m_ismMultiCue.getPatchHypotheses ( m_vvPointsInside[0], m_nVotingBinSize,
                                       vImgPatchVotes, bVerbose );

  
  /*******************************/
  /*   Show the voting results   */
  /*******************************/
  if( bDisplayResults ) {
    vector<OpGrayImage> vHistoValues;
    vector<QImage>      vImgs;

    for(unsigned i=0; i<vImgPatchVotes.size(); i++ ) {
      OpGrayImage imgHisto = vImgPatchVotes[i]; 
        
      /* add vote image for scale i */
      vImgs.push_back( imgHisto.getQtImage() );
      vHistoValues.push_back( imgHisto );
    }

    /* add original image */
    //vImgs.push_back( m_grayImg.getQtImage() );
    //vHistoValues.push_back( m_grayImg );
        
    if( true ) {
      /* create a scale histogram */
      VisualHistogram hScales     ( 100, 0.0, 5.0 );
      VisualHistogram hScaleScores( 100, 0.0, 5.0 );
      list<HoughVote> vVotes;
      float dRecoScaleMin = m_parReco.params()->m_dRecoScaleMin;
      float dRecoScaleMax = m_parReco.params()->m_dRecoScaleMax;
      
      /* get all votes in the VotingSpace */
      FeatureVector fvWindowPos( 3 );
      fvWindowPos.setValue( 0, m_nImgWidth/2 );
      fvWindowPos.setValue( 1, m_nImgHeight/2 );
      fvWindowPos.setValue( 2, dRecoScaleMin + (dRecoScaleMax-dRecoScaleMin)/2 );
      FeatureVector fvWindowSize( 3 );
      fvWindowSize.setValue( 0, m_nImgWidth );
      fvWindowSize.setValue( 1, m_nImgHeight );
      fvWindowSize.setValue( 2, dRecoScaleMax-dRecoScaleMin );
      
      vVotes = m_ismMultiCue.getSupportingVotes( fvWindowPos, fvWindowSize );
      
//       for(unsigned i=0; i<vVotes.size(); i++ ) {
      for( list<HoughVote>::iterator it=vVotes.begin(); 
           it!=vVotes.end(); it++ ) {
        hScales.insertValue( it->getCoords().at(2) );
        hScaleScores.incrementValue( it->getCoords().at(2),
                                     it->getValue() );
      }
      QImage imgQScaleHisto; 
      hScales.drawHistogram( imgQScaleHisto );
      vImgs.push_back( imgQScaleHisto );
      vHistoValues.push_back( OpGrayImage( imgQScaleHisto ) );
      hScaleScores.drawHistogram( imgQScaleHisto );
      vImgs.push_back( imgQScaleHisto );
      vHistoValues.push_back( OpGrayImage( imgQScaleHisto ) );
    }

    /* display the images in a browser window */
    QtImgBrowser *qtHoughBrowser = new QtImgBrowser( 0, "Hough Results" ); 
    qtHoughBrowser->setGeometry( 950, 200, 300, 350 );
    qtHoughBrowser->load( vImgs, vHistoValues );
    connect( qtHoughBrowser, SIGNAL(imageClicked(int, int, int)), 
             this, SLOT(showSupportingPatches(int, int, int)) );
    
    qtHoughBrowser->show();
  }  

  return vPatchHypos;
}


vector<Hypothesis> Detector::adjustHypoScoresGP( const vector<Hypothesis> &vHypos )
{
  bool bVerbose = true;
  vector<Hypothesis> vResultHypos = vHypos;

  float dStdHeight = params()->m_dRealHeight;
  float dHeightVar = params()->m_dHeightVar;
  
  for( int i=0; i<(int)vHypos.size(); i++ ) {
    /* get the real object size */
    float dDist, dSize;
    getRealObjSize( m_calCamera, vHypos[i], dDist, dSize );
    
    /* store the hypothesized dist and size */
    vResultHypos[i].dRealDist = dDist;
    vResultHypos[i].dRealSize = dSize;

    /* weight score with a Gaussian */
    float dDev = dSize - dStdHeight;
    vResultHypos[i].dScore *= exp( -(dDev*dDev)/(2.0*dHeightVar) );
  }
  
  /* sort the adjusted hypotheses */
  int nNumHypos = vResultHypos.size();
  for( int j=0; j<nNumHypos-1; j++ )
    for( int i=1; i<nNumHypos-j; i++ )
      if( vResultHypos[i].dScore > vResultHypos[i-1].dScore ) {
        Hypothesis tmp = vResultHypos[i-1];
        vResultHypos[i-1] = vResultHypos[i];
        vResultHypos[i] = tmp;
      }
  
  if( bVerbose ) {
    cout << "=======================================" << endl;
    cout << "Single-Patch Voting Hypotheses after Ground Plane Weighting:" 
         << endl;
    for( int i=0; i<(int)vResultHypos.size(); i++ ) {
      //if( vResultHypos[i].dScore > 0.9*dThresh ) {
        cout << "  " << setw(2) << i+1 << ". ";
        printHypothesis( vResultHypos[i] );
        //}
    }
    cout << "=======================================" << endl;
    cout << endl;
  }

  return vResultHypos;
}


vector<Hypothesis> Detector::removeDuplicateHypos( const vector<Hypothesis> &vHypos,
                                                   bool bVerbose )
{
  float dMSMESizeX    = m_parReco.params()->m_dMSMESizeX;
  float dMSMESizeY    = m_parReco.params()->m_dMSMESizeY;
  float dMSMESizeS    = m_parReco.params()->m_dMSMESizeS;

  /*-----------------------------*/
  /* Remove duplicate hypotheses */
  /*-----------------------------*/
  if( bVerbose )
    cout << "  Reducing the number of hypotheses from " << vHypos.size();
  vector<Hypothesis> vReducedHypos;
  vector<bool>       vDropped( vHypos.size(), false );
  for( int i=0; i<(int)vHypos.size(); i++ )
    if( !vDropped[i] && 
        (vHypos[i].dScore>=m_parReco.params()->m_dScoreThreshSingle) ) {
      Hypothesis hypo = vHypos[i];
      
      /* check all other hypotheses if they are similar */
      for( int j=i+1; j<(int)vHypos.size(); j++ )
        if( !vDropped[j] ) 
          if( (fabs((double)vHypos[j].x - hypo.x) < dMSMESizeX) &&
              (fabs((double)vHypos[j].y - hypo.y) < dMSMESizeY) &&
              (fabs((double)vHypos[j].dScale - hypo.dScale) < dMSMESizeS) ) {
            /* found similar hypothesis => drop the weaker one */
            vDropped[j] = true;
          }
      
      vReducedHypos.push_back( hypo );
    }
  if( bVerbose )
    cout << " to " << vReducedHypos.size() << "." << endl;
  return vReducedHypos;
}
  

vector<Hypothesis> Detector::setHypoBBox( const vector<Hypothesis> &vHypos,
                                         int nObjWidth, int nObjHeight )
{
  vector<Hypothesis> vHyposBox;
  for( int i=0; i<(int)vHypos.size(); i++ )
    if( vHypos[i].dScore > m_parReco.params()->m_dScoreThreshSingle ) {
      Hypothesis hypo = vHypos[i];

      /* set fixed bbox size */
      int width  = (int) floor(nObjWidth*hypo.dScale + 0.5);
      int height = (int) floor(nObjHeight*hypo.dScale + 0.5);
      hypo.nBoxX1 = (int)(hypo.x - width/2);
      hypo.nBoxY1 = (int)(hypo.y - height/2);
      hypo.nBoxWidth = width;
      hypo.nBoxHeight = height;

      /* adjust for different aspects (if selected) */
      if( hypo.dAspect>0.0 ) {
        if( m_parReco.params()->m_nFixObjDim==OBJDIM_WIDTH ) { // w fixed
          height = (int)round((float)width/hypo.dAspect);
        } else {                                               // h fixed
          width  = (int)round((float)height*hypo.dAspect);
        }
        hypo.nBoxX1 = (int)(hypo.x - width/2);
        hypo.nBoxY1 = (int)(hypo.y - height/2);
        hypo.nBoxWidth = width;
        hypo.nBoxHeight = height;
      }

      /* set the area factor */
      hypo.dAreaFactor = m_guiDetect->m_dAreaFactor;

      vHyposBox.push_back( hypo );
    }

  return vHyposBox;
}


Hypothesis Detector::flipHypothesis( const Hypothesis &hypo, int nImgWidth )
{
  Hypothesis hResult(hypo);

  /* set fixed bbox size */
  hResult.x      = nImgWidth - hypo.x - 1;
  hResult.nBoxX1 = nImgWidth - hypo.nBoxX1 - 1;
  
  return hResult;
}


Segmentation Detector::getHypoSegmentation ( Hypothesis hypo,
                                             int nObjWidth, int nObjHeight,
                                             int nImgWidth, int nImgHeight )
{
  /* extract the hypothesis support */
  FeatureVector fvWindowPos( 3 );
  fvWindowPos.setValue( 0, hypo.x );
  fvWindowPos.setValue( 1, hypo.y );
  fvWindowPos.setValue( 2, hypo.dScale );
  
  /* get the supporting votes */
  list<HoughVote> vSupporting;
  vSupporting = m_ismMultiCue.getSupportingVotes( fvWindowPos );
  //cout << "      #votes: " << vSupporting.size() << endl;
  //cout << " (" << vSupporting.size() << " votes)." << flush;

  vector<list<HoughVote> > vlCueSupport;
  vlCueSupport = splitUpCueVotes( vSupporting );
  
  int nHypoWidth  = (int)floor(nObjWidth*hypo.dScale*1.75 + 0.5);
  int nHypoHeight = (int)floor(nObjHeight*hypo.dScale*1.75+ 0.5);
  int nSegOffX    = max( 0, hypo.x - nHypoWidth/2 );
  int nSegOffY    = max( 0, hypo.y - nHypoHeight/2 );
  int nSegWidth   = min( nHypoWidth, nImgWidth-nSegOffX );
  int nSegHeight  = min( nHypoHeight, nImgHeight-nSegOffY );
  
  /* draw a segmentation for each cue */
  vector<Segmentation> vSeg( m_nNumCues, Segmentation() );
  for(unsigned k=0; k<m_nNumCues; k++ ) {
    if( vlCueSupport[k].size()==0 ) {
      cerr << "  WARNING: No features for cue " << k << "!" << endl;
      vSeg[k] = Segmentation( OpGrayImage(nSegWidth,nSegHeight), 
                              OpGrayImage(nSegWidth,nSegHeight),
                              OpGrayImage(nSegWidth,nSegHeight),
                              nSegOffX, nSegOffY, nImgWidth, nImgHeight );

    } else 
      vSeg[k] = m_vISMReco[k].drawSegmentationOffset( vlCueSupport[k], 
                                                      m_vvPointsInside[k], 
                                                      m_vCues[k],
                                                      nSegOffX, nSegOffY, 
                                                      1.0, 
                                                      nSegWidth, nSegHeight,
                                                      true, 1.0, false );

    // check that the segmentations are consistent
//     cout << " check... " << flush;
    assert( vSeg[k].getImgPFig().width()==vSeg[0].getImgPFig().width() );
//     cout << "OK(" << vSeg[k].getImgPFig().width() << "). " << flush;
  }
  //  cout << endl;

  /* combine the different cues */
  Segmentation segNew;
  switch( m_nCombiMethod ) {
  case CUE_COMBI_SUM: segNew = add( vSeg ); break;
  case CUE_COMBI_AVG: segNew = avg( vSeg ); break;
  case CUE_COMBI_MAX: segNew = max( vSeg ); break;
    
  default:
    cerr << "Error in Detector::getHypoSegmentation(): "
         << "Invalid cue combination method (" << m_nCombiMethod << ")!"
         << endl;
    return segNew;
  }

  return segNew;
}


Segmentation Detector::getHypoSegmentationLeft( Hypothesis hypo,
                                               int nObjWidth, int nObjHeight,
                                               int nImgWidth, int nImgHeight )
{
  /* extract the hypothesis support */
  FeatureVector fvWindowPos( 3 );
  fvWindowPos.setValue( 0, hypo.x );
  fvWindowPos.setValue( 1, hypo.y );
  fvWindowPos.setValue( 2, hypo.dScale );
  
  /* get the supporting votes */
  list<HoughVote> vSupporting;
  vSupporting = m_ismMultiCueLeft.getSupportingVotes( fvWindowPos );
  //cout << "      #votes: " << vSupporting.size() << endl;
  
  vector<list<HoughVote> > vlCueSupport;
  vlCueSupport = splitUpCueVotes( vSupporting );
  
  int nHypoWidth  = (int)floor(nObjWidth*hypo.dScale*1.75 + 0.5);
  int nHypoHeight = (int)floor(nObjHeight*hypo.dScale*1.75+ 0.5);
  int nSegOffX    = max( 0, hypo.x - nHypoWidth/2 );
  int nSegOffY    = max( 0, hypo.y - nHypoHeight/2 );
  int nSegWidth   = min( nHypoWidth, nImgWidth-nSegOffX );
  int nSegHeight  = min( nHypoHeight, nImgHeight-nSegOffY );
  
  /* draw a segmentation for each cue */
  vector<Segmentation> vSeg( m_nNumCues, Segmentation() );
  for(unsigned k=0; k<m_nNumCues; k++ ) {
    if( vlCueSupport[k].size()==0 ) {
      cout << "  WARNING: No features for cue " << k << "!" << endl;
      vSeg[k] = Segmentation( OpGrayImage(nSegWidth,nSegHeight), 
                              OpGrayImage(nSegWidth,nSegHeight),
                              OpGrayImage(nSegWidth,nSegHeight),
                              nSegOffX, nSegOffY, nImgWidth, nImgHeight );

    } else 
      vSeg[k] = m_vISMReco[k].drawSegmentationOffset( vlCueSupport[k], 
                                                      m_vvPointsInsideLeft[k], 
                                                      m_vCues[k],
                                                      nSegOffX, nSegOffY, 
                                                      1.0, 
                                                      nSegWidth, nSegHeight,
                                                      true );

    // check that the segmentations are consistent
//     cout << " check... " << flush;
    if( vSeg[k].getImgPFig().width()!=vSeg[0].getImgPFig().width() ) {
      cerr << "ERROR in Detector::getHypoSegmentationLeft(): "
           << "Segmentations have inconsistent size at element(" << k 
           << ")! " << endl;
      for(unsigned i=0; i<vSeg.size(); i++ )
        cerr << "    Seg " << i << ": " << vSeg[i].getImgPFig().width() << "x"
             << vSeg[i].getImgPFig().height() 
             << " from " << vlCueSupport[i].size() << "/"
             << vSupporting.size() << " features." << endl;
      cerr << endl;
      //exit(0);
    }
    assert( vSeg[k].getImgPFig().width()==vSeg[0].getImgPFig().width() );
//     cout << "OK(" << vSeg[k].getImgPFig().width() << "). " << flush;
  }

  /* combine the different cues */
  Segmentation segNew;
  switch( m_nCombiMethod ) {
  case CUE_COMBI_SUM: segNew = add( vSeg ); break;
  case CUE_COMBI_AVG: segNew = avg( vSeg ); break;
  case CUE_COMBI_MAX: segNew = max( vSeg ); break;
    
  default:
    cerr << "Error in Detector::getHypoSegmentationLeft(): "
         << "Invalid cue combination method (" << m_nCombiMethod << ")!"
         << endl;
    return segNew;
  }

  /* flip the final segmentation */
  OpGrayImage imgPFig = segNew.getImgPFig().flipHorizontal();
  OpGrayImage imgPGnd = segNew.getImgPGnd().flipHorizontal();
  OpGrayImage imgSeg  = segNew.getImgSeg().flipHorizontal();

  Segmentation segFlipped( imgPFig, imgPGnd, imgSeg,
                           nImgWidth-(nSegOffX+imgPFig.width()-1)-1, nSegOffY, 
                           nImgWidth, nImgHeight );

  return segFlipped;
}


/*---------------------------------------------------------*/
/*                   Supporting Functions                  */
/*---------------------------------------------------------*/

vector<list<HoughVote> > Detector::splitUpCueVotes( const list<HoughVote> &vVotes)
{
  list<HoughVote> vTmp;
  vector<list<HoughVote> > vlResults( m_nNumCues, vTmp );

//   for(unsigned i=0; i<vVotes.size(); i++ )
  for( list<HoughVote>::const_iterator it=vVotes.begin(); 
       it!=vVotes.end(); it++ )
    vlResults[it->getCueId()].push_back( *it );

  return vlResults;
}


void Detector::showSupportingPatches( int page, int x, int y )
  /*******************************************************************/
  /* Show the patches supporting the hypothesis shown in cell (x,y)  */
  /* of the Hough accumulator histogram visible at page <page> of    */
  /* the result QImgBrowser window.                                  */
  /*******************************************************************/
{
  float dRecoScaleMin = m_parReco.params()->m_dRecoScaleMin;
  float dRecoScaleMax = m_parReco.params()->m_dRecoScaleMax;
  float dMSMESizeS    = m_parReco.params()->m_dMSMESizeS;
  int   nObjWidth     = m_parReco.params()->m_nObjWidth;
  int   nObjHeight    = m_parReco.params()->m_nObjHeight;

  int   nScaleSteps= (int)floor((dRecoScaleMax - dRecoScaleMin)/
                                dMSMESizeS) + 1;
  float dScaleMin  = dRecoScaleMin - dMSMESizeS/2.0;
  float dScaleMax  = dRecoScaleMax + dMSMESizeS/2.0;
  float dScaleRange= dScaleMax - dScaleMin;

  float dCellSizeX = (float)m_nImgWidth / (float)(m_nImgWidth /
                                                  m_nVotingBinSize);
  float dCellSizeY = (float)m_nImgHeight / (float)(m_nImgHeight /
                                                   m_nVotingBinSize);
  float dCellSizeS = ( dScaleRange / (float)nScaleSteps );

  FeatureVector fvWindowPos( 3 );
  fvWindowPos.setValue( 0, (x+0.5)*dCellSizeX );
  fvWindowPos.setValue( 1, (y+0.5)*dCellSizeY );
  fvWindowPos.setValue( 2, dScaleMin+(page+0.5)*dCellSizeS );

  cout << "    Showing hypothesis support for (" << fvWindowPos.at(0)
       << "," << fvWindowPos.at(1) << "," << fvWindowPos.at(2) << ")..." 
       << endl;

  unsigned nIdx = 0;
  showSupportingPatches( nIdx, fvWindowPos, true );

  /* create a single hypothesis for this position */
  Hypothesis hypo;
  hypo.x      = (int)floor(fvWindowPos.at(0) + 0.5);
  hypo.y      = (int)floor(fvWindowPos.at(1) + 0.5);
  hypo.dScale = fvWindowPos.at(2);
  hypo.nBoxX1 = (int)floor(hypo.x - 0.5*(nObjWidth*hypo.dScale) +0.5);
  hypo.nBoxY1 = (int)floor(hypo.y - 0.5*(nObjHeight*hypo.dScale) +0.5);
  hypo.nBoxWidth = (int)floor(nObjWidth*hypo.dScale + 0.5);
  hypo.nBoxHeight= (int)floor(nObjHeight*hypo.dScale + 0.5);
  hypo.dScore    = -1.0;
  hypo.dScoreMDL = -1.0;
  hypo.nCategory = -1;
  hypo.nPose     = -1;
  hypo.nTemplateId = -1;
  hypo.dAreaFactor = 1.0;
  //m_vHyposSingle.clear();
  //m_vHyposSingle.push_back( hypo );

  /* draw a rectangle with the selected object scale */
  emit sigDrawHypothesis ( hypo, QColor( 0, 0, 255 ), false );
  qApp->processEvents();

  cout << "    done." << endl;
}


void Detector::showSupportingPatches( unsigned nIdx, 
                                      FeatureVector fvWindowPos, 
                                      bool bDrawMaps )
  /*******************************************************************/
  /* Show the patches supporting the hypothesis described by the     */
  /* Hough window around fvWindowPos.                                */
  /*******************************************************************/
{
  /********************************/
  /*   Get the supporting votes   */
  /********************************/
  list<HoughVote> vSupporting;

  vSupporting = m_vISMReco[nIdx].getSupportingVotes( fvWindowPos );

  //m_vActiveVotes = vSupporting;
  //cout << "      #Active votes: " << m_vActiveVotes.size() << endl;
  cout << "      #Active votes: " << vSupporting.size() << endl;

  //displayVotePatches( vSupporting, bDrawMaps, m_bDrawConfidence );

  /* draw a segmentation and compute the SumPFig score */
  OpGrayImage imgSeg;
  OpGrayImage imgPFig;
  OpGrayImage imgPGnd;
  m_vISMReco[nIdx].drawSegmentation( vSupporting, m_vvPointsInside[nIdx], 
                                     m_vCues[nIdx], 
                                     imgPFig, imgPGnd, imgSeg, true );
  
  float dFigArea = imgSeg.getSum()/255.0;
  float dSumPFig = imgPFig.getSum();
  cout << "      FigArea=" << dFigArea << ", Sum(pfig)=" << dSumPFig << endl;
}


void Detector::displayOccScaleFootprint()
  /*******************************************************************/
  /* Display a scale footprint of the current occurrence set. That   */
  /* is, show a histogram of the scales on which occurrences were    */
  /* found.                                                          */
  /*******************************************************************/
{
  /* initialize scale histograms */
  VisualHistogram hOccScales  ( 100, 1.0, 9.0 );

  /* initialize image vectors */
  vector<QImage> vQImgs;
  QImage imgQScaleHisto; 

  /* Create a histogram for each cue */
  for(unsigned nIdx=0; nIdx<m_nNumCues; nIdx++ ) {
    /* Get the occurrences */
    VecVecOccurrence vvOccurrences = m_vISMReco[nIdx].getOccurrences();

    /* create a scale footprint for the occurrences */
    for(unsigned i=0; i<vvOccurrences.size(); i++ )
      for(unsigned j=0; j<vvOccurrences[i].size(); j++ )
        hOccScales.insertValue( vvOccurrences[i][j].dScale );
    hOccScales.drawHistogram( imgQScaleHisto );
    vQImgs.push_back( imgQScaleHisto );
  }

  /* display the footprints in a browser window */
  QtImgBrowser *qtBrowser = new QtImgBrowser( 0, "Scale Footprints" ); 
  qtBrowser->setGeometry( 950, 200, 300, 350 );
  qtBrowser->load( vQImgs );
  
  qtBrowser->show();  
}


/*---------------------------------------------------------*/
/*                   Associated Functions                  */
/*---------------------------------------------------------*/

void getRealObjSize( Calibration &calCamera, const Hypothesis& hypo,
                     float &dDist, float &dSize )
{
  getRealObjSize( calCamera, hypo.x, hypo.y + hypo.nBoxHeight/2, 
                  hypo.y - hypo.nBoxHeight/2, dDist, dSize );
}


void getRealObjSize( Calibration &calCamera, 
                     float x, float y1, float y2,
                     float &dDist, float &dSize )
{
  /*-----------------------------------------------*/
  /* Get the object base point on the ground plane */
  /*-----------------------------------------------*/
  FeatureVector fvPosI1(3);
  FeatureVector fvPosW1(3);
  fvPosI1.at(0) = x;  //hypo.x;
  fvPosI1.at(1) = y1; //hypo.y + hypo.nBoxHeight/2;
  fvPosI1.at(2) = 1.0;
  if( !calCamera.posOnGroundPlane( fvPosI1, fvPosW1 ) ) {
    dDist = -1.0;
    dSize = -1.0;
    return;
  }

  /*------------------------------------*/
  /* Compute its distance to the camera */
  /*------------------------------------*/
  dDist = calCamera.distToCamera( fvPosW1 )*calCamera.getWorldScale();
  
  /*-------------------------------------*/
  /* Get the ray to the object top point */
  /*-------------------------------------*/
  FeatureVector fvPosI2(3);
  FeatureVector fvPosW2(3);
  fvPosI2.at(0) = x;  //hypo.x;
  fvPosI2.at(1) = y2; //hypo.y - hypo.nBoxHeight/2;
  fvPosI2.at(2) = 1.0;

  /*-------------------------------------------------------------*/
  /* Intersect it with the vertical plane through the base point */
  /*-------------------------------------------------------------*/
  FeatureVector fvVPN = calCamera.getVPN();
  if( calCamera.intersectWithPlane(fvPosI2, fvVPN, fvPosW1, fvPosW2) )
    dSize = calCamera.distFromGround( fvPosW2 )*calCamera.getWorldScale();
  else
    dSize = -1.0;
}


