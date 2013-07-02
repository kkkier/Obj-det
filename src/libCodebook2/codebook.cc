/*********************************************************************/
/*                                                                   */
/* FILE         codebook.cc                                          */
/* AUTHORS      Bastian Leibe, Edgar Seemann                         */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Class for loading/saving/creating/matching codebooks.*/
/*                                                                   */
/* BEGIN        Tue Mar 15 2005                                      */
/* LAST CHANGE  Tue Mar 15 2005                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <math.h>

#include <qfiledialog.h>

#include <resources.hh>
#include <pca.hh>
#include <clkmeans.hh>
#include <clpostagglo.hh>
#include <clrnncagglo.hh>
#include <clfastrnncagglo.hh>
#include <clfastrnncagglo2.hh>
#include <chamfermatching.h>

#include "clusterparams.hh"
#include "matchingparams.hh"
#include "codebook.hh"

/*******************/
/*   Definitions   */
/*******************/

/*===================================================================*/
/*                          Class Codebook                           */
/*===================================================================*/

/*---------------------------------------------------------*/
/*                       Constructors                      */
/*---------------------------------------------------------*/

Codebook::Codebook()
{
  //_refcount = 1;
  m_bClustersValid   = false;
  m_bPrototypesValid = false;
  m_bKeepPatches     = true;
}


Codebook::~Codebook()
{
//   if( _refcount > 1 ) {
//     cerr << "WARNING: "
//          << "Trying to destroy Codebook object with pending references!" 
//          << endl;
//     throw ObjectInUseException();
//   }
}

/*---------------------------------------------------------*/
/*                   Parameter Handling                    */
/*---------------------------------------------------------*/

void Codebook::setClusterParams( ClusterParams parCluster )
{
  m_parCluster = parCluster;
}


void Codebook::setMatchingParams( MatchingParams parMatching )
{
  m_parMatching = parMatching;
}


/*---------------------------------------------------------*/
/*                     Adding Features                     */
/*---------------------------------------------------------*/

void Codebook::clear()
{
  /* clear the features */
  m_vFeatures.clear();
  m_vImagePatches.clear();

  /* invalidate the clustering results */
  m_bClustersValid = false;
  m_bPrototypesValid = false;
  m_bKeepPatches = true;
  m_vClusters.clear();
  m_vClusterPatches.clear();
  m_vClusterAssignment.clear();
  m_vPrototypes.clear();
}


void Codebook::addFeatures( const vector<FeatureVector> &vFeatures )
{
  /* add the new features */
  m_vFeatures.insert( m_vFeatures.end(), 
                      vFeatures.begin(), vFeatures.end() );

  /* don't add any patches */
  m_bKeepPatches = false;
  m_vImagePatches.clear();

  /* invalidate the clustering results */
  //m_bClustersValid = false;
  //m_bPrototypesValid = false;
  //m_vClusters.clear();
  //m_vClusterPatches.clear();
  m_vClusterAssignment.clear();
  //m_vPrototypes.clear();
}


void Codebook::addFeatures( const vector<FeatureVector> &vFeatures,
                            const vector<OpGrayImage>   &vImagePatches )
{
  /* add the new features */
  m_vFeatures.insert( m_vFeatures.end(), 
                      vFeatures.begin(), vFeatures.end() );

  /* only add the patches if they shall be kept */
  if( m_bKeepPatches )
    m_vImagePatches.insert( m_vImagePatches.end(), 
                            vImagePatches.begin(), vImagePatches.end() );

  /* invalidate the clustering results */
  //m_bClustersValid = false;
  //m_bPrototypesValid = false;
  //m_vClusters.clear();
  //m_vClusterPatches.clear();
  m_vClusterAssignment.clear();
  //m_vPrototypes.clear();
}

void Codebook::addFeatures( const vector<FeatureVector> &vFeatures,
                            const vector<OpGrayImage>   &vImagePatches,
			    const vector<int>		&vFeatureClass )
{
  /* add the new features */
  m_vFeatures.insert( m_vFeatures.end(), 
                      vFeatures.begin(), vFeatures.end() );

  /* only add the patches if they shall be kept */
  if( m_bKeepPatches )
    m_vImagePatches.insert( m_vImagePatches.end(), 
                            vImagePatches.begin(), vImagePatches.end() );
  m_vFeatureClass.insert(m_vFeatureClass.end(),
			 vFeatureClass.begin(), vFeatureClass.end());
  /* invalidate the clustering results */
  //m_bClustersValid = false;
  //m_bPrototypesValid = false;
  //m_vClusters.clear();
  //m_vClusterPatches.clear();
  m_vClusterAssignment.clear();
  //m_vPrototypes.clear();
}
void Codebook::loadFeatureClass(const vector<int>& vFeatureClass)
{
	m_vFeatureClass.insert(m_vFeatureClass.end(), 
				vFeatureClass.begin(), vFeatureClass.end());
}
void Codebook::normalizeFeatures( int nFeatureType )
{
  normalizeFeatures( m_vFeatures, nFeatureType );
}


void Codebook::normalizeClusters( int nFeatureType )
{
  normalizeFeatures( m_vClusters, nFeatureType );
}


void Codebook::normalizeFeatures( vector<FeatureVector> &vFeatures,
                                  int nFeatureType )
{
  switch( nFeatureType ) {
  case FEATURE_PATCH:
  case FEATURE_PATCHMIKO:
    for( int i=0; i<(int)vFeatures.size(); i++ )
      vFeatures[i].normalizeZeroMeanUnitStdDev2();
    break;

  case FEATURE_PATCHMIKO2:
  case FEATURE_STEERABLE:
  case FEATURE_SIFT:
  case FEATURE_GLOH:
  case FEATURE_MOMENTS:
  case FEATURE_DIFFINV:
  case FEATURE_COMPLEX:
  case FEATURE_SHAPECONTEXT:
  case FEATURE_SPINIMGS:
  case FEATURE_GRADIENTPCA:
  case FEATURE_SURF64:
  case FEATURE_SURF128:
    break;

  default:
    cerr << "  Error in Codebook::normalizeFeatures(): "
         << "Unknown feature type (" << nFeatureType << ")!" << endl;
    return;
  }
}


/*---------------------------------------------------------*/
/*                     Content Access                      */
/*---------------------------------------------------------*/

int Codebook::getNumClusters() const    
{ 
  return (int)m_vClusters.size(); 
}


vector<FeatureVector> Codebook::getClusters() const    
{ 
  if( !m_bClustersValid )
    cerr << "  WARNING in Codebook::getClusters(): "
         << "Clusters not valid yet!" << endl;

  return m_vClusters; 
}


vector<int> Codebook::getPrototypes() const  
{ 
  if( !m_bClustersValid )
    cerr << "  WARNING in Codebook::getPrototypes(): "
         << "Clusters not valid yet!" << endl;

  return m_vPrototypes; 
}


vector<int> Codebook::getClusterAssignment() const  
{ 
  if( !m_bClustersValid )
    cerr << "  WARNING in Codebook::getClusterAssignment(): "
         << "Clusters not valid yet!" << endl;

  return m_vClusterAssignment; 
}


vector<OpGrayImage> Codebook::getClusterPatches() const 
{
  if( !m_bClustersValid )
    cerr << "  WARNING in Codebook::getClusterPatches(): "
         << "Clusters not valid yet!" << endl;

  if( !m_bKeepPatches )
    cerr << "  WARNING in Codebook::getClusterPatches(): "
         << "No patches available!" << endl;

  return m_vClusterPatches;
}


OpGrayImage Codebook::getClusterPatch( int idx ) const 
{
  if( !m_bClustersValid )
    cerr << "  WARNING in Codebook::getClusterPatch(): "
         << "Clusters not valid yet!" << endl;

  if( !m_bKeepPatches ) {
    cerr << "  WARNING in Codebook::getClusterPatch(): "
         << "No patches available!" << endl;
    return OpGrayImage();
  }

  assert( (idx>=0) && (idx<(int)m_vClusterPatches.size()) );

  return m_vClusterPatches[idx];
}


/*---------------------------------------------------------*/
/*                        File I/O                         */
/*---------------------------------------------------------*/

void Codebook::loadCodebook( string sFileName, FeatureCue &fcCue, 
                             bool bKeepPatches, bool bVerbose )
/*******************************************************************/
/* Load a stored codebook (including all supplementary files) and  */
/* directly load also the feature extraction parameters.           */
/*******************************************************************/
{
  assert( fcCue.params() != 0 );

  loadCodebook( sFileName, bKeepPatches, bVerbose );

  /*-------------------------*/
  /* Load the parameter file */
  /*-------------------------*/
  if( bVerbose )
    cout << "  Loading Feature extraction parameters ..." << endl;
  string sRawName( sFileName );
  int pos = sRawName.rfind( "." );
  sRawName.erase( pos );
  string sParamName( sRawName + ".params" );
  fcCue.params()->loadParams( sParamName );
  if( bVerbose )
    cout << "  done." << endl;
}


void Codebook::loadCodebook( string sFileName, bool bKeepPatches, 
                             bool bVerbose )
