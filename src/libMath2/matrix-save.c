/************************************************************************
*
*       Modulename      : matrix.c
*
*       Author          : Bernt Schiele
*
*       Date            : 17 April 1994
*       Version         : 0.0
*       Goal            : 
*
*       Modifications   : author(s)     : 
*                         date          : 
*                         justification : 
*
************************************************************************/


/*********************************************************************
*
*   liste of Includes   
*
********************************************************************/
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include "libMath.h"
#include "matrix.itf"    /* MATRIX_.. */

/*********************************************************************
*
*   global Variables of this module
*
********************************************************************/
static Matrix *mat_ptr[MATRIX_MAX];
static char   mat_flag[MATRIX_MAX];
static int    mat_ptr_num = 0;

/*********************************************************************
*
*   Functions implemented in that module
*
********************************************************************/
static void     matrix_add_ptr ();
extern Matrix * matrix_alloc ();
extern int      matrix_copy ();
static void     matrix_del_ptr ();
extern int      matrix_diff ();
extern int      matrix_free ();
extern void     matrix_free_all ();
extern int      matrix_getcol();
extern int      matrix_out ();
extern int      matrix_prod ();
extern int      matrix_transpose ();
extern int      matrix_setcol ();
extern int      matrix_sum ();
extern int      matrix_vector ();



/*********************************************************************
*
*   Body of the functions
*
********************************************************************/

/********************************************************************
*
*   Function    : matrix_add_ptr
*   Goal        : 
*
*    
*   input parameters   : dim_M : number of rows
*                        dim_N : number of columns
*
*   output parameters  : 
*
*   returnted Value    :
*
********************************************************************/
static void matrix_add_ptr( Matrix* m)
{
  int i;
  for( i = 0; i < mat_ptr_num; i++ )
    if( mat_flag[i] == MATRIX_DEL )
      { mat_ptr[i] = m;
	mat_flag[i] = MATRIX_ADD;
	break;
      }	
  if( i == mat_ptr_num )
    if( i < MATRIX_MAX )
      { mat_ptr_num++;
	mat_ptr[i] = m;
	mat_flag[i] = MATRIX_ADD;
      }
    else 
      { printf(" matrix_add_ptr: too much Matrix-Pointer\n");
      }
}

/********************************************************************
*
*   Function    : matrix_alloc 
*   Goal        : 
*
*    
*   input parameters   : dim_M : number of rows
*                        dim_N : number of columns
*
*   output parameters  : 
*
*   returnted Value    :
*
********************************************************************/
extern Matrix * matrix_alloc ( int dim_M, int dim_N )
{
  char        *ret;
  Matrix      *mat;
  double      **rowptr;
  char        *memptr;
  char        *help;
  int         elementsize;
  int         i, memsize;
  int diff;

  elementsize = sizeof(double);
  
  memsize = (dim_N * dim_M * elementsize)  /* mem for matrix */
             + (sizeof(Matrix))            /* mem for info */
             + (dim_N * elementsize);      /* mem for pointer to rows */
                                           /* Remark, that you need (?!) */
                                           /* sizeof(double) for the ptr */
  ret = (char *) malloc( memsize );
  printf(" memsize is %d\n",memsize);
  if( ret == NULL )
    { printf(" matrix_alloc: cannot allocate matrix \n");
      exit( 0 );
    }
  mat = (Matrix *) ret;
  mat->dim_M = dim_M;
  mat->dim_N = dim_N;
  rowptr  = (double **) ( ((char *) mat) + sizeof(Matrix) ) ;
  memptr  = (char *)    ( ((char *) rowptr) + (dim_M * elementsize) );

  mat->mem   =  (double *) memptr;
  mat->val   =  (double **) rowptr;
  
  for( i = 0; i < dim_M; i++ )
    { rowptr[i] = (double*) memptr;
      memptr += dim_N * elementsize;
    }
  /* memset( (char*) mat->mem, 0, dim_M * dim_N * sizeof(double) );*/
  matrix_add_ptr( mat );
  return( mat ) ;
  
}


