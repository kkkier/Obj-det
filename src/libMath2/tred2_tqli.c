/************************************************************************
*
*       Modulename      : tqli.c
*
*       Author          : Bernt Schiele
*
*       Date            : June 1996
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
#include "sys_const.itf"
#include "sys_proto.h"
#include "libMath.h"


/*********************************************************************
*
*   global Variables of this module
*
********************************************************************/

/* #define SIGN(a,b) ((b)<0 ? -fabs(a) : fabs(a)) */

/*********************************************************************
*
*   Functions implemented in that module
*
********************************************************************/
static int tqli( double d[], double e[], int n, Matrix *z);
static int tred2( Matrix *a, int n, double d[], double e[]);

extern int eigen_householder( Matrix *mat, Vector *eig_val, 
			      Matrix *eig_vec)
{
  double *d, *e;
  int    i, n, ret;

  if( mat == NULL || eig_val == NULL || eig_vec == NULL )
    { printf("eigen_householder: NUL-pointer\n");
      return( MATH_FATAL_ERROR );
    }
  printf("inside householder\n"); fflush(stdout);
  n = MDim1( mat );
  if( MDim2( mat ) != n     || VDim( eig_val ) != n ||
      MDim1( eig_vec ) != n || MDim2( eig_vec ) != n )
    { printf(" eigen_householder: dim-problems\n" );
      return( MATH_FATAL_ERROR );
    }

  d = (double*) malloc( sizeof(double) * n );
  e = (double*) malloc( sizeof(double) * n );

  if( d == NULL || e == NULL )
    { printf(" eigen_householder: no Memory\n" );
      return( MATH_FATAL_ERROR );
    }
  
  printf("tred2\n"); fflush(stdout);
  if( (ret = tred2( mat, n, d, e )) != MATH_SUCCESS ) return( ret );
  printf("tqli\n"); fflush(stdout);
  if( (ret = tqli( d, e, n, mat )) != MATH_SUCCESS )  return( ret );

  matrix_copy( mat, eig_vec );
  for( i = 0; i < n; i++ ) V( eig_val, i ) = d[i];
  free( d );
  free( e );
  eigsort( eig_vec, eig_val );
  
  return( MATH_SUCCESS );
}

static int tqli( double d[], double e[], int n, Matrix *z)
{
  int     m, l, iter, i, k;
  double  s, r, p, g, f, dd, c, b;

  for( i=1; i < n; i++) e[i-1]=e[i];
  e[n-1] = 0.0;

  for( l=0; l<n; l++ ) 
    {
      iter = 0;
      do 
	{
	  for( m = l; m < n-1; m++ ) 
	    { dd = SYS_FABS( d[m] ) + SYS_FABS( d[m+1] );
	      if( SYS_FABS( e[m] ) + dd == dd ) break;
	    } 
	  if (m != l) 
	    {
	      if (iter++ == 30) 
		{ printf(" Too many iterations in TQLI");
		  return( MATH_FATAL_ERROR );
		}
	      g=(d[l+1]-d[l])/(2.0*e[l]);
	      r=sqrt((g*g)+1.0);
	      g=d[m]-d[l]+e[l]/(g+SIGN(r,g));
	      s=c=1.0;
	      p=0.0;
	      for( i = m-1; i >= l; i--) 
		{
		  f=s*e[i];
		  b=c*e[i];
		  if (SYS_FABS(f) >= SYS_FABS(g)) 
		    { c=g/f;
		      r=sqrt((c*c)+1.0);
		      e[i+1]=f*r;
		      c *= (s=1.0/r);
		    } 
		  else 
		    { s=f/g;
		      r=sqrt((s*s)+1.0);
		      e[i+1]=g*r;
		      s *= (c=1.0/r);
		    }
		  g=d[i+1]-p;
		  r=(d[i]-g)*s+2.0*c*b;
		  p=s*r;
		  d[i+1]=g+p;
		  g=c*r-b;
		  
		  /* Next loop can be omitted if eigenvectors not wanted */
		  for( k = 0; k < n; k++) 
		    { f = M(z,k,i+1);
		      M(z,k,i+1) = s * M(z,k,i) + c*f;
		      M(z,k,i)   = c * M(z,k,i) - s*f;
		    }
		}
	      
	      d[l] = d[l] - p;
	      e[l] = g;
	      e[m] = 0.0;
	    }
	} while (m != l);
    }
  return( MATH_SUCCESS );
}


static int tred2( Matrix *a, int n, double d[], double e[])
{
  int    l, k, j, i;
  double scale, hh, h, g, f;
  
  for( i = n-1; i >= 1; i-- ) 
    {
      l=i-1;
      h = scale = 0.0;
      if (l > 0) 
	{ for( k = 0; k <= l; k++)
	    scale += SYS_FABS( M(a,i,k) );
	  if (scale == 0.0)
	    e[i] = M(a,i,l);
	  else {
	    for( k = 0;k <= l; k++ ) 
	      {
		M(a,i,k) /= scale;
		h += M(a,i,k) * M(a,i,k);
	      }
	    f = M(a,i,l);
	    g = f>0 ? -sqrt(h) : sqrt(h);
	    e[i]=scale*g;
	    h -= f*g;
	    M(a,i,l) = f-g;
	    f=0.0;
	    for( j = 0;j <= l; j++ ) 
	      {
		/* Next statement can be omitted if eigenvectors not wanted */
		M(a,j,i) = M(a,i,j) / h;
		g=0.0;
		for( k = 0; k <= j; k++)
		  g += M(a,j,k) * M(a,i,k);
		for( k = j+1; k<=l; k++)
		  g += M(a,k,j)* M(a,i,k);
		e[j]=g/h;
		f += e[j] * M(a,i,j);
	      }
	    hh=f/(h+h);
	    for( j = 0; j <= l; j++) 
	      {
		f = M(a,i,j);
		e[j]=g=e[j]-hh*f;
		for( k = 0; k<=j ;k++ )
		  M(a,j,k) -= (f * e[k] + g * M(a,i,k) );
	      }
	  }
      } else
	e[i] = M(a,i,l);
      d[i]=h;
    }
  /* Next statement can be omitted if eigenvectors not wanted */
  d[0]=0.0;
  e[0]=0.0;
  /* Contents of this loop can be omitted if eigenvectors not
     wanted except for statement d[i]=a[i][i]; */
  for( i = 0; i < n; i++) 
    {
      l = i-1;
      if (d[i]) {
	for( j = 0; j<=l; j++) 
	  { g = 0.0;
	    for( k = 0; k <= l; k++ )
	      g += M(a,i,k) * M(a,k,j);
	    for( k = 0; k <= l; k++ )
	      M(a,k,j) -= g * M(a,k,i);
	  }
      }
      d[i] = M(a,i,i);
      M(a,i,i) = 1.0;
      for( j = 0; j <= l; j++ ) M(a,j,i) = M(a,i,j) = 0.0;
    }

  return( MATH_SUCCESS );
}