/*******************************************************************/
/* Load a stored codebook (including all supplementary files).     */
/*******************************************************************/
{
  if( bVerbose )
    cout << "LoadCodebook() called..." << endl;
  clear();
  m_bKeepPatches = bKeepPatches;

  /*-----------------------------------------------*/
  /* Prepare the file name and erase the extension */
  /*-----------------------------------------------*/
  string sRawName( sFileName );
  int pos = sRawName.rfind( "." );
  sRawName.erase( pos );

  /*----------------------------*/
  /* Load the codebook features */
  /*----------------------------*/
  if( !loadFeatures( sFileName, m_vFeatures, bVerbose ) )
    return;
  
  /*--------------------------------------*/
  /* Load the corresponding image patches */
  /*--------------------------------------*/
  if( m_bKeepPatches ) {
    string sPatchFile( sRawName + ".patches.flz" );
    loadPatches( sPatchFile, m_vImagePatches, bVerbose );
  }

  /*------------------------------*/
  /* Load the cluster assignments */
  /*------------------------------*/
  string sAssignName( sRawName + ".ass" );  
  loadClusterAssignments( sAssignName, m_vClusterAssignment, bVerbose );
  
  /*------------------------*/
  /* Load the cluster trace */
  /*------------------------*/
  string sTraceFile( sRawName + ".trc" );
  loadClusterTrace( sTraceFile, m_vClusterTrace, bVerbose );

  /*-----------------------------*/
  /* Compute the cluster centers */
  /*-----------------------------*/
  computeClusterCenters();

  /*---------------------*/
  /* Load the prototypes */
  /*---------------------*/
  string sChamferFile   ( sRawName + "chamfer" );
  string sChamferOriFile( sRawName + "chamferOrientation" );
  bool bChamfer    = QFile::exists( sChamferFile );
  bool bChamferOri = QFile::exists( sChamferOriFile );
  if ( !bChamfer && !bChamferOri ) {
    if( bVerbose )
      cerr << "    No corresponding prototype file found "
           << "(*.chamfer, *.chamferOrientation)." << endl;

  } else
    if( bChamfer ) {
      loadPrototypes( sChamferFile, bVerbose );
      //m_nFeatureType = FEATURE_LINE;

    } else if( bChamferOri ) {
      loadPrototypes( sChamferOriFile, bVerbose );
      //m_nFeatureType = FEATURE_PLANES;
    }

  /*-------------------------*/
  /* Load the parameter file */
  /*-------------------------*/
  if( bVerbose )
    cout << "  Loading parameters ..." << endl;
  string sParamName( sRawName + ".params" );
  if( m_parCluster.isValid() )
    m_parCluster.params()->loadParams( sParamName );
  if( m_parMatching.isValid() )
    m_parMatching.params()->loadParams( sParamName );
  if( bVerbose )
    cout << "  done." << endl;

  if( bVerbose )
    cout << "Loading Codebook done." << endl;
}


void Codebook::appendCodebook( string sFileName, FeatureCue &fcCue,
                               bool bKeepPatches, bool bVerbose )
/*******************************************************************/
/* Load a stored codebook (including all supplementary files), ap- */
/* pend it to the current codebook, and directly load also the     */
/* feature extraction parameters.                                  */
/*******************************************************************/
{
  appendCodebook( sFileName, bKeepPatches, bVerbose );

  /*-------------------------*/
  /* Load the parameter file */
  /*-------------------------*/
  if( bVerbose )
    cout << "  Loading Feature extraction parameters ..." << endl;
  string sRawName( sFileName );
  int pos = sRawName.rfind( "." );
  sRawName.erase( pos );
  string sParamName( sRawName + ".params" );
  fcCue.params()->loadParams( sParamName );
  if( bVerbose )
    cout << "  done." << endl;
}


void Codebook::appendCodebook( string sFileName, bool bKeepPatches,
                               bool bVerbose )
/*******************************************************************/
/* Load a stored codebook and append it to the current codebook.   */
/*******************************************************************/
{
  if( bVerbose )
    cout << "  appendCodebook() called..." << endl;
  m_bKeepPatches = m_bKeepPatches && bKeepPatches;
  if( !m_bKeepPatches )
    m_vImagePatches.clear();
  
  /*-----------------------------------------------*/
  /* Prepare the file name and erase the extension */
  /*-----------------------------------------------*/
  string sRawName( sFileName );
  int pos = sRawName.rfind( "." );
  sRawName.erase( pos );
  
  /*-----------------------------------------*/
  /* Save information about the old codebook */
  /*-----------------------------------------*/
  //int nOldSize = (int)m_vFeatures.size();

  /*----------------------------*/
  /* Load the codebook features */
  /*----------------------------*/
  vector<FeatureVector> vFeatures;
  if( !loadFeatures( sFileName, vFeatures, bVerbose ) )
    return;

  m_vFeatures.insert( m_vFeatures.end(), vFeatures.begin(), vFeatures.end() );
  
  /*--------------------------------------*/
  /* Load the corresponding image patches */
  /*--------------------------------------*/
  if( m_bKeepPatches ) {
    string sPatchFile( sRawName + ".patches.flz" );
    vector<OpGrayImage> vImagePatches;
    loadPatches( sPatchFile, vImagePatches, bVerbose );
    m_vImagePatches.insert( m_vImagePatches.end(), 
                            vImagePatches.begin(), vImagePatches.end() );
  }

  /*------------------------------*/
  /* Load the cluster assignments */
  /*------------------------------*/
  string sAssignName( sRawName + ".ass" );  
  vector<int> vClusterAssignment;
  loadClusterAssignments( sAssignName, vClusterAssignment, bVerbose );
  m_vClusterAssignment.insert( m_vClusterAssignment.end(), 
                               vClusterAssignment.begin(), 
                               vClusterAssignment.end() );
  
  /*------------------------*/
  /* Load the cluster trace */
  /*------------------------*/
  string sTraceFile( sRawName + ".trc" );
  vector<ClStep> vClusterTrace;
  loadClusterTrace( sTraceFile, vClusterTrace, bVerbose );
  m_vClusterTrace.insert( m_vClusterTrace.end(), 
                          vClusterTrace.begin(), vClusterTrace.end() );

  /*-----------------------------*/
  /* Compute the cluster centers */
  /*-----------------------------*/
  computeClusterCenters();

  /*-------------------------*/
  /* Load the parameter file */
  /*-------------------------*/
  if( bVerbose )
    cout << "Loading parameters ..." << endl;
  string sParamName( sRawName + ".params" );
  if( m_parCluster.isValid() )
    m_parCluster.params()->loadParams( sParamName );
  if( m_parMatching.isValid() )
    m_parMatching.params()->loadParams( sParamName );
  if( bVerbose )
    cout << "done." << endl;

  if( bVerbose )
    cout << "Loading Feature Vectors done." << endl;
}


void Codebook::saveCodebook( string sFileName, FeatureCue &fcCue,
                             bool bKeepPatches, bool bVerbose )
/*******************************************************************/
/* Save the current codebook (including all supplementary files)   */
/* and directly save also the feature extraction parameters.       */
/*******************************************************************/
{
  saveCodebook( sFileName, bKeepPatches, bVerbose );

  /*-------------------------*/
  /* Save the parameter file */
  /*-------------------------*/
  if( bVerbose )
    cout << "  Saving Feature extraction parameters ..." << endl;
  string sRawName( sFileName );
  int pos = sRawName.rfind( "." );
  sRawName.erase( pos );
  string sParamName( sRawName + ".params" );
  fcCue.params()->saveParams( sParamName );
  if( bVerbose )
    cout << "  done." << endl;
}


void Codebook::saveCodebook( string sFileName, bool bKeepPatches, 
                             bool bVerbose )
/*******************************************************************/
/* Save the current codebook to disk (producing 2-3 files: a list. */
/* of feature vectors (*.flz), an ASCII file containing the clus-  */
/* ter assignments (*.ass), and an optional list of patch segmen-  */
/* tation masks (*.seg.flz, will only be saved if segmentations    */
/* were available for the training images!).                       */
/*******************************************************************/
{ 
  if( bVerbose )
    cout << "SaveCodebook() called..." << endl;

  /*-----------------------------------------------*/
  /* Prepare the file name and erase the extension */
  /*-----------------------------------------------*/
  string sExtension( ".flz" );
  int pos = sFileName.rfind( sExtension );
  if( pos == (int)string::npos )
    sFileName = sFileName + sExtension;

  string sRawName( sFileName );
  pos = sRawName.rfind( "." );
  if( pos != (int)string::npos )
    sRawName.erase( pos );

  /*----------------------------*/
  /* Save the codebook features */
  /*----------------------------*/
  if ( !saveFeatures( sFileName, bVerbose ) )
    cerr << "  WARNING: Couldn't save features!" << endl;
  
  /*--------------------------------------*/
  /* Save the corresponding image patches */
  /*--------------------------------------*/
  if( bKeepPatches ) {
    string sPatchFile( sRawName + ".patches.flz" );
    savePatches( sPatchFile, bVerbose );
  }

  /*------------------------------*/
  /* Save the cluster assignments */
  /*------------------------------*/
  string sAssignName( sRawName + ".ass" );
  saveClusterAssignments( sAssignName, bVerbose );
  
  /*------------------------*/
  /* Save the cluster trace */
  /*------------------------*/
  string sTraceFile( sRawName + ".trc" );
  if( (int)m_vClusterTrace.size() != 0 )
    saveClusterTrace( sTraceFile, bVerbose );

  /*-----------------------------*/
  /* Save the cluster prototypes */
  /*-----------------------------*/
  string sChamferFile( sRawName + ".chamfer" );
  if( m_bPrototypesValid )
    savePrototypes( sChamferFile, bVerbose );
  
  /*-------------------------*/
  /* Save the parameter file */
  /*-------------------------*/
  if( bVerbose )
    cout << "  Saving parameters ..." << endl;
  string sParamName( sRawName + ".params" );
  if( m_parCluster.isValid() )
    m_parCluster.params()->saveParams( sParamName );
  if( m_parMatching.isValid() )
    m_parMatching.params()->saveParams( sParamName );
  if( bVerbose )
    cout << "  done." << endl;

  if( bVerbose )
    cout << "Saving Codebook done." << endl;
}
void Codebook::computeClusterCenters_HF()
{
	// Get the cluster in random forest format
	vector<vector<int> > vvAssignment = m_rfRandomForest.getClusterAssignment();
	// Fake a m_vClusterAssignment
	m_vClusterAssignment.clear();
	m_vClusterAssignment.assign(m_vImagePatches.size(),0);
	
	// Calculate average Patch in every leaf node and store in m_vClusterPatches
	OpGrayImage imgTmp( m_vImagePatches.front().width(),
                            m_vImagePatches.front().height() );
   	vector<OpGrayImage> vTmpImg( vvAssignment.size(), imgTmp );
   	m_vClusterPatches = vTmpImg;
	
	int currentIndex = 1;
	for(vector<vector<int> >::iterator itr_1 = vvAssignment.begin(); itr_1!=vvAssignment.end();itr_1++){
		
		for(vector<int>::iterator itr_2 = itr_1->begin(); itr_2!=itr_1->end();itr_2++){
			m_vClusterAssignment[*itr_2] = currentIndex;
			m_vClusterPatches[currentIndex-1].add(m_vImagePatches[*itr_2]);
		}
		m_vClusterPatches[currentIndex-1].div(itr_1->size());
		currentIndex++;
	}
	// Fake a m_vFeatures, empty one
	FeatureVector fvTmp( m_vFeatures.front().numDims() );
  	vector<FeatureVector> vTmp( vvAssignment.size(), fvTmp );
 	m_vClusters = vTmp;

	m_bClustersValid = true;
}

