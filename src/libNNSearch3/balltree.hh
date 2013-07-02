/*********************************************************************/
/*                                                                   */
/* FILE         balltree.hh                                          */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Implement an efficient NN search based on a balltree */
/*              structure. The search structure is built using the   */
/*              Anchors Hierarchy algorithm described in the fol-    */
/*              lowing paper:                                        */
/*                                                                   */
/*              Andrew W. Moore,                                     */
/*              The Anchors Hierarchy: Using the Triangle Inequality */
/*              to Survive High Dimensional Data.                    */
/*              Intern. Conf. on Uncertainty in AI, 2000.            */
/*                                                                   */
/* BEGIN        Tue Sep 06 2005                                      */
/* LAST CHANGE  Tue Sep 06 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef BALLTREE_HH
#define BALLTREE_HH


/****************/
/*   Includes   */
/****************/
#include <vector>
#include <list>
#include <string>
#include <cassert>
#include <featurevector.hh>
#include <clstep.hh>

/*******************/
/*   Definitions   */
/*******************/
const unsigned DEF_MAXNUMOWNED = 200;

/*************************/
/*   Class Definitions   */
/*************************/
typedef pair<float,unsigned> ValuePair;

/*===================================================================*/
/*                         Class BallTreeNode                        */
/*===================================================================*/
class BallTreeNode
{
public:
  BallTreeNode ( const FeatureVector &fvMean );
  BallTreeNode ( unsigned nPivot, const FeatureVector &fvMean );
  BallTreeNode ( const BallTreeNode &other );
  ~BallTreeNode();

  BallTreeNode& operator=( const BallTreeNode &other );

protected:
  void copyFrom( const BallTreeNode &other );
  
public:
  bool hasPivot   () const { return bHasPivot; }
  void addPoint   ( unsigned nPtIdx, float dDist );
  bool removePoint( unsigned nIdx );
  void removePivot();

  void addChild   ( float dDist, float dRad, BallTreeNode* pbnChild );
  void removeChild( BallTreeNode* pbnChild );

protected:
  void adaptRadius( const BallTreeNode *pbnChild, float dChildRadius );

public:
  void printNode    () const;

public:
  bool              bHasPivot;
  unsigned          nPivot;

  FeatureVector     fvMean;
  float             dRadius;

  //float             dDistChild1;
  //float             dDistChild2;
  //float             dRadChild1;
  //float             dRadChild2; 
  int               nNumChildren;
  vector<float>     vDistChild;
  vector<float>     vRadChild;

  unsigned          nNumOwned;
  //list<ValuePair> lPoints;
  list<unsigned>    vOwned;
  list<float>       vDist;
  
  BallTreeNode      *pbnParent;
  //BallTreeNode    *pbnChild1;
  //BallTreeNode    *pbnChild2;
  vector<BallTreeNode*> vpbnChild;

  unsigned           nVisited;
  float              dVisitMaxDist;
  float              dVisitMaxDist2;
};  


/*===================================================================*/
/*                           Class BallTree                          */
/*===================================================================*/
/* Define a class for efficient NN search in high dimensions*/
class BallTree
{
public:
	BallTree ();
	BallTree ( const vector<FeatureVector> &vData,
             unsigned nMaxNumOwned=DEF_MAXNUMOWNED );
	~BallTree();

public:
  /**********************/
  /*   Content Access   */
  /**********************/
  bool isValid() { return m_bIsValid; }

  void init( const vector<FeatureVector> &vData, bool bVerbose=false );

  void clear();
  void clearPoints();

public:
  /***************************************/
  /*   Construction: Anchors Hierarchy   */
  /***************************************/
  void build  ( const vector<FeatureVector> &vData, 
                unsigned nMaxNumOwned=DEF_MAXNUMOWNED, 
                bool bVerbose=false );
  void rebuild( unsigned nMaxNumOwned=DEF_MAXNUMOWNED, bool bVerbose=false );

protected:
  void createAnchorTree( list<unsigned> &vIdzs, 
                         unsigned nMaxNumOwned,
                         BallTreeNode* &pbnRoot );
  void createAnchor    ( vector<BallTreeNode*> &vpNodes );
  void clusterAnchors  ( const vector<BallTreeNode*> &vpNodes, 
                         BallTreeNode* &pbnRoot );
  
  float getMaxDist     ( const BallTreeNode *pbnNode, 
                         const FeatureVector &fvPoint ) const;
  float getDistQuantile( const BallTreeNode *pbnNode, 
                         const FeatureVector &fvPoint,
                         float dQuantile ) const;
  void  getAllDists    ( const BallTreeNode *pbnNode, 
                         const FeatureVector &fvPoint,
                         vector<float> &vDists ) const;
  int   getNumPointsIn ( const BallTreeNode *pbnNode ) const;

public:
  /****************************************/
  /*   Construction: from Cluster Trace   */
  /****************************************/

