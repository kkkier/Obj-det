/************************************************************************
*
*       Modulename      : libMath.h
*
*       Author          : Bernt Schiele
*
*       Date            : 17. April 1994
*       Version         : 0.0
*       Goal            : 
*
*       Modifications   : author(s)     : 
*                         date          : 
*                         justification : 
*
************************************************************************/
#ifndef _libMath_h
#define _libMath_h

/*********************************************************************
*
*   liste of Includes   
*
********************************************************************/

/*********************************************************************
*
*   liste of constants
*
********************************************************************/
#define MATH_SUCCESS         0
#define MATH_WARNING        -1
#define MATH_FATAL_ERROR    -2

#define M( m, i, j )          (m)->val[j][i]
#define MVal( val, i, j )     val[j][i]
#define MDim1(m)              (m)->dim_M
#define MDim2(m)              (m)->dim_N

#define V(v,i)                (v)->val[0][i]
#define VVal(val,i)           val[0][i]
#define VDim( v )             (v)->dim_M


#define SIGN(a,b)  ( (b) >= 0.0 ? SYS_FABS(a) : - SYS_FABS(a) )

/*********************************************************************
*
*   Structures of data
*
********************************************************************/
/* typedef double Value; */

typedef struct Matrix
{
  int         dim_M;    /*   */
  int         dim_N;
  double      *mem;
  double      **val;
} Matrix, Vector;

/*********************************************************************
*
*   Functions implemented in that module
*
********************************************************************/
/* elmhes.c */
extern int      elmhes( Matrix *mat );

/* hpr.c */
extern int      hpr( Matrix *mat, Vector *w_real, Vector *w_imag );

/* jacobi.c */
extern int      jacobi( Matrix* mat, Vector *d, Matrix *eig_v );
extern void     eigsort( Matrix *eig_v, Vector *eig_w );

/* tred2_tqli.c */
extern int     eigen_householder(  Matrix *mat, Vector *d, Matrix *eig_v);

/* svdcmp.c */
extern int      svdcmp( Matrix *a, Matrix *u, Vector *w, Matrix *v );

/* solve_lin_system.c */
extern int     matrix_solve_lin_system( Matrix* A, Vector *b, Vector *x ); 

/* matrix.c */
extern Matrix * matrix_alloc ( int dim_M, int dim_N );
extern int      matrix_copy( Matrix* m1, Matrix* m2 );
extern int      matrix_diff( Matrix *m1, Matrix *m2, Matrix *mres );
extern int      vector_ene( Vector *v, int n, double* ene );
extern int      matrix_free( Matrix* m );
extern void     matrix_free_all( );
extern int      matrix_getcol( Matrix *mat, int j, Vector *vec ) ;
extern int      matrix_getline( Matrix *mat, int j, Vector *vec ) ;
extern int      matrix_norm( Matrix * mat , double* result );
extern int      matrix_out( Matrix* mat, char* comment );
extern int      matrix_prod( Matrix *m1, Matrix *m2, Matrix *mres );
extern int      matrix_prod_transpose( Matrix *m1, Matrix *mres );
extern int      matrix_transpose( Matrix *m, Matrix *mT );
extern int      matrix_scalar( Matrix *mat, double scal, Matrix* mres );
extern int      matrix_setcol( Matrix *m, int j, Vector *v );
extern int      matrix_setline( Matrix *mat, int j, Vector *vec ) ;
extern int      matrix_sum( Matrix *m1, Matrix *m2, Matrix *mres );
extern int      matrix_vector( Matrix* mat, Vector* vec, Vector* vres );
extern int      matrix_zero( Matrix *mat );

/* vector.c */
extern Vector  *vector_alloc ( int dim );
extern int     vector_copy( Vector* v1, Vector* v2 );
extern int     vector_diff( Vector* v1, Vector *v2, Vector *vres );
extern int     vector_free( Vector* v );
extern int     vector_norm( Vector* v, double *norm );
extern int     vector_out( Vector * vec, char* comment );
extern int     vector_prod( Vector *v1, Vector *v2, double *vres );
extern int     vector_scalar( Vector* vec, double scal, Vector* vres );
extern int vector_minmax( Vector* vec, 
			  double* min, double* max );
extern int     vector_sum( Vector *v1, Vector *v2, Vector *vres );
extern int     vector_zero( Vector *mat );

#endif

/*  -----------------------  end of file  ----------------------- */