void Codebook::computeClusterCenters()
/*******************************************************************/
/* Compute the cluster centers for each cluster.                   */
/*******************************************************************/
{
  /*-----------------------------*/
  /* Compute the cluster centers */
  /*-----------------------------*/
  /* get the number of clusters */
  int nNumClusters = 0;
  for( int i=0; i<(int)m_vClusterAssignment.size(); i++ ) {
    if ( m_vClusterAssignment[i] > nNumClusters ) 
      nNumClusters = m_vClusterAssignment[i];
  }
  nNumClusters++;

  /* resize and clear the clusters */
  if( m_bKeepPatches ) {
    OpGrayImage imgTmp( m_vImagePatches.front().width(),
                        m_vImagePatches.front().height() );
    //m_vClusterPatches.resize( nNumClusters, imgTmp );
    vector<OpGrayImage> vTmpImg( nNumClusters, imgTmp );
    m_vClusterPatches = vTmpImg;
  }

  FeatureVector fvTmp( m_vFeatures.front().numDims() );
  //m_vClusters.resize( nNumClusters, fvTmp );
  vector<FeatureVector> vTmp( nNumClusters, fvTmp );
  m_vClusters = vTmp;

  vector<int> vNumMembers( nNumClusters, 0 );
  for( int i=0; i<(int)m_vClusterAssignment.size(); i++ ) {
    int idx = m_vClusterAssignment[i];

    m_vClusters[idx] += m_vFeatures[i];
    if( m_bKeepPatches )
      m_vClusterPatches[idx] = m_vClusterPatches[idx].add(m_vImagePatches[i]);
    vNumMembers[idx]++;
  }

  for( int i=0; i<(int)vNumMembers.size(); i++ ) {
    m_vClusters[i]      /= (float)vNumMembers[i];
    if( m_bKeepPatches )
      m_vClusterPatches[i] = m_vClusterPatches[i].div((float)vNumMembers[i]);
  }

  m_bClustersValid = true;
}


void Codebook::computeClusterPrototypes( vector<float> &mSimMatrix )
/*******************************************************************/
/* Find the most typical patch in each cluster (used e.g. for      */
/* Chamfer similarities, where the cluster mean makes no sense).   */
/*******************************************************************/
{
  /*--------------------*/
  /* Compute prototypes */
  /*--------------------*/
  int nNumFeatures = m_vFeatures.size();
  vector<float> vDistToCluster( nNumFeatures, 0.0 );
  vector<int>   vPrototypes   ( (int)m_vClusters.size() );
  for( int i=0; i<nNumFeatures; i++ ) {
    for( int j=0; j<nNumFeatures; j++ ) {
      if( i > j ) {
        int idx = i*nNumFeatures + j;
        if( m_vClusterAssignment[i]==m_vClusterAssignment[j] ) {
          // same cluster?
          float val= 1.0 - mSimMatrix[idx];
          vDistToCluster[i]+=val;
          vDistToCluster[j]+=val;
        }
        // init best to something
        vPrototypes[m_vClusterAssignment[j]]=j;
      }
    }
  }
  m_vPrototypes = vPrototypes;

  m_bPrototypesValid = true;
}


/*---------------------------------------------------------*/
/*                      Visualization                      */
/*---------------------------------------------------------*/

void Codebook::drawClusters( QtClusterView *qClassView )
/*******************************************************************/
/* Draw the clusters from the current codebook and display them    */
/* in the IconViewer window.                                       */
/*******************************************************************/
{  
  /* compute the cluster centers if necessary */
  if( !m_bClustersValid )
    computeClusterCenters();

  /*--------------------------------------*/
  /* Count the number of 1-patch clusters */
  /*--------------------------------------*/
  int nNumClusters = (int)m_vClusters.size();
  vector<int> vNumMembers( nNumClusters, 0 );
  for( int i=0; i<(int)m_vClusterAssignment.size(); i++ ) {
    int idx = m_vClusterAssignment[i];
    vNumMembers[idx]++;
  }

  int nCount1pc=0;
  for( int i=0; i<(int)vNumMembers.size(); i++ ) {
    if( vNumMembers[i] == 1 )
      nCount1pc++;
  }
  
  cout << "    Number of feature points: " << m_vClusterAssignment.size()
       << endl;
  cout << "    Number of clusters      : " << m_vClusters.size() 
       << endl;
  cout << "    (containing just 1 patch: " << nCount1pc << ")" << endl;
  
  /* display the clustered patches */ 
  cout<< "ImagePatches size = "<< m_vImagePatches.size()<<endl;
  cout<< "ClusterAssignment size = "<< m_vClusterAssignment.size()<<endl;
  if( m_bKeepPatches ) 
    qClassView->loadImageSets( m_vClusterPatches, "cl",
                               m_vImagePatches, m_vClusterAssignment );
}


/*---------------------------------------------------------*/
/*                   Clustering Functions                  */
/*---------------------------------------------------------*/

void Codebook::clusterPatches( int nFeatureType )
/*******************************************************************/
/* Start the clustering process.                                   */
/*******************************************************************/
{
  cout << "  Codebook::clusterPatches() called..." << endl;

  if( m_vFeatures.empty() ) {
    cerr << "    Error in Codebook::clusterPatches(): "
         << "No features computed yet!" << endl;
    return;
  }

  if( !m_parCluster.isValid() ) {
    cerr << "    Error in Codebook::clusterPatches(): "
         << "No parameters defined yet!" << endl;
    return;
  }

  m_vClusterTrace.clear();

  // TIMING CODE
  time_t  tu1, tu2, tu3;
  double  tc1, tc2, tc3;
  time(&tu1);
  tc1 = CPUTIME();

  int nNumFeatures = (int)m_vFeatures.size();
  
  /***********************************/
  /* Perform PCA for fast Clustering */
  /***********************************/
  vector<FeatureVector>     vEigenVectors;
  FeatureVector             fvEigenValues;
  FeatureVector             fvAvgImage;
  vector<FeatureVector>     vFeaturesOrig;
  if( m_parCluster.params()->m_bUsePCA ) {
    /*----------------------------*/
    /* Save the original features */
    /*----------------------------*/
    vFeaturesOrig = m_vFeatures;

    /*-----------------------------*/
    /* Compute a global Eigenspace */
    /*-----------------------------*/
    cout << "    Computing the PCA..." << endl;
    vector<float> vEigVals;
    calcEigenVectors( m_vFeatures,
                      vEigenVectors, vEigVals, fvAvgImage );
    fvEigenValues = vEigVals;
    
    /*--------------------------------------------------------------*/
    /* Get the PCA components corresponding to a given energy level */
    /*--------------------------------------------------------------*/
    cout << "    Examining the EV energies..." << endl;
    vector<FeatureVector> vComponents;
    getComponentsEnergy( vEigenVectors, fvEigenValues, 
                         m_parCluster.params()->m_dPCAEnergy,
                         vComponents );
    cout << "      " << setw(3) << m_parCluster.params()->m_dPCAEnergy*100.0
         << "% of the energy correspond to " << vComponents.size()
         << " components..." << endl;
    
    /*-------------------------------------------------*/
    /* Project all Feature Vectors into the Eigenspace */
    /*-------------------------------------------------*/
    cout << "    Projecting all feature vectors into the eigenspace..."
         << endl;
    for( int i=0; i<nNumFeatures; i++ )
      applyEigenVectors( vComponents, fvAvgImage, m_vFeatures[i],
                         m_vFeatures[i] );
  }

  // TIMING CODE
  time(&tu2);
  tc2 = CPUTIME();

  /*********************************************/
  /* Execute the appropriate clustering method */
  /*********************************************/
  int nClusterMethod = m_parCluster.params()->m_nClusterMethod;
  switch( nClusterMethod ) {
  case CLUSTER_KMEANS:
    /*-------------------*/
    /* KMeans Clustering */
    /*-------------------*/
    clusterPatchesKMeans( m_vFeatures, 
                          m_parCluster.params()->m_nNumClusters, 
                          m_parCluster.params()->m_dEps, 
                          m_parCluster.params()->m_nMaxIter ,
                          m_vClusterAssignment, m_vClusterTrace);
    break;

  case CLUSTER_POSTAGGLO:
    /*----------------------------------------------*/
    /* Agglomerative Clustering (similarity matrix) */
    /*----------------------------------------------*/
    clusterPatchesPostAgglo( m_vFeatures, 
                             nFeatureType, 
                             m_parCluster.params()->m_dSimilarity, 
                             m_parCluster.params()->m_dFeatureSimFact,
                             m_vClusterAssignment, m_vClusterTrace );
    break;

  case CLUSTER_RNNCAGGLO:
    /*---------------------------------*/
    /* Agglomerative Clustering (RNNC) */
    /*---------------------------------*/
    clusterPatchesRNNCAgglo( m_vFeatures, 
                             nFeatureType, 
                             m_parCluster.params()->m_dSimilarity, 
                             m_parCluster.params()->m_dFeatureSimFact,
                             m_vClusterAssignment, m_vClusterTrace );
    break;

  case CLUSTER_FASTRNNCAGGLO:
    /*-----------------------------------------------------*/
    /* Agglomerative Clustering (RNNC) with fast NN-Search */
    /*-----------------------------------------------------*/
    clusterPatchesFastRNNCAgglo( m_vFeatures, 
                                 nFeatureType, 
                                 m_parCluster.params()->m_dSimilarity, 
                                 m_parCluster.params()->m_dFeatureSimFact,
                                 m_vClusterAssignment, m_vClusterTrace );
    break;
  case CLUSTER_RANDOMFOREST:
    /*-----------------*/
    /*  Random Forest  */
    /*-----------------*/
    clusterPatchesRandomForest(	m_vImagePatches,
				m_parCluster.params()->m_nTreeNumber,
				m_parCluster.params()->m_nTreeDepth,
				m_parCluster.params()->m_nWeekClassifier,
				m_parCluster.params()->m_nMinSample,
				m_rfRandomForest);
    break;
  default:
    cerr << "  Error: Unknown clustering method '" << nClusterMethod << "'."
         << endl;
  }

  /**************************/
  /* Clustering is finished */
  /**************************/
  // TIMING CODE
  time(&tu3);
  tc3 = CPUTIME();
  cout << endl;
  
  /*******************************/
  /* Backproject the PCA Results */
  /*******************************/
  if( m_parCluster.params()->m_bUsePCA ) {
    /* restore the original feature vectors */
    m_vFeatures = vFeaturesOrig;
    }

  /***************************/
  /* Compute cluster centers */
  /***************************/
  cout << "    Computing cluster centers..." << endl;
  if(nClusterMethod == CLUSTER_RANDOMFOREST)
	computeClusterCenters_HF();
  else
  	computeClusterCenters();

  cout << "  Found " << m_vClusters.size() << " clusters." << endl;

  cout << "  ----------------------" << endl;
  cout << "  Time spent for..." << endl;
  if( m_parCluster.params()->m_bUsePCA )
    cout << "    PCA          : " << setw(12)
         << tc2-tc1 << "s (system), "
         << tu2-tu1 << "s (user)" << endl;
  cout << "    Clustering   : " << setw(12)
       << tc3-tc2 << "s (system), "
       << tu3-tu2 << "s (user)" << endl;
  cout << endl;
  cout << "  ----------------------" << endl;
  cout << endl;

  /* also set the parameters for the MatchingGUI */
  updateMatchingParams( nFeatureType );
}

