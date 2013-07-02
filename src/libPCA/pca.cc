/*********************************************************************/
/*                                                                   */
/* FILE         pca.cc                                               */
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

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <math.h>
#include <values.h>   // for FLT_MAX
#include <stdio.h>
#include <algorithm>

#include "pca.hh"

/******************************/
/*   Prototype Declarations   */
/******************************/
bool feature_calc_avg( const vector<FeatureVector> &vFeatures, 
                       FeatureVector &avg );

/*****************/
/*   Functions   */
/*****************/

void calcEigenVectors( const vector<FeatureVector> &vFeatures, 
                       Matrix **eig_vec, Vector **eig_val, Vector** psi )
{
  int i, j, dimSmall, dimLarge;
  FeatureVector avg;
  Matrix *PhiA, *PhiAT, *L, *C, *eig_vec_tmp, *eig_vec_tmp2;
  Vector *vec1, *vec2;
  double **mat_val, sum, help;
  Vector *tmp1, *tmp2;
  int num_data, num;

  num_data = num = vFeatures.size();
  
  if( num > vFeatures[0].numDims() ) {
    /*******************/
    /*   Regular PCA   */
    /*******************/
    /* dimensions of PhiA (matrix of data), psi,... */
    int dimImages = dimLarge = num;
    int dimPixels = dimSmall = vFeatures[0].numDims();
    
    printf(" dimS = %d -- dimL = %d\n", dimSmall, dimLarge );
    
    /* average img_u and concatenate as psi (average-vector) */
    feature_calc_avg( vFeatures, avg );
    *psi = vector_alloc( dimPixels );
    feature_2_vec( *psi, avg );
    
    //img_close( &av_u );
    
    /* write data-images into PhiA */
    PhiA = matrix_alloc( dimPixels, dimImages );
    vec1 = vector_alloc( dimPixels );
    vec2 = vector_alloc( dimPixels );
    for( i = 0; i < dimImages; i++ )
      { 
        feature_2_vec( vec1, vFeatures[i] );
        
        vector_diff( vec1, *psi, vec2 );
        matrix_setcol( PhiA, i, vec2 );
      }
    vector_free( vec1 );
    vector_free( vec2 );
    
    /* calc PhiA transposed -> PhiAT */
    PhiAT  = matrix_alloc( dimImages, dimPixels );
    matrix_transpose( PhiA, PhiAT );
    
    /* calc covariance matrix C = PhiA x PhiAT but: */
    C      = matrix_alloc( dimPixels, dimPixels );
    matrix_prod( PhiA, PhiAT, C );
    matrix_free( PhiAT );
    
    /* eigenvector/eigenval of C */
    printf(" calculate eigenvectors, householder..."); fflush( stdout ); 
    *eig_val = vector_alloc( dimPixels );
    *eig_vec  = matrix_alloc( dimPixels, dimPixels );
    eigen_householder( C, *eig_val, *eig_vec ); 
    printf("  done\n");
    matrix_free( C );
    
    matrix_free( PhiA );
    
  } else {
    /**************************************/
    /*   PCA with transposed Cov matrix   */
    /**************************************/
    /* dimensions of PhiA (matrix of data), psi,... */
    dimSmall = num;
    dimLarge = vFeatures[0].numDims();
    
    printf(" dimS = %d -- dimL = %d\n", dimSmall, dimLarge );
    
    /* average img_u and concatenate as psi (average-vector) */
    feature_calc_avg( vFeatures, avg );
    *psi = vector_alloc( dimLarge );
    feature_2_vec( *psi, avg );
    
    //img_close( &av_u );
    
    /* write data-images into PhiA */
    PhiA = matrix_alloc( dimLarge, dimSmall );
    vec1 = vector_alloc( dimLarge );
    vec2 = vector_alloc( dimLarge );
    for( i = 0; i < dimSmall; i++ )
      { 
        feature_2_vec( vec1, vFeatures[i] );
        
        vector_diff( vec1, *psi, vec2 );
        matrix_setcol( PhiA, i, vec2 );
      }
    vector_free( vec1 );
    vector_free( vec2 );
    
    /* calc PhiA transposed -> PhiAT */
    PhiAT  = matrix_alloc( dimSmall, dimLarge );
    matrix_transpose( PhiA, PhiAT );
    
    /* calc not covariance matrix C = PhiA x PhiAT but: */
    L      = matrix_alloc( dimSmall, dimSmall );
    matrix_prod( PhiAT, PhiA, L );
    matrix_free( PhiAT );
    
    /* eigenvector/eigenval of L */
    printf(" calculate eigenvectors, householder..."); fflush( stdout ); 
    *eig_val    = vector_alloc( dimSmall );
    eig_vec_tmp = matrix_alloc( dimSmall, dimSmall );
    eigen_householder( L, *eig_val, eig_vec_tmp ); 
    printf("  done\n");
    matrix_free( L );
    
    /* calc real eigenvector of covariance matrix C */
    eig_vec_tmp2 = matrix_alloc( dimLarge, dimSmall );
    matrix_prod( PhiA, eig_vec_tmp, eig_vec_tmp2 );

    /* normalize all vectors inside eigenmatrix */
    mat_val = eig_vec_tmp2->val;  
    for( j = 0; j < dimSmall; j++ ) { 
      sum = 0.0;
      for( i = 0; i < dimLarge; i++ ) { 
        help = MVal( mat_val, i, j );
        sum += help*help; //SYS_SQR( help );
      }
      
      sum = sqrt( sum );
      for( i = 0; i < dimLarge; i++ )
        MVal( mat_val, i, j ) = MVal( mat_val, i, j ) / sum;
    }
    
    *eig_vec = matrix_alloc( dimSmall, dimLarge );  
    matrix_transpose( eig_vec_tmp2, *eig_vec );
    
    matrix_free( eig_vec_tmp );
    matrix_free( eig_vec_tmp2 );
    matrix_free( PhiA );
  }

  return;
}


