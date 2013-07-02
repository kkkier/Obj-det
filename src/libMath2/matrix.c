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
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include "libMath.h"
#include "matrix.itf"    /* MATRIX_.. */
#include "sys_const.itf" /* MIN */
#include "sys_proto.h"

/*********************************************************************
*
*   global Variables of this module
*
********************************************************************/
static Matrix *mat_ptr[MATRIX_MAX];
static int    mat_flag[MATRIX_MAX];
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
extern int      matrix_getline();
extern int      matrix_out ();
extern int      matrix_norm ();
extern int      matrix_prod ();
extern int      matrix_prod_transpose ();
extern int      matrix_transpose ();
extern int      matrix_setcol ();
extern int      matrix_setline();
extern int      matrix_sum ();
extern int      matrix_vector ();
extern int      matrix_zero ();


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
*   input parameters   : 
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
  int         elementsize;
  int         i, memsize;

  elementsize = sizeof(double);
  
  memsize = (dim_N * dim_M * elementsize) /* mem for matrix-values      */
             + (sizeof(Matrix))           /* mem for info/structure     */
             + (dim_N * elementsize);     /* mem for pointer to rows    */
                                          /* Remark, that you need (?!) */
                                          /* sizeof(double) for the ptr */
  ret = (char *) malloc( memsize );
  /* printf(" memsize is %d\n",memsize); */
  if( ret == NULL )
    { printf(" matrix_alloc: cannot allocate matrix \n");
      exit( 0 );
    }
  mat = (Matrix *) ret;
  mat->dim_M = dim_M;
  mat->dim_N = dim_N;
  rowptr  = (double **) ( ((char *) mat) + sizeof(Matrix) ) ;
  memptr  = (char *)    ( ((char *) rowptr) + (dim_N * elementsize) );

  mat->mem   =  (double *) memptr;
  mat->val   =  (double **) rowptr;
  
  for( i = 0; i < dim_N; i++ )
    { rowptr[i] = (double*) memptr;
      memptr += dim_M * elementsize;
    }

  memset( (char*) mat->mem, 0, dim_M * dim_N * sizeof(double) );
  /* matrix_add_ptr( mat ); */
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

  if( (MDim1(m1) != MDim1(m2)) || (MDim2(m1) != MDim2(m2)) )
    { printf(" matrix_copy: matrices not the same taille\n");
      return( MATH_WARNING );
    }

  for( i = 0; i < MDim1(m1); i++ )
    for( j = 0; j < MDim2(m2); j++ )
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

  if( (MDim1(m1) != MDim1(m2)) || (MDim1(m1) != MDim1(mres)) ||
      (MDim2(m1) != MDim2(m2)) || (MDim2(m1) != MDim2(mres)) )
    { printf(" matrix_diff: wrong dimensions of matrices\n");
      return( MATH_WARNING );
    }

  for( i = 0; i < MDim1(mres); i++ )
    for( j = 0; j < MDim2(mres); j++ )
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
    { /* matrix_del_ptr( m ); */
      /* free( m->val ); */
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
  int    size;
  char   *mem_m, *mem_v;

  if( (mat == NULL) || (vec == NULL) )
    { printf(" matrix_getcol: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( MDim1(mat) != VDim(vec) )
    { printf(" matrix_getcol: wrong size of matrix or vector\n");
      return( MATH_WARNING );
    }

  if( (j < 0) || (j >= MDim2(mat)) )
    { printf(" matrix_getcol: column %i does not exist ",j);
      return( MATH_WARNING );
    }

  mem_m = (char*) mat->mem;
  mem_m += j * MDim1(mat) * sizeof(double);
  mem_v = (char*) vec->mem;
  size  = VDim(vec) * sizeof(double);
  memcpy( mem_v, mem_m, size );
  
  /* for( i = 0; i < MDim1(mat); i++ )
     V(vec,i) = M(mat,i,j);
     */
  
  return( MATH_SUCCESS );      

}

/********************************************************************
*
*   Function    : matrix_getline
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
extern int matrix_getline( Matrix *mat, int i, Vector *vec ) 
{
  int    j;
  char   *mem_m, *mem_v;

  if( (mat == NULL) || (vec == NULL) )
    { printf(" matrix_getline: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( MDim2(mat) != VDim(vec) )
    { printf(" matrix_getline: wrong size of matrix or vector\n");
      return( MATH_WARNING );
    }

  if( (i < 0) || (i >= MDim1(mat)) )
    { printf(" matrix_getline: line %d does not exist ",j);
      return( MATH_WARNING );
    }

  for( j = 0; j < MDim2(mat); j++ )
    V(vec,j) = M(mat,i,j);

  return( MATH_SUCCESS );      

}

/********************************************************************
*
*   Function    : matrix_norm
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
extern int matrix_norm( Matrix * mat , double* result )
{
  int i, j, m, n;
  double **val, sum;

  if( mat == NULL )
    { printf(" matrix_norm: you gave me a NULL-pointer \n");
      return( MATH_FATAL_ERROR );
    }
  m = MDim1(mat);
  n = MDim2(mat);
  val = mat->val;
  sum = 0.0;
  for( i = 0; i < m; i++ )
    for( j = 0; j < n; j++ )
      sum += SYS_SQR( MVal(val,i,j) );

  *result = sqrt( sum );
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
  for( i = 0; i < MDim1(mat); i++ )
    { printf(" row %d : ",i);
      for( j = 0; j < MDim2(mat); j++ )
	printf(" %f , ",M(mat,i,j));
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
  int    i,j,k,m,n,kmax,kcal;
  double sum;
  double sum1, sum2, sum3, sum4;
  double a1, b1, c1, d1;
  double a2, b2, c2, d2;
  double **val1, **val2, **valres;

  if( (m1 == NULL) || (m2 == NULL) || (mres == NULL) )
    { printf(" matrix_prod: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }


  if( (MDim1(m1) != MDim1(mres)) || (MDim2(m1) != MDim1(m2)) || 
      (MDim2(m2) != MDim2(mres)) )
    { printf(" matrix_prod: wrong dimensions of matrices\n");
      return( MATH_WARNING );
    }

  m = MDim1(mres); n = MDim2(mres);
  if( SYS_MIN( m, n ) <= 5 )
    { for( i = 0; i < m; i++ )
	for( j = 0; j < n; j++ )
	  { sum = 0.0;
	    for( k = 0; k < MDim2(m1); k++ )
	      sum += M(m1,i,k) * M(m2,k,j);
	    M(mres,i,j) = sum;
	  }
    }
  else
    { val1 = m1->val;
      val2 = m2->val;
      valres = mres->val;
      kmax = MDim2(m1);
      if( kmax % 2 ) kcal = 1; else kcal = 0;
      for( i = 0; i < m-1; i+=2 )
	for( j = 0; j < n-1; j+=2 )
	  { sum1 = sum2 = sum3 = sum4 = 0.0;
	    for( k = 0; k < kmax - 1; k+=2 )
	      { a1 = MVal(val1, i ,k); b1 = MVal(val1, i ,k+1);
		c1 = MVal(val1,i+1,k); d1 = MVal(val1,i+1,k+1);
		a2 = MVal(val2, k ,j); b2 = MVal(val2, k ,j+1);
		c2 = MVal(val2,k+1,j); d2 = MVal(val2,k+1,j+1);
		
		sum1 += a1 * a2 + b1 * c2;
		sum2 += a1 * b2 + b1 * d2;
		sum3 += c1 * a2 + d1 * c2;
		sum4 += c1 * b2 + d1 * d2;
	      }
	    if( kcal )
	      { k = kmax-1;
		sum1 += MVal(val1, i ,k) * MVal(val2,k, j );
		sum2 += MVal(val1, i ,k) * MVal(val2,k,j+1);
		sum3 += MVal(val1,i+1,k) * MVal(val2,k, j );
		sum4 += MVal(val1,i+1,k) * MVal(val2,k,j+1);
	      }
	    MVal(valres, i , j ) = sum1;
	    MVal(valres, i ,j+1) = sum2;
	    MVal(valres,i+1, j ) = sum3;
	    MVal(valres,i+1,j+1) = sum4;
	  }
      
      if( m % 2 )
	{ for( j = 0; j < n; j++ )
	    { sum = 0.0;
	      for( k = 0; k < kmax; k++ )
		sum += MVal(val1,n-1,k) * MVal(val2,k,j);
	      MVal(valres,n-1, j ) = sum;
	    }
	}

      if( n % 2 )
	{ if( m % 2 ) m--;
	  for( i = 0; i < m; i++ )
	    { sum = 0.0;
	      for( k = 0; k < kmax; k++ )
		sum += MVal(val1,i,k) * MVal(val2,k,n-1);
	      MVal(valres, i ,n-1) = sum;
	    }
	}

    }


  return( MATH_SUCCESS );
}

/********************************************************************
*
*   Function    : matrix_prod_transpose
*   Goal        : multiply matrix with its transpose
*
*    
*   input parameters   : m1 : original matrices
*
*   output parameters  : m_out = m_int * m_in^T
*
*   returnted Value    : success or error messages
*
*   Author             : Frank Wallner
********************************************************************/

int matrix_prod_transpose( Matrix *m_in, Matrix *m_out )
{
  int  i,j,k,m,n;
  double sum;

  if( (m_in == NULL) || (m_out == NULL) )
    { printf(" matrix_prod: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }


  if(MDim1(m_in) != MDim1(m_out))
    { printf(" matrix_prod: wrong dimensions of matrices\n");
      return( MATH_WARNING );
    }
  
  m = MDim1(m_out); n = MDim2(m_out);
  for( i = 0; i < m; i++ ) 
    {
      printf("\r line: %d\r",i);
      fflush(stdout);
      for( j = 0; j<=i; j++ )
	{
	  sum = 0.0;
	  for( k = 0; k < MDim2(m_in); k++ ) 
	    sum += M(m_in,i,k) * M(m_in,j,k);
	  M(m_out,i,j) = sum;
	  M(m_out,j,i) = sum;
	}
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
*   Function    : matrix_scalar
*   Goal        : 
*
*    
*   input parameters   : v1  : orignial matrix 
*
*   output parameters  : vres = v1 + v2
*
*   returnted Value    : error or success messages
*
********************************************************************/
extern int matrix_scalar( Matrix* mat, double scal, Matrix* matres )
{
  int i, j;
  double **vres, **val;

  if( (mat == NULL) || (matres == NULL) )
    { printf(" matrix_scalar: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }
  
  if( MDim1(mat) != MDim1(matres) || MDim2(mat) != MDim2(matres) )
    { printf(" matrix_scalar: mat and matres not compatible\n");
      return( MATH_FATAL_ERROR );
    }
  
  vres = matres->val;
  val  = mat->val;
  for( i = 0; i < MDim1(mat); i++ )
    for( j = 0; j < MDim2(mat); j++ )
      MVal( vres, i, j ) = scal * MVal( val, i, j);

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
*                        j    : number of column
*
*   output parameters  : m    : m with m[i] = v (column i is set to v)
*
*   returnted Value    : error or success messages
*
********************************************************************/
extern int matrix_setcol( Matrix *mat, int j, Vector *vec )
{
  int size;
  char   *mem_v, *mem_m;

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

  mem_m = (char*) mat->mem;
  mem_m += j * MDim1( mat ) * sizeof(double);
  mem_v = (char*) vec->mem;
  size = VDim( vec ) * sizeof(double);

  memcpy( mem_m, mem_v, size );

  return( MATH_SUCCESS );
}


/********************************************************************
*
*   Function    : matrix_setline
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
extern int matrix_setline( Matrix *mat, int i, Vector *vec ) 
{
  int    j, size;
  char   *mem_m, *mem_v;

  if( (mat == NULL) || (vec == NULL) )
    { printf(" matrix_setline: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( MDim2(mat) != VDim(vec) )
    { printf(" matrix_setline: wrong size of matrix or vector\n");
      return( MATH_WARNING );
    }

  if( (i < 0) || (i >= MDim1(mat)) )
    { printf(" matrix_setline: line %d does not exist ",j);
      return( MATH_WARNING );
    }

  for( j = 0; j < MDim2(mat); j++ )
    M(mat,i,j) = V(vec,j);
  
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

  if( (MDim1(m1) != MDim1(m2)) || (MDim1(m1) != MDim1(mres)) ||
      (MDim2(m1) != MDim2(m2)) || (MDim2(m1) != MDim2(mres)) )
    { printf(" matrix_sum: wrong dimensions of matrices\n");
      return( MATH_WARNING );
    }

  for( i = 0; i < MDim1(mres); i++ )
    for( j = 0; j < MDim2(mres); j++ )
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
  int ret;

  if( (mat == NULL) || (vec == NULL) || (vres == NULL) )
    { printf(" matrix_vector: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( (MDim1(mat) != VDim(vres)) || (MDim2(mat) != VDim(vec)) )
    { printf(" matrix_vector: wrong dimensions of matrice or vectors\n");
      return( MATH_WARNING );
    }
 
  ret = matrix_prod( mat, vec, vres );
  /*
  mval = mat->val;
  vval = vec->val;
  for( i = 0; i < MDim1(mat); i++ )
    { sum = 0.0;
      for( j = 0; j < MDim2(mat); j++ )
	sum += MVal(mval,i,j) * VVal(vval,j);
      V(vres,i) = sum;
    }
    */
  return( ret );
}

/********************************************************************
*
*   Function    : matrix_zero
*   Goal        : 
*
*    
*   input parameters   : mat : original matrix
*
*   output parameters  : none
*
*   returnted Value    : error or success messages
*
********************************************************************/
extern int matrix_zero( Matrix *mat )
{
  int dim_M, dim_N;

  if( mat == NULL )
    { printf(" matrix_zero: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  dim_M = MDim1( mat );
  dim_N = MDim2( mat );
  memset( (char*) mat->mem, 0, dim_M * dim_N * sizeof(double) );
  return( MATH_SUCCESS );
}
/*  -----------------------  end of file  ----------------------- */













