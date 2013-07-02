/*********************************************************************/
/*                                                                   */
/* FILE         kballtree.cc                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Implement an efficient NN search based on a balltree */
/*              structure. The search structure is built using the   */
/*              clustering trace. This file calls functions written  */
/*              by Krystian Mikolajczyk.                             */
/*                                                                   */
/* BEGIN        Fri Sep 23 2005                                      */
/* LAST CHANGE  Fri Sep 23 2005                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>

#include "kballtree.hh"

/*******************/
/*   Definitions   */
/*******************/

/*===================================================================*/
/*                          Class KBallTree                          */
/*===================================================================*/

KBallTree::KBallTree()
  /* standard constructor */
{
  m_vKBallTree.clear();
  m_bIsValid = false;
}


KBallTree::~KBallTree()
  /* destructor */
{
  KM::deleteDescriptors( m_vKBallTree );
}


/***********************************************************/
/*                      Construction                       */
/***********************************************************/

void KBallTree::build  ( const vector<FeatureVector> &vData,
                         /*const*/ vector<ClStep>    &vClusterTrace,
                         float dMinThresh, float dMaxThresh, int nLevels,
                         float dQuant, bool bVerbose )
{
  if( bVerbose )
    cout << "  KBallTree::build() called with " << vData.size() 
         << " data points..." << endl;

  /*--------------------------------------------*/
  /* Convert the feature vectors to descriptors */
  /*--------------------------------------------*/
  vector<KM::CornerDescriptor*> vDescr;
  cnvFeatureVecsToDescriptors( vData, vDescr );
  
  /*--------------------------------------------*/
  /* Build up a balltree from the cluster trace */
  /*--------------------------------------------*/
  m_vKBallTree.clear();
  KM::traceToBallTree( vClusterTrace, vDescr, m_vKBallTree, 
                       dMinThresh, dMaxThresh, nLevels, dQuant );

  m_bIsValid = true;

  if( bVerbose )
    cout << "  done." << endl;
}


/***********************************************************/
/*                       NN Search                         */
/***********************************************************/

vector<FeatureVector> KBallTree::findNeighbors( const FeatureVector &fvQuery, 
                                                float dMaxDist2, int nLevel, 
                                                bool bVerbose )
{
  assert( m_bIsValid );
  assert( m_vKBallTree.size() > 0 );

  /*-------------------------*/
  /* Convert the query point */
  /*-------------------------*/
  int dim = fvQuery.numDims();
  KM::CornerDescriptor *desc = new KM::CornerDescriptor;
  desc->allocVec(dim);
  float *vec = desc->getVec();
  for( int v=0; v<dim; v++ ) {         
    vec[v]=fvQuery.at(v);          
  }

  /*-------------------*/
  /* Run the NN search */
  /*-------------------*/
  vector<KM::CornerDescriptor*> vMatches;
  KM::testBallTree( desc, m_vKBallTree, dMaxDist2, vMatches, nLevel );

  /*--------------------------*/
  /* Convert the results back */
  /*--------------------------*/
  vector<FeatureVector> vResults;
  cnvDescriptorsToFeatureVecs( vMatches, vResults );
  
  return vResults;
}


vector<KM::CornerDescriptor*> KBallTree::findNeighbors2( const FeatureVector &fvQuery, 
                                                     float dMaxDist2, int nLevel, 
                                                     bool bVerbose )
{
  assert( m_bIsValid );
  assert( m_vKBallTree.size() > 0 );

  /*-------------------------*/
  /* Convert the query point */
  /*-------------------------*/
  int dim = fvQuery.numDims();
  KM::CornerDescriptor *desc = new KM::CornerDescriptor;
  desc->allocVec(dim);
  float *vec = desc->getVec();
  for( int v=0; v<dim; v++ ) {         
    vec[v]=fvQuery.at(v);          
  }

  /*-------------------*/
  /* Run the NN search */
  /*-------------------*/
  vector<KM::CornerDescriptor*> vMatches;
  KM::testBallTree( desc, m_vKBallTree, dMaxDist2, vMatches, nLevel );

  /* don't convert the results back */
  return vMatches;
}


/***********************************************************/
/*                    Service Functions                    */
/***********************************************************/

void KBallTree::cnvFeatureVecsToDescriptors( const vector<FeatureVector> &vFeatures,
                                             vector<KM::CornerDescriptor *>  &vDesc )
  /* Convert the feature vector data to a vector of cluster points.  */
{
  float *vec;
  int dim=vFeatures[0].numDims();
  KM::CornerDescriptor *ds;
  for( uint i=0; i<vFeatures.size(); i++ ){
    ds=new KM::CornerDescriptor();      
    ds->allocVec(dim);
    vec=ds->getVec();
    for( int v=0; v<dim; v++ ) {         
      vec[v]=vFeatures[i].at(v);          
    }
    vDesc.push_back(ds);
  }
}


void KBallTree::cnvDescriptorsToFeatureVecs( vector<KM::CornerDescriptor *>  &vDesc,
                                             vector<FeatureVector> &vFeatures )
{
  float *vec;
  int dim=DESC_DIMENSION;
  vFeatures.clear();
  for( uint i=0; i<vDesc.size(); i++ ){
    vFeatures.push_back(FeatureVector(dim));
    vec=vDesc[i]->getVec();
    for( int v=0; v<dim; v++ )         
      vFeatures[i].at(v) = vec[v];
  }
}