void Codebook::clusterPatchesRandomForest(	const vector<OpGrayImage>& vPatches,
						int nTreeNumber,
						int nTreeDepth,
						int nWeekClassifier,	
						int nMinSample,
						RandomForest& rfForest)
{
	// Check if the Patches are loaded
	cout<<"	   Clustering with random forest..."<<endl;
	if(vPatches.size()==0){
	cout<<"    Error: No patch has been loaded!" << endl;
  	return;
	}
	// Set the parameters
	rfForest.setParams(nTreeNumber, nTreeDepth, nWeekClassifier, nMinSample);
	cout<<"    Preparing the patches..."<<endl;
	vector<PatchSample> vPatchSamples;
 	vector<int>::iterator itr_class = m_vFeatureClass.begin();	
	int nTotalClass = *max_element(m_vFeatureClass.begin(),m_vFeatureClass.end())+1;
	for(vector<OpGrayImage>::const_iterator itr = vPatches.begin();itr!=vPatches.end();++itr){
		PatchSample tmpPatchSample;
		tmpPatchSample.vChannels.push_back(*itr);
		tmpPatchSample.nChannels = 1;
		tmpPatchSample.nTotalClasses = nTotalClass;
		tmpPatchSample.nClassLabel = *itr_class;
		vPatchSamples.push_back(tmpPatchSample);
		itr_class++;
	}
	// Train the forest
	rfForest.trainRandomForest(vPatchSamples);
	cout<<"	   done!"<<endl;
	// save the centers
	// 
}
void Codebook::clusterPatchesKMeans(  const vector<FeatureVector> &vFeatures,
                                      int nNumClusters,
                                      float dEps, int nMaxIter,
                                      vector<int>    &vClusterAssignment,
                                      vector<ClStep> &vClusterTrace )
{
  cout << "    Clustering with k-Means..." << endl;

  /* convert the features into the clustering format */
  //vector<ClPoint> vClPoints;
  //vClPoints = cnvFeatureVecsToClPoints( vFeatures );

  cout << "      Initializing with " << nNumClusters << " Clusters..."
       << endl;
  //ClKMeans clkmeans( vClPoints );
  ClKMeans clkmeans( vFeatures );
  clkmeans.initClusters( nNumClusters );
  
  cout << "      Performing the clustering steps..." << endl;
  clkmeans.doClusterSteps( dEps, nMaxIter );
  
  /* get the cluster centers */
  //vClPoints = clkmeans.getClusterCenters();

  /* get the cluster assignment */
  vClusterAssignment = clkmeans.getClusterAssignment();
  vClusterTrace.clear();

  cout << "  done. " << endl;
}


void Codebook::clusterPatchesPostAgglo( const vector<FeatureVector> &vFeatures,
                                        int   nFeatureType, 
                                        float dSimilarity,
                                        float dFeatureSimFact,
                                        vector<int>    &vClusterAssignment,
                                        vector<ClStep> &vClusterTrace )
{
  cout << "    Clustering with SimAgglo... (post)" << endl;
  /* different treatment for the different feature types! */
  
  /* Precompute the similarity matrix */
  vector<float> mSimMatrix;
  computeSimilarityMatrix( vFeatures, nFeatureType, dFeatureSimFact,
                           mSimMatrix );
  
  /* Start the clustering process */
  //vector<ClPoint> vClPoints = cnvFeatureVecsToClPoints( vFeatures );
  //ClPostAgglo clAgglo( vClPoints, mSimMatrix );
  ClPostAgglo clAgglo( vFeatures, mSimMatrix );
  
  int nNumFeatures = (int)vFeatures.size();
  cout << "      Initializing with " << nNumFeatures << " Clusters..."
       << endl;
  clAgglo.initClusters( nNumFeatures );
  
  cout << "      Performing the clustering steps..." << endl;
  clAgglo.doClusterSteps( dSimilarity );
  
  //vClPoints = clAgglo.getClusterCenters();

  /* get the cluster assignment */
  vClusterAssignment = clAgglo.getClusterAssignment();
  vClusterTrace      = clAgglo.getClusterTrace();

  cout << "      Computing cluster prototypes..." << endl;
  computeClusterPrototypes( mSimMatrix );

  cout << "  done. " << endl;
}


void Codebook::clusterPatchesRNNCAgglo( const vector<FeatureVector> &vFeatures,
                                        int   nFeatureType, 
                                        float dSimilarity,
                                        float dFeatureSimFact,
                                        vector<int>    &vClusterAssignment,
                                        vector<ClStep> &vClusterTrace )
{
  cout << "    Clustering with RNNCAgglo..." << endl;
  
  /* convert the features into the clustering format */
  //vector<ClPoint> vClPoints;
  //vClPoints = cnvFeatureVecsToClPoints( vFeatures );

  //ClRNNCAgglo clAgglo( vClPoints );
  ClRNNCAgglo clAgglo( vFeatures );
  //vClPoints.clear();
  
  int nNumFeatures = (int)vFeatures.size();
  cout << "      Initializing with " << nNumFeatures << " Clusters..." << endl;
  clAgglo.initClusters( nNumFeatures );
    
  /* different treatment for the different feature types! */
  float dSimThresh;
  switch( nFeatureType ) {
  case FEATURE_PATCH:
  case FEATURE_PATCHMIKO:
    /*-=-=-=-=-=-=-=-=*/
    /* Patch Features */
    /*-=-=-=-=-=-=-=-=*/
    clAgglo.setMetric( METRIC_NGC );
    dSimThresh = dSimilarity;
    break;
    
  case FEATURE_PATCHMIKO2:
  case FEATURE_STEERABLE:
  case FEATURE_SIFT:
  case FEATURE_GLOH:
  case FEATURE_MOMENTS:
  case FEATURE_DIFFINV:
  case FEATURE_COMPLEX:
  case FEATURE_SHAPECONTEXT:
  case FEATURE_SPINIMGS:
  case FEATURE_GRADIENTPCA:
  case FEATURE_SURF64:
  case FEATURE_SURF128: {
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    /* Mikolajczyk's Features */
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    clAgglo.setMetric( METRIC_EUCLID );
    
    float dDimFact = (float)m_vFeatures.front().numDims();
    dSimThresh  = dFeatureSimFact*dDimFact * log(dSimilarity);
    break;
  }
    
  default:
    cerr << "    Error in clusterPatches(): "
         << "Unknown feature type (" << nFeatureType << ")!" << endl;
    return;
  }

  cout << "    Performing the clustering steps..." << endl;
  clAgglo.doClusterSteps( dSimThresh );
  
  /* get the cluster assignment */
  vClusterAssignment = clAgglo.getClusterAssignment();
  vClusterTrace      = clAgglo.getClusterTrace();

  cout << "  done. " << endl;
}


