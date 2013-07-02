/*********************************************************************/
/*                                                                   */
/* FILE         kballtree.hh                                         */
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

#ifndef KBALLTREE_HH
#define KBALLTREE_HH


/****************/
/*   Includes   */
/****************/
#include <vector>
#include <list>
#include <string>
#include <cassert>

#include <featurevector.hh>
#include <clstep.hh>

namespace KM {
#include "kballtree/cluster/codebook.h"
}

/*******************/
/*   Definitions   */
/*******************/

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                           Class KBallTree                         */
/*===================================================================*/
/* Define a class for efficient NN search in high dimensions*/
class KBallTree
{
public:
	KBallTree ();
  ~KBallTree();

public:
  /********************/
  /*   Construction   */
  /********************/
  bool isValid() { return m_bIsValid; }

  void build  ( const vector<FeatureVector> &vData,
                /*const*/ vector<ClStep>    &vClusterTrace,
                float dMinThresh, float dMaxThresh, int nLevels,
                float dQuant=0.5, bool bVerbose=false );

public:
  /*****************/
  /*   NN Search   */
  /*****************/
  vector<FeatureVector>     findNeighbors ( const FeatureVector &fvQuery, 
                                            float dMaxDist2, int nLevel=0, 
                                            bool bVerbose=false );

  vector<KM::CornerDescriptor*> findNeighbors2( const FeatureVector &fvQuery, 
                                                float dMaxDist2, int nLevel=0, 
                                                bool bVerbose=false );

public:
  /*************************/
  /*   Service Functions   */
  /*************************/
  void cnvFeatureVecsToDescriptors( const vector<FeatureVector> &vFeatures,
                                    vector<KM::CornerDescriptor *>  &vDesc );

  void cnvDescriptorsToFeatureVecs( vector<KM::CornerDescriptor *>  &vDesc,
                                    vector<FeatureVector> &vFeatures );

protected:
  vector<KM::CornerDescriptor*> m_vKBallTree;
  bool m_bIsValid;
};


// void traceToBallTree(vector<ClStep> vClusterTrace, 
//                      vector<CornerDescriptor *> vDesc,
//                      vector< CornerDescriptor *> &nodes, 
//                      float minThresh, float maxThresh, int levels, 
//                      float quant=0.5);

// void testBallTree( CornerDescriptor *desc, 
//                    vector<CornerDescriptor* > nodes, 
//                    float dThres,  
//                    vector<CornerDescriptor* > &matches, int level=0);


#endif