void calcEigenVectors( const vector<FeatureVector> &vFeatures,
                       vector<FeatureVector> &vEigenVectors, 
                       vector<float> &vEigenValues,
                       FeatureVector &fvMean )
{
  int i, j, dimSmall, dimLarge;
  FeatureVector avg;
  Matrix *PhiA, *PhiAT, *L, *C, *eig_vec_tmp, *eig_vec_tmp2;
  Vector *vec1, *vec2, *psi;
  Vector *eig_val;
  double **mat_val, sum, help;
  Vector *tmp1, *tmp2;
  Matrix *eig_vec;
  int num_data, num;

  printf( "  Performing PCA...\n" );
  num_data = num = vFeatures.size();
  
  if( num > vFeatures[0].numDims() ) {
    /*******************/
    /*   Regular PCA   */
    /*******************/
    /* dimensions of PhiA (matrix of data), psi,... */
    int dimImages = dimLarge = num;
    int dimPixels = dimSmall = vFeatures[0].numDims();
    
    printf("    dimS = %d -- dimL = %d\n", dimSmall, dimLarge );
    
    /* average img_u and concatenate as psi (average-vector) */
    printf( "    Computing the average image...\n" );
    feature_calc_avg( vFeatures, avg );
    psi = vector_alloc( dimPixels );
    feature_2_vec( psi, avg );
    fvMean = avg;
    
    //img_close( &av_u );
    
    /* write data-images into PhiA */
    printf( "    Computing the difference matrix...\n" );
    PhiA = matrix_alloc( dimPixels, dimImages );
    vec1 = vector_alloc( dimPixels );
    vec2 = vector_alloc( dimPixels );
    for( i = 0; i < dimImages; i++ )
      { 
        feature_2_vec( vec1, vFeatures[i] );
        
        vector_diff( vec1, psi, vec2 );
        matrix_setcol( PhiA, i, vec2 );
      }
    vector_free( vec1 );
    vector_free( vec2 );
    
    /* calc PhiA transposed -> PhiAT */
    PhiAT  = matrix_alloc( dimImages, dimPixels );
    matrix_transpose( PhiA, PhiAT );
    
    /* calc covariance matrix C = PhiA x PhiAT: */
    printf( "    Computing the covariance matrix C...\n" );
    C      = matrix_alloc( dimPixels, dimPixels );
    matrix_prod( PhiA, PhiAT, C );
    matrix_free( PhiAT );
    
    /* eigenvector/eigenval of C */
    printf( "    Calculating eigenvectors, householder...\n" ); 
    fflush( stdout ); 
    eig_val  = vector_alloc( dimPixels );
    eig_vec  = matrix_alloc( dimPixels, dimPixels );
    eigen_householder( C, eig_val, eig_vec ); 
    printf( "    done\n" );
    matrix_free( C );
    
    matrix_free( PhiA );
    
    /* copy the Eigenvectors */
    vEigenVectors.clear();
    Vector *tmp = vector_alloc( dimPixels );
    for( i=0; i<dimSmall; i++ ) {
      FeatureVector fvEigen;
      
      matrix_getcol( eig_vec, i, tmp );
      vec_2_feature( tmp, fvEigen );
      
      vEigenVectors.push_back( fvEigen );
    }
    vector_free( tmp );

  } else {
    /**************************************/
    /*   PCA with transposed Cov matrix   */
    /**************************************/
    /* dimensions of PhiA (matrix of data), psi,... */
    int dimImages = dimSmall = num;
    int dimPixels = dimLarge = vFeatures[0].numDims();
    
    printf( "    dimS = %d -- dimL = %d\n", dimSmall, dimLarge );
    
    /* average img_u and concatenate as psi (average-vector) */
    printf( "    Computing the average image...\n" );
    feature_calc_avg( vFeatures, avg );
    psi = vector_alloc( dimLarge );
    feature_2_vec( psi, avg );
    fvMean = avg;
    
    //img_close( &avg );
    
    /* write data-images into PhiA */
    printf( "    Computing the difference matrix...\n" );
    PhiA = matrix_alloc( dimLarge, dimSmall );
    vec1 = vector_alloc( dimLarge );
    vec2 = vector_alloc( dimLarge );
    for( i = 0; i < dimSmall; i++ )
      { 
        feature_2_vec( vec1, vFeatures[i] );
        
        vector_diff( vec1, psi, vec2 );
        matrix_setcol( PhiA, i, vec2 );
      }
    vector_free( vec1 );
    vector_free( vec2 );
    
    /* calc PhiA transposed -> PhiAT */
    PhiAT  = matrix_alloc( dimSmall, dimLarge );
    matrix_transpose( PhiA, PhiAT );
    
    /* calc not covariance matrix C = PhiA x PhiAT but: */
    printf( "    Computing the transpose-covariance matrix L...\n" );
    L      = matrix_alloc( dimSmall, dimSmall );
    matrix_prod( PhiAT, PhiA, L );
    matrix_free( PhiAT );
    
    /* eigenvector/eigenval of L */
    printf( "    Calculating eigenvectors, householder...\n" ); 
    fflush( stdout ); 
    eig_val    = vector_alloc( dimSmall );
    eig_vec_tmp = matrix_alloc( dimSmall, dimSmall );
    eigen_householder( L, eig_val, eig_vec_tmp ); 
    printf( "    done\n");
    matrix_free( L );
    
    /* calc real eigenvector of covariance matrix C */
    eig_vec_tmp2 = matrix_alloc( dimLarge, dimSmall );
    matrix_prod( PhiA, eig_vec_tmp, eig_vec_tmp2 );
    
    /* normalize all vectors inside eigenmatrix */
    mat_val = eig_vec_tmp2->val;  
    for( j = 0; j < dimSmall; j++ ) { 
      sum = 0.0;
      for( i = 0; i < dimLarge; i++ ) { 
        help = MVal( mat_val, i, j );
        sum += help*help; //SYS_SQR( help );
      }
      
      sum = sqrt( sum );
      for( i = 0; i < dimLarge; i++ )
        MVal( mat_val, i, j ) = MVal( mat_val, i, j ) / sum;
    }
    
    eig_vec = matrix_alloc( dimSmall, dimLarge );  
    matrix_transpose( eig_vec_tmp2, eig_vec );
    
    /* copy the Eigenvectors */
    vEigenVectors.clear();
    Vector *tmp = vector_alloc( dimPixels );
    for( i=0; i<dimSmall; i++ ) {
      FeatureVector fvEigen;
      
      matrix_getcol( eig_vec_tmp2, i, tmp );
      vec_2_feature( tmp, fvEigen );
      
      vEigenVectors.push_back( fvEigen );
    }

    vector_free( tmp );
    matrix_free( eig_vec_tmp );
    matrix_free( eig_vec_tmp2 );
    matrix_free( PhiA );
  }
  
  /* copy the mean vector */
  vec_2_feature( psi, fvMean );
  
  /* copy the Eigenvalues */
  vEigenValues.clear();
  for( i=0; i<dimSmall; i++ )
    vEigenValues.push_back( V( eig_val, i ) );

  vector_free( psi );
  vector_free( eig_val );
  matrix_free( eig_vec );

  printf( "  done.\n" );
  return;  
}