void Codebook::clusterPatchesFastRNNCAgglo( const vector<FeatureVector> &vFeatures,
                                            int   nFeatureType, 
                                            float dSimilarity,
                                            float dFeatureSimFact,
                                            vector<int>    &vClusterAssignment,
                                            vector<ClStep> &vClusterTrace )
{
  cout << "    Clustering with RNNCAgglo..." << endl;
  
  ClFastRNNCAgglo clAgglo( vFeatures, m_parCluster.params()->m_nMaxNodeSize );
  
  int nNumFeatures = (int)vFeatures.size();
  cout << "      Initializing with " << nNumFeatures << " Clusters..." << endl;
  clAgglo.initClusters( nNumFeatures );
    
  /* different treatment for the different feature types! */
  float dSimThresh;
  switch( nFeatureType ) {
  case FEATURE_PATCH:
  case FEATURE_PATCHMIKO:
    /*-=-=-=-=-=-=-=-=*/
    /* Patch Features */
    /*-=-=-=-=-=-=-=-=*/
    cerr << "  Error in ClusterPatchesFastRNNCAgglo(): "
         << "Fast NN-Search only implemented for Euclidean distances!" << endl;
    return;
    break;
    
  case FEATURE_PATCHMIKO2:
  case FEATURE_STEERABLE:
  case FEATURE_SIFT:
  case FEATURE_GLOH:
  case FEATURE_MOMENTS:
  case FEATURE_DIFFINV:
  case FEATURE_COMPLEX:
  case FEATURE_SHAPECONTEXT:
  case FEATURE_SPINIMGS:
  case FEATURE_GRADIENTPCA: 
  case FEATURE_SURF64:
  case FEATURE_SURF128: {
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    /* Mikolajczyk's Features */
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    clAgglo.setMetric( METRIC_EUCLID );
    
    float dDimFact = (float)m_vFeatures.front().numDims();
    dSimThresh  = dFeatureSimFact*dDimFact * log(dSimilarity);
    break;
  }
    
  default:
    cerr << "    Error in clusterPatches(): "
         << "Unknown feature type (" << nFeatureType << ")!" << endl;
    return;
  }

  cout << "    Performing the clustering steps..." << endl;
  clAgglo.doClusterSteps( dSimThresh );
  
  /* get the cluster assignment */
  vClusterAssignment = clAgglo.getClusterAssignment();
  vClusterTrace      = clAgglo.getClusterTrace();

  cout << "  done. " << endl;
}


void Codebook::computeSimilarityMatrix( const vector<FeatureVector> &vFeatures,
                                        int   nFeatureType,
                                        float dFeatureSimFact,
                                        vector<float> &mSimMatrix )
{
  int nNumFeatures = (int)vFeatures.size();
  cout << "    Computing similarity matrix for " << nNumFeatures
       << " features..." << endl;
  cout << "      (>20,000 means more than 2GB of RAM needed!)" << endl;

  mSimMatrix = vector<float>( nNumFeatures*nNumFeatures );

  float dDimFact = 1.0;
  cout << "      using feature type: " << nFeatureType << endl;
  cout << "      with dimensionality: " << m_vFeatures.front().numDims() 
       << endl;

  switch( nFeatureType ) {
  case FEATURE_PATCH:
  case FEATURE_PATCHMIKO: {
    /*-=-=-=-=-=-=-=-=*/
    /* Patch Features */
    /*-=-=-=-=-=-=-=-=*/
    for( int i=0; i<nNumFeatures; i++ )
      for( int j=0; j<nNumFeatures; j++ )
        if( i > j ) {
          int idx = i*nNumFeatures + j;

          float dSim = vFeatures[i].compCorrelation( vFeatures[j] );
          mSimMatrix[idx] = dSim;
        }
    break;
  }
    
  case FEATURE_PATCHMIKO2:
  case FEATURE_STEERABLE:
  case FEATURE_SIFT:
  case FEATURE_GLOH:
  case FEATURE_MOMENTS:
  case FEATURE_DIFFINV:
  case FEATURE_COMPLEX:
  case FEATURE_SHAPECONTEXT:
  case FEATURE_SPINIMGS:
  case FEATURE_GRADIENTPCA:
  case FEATURE_SURF64:
  case FEATURE_SURF128: 
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    /* Mikolajczyk's Features */
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    dDimFact = (float)vFeatures.front().numDims();
    
    for( int i=0; i<nNumFeatures; i++ )
      for( int j=0; j<nNumFeatures; j++ )
        if( i > j ) {
          int idx = i*nNumFeatures + j;
          float dDist = vFeatures[i].compSSD( vFeatures[j] );
          float dSim  = exp( -dDist/(dFeatureSimFact*dDimFact) );
          mSimMatrix[idx] = dSim;
        }
    break;
    
  default: 
    cerr << "      Error in computeSimilarityMatrix(): "
         << "Unknown feature type (" << nFeatureType << ")!" << endl;
  }
  
  cout << "    Similarity matrix computed. (size: " 
       << mSimMatrix.size() << ")" << endl;
}


void Codebook::recreateCodebookFromTrace( int nFeatureType )
{
  float dSimLevel = m_parCluster.params()->m_dSimilarity;
  float dDimFact = 1.0;
  switch( nFeatureType ) {
  case FEATURE_PATCH:
  case FEATURE_PATCHMIKO:
    /*-=-=-=-=-=-=-=-=*/
    /* Patch Features */
    /*-=-=-=-=-=-=-=-=*/
    break;
    
  case FEATURE_PATCHMIKO2:
  case FEATURE_STEERABLE:
  case FEATURE_SIFT:
  case FEATURE_GLOH:
  case FEATURE_MOMENTS:
  case FEATURE_DIFFINV:
  case FEATURE_COMPLEX:
  case FEATURE_SHAPECONTEXT:
  case FEATURE_SPINIMGS:
  case FEATURE_GRADIENTPCA:
  case FEATURE_SURF64:
  case FEATURE_SURF128: 
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    /* Mikolajczyk's Features */
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    dDimFact = (float)m_vFeatures.front().numDims();
    
    dSimLevel  = ( m_parCluster.params()->m_dFeatureSimFact*dDimFact * 
                   log(m_parCluster.params()->m_dSimilarity) );
    break;
    
  default:
    cerr << "    Error in createCodebookFromTrace(): "
         << "Unknown feature type (" << nFeatureType 
         << ")!" << endl;
    }

  switch( m_parCluster.params()->m_nReconstMethod ) {
  case RECONST_COMPRATIO:
    recreateCodebookCompRatio( m_parCluster.params()->m_dCompressionRatio,
                               nFeatureType );
    break;
  case RECONST_SIMLEVEL:
    recreateCodebookSimLevel( m_parCluster.params()->m_dSimLevel,
                              nFeatureType );
    break;

  default:
    cerr << "    Error in createCodebookFromTrace(): "
         << "Unknown cluster reconstruction method (" 
         <<  m_parCluster.params()->m_nReconstMethod << ")!" << endl;
  }
}


void Codebook::recreateCodebookSimLevel( float dSimLevel, int nFeatureType )
{
  cout << "  Recreating codebook to similarity level " << dSimLevel 
       << "..." << endl;

  /*----------------------*/
  /* Adjust the sim level */
  /*----------------------*/
  float dDimFact = (float)m_vFeatures.front().numDims();
  //dSimLevel  = ( m_parCluster.params()->m_dFeatureSimFact*dDimFact * 
  //               log(dSimLevel) );
  switch( nFeatureType ) {
  case FEATURE_PATCH:
  case FEATURE_PATCHMIKO:
    /*-=-=-=-=-=-=-=-=*/
    /* Patch Features */
    /*-=-=-=-=-=-=-=-=*/
    //dSimLevel = dSimLevel;
    break;
    
  case FEATURE_PATCHMIKO2:
  case FEATURE_STEERABLE:
  case FEATURE_SIFT:
  case FEATURE_GLOH:
  case FEATURE_MOMENTS:
  case FEATURE_DIFFINV:
  case FEATURE_COMPLEX:
  case FEATURE_SHAPECONTEXT:
  case FEATURE_SPINIMGS:
  case FEATURE_GRADIENTPCA:
  case FEATURE_SURF64:
  case FEATURE_SURF128: 
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    /* Mikolajczyk's Features */
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    dSimLevel  = ( m_parCluster.params()->m_dFeatureSimFact*dDimFact*
                    log(dSimLevel) );
    break;
    
  default:
    cerr << "    Error in createCodebookSimLevel(): "
         << "Unknown feature type (" << nFeatureType << ")!" << endl;
    return;
  }
  cout << "    (corresponding to internal sim level: " << dSimLevel << ")" 
       <<endl;

  /*-----------------------------*/
  /* Create an assignment vector */
  /*-----------------------------*/
  int nNumFeatures =  (int)m_vClusterAssignment.size();
  vector<int> vAssignment( nNumFeatures );
  for( int i=0; i<nNumFeatures; i++ )
    vAssignment[i] = i;

  /*------------------------------*/
  /* Process the clustering trace */
  /*------------------------------*/
  /* DON'T sort the trace!!! */
  //stable_sort( m_vClusterTrace.begin(), m_vClusterTrace.end(), compClStepAsc() );

  cout << "    Processing the trace (" << m_vClusterTrace.size() 
       << " entries)..." << endl;
  for( int i=0; i<(int)m_vClusterTrace.size(); i++ ) {
    ClStep csStep = m_vClusterTrace[i];
    if( csStep.dSim >= dSimLevel ) {
      vAssignment[csStep.nIdx1] = csStep.nNewIdx;
      vAssignment[csStep.nIdx2] = csStep.nNewIdx;
    }
  }

  /*----------------------------------------*/
  /* Resolve assignments over several steps */
  /*----------------------------------------*/
  cout << "    Resolving assignments..." << endl;
  vector<int> vNewAssign = vAssignment;
  for( int i=0; i<nNumFeatures; i++ )
    if( vAssignment[i] != i ) {
      int nNewIdx = vAssignment[i];
      while( vAssignment[nNewIdx] != nNewIdx )
        nNewIdx = vAssignment[nNewIdx];
      vNewAssign[i] = nNewIdx;
    }

  /*-----------------------*/
  /* Renaming the clusters */
  /*-----------------------*/
  cout << "    Renaming the clusters..." << endl;
  vector<int> vNewClusterIdx( nNumFeatures, -1 );
  int nClusters=0;
  for( int i=0; i<nNumFeatures; i++ ) {
    
    /* check if the cluster has already been registered */
    if( vNewClusterIdx[ vNewAssign[i] ] == -1 ) {
      vNewClusterIdx[ vNewAssign[i] ] = nClusters;
      nClusters++;
    }

    /* assign the new cluster index */
    vNewAssign[i] = vNewClusterIdx[ vNewAssign[i] ];
  }
  cout << "      found " << nClusters << " clusters." << endl;

  /*------------------------------------------------------*/
  /* Create the final codebook from the assignment vector */
  /*------------------------------------------------------*/
  cout << "    Computing cluster centers..." << endl;
  m_vClusterAssignment = vNewAssign;
  computeClusterCenters();

  cout << "  done." << endl;
}


