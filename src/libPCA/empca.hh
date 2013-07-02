/*********************************************************************/
/*                                                                   */
/* FILE         empca.hh                                             */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Implements the EM PCA algorithm from Roweis for fin- */
/*              ding the first k eigenvectors of a matrix very effi- */
/*              ciently. Details to the algorithm can be found in    */
/*              the following paper:                                 */
/*                                                                   */
/*                S. Roweis,                                         */
/*                EM Algorithms for PCA and SPCA.                    */
/*                In Proc. NIPS'97,  pp.626-632, 1997.               */
/*                                                                   */
/*                                                                   */
/* BEGIN        Mon May 31 2004                                      */
/* LAST CHANGE  Mon May 31 2004                                      */
/*                                                                   */
/*********************************************************************/

#ifndef EMPCA_HH
#define EMPCA_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <cassert>

#include <featurevector.hh>
#include <featurematrix.hh>

#include <cluster.hh>

/*******************/
/*   Definitions   */
/*******************/

/******************************/
/*   Prototype Declarations   */
/******************************/
void calcEMPCA( const vector<FeatureVector> &vPoints, 
                int nNumComponents, int nNumIter,
                vector<FeatureVector> &vEigenVectors,
                FeatureVector &fvMean );

void calcEMPCA( const vector<ClPoint> &vPoints, 
                int nNumComponents, int nNumIter,
                vector<ClPoint> &vEigenVectors,
                ClPoint &clMean );

void calcEMPCAIdzs( const vector<ClPoint> &vPoints, 
                    const vector<int>     &vIdzs,
                    int nNumComponents, int nNumIter,
                    vector<ClPoint> &vEigenVectors,
                    ClPoint &clMean );


#endif
