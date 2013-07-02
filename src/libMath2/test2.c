#include <stdio.h>
#include "libMath.h"

/* #include "libU.h" */

int main( )
{
  Matrix *A;
  Vector *x, *b;
  int i;

  A = matrix_alloc( 2, 2 );
  /* for( i = 0; i < m->dim_M; i++ )
     for( j = 0; j < m->dim_N; j++ )
     { 
     M( m, i, j) = (double) i*i + 120.0;
     }
     */

  M( A, 0, 0 ) = 1;
  M( A, 0, 1 ) = 2.1113;
  M( A, 1, 0 ) = 1;
  M( A, 1, 1 ) = 2.1112;

  b = vector_alloc( 2 );
  V( b, 0 ) = 50;
  V( b, 1 ) = 52;
  
  x = vector_alloc( 2 );

  matrix_solve_lin_system( A, b, x );

  for( i = 0; i < 2; i++ )
    printf( " %d =  %f \n", i, V( x, i ) );
  
  
}





