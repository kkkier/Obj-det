#include "libMath.h"
#include "getdouble.h"
#include "libU.h"

void main( )
{
  Matrix *m, *mT, *eig_vec_J, *eig_vec_T, *L;
  Vector *eig_val_J, *eig_val_T;
  int i, j, tt_ja, tt_ho;

  m = matrix_alloc( 100, 50 );
  for( i = 0; i < m->dim_M; i++ )
    for( j = 0; j < m->dim_N; j++ )
      { 
        M( m, i, j) = (double) i*i + 120.0;
      }

  mT = matrix_alloc( 50, 100 );
  matrix_transpose( m, mT );
  
  eig_vec_J = matrix_alloc( 100, 100 );
  eig_val_J = vector_alloc( 100  );
  L       = matrix_alloc( 100, 100 );

  matrix_prod( m, mT, L );
  Start_Clock_once( &tt_ja );
  jacobi( L, eig_val_J, eig_vec_J );
  End_ms_Clock_once( tt_ja, 1, "jacobi used " );

  printf("done with jacobi\n" );
  /* for( i = 0; i < 100; i++ ) printf(" %f", V(eig_val_J,i) );
     printf("\n");
     for( i = 0; i < 100; i++ ) printf(" %f", M(eig_vec_J,i,0) );
     printf("\n");
   */

  matrix_prod( m, mT, L );
  eig_vec_T = matrix_alloc( 100, 100 );
  eig_val_T = vector_alloc( 100  );

  Start_Clock_once( &tt_ho );
  eigen_householder( L, eig_val_T, eig_vec_T );  
  End_ms_Clock_once( tt_ho, 1, "householder used " );

  printf(" done with householder\n" );
  /* for( i = 0; i < 100; i++ ) printf(" %f", V(eig_val_T,i) );
     printf("\n");
     for( i = 0; i < 100; i++ ) printf(" %f", M(eig_vec_T,i,0) );
     printf("\n");
   */


  
}