void Codebook::recreateCodebookCompRatio( float dCompRatio, int nFeatureType )
{
  cout << "  Recreating codebook to compression ratio " << dCompRatio 
       << "..." << endl;

  /*------------------------------------------*/
  /* Determine the desired number of clusters */
  /*------------------------------------------*/
  int nTargetClusters = (int)floor(m_vClusterAssignment.size()/dCompRatio+0.5);

  /*----------------------*/
  /* Sort the trace steps */
  /*----------------------*/
  /* sort the trace!!! */
  stable_sort(m_vClusterTrace.begin(), m_vClusterTrace.end(), compClStepAsc());

  /*-----------------------------*/
  /* Create an assignment vector */
  /*-----------------------------*/
  int nNumFeatures =  (int)m_vClusterAssignment.size();
  vector<int> vAssignment( nNumFeatures );
  for( int i=0; i<nNumFeatures; i++ )
    vAssignment[i] = i;

  /*------------------------------*/
  /* Process the clustering trace */
  /*------------------------------*/
  cout << "    Processing the trace (" << m_vClusterTrace.size() 
       << " entries)..." << endl;
  int nSteps = (int)min( m_vClusterAssignment.size()-nTargetClusters, 
                         m_vClusterTrace.size() );
  cout << "      for " << nSteps << " steps." << endl;
  float dLastSim = 0.0;
  for( int i=0; i<nSteps; i++ ) {
    ClStep csStep = m_vClusterTrace[i];
    //if( csStep.dSim >= dSimLevel ) {
    vAssignment[csStep.nIdx1] = csStep.nNewIdx;
    vAssignment[csStep.nIdx2] = csStep.nNewIdx;
    dLastSim = csStep.dSim;
    //}
  }
  cout << "      last similarity: " << dLastSim << endl;

  /*------------------------------------------*/
  /* Reconstruct the cut-off similarity level */
  /*------------------------------------------*/
  float dSimLevel;
  float dDimFact = 1.0;
  switch( nFeatureType ) {
  case FEATURE_PATCH:
  case FEATURE_PATCHMIKO:
    /*-=-=-=-=-=-=-=-=*/
    /* Patch Features */
    /*-=-=-=-=-=-=-=-=*/
    dSimLevel = dLastSim;
    break;
    
  case FEATURE_PATCHMIKO2:
  case FEATURE_STEERABLE:
  case FEATURE_SIFT:
  case FEATURE_GLOH:
  case FEATURE_MOMENTS:
  case FEATURE_DIFFINV:
  case FEATURE_COMPLEX:
  case FEATURE_SHAPECONTEXT:
  case FEATURE_SPINIMGS:
  case FEATURE_GRADIENTPCA:
  case FEATURE_SURF64:
  case FEATURE_SURF128: 
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    /* Mikolajczyk's Features */
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    dDimFact = (float)m_vFeatures.front().numDims();
    
    dSimLevel  = exp( dLastSim/(m_parCluster.params()->m_dFeatureSimFact*
                                dDimFact) );

    break;
    
  default:
    cerr << "    Error in createCodebookCompRatio(): "
         << "Unknown feature type (" << nFeatureType << ")!" << endl;
    return;
  }
  cout << "    sim level: " << dSimLevel << endl;
  m_parCluster.params()->slotSetSimilarity(QString::number(dSimLevel));

  /* also set the parameters for the MatchingGUI */
  updateMatchingParams( nFeatureType );

  /*----------------------------------------*/
  /* Resolve assignments over several steps */
  /*----------------------------------------*/
  cout << "    Resolving assignments..." << endl;
  vector<int> vNewAssign = vAssignment;
  for( int i=0; i<nNumFeatures; i++ )
    if( vAssignment[i] != i ) {
      int nNewIdx = vAssignment[i];
      while( vAssignment[nNewIdx] != nNewIdx )
        nNewIdx = vAssignment[nNewIdx];
      vNewAssign[i] = nNewIdx;
    }

  /*---------------------*/
  /* Rename the clusters */
  /*---------------------*/
  cout << "    Renaming the clusters..." << endl;
  vector<int> vNewClusterIdx( nNumFeatures, -1 );
  int nClusters=0;
  for( int i=0; i<nNumFeatures; i++ ) {
    
    /* check if the cluster has already been registered */
    if( vNewClusterIdx[ vNewAssign[i] ] == -1 ) {
      vNewClusterIdx[ vNewAssign[i] ] = nClusters;
      nClusters++;
    }

    /* assign the new cluster index */
    vNewAssign[i] = vNewClusterIdx[ vNewAssign[i] ];
  }
  cout << "      found " << nClusters << " clusters." << endl;

  /*------------------------------------------------------*/
  /* Create the final codebook from the assignment vector */
  /*------------------------------------------------------*/
  cout << "    Drawing the codebook..." << endl;
  m_vClusterAssignment = vNewAssign;
  computeClusterCenters();

  cout << "  done." << endl;
}


void Codebook::remove1PatchClusters()
/*******************************************************************/
/* Remove all clusters that contain only a single patch.           */
/*******************************************************************/
{
  vector<int> vRemovedNums;
  remove1PatchClusters( vRemovedNums );
}


void Codebook::remove1PatchClusters( vector<int> &vRemovedNums )
/*******************************************************************/
/* Remove all clusters that contain only a single patch.           */
/*******************************************************************/
{
  if( !m_bClustersValid ) {
    cerr << "  WARNING in Codebook::remove1PatchClusters(): "
         << "Clusters not valid yet!" << endl;
    return;
  }

  /* compute the number of clusters */
  int nNumFeatures = (int)m_vFeatures.size();
  int nNumClusters = (int)m_vClusters.size();
  
  /*---------------------------*/
  /* Compute the cluster sizes */
  /*---------------------------*/
  vector<int> vSizes( nNumClusters, 0 );
  for( int i=0; i<(int)m_vClusterAssignment.size(); i++ )
    vSizes[m_vClusterAssignment[i]]++;
  
  vector<bool> vRemoveCl( nNumClusters, false );
  vRemovedNums.clear();
  for( int i=0; i<nNumClusters; i++ )
    if( vSizes[i]<=1 ) {
      vRemoveCl[i] = true;
      vRemovedNums.push_back( i );
    }

  /*-----------------------------*/
  /* Remove the flagged clusters */
  /*-----------------------------*/
  removeClusters( vRemoveCl );
}
 

