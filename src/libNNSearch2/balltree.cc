/*********************************************************************/
/*                                                                   */
/* FILE         balltree.cc                                          */
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

  
/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <algorithm>
#include <queue>

#include <clsimilarity.hh>

#include "balltree.hh"

/*******************/
/*   Definitions   */
/*******************/

/*===================================================================*/
/*                         Class BallTreeNode                        */
/*===================================================================*/

struct compPairIncr
{
  bool operator()( const ValuePair &x, 
                   const ValuePair &y )
  { return (x.first < y.first); }
};


struct compPairDecr
{
  bool operator()( const ValuePair &x, 
                   const ValuePair &y )
  { return (x.first > y.first); }
};


// void insertSortedDecr( list<ValuePair> &lPoints, 
//                        unsigned nIdx, float dDist )
// {
//   list<ValuePair>::iterator it;
//   for( it=lPoints.begin(); it!=lPoints.end(); it++ )
//     if( it->first < dDist ) {
//       lPoints.insert( it, ValuePair(dDist,nIdx) );
//       break;
//     }
// }


// void removePoint( const list<ValuePair> &lPoints, unsigned nIdx )
// {
//   list<ValuePair>::iterator it;
//   for( it=lPoints.begin(); it!=lPoints.end(); it++ )
//     if( it->second == nIdx ) {
//       lPoints.remove( it );
//       break;
//     }
// }


/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

BallTreeNode::BallTreeNode( const FeatureVector &fvNewMean )
{
  nPivot    = 0;
  bHasPivot = false;
  fvMean    = fvNewMean;
  dRadius   = 0.0;

  pbnParent = NULL;
//   pbnChild1 = NULL;
//   pbnChild2 = NULL;
  nNumChildren = 0;
  vDistChild.clear();
  vRadChild.clear();
  vpbnChild.clear();

  nNumOwned = 0;
  vOwned.clear();
  vDist.clear();
  nVisited  = 0;
  dVisitMaxDist  = 0.0;
  dVisitMaxDist2 = 0.0;
}


BallTreeNode::BallTreeNode( unsigned nNewPivot, 
                            const FeatureVector &fvNewMean )
{
  nPivot      = nNewPivot;
  bHasPivot   = true;
  fvMean      = fvNewMean;
  dRadius     = 0.0;
  
  //dDistChild1 = 0.0;
  //dDistChild2 = 0.0;
  //dRadChild1  = 0.0;
  //dRadChild2  = 0.0;
  nNumChildren = 0;
  vDistChild.clear();
  vRadChild.clear();

  nNumOwned   = 0;
  pbnParent   = NULL;
  //pbnChild1   = NULL;
  //pbnChild2   = NULL;
  vpbnChild.clear();

  vOwned.clear();
  vDist.clear();
  nVisited    = 0;
  dVisitMaxDist  = 0.0;
  dVisitMaxDist2 = 0.0;
}


BallTreeNode::BallTreeNode( const BallTreeNode &other )
  /* Copy constructor */
{
  copyFrom( other );
}


BallTreeNode::~BallTreeNode()
{
  //lPoints.clear();
  vOwned.clear();
  vDist.clear();
  
  vDistChild.clear();
  vRadChild.clear();

//   if( pbnChild1 )
//     delete pbnChild1;
//   pbnChild1 = NULL;
//   if( pbnChild2 )
//     delete pbnChild2;
//   pbnChild2 = NULL;
  for( int i=0; i<nNumChildren; i++ ) {
    if( vpbnChild[i] )
      delete vpbnChild[i];
    vpbnChild[i] = NULL;
  }
  vpbnChild.clear();
  nNumChildren = 0;
}


BallTreeNode& BallTreeNode::operator=( const BallTreeNode &other )
  /* Assignment operator */
{ 
  copyFrom( other );
  return *this;
}


void BallTreeNode::copyFrom( const BallTreeNode &other )
{
  nPivot      = other.nPivot;
  bHasPivot   = other.bHasPivot;
  fvMean      = other.fvMean;
  dRadius     = other.dRadius;

//   dDistChild1 = other.dDistChild1;
//   dDistChild2 = other.dDistChild2;
//   dRadChild1  = other.dRadChild1;
//   dRadChild2  = other.dRadChild2;
  nNumChildren= other.nNumChildren;
  vDistChild  = other.vDistChild;
  vRadChild   = other.vRadChild;

  nNumOwned   = other.nNumOwned;
  vOwned      = other.vOwned;
  vDist       = other.vDist;  
  pbnParent   = other.pbnParent;
//   pbnChild1   = other.pbnChild1;
//   pbnChild2   = other.pbnChild2;
  vpbnChild   = other.vpbnChild;

  nVisited    = other.nVisited;
  dVisitMaxDist  = other.dVisitMaxDist;
  dVisitMaxDist2 = other.dVisitMaxDist2;
}


/***********************************************************/
/*                      Owned Points                       */
/***********************************************************/

void BallTreeNode::addPoint( unsigned nPtIdx, float dDist )
{
  //cout << "        BallTreeNode::addPoint(" << nPtIdx << "," << dDist 
  //     << ") called... " << flush;
  /* insert the new point into the sorted list */
  if( nNumOwned== 0 ) {
    // start a new list
    vOwned.insert( vOwned.begin(), nPtIdx );
    vDist.insert ( vDist.begin(), dDist );
    nNumOwned++;

  } else {
    list<unsigned>::iterator itIdx;
    list<float>::iterator    itDist;
    bool bFound = false;
    for( itIdx=vOwned.begin(), itDist=vDist.begin(); 
         itIdx!=vOwned.end() && itDist!=vDist.end(); itIdx++, itDist++ )
      if( (*itDist) < dDist ) {
        vOwned.insert( itIdx, nPtIdx );
        vDist.insert ( itDist, dDist );
        nNumOwned++;
        bFound = true;
        break;
      }
    if( !bFound ) {
      // insert at the end 
      vOwned.insert( vOwned.end(), nPtIdx );
      vDist.insert ( vDist.end(), dDist );
      nNumOwned++;
      bFound = true;
    }
  }

  /* update the radius */
  if( dDist > dRadius )
    dRadius = dDist;
  //cout << "OK" << endl;
}


void BallTreeNode::adaptRadius( const BallTreeNode *pbnChild,
                                float dChildRadius )
{
  bool  bModified = false;

  /* determine if the function was called from one of the children */
//   if( pbnChild==pbnChild1 ) { // called from child 1
//     float dCheckRad = dDistChild1 + dChildRadius;
//     if( dCheckRad < dRadChild1 ) {
//       dRadChild1 = dCheckRad; 
//       if( dRadChild1 >= dRadChild2 ) {
//         /* modify radius */
//         dRadius   = dRadChild1;
//         bModified = true;
//       }
//     }

//   } else {                    // called from child 2
//     float dCheckRad = dDistChild2 + dChildRadius;
//     if( dCheckRad < dRadChild2 ) {
//       dRadChild2 = dCheckRad; 
//       if( dRadChild2 >= dRadChild1 ) {
//         /* modify radius */
//         dRadius   = dRadChild2;
//         bModified = true;
//       }
//     }
//   }

  /* determine if the function was called from one of the children */
  bool bFound = false;
  for( int i=0; i<nNumChildren && !bFound; i++ )
    if( pbnChild==vpbnChild[i] ) { // called from child i
      bFound = true;

      float dCheckRad = vDistChild[i] + dChildRadius;
      if( dCheckRad < vRadChild[i] ) {
        vRadChild[i] = dCheckRad;
        bool bGreatest = true;
        for( int j=0; j<nNumChildren && bGreatest; j++ )
          if( i!=j )
            if( vRadChild[i] < vRadChild[j] )
              bGreatest = false;
        if( bGreatest ) {
          /* modify radius */
          dRadius   = vRadChild[i];
          bModified = true;
        }
      }
    }    
  
  if( bModified && pbnParent )
    pbnParent->adaptRadius( this, dRadius );
}


bool BallTreeNode::removePoint( unsigned nIdx )
{
  /* remove the point from the sorted list (if it exists) */
  bool bFound = false;
  list<unsigned>::iterator itIdx;
  list<float>::iterator    itDist;
  for( itIdx=vOwned.begin(), itDist=vDist.begin(); 
       itIdx!=vOwned.end() && itDist != vDist.end(); itIdx++, itDist++ )
    if( (*itIdx) == nIdx ) {
      float dDistRemoved = *itDist;
      vOwned.erase( itIdx );
      vDist.erase ( itDist );
      nNumOwned--;
      bFound = true;

      /* update the radius */
      if( dDistRemoved >= dRadius ) {
        if( nNumOwned > 0 )
          dRadius = *(vDist.begin()); //vDist[0];
        else
          dRadius = 0.0;

        /* check if the parent's radius needs to be adapted, too */
        //if( pbnParent )
        //  pbnParent->adaptRadius( this, dRadius );
      }
      break;
    }

  /* check the pivot */
  if( !bFound && bHasPivot )
    if( nPivot == nIdx ) {
      removePivot();
      bFound = true;
    }
  assert( bFound );

  return bFound;
}


void BallTreeNode::removePivot()
{
  /* remove the pivot */
  nPivot    = 0;
  bHasPivot = false;
}


void BallTreeNode::addChild( float dDist, float dRad, BallTreeNode* pbnChild )
{
  nNumChildren++;
  vDistChild.push_back( dDist );
  vRadChild.push_back( dRad );
  vpbnChild.push_back( pbnChild );
}


void BallTreeNode::removeChild( BallTreeNode* pbnChild )
{
  vector<float> vNewDistChild;
  vector<float> vNewRadChild;
  vector<BallTreeNode*> vNewpbnChild;

  /* find which child shall be removed */
  bool bFound = false;
  for( int i=0; i<nNumChildren && !bFound; i++ )
    if( pbnChild==vpbnChild[i] ) { // found the child
      bFound = true;
    } else {
      vNewDistChild.push_back( vDistChild[i] );
      vNewRadChild.push_back( vRadChild[i] );
      vNewpbnChild.push_back( vpbnChild[i] );
    }

  if( bFound ) {
    nNumChildren--;
    vDistChild = vNewDistChild;
    vRadChild  = vNewRadChild;
    vpbnChild  = vNewpbnChild;

  } else {
    cout << "  Error in BallTreeNode::removeChild(" << pbnChild << "): " 
         << "Node isn't a child!" << endl;
    exit(0);
  }
}


void BallTreeNode::printNode() const
{
  cout << "    -------------------------------" << endl;
  cout << "    Content of node (" << this << "):" << endl
       << "      parent: (" << pbnParent << ")" << endl;
  if( bHasPivot )
    cout << "      pivot : " << nPivot << endl;
  else
    cout << "      pivot : none" << endl;
  cout << "      radius: " << dRadius << endl
       << "      #owned: " << nNumOwned << endl;
  list<unsigned>::const_iterator itIdx;
  list<float>::const_iterator    itDist;
  int i=0;
  for( itIdx=vOwned.begin(), itDist=vDist.begin(), i=0; 
       itIdx!=vOwned.end() && itDist!=vDist.end(); itIdx++, itDist++, i++ )
    cout << "        " << i+1 << ": " << *itIdx 
         << ", dist=" << *itDist << endl;
  cout << "      #children: " << nNumChildren << endl;
//   cout << "      child1: (" << pbnChild1 << ")" << endl
//        << "      child2: (" << pbnChild2 << ")" << endl;
  for( int i=0; i<nNumChildren; i++ )
    cout << "      child" << i+1 << ": (" << vpbnChild[i] << ")" << endl;    
  cout << "    -------------------------------" << endl;
}


/*===================================================================*/
/*                           Class BallTree                          */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

BallTree::BallTree()
  /* standard constructor */
{
  m_vData.clear();
  m_vPenalties.clear();
  m_vpInNode.clear();
  m_vActive.clear();
  m_pbnRoot = NULL;
  m_bIsValid = false;
  clearStats();
}


BallTree::BallTree( const vector<FeatureVector> &vData, unsigned nMaxNumOwned )
{
  build( vData, nMaxNumOwned );
  clearStats();
}


BallTree::~BallTree()
  /* standard destructor */
{
  m_vData.clear();
  m_vPenalties.clear();
  m_vpInNode.clear();
  m_vActive.clear();
  //if( m_pbnRoot )
  //  delete m_pbnRoot;
  //m_pbnRoot = NULL;
}


void BallTree::clear()
{
  /* Remove all nodes */
  if( m_pbnRoot )
    clearNode( m_pbnRoot );

  /* Free the data structures */
  m_vData.clear();
  m_vPenalties.clear();
  m_vpInNode.clear();
  m_vActive.clear();
  m_pbnRoot = NULL;
  m_bIsValid = false;
  clearStats();
}


void BallTree::clearPoints()
{
  /* Free the points but keep the tree */
  if( m_pbnRoot )
    removePoints( m_pbnRoot );
  
  /* Free the point data structures */
  m_vData.clear();
  m_vPenalties.clear();
  m_vpInNode.clear();
  m_vActive.clear();
  clearStats();
}


