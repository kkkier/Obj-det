/*********************************************************************/
/*                                                                   */
/* FILE         codebook.hh                                          */
/* AUTHORS      Bastian Leibe, Edgar Seemann                         */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Class for loading/saving/creating/matching codebooks.*/
/*                                                                   */
/* BEGIN        Tue Mar 15 2005                                      */
/* LAST CHANGE  Tue Mar 15 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef CODEBOOK_HH
#define CODEBOOK_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <qstring.h>
#include <qfile.h>
#include <qdir.h>

#include <featurevector.hh>
#include <cluster.hh>
#include <clstep.hh>
#include <featurecue.hh>
#include <qtclusterview.hh>
#include <container.hh>

#include "clusterparams.hh"
#include "matchingparams.hh"
#include "matchinginfo.hh"
#include "randomforest.hh"

/*******************/
/*   Definitions   */
/*******************/


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                          Class Codebook                           */
/*===================================================================*/

class Codebook: public Container
{
public:
  Codebook();
  ~Codebook();
  
public:
  /******************/
  /*   Parameters   */
  /******************/
  void setClusterParams ( ClusterParams parCluster );
  void setMatchingParams( MatchingParams parMatching );

public:
  /***********************/
  /*   Adding Features   */
  /***********************/
  void clear();
  void addFeatures( const vector<FeatureVector> &vFeatures );
  void addFeatures( const vector<FeatureVector> &vFeatures,
                    const vector<OpGrayImage>   &vImagePatches );
   void addFeatures( const vector<FeatureVector> &vFeatures,
                     const vector<OpGrayImage>   &vImagePatches,
		     const vector<int>		&vFeatureClass );
  void loadFeatureClass(const vector<int>& vFeatureClass);

  void normalizeFeatures( int nFeatureType );
  void normalizeClusters( int nFeatureType );

  void normalizeFeatures( vector<FeatureVector> &vFeatures, int nFeatureType );

public:
  /**********************/
  /*   Content Access   */
  /**********************/
  int                   getNumFeatures() const       
  { return (int)m_vFeatures.size(); }
  vector<FeatureVector> getFeatures() const    { return m_vFeatures; }
  
  int                   getNumClusters() const;
  vector<FeatureVector> getClusters() const;
  vector<int>           getPrototypes() const;
  vector<int>           getClusterAssignment() const;

  vector<OpGrayImage>   getImagePatches() const   {return m_vImagePatches; }
  vector<OpGrayImage>   getClusterPatches() const;
  OpGrayImage           getClusterPatch( int idx ) const;

  vector<ClStep>        getClusterTrace() const {return m_vClusterTrace; }

  void                  setClusterAssignment( const vector<int> &vAssignment )
  { assert( m_vFeatures.size()==vAssignment.size() );
  m_vClusterAssignment = vAssignment; 
  m_bClustersValid = false; m_bPrototypesValid = false; }
  void                  setClusterTrace     ( const vector<ClStep> &vTrace )
  { m_vClusterTrace = vTrace;
  m_bClustersValid = false; m_bPrototypesValid = false; }

public:
  /************/
  /* File I/O */
  /************/
  void loadCodebook  ( string sFileName, bool bKeepPatches=true, 
                       bool bVerbose=true );
  void loadCodebook  ( string sFileName, FeatureCue &fcCue, 
                       bool bKeepPatches=true, bool bVerbose=true );
  void appendCodebook( string sFileName, bool bKeepPatches=true,
                       bool bVerbose=true );
  void appendCodebook( string sFileName, FeatureCue &fcCue,
                       bool bKeepPatches=true, bool bVerbose=true );
  void saveCodebook  ( string sFileName, bool bKeepPatches=true,
                       bool bVerbose=true );
  void saveCodebook  ( string sFileName, FeatureCue &fcCue,
                       bool bKeepPatches=true, bool bVerbose=true );

  void computeClusterCenters   ();
  void computeClusterCenters_HF();
  void computeClusterPrototypes( vector<float> &mSimMatrix );
  
public:
  /************************/
  /* Matching to Codebook */
  /************************/
  /* routines in codebook-matching.cc */
  void matchToCodebook( const vector<FeatureVector> &vImgFeatures,
                        float dRejectionThresh,
                        int   nFeatureType,
                        vector<int>             &vNearestNeighbor,
                        vector<float>           &vNearestNeighborSim,
                        vector< vector<int> >   &vvAllNeighbors,
                        vector< vector<float> > &vvAllNeighborsSim ) const;
  void matchToCodebook( const vector<FeatureVector> &vImgFeatures,
                        float dRejectionThresh,
                        int   nFeatureType,
                        MatchingInfo &miMatchResult ) const;
  MatchingInfo matchToCodebook( const vector<FeatureVector> &vImgFeatures,
                                float dRejectionThresh,
                                int   nFeatureType ) const;
 
  void matchToCodebookCorr  ( const vector<FeatureVector> &vImgFeatures,
                              float dRejectionThresh,
                              vector<int>             &vNearestNeighbor,
                              vector<float>           &vNearestNeighborSim,
                              vector<vector<int> >   &vvAllNeighbors,
                              vector<vector<float> > &vvAllNeighborsSim) const;
  void matchToCodebookEuclid( const vector<FeatureVector> &vImgFeatures,
                              float dRejectionThresh,
                              float dDistFact,
                              vector<int>             &vNearestNeighbor,
                              vector<float>           &vNearestNeighborSim,
                              vector<vector<int> >   &vvAllNeighbors,
                              vector<vector<float> > &vvAllNeighborsSim) const;
 