void applyEigenVectors( Matrix* eig_vec, Vector *psi, 
                        FeatureVector &FeatureVec,
                        Vector *vres )  
  /* double* dff, double *dif ) */
{
  Vector* vec1, *vec2;
  /* double  ene_vec, ene_res; */
  
  vec1 = vector_alloc( VDim( psi ) );
  vec2 = vector_alloc( VDim( psi ) );
  
  feature_2_vec( vec1, FeatureVec );
  
  vector_diff( vec1, psi, vec2 );
  matrix_vector( eig_vec, vec2, vres );

  /* vector_ene( vec2, VDim( vec2 ), &ene_vec ); 
     vector_ene( vres, num_data-1,   &ene_res ); 
     *dff = ene_vec - ene_res;
     *dif = ene_res;
     */
  
  vector_free( vec2 );
  vector_free( vec1 );

  return;
}


void applyEigenVectors( const vector<FeatureVector> &vEigenVectors,
                        const FeatureVector &fvMean,
                        const FeatureVector &fvFeatureVec,
                        FeatureVector &fvResult )  
     /* double* dff, double *dif ) */
{
  Vector* vec1, *vec2, *vres;
  Vector* psi;
  Matrix* eig_vec;
  /* double  ene_vec, ene_res; */
  
  vec1    = vector_alloc( fvMean.numDims() );
  vec2    = vector_alloc( fvMean.numDims() );
  vres    = vector_alloc( vEigenVectors.size() );
  psi     = vector_alloc( fvMean.numDims() );
  eig_vec = matrix_alloc( vEigenVectors.size(), fvMean.numDims() );  
  
  /* copy the input vectors */
  feature_2_vec( vec1, fvFeatureVec );
  feature_2_vec( psi , fvMean );

  Vector *tmp = vector_alloc( fvFeatureVec.numDims() );
  for( int i=0; i<vEigenVectors.size(); i++ ) {    
    feature_2_vec( tmp, vEigenVectors[i] );
    matrix_setline( eig_vec, i, tmp );
  }
  vector_free( tmp );

  vector_diff( vec1, psi, vec2 );
  matrix_vector( eig_vec, vec2, vres );

  /* vector_ene( vec2, VDim( vec2 ), &ene_vec ); 
     vector_ene( vres, num_data-1,   &ene_res ); 
     *dff = ene_vec - ene_res;
     *dif = ene_res;
     */
  
  vec_2_feature( vres, fvResult );

  vector_free( vec2 );
  vector_free( vec1 );
  vector_free( vres );
  vector_free( psi );
  matrix_free( eig_vec );

  return;
}