void BallTree::init( const vector<FeatureVector> &vData, bool bVerbose )
{
  /*----------------------------*/
  /* Initialize the data points */
  /*----------------------------*/
  m_vData = vData;
  
  /* initialize the penalties */
  vector<float> vTmp( m_vData.size(), 0.0 );
  m_vPenalties = vTmp;

  /* initialize the reverse index */
  vector<BallTreeNode*> vTmp2( m_vData.size(), NULL );
  m_vpInNode = vTmp2;

  /* initialize the 'active' flags */
  vector<bool> vTmp3( m_vData.size(), true );
  m_vActive = vTmp3;

  unsigned nNumPoints = m_vData.size();
  unsigned nNumDims   = m_vData[0].numDims();

  if( bVerbose )
    cout << "  BallTree::init() called." << endl
         << "    with " << nNumPoints << " points and " << nNumDims 
         << " dimensions." << endl;
}


/***********************************************************/
/*             Construction: Anchors Hierarchy             */
/***********************************************************/

void BallTree::build( const vector<FeatureVector> &vData, 
                      unsigned nMaxNumOwned, bool bVerbose )
  /*******************************************************************/
  /* Build a balltree structure from the given data using the An-    */
  /* chors Hierarchy algorithm by Moore.                             */
  /*******************************************************************/
{
  assert( !vData.empty() );

  /*----------------------*/
  /* Copy the data points */
  /*----------------------*/
  init( vData, bVerbose );

  unsigned nNumPoints = m_vData.size();

  /*------------------------*/
  /* Initialize the indizes */
  /*------------------------*/
  //vector<unsigned> vIdzs( nNumPoints );
  //for(unsigned i=0; i<nNumPoints; i++ )
  //  vIdzs[i] = i;
  list<unsigned> vIdzs( nNumPoints );
  vIdzs.clear();
  for(unsigned i=0; i<nNumPoints; i++ )
    vIdzs.push_back( i );

  /*-----------------------------*/
  /* Build the Anchors Hierarchy */
  /*-----------------------------*/
  createAnchorTree( vIdzs, nMaxNumOwned, m_pbnRoot );

  m_bIsValid = true;
  if( bVerbose ) {
    cout << "  done." << endl;
    cout << "    (root radius: " << m_pbnRoot->dRadius << ")" << endl;
  }
}


void BallTree::rebuild( unsigned nMaxNumOwned, bool bVerbose )
  /*******************************************************************/
  /* Rebuild the balltree structure from all active data points using*/
  /* the Anchors Hierarchy algorithm by Moore.                       */
  /*******************************************************************/
{
  assert( !m_vData.empty() );
  assert( m_bIsValid );

  /*--------------------*/
  /* Clear the old tree */
  /*--------------------*/
  delete m_pbnRoot;
  m_pbnRoot  = NULL;
  m_bIsValid = false;

  /*----------------------------*/
  /* Initialize the data points */
  /*----------------------------*/
  /* Initialize the reverse index */
  vector<BallTreeNode*> vTmp2( m_vData.size(), NULL );
  m_vpInNode = vTmp2;

  /* Count the active points */
  unsigned nNumActive = 0;
  for( unsigned i=0; i<m_vActive.size(); i++ )
    if( m_vActive[i] )
      nNumActive++;

  unsigned nNumPoints = m_vData.size(); //nNumActive;
  unsigned nNumDims   = m_vData[0].numDims();

  if( bVerbose ) {
    cout << "  BallTree::rebuild() called." << endl;
    cout << "    with " << nNumActive << " points, " 
         << nNumDims << " dimensions, and a maximal node size of " 
         << nMaxNumOwned<< "." << endl;
  }

  /*------------------------*/
  /* Initialize the indizes */
  /*------------------------*/
  list<unsigned> vIdzs;
  vIdzs.clear();
  for(unsigned i=0; i<nNumPoints; i++ )
    if( m_vActive[i] )
      vIdzs.push_back( i );

  /*-----------------------------*/
  /* Build the Anchors Hierarchy */
  /*-----------------------------*/
  createAnchorTree( vIdzs, nMaxNumOwned, m_pbnRoot );

  m_bIsValid = true;
  if( bVerbose ) {
    cout << "  done." << endl;
    cout << "    (root radius: " << m_pbnRoot->dRadius << ")" << endl;
  }
}


void BallTree::createAnchorTree( list<unsigned> &vIdzs, 
                                 unsigned nMaxNumOwned,
                                 BallTreeNode* &pbnRoot )
{
  //cout << "    BallTree::createAnchorTree() called..." << endl;
  //     << "      with " << vIdzs.size() << " points." << endl;
  /*========================*/
  /* Create sqrt(N) anchors */
  /*========================*/
  unsigned nNumPoints = vIdzs.size();            
  vector<BallTreeNode*> vpNodes;

  /*-------------------------------------------*/
  /* Start with a random point as first anchor */
  /*-------------------------------------------*/
  //cout << "      Starting with the first point (index: " << *(vIdzs.begin())
  //     << ")..." << endl;
  unsigned nIdxFirst = *(vIdzs.begin());
  BallTreeNode *pbnFirst = new BallTreeNode( nIdxFirst, 
                                             m_vData[nIdxFirst] );
  vIdzs.erase( vIdzs.begin() );
  m_vpInNode[nIdxFirst] = pbnFirst;

  /* assign all points to it */
  for( list<unsigned>::iterator it=vIdzs.begin(); it!=vIdzs.end(); 
       it++ ) {
    float dDist = sqrt( m_vData[nIdxFirst].compSSD(m_vData[*it]) );
    pbnFirst->addPoint( *it, dDist );
    m_vpInNode[*it] = pbnFirst;
  }
  vpNodes.push_back( pbnFirst );
  vIdzs.clear();

  /*--------------------------------*/
  /* Create sqrt(N)-1 other anchors */
  /*--------------------------------*/
  int nOthers = (int)floor(sqrt((double)nNumPoints)+0.5)-1;
  nOthers = min(nOthers, (int)floor(2.0*nNumPoints/((float)nMaxNumOwned)+0.5)-1 );
  //cout << "      Creating " << nOthers << " other anchors..." << endl;
  for( int i=0; i<nOthers; i++ )
    createAnchor( vpNodes );

  /* verify anchors */
  long nCount = 0;
  for(unsigned i=0; i<vpNodes.size(); i++ )
    nCount += vpNodes[i]->nNumOwned;
  //cout << "        (anchors contain together " << nCount << " points, plus "
  //     << vpNodes.size() << " pivots)." << endl;

  /*=========================================*/
  /* Build the hierarchy up from the anchors */
  /*=========================================*/
  //cout << "      Clustering the hierarchy up from the anchors..." << endl;
  clusterAnchors( vpNodes, pbnRoot );
  //cout << "        (root=" << pbnRoot << ")" << endl;
  //cout << "        (root radius: " << pbnRoot->dRadius << ")." << endl;

  /*==========================================*/
  /* Recursively rebuild the original anchors */
  /*==========================================*/
  //cout << "      Recursively rebuilding the original " << vpNodes.size() 
  //     << " anchors..." << endl;
  for(unsigned i=0; i<vpNodes.size(); i++ ) {
    //cout << "         Anchor " << i << " contains " 
    //     << vpNodes[i]->nNumOwned << " nodes." << endl;
    if( vpNodes[i]->nNumOwned > nMaxNumOwned ) {
      /* recursively rebuild the child nodes into a hierarchy */
      BallTreeNode *pbnThis = vpNodes[i];
      BallTreeNode *pbnSubRoot = NULL;
      list<unsigned> vOwnedIdzs = pbnThis->vOwned;
      if( pbnThis->hasPivot() ) {
        vOwnedIdzs.push_front( pbnThis->nPivot );
        pbnThis->removePivot();
      }
      createAnchorTree( vOwnedIdzs, nMaxNumOwned, pbnSubRoot );

      /* update the original anchor */
      //cout << "        Updating the original anchor with sub-root... " 
      //     << endl;
      //cout << "          (sub-root radius: " << pbnSubRoot->dRadius << ")"
      //     << endl;
      
      assert( pbnSubRoot );
      // The following fields stay the same:
      // pbnThis->pbnParent;
      // pbnThis->nPivot;
      // pbnThis->bHasPivot;
      pbnThis->fvMean      = pbnSubRoot->fvMean;
      pbnThis->dRadius     = pbnSubRoot->dRadius; 

//       pbnThis->dDistChild1 = pbnSubRoot->dDistChild1; 
//       pbnThis->dDistChild2 = pbnSubRoot->dDistChild2; 
//       pbnThis->dRadChild1  = pbnSubRoot->dRadChild1; 
//       pbnThis->dRadChild2  = pbnSubRoot->dRadChild2; 
      pbnThis->nNumChildren= pbnSubRoot->nNumChildren;
      pbnThis->vDistChild  = pbnSubRoot->vDistChild;
      pbnThis->vRadChild   = pbnSubRoot->vRadChild;

      pbnThis->nNumOwned   = pbnSubRoot->nNumOwned;
      pbnThis->vOwned      = pbnSubRoot->vOwned;
      pbnThis->vDist       = pbnSubRoot->vDist;
//       if( pbnSubRoot->pbnChild1 )
//         pbnSubRoot->pbnChild1->pbnParent = pbnThis;
//       if( pbnSubRoot->pbnChild2 )
//         pbnSubRoot->pbnChild2->pbnParent = pbnThis;
//       pbnThis->pbnChild1   = pbnSubRoot->pbnChild1;
//       pbnThis->pbnChild2   = pbnSubRoot->pbnChild2;
      vector<BallTreeNode*> vTmp( pbnThis->nNumChildren );
      pbnThis->vpbnChild = vTmp;
      for( int i=0; i<pbnSubRoot->nNumChildren; i++ ) {
        if( pbnSubRoot->vpbnChild[i] )
          pbnSubRoot->vpbnChild[i]->pbnParent = pbnThis;
        pbnThis->vpbnChild[i] = pbnSubRoot->vpbnChild[i];
      }
      
      /* save the pivots */
      assert( !pbnThis->hasPivot() );
      if( pbnThis->hasPivot() ) {
        FeatureVector &fvPivot = m_vData[pbnThis->nPivot];
        float dDist = sqrt(pbnThis->fvMean.compSSD( fvPivot ));
        pbnThis->addPoint( pbnThis->nPivot, dDist );
        m_vpInNode[pbnThis->nPivot] = pbnThis;
        pbnThis->removePivot();
      }        
      if( pbnSubRoot->hasPivot() ) {
        FeatureVector &fvPivot = m_vData[pbnSubRoot->nPivot];
        float dDist = sqrt(pbnThis->fvMean.compSSD( fvPivot ));
        pbnThis->addPoint( pbnSubRoot->nPivot, dDist );
        m_vpInNode[pbnSubRoot->nPivot] = pbnThis;
        pbnSubRoot->removePivot();
      }

      /* set the indizes for all owned points */
      if( pbnSubRoot->nNumOwned>0 )
        for(list<unsigned>::iterator it=pbnSubRoot->vOwned.begin();
            it!=pbnSubRoot->vOwned.end(); it++ )
          m_vpInNode[*it] = pbnThis;

      /* free the temporary subroot node */
      //cout << "        Deleting sub-root (" << pbnSubRoot << ")" << endl;
//       pbnSubRoot->pbnChild1 = NULL;
//       pbnSubRoot->pbnChild2 = NULL;
      for( int i=0; i<pbnSubRoot->nNumChildren; i++ )
        pbnSubRoot->vpbnChild[i] = NULL;
      delete pbnSubRoot;
    }
  }
  //cout << "    done." << endl;
}