/********************************************************************
*
*   Function    : matrix_copy
*   Goal        : 
*
*    
*   input parameters   : m1 : original matrix (source)
*
*   output parameters  : m2 : will be overwrited by m1 (destination)
*                             must be allready allocated
*
*   returnted Value    : success or error-value
*
********************************************************************/
extern int matrix_copy( Matrix* m1, Matrix* m2 )
{
  int i,j;

  if( (m1 == NULL) || (m2 == NULL) )
    { printf(" matrix_copy: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( (m1->dim_M != m2->dim_M) || (m1->dim_N != m2->dim_N) )
    { printf(" matrix_copy: matrices not the same taille\n");
      return( MATH_WARNING );
    }

  for( i = 0; i < m1->dim_M; i++ )
    for( j = 0; j < m2->dim_N; j++ )
      M(m2,j,i) = M(m1,j,i);
  
  return( MATH_SUCCESS );
}

/********************************************************************
*
*   Function    : matrix_del_ptr
*   Goal        : 
*
*    
*   input parameters   : m
*
*   output parameters  : 
*
*   returnted Value    : 
*
********************************************************************/
static void matrix_del_ptr( Matrix *m )
{
  int i;
  for( i = 0; i < mat_ptr_num; i++ )
    if( m == mat_ptr[i] )
      { mat_flag[i] = MATRIX_DEL;
	break;
      }
  if( i >= mat_ptr_num )
    printf(" matrix_del_ptr: couldn't find Matrix-Pointer\n");
  else if( i == mat_ptr_num-1 )
    mat_ptr_num--;
}

/********************************************************************
*
*   Function    : matrix_diff
*   Goal        : 
*
*    
*   input parameters   : m1, m2 : the original matices
*
*   output parameters  : mres = m1 - m2
*
*   returnted Value    : success or error messages
*
********************************************************************/
extern int matrix_diff( Matrix *m1, Matrix *m2, Matrix *mres )
{
  int i,j;

  if( (m1 == NULL) || (m2 == NULL) || (mres == NULL) )
    { printf(" matrix_diff: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( (m1->dim_M != m2->dim_M) || (m1->dim_M != mres->dim_M) ||
      (m1->dim_N != m2->dim_N) || (m1->dim_N != mres->dim_N) )
    { printf(" matrix_diff: wrong dimensions of matrices\n");
      return( MATH_WARNING );
    }

  for( i = 0; i < mres->dim_M; i++ )
    for( j = 0; j < mres->dim_N; j++ )
      M(mres,i,j) = M(m1,i,j) - M(m2,i,j) ;

  return( MATH_SUCCESS );
}

/********************************************************************
*
*   Function    : matrix_free
*   Goal        : 
*
*    
*   input parameters   : 
*
*   output parameters  : 
*
*   returnted Value    :
*
********************************************************************/
extern int matrix_free( Matrix* m )
{
  if( m != NULL )
    { matrix_del_ptr( m );
      free( m->val );
      free( m );
      return( MATH_SUCCESS );
    }
  else
    { return( MATH_WARNING );
    }
}

/********************************************************************
*
*   Function    : matrix_free_all
*   Goal        : 
*
*    
*   input parameters   : 
*
*   output parameters  : 
*
*   returnted Value    :
*
********************************************************************/
extern void matrix_free_all ( void )
{
  int i;
  for( i = 0; i < mat_ptr_num; i++ )
    if( mat_flag[i] == MATRIX_ADD )
      { matrix_free( mat_ptr[i] );
	mat_flag[i] = MATRIX_DEL;
      }
  mat_ptr_num = 0;
}
/********************************************************************
*
*   Function    : matrix_getcol
*   Goal        : 
*
*    
*   input parameters   : 
*
*   output parameters  : 
*
*   returnted Value    :
*
********************************************************************/
extern int matrix_getcol( Matrix *mat, int j, Vector *vec ) 
{
  int i;

  if( (mat == NULL) || (vec == NULL) )
    { printf(" matrix_getcol: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( mat->dim_M != vec->dim )
    { printf(" matrix_getcol: wrong size of matrix or vector\n");
      return( MATH_WARNING );
    }

  for( i = 0; i < mat->dim_M; i++ )
    V(vec,i) = V(mat,i,j);

  return( MATH_SUCCESS );      

}
/********************************************************************
*
*   Function    : matrix_out
*   Goal        : 
*
*    
*   input parameters   : 
*
*   output parameters  : 
*
*   returnted Value    :
*
********************************************************************/
extern int matrix_out( Matrix* mat, char* comment )
{
  int i, j;

  if( mat == NULL )
    { printf(" matrix_out: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  printf(" matrix_out: %s \n",comment);
  for( i = 0; i < mat->dim_M; i++ )
    { printf(" row %d : ",i);
      for( j = 0; j < mat->dim_N; j++ )
	printf(" %f , ",M(mat,i,j);
      printf("\n");
    }
  printf("\n");

  return( MATH_SUCCESS );
}


/********************************************************************
*
*   Function    : matrix_prod
*   Goal        : 
*
*    
*   input parameters   : m1, m2 : original matrices
*
*   output parameters  : mres = m1 * m2
*
*   returnted Value    : success or error messages
*
********************************************************************/
extern int matrix_prod( Matrix *m1, Matrix *m2, Matrix *mres )
{
  int    i,j,n;
  double sum;

  if( (m1 == NULL) || (m2 == NULL) || (mres == NULL) )
    { printf(" matrix_prod: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( (m1->dim_M != mres->dim_M) || (m1->dim_N != m2->dim_M) || 
      (m2->dim_N != mres->dim_N) )
    { printf(" matrix_prod: wrong dimensions of matrices\n");
      return( MATH_WARNING );
    }

  for( i = 0; i < mres->dim_M; i++ )
    for( j = 0; j < mres->dim_N; j++ )
      { sum = 0.0;
	for( n = 0; n < m1->dim_N; n++ )
	  sum += M(m1,i,n) * M(m2,n,j);
	M(mres,i,j) = sum;
      }

  return( MATH_SUCCESS );
}

/********************************************************************
*
*   Function    : matrix_transpose
*   Goal        : 
*
*    
*   input parameters   : m  :original matrix
*
*   output parameters  : mT = m^T (transposed matrix)
*
*   returnted Value    : success or error messages
*
********************************************************************/
extern int matrix_transpose( Matrix *m, Matrix *mT )
{
  int    i, j;
  double **val, **val_T;

  if( (m == NULL) || (mT == NULL) )
    { printf(" matrix_transpose: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( ( MDim1(m) != MDim2(mT) ) || ( MDim2(m) != MDim1(mT) ) )
    { printf(" matrix_transpose: wrong dimensions of matrices\n");
      return( MATH_WARNING );
    }

  val = m->val;
  val_T = mT->val;

  for( i = 0; i < MDim1(m); i++ )
    for( j = 0; j < MDim2(m); j++ )
      MVal( val_T, j, i ) = MVal(val,i,j);

  return( MATH_SUCCESS );
}

/********************************************************************
*
*   Function    : matrix_setcol
*   Goal        : 
*
*    
*   input parameters   : m    : original matrix
*                        v    : vector to be set in matrix
*                        i    : number of column
*
*   output parameters  : m    : m with m[i] = v (column i is set to v)
*
*   returnted Value    : error or success messages
*
********************************************************************/
extern int matrix_setcol( Matrix *mat, int j, Vector *vec )
{
  int i;
  double **mval, *vval, help;

  if( (mat == NULL) || (vec == NULL) )
    { printf(" matrix_setcol: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( MDim1(mat) != VDim(vec) )
    { printf(" matrix_setcol: wrong dimensions of matrix or vector\n");
      return( MATH_WARNING );
    }
  
  if( (j < 0) || (j >= MDim2(mat)) )
    { printf(" matrix_setcol: column %i does not exist ",j);
      return( MATH_WARNING );
    }

  /* 
  mval = mat->val;
  vval = vec->val;
  */
  for( i = 0; i < mat->dim_M; i++ )
    { help = (double) V(vec, i);
      M(mat,i,j) = (double) help;
    }

  return( MATH_SUCCESS );
}


/********************************************************************
*
*   Function    : matrix_sum
*   Goal        : 
*
*    
*   input parameters   : m1, m2 : original matrices
*
*   output parameters  : mres = m1 + m2
*
*   returnted Value    : error or success messages
*
********************************************************************/
extern int matrix_sum( Matrix *m1, Matrix *m2, Matrix *mres )
{
  int i,j;

  if( (m1 == NULL) || (m2 == NULL) || (mres == NULL) )
    { printf(" matrix_sum: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( (m1->dim_M != m2->dim_M) || (m1->dim_M != mres->dim_M) ||
      (m1->dim_N != m2->dim_N) || (m1->dim_N != mres->dim_N) )
    { printf(" matrix_sum: wrong dimensions of matrices\n");
      return( MATH_WARNING );
    }

  for( i = 0; i < mres->dim_M; i++ )
    for( j = 0; j < mres->dim_N; j++ )
      M(mres,i,j) = M(m1,i,j) + M(m2,i,j) ;

  return( MATH_SUCCESS );
}


/********************************************************************
*
*   Function    : matrix_vector
*   Goal        : 
*
*    
*   input parameters   : 
*
*   output parameters  : 
*
*   returnted Value    :
*
********************************************************************/
extern int matrix_vector( Matrix* mat, Vector* vec, Vector* vres )
{
  int i, j;
  double *vval, **mval, sum;

  if( (mat == NULL) || (vec == NULL) || (vres == NULL) )
    { printf(" matrix_vector: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( (mat->dim_M != vres->dim) || (mat->dim_N != vec->dim) )
    { printf(" matrix_vector: wrong dimensions of matrice or vectors\n");
      return( MATH_WARNING );
    }
 
  mval = mat->val;
  vval = vec->val;
  for( i = 0; i < mat->dim_M; i++ )
    { sum = 0.0;
      for( j = 0; j < mat->dim_N; j++ )
	sum += MVal(mval,i,j) * VVal(vval,j);
      vres->val[i] = sum;
    }

  return( MATH_SUCCESS );
}

/*  -----------------------  end of file  ----------------------- */













