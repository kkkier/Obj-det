/*********************************************************************/
/*                                                                   */
/* FILE         empca.cc                                             */
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

/****************/
/*   Includes   */
/****************/
#include <math.h>
#include <stdlib.h>
#include <vector>

#include <featurematrix.hh>
#include "empca.hh"


/********************************/
/*   Function Implementations   */
/********************************/

void calcEMPCA( const vector<FeatureVector> &vPoints, 
                int nNumComponents, int nNumIter,
                vector<FeatureVector> &vEigenVectors,
                FeatureVector &fvMean )
{
  assert( vPoints.size() > 0 );

  /******************/
  /* Initialization */
  /******************/
  int nNumPoints = vPoints.size();
  int nNumDims   = vPoints[0].numDims();

  /*--------------------------------*/
  /* Compute the mean of all points */
  /*--------------------------------*/
  fvMean = vPoints[0];
  for( int i=1; i<nNumPoints; i++ )
    fvMean += vPoints[i];
  fvMean /= (float) vPoints.size();

  /*-----------------------------------*/
  /* Subtract the mean from all points */
  /*-----------------------------------*/
  FeatureMatrix Y( nNumPoints, nNumDims );
  for( int i=0; i<nNumPoints; i++ )
    for( int j=0; j<nNumDims; j++ )
      Y(i,j) = vPoints[i].at(j) - fvMean.at(j);

  /*-----------------------------------*/
  /* Initialize the eigenvector matrix */
  /*-----------------------------------*/
  FeatureMatrix C( nNumComponents, nNumDims );
  for( int i=0; i<nNumComponents; i++ )
    for( int j=0; j<nNumDims; j++ )
      C(i,j) = floor(rand() / (RAND_MAX+1.0));

  /*-----------------------------------------*/
  /* Initialize the matrix of unknown states */
  /*-----------------------------------------*/
  FeatureMatrix X( nNumPoints, nNumComponents );


  /****************************/
  /* Perform the EM Iteration */
  /****************************/
  FeatureMatrix Ct;
  FeatureMatrix Xt;
  for( int i=0; i<nNumIter; i++ ) {
    /* E-Step */
    Ct = C.transpose();
    X = (Ct * C).invert() * Ct * Y;

    /* M-Step */
    Xt = X.transpose();
    C = Y * Xt * (X * Xt).invert();
  }


  /****************************/
  /* Return the Result Values */
  /****************************/
  vEigenVectors.clear();
  for( int i=0; i<nNumComponents; i++ ) {
    FeatureVector fvComponent( nNumDims );

    for( int j=0; j<nNumDims; j++ )
      fvComponent.at(j) = C(i,j);

    vEigenVectors.push_back( fvComponent );
  }
}


void calcEMPCA( const vector<ClPoint> &vPoints, 
                int nNumComponents, int nNumIter,
                vector<ClPoint> &vEigenVectors,
                ClPoint &clMean )
{
  assert( vPoints.size() > 0 );

  /******************/
  /* Initialization */
  /******************/
  int nNumPoints = vPoints.size();
  int nNumDims   = vPoints[0].dim();

  /*--------------------------------*/
  /* Compute the mean of all points */
  /*--------------------------------*/
  clMean = vPoints[0];
  for( int i=1; i<nNumPoints; i++ )
    clMean = clMean.add( vPoints[i] );
  clMean = clMean.div( (float) vPoints.size() );

  /*-----------------------------------*/
  /* Subtract the mean from all points */
  /*-----------------------------------*/
  FeatureMatrix Y( nNumPoints, nNumDims );
  for( int i=0; i<nNumPoints; i++ )
    for( int j=0; j<nNumDims; j++ )
      Y(i,j) = vPoints[i].at(j) - clMean.at(j);

  /*-----------------------------------*/
  /* Initialize the eigenvector matrix */
  /*-----------------------------------*/
  FeatureMatrix C( nNumComponents, nNumDims );
  for( int i=0; i<nNumComponents; i++ )
    for( int j=0; j<nNumDims; j++ )
      C(i,j) = floor(rand() / (RAND_MAX+1.0));

  /*-----------------------------------------*/
  /* Initialize the matrix of unknown states */
  /*-----------------------------------------*/
  FeatureMatrix X( nNumPoints, nNumComponents );


  /****************************/
  /* Perform the EM Iteration */
  /****************************/
  FeatureMatrix Ct;
  FeatureMatrix Xt;
  for( int i=0; i<nNumIter; i++ ) {
    /* E-Step */
    Ct = C.transpose();
    X = (Ct * C).invert() * Ct * Y;

    /* M-Step */
    Xt = X.transpose();
    C = Y * Xt * (X * Xt).invert();
  }


  /****************************/
  /* Return the Result Values */
  /****************************/
  vEigenVectors.clear();
  for( int i=0; i<nNumComponents; i++ ) {
    ClPoint clComponent( nNumDims );

    for( int j=0; j<nNumDims; j++ )
      clComponent.at(j) = C(i,j);

    vEigenVectors.push_back( clComponent );
  }
}


void calcEMPCAIdzs( const vector<ClPoint> &vPoints, 
                    const vector<int>     &vIdzs,
                    int nNumComponents, int nNumIter,
                    vector<ClPoint> &vEigenVectors,
                    ClPoint &clMean )
  /* version for in-place computation on only a subset of the origi- */
  /* nal vectors.                                                    */
{
  assert( vPoints.size() > 0 );

  /******************/
  /* Initialization */
  /******************/
  int nNumPoints = vIdzs.size();
  int nNumDims   = vPoints[0].dim();

  /*--------------------------------*/
  /* Compute the mean of all points */
  /*--------------------------------*/
  clMean = vPoints[ vIdzs[0] ];
  for( int i=1; i<nNumPoints; i++ )
    clMean = clMean.add( vPoints[ vIdzs[i] ] );
  clMean = clMean.div( (float) nNumPoints );

  /*-----------------------------------*/
  /* Subtract the mean from all points */
  /*-----------------------------------*/
  FeatureMatrix Y( nNumPoints, nNumDims );
  for( int i=0; i<nNumPoints; i++ )
    for( int j=0; j<nNumDims; j++ )
      Y(i,j) = vPoints[ vIdzs[i] ].at(j) - clMean.at(j);

  /*-----------------------------------*/
  /* Initialize the eigenvector matrix */
  /*-----------------------------------*/
  FeatureMatrix C( nNumComponents, nNumDims );
  for( int i=0; i<nNumComponents; i++ )
    for( int j=0; j<nNumDims; j++ )
      C(i,j) = floor(rand() / (RAND_MAX+1.0));

  /*-----------------------------------------*/
  /* Initialize the matrix of unknown states */
  /*-----------------------------------------*/
  FeatureMatrix X( nNumPoints, nNumComponents );


  /****************************/
  /* Perform the EM Iteration */
  /****************************/
  FeatureMatrix Ct;
  FeatureMatrix Xt;
  for( int i=0; i<nNumIter; i++ ) {
    /* E-Step */
    Ct = C.transpose();
    X = (Ct * C).invert() * Ct * Y;

    /* M-Step */
    Xt = X.transpose();
    C = Y * Xt * (X * Xt).invert();
  }


  /****************************/
  /* Return the Result Values */
  /****************************/
  vEigenVectors.clear();
  for( int i=0; i<nNumComponents; i++ ) {
    ClPoint clComponent( nNumDims );

    for( int j=0; j<nNumDims; j++ )
      clComponent.at(j) = C(i,j);

    vEigenVectors.push_back( clComponent );
  }
}