void BallTree::createAnchor( vector<BallTreeNode*> &vpNodes )
{
  //cout << "        BallTree::createAnchor() called..." << endl;
  /*-------------------------------------------*/
  /* Search for the anchor with maximum radius */
  /*-------------------------------------------*/
  //cout << "          Searching for anchor with maximum radius... " << flush;
  unsigned nMaxIdx = 0;
  float    dMaxRad = 0.0;
  for(unsigned i=0; i<vpNodes.size(); i++ )
    if( vpNodes[i]->dRadius > dMaxRad ) {
      dMaxRad = vpNodes[i]->dRadius;
      nMaxIdx = i;
    }
  //cout << "=> " << nMaxIdx << endl;

  /*---------------------------------------------*/
  /* Create a new anchor from the farthest point */
  /*---------------------------------------------*/
  //cout << "            (anchor " << nMaxIdx << " owns " 
  //     << vpNodes[nMaxIdx]->vOwned.size() << " points)." << endl;
  unsigned nPivot = *(vpNodes[nMaxIdx]->vOwned.begin());
  //cout << "          Creating new anchor from point " << nPivot 
  //     << "..." << endl;
  vpNodes[nMaxIdx]->removePoint( nPivot );
  BallTreeNode *panNew = new BallTreeNode( nPivot, m_vData[nPivot] );
  m_vpInNode[nPivot] = panNew;

  /*----------------------------------------*/
  /* Steal points from the existing anchors */
  /*----------------------------------------*/
  //cout << "          Stealing points from existing anchors..." << endl;
  FeatureVector &fvNew = m_vData[nPivot];
  for(unsigned i=0; i<vpNodes.size(); i++ ) {
    /* Compute the (half) distance to the other anchor's pivot */
    assert( vpNodes[i]->hasPivot() );
    FeatureVector &fvOther = m_vData[vpNodes[i]->nPivot];
    float dDist2 = sqrt(fvNew.compSSD( fvOther ))/2.0;

    /* Steal the closer points */
    bool bSkipRest = false;
    vector<unsigned> vToRemove(0);
    list<unsigned>::iterator itIdx;
    list<float>::iterator    itDist;
    for( itIdx=vpNodes[i]->vOwned.begin(), itDist=vpNodes[i]->vDist.begin(); 
         ( itIdx!=vpNodes[i]->vOwned.end() && itDist!=vpNodes[i]->vDist.end() 
           && !bSkipRest ); itIdx++, itDist++ ) 
      if( (*itDist) < dDist2 )
        bSkipRest = true;
      else {
        /* compute the full distance */
        FeatureVector &fvPoint = m_vData[*itIdx];
        float dDistToNew = sqrt(fvNew.compSSD( fvPoint ));
        //if( dDistToNew < vpNodes[i]->vDist[j] ) {
        if( dDistToNew < (*itDist) ) {
          /* mark the point as stolen */
          vToRemove.push_back( *itIdx );
          panNew->addPoint( *itIdx, dDistToNew );
          /* set the reverse index */
          m_vpInNode[*itIdx] = panNew;
        }
      }
    
    /* remove the stolen points from the old node */
    for(unsigned j=0; j<vToRemove.size(); j++ )
      vpNodes[i]->removePoint( vToRemove[j] );
    //cout << "          removed " << vToRemove.size() << " points." << endl;
  }

  /*--------------------------------*/
  /* Add the new anchor to the list */
  /*--------------------------------*/
  //cout << "          Adding new anchor to the list..." << endl;
  vpNodes.push_back( panNew );
  //cout << "        done." << endl;
}


void BallTree::clusterAnchors( const vector<BallTreeNode*> &vpOrig, 
                               BallTreeNode* &pbnRoot )
{
  //cout << "        BallTree::clusterAnchors() called..." << endl;
  //  << "          (with " << vpOrig.size() << " anchors)" << endl;
  /************************************************************/
  /* Find the pair of nodes with the smallest combined radius */
  /* (approximating the radius of two nodes p and q by        */
  /*     rad(p+q) >= dist(p,q) + rad(p) + rad(q)              */
  /*  and storing the candidate pairs in a priority queue).   */
  /************************************************************/
  vector<BallTreeNode*> vpNodes = vpOrig;

  /*===============================*/
  /* Initialize the priority queue */
  /*===============================*/
  //cout << "          Initializing the priority queue..." << flush;
  unsigned nNumNodes = vpNodes.size();
  priority_queue<ClSimilarity, vector<ClSimilarity>, ClSimCompare> pqRadQueue;
  vector<bool> vValid( nNumNodes, true );
  for(unsigned i=0; i<nNumNodes; i++ )
    for(unsigned j=0; j<i; j++ ) {
      /* compute the combined radius of node i and j */
      assert( vpNodes[i]->hasPivot() );
      assert( vpNodes[j]->hasPivot() );
      FeatureVector &fvNode1 = m_vData[vpNodes[i]->nPivot];
      FeatureVector &fvNode2 = m_vData[vpNodes[j]->nPivot];
      float dRadius = ( sqrt(fvNode1.compSSD( fvNode2 )) +
                        vpNodes[i]->dRadius + vpNodes[j]->dRadius );
      ClSimilarity simCurrent( dRadius, i, j );
      pqRadQueue.push( simCurrent );
    }
  //cout << "OK" << endl;

  /*========================*/
  /* Perform the Clustering */
  /*========================*/
  //cout << "          Clustering... " << flush;
  bool bStop = false;
  while( !pqRadQueue.empty() && !bStop ) {
    /*------------------------------------------------------------*/
    /* Check if the current minimal radius is between valid nodes */
    /*------------------------------------------------------------*/
    //cout << "            Getting the new minimal radius from the queue..." 
    //     << flush;
    int   a   = pqRadQueue.top().m_A;
    int   b   = pqRadQueue.top().m_B;
    float rad = pqRadQueue.top().m_sim;
    while ( (!vValid[a]) || (!vValid[b]) ) {
        pqRadQueue.pop();
      if ( pqRadQueue.empty() ) {
        bStop = true;
        break;
      }
      a   = pqRadQueue.top().m_A;
      b   = pqRadQueue.top().m_B;
      rad = pqRadQueue.top().m_sim;
    }
    //cout << "OK" << endl;

    /*-----------------------------*/
    /* Merge the two closest nodes */
    /*-----------------------------*/
    if ( !bStop ) {
      //cout << "            Merging the two closest nodes " << a << " and "
      //     << b << "... " << endl;
      /* pop the top element of the queue */
      pqRadQueue.pop();
    
      /* create the new combined node */
      FeatureVector &fvNode1 = vpNodes[a]->fvMean;
      FeatureVector &fvNode2 = vpNodes[b]->fvMean;
      FeatureVector fvCenter = (fvNode1 + fvNode2) / 2.0;
      BallTreeNode *pbnNew = new BallTreeNode( fvCenter );

      /* get the distances to the children */
      float dDist1 = sqrt( fvCenter.compSSD(fvNode1) );
      float dDist2 = sqrt( fvCenter.compSSD(fvNode2) );
//       pbnNew->dDistChild1 = dDist1;
//       pbnNew->dDistChild2 = dDist2;

      /* compute its exact radius */
      float dRad1 = getMaxDist( vpNodes[a], fvCenter );
      float dRad2 = getMaxDist( vpNodes[b], fvCenter );
      float dNewRadius = max( dRad1, dRad2 );
//       pbnNew->dRadChild1 = dRad1;
//       pbnNew->dRadChild2 = dRad2;
      pbnNew->dRadius    = dNewRadius;

      /* add the two merged nodes as children */
      pbnNew->addChild( dDist1, dRad1, vpNodes[a] );
      pbnNew->addChild( dDist2, dRad2, vpNodes[b] );

      /* set it as parent of the two merged nodes */
//       pbnNew->pbnChild1 = vpNodes[a];
//       pbnNew->pbnChild2 = vpNodes[b];
      vpNodes[a]->pbnParent = pbnNew;
      vpNodes[b]->pbnParent = pbnNew;

      /* invalidate the two old nodes */
      vValid[a] = false;
      vValid[b] = false;

      /* append the new node */
      vpNodes.push_back( pbnNew );
      vValid.push_back( true );
      unsigned nNewIdx = vpNodes.size()-1;
      //cout << "            Creating new node with index " << nNewIdx << endl;

      /* compute the new radii to all other nodes */
      //cout << "            Computing the new radii to all other nodes..." 
      //     << flush;
      long nCount = 0;
      for(unsigned j=0; j<vpNodes.size()-1; j++ )
        if( vValid[j] ) {
          FeatureVector &fvNode = vpNodes[j]->fvMean;
          float dRadius = ( sqrt(fvCenter.compSSD( fvNode )) +
                            dNewRadius + vpNodes[j]->dRadius );
          ClSimilarity simNew( dRadius, nNewIdx, j );
          pqRadQueue.push( simNew );
          nCount++;
        }
      //cout << nCount << " entries added." << endl;
    }
  }
  //cout << "OK" << endl;

  /*=======================*/
  /* Set the new root node */
  /*=======================*/
  //cout << "          Setting the new root node... " << flush;
  unsigned nLastIdx = vpNodes.size()-1;
  assert( vValid[nLastIdx] );
  pbnRoot = vpNodes[nLastIdx];
  //cout << "(" << pbnRoot << ")" << endl;
}


float BallTree::getMaxDist( const BallTreeNode *pbnNode, 
                            const FeatureVector &fvPoint ) const
{
  float dMaxDist = 0.0;
  list<unsigned>::const_iterator itIdx;
  list<float>::const_iterator    itDist;
  for( itIdx=pbnNode->vOwned.begin(), itDist=pbnNode->vDist.begin();
       itIdx!=pbnNode->vOwned.end() && itDist!=pbnNode->vDist.end(); 
       itIdx++, itDist++ )
    if( m_vActive[*itIdx] ) {
      //FeatureVector &fvOwned = m_vData[*itIdx];
      float dDist = sqrt( fvPoint.compSSD( m_vData[*itIdx] ) );
      if( dDist > dMaxDist ) {
        dMaxDist = dDist;
      }
    }
  
  /* check pivot */
  if( pbnNode->hasPivot() && m_vActive[pbnNode->nPivot] )
    dMaxDist = max( dMaxDist, 
                    (float)sqrt( fvPoint.compSSD(m_vData[pbnNode->nPivot]) ) );

  /* recursively call children */
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    if( pbnNode->vpbnChild[i] )
      dMaxDist = max( dMaxDist, getMaxDist( pbnNode->vpbnChild[i], fvPoint ) );
      
  return dMaxDist;
}


float BallTree::getDistQuantile( const BallTreeNode *pbnNode, 
                                 const FeatureVector &fvPoint,
                                 float dQuantile ) const 
{
  assert( dQuantile>=0.0 && dQuantile<=1.0 );

  /* first get all distances to stored points */
  vector<float> vAllDists;
  getAllDists( pbnNode, fvPoint, vAllDists );

  /* sort the distance vector */
  sort( vAllDists.begin(), vAllDists.end() );
  
  /* look up the given quantile (and err on the large side) */
  int nIdx = (int)ceil( dQuantile*(vAllDists.size()-1) );
  if( nIdx>=0 )
    return vAllDists[nIdx];
  else 
    return 0.0;
}


void BallTree::getAllDists( const BallTreeNode *pbnNode, 
                            const FeatureVector &fvPoint,
                            vector<float> &vDists ) const
{
  list<unsigned>::const_iterator itIdx;
  list<float>::const_iterator    itDist;
  for( itIdx=pbnNode->vOwned.begin(); itIdx!=pbnNode->vOwned.end(); itIdx++ )
    if( m_vActive[*itIdx] ) {
      //FeatureVector &fvOwned = m_vData[*itIdx];
      vDists.push_back( sqrt( fvPoint.compSSD( m_vData[*itIdx] ) ) );
    }
  
  /* check pivot */
  if( pbnNode->hasPivot() && m_vActive[pbnNode->nPivot] )
    vDists.push_back( sqrt( fvPoint.compSSD(m_vData[pbnNode->nPivot]) ) );

  /* recursively call children */
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    if( pbnNode->vpbnChild[i] )
      getAllDists( pbnNode->vpbnChild[i], fvPoint, vDists );
}


int BallTree::getNumPointsIn( const BallTreeNode *pbnNode ) const
  /* return the number of points in and under the node.       */
{
  assert( pbnNode );
  
  /* get the number of owned points */
  int nCount = pbnNode->nNumOwned;

  /* check if the node contains a pivot element */
  if( pbnNode->bHasPivot )
    nCount++;

  /* add the number of points owned by children */
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    if( pbnNode->vpbnChild[i] )
      nCount += getNumPointsIn( pbnNode->vpbnChild[i] );
    
  return nCount;
}


void BallTree::updateRadii( float dQuantile )
/* Update the radii of all nodes in the tree */
{
  //cout << "  BallTree::updateRadii() called..." << endl;
  cout << endl
       << "    Updating radii of all nodes in the balltree... " << flush;
  if( m_pbnRoot )
    updateRadii( m_pbnRoot, dQuantile );
  cout << "    done. (root radius: " << m_pbnRoot->dRadius << ")" << endl;
}


void BallTree::updateRadii( BallTreeNode *pbnNode, float dQuantile )
/* Update the radii of this node and all subnodes in the tree */
{
  assert( pbnNode );

  FeatureVector &fvMean = pbnNode->fvMean;
//   float dRad1 = 0.0;
//   float dRad2 = 0.0;

   /* only update the radius if the node doesn't own points */
  if( pbnNode->nNumOwned==0 ) {
    float dRad = -1.0;
    if( dQuantile<1.0 )
      dRad = getDistQuantile( pbnNode, fvMean, dQuantile );
    else
      dRad = getMaxDist( pbnNode, fvMean );
    pbnNode->dRadius = dRad;
  }

//   /* only update the radius if the node doesn't own points */
//   if( pbnNode->nNumOwned==0 ) {
//     float dMaxRad = 0.0;
//     for( int i=0; i<pbnNode->nNumChildren; i++ )
//       if( pbnNode->vpbnChild[i] ) {
//         float dRad;
//         if( dQuantile<1.0 )
//           dRad = getDistQuantile( pbnNode->vpbnChild[i], fvMean, dQuantile );
//         else
//           dRad = getMaxDist( pbnNode->vpbnChild[i], fvMean );
//         if( dRad>dMaxRad )
//           dMaxRad = dRad;
//       }
//     pbnNode->dRadius = dMaxRad;
//   }

  for( int i=0; i<pbnNode->nNumChildren; i++ )
    if( pbnNode->vpbnChild[i] )
      updateRadii( pbnNode->vpbnChild[i], dQuantile );
}