  void buildFromClusterTrace( const vector<FeatureVector> &vFeatures, 
                              const vector<ClStep>        &vTrace,
                              float dClusterSim, float dMaxLevel, 
                              int nNumLevels,
                              float dQuantile=1.0,
                              bool bVerbose=true );

protected:
  void groupFeatures        ( const vector<int> &vOldAssign, 
                              vector<ClStep>    &vTrace, 
                              unsigned nStartIdx, float dMinSim, 
                              vector<int>       &vNewAssign, 
                              vector<int>       &vRepresentatives, 
                              unsigned &nNumClusters, unsigned &nStopIdx, 
                              bool bVerbose=true );
  void computeClusterMeans  ( const vector<FeatureVector> &vFeatures,
                              const vector<int>           &vAssignment,
                              const vector<unsigned>      &vNumMembers,
                              unsigned                    nNumClusters,
                              vector<FeatureVector>       &vCenters,
                              vector<unsigned>            &vNewNumMembers );

public:
  /************************/
  /* Content Modification */
  /************************/
  void insertPoint ( const FeatureVector &fvPoint );

  void setPenalty  ( unsigned nIdx, float dPenalty );

  void makeActive  ( unsigned nIdx );
  void makeInactive( unsigned nIdx );
  void updatePoint ( unsigned nIdx, const FeatureVector &fvPoint,
                     float dPenalty );

  void mergePoints ( unsigned nIdx1, unsigned nIdx2, unsigned nNewIdx,
                     const FeatureVector &fvNewPoint, float dNewPenalty );

  void updateRadii ( float dQuantile=1.0 );

  void clearStats  () 
  { m_nCntChecked=0; m_nCntVisited=0; m_nCntDists=0; }
  void getStats    ( long &nCntChecked, long &nCntVisited, long &nCntDists )
  { nCntChecked=m_nCntChecked; nCntVisited=m_nCntVisited; 
    nCntDists=m_nCntDists; }

protected:
  BallTreeNode* insertPoint( BallTreeNode *pbnNode, unsigned nIdx, 
                             float dDist );
  void removePoint ( BallTreeNode *pbnNode, unsigned nIdx );
  void compactNode ( BallTreeNode *pbnNode );
  void clearNode   ( BallTreeNode *pbnNode );
  void removePoints( BallTreeNode *pbnNode );
  void removeNode  ( BallTreeNode *pbnNode );

  void updateRadii ( BallTreeNode *pbnNode, float dQuantile=1.0 );

public:
  /*****************/
  /*   NN Search   */
  /*****************/
  bool             findNN( const FeatureVector &fvQuery, float dMaxDist, 
                           unsigned &nResult, float &dDist );
  bool             findNN( unsigned nQueryIdx, float dMaxDist, 
                           unsigned &nResult, float &dDist );
  vector<unsigned> findNeighbors( const FeatureVector &fvQuery,
                                  float dMaxDist );

protected:
  bool findNN       ( BallTreeNode *pbnNode, 
                      const FeatureVector &fvQuery, 
                      float &dMaxDist, float &dMaxDist2,
                      unsigned &nResult,
                      BallTreeNode *pbnStopAt=NULL,
                      bool bVerbose=false );
  bool findNN       ( BallTreeNode *pbnNode, 
                      const FeatureVector &fvQuery,
                      unsigned nQuerIdx,
                      float &dMaxDist, float &dMaxDist2,
                      unsigned &nResult,
                      BallTreeNode *pbnStopAt=NULL,
                      bool bVerbose=false );
  void findNeighbors( BallTreeNode *pbnNode, 
                      const FeatureVector &fvQuery, 
                      float &dMaxDist, float &dMaxDist2,
                      vector<unsigned> &vResult );
  void takeAllChildPoints( BallTreeNode *pbnNode,
                           vector<unsigned> &vResult );

  void findNeighborsPenalty( BallTreeNode *pbnNode, 
                             const FeatureVector &fvQuery, 
                             float &dMaxDist, float &dMaxDist2,
                             vector<unsigned> &vResult );

public:
  /*************************/
  /*   Service Functions   */
  /*************************/
  void printStorageNode    ( unsigned nIdx );
  void printStorageNodeDist( unsigned nIdx, unsigned nQueryIdx );

  void printTraceToRoot    ( unsigned nIdx );
  void clearAllVisited     ();
  bool checkTree           ();

protected:
  void printNodeDist       ( const BallTreeNode *pbnNode,
                             unsigned nQueryIdx ) const;
  void printTraceToRoot    ( const BallTreeNode *pbnNode );
  void clearVisited        ( BallTreeNode *pbnNode );
  void visitTree           ( BallTreeNode *pbnNode );
  bool checkTree           ( BallTreeNode *pbnNode );
  bool checkReachable      ( BallTreeNode *pbnNode );
  
protected:
  vector<FeatureVector> m_vData;
  vector<float>         m_vPenalties;
  vector<BallTreeNode*> m_vpInNode;
  vector<bool>          m_vActive;

  BallTreeNode *m_pbnRoot;

  bool m_bIsValid;

  long m_nCntVisited;
  long m_nCntChecked;
  long m_nCntDists;
};

#endif // BALLTREE_HH