double calcRecoError( const vector<FeatureVector> &vEigenVectors,
                      const FeatureVector &fvMean,
                      const FeatureVector &fvOriginal,
                      const FeatureVector &fvProjected )
{
  // Matlab code:
  // Phi_f = avg_img;
  // for i=1:M,
  //   Phi_f =  Phi_f + new_weights(i)*eig_img(:,i);   % w_i, u_i
  // end
  
  // % dffs
  // Phi = new_vec; %-avg_img;
  // dffs = norm(Phi - Phi_f); % accumulated sum using "components" (number) 
  //                           % of eigenvectors
  
  Vector *vec1    = vector_alloc( fvMean.numDims() );
  Vector *vec2    = vector_alloc( fvProjected.numDims() );
  Vector *reco    = vector_alloc( fvMean.numDims() );
  Vector *psi     = vector_alloc( fvMean.numDims() );
  Matrix *eig_vec = matrix_alloc( fvMean.numDims(), vEigenVectors.size() );
  
  /* copy the input vectors */
  feature_2_vec( psi , fvMean );
  feature_2_vec( vec1, fvOriginal );
  feature_2_vec( vec2, fvProjected );

  Vector *tmp = vector_alloc( fvOriginal.numDims() );
  for( int i=0; i<vEigenVectors.size(); i++ ) {    
    feature_2_vec( tmp, vEigenVectors[i] );
    matrix_setcol( eig_vec, i, tmp );
  }

  /* reconstruct the projected vector */
  feature_2_vec( reco, fvMean );
  for( int i=0; i<vEigenVectors.size(); i++ ) {
    matrix_getcol( eig_vec, i, tmp );
    vector_scalar( tmp, fvProjected.at(i), tmp );
    vector_sum( reco, tmp, reco );
  }

  /* compute the distance */
  double dffs;
  vector_diff( vec1, reco, tmp );
  vector_norm( tmp, &dffs );

  /* clean up */
  vector_free( tmp );  
  vector_free( vec1 );
  vector_free( vec2 );
  vector_free( reco );
  vector_free( psi );
  matrix_free( eig_vec );
  
  return dffs;
}