/***********************************************************/
/*             Construction from Cluster Trace             */
/***********************************************************/

void BallTree::buildFromClusterTrace( const vector<FeatureVector> &vFeatures, 
                                      const vector<ClStep>        &vTrace,
                                      float dClusterSim, float dMaxLevel, 
                                      int nNumLevels,
                                      float dQuantile, bool bVerbose )
{
  /*===========================================*/
  /* Initialize the Balltree and Cluster Trace */
  /*===========================================*/
  clear();
  assert( !vFeatures.empty() );

  /*------------------------*/
  /* Clean up cluster trace */
  /*------------------------*/
  vector<ClStep> vNewTrace;
  if( bVerbose )
    cout << "    Cleaning up cluster trace... " << flush;
  for(unsigned i=0; i<vTrace.size(); i++ )
    if( !(vTrace[i].nIdx1<0 || vTrace[i].nIdx2<0 || vTrace[i].nNewIdx<0) )
      vNewTrace.push_back( vTrace[i] );

  unsigned nNumEntries = vNewTrace.size();
  if( bVerbose )
    cout << "reduced " << vTrace.size() << " entries to " << nNumEntries 
         << "." << endl;

  /*----------------*/
  /* Sort the trace */
  /*----------------*/
  /* DON'T sort the trace!!! */
  stable_sort( vNewTrace.begin(), vNewTrace.end(), compClStepAsc() );

  float dMinLevel = vNewTrace[0].dSim;
  if( dMaxLevel<0.0 )
    dMaxLevel = vNewTrace[nNumEntries-1].dSim;
  float dSimStep  = (dMaxLevel - dClusterSim)/(float)nNumLevels;
  if( bVerbose )
    cout << "    Cluster trace:" << endl
         << "      min     level: " << dMinLevel << endl
         << "      cluster level: " << dClusterSim << endl
         << "      max     level: " << dMaxLevel << endl
         << "    BallTree:" << endl
         << "      #tree levels : " << nNumLevels << endl
         << "      sim step size: " << dSimStep << endl;

  /*============================*/
  /* Build up the Cluster Level */
  /*============================*/
  /* Create the assignment vector */
  unsigned nNumFeatures = vFeatures.size();
  vector<int> vAssignment( nNumFeatures );
  for(unsigned i=0; i<nNumFeatures; i++ )
    vAssignment[i] = i;

  /*--------------------------------------------*/
  /* Group the features up to the cluster level */
  /*--------------------------------------------*/
  if( bVerbose )
    cout << "    Grouping the features up to the cluster level (" 
         << dClusterSim << ")... " << endl;
  vector<int> vClAssign;
  vector<int> vClRepres;
  unsigned    nNumClusters = 0;
  unsigned    nStartIdx    = 0;
  unsigned    nStopIdx     = 0;
  groupFeatures( vAssignment, vNewTrace, nStartIdx, dClusterSim,
                 vClAssign, vClRepres, nNumClusters, nStopIdx, false );

  /*-----------------------------*/
  /* Compute the cluster centers */
  /*-----------------------------*/
  if( bVerbose )
    cout << "      Computing the cluster centers... " << flush;
  vector<FeatureVector> vClusters;
  vector<unsigned>      vInit( nNumFeatures, 1 );
  vector<unsigned>      vNumClMembers;
  computeClusterMeans( vFeatures, vClAssign, vInit, nNumClusters, 
                       vClusters, vNumClMembers );

//   unsigned nNum0PCs = 0;
//   unsigned nNum1PCs = 0;
//   for(unsigned i=0; i<nNumClusters; i++ )
//     if( vNumClMembers[i] == 1 )
//       nNum1PCs++;
//     else if( vNumClMembers[i] == 0 )
//       nNum0PCs++;
//   if( bVerbose )
//     cout << "found " << nNum1PCs << " 1-patch clusters and " << nNum0PCs 
//          << " 0-patch clusters." << endl;

  /*----------------------------*/
  /* Initialize the data points */
  /*----------------------------*/
  init( vClusters, bVerbose );

  /*===============================*/
  /* Build up the First Tree Level */
  /*===============================*/
  if( bVerbose )
    cout << "    Building up the first tree level... " << endl;

  /*-----------------------------------------------*/
  /* Group the features up to the first tree level */
  /*-----------------------------------------------*/
  vector<int> vLeafAssign;
  vector<int> vLeafRepres;
  unsigned    nNumLeaves = 0;
  nStartIdx = nStopIdx;
  float dCurrentSim = dClusterSim + dSimStep;
  if( bVerbose )
    cout << "      Grouping clusters up to sim " << dCurrentSim << "..." 
         << endl;
//   groupFeatures( vClAssign, vNewTrace, nStartIdx, dCurrentSim,
//                  vLeafAssign, vLeafRepres, nNumLeaves, nStopIdx );
  groupFeatures( vAssignment, vNewTrace, 0, dCurrentSim,
                 vLeafAssign, vLeafRepres, nNumLeaves, nStopIdx, false );

  /*--------------------------*/
  /* Compute the node centers */
  /*--------------------------*/
  vector<int> vShortAssign( nNumClusters, -1 );
  for(unsigned i=0; i<nNumClusters; i++ ) {
    if( !(vClRepres[i]>=0 && vClRepres[i]<(int)vLeafAssign.size()) ) {
      cerr << "ERROR in BallTree::buildFromClusterTrace(): "
           << "vClRepres[" << i << "]==" << vClRepres[i] 
           << " > vLeafAssign.size()==" << vLeafAssign.size() << endl;
      exit(-1);
    }
    vShortAssign[i] = vLeafAssign[ vClRepres[i] ];
  }

  if( bVerbose )
    cout << "      Computing the node centers for " << nNumLeaves 
         << " clusters... " << endl;
  vector<FeatureVector> vMeans;
  vector<unsigned>      vNumMembers;
//   computeClusterMeans( vClusters, vShortAssign, vNumClMembers, nNumLeaves, 
//                        vMeans, vNumMembers );
  computeClusterMeans( vFeatures, vLeafAssign, vInit, nNumLeaves, 
                       vMeans, vNumMembers );

  /*--------------------------------*/
  /* Create the balltree leaf nodes */
  /*--------------------------------*/
  if( bVerbose )
    cout << "      Creating the leaf nodes... " << endl;
  vector<BallTreeNode*> vpbnNodes;
  for(unsigned i=0; i<nNumLeaves; i++ ) {
    /* create a new leaf node */
    BallTreeNode* pbnNode = new BallTreeNode( vMeans[i] );
    
    /* add all assigned codebook entries */
    for(unsigned j=0; j<nNumClusters; j++ )
      if( vShortAssign[j]==(int)i ) {
        float dDist = sqrt(vMeans[i].compSSD( vClusters[j] ));
        pbnNode->addPoint( j, dDist );
        m_vpInNode[j] = pbnNode;
      }

    /* if only one point was assigned, make it a pivot */
//     if( pbnNode->nNumOwned==1 ) {
//       pbnNode->bHasPivot = true;
//       pbnNode->nPivot = pbnNode->vOwned.front();
//       pbnNode->nNumOwned = 0;
//       pbnNode->vOwned.clear();
//       pbnNode->vDist.clear();
//     }

    /* add the node to the forest */
    vpbnNodes.push_back( pbnNode );
  }

  /*=================================*/
  /* Build up the Higher Tree Levels */
  /*=================================*/
  vector<int> vNewAssign = vLeafAssign;
  vector<int> vRepres    = vLeafRepres;
  unsigned    nNumGroups = nNumLeaves;
  for( int k=1; k<nNumLevels-1; k++ ) {
    if( bVerbose )
      cout << "    Building up tree level " << k+1 << "... " << endl;
    
    /*-----------------------------------------*/
    /* Group the features up to the next level */
    /*-----------------------------------------*/
    dCurrentSim = dCurrentSim + dSimStep;
    if( bVerbose )
      cout << "      Grouping clusters up to sim " << dCurrentSim << "..." 
           << endl;
    nStartIdx = nStopIdx;
    vector<int> vOldAssign = vNewAssign;
    vector<int> vNewRepres;
    unsigned nNewNumGroups;
//     groupFeatures( vOldAssign, vNewTrace, nStartIdx, dCurrentSim,
//                    vNewAssign, vNewRepres, nNewNumGroups, nStopIdx );
    groupFeatures( vAssignment, vNewTrace, 0, dCurrentSim,
                   vNewAssign, vNewRepres, nNewNumGroups, nStopIdx, false );

//     cout << "      vNewAssign.size() ==" << vNewAssign.size() << endl
//          << "      vRepres.size()    ==" << vRepres.size() << endl
//          << "      vNewRepres.size() ==" << vNewRepres.size() << endl
//          << "      vNumMembers.size()==" << vNumMembers.size() << endl
//          << "      vMeans.size()     ==" << vMeans.size() << endl
//          << "      vpbnNodes.size()  ==" << vpbnNodes.size() << endl
//          << "      nNumGroups        ==" << nNumGroups << endl
//          << "      nNewNumGroups     ==" << nNewNumGroups << endl;

    /*--------------------------*/
    /* Compute the node centers */
    /*--------------------------*/
    vector<int> vShortAssign( nNumGroups, -1 );
    for(unsigned i=0; i<nNumGroups; i++ )
      vShortAssign[i] = vNewAssign[ vRepres[i] ];
    
//     cout << "      vShortAssign.size()==" << vShortAssign.size() << endl;

    if( bVerbose )
      cout << "      Computing the node centers for " << nNewNumGroups 
           << " clusters... " << endl;
    vector<FeatureVector> vNewMeans;
    vector<unsigned>      vNewNumMembers;
//     computeClusterMeans( vMeans, vShortAssign, vNumMembers, nNewNumGroups, 
//                          vNewMeans, vNewNumMembers );
    computeClusterMeans( vFeatures, vNewAssign, vInit, nNewNumGroups, 
                         vNewMeans, vNewNumMembers );
    
    /* Compute the number of children */
    vector<unsigned> vNumChildren( nNewNumGroups, 0 );
    for(unsigned j=0; j<nNumGroups; j++ )
      vNumChildren[ vShortAssign[j] ]++;
        
//     unsigned nNum0Child = 0;
//     unsigned nNum1Child = 0;
//     unsigned nNum2Child = 0;
//     unsigned nNum3Child = 0;
//     unsigned nNum4Child = 0;
//     for(unsigned j=0; j<vNumChildren.size(); j++ )
//       if( vNumChildren[j]==0 )
//         nNum0Child++;
//       else if( vNumChildren[j]==1 )
//         nNum1Child++;
//       else if( vNumChildren[j]==2 )
//         nNum2Child++;
//       else if( vNumChildren[j]==3 )
//         nNum3Child++;
//       else
//         nNum4Child++;
//     cout << "      #nodes with 0 children: " << nNum0Child << endl
//          << "      #nodes with 1 child   : " << nNum1Child << endl
//          << "      #nodes with 2 children: " << nNum2Child << endl
//          << "      #nodes with 3 children: " << nNum3Child << endl
//          << "      #nodes with 4+children: " << nNum4Child << endl;

    assert( vpbnNodes.size()==nNumGroups );

    /*-------------------------------*/
    /* Create the new balltree nodes */
    /*-------------------------------*/
    if( bVerbose )
      cout << "      Creating the new balltree nodes... " << endl;
    vector<BallTreeNode*> vpbnNewNodes;
    for(unsigned i=0; i<nNewNumGroups; i++ ) {
      if( vNumChildren[i] > 1 ) { // several children => create a new node
        /* create a new node */
        BallTreeNode* pbnNode = new BallTreeNode( vNewMeans[i] );
      
        /* add all assigned children */
        vector<unsigned> vToAdd;
        for(unsigned j=0; j<nNumGroups; j++ )
          if( vShortAssign[j]==(int)i )
            if( vpbnNodes[j]->nNumChildren==0 && vpbnNodes[j]->nNumOwned==1 ) {
              /* just one point in child node => transfer to this node */
              int k = vpbnNodes[j]->vOwned.front();
              vToAdd.push_back( k );

              /* free the unused node */
              delete vpbnNodes[j];
              vpbnNodes[j] = NULL;

            } else {
              /* regular child => add below this node */
              float dDist = sqrt(vNewMeans[i].compSSD( vMeans[j] ));
              pbnNode->addChild( dDist, vpbnNodes[j]->dRadius, vpbnNodes[j] );
              vpbnNodes[j]->pbnParent = pbnNode;
            }

        /* add all points from removed children */
        for(unsigned k=0; k<vToAdd.size(); k++ ) {
          unsigned nIdx = vToAdd[k];
          float dDist = sqrt(vNewMeans[i].compSSD( m_vData[nIdx] ));
          //pbnNode->addPoint( nIdx, dDist );
          //m_vpInNode[nIdx] = pbnNode;
          m_vpInNode[nIdx] = insertPoint( pbnNode, nIdx, dDist );
        } 
        
        /* set the radius */
        if( dQuantile<1.0 )
          pbnNode->dRadius = getDistQuantile( pbnNode, vNewMeans[i], 
                                              dQuantile );
        else
          pbnNode->dRadius = getMaxDist( pbnNode, vNewMeans[i] );

        /* add the node to the forest */
        vpbnNewNodes.push_back( pbnNode );

      } else {                    // just one child => just propagate the node
        /* propagate the node to the next level */
        for(unsigned j=0; j<nNumGroups; j++ )
          if( vShortAssign[j]==(int)i )
            vpbnNewNodes.push_back( vpbnNodes[j] );
      }
    }

    /*-----------------------------------------*/
    /* Update the state for the next iteration */
    /*-----------------------------------------*/
    nNumGroups  = nNewNumGroups;
    vMeans      = vNewMeans;
    vRepres     = vNewRepres;
    vNumMembers = vNewNumMembers;
    vpbnNodes   = vpbnNewNodes;

    assert( vpbnNodes.size()==nNumGroups );
    assert( vMeans.size()==nNumGroups );
    assert( vNumMembers.size()==nNumGroups );
  } // end forall tree levels
 
  /*===========================*/
  /* Finish the Top Tree Level */
  /*===========================*/
  if( bVerbose )
    cout << "    Finishing the top tree level... " << endl;
  if( vpbnNodes.size()==1 )
    /* add the node as root */
    m_pbnRoot = vpbnNodes[0];

  else {
    /* compute the mean */
    FeatureVector fvRootMean( vFeatures.front().numDims() );
    unsigned nRootMembers = 0;
    for(unsigned i=0; i<nNumGroups; i++ ) {
      fvRootMean += vMeans[i]*(float)vNumMembers[i];
      nRootMembers += vNumMembers[i];
    }
    fvRootMean /= (float)nRootMembers;
    assert( nRootMembers==nNumFeatures );

    /* create a new node */
    BallTreeNode* pbnNode = new BallTreeNode( fvRootMean );
      
    /* add all assigned children */
    for(unsigned j=0; j<nNumGroups; j++ ) {
      float dDist = sqrt( fvRootMean.compSSD( vMeans[j] ) );
      pbnNode->addChild( dDist, vpbnNodes[j]->dRadius, vpbnNodes[j] );
      vpbnNodes[j]->pbnParent = pbnNode;
    }
    
    /* set the radius */
    if( dQuantile<1.0 )
      pbnNode->dRadius = getDistQuantile( pbnNode, fvRootMean, dQuantile );
    else
      pbnNode->dRadius = getMaxDist( pbnNode, fvRootMean );
    
    /* add the node as root */
    m_pbnRoot = pbnNode;    
  }
}