void Codebook::removeClusters( const vector<bool> &vIdzs )
/*******************************************************************/
/* Remove all clusters that contain only a single patch.           */
/*******************************************************************/
{
  if( vIdzs.size()!=m_vClusters.size() ) {
    cerr << "ERROR in Codebook::removeClusters(): "
         << "dimension of index vector doesn't match ("
         << vIdzs.size() << " instead of " << m_vClusters.size()
         << ")!" << endl;
    return;
  }
  
  /* compute the number of clusters */
  int nNumFeatures = (int)m_vFeatures.size();
  int nNumClusters = (int)m_vClusters.size();

  /*-----------------------------------*/
  /* Remove the corresponding Clusters */
  /*-----------------------------------*/  
  /* Get the new cluster numbers */
  int nCurrent = 0;
  vector<int> vNewClusterNum( nNumClusters );
  for( int i=0; i<nNumClusters; i++ )
    if( vIdzs[i] )
      vNewClusterNum[i] = -1;
    else
      vNewClusterNum[i] = nCurrent++;
  
  /*-----------------------------*/
  /* Remove all flagged features */
  /*-----------------------------*/
  /* Get the new assignment numbers */
  nCurrent = 0;
  vector<int>           vNewFeatureNum( nNumFeatures );
  vector<OpGrayImage>   vNewPatches;
  vector<FeatureVector> vNewFeatures;
  vector<int>           vNewAssignment;
  for( int i=0; i<nNumFeatures; i++ )
    if( vIdzs[m_vClusterAssignment[i]] )
      vNewFeatureNum[i] = -1;
    else {
      vNewFeatureNum[i] = nCurrent++;

      if( !m_vImagePatches.empty() && m_bKeepPatches )
        vNewPatches.push_back( m_vImagePatches[i] );
      vNewFeatures.push_back( m_vFeatures[i] );
      vNewAssignment.push_back(vNewClusterNum[m_vClusterAssignment[i]]);

      if( vNewClusterNum[m_vClusterAssignment[i]]<0 )
        cerr << "WARNING in Codebook::removeClusters(): "
             << "new feature " << i << " is assigned to removed cluster!"
             << endl;
    }
    
  /*--------------------------------------*/
  /* Adjust the cluster trace accordingly */
  /*--------------------------------------*/
  /* DON'T sort the trace!!! */
  //stable_sort( m_vClusterTrace.begin(), m_vClusterTrace.end(), compClStepAsc() );

  vector<ClStep> vNewTrace;
  for( int i=0; i<(int)m_vClusterTrace.size(); i++ ) {
    ClStep &csItem = m_vClusterTrace[i];

    if( vNewFeatureNum[csItem.nNewIdx]<0 ) {
      if( vNewFeatureNum[csItem.nIdx1]<0 )
        vNewFeatureNum[csItem.nNewIdx] = vNewFeatureNum[csItem.nIdx2];
      else
        vNewFeatureNum[csItem.nNewIdx] = vNewFeatureNum[csItem.nIdx1];
      
    } else if( vNewFeatureNum[csItem.nIdx1]<0 ) {
      // -1 + A --> B
      if( vNewFeatureNum[csItem.nNewIdx]!=vNewFeatureNum[csItem.nIdx2] ) {
        cerr << "  WARNING: case -1 + A --> B in line " << i << " of trace!"
             << endl;
        vNewFeatureNum[csItem.nNewIdx] = vNewFeatureNum[csItem.nIdx2];
      }
    } else if( vNewFeatureNum[csItem.nIdx2]<0 ) {
      // A + -1 --> B
      if( vNewFeatureNum[csItem.nNewIdx]!=vNewFeatureNum[csItem.nIdx1] ) {
        cerr << "  WARNING: case A + -1 --> B in line " << i << " of trace!"
             << endl;
        vNewFeatureNum[csItem.nNewIdx] = vNewFeatureNum[csItem.nIdx1];
      }
      
    } else {
      csItem.nIdx1   = vNewFeatureNum[csItem.nIdx1];
      csItem.nIdx2   = vNewFeatureNum[csItem.nIdx2];
      csItem.nNewIdx = vNewFeatureNum[csItem.nNewIdx];
      vNewTrace.push_back( csItem );
    }
  }
  
  /*----------------------------------*/
  /* Write the Filtered Clusters Back */
  /*----------------------------------*/
  assert( vNewFeatures.size() == vNewAssignment.size() );
  
  //-- update members --//
  m_vFeatures.clear();
  m_vImagePatches.clear();

  if( m_bKeepPatches )
    m_vImagePatches    = vNewPatches;
  m_vFeatures          = vNewFeatures;
  m_vClusterAssignment = vNewAssignment;
  m_vClusterTrace      = vNewTrace;

  computeClusterCenters();
}

 
/*---------------------------------------------------------*/
/*                   Eigenspace Functions                  */
/*---------------------------------------------------------*/

/*---------------------------------------------------------*/
/*                   Auxiliary Functions                   */
/*---------------------------------------------------------*/


bool Codebook::loadFeatures( string sFileName, 
                             vector<FeatureVector> &vFeatures,
                             bool bVerbose )
/*******************************************************************/
/* load the extracted features from disk (as a FeatureVector list).*/
/*******************************************************************/
{
  if( bVerbose )
    cout << "  Loading features..." << endl;

  /* load the FeatureVector list */
  bool ok = loadFeatureVectorList( sFileName, vFeatures, bVerbose );
  if ( !ok ) {
    cerr << "    Loading Feature Vectors failed..." << endl
         << "      (Path is: '" << sFileName << "')" << endl;
    return false;
  }
  
  if( bVerbose )
    cout << "  done." << endl;
  return true;
}


bool Codebook::saveFeatures( string sFileName, bool bVerbose )
/*******************************************************************/
/* Save the extracted features to disk (as a FeatureVector list).  */
/*******************************************************************/
{
  if( bVerbose )
    cout << "  Saving features..." << endl;

  /* save the FeatureVector list */
  bool ok = saveFeatureVectorList( sFileName, m_vFeatures );
  if ( !ok ) {
    cerr << "    Saving Feature Vectors failed..." << endl
         << "      (Path is: " << sFileName << "')" << endl;
    return false;
  }
  
  if( bVerbose )
    cout << "  done." << endl;
  return true;
}


void Codebook::loadPatches( string sFileName, 
                            vector<OpGrayImage> &vImagePatches,
                            bool bVerbose )
/*******************************************************************/
/* load the extracted patches from disk (as a FeatureVector list). */
/*******************************************************************/
{
  if( bVerbose )
    cout << "  Loading image patches..." << endl;

  /* load a feature vector list with the patches */
  vector<FeatureVector> vData;
  bool ok = loadFeatureVectorList( sFileName, vData );
  if ( !ok ) {
    if( bVerbose )
      cerr << "    Loading patches failed..." << endl
           << "      (Path is: '" << sFileName << "')" << endl
           << "    Trying to use the features as patches..." << endl;
    vData = m_vFeatures;
  }

  /* convert the feature vectors patches */
  vImagePatches.clear();
  for( int i=0; i<(int)vData.size(); i++ ) {
    OpGrayImage imgPatch;
    //int imgsize = 2*m_fcCue.params()->m_nPatchSize+1;
    int imgsize = (int)floor(sqrt((double)vData[i].numDims())+0.5);
    imgPatch.loadFromData( imgsize, imgsize, vData[i].getData() );
    vImagePatches.push_back( imgPatch );
  }
  if( bVerbose )
    cout << "  done." << endl;
}


void Codebook::savePatches( string sFileName, bool bVerbose )
/*******************************************************************/
/* Save the extracted patches to disk (as a FeatureVector list).   */
/*******************************************************************/
{
  if( bVerbose )
    cout << "  Saving image patches..." << endl;

  /* create a FeatureVector list from the patches */
  vector<FeatureVector> vData;
  for( int i=0; i<(int)m_vImagePatches.size(); i++ )
    vData.push_back( m_vImagePatches[i].getData() );
    cout<<"   Number of image patches:"<<(int)m_vImagePatches.size()<<endl;
  /* save the converted feature vectors */
  bool ok = saveFeatureVectorList( sFileName, vData );
  if ( !ok )
    cerr << "    Saving patches failed..." << endl
         << "    Path is: '" << sFileName << "'" << endl;

  if( bVerbose )
    cout << "  done." << endl;
}


void Codebook::loadClusterAssignments( string sFileName, 
                                     vector<int> &vClusterAssignment,
                                       bool bVerbose )
/*******************************************************************/
/* Load the cluster assignments from disk (in plain ASCII format). */
/*******************************************************************/
{
  if( bVerbose )
    cout << "  Loading cluster assignments..." << endl;

  ifstream ifile( sFileName.c_str() );
  if ( !ifile ) {
    cerr << "No corresponding assignment file (*.ass) found." << endl;
    return;
  }
  
  vClusterAssignment.clear();
  int nNumAssignments;
  int currentCl;
  ifile >> nNumAssignments;
  for(int i=0; i < nNumAssignments; i++) {
    ifile >> currentCl;
    vClusterAssignment.push_back( currentCl );
  }
  ifile.close();

  if( bVerbose )
    cout << "  done." << endl;
}


void Codebook::saveClusterAssignments( string sFileName, bool bVerbose )
/*******************************************************************/
/* Save the cluster assignments to disk (in plain ASCII format).   */
/*******************************************************************/
{
  if( bVerbose )
    cout << "  Saving cluster assignments..." << endl;

  ofstream ofile( sFileName.c_str() );
  if ( !ofile ) {
    cerr <<"Can't open output file: " << sFileName << endl;
    return;
  }
  ofile << m_vClusterAssignment.size() << endl;
  for(int i=0; i < (int)m_vClusterAssignment.size(); i++) {
    ofile << m_vClusterAssignment[i] << " ";
  }
  ofile.close();

  if( bVerbose )
    cout << "  done." << endl;
}


void Codebook::loadClusterTrace( string sFileName,
                                 vector<ClStep> &vClusterTrace,
                                 bool bVerbose )
/*******************************************************************/
/* Save the cluster trace to disk (in plain ASCII format.)         */
/*******************************************************************/
{
  if( bVerbose )
    cout << "  Loading cluster trace..." << endl;

  ifstream ifile( sFileName.c_str() );
  if ( !ifile ) {
    cerr <<"Can't open input file: " << sFileName << endl;
    return;
  }

  vClusterTrace.clear();
  int nNumTraceSteps;
  ifile >> nNumTraceSteps;
  for( int i=0; i<nNumTraceSteps; i++ ) {
    int   nIdx1, nIdx2, nNewIdx;
    float dSim;
    ifile >> nIdx1 >> nIdx2 >> dSim >> nNewIdx;
    vClusterTrace.push_back( ClStep(nIdx1, nIdx2, dSim, nNewIdx) );
  }
  ifile.close();

  if( bVerbose )
    cout << "  done." << endl;
}

void Codebook::saveClusterTrace( string sFileName, bool bVerbose )
/*******************************************************************/
/* Save the cluster trace to disk (in plain ASCII format.)         */
/*******************************************************************/
{
  if( bVerbose )
    cout << "  Saving cluster trace..." << endl;

  ofstream ofile( sFileName.c_str() );
  if ( !ofile ) {
    cerr <<"Can't open output file: " << sFileName << endl;
    return;
  }

  ofile << m_vClusterTrace.size() << endl;
  for(int i=0; i < (int)m_vClusterTrace.size(); i++) {
    ofile << m_vClusterTrace[i].nIdx1 << " "
          << m_vClusterTrace[i].nIdx2 << " "
          << m_vClusterTrace[i].dSim  << " "
          << m_vClusterTrace[i].nNewIdx << endl;
  }
  ofile.close();

  if( bVerbose )
    cout << "  done." << endl;
}