void getComponentsEnergy( const vector<FeatureVector> &vEigenVectors, 
                          FeatureVector fvEigenValues,
                          float dEVEnergy,
                          vector<FeatureVector> &vComponents )
  /* return the n components correponding to the eigenvalue energy   */
  /* of dEVEnergy (as a fraction of the total EV energy).            */
{
  float dTotalSum = fvEigenValues.getSum();
  float dSum = fvEigenValues.at(0);
  vComponents.clear();
  vComponents.push_back( vEigenVectors[0] );
  for( int i=1; i<fvEigenValues.numDims(); i++ ) {
    dSum += fvEigenValues.at(i);
    if( dSum > dEVEnergy*dTotalSum )
      break;
    else
      vComponents.push_back( vEigenVectors[i] );
  }
}


bool feature_2_vec( Vector* vec, const FeatureVector &FeatureVec )
{
  float  help;
  int    i;
  
  if( vec == NULL || !FeatureVec.isValid() )
    { 
      printf(" feature_2_vec: you gave me NULL pointer\n");
      return false;
    }

  /* double-check, not really necessary */
  if( VDim( vec ) != FeatureVec.numDims() )
    { 
      printf(" img_2_vec: wrong dimensions of vector/img\n");
      return( false );
    }
  
  /* vec_val = vec->val; */
  for( i = 0; i < FeatureVec.numDims(); i++ ) {
    help = FeatureVec.at(i);
    V( vec, i ) = (double) help;
  }
  
  return( true );
}


bool vec_2_feature( const Vector* vec, FeatureVector &FeatureVec )
{
  int  help;
  int  x, y, n, n1, n2;
  double min_val, max_val, fac;
  
  if( vec == NULL )
    { 
      printf(" vec_2_feature: you gave me NULL pointer\n");
      return false;
    }
  
  FeatureVec.clear();
  FeatureVec.setNumDims( VDim( vec ) );
  
  for( n = 0; n < VDim(vec); n++ )
    FeatureVec.setValue( n, V( vec, n ) );

  return( true );
}


bool feature_calc_avg( const vector<FeatureVector> &vFeatures, 
                       FeatureVector &avg )
{
  int i, x, y;
  float help;
  
  for( i = 0; i < vFeatures.size(); i++ )
    if( vFeatures[i].numDims() != vFeatures[0].numDims() ) {
      printf(" feature_calc_av: feature vectors are not compatible\n");
      return false;
    }
  
  avg = vFeatures[0];

  for( i = 1; i < vFeatures.size(); i++ )
    avg.addVector( vFeatures[i] );

  avg.multFactor( 1.0 / (float) vFeatures.size() );

  return( true );  
}


bool vecfeature_2_mat( Matrix* mat, const vector<FeatureVector> &vFeatureVecs)
{
  float  help;
  
  if( vFeatureVecs.size() == 0 ) {
    cerr << "  vecfeature_2_mat: No FeatureVectors passed to function!" 
         << endl;
    return false; 
  }

  if( mat == NULL ) { 
    cerr << "  vecfeature_2_mat: you gave me NULL pointer" << endl;
    return false;
  }
  
  /* double-check, not really necessary */
  if( MDim1( mat ) != vFeatureVecs.size() ) { 
    cerr << "  vecfeature_2_mat: wrong dimension1 of vector/img" << endl;
    return false;
  }

  for( int i=0; i<vFeatureVecs.size(); i++ ) 
    if( !vFeatureVecs[i].isValid() ||
        ( MDim2( mat ) != vFeatureVecs[i].numDims() ) ) {
      cerr << "  vecfeature_2_mat: wrong dimension2 of vector/img" << endl;
      return false;

    } else {
      /* copy the FeatureVector entries into the matrix */
      for( int j = 0; j < vFeatureVecs[i].numDims(); j++ ) {
        help = vFeatureVecs[i].at(j);
        M( mat, i, j) = (double) help;
      }
    }
  
  return true;
}


bool mat_2_vecfeature( const Matrix* mat, vector<FeatureVector> &vFeatureVecs)
{
  int  help;
  
  if( mat == NULL )
    { 
      cerr << "  mat_2_vecfeature: you gave me NULL pointer" << endl;
      return false;
    }
  
  vFeatureVecs.clear();
  for( int i = 0; i < MDim1(mat); i++ ) {
    FeatureVector fvNew( MDim2(mat) );
  
    for( int j = 0; j < MDim2(mat); j++ ) {
      fvNew.setValue( j, M( mat, i, j ) );
    }
    vFeatureVecs.push_back( fvNew );
  }

  return true;
}