void BallTree::groupFeatures( const vector<int> &vOldAssign, 
                              vector<ClStep> &vTrace, 
                              unsigned nStartIdx, float dMinSim, 
                              vector<int> &vNewAssign, 
                              vector<int> &vRepresentatives, 
                              unsigned &nNumClusters, 
                              unsigned &nStopIdx, bool bVerbose )
{
  /*--------------------------------------------*/
  /* Group the features up to the desired level */
  /*--------------------------------------------*/
  unsigned nNumFeatures = vOldAssign.size();
  vector<int> vAssignment = vOldAssign;
  if( bVerbose )
    cout << "        Processing the trace... " << flush;
  unsigned nStep = nStartIdx;
  while( vTrace[nStep].dSim >= dMinSim ) {
    ClStep &csStep = vTrace[nStep];
    vAssignment[csStep.nIdx1] = csStep.nNewIdx;
    vAssignment[csStep.nIdx2] = csStep.nNewIdx;
    nStep++;
  }
  nStopIdx = nStep;
  if( bVerbose )
    cout << "for " << nStopIdx-nStartIdx << " entries." << endl;

  /*---------------------*/
  /* Resolve assignments */
  /*---------------------*/
  if( bVerbose )
    cout << "        Resolving assignments..." << endl;
  vNewAssign = vAssignment;
  for(unsigned i=0; i<nNumFeatures; i++ )
    if( vAssignment[i] != (int)i ) {
      int nNewIdx = vAssignment[i];
      while( vAssignment[nNewIdx] != nNewIdx )
        nNewIdx = vAssignment[nNewIdx];
      vNewAssign[i] = nNewIdx;
    }

  /*---------------------*/
  /* Rename the clusters */
  /*---------------------*/
  if( bVerbose )
    cout << "        Renaming the clusters... " << flush;
  vector<int> vNewClusterIdx( nNumFeatures, -1 );
  nNumClusters=0; 
  vRepresentatives.clear();
  for(unsigned i=0; i<nNumFeatures; i++ ) {
    
    /* check if the cluster has already been named */
    if( vNewClusterIdx[ vNewAssign[i] ] == -1 ) {
      vNewClusterIdx[ vNewAssign[i] ] = nNumClusters;
      nNumClusters++;
      vRepresentatives.push_back( vNewAssign[i] );
    }

    /* assign the new cluster index */
    vNewAssign[i] = vNewClusterIdx[ vNewAssign[i] ];
  }
  if( bVerbose )
    cout << "found " << nNumClusters << " clusters." << endl;

  assert( vNewAssign.size()==vOldAssign.size() );
  assert( vRepresentatives.size()==nNumClusters );
}


void BallTree::computeClusterMeans( const vector<FeatureVector> &vFeatures,
                                    const vector<int>         &vAssignment,
                                    const vector<unsigned>    &vNumMembers,
                                    unsigned nNumClusters,
                                    vector<FeatureVector>     &vCenters,
                                    vector<unsigned>          &vNewNumMembers )
{
  assert( !vFeatures.empty() );
  assert( vAssignment.size()==vFeatures.size() );
  assert( vAssignment.size()==vNumMembers.size() );

  FeatureVector         fvTmp( vFeatures.front().numDims() );
  vector<FeatureVector> vTmp ( nNumClusters, fvTmp );
  vector<unsigned>      vTmp2( nNumClusters, 0 );
  vCenters       = vTmp;
  vNewNumMembers = vTmp2;
  for(unsigned i=0; i<vAssignment.size(); i++ ) {
    int idx = vAssignment[i];
    if( idx<0 || idx>=(int)vCenters.size() )
      cerr << "ERROR in BallTree::computeClusterMeans(): "
           << "vAssignment[" << i << "]==" << idx << " > vCenters.size()=="
           << vCenters.size() << "!" << endl;

    vCenters[idx]       += vFeatures[i]*(float)vNumMembers[i];
    vNewNumMembers[idx] += vNumMembers[i];
  }

  for(unsigned i=0; i<nNumClusters; i++ ) {
    if( vNewNumMembers[i]<=0 )
      cerr << "ERROR in BallTree::computeClusterMeans(): "
           << "Cluster " << i << " has " << vNewNumMembers[i] << " members."
           << endl;
    vCenters[i] /= (float)vNewNumMembers[i];
  }
}


/***********************************************************/
/*                  Content Modification                   */
/***********************************************************/

void BallTree::insertPoint( const FeatureVector &fvPoint )
  /*******************************************************************/
  /* Insert a new point into the balltree without changing its       */
  /* structure.                                                      */
  /*******************************************************************/
{
  /* insert the point into the data structures */
  m_vData.push_back( fvPoint );
  m_vPenalties.push_back( 0.0 );
  m_vActive.push_back( true );

  /* find the best node to store it */
  BallTreeNode* pbnStorage = NULL;
  if( m_pbnRoot )
    pbnStorage = insertPoint( m_pbnRoot, m_vData.size()-1, 
                              sqrt(m_pbnRoot->fvMean.compSSD( fvPoint )) );

  m_vpInNode.push_back( pbnStorage );

}


void BallTree::setPenalty( unsigned nIdx, float dPenalty )
  /*******************************************************************/
  /* Set a penalty to a certain data point (which will be added to   */
  /* all distances computed from that point during NN search).       */
  /*******************************************************************/
{
  assert( nIdx < m_vData.size() );
  m_vPenalties[nIdx] = dPenalty;
}


void BallTree::makeActive( unsigned nIdx )
  /*******************************************************************/
  /* Set the given point to 'active' (meaning it will be used in the */
  /* NN search).                                                     */
  /*******************************************************************/
{
  assert( nIdx < m_vData.size() );
  m_vActive[nIdx] = true;
}


void BallTree::makeInactive( unsigned nIdx )
  /*******************************************************************/
  /* Set the given point to 'inactive' (meaning it won't be used in  */
  /* the NN search).                                                 */
  /*******************************************************************/
{
  assert( nIdx < m_vData.size() );
  m_vActive[nIdx] = false;
}


void BallTree::updatePoint( unsigned nIdx, const FeatureVector &fvPoint,
                            float dPenalty )
  /*******************************************************************/
  /* Update the point without updating the balltree.                 */
  /* CAUTION: This function may completely mess up the balltree if   */
  /* you don't know exactly what you're doing!                       */
  /*******************************************************************/
{
  assert( nIdx < m_vData.size() );

  m_vData[nIdx]      = fvPoint;
  m_vPenalties[nIdx] = dPenalty;
}


void BallTree::mergePoints( unsigned nIdx1, unsigned nIdx2, unsigned nNewIdx,
                            const FeatureVector &fvNewPoint, 
                            float dNewPenalty )
  /*******************************************************************/
  /* Merge the two given points and store the resulting point under  */
  /* the given new index.                                            */
  /* CAUTION: This function assumes that the two merged points were  */
  /* previously NNs, that the new coordinates are a weighted combi-  */
  /* nation of the two merged ones, and that the new index is either */
  /* nIdx1 or nIdx2 (i.e. the reorganization is done in-situ).       */
  /* Improper use may screw up the whole balltree structure!!!       */
  /*******************************************************************/
{
  //cout << "    BallTree::mergePoints(" << nIdx1 << "," << nIdx2 << ","
  //     << nNewIdx << ") called..." << endl;
  assert( nIdx1 < m_vData.size() );
  assert( nIdx2 < m_vData.size() );
  assert( (nNewIdx==nIdx1) || (nNewIdx==nIdx2) );
  assert( m_vpInNode[nIdx1] );
  assert( m_vpInNode[nIdx2] );

  /*--------------------------------*/
  /* Retrieve the two storage nodes */
  /*--------------------------------*/
  BallTreeNode *pbnNode1 = m_vpInNode[nIdx1];
  BallTreeNode *pbnNode2 = m_vpInNode[nIdx2];

  /*-----------------------*/
  /* Remove the old points */
  /*-----------------------*/
  pbnNode1->removePoint( nIdx1 );
  pbnNode2->removePoint( nIdx2 );
  m_vpInNode[nIdx1] = NULL;
  m_vpInNode[nIdx2] = NULL;

  /*----------------------*/
  /* Insert the new point */
  /*----------------------*/
  m_vData[nNewIdx] = fvNewPoint;
  float dDist1 = sqrt(fvNewPoint.compSSD( pbnNode1->fvMean ) );
  float dDist2 = sqrt(fvNewPoint.compSSD( pbnNode2->fvMean ) );
  if( dDist1 < dDist2 ) {
    /* insert into node 1 */
    pbnNode1->addPoint( nNewIdx, dDist1 );
    m_vpInNode[nNewIdx] = pbnNode1;

    /* check if node 2 is empty and should be deleted */
    compactNode( pbnNode2 );
  } else {
    /* insert into node 2 */
    pbnNode2->addPoint( nNewIdx, dDist2 );
    m_vpInNode[nNewIdx] = pbnNode2;

    /* check if node 1 is empty and should be deleted */
    compactNode( pbnNode1 );
  }

  /*----------------------*/
  /* Update the penalties */
  /*----------------------*/
  m_vPenalties[nIdx1]   = 0.0;
  m_vPenalties[nIdx2]   = 0.0;
  m_vPenalties[nNewIdx] = dNewPenalty;
  //cout << "    done." << endl;
}


BallTreeNode* BallTree::insertPoint( BallTreeNode *pbnNode, unsigned nIdx, float dDist )
  /*******************************************************************/
  /* Insert the point into the given node or one of its children.    */
  /*******************************************************************/
{
  assert( pbnNode );

  /* find the closest node among the children */
  float dMinDist = 999999999999999999999.9;
  int   nMinIdx = -1;
  for( int i=0; i<pbnNode->nNumChildren; i++ ) 
    if( pbnNode->vpbnChild[i] ) {
      float d = sqrt(pbnNode->vpbnChild[i]->fvMean.compSSD(m_vData[nIdx]));
      if( d < dMinDist ) {
        dMinDist = d; 
        nMinIdx = i;
      }
    }
  
  if( nMinIdx<0 ) {
    /* leaf node => insert the node here */
    pbnNode->addPoint( nIdx, dDist ); 
    return pbnNode;

  } else
    /* insert the point into the closest child node */
    return insertPoint( pbnNode->vpbnChild[nMinIdx], nIdx, dMinDist );
}


