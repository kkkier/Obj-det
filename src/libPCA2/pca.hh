/*********************************************************************/
/*                                                                   */
/* FILE         pca.hh                                               */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Code for principal component analysis (PCA) on       */
/*              arbitrary feature vectors.                           */
/*                                                                   */
/* BEGIN        Wed Aug 07 2002                                      */
/* LAST CHANGE  Wed Aug 07 2002                                      */
/*                                                                   */
/*********************************************************************/

/*----------------------- CVS Log ------------------------------
 * $Log: pca.hh,v $
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.1.1.1  2004/04/05 08:23:06  seemanne
 * Imported sources
 *
 * Revision 1.2  2003/05/08 09:10:09  leibe
 * CVS log added.
 *
 */

#ifndef LEIBE_PCA_HH
#define LEIBE_PCA_HH
  
/****************/
/*   Includes   */
/****************/
#include <vector>
#include <cassert>

extern "C" {
#include <libMath.h>
}
#include <featurevector.hh>

/* PCA calculation */
void calcEigenVectors( const vector<FeatureVector> &vFeatures, 
                       Matrix **eig_vec, Vector **eig_val, Vector** psi );
void calcEigenVectors( const vector<FeatureVector> &vFeatures,
                       vector<FeatureVector> &vEigenVectors, 
                       vector<float> &vEigenValues,
                       FeatureVector &fvMean );

void applyEigenVectors( Matrix* eig_vec, Vector *psi, 
                        FeatureVector &FeatureVec,
                        Vector *vres );
void applyEigenVectors( const vector<FeatureVector> &vEigenVectors,
                        const FeatureVector &fvMean,
                        const FeatureVector &fvFeatureVec,
                        FeatureVector &fvResult );  

double calcRecoError( const vector<FeatureVector> &vEigenVectors,
                      const FeatureVector &fvMean,
                      const FeatureVector &fvOriginal,
                      const FeatureVector &fvProjected ); 

void getComponentsEnergy( const vector<FeatureVector> &vEigenVectors, 
                          FeatureVector fvEigenValues,
                          float dEVEnergy,
                          vector<FeatureVector> &vComponents );

/* Conversion Functions */
bool feature_2_vec( Vector* vec, const FeatureVector &FeatureVec );
bool vec_2_feature( const Vector* vec, FeatureVector &FeatureVec );

bool vecfeature_2_mat( Matrix* mat, const vector<FeatureVector> &vFeatureVecs);
bool mat_2_vecfeature( const Matrix* mat, vector<FeatureVector> &vFeatureVecs);



#endif // LEIBE_PCA_HH
