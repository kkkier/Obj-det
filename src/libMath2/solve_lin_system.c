#include <stdio.h>
#include <malloc.h>
#include <math.h>
/* #include <nrutil.h> */
#include "libMath.h"
#include "sys_const.itf"

/* void ludcmp(float **a, int n, int *indx, float *d);
   void lubksb(float **a, int n, int *indx, float b[]); */



/* void solve_lin_system_in_place(float **A, float *b, float *x, int size); */


#define TINY 1.0e-20;

static int ludcmp( Matrix *a, int n, int *indx, double *d )
{
  int i,imax,j,k;
  double big,dum,sum,temp;
  /* float *vv; */
  Vector* vv;
  
  /* vv=vector(0,n-1); */
  vv = vector_alloc( n );
  *d=1.0;
  for (i=0;i< n;i++) 
    {
      big=0.0;
      for (j = 0;j< n;j++)
	if ((temp = SYS_FABS( M(a,i,j) )) > big ) big=temp;
      if (big == 0.0) 
	{
	  printf( "Singular matrix in routine ludcmp\n" );
	  return(-1);
	}
      V(vv,i) = 1.0/big;
    }
  
  for (j = 0;j< n;j++) {
    for (i=0;i<j;i++) {
      sum = M(a,i,j);
      for (k=0;k<i;k++) sum -= M(a,i,k)*M(a,k,j);
      M(a,i,j)=sum;
    }
    big=0.0;
    for (i=j;i< n;i++) 
      {
	sum = M(a,i,j);
	for (k = 0;k<j;k++)
	  sum -= M(a,i,k)*M(a,k,j);
	M(a,i,j)=sum;
	if ( (dum=V(vv,i)*fabs(sum)) >= big) 
	  {
	    big=dum;
	    imax=i;
	  }
      }
    if (j != imax) {
      for (k=0;k<n;k++) {
	dum=M(a,imax,k);
	M(a,imax,k) = M(a,j,k);
	M(a,j,k) = dum;
      }
      *d = -(*d);
      V(vv,imax) = V(vv,j);
    }
    indx[j]=imax;
    if ( M(a,j,j) == 0.0 ) M(a,j,j)=TINY;
    if (j != n-1) 
      {
	dum=1.0/(M(a,j,j));
	for (i=j+1;i< n;i++) M(a,i,j) *= dum;
      }
  }
  /* free_vector(vv,0,n-1); */
  vector_free( vv );

  return( 0 );
}
#undef TINY


static void lubksb( Matrix *a, int n, int *indx, Vector *b)
{
  int i,ii=-1,ip,j;
  double sum;
  
  for (i=0;i<n;i++) {
    ip = indx[i];
    sum = V(b,ip);
    V(b,ip) = V(b,i);
    if (ii > -1)
      for (j=ii;j<=i-1;j++) sum -= M(a,i,j) * V(b,j);
    else if (sum) ii=i;
    V(b,i)=sum;
  }
  for (i=n-1;i>= 0;i--) {
    sum = V(b,i);
    for (j=i+1;j<n;j++) sum -= M(a,i,j) * V(b,j);
    V(b,i) = sum / M(a,i,i);
  }
}



extern int matrix_solve_lin_system( Matrix* A, Vector *b, Vector *x )
/* void 
   solve_lin_system( Matrix *A, Vector *b, Vector *x, int size) */
     /* Solve linear system Ax = b for x
        where A is a size*size matrix 

	Assumes: A,x,b all allocated and valid

	copies A so it isn't bashed
	*/
{
  int i, j, *indx;
  double d;
  int size;
  /* float **A_copy; */
  Matrix *A_copy;
  
  /* make copy of A; */
  /*malloc it*/
  /* A_copy = (float **)malloc(size * sizeof(float *)); 
     for (i = 0; i< size; i++) {
     A_copy[i] = (float *)malloc(size * sizeof(float));
     }
     */
  
  if( A == NULL || x == NULL || b == NULL )
    { printf(" matrix_solve_lin_system: you gave me a NULL-pointer\n");
      return( MATH_FATAL_ERROR );
    }

  size = MDim1( A );
  if( size != MDim2( A ) || size != VDim( x ) || size != VDim( b ) )
    { printf(" matrix_solve_lin_system: some dimension problems\n");
      return( MATH_WARNING );
    }

  
  A_copy = matrix_alloc( size, size );

  /*copy values into A_copy*/
  matrix_copy( A, A_copy );
  /* for (i = 0; i< size; i++)
     for (j = 0; j< size; j++)
     A_copy[i][j] = A[i][j];
     */

  indx = (int *)malloc(size*sizeof(int));

  if( ludcmp(A_copy, size,  indx, &d) < 0 )
    return( MATH_FATAL_ERROR );
  
  lubksb(A_copy, size, indx, b);

  /* for (i = 0; i < size; i++) {
     x[i] = b[i];
     }
     */
  vector_copy( b, x );

  free(indx);
  
  /* for (i = 0; i < size; i++)
     free(A_copy[i]);
     free(A_copy);
     */
  matrix_free( A_copy );
  
  return( MATH_SUCCESS );
}


/* void  */
/* solve_lin_system_in_place(float **A, float *b, float *x, int size) */
/*       Solve linear system Ax = b for x */
/*         where A is a size*size matrix  */

/* 	Assumes: A,x,b all allocated and valid */
/* 	*/ 
/* { */
/*   int i, j, *indx; */
/*   float d; */

/*   indx = (int *)malloc(size*sizeof(int)); */

/*   ludcmp(A, size,  indx, &d); */
/*   lubksb(A, size, indx, b); */

/*   for (i = 0; i < size; i++) { */
/*     x[i] = b[i]; */
/*   } */

/*   free(indx); */
/* } */