void BallTree::removePoint( BallTreeNode *pbnNode, unsigned nIdx )
  /*******************************************************************/
  /* Remove the point from the given balltree node.                  */
  /*******************************************************************/
{
  /* Remove the old point */
  bool bFound = pbnNode->removePoint( nIdx );
  if( !bFound && pbnNode->hasPivot() )
    if( pbnNode->nPivot==nIdx ) {
      pbnNode->removePivot();
      bFound = true;
    }
  assert( bFound );
}


void BallTree::compactNode( BallTreeNode *pbnNode )
  /*******************************************************************/
  /* Remove the (half-empty) node from the balltree and directly pin */
  /* its one child to the parent node.                               */
  /*******************************************************************/
{
  assert( pbnNode );

  /* Check if the function is applicable */
//   if( (pbnNode->nNumOwned>0) || (pbnNode->pbnChild1 && pbnNode->pbnChild2) )
  if( (pbnNode->nNumOwned>0) || (pbnNode->nNumChildren>=2) )
    return;

  /* Only compact the node if it is not the last one */
//   if( !pbnNode->pbnParent && !pbnNode->pbnChild1 && !pbnNode->pbnChild2 )
  if( !pbnNode->pbnParent && pbnNode->nNumChildren==0 )
    return;

  //cout << "BallTree::compactNode() called..." << endl;

  /*--------------------------------------------------*/
  /* At least one child is missing:                   */
  /* => connect the other directly to the parent node */
  /*--------------------------------------------------*/
  /* Find the intact child */
//   BallTreeNode *pbnChild = pbnNode->pbnChild1;
  BallTreeNode *pbnChild = pbnNode->vpbnChild[0];
//   if( !pbnChild )
//     pbnChild = pbnNode->pbnChild2;
  assert( pbnChild );


  /* Remove the node from its parent and directly pin the child there */
  BallTreeNode *pbnParent = pbnNode->pbnParent;
  if( !pbnParent )         // root node 
    m_pbnRoot = pbnChild;
  else {                   // regular node
//     if( pbnParent->pbnChild1==pbnNode )
//       pbnParent->pbnChild1 = pbnChild;
//     else if( pbnParent->pbnChild2==pbnNode )
//       pbnParent->pbnChild2 = pbnChild;
//     else {
//       cout << "  Error compacting node (" << pbnNode << "): " 
//            << "Node isn't its parent's child!" << endl;
//       exit(0);
//     }
    bool bFound = false;
    for( int i=0; i<pbnParent->nNumChildren && !bFound; i++ )
      if( pbnNode==pbnParent->vpbnChild[i] ) { // node is parent's child i
        bFound = true;
        pbnParent->vpbnChild[i] = pbnChild;
      }
    if( !bFound ) {
      cout << "  Error compacting node (" << pbnNode << "): " 
           << "Node isn't its parent's child!" << endl;
      exit(0);
    }
  }

  /* Update the child */
  if( pbnChild )
    pbnChild->pbnParent = pbnParent;

  /* (Special case when the root node is compacted) */
  BallTreeNode *pbnStorage = pbnParent;
  if( !pbnStorage )
    pbnStorage = pbnChild;

  /* If the node still has a pivot element, connect it to the parent */
  if( pbnNode->hasPivot() ) {
    float dDist = sqrt( m_vData[pbnNode->nPivot].compSSD( pbnStorage->fvMean));
    pbnStorage->addPoint( pbnNode->nPivot, dDist );
    m_vpInNode[pbnNode->nPivot] = pbnStorage;
  }

  /* Remove the children from this node */
//   pbnNode->pbnChild1 = NULL;
//   pbnNode->pbnChild2 = NULL;
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    pbnNode->vpbnChild[i] = NULL;

  /* delete the node */
  delete pbnNode;

  /* Proceed to the parent */ 
  if( pbnParent )
    compactNode( pbnParent );
}


void BallTree::clearNode( BallTreeNode *pbnNode )
  /*******************************************************************/
  /* Clear the node and all of its children.                         */
  /*******************************************************************/
{
  assert( pbnNode );
  
  /* remove all points from the node */
  pbnNode->nNumOwned = 0;
  pbnNode->vOwned.clear();
  pbnNode->vDist.clear();
  pbnNode->bHasPivot = false;

  /* proceed to all children and clear them */
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    clearNode( pbnNode->vpbnChild[i] );
  pbnNode->vDistChild.clear();
  pbnNode->vRadChild.clear();
  pbnNode->vpbnChild.clear();

  /* physically remove the node */
  removeNode( pbnNode );
}


void BallTree::removePoints( BallTreeNode *pbnNode )
  /*******************************************************************/
  /* Remove all points from the node and its children.               */
  /*******************************************************************/
{
  assert( pbnNode );

  /* remove all points from the node */
  pbnNode->nNumOwned = 0;
  pbnNode->vOwned.clear();
  pbnNode->vDist.clear();
  if( pbnNode->bHasPivot ) {
    pbnNode->bHasPivot = false;
    pbnNode->fvMean = m_vData[pbnNode->nPivot];
  }

  /* proceed to all children and clear the points from them */
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    if( pbnNode->vpbnChild[i] ) {
      removePoints( pbnNode->vpbnChild[i] );
      pbnNode->vRadChild[i] = 0.0;
    }
}


void BallTree::removeNode( BallTreeNode *pbnNode )
  /*******************************************************************/
  /* Remove the (empty) node from the balltree.                      */
  /*******************************************************************/
{
  assert( pbnNode );
  assert( pbnNode->nNumOwned==0 && !pbnNode->hasPivot() );
  //assert( !pbnNode->pbnChild1 && !pbnNode->pbnChild2 );
  assert( pbnNode->nNumChildren==0 );

  /* Remove the node from its parent */
  BallTreeNode *pbnParent = pbnNode->pbnParent;
  if( !pbnParent )         // root node
    m_pbnRoot = NULL;
  else                     // regular node
//     if( pbnParent->pbnChild1==pbnNode )
//       pbnParent->pbnChild1 = NULL;
//     else
//       pbnParent->pbnChild2 = NULL;
    pbnParent->removeChild( pbnNode );

  /* delete the node */
  delete pbnNode;
  pbnNode = NULL;
}


/***********************************************************/
/*                       NN Search                         */
/***********************************************************/

bool BallTree::findNN( const FeatureVector &fvQuery, float dMaxDist, 
                       unsigned &nResult, float &dDist )
  /*******************************************************************/
  /* Find the nearest neighbor for the given query point in the ball-*/
  /* tree, if there is one within a search radius of dMaxDist.       */
  /*******************************************************************/
{
  /* call the recursive search function */
  float dMaxDist2 = dMaxDist;
  dMaxDist = sqrt( dMaxDist2 );

  bool bSuccess = findNN( m_pbnRoot, fvQuery, dMaxDist, dMaxDist2, nResult );
  dDist = dMaxDist2;

  return bSuccess;
}

 
bool BallTree::findNN( unsigned nQueryIdx, float dMaxDist, 
                       unsigned &nResult, float &dDist )
  /*******************************************************************/
  /* Find the nearest neighbor for the given query point in the ball-*/
  /* tree, if there is one within a search radius of dMaxDist.       */
  /* Calling variant for the case that the query point is already in */
  /* the tree (which allows some additional speed-ups).              */
  /*******************************************************************/
{
  //cout << "    BallTree::findNN(" << nQueryIdx << "," << dMaxDist 
  //     << ") called..." << endl;

  assert( nQueryIdx < m_vpInNode.size() );
  assert( m_vpInNode[nQueryIdx] != NULL );

  /* set visit flags to zero */
  //clearAllVisited();

  float dMaxDist2 = dMaxDist - m_vPenalties[nQueryIdx];
  dMaxDist = sqrt( dMaxDist2 );

  /* First check the node in which this point is stored */
  //cout << "      Checking storage node first... " << endl;//flush;
  FeatureVector &fvQuery = m_vData[nQueryIdx];
  bool bSuccess = false;
  bSuccess = findNN( m_vpInNode[nQueryIdx], fvQuery, nQueryIdx, 
                     dMaxDist, dMaxDist2,
                     nResult, NULL, false );

//   if( bSuccess )
//     cout << "      Found NN candidate (" << nResult << "," << dMaxDist2 << 
//       "," << m_vPenalties[nResult] << ")." 
//          << endl;
//   else
//     cout << "      Found no NN in storage node." << endl;

  /* call the recursive search function for the remaining tree */
  bSuccess = findNN( m_pbnRoot, fvQuery, nQueryIdx,
                     dMaxDist, dMaxDist2, 
                     nResult, m_vpInNode[nQueryIdx], false ) || bSuccess;
  
  dDist = dMaxDist2 + m_vPenalties[nQueryIdx]; // + m_vPenalties[nResult];

//   cout << "    done. " << flush;
//   if( bSuccess )
//     cout << "Found NN (" << nResult << "," << dDist << ","
//          << m_vPenalties[nQueryIdx] + m_vPenalties[nResult] << ")." << endl;
//   else
//     cout << "Found no NN in search volume." << endl;

  return bSuccess;
}

 
bool BallTree::findNN( BallTreeNode *pbnNode, 
                       const FeatureVector &fvQuery,
                       float &dMaxDist, float &dMaxDist2,
                       unsigned &nResult,
                       BallTreeNode *pbnStopAt,
                       bool bVerbose )
  /*******************************************************************/
  /* Find the nearest neighbor for the given query point in the ball-*/
  /* tree, if there is one within a search radius of dMaxDist.       */
  /*******************************************************************/
{
  if( bVerbose )
    cout << "      BallTree::findNN(" << pbnNode << ") called recursively." 
         << endl;
  /*--------------------------*/
  /* Check the stop condition */
  /*--------------------------*/
  if( pbnNode == pbnStopAt )
    return false;

  /*--------------------------------------------------------*/
  /* Check if the current node intersects the search volume */
  /*--------------------------------------------------------*/
  if( bVerbose )
    cout << "        Checking if the node intersects the volume... " << endl;
  float bSuccess = false;
  float dDistCenter2;
  if( pbnNode->hasPivot() ) // compare with pivot element
    dDistCenter2 = fvQuery.compSSD(m_vData[pbnNode->nPivot] );
  else                      // compare with node mean
    dDistCenter2 = fvQuery.compSSD( pbnNode->fvMean );

  //float dDistCenter = sqrt(dDistCenter2);
  if( bVerbose ) {
    cout << "            dist2 =" << dDistCenter2 << endl;
    cout << "            radius=" << pbnNode->dRadius << endl;
  }
  float d = (dMaxDist+pbnNode->dRadius);
  //if( dDistCenter - pbnNode->dRadius > dMaxDist )
  if( dDistCenter2 > d*d )
    return false;

  if( bVerbose )
      cout << "        OK." << endl;

  if( bVerbose ) {
    /* visit the node */
    pbnNode->nVisited++;
    pbnNode->dVisitMaxDist = dMaxDist;
    pbnNode->dVisitMaxDist2= dMaxDist2;  
  }
  
  /*-----------------------------------------------*/
  /* If it is an anchor node, also check the pivot */
  /*-----------------------------------------------*/
  if( pbnNode->hasPivot() && m_vActive[pbnNode->nPivot] ) {
    /* check if the pivot needs to be checked */
    float dPenalty = m_vPenalties[pbnNode->nPivot];
    if( dDistCenter2 + dPenalty < dMaxDist2 ) {
      /* found a new NN => update the search radius */
      nResult   = pbnNode->nPivot;
      dMaxDist2 = dDistCenter2 + dPenalty;
      dMaxDist  = sqrt( dMaxDist2 );
      bSuccess = true;
    }
  }

  /*------------------------------------------------*/
  /* If the node owns points, check their distances */
  /*------------------------------------------------*/
  bool bSkipRest = false;
  list<unsigned>::iterator itIdx;
  list<float>::iterator    itDist;
  for( itIdx=pbnNode->vOwned.begin(), itDist=pbnNode->vDist.begin(); 
       ( itIdx!=pbnNode->vOwned.end() && itDist!=pbnNode->vDist.end() && 
         !bSkipRest ); itIdx++, itDist++ )
    if( m_vActive[*itIdx] ) {
//       /* check if the point needs to be checked */
//       if( dDistCenter - (*itDist) > dMaxDist )
//         bSkipRest = true;
      /* check if the point needs to be checked */
      float d2 = ((*itDist) + dMaxDist)*((*itDist) + dMaxDist);
      if( dDistCenter2 > d2 )
        bSkipRest = true;
      else {
        FeatureVector &fvPoint = m_vData[*itIdx];
        float dDistPoint2 = fvQuery.compSSD( fvPoint );
        float dPenalty = m_vPenalties[*itIdx];
        if( dDistPoint2 + dPenalty < dMaxDist2 ) {
          /* found a new NN => update the search radius */
          nResult   = *itIdx;
          dMaxDist2 = dDistPoint2 + dPenalty;
          dMaxDist  = sqrt( dMaxDist2 );
          bSuccess = true;
        }
      }
    }
  
  /*--------------------------*/
  /* Check the children nodes */
  /*--------------------------*/
//   if( pbnNode->pbnChild1 )
//     bSuccess = findNN( pbnNode->pbnChild1, fvQuery, 
//                        dMaxDist, dMaxDist2, nResult, pbnStopAt ) || bSuccess;
//   if( pbnNode->pbnChild2 )
//     bSuccess = findNN( pbnNode->pbnChild2, fvQuery, 
//                        dMaxDist, dMaxDist2, nResult, pbnStopAt ) || bSuccess;
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    if( pbnNode->vpbnChild[i] )
      bSuccess = findNN( pbnNode->vpbnChild[i], fvQuery, 
                         dMaxDist, dMaxDist2, nResult, pbnStopAt ) || bSuccess;

  return bSuccess;
}


