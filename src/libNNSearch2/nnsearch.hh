/*********************************************************************/
/*                                                                   */
/* FILE         nnsearch.hh                                          */
/* AUTHORS      Bastian Leibe, based on code by Sameer Nene          */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Implement an efficient NN search based on the method */
/*              by Nene and Nayar, as described in the following     */
/*              paper:                                               */
/*                                                                   */
/*              S. Nene, S. Nayar,                                   */
/*              A Simple Algorithm for Nearest Neighbor Search in    */
/*              High Dimensions.                                     */
/*              IEEE Trans. PAMI, Vol. 19(9), pp. 989-1003, 1997.    */
/*                                                                   */
/*              This implementation is based on code by Sameer Nene, */
/*              as provided on the CAVE web page at Columbia Univ.   */
/*              The code has been modified to work on the Feature-   */
/*              Vector data structure.                               */
/*                                                                   */
/* BEGIN        Wed Jul 24 2002                                      */
/* LAST CHANGE  Wed Jul 24 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_NNSEARCH_HH
#define LEIBE_NNSEARCH_HH

//#define NDEBUG
  
/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>
#include <cassert>

#include <featurevector.hh>

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                           Class NNSearch                          */
/*===================================================================*/
/* Define a class for efficient NN search in high dimensions*/
class NNSearch
{
public:
	NNSearch ();
	NNSearch ( const vector<FeatureVector> &vData );
	~NNSearch();

public:
  /********************/
  /*   Construction   */
  /********************/
  void init( const vector<FeatureVector> &vData, bool bVerbose=false );
  
public:
  /*****************/
  /*   NN Search   */
  /*****************/
  int         getNN    ( /*const*/ FeatureVector &fvQuery, float dMaxDist, 
                         float dEps, bool bVerbose=false );
  void        getNNs   ( /*const*/ FeatureVector &fvQuery, float dMaxDist, 
                         float dEps,
                         vector<int> &vResultIdzs, 
                         vector<float> &vResultDist2, bool bVerbose=false );
  vector<int> getNNIdzs( /*const*/ FeatureVector &fvQuery, 
                         float dMaxDist, float dEps, bool bVerbose=false );

protected:
  /*************************/
  /*   Service Functions   */
  /*************************/
  int binSearchL( int nCol, float dMinVal ) const;
  int binSearchR( int nCol, float dMinVal ) const;

protected:
  vector< vector<int> > m_vvBackMap;
  vector< vector<int> > m_vvForwardMap;
  vector<FeatureVector> m_vData;

  vector<FeatureVector> m_vOrigData;

  bool m_bIsValid;
};


#endif // LEIBE_NNSEARCH_HH