  void matchToCodebookChamfer   ( const vector<FeatureVector> &vFeatures,
                                  float dRejectionThresh,
                                  vector<int>             &vNearestNeighbor,
                                  vector<float>           &vNearestNeighborSim,
                                  vector< vector<int> >   &vvAllNeighbors,
                                  vector< vector<float> > &vvAllNeighborsSim, 
                                  bool bSymDist=false ) const;
  void matchToCodebookChamferOri( const vector<FeatureVector> &vFeatures,
                                  float dRejectionThresh,
                                  vector<int>             &vNearestNeighbor,
                                  vector<float>           &vNearestNeighborSim,
                                  vector< vector<int> >   &vvAllNeighbors,
                                  vector< vector<float> > &vvAllNeighborsSim, 
                                  bool bSymDist=false ) const;

public:
  /************************/
  /* Clustering Functions */
  /************************/
  void clusterPatches( int nFeatureType );

  void recreateCodebookFromTrace( int nFeatureType );
  void recreateCodebookSimLevel ( float dSimLevel, int nFeatureType );
  void recreateCodebookCompRatio( float dCompRatio, int nFeatureType );

  void remove1PatchClusters();
  void remove1PatchClusters( vector<int> &vRemovedNums );
  void removeClusters      ( const vector<bool> &vIdzs );

protected:
  void clusterPatchesKMeans       ( const vector<FeatureVector> &vFeatures,
                                    int nNumClusters, float dEps, int nMaxIter,
                                    vector<int>    &vClusterAssignment,
                                    vector<ClStep> &vClusterTrace );
  void clusterPatchesPostAgglo    ( const vector<FeatureVector> &vFeatures,
                                    int   nFeatureType, 
                                    float dSimilarity,
                                    float dFeatureSimFact,
                                    vector<int>    &vClusterAssignment,
                                    vector<ClStep> &vClusterTrace );
  void clusterPatchesRNNCAgglo    ( const vector<FeatureVector> &vFeatures,
                                    int   nFeatureType, 
                                    float dSimilarity,
                                    float dFeatureSimFact,
                                    vector<int>    &vClusterAssignment,
                                    vector<ClStep> &vClusterTrace );
  void clusterPatchesFastRNNCAgglo( const vector<FeatureVector> &vFeatures,
                                    int   nFeatureType, 
                                    float dSimilarity,
                                    float dFeatureSimFact,
                                    vector<int>    &vClusterAssignment,
                                    vector<ClStep> &vClusterTrace );

  void computeSimilarityMatrix    ( const vector<FeatureVector> &vFeatures,
                                    int   nFeatureType,
                                    float dFeatureSimFact,
                                    vector<float>       &mSimMatrix );
  void clusterPatchesRandomForest(	const vector<OpGrayImage> &vPatches,
				 	int nTreeNumber,
					int nTreeDepth,
					int nWeekClassifier,
					int nMinSample,
					RandomForest &rfForest );
  
public:
  /*****************/
  /* Visualization */
  /*****************/
  void drawClusters( QtClusterView *qClassView );

public:
  /************************/
  /* Eigenspace Functions */
  /************************/
  
public:
  /***********************/
  /* Auxiliary Functions */
  /***********************/
  bool saveFeatures          ( string sFileName, bool bVerbose=true );
  bool loadFeatures          ( string sFileName, 
                               vector<FeatureVector> &vFeatures, 
                               bool bVerbose=true );  
  void savePatches           ( string sFileName, bool bVerbose=false );
  void loadPatches           ( string sFileName,
                               vector<OpGrayImage> &vImagePatches,
                               bool bVerbose=false );  
  void saveClusterAssignments( string sFileName, bool bVerbose=false );
  void loadClusterAssignments( string sFileName, 
                               vector<int> &vClusterAssignment,
                               bool bVerbose=false );
  void saveClusterTrace      ( string sFileName, bool bVerbose=false );
  void loadClusterTrace      ( string sFileName,
                               vector<ClStep> &vClusterTrace,
                               bool bVerbose=false );

  void savePrototypes        ( string sFilename, bool bVerbose=false );
  void loadPrototypes        ( string sFileName, bool bVerbose=false );

protected:
  void updateMatchingParams     ( int nFeatureType );

public:
  /********************/
  /* Export Functions */
  /********************/
  void saveFeaturesASCII   ( string sFileName );
  void savePatchesASCII    ( string sFileName );
  void saveClustersASCII   ( string sFileName );
  
  void saveClusterImages   ( string sFileName );

public:
  ClusterParams  m_parCluster;
  MatchingParams m_parMatching;

  vector<FeatureVector> m_vFeatures;
  vector<OpGrayImage>   m_vImagePatches;
  vector<int>           m_vFeatureClass;

  RandomForest 		m_rfRandomForest;  

  vector<FeatureVector> m_vClusters;
  vector<int>           m_vClusterAssignment;
  vector<ClStep>        m_vClusterTrace;
  
  vector<OpGrayImage>   m_vClusterPatches;
  vector<int>           m_vPrototypes;

  int m_nTreeNumber;
  int m_nTreeDepth;
  int m_nWeekClassifier;
  int m_nMinSample;

  bool m_bClustersValid;
  bool m_bPrototypesValid;
  bool m_bKeepPatches;

// protected:
//   unsigned _refcount;

// public:
//   class ObjectInUseException{};
//   friend class SharedCodebook;
};

typedef SharedContainer<Codebook> SharedCodebook;

#endif