bool BallTree::findNN( BallTreeNode *pbnNode, 
                       const FeatureVector &fvQuery,
                       unsigned nQueryIdx,
                       float &dMaxDist, float &dMaxDist2,
                       unsigned &nResult,
                       BallTreeNode *pbnStopAt,
                       bool bVerbose )
  /*******************************************************************/
  /* Find the nearest neighbor for the given query point in the ball-*/
  /* tree, if there is one within a search radius of dMaxDist.       */
  /*******************************************************************/
{
  if( bVerbose )
    cout << "      BallTree::findNN(" << pbnNode << ") called recursively." 
         << endl;
  /*--------------------------*/
  /* Check the stop condition */
  /*--------------------------*/
  if( pbnNode == pbnStopAt )
    return false;

  /*--------------------------------------------------------*/
  /* Check if the current node intersects the search volume */
  /*--------------------------------------------------------*/
  if( bVerbose )
    cout << "        Checking if the node intersects the volume... " << endl;
  float bSuccess = false;
  float dDistCenter2;
  if( pbnNode->hasPivot() ) // compare with pivot element
    dDistCenter2 = fvQuery.compSSD(m_vData[pbnNode->nPivot] );
  else                      // compare with node mean
    dDistCenter2 = fvQuery.compSSD( pbnNode->fvMean );

  //float dDistCenter = sqrt(dDistCenter2);
  if( bVerbose ) {
    cout << "            dist2 =" << dDistCenter2 << endl;
    cout << "            radius=" << pbnNode->dRadius << endl;
  }

  float d = (dMaxDist+pbnNode->dRadius);
  //if( dDistCenter - pbnNode->dRadius > dMaxDist )
  if( dDistCenter2 > d*d )
    return false;

  if( bVerbose )
      cout << "        OK." << endl;

  if( bVerbose ) {
    /* check if already visited */
    if( pbnNode->nVisited>0 ) {
      cout << "    Error in findNN(" << nQueryIdx << "," << dMaxDist << ","
           << dMaxDist2 << "): node already visited!" << endl;
      printNodeDist( pbnNode, nQueryIdx );
      cout << "    Tracing node (" << pbnNode << ") to the root: " << endl;
      printTraceToRoot( pbnNode );
      cout << "    -------------------------------" << endl;
      exit(0);
    }
    
    /* visit the node */
    pbnNode->nVisited++;
    pbnNode->dVisitMaxDist = dMaxDist;
    pbnNode->dVisitMaxDist2= dMaxDist2; 
    //cout << "        Visiting node (" << pbnNode << ")..." << endl;
  }

  /*-----------------------------------------------*/
  /* If it is an anchor node, also check the pivot */
  /*-----------------------------------------------*/
  if( pbnNode->hasPivot() && (pbnNode->nPivot!=nQueryIdx) && 
      m_vActive[pbnNode->nPivot] ) {
    /* check if the pivot needs to be checked */
    float dPenalty = m_vPenalties[pbnNode->nPivot];
    if( dDistCenter2 + dPenalty < dMaxDist2 ) {
      /* found a new NN => update the search radius */
      nResult   = pbnNode->nPivot;
      dMaxDist2 = dDistCenter2 + dPenalty;
      dMaxDist  = sqrt( dMaxDist2 );
      bSuccess = true;
    }
  }

  /*------------------------------------------------*/
  /* If the node owns points, check their distances */
  /*------------------------------------------------*/
  bool bSkipRest = false;
  list<unsigned>::iterator itIdx;
  list<float>::iterator    itDist;
  for( itIdx=pbnNode->vOwned.begin(), itDist=pbnNode->vDist.begin(); 
       ( itIdx!=pbnNode->vOwned.end() && itDist!=pbnNode->vDist.end() && 
         !bSkipRest ); itIdx++, itDist++ )
    if( (*itIdx != nQueryIdx) && m_vActive[*itIdx] ) {
//       /* check if the point needs to be checked */
//       if( dDistCenter - (*itDist) > dMaxDist )
//         bSkipRest = true;
      /* check if the point needs to be checked */
      float d2 = ((*itDist) + dMaxDist)*((*itDist) + dMaxDist);
      if( dDistCenter2 > d2 )
        bSkipRest = true;
      else {
        FeatureVector &fvPoint = m_vData[*itIdx];
        float dDistPoint2 = fvQuery.compSSD( fvPoint );
        float dPenalty = m_vPenalties[*itIdx];
        if( dDistPoint2 + dPenalty < dMaxDist2 ) {
          /* found a new NN => update the search radius */
          nResult   = *itIdx;
          dMaxDist2 = dDistPoint2 + dPenalty;
          dMaxDist  = sqrt( dMaxDist2 );
          bSuccess = true;
        }
      }
    }
  
  /*--------------------------*/
  /* Check the children nodes */
  /*--------------------------*/
//   if( pbnNode->pbnChild1 )
//     bSuccess = findNN( pbnNode->pbnChild1, fvQuery, nQueryIdx,
//                        dMaxDist, dMaxDist2, nResult, pbnStopAt ) || bSuccess;
//   if( pbnNode->pbnChild2 )
//     bSuccess = findNN( pbnNode->pbnChild2, fvQuery, nQueryIdx,
//                        dMaxDist, dMaxDist2, nResult, pbnStopAt ) || bSuccess;
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    if( pbnNode->vpbnChild[i] )
      bSuccess = findNN( pbnNode->vpbnChild[i], fvQuery, nQueryIdx,
                         dMaxDist, dMaxDist2, nResult, pbnStopAt ) || bSuccess;

  return bSuccess;
}


vector<unsigned> BallTree::findNeighbors( const FeatureVector &fvQuery,
                                          float dMaxDist )
  /*******************************************************************/
  /* Find all neighbors for the given query point in the balltree    */
  /* that are within a search radius of dMaxDist.                    */
  /*******************************************************************/
{
  vector<unsigned> vResult;
  vResult.reserve( 20 );

  /* call the recursive search function */
  float dMaxDist2 = dMaxDist;
  dMaxDist = sqrt( dMaxDist2 );

  /*-------------------*/
  /* Start at the root */
  /*-------------------*/
  if( m_pbnRoot->bHasPivot || m_pbnRoot->nNumOwned>0 )
    /* If the root owns points itself, start checking there */
    findNeighbors( m_pbnRoot, fvQuery, dMaxDist, dMaxDist2, vResult );
  else 
    /* Else, start directly at the root's children nodes */
    for( int i=0; i<m_pbnRoot->nNumChildren; i++ )
      if( m_pbnRoot->vpbnChild[i] )
        findNeighbors( m_pbnRoot->vpbnChild[i], fvQuery, 
                       dMaxDist, dMaxDist2, vResult );

  return vResult;
}

 
void BallTree::findNeighbors( BallTreeNode *pbnNode, 
                              const FeatureVector &fvQuery,
                              float &dMaxDist, float &dMaxDist2,
                              vector<unsigned> &vResult )
  /*******************************************************************/
  /* Find all neighbors for the given query point in the balltree    */
  /* that are within a search radius of dMaxDist.                    */
  /*******************************************************************/
{
  assert( pbnNode );
  m_nCntChecked++;

  /*--------------------------------------------------------*/
  /* Check if the current node intersects the search volume */
  /*--------------------------------------------------------*/
  float dDistCenter2;
  if( pbnNode->hasPivot() ) // compare with pivot element
    dDistCenter2 = fvQuery.compSSD(m_vData[pbnNode->nPivot] );
  else                      // compare with node mean
    dDistCenter2 = fvQuery.compSSD( pbnNode->fvMean );
  
  //float dDistCenter = sqrt(dDistCenter2);
  //if( dDistCenter - pbnNode->dRadius > dMaxDist )
  float dDistCenter = -1.0;
  float d = (dMaxDist+pbnNode->dRadius);
  if( dDistCenter2 > d*d )
    return;
//   else if( dDistCenter + pbnNode->dRadius <= dMaxDist )
//     takeAllChildPoints( pbnNode, vResult );
  m_nCntVisited++;

  /*-----------------------------------------------*/
  /* If it is an anchor node, also check the pivot */
  /*-----------------------------------------------*/
  if( pbnNode->hasPivot() && m_vActive[pbnNode->nPivot] ) {
    /* check if the pivot needs to be checked */
    if( dDistCenter2 <= dMaxDist2 )
      /* add the pivot to the neighbor list */
      vResult.push_back( pbnNode->nPivot );
  }

  /*------------------------------------------------*/
  /* If the node owns points, check their distances */
  /*------------------------------------------------*/
  bool bSkipRest = false;
  bool bTakeRest = false;
  list<unsigned>::iterator itIdx;
  list<float>::iterator    itDist;
  for( itIdx=pbnNode->vOwned.begin(), itDist=pbnNode->vDist.begin();
       ( itIdx!=pbnNode->vOwned.end() && itDist!=pbnNode->vDist.end() && 
         !bSkipRest ); itIdx++, itDist++ )
    if( m_vActive[*itIdx] )
      if( bTakeRest )
        vResult.push_back( *itIdx );
      else {
//         float d = dDistCenter - (*itDist);
//         /* check if the point needs to be checked */
//         if( d > dMaxDist )
//           // skip all further points
//           bSkipRest = true;
//         else if( d < -dMaxDist )
//           // skip this point
//           continue;
//         else if( dDistCenter + (*itDist) <= dMaxDist ) {
        //float d1 = ((*itDist) - dMaxDist)*((*itDist) - dMaxDist);
        float d2  = ((*itDist) + dMaxDist); d2 = d2*d2;
        /* check if the point needs to be checked */
        if( dDistCenter2 > d2 )
          // skip all further points
          bSkipRest = true;
//         else if( dDistCenter2 < d1 )
//           // skip this point
//           continue;
//         else if( dDistCenter + (*itDist) <= dMaxDist ) {
//           // take all further points without checking
//           vResult.push_back( *itIdx );
//           bTakeRest = true;
//         } else {
        else {
          // use the non-squared distance to enforce further restrictions
          if( dDistCenter<0 )
            dDistCenter = sqrt( dDistCenter2 );
          if( dDistCenter - (*itDist) < -dMaxDist )
            // skip this point
            continue;
          else if( dDistCenter + (*itDist) <= dMaxDist ) {
            // take all further points without checking
            vResult.push_back( *itIdx );
            bTakeRest = true;
          } else {
            // check the point
            FeatureVector &fvPoint = m_vData[*itIdx];
            float dDistPoint2 = fvQuery.compSSD( fvPoint );
            m_nCntDists++;
            if( dDistPoint2 <= dMaxDist2 ) {
              /* add the point to the neighbor list */
              vResult.push_back( *itIdx );
            }
          }
        }
      }
  
  /*--------------------------*/
  /* Check the children nodes */
  /*--------------------------*/
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    if( pbnNode->vpbnChild[i] )
      /* check if the child needs to be checked */
      findNeighbors( pbnNode->vpbnChild[i], fvQuery, 
                     dMaxDist, dMaxDist2, vResult );
}