void Codebook::loadPrototypes( string sFileName, bool bVerbose )
/*******************************************************************/
/* Load the most typical patch in each cluster (used e.g. for      */
/* Chamfer similarities, where cluster mean makes no sense).       */
/*******************************************************************/
{
  if( bVerbose )
    cout << "  Loading prototypes..." << endl;

  ifstream ifile( sFileName.c_str() );
  int nNumEntries;
  ifile >> nNumEntries;
  for( int i=0; i<nNumEntries; i++ ) {
    int nIdx;
    ifile >> nIdx;
    m_vPrototypes.push_back( nIdx );
  }
  ifile.close();

  if( bVerbose )
    cout << "  done." << endl;
}


void Codebook::savePrototypes( string sFileName, bool bVerbose )
/*******************************************************************/
/* Save the most typical patch in each cluster (used e.g. for      */
/* Chamfer similarities, where cluster mean makes no sense).       */
/*******************************************************************/
{
  if( !m_bPrototypesValid ) {
    cerr << "  Error in Codebook::savePrototypes(): "
         << "Prototypes not yet computed!" << endl;
    return;
  }

  /*-----------------*/
  /* Save prototypes */
  /*-----------------*/
  if( bVerbose )
    cout << "  Saving prototypes..." << endl;

  ofstream ofile( sFileName.c_str() );
  if ( !ofile ) {
    cerr <<"Can't open output file: " << sFileName << endl;
    return;
  }

  ofile << m_vPrototypes.size() << endl;
  for( int i=0; i <(int)m_vPrototypes.size(); i++ ) {
    assert(m_vPrototypes[i]<(int)m_vFeatures.size());

    ofile << m_vPrototypes[i] << " ";
  }
  ofile << endl;
  ofile.close();

  if( bVerbose )
    cout << "  done." << endl;
}


void Codebook::updateMatchingParams( int nFeatureType )
{  
  /* set the rejection threshold */
  m_parMatching.params()->slotSetRejectionThreshold(QString::number(m_parCluster.params()->m_dSimilarity));

  /* set the feature sim factor */
  m_parMatching.params()->slotSetFeatureSimFact(QString::number(m_parCluster.params()->m_dFeatureSimFact));
 
  /* set the similarity measure */
  switch( nFeatureType ) {
  case FEATURE_PATCH:
  case FEATURE_PATCHMIKO:
    /*-=-=-=-=-=-=-=-=*/
    /* Patch Features */
    /*-=-=-=-=-=-=-=-=*/
    m_parMatching.params()->slotSelectCompMethod(CMP_CORRELATION);
    break;
    
  case FEATURE_PATCHMIKO2:
  case FEATURE_STEERABLE:
  case FEATURE_SIFT:
  case FEATURE_GLOH:
  case FEATURE_MOMENTS:
  case FEATURE_DIFFINV:
  case FEATURE_COMPLEX:
  case FEATURE_SHAPECONTEXT:
  case FEATURE_SPINIMGS:
  case FEATURE_GRADIENTPCA:
  case FEATURE_SURF64:
  case FEATURE_SURF128: 
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    /* Mikolajczyk's Features */
    /*-=-=-=-=-=-=-=-=-=-=-=-=*/
    m_parMatching.params()->slotSelectCompMethod(CMP_EUKLIDEAN);
    break;
    
  default:
    cerr << "    Error in setMatchingParams(): "
         << "Unknown feature type (" << nFeatureType << ")!" << endl;
  }
}


//*---------------------------------------------------------*/
/*                     Export Functions                    */
/*---------------------------------------------------------*/

void Codebook::saveFeaturesASCII( string sFileName )
/*******************************************************************/
/* Save the extracted features to disk in plain ASCII format (to   */
/* be read in by Matlab). Each line contains one codebook entry.   */
/*******************************************************************/
{
  cout << "  Saving Features in ASCII format..." << endl;
  ofstream ofile( sFileName.c_str() );
  if( !ofile ) {
    cerr << "ERROR: Couldn't open file " << sFileName << "!" << endl;
    return;
  }
  
  for( int i=0; i<(int)m_vFeatures.size(); i++ )
    m_vFeatures[i].writeData( ofile );
  ofile.close();
  cout << "  done." << endl;
}


void Codebook::savePatchesASCII( string sFileName )
/*******************************************************************/
/* Save the extracted patches to disk in plain ASCII format (to be */
/* read in by Matlab). Each line contains one codebook entry.      */
/*******************************************************************/
{
  cout << "  Saving image patches in ASCII format..." << endl;

  ofstream ofile( sFileName.c_str() );
  if( !ofile ) {
    cerr << "ERROR: Couldn't open file " << sFileName << "!" << endl;
    return;
  }
  
  for( int i=0; i<(int)m_vImagePatches.size(); i++ ) {
    FeatureVector fvData = m_vImagePatches[i].getData();
    fvData.writeData( ofile );
  }
  ofile.close();

  cout << "  done." << endl;
}


void Codebook::saveClustersASCII( string sFileName )
  /*******************************************************************/
  /* Save the codebook entries to disk in plain ASCII format (to be  */
  /* read in by Matlab). Each line contains one codebook entry.      */
  /*******************************************************************/
{
  cout << "  Saving cluster centers in ASCII format..." << endl;

  ofstream ofile( sFileName.c_str() );
  if( !ofile ) {
    cerr << "ERROR: Couldn't open file " << sFileName << "!" << endl;
    return;
  }

  for( int i=0; i<(int)m_vClusters.size(); i++ )
    m_vClusters[i].writeData( ofile );
  ofile.close();

  cout << "  done." << endl;
}


void Codebook::saveClusterImages( string sFileName )
/*******************************************************************/
/* Save all cluster centers and all corresponding feature vectors  */
/* as images. The routine opens a file dialog to ask for a target  */
/* directory. All cluster centers and their assigned image patches */
/* are then written to this directory as images.                   */
/*******************************************************************/
{
  cout << "saveClusterImages called..." << endl;

  if ( m_vFeatures.empty() ) {
    cerr << "  You first have to execute the clustering." << endl;
    return;
  }

  if( !m_bKeepPatches ) {
    cerr << "  ERROR in Codebook::saveClusterImages(): "
         << "No patches available!" << endl;
    return;
  }

  /*--------------------------------*/
  /* Find the proper file extension */
  /*--------------------------------*/
  string sExtension;
  unsigned dotpos = sFileName.rfind( "." );
  if ( dotpos == string::npos )
    sExtension = "png";
  else {
    sExtension = sFileName.substr( dotpos+1, string::npos );
    sFileName.erase( dotpos, string::npos );
  }
  QString qsExtension( sExtension.c_str() );
  qsExtension = qsExtension.upper();

  /*-----------------------------*/
  /* Compute the cluster centers */
  /*-----------------------------*/
  computeClusterCenters();

  typedef vector<int> vecint;
  vector<vecint> vAllBelongingFV;
  vecint vInit;
  for( int i=0; i <(int)m_vClusters.size(); i++ )
    vAllBelongingFV.push_back( vInit );

  for( int i=0; i<(int)m_vClusterAssignment.size(); i++ )
    vAllBelongingFV[ m_vClusterAssignment[i] ].push_back( i );

  /*----------------------------------------*/
  /* Save qt images for the cluster centers */
  /*----------------------------------------*/
  for( int i=0; i <(int)m_vClusters.size(); i++ ) {
    QImage qimgTemp = m_vClusterPatches[i].getQtImage();

    QString qsNum = QString::number( i );
    if ( i < 100 ) qsNum.insert(0, '0');
    if ( i <  10 ) qsNum.insert(0, '0');
    string sNum( qsNum.latin1() );

    string  sSaveTo( sFileName + sNum + "." + sExtension );
    QString qsSaveTo( sSaveTo.c_str() );

    bool bSavedOK = qimgTemp.save( qsSaveTo, qsExtension.latin1() );
    if ( !bSavedOK ) {
      cerr << "  ERROR: Couldn't save image as '" << qsSaveTo << "'!" << endl;
      return;
    }
  }
  cout << "  Saved cluster centers in files " << sFileName << "." << endl;

  /*--------------------------------------*/
  /* Save qt images for the image patches */
  /*--------------------------------------*/
  sFileName.append( "_FVinCC" );
  for( int i=0; i<(int)vAllBelongingFV.size(); i++ ) {
    QString qsClNum = QString::number( i );
    if ( i < 100 ) qsClNum.insert( 0, '0');
    if ( i <  10 ) qsClNum.insert( 0, '0');
    string sClNum( qsClNum.latin1() );

    for( int j=0; j<(int)vAllBelongingFV[i].size(); j++ ) {
      QString qsFvNum = QString::number( j );
      if ( j < 100 ) qsFvNum.insert(0, '0');
      if ( j <  10 ) qsFvNum.insert(0, '0');
      string sFvNum( qsFvNum.latin1() );

      string  sSaveTo( sFileName + sClNum + "_" + sFvNum + "." + sExtension );
      QString qsSaveTo( sSaveTo.c_str() );

      QImage qimgTemp = m_vImagePatches[vAllBelongingFV[i][j]].getQtImage();

      bool bSavedOK = qimgTemp.save( qsSaveTo, qsExtension.latin1() );
      if ( !bSavedOK ) {
        cerr << "  ERROR: Couldn't save image as '" << qsSaveTo << "'!" 
             << endl;
        return;
      }
    }
  }

  cout << "  Saved corresponding image patches in files" << sFileName
       << "." << endl;
  cout << "done." << endl;
}