void BallTree::takeAllChildPoints  ( BallTreeNode *pbnNode,
                                     vector<unsigned> &vResult )
{
  assert( pbnNode );
  
  /*----------------------------------------------*/
  /* If it is an anchor node, also take the pivot */
  /*----------------------------------------------*/
  if( pbnNode->hasPivot() && m_vActive[pbnNode->nPivot] )
    vResult.push_back( pbnNode->nPivot );

  /*------------------------------*/
  /* Take the node's owned points */
  /*------------------------------*/
  list<unsigned>::iterator itIdx;
  for( itIdx=pbnNode->vOwned.begin(); itIdx!=pbnNode->vOwned.end(); itIdx++ )
    if( m_vActive[*itIdx] )
      vResult.push_back( *itIdx );
  
  /*----------------------------*/
  /* Take the children's points */
  /*----------------------------*/
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    if( pbnNode->vpbnChild[i] )
      /* check if the child needs to be checked */
      takeAllChildPoints( pbnNode->vpbnChild[i], vResult );
}

 
void BallTree::findNeighborsPenalty( BallTreeNode *pbnNode, 
                                     const FeatureVector &fvQuery,
                                     float &dMaxDist, float &dMaxDist2,
                                     vector<unsigned> &vResult )
  /*******************************************************************/
  /* Find all neighbors for the given query point in the balltree    */
  /* that are within a search radius of dMaxDist.                    */
  /*******************************************************************/
{
  assert( pbnNode );
  m_nCntVisited++;

  /*--------------------------------------------------------*/
  /* Check if the current node intersects the search volume */
  /*--------------------------------------------------------*/
  float dDistCenter2;
  if( pbnNode->hasPivot() ) // compare with pivot element
    dDistCenter2 = fvQuery.compSSD(m_vData[pbnNode->nPivot] );
  else                      // compare with node mean
    dDistCenter2 = fvQuery.compSSD( pbnNode->fvMean );
  
  float dDistCenter = sqrt(dDistCenter2);
  if( dDistCenter - pbnNode->dRadius > dMaxDist )
    return;

  /*-----------------------------------------------*/
  /* If it is an anchor node, also check the pivot */
  /*-----------------------------------------------*/
  if( pbnNode->hasPivot() && m_vActive[pbnNode->nPivot] ) {
    /* check if the pivot needs to be checked */
    float dPenalty = m_vPenalties[pbnNode->nPivot];
    if( dDistCenter2 + dPenalty <= dMaxDist2 )
      /* add the pivot to the neighbor list */
      vResult.push_back( pbnNode->nPivot );
  }

  /*------------------------------------------------*/
  /* If the node owns points, check their distances */
  /*------------------------------------------------*/
  bool bSkipRest = false;
  bool bTakeRest = false;
  list<unsigned>::iterator itIdx;
  list<float>::iterator    itDist;
  for( itIdx=pbnNode->vOwned.begin(), itDist=pbnNode->vDist.begin();
       ( itIdx!=pbnNode->vOwned.end() && itDist!=pbnNode->vDist.end() && 
         !bSkipRest ); itIdx++, itDist++ )
    if( m_vActive[*itIdx] )
      if( bTakeRest )
        vResult.push_back( *itIdx );
      else {
        float d = dDistCenter - (*itDist);
        /* check if the point needs to be checked */
        if( d > dMaxDist )
          // skip all further points
          bSkipRest = true;
        else if( d < -dMaxDist )
          // skip this point
          continue;
        else {
          // check the point
          FeatureVector &fvPoint = m_vData[*itIdx];
          float dDistPoint2 = fvQuery.compSSD( fvPoint );
          m_nCntDists++;
          float dPenalty = m_vPenalties[*itIdx];
          if( dDistPoint2 + dPenalty <= dMaxDist2 ) {
            /* add the point to the neighbor list */
            vResult.push_back( *itIdx );
          }
        }
      }
  
  /*--------------------------*/
  /* Check the children nodes */
  /*--------------------------*/
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    if( pbnNode->vpbnChild[i] )
      /* check if the child needs to be checked */
      findNeighborsPenalty( pbnNode->vpbnChild[i], fvQuery, 
                            dMaxDist, dMaxDist2, vResult );
}


void BallTree::printStorageNode( unsigned nIdx )
{
  assert( nIdx < m_vData.size() );
  m_vpInNode[nIdx]->printNode();
}


void BallTree::printStorageNodeDist( unsigned nIdx, unsigned nQueryIdx )
{
  assert( nIdx < m_vData.size() );
  assert( nQueryIdx < m_vData.size() );
  printNodeDist( m_vpInNode[nIdx], nQueryIdx );

  BallTreeNode* pbnParent = m_vpInNode[nIdx]->pbnParent;
  while( pbnParent ) {
    cout << "    -------------------------------" << endl;
    cout << "    Parent node: " << endl;
    printNodeDist( pbnParent, nQueryIdx );
    pbnParent = pbnParent->pbnParent;
  }
}


void BallTree::printNodeDist( const BallTreeNode* pbnNode,
                              unsigned nQueryIdx ) const
{
  FeatureVector fvQuery = m_vData[nQueryIdx];
  cout << "    -------------------------------" << endl;
  cout << "    Content of node (" << pbnNode << "):" << endl;
  if( pbnNode->nVisited>0 )
    cout << "      visited: YES, " << pbnNode->nVisited << " times." << endl
         << "      (max dist=" << pbnNode->dVisitMaxDist 
         << ", max dist2=" << pbnNode->dVisitMaxDist2 << ")" << endl;
  else
    cout << "      visited: NO" << endl;
  cout << "      parent : (" << pbnNode->pbnParent << ")" << endl;
  if( pbnNode->bHasPivot )
    cout << "      pivot  : " << pbnNode->nPivot << ", dist="
         << sqrt(fvQuery.compSSD(m_vData[pbnNode->nPivot])) << "   \t["
         << fvQuery.compSSD(m_vData[pbnNode->nPivot]) << "/" 
         << ( fvQuery.compSSD(m_vData[pbnNode->nPivot]) +
              m_vPenalties[pbnNode->nPivot] + m_vPenalties[nQueryIdx] ) 
         << "] " << (m_vActive[pbnNode->nPivot]?"(a)":"(i)") << endl;
  else
    cout << "      pivot  : none" << endl
         << "      distance to mean=" << sqrt(fvQuery.compSSD(pbnNode->fvMean))
         << "   \t[" << fvQuery.compSSD(pbnNode->fvMean) << "]" << endl;
  cout << "      radius : " << pbnNode->dRadius << endl
       << "      #owned : " << pbnNode->nNumOwned << endl;
  list<unsigned>::const_iterator itIdx;
  list<float>::const_iterator    itDist;
  int i=0;
  for( itIdx=pbnNode->vOwned.begin(), itDist=pbnNode->vDist.begin(), i=0; 
       itIdx!=pbnNode->vOwned.end() && itDist!=pbnNode->vDist.end(); 
       itIdx++, itDist++, i++ )
    cout << "        " << i+1 << ": " << *itIdx 
         << ", dist=" << *itDist << "   \t["
         << fvQuery.compSSD(m_vData[*itIdx]) << "/"
         << ( fvQuery.compSSD(m_vData[*itIdx]) + 
              m_vPenalties[*itIdx] + m_vPenalties[nQueryIdx] )
         << "] " << (m_vActive[*itIdx]?"(a)":"(i)") << endl;
  cout << "      #children: " << pbnNode->nNumChildren << endl;
//   cout << "      child1 : (" << pbnNode->pbnChild1 << ")" << endl
//        << "      child2 : (" << pbnNode->pbnChild2 << ")" << endl;
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    cout << "      child" << i+1 << ": (" << pbnNode->vpbnChild[i] << ")" 
         << "\tdist=" << pbnNode->vDistChild[i] 
         << "\trad=" << pbnNode->vRadChild[i] 
         << "\t#points=" << getNumPointsIn( pbnNode->vpbnChild[i] )
         << endl;    
  cout << "    -------------------------------" << endl;
}


void BallTree::printTraceToRoot( unsigned nIdx )
{
  assert( nIdx < m_vData.size() );
  BallTreeNode *pbnNode = m_vpInNode[nIdx];
  cout << "    Tracing node (" << pbnNode << ") to the root: " << endl;
  printTraceToRoot( pbnNode );
  cout << "    -------------------------------" << endl;
}


void BallTree::printTraceToRoot( const BallTreeNode *pbnNode )
{
  //while( pbnNode ) {
  //  cout << "    -> (" << pbnNode->pbnParent << ")" << endl;
  //  pbnNode = pbnNode->pbnParent;
  //}
  if( pbnNode->pbnParent )
    printTraceToRoot( pbnNode->pbnParent );
  if( !pbnNode->pbnParent )
    cout << "       (" << pbnNode->pbnParent << ")" << endl
         << "    -> (";
  else {
//     if( pbnNode->pbnParent->pbnChild1==pbnNode )
//       cout << "     /";
//     if( pbnNode->pbnParent->pbnChild2==pbnNode )
//       cout << "     \\";
//     if( pbnNode->pbnParent->pbnChild1!=pbnNode && 
//         pbnNode->pbnParent->pbnChild2!=pbnNode )
//       cout << "     Error! Node isn't its parent's child:";
    bool bFound = false;
    int  nIdx   = -1;
    for( int i=0; i<pbnNode->pbnParent->nNumChildren && !bFound; i++ )
      if( pbnNode==pbnNode->pbnParent->vpbnChild[i] ) { 
        // node is parent's child i
        bFound = true;
        nIdx   = i;
      }

    if( bFound )
      if( nIdx==0 )
        cout << "     /";
      else if( nIdx==pbnNode->pbnParent->nNumChildren-1 )
        cout << "     \\";
      else
        cout << "     |";
    else
      cout << "     Error! Node isn't its parent's child:";

    cout << " (";
  }
  cout << pbnNode << ") ";
  if( pbnNode->bHasPivot )
    cout << "p";
  cout << endl;
}


void BallTree::clearAllVisited()
{
  clearVisited( m_pbnRoot );
}


void BallTree::clearVisited( BallTreeNode *pbnNode )
{
  assert( pbnNode );
  pbnNode->nVisited = 0;
  pbnNode->dVisitMaxDist  = 0.0;
  pbnNode->dVisitMaxDist2 = 0.0;
//   if( pbnNode->pbnChild1 )
//     clearVisited( pbnNode->pbnChild1 );
//   if( pbnNode->pbnChild2 )
//     clearVisited( pbnNode->pbnChild2 );
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    if( pbnNode->vpbnChild[i] )
      clearVisited( pbnNode->vpbnChild[i] );
}


bool BallTree::checkTree()
{
  clearVisited( m_pbnRoot );
  bool bCorrect;
  cout << "    -------------------------------" << endl;
  cout << "    Checking the tree top-down..." << endl;
  visitTree( m_pbnRoot );
  if( bCorrect=checkTree(m_pbnRoot) )
    cout << "    SUCCESS." << endl;
  else
    cout << "    FAILED!" << endl;

  cout << "    Checking the tree bottom-up..." << endl;
  for(unsigned i=0; i<m_vpInNode.size(); i++ )
    bCorrect = checkReachable( m_vpInNode[i] ) && bCorrect;
  if( bCorrect )
    cout << "    SUCCESS." << endl;
  else
    cout << "    FAILED!" << endl;
  
  cout << "    -------------------------------" << endl;
  return bCorrect;
}


void BallTree::visitTree( BallTreeNode *pbnNode )
{
  assert( pbnNode );
  pbnNode->nVisited++;
//   if( pbnNode->pbnChild1 )
//     visitTree( pbnNode->pbnChild1 );
//   if( pbnNode->pbnChild2 )
//     visitTree( pbnNode->pbnChild2 );
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    if( pbnNode->vpbnChild[i] )
      visitTree( pbnNode->vpbnChild[i] );
}

bool BallTree::checkTree( BallTreeNode *pbnNode )
{
  assert( pbnNode );
  bool bCorrect = true;
  if( pbnNode->nVisited != 1 ) {
    cout << "      Error: node (" << pbnNode << ") visited " 
         << pbnNode->nVisited << " times!" << endl;
    bCorrect = false;
  }
  if( pbnNode->pbnParent ) {
//     if( pbnNode->pbnParent->pbnChild1!=pbnNode && 
//         pbnNode->pbnParent->pbnChild2!=pbnNode ) {
    bool bFound = false;
    for( int i=0; i<pbnNode->pbnParent->nNumChildren && !bFound; i++ )
      if( pbnNode==pbnNode->pbnParent->vpbnChild[i] )  
        // node is parent's child i
        bFound = true;

    if( !bFound ) {
      cout << "      Error: node (" << pbnNode << " isn't its parent's child!"
           << endl;
      bCorrect = false;
    }
  } else if( m_pbnRoot!=pbnNode ) {
    cout << "      Error: node doesn't have a parent but isn't the root!"
         << endl;
    bCorrect = false;
  }

//   if( pbnNode->pbnChild1 )
//     bCorrect = checkTree( pbnNode->pbnChild1 ) && bCorrect;
//   if( pbnNode->pbnChild2 )
//     bCorrect = checkTree( pbnNode->pbnChild2 ) && bCorrect;
  for( int i=0; i<pbnNode->nNumChildren; i++ )
    if( pbnNode->vpbnChild[i] )
      bCorrect = checkTree( pbnNode->vpbnChild[i] ) && bCorrect;

  return bCorrect;
}


bool BallTree::checkReachable( BallTreeNode *pbnNode )
{
  assert( pbnNode );

  if( !pbnNode->pbnParent )
    if( m_pbnRoot==pbnNode )
      return true;
    else {
      cout << "      Error: node (" << pbnNode 
           << ") doesn't have a parent but isn't the root!" << endl;
      return false;
    }
  else
    return checkReachable( pbnNode->pbnParent );
}

