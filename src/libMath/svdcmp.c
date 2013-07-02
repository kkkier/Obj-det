/************************************************************************
*
*       Modulename      : svdcmp.c
*
*       Author          : Bernt Schiele
*
*       Date            : 20. April 1994
*       Version         : 0.0
*       Goal            : see {92PrTeVeFl} p 67ff
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
#include "sys_proto.h"
#include "sys_const.itf"
#include "libMath.h"


/*********************************************************************
*
*   global Variables of this module
*
********************************************************************/



/*********************************************************************
*
*   Functions implemented in that module
*
********************************************************************/
static double pythag( double a, double b );
extern int    svdcmp( Matrix *a, Matrix *u, Vector *w, Matrix *v );

/*********************************************************************
*
*   Body of the functions
*
********************************************************************/


/********************************************************************
*
*   Function    : pythag
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
static double pythag( double a, double b )
{
  double absa, absb;
  absa = SYS_FABS( a );
  absb = SYS_FABS( b );
  if( absa > absb )
    return( absa * sqrt( 1.0 + SYS_SQR( absb/absa )) );
  else
    return( (absb == 0.0 ? 0.0 : absb * sqrt( 1.0 + SYS_SQR( absa/absb ))) );
}


/********************************************************************
*
*   Function    : svdcmp
*   Goal        : single value decomposition : a = u * w * v^T
*
*    
*   input parameters   : a : the original matrix (m x n)
*
*   output parameters  : output of u (m x n)
*                        output of w (diagonal-matrix) in Vector w (n x 1)
*                        output of v (!not v^T) (n x n)
*
*   returnted Value    : success or error-messages
*
********************************************************************/
extern int svdcmp( Matrix *a, Matrix *u, Vector *w, Matrix *v )
{
  int     flag, i, its, j, jj, k, l, nm, n, m;
  double  anorm, c, f, g, h, s, scale, x, y, z;
  Vector  *rv1;

  if( (a == NULL) || (u == NULL) || (w == NULL) || (v == NULL) )
    { printf(" svdcmp: you gave me NULL-pointer !\n");
      return( MATH_FATAL_ERROR );
    }

  matrix_copy( a, u );
  m = MDim1( u );
  n = MDim2( u );

  if( (n != MDim1(v)) || (n != MDim2(v)) || (n != VDim(w)) )
    { printf(" svdcmp: wrong dimensions of matrices/vector \n");
      return( MATH_WARNING );
    }
    
  rv1 = vector_alloc( n );

  g = scale = anorm = 0.0;
  for( i = 0; i < n; i++ )
    { l = i + 1;
      V(rv1,i) = scale * g;
      g = s = scale = 0.0;
      if( i < m )
	{ for( k = i; k < m; k++ ) scale += SYS_FABS( M(u,k,i) );
	  if( scale )
	    { for( k = i; k < m; k++ ) 
		{ M(u,k,i) /= scale;
		  s += M(u,k,i) * M(u,k,i);
		}
	      f = M(u,i,i);
	      g = -SIGN(sqrt(s), f );
	      h = f * g - s;
	      M(u,i,i) = f - g;
	      for( j = l; j < n; j++ )
		{ for( s = 0.0, k = i; k < m; k++ ) 
		    s += M(u,k,i) * M(u,k,j);
		  f = s/h;
		  for( k = i; k < m; k++ )
		    M(u,k,j) += f * M(u,k,i);
		}
	      for( k = i; k < m; k++ ) M(u,k,i) *= scale;
	    }
	}
      V(w,i) = scale * g;
      g = s = scale = 0.0;
      if( (i < m ) && (i != n-1) )
	 { for( k = l; k < n; k++ ) scale += SYS_FABS( M(u,i,k) );
	   if( scale )
	     { for( k = l; k < n; k++ )
		 { M(u,i,k) /= scale;
		   s += M(u,i,k) * M(u,i,k);
		 }
	       f = M(u,i,l);
	       g = -SIGN( sqrt(s),f );
	       h = f * g - s;
	       M(u,i,l) = f - g;
	       for( k = l; k < n; k++ ) V(rv1,k) = M(u,i,k) / h;
	       for( j = l; j < m; j++ )
		 { for( s = 0.0, k = l; k < n; k++ ) 
		     s += M(u,j,k) * M(u,i,k);
		   for( k = l; k < n; k++ )
		     M(u,j,k) += s * V(rv1,k);
		 }
	       for( k = l; k < n; k++ ) M(u,i,k) *= scale;
	     }
	 }
      anorm = SYS_MAX( anorm, (SYS_FABS(V(w,i)) + SYS_FABS(V(rv1,i))) );
    }

  for( i = n-1; i >= 0; i-- )
    { if( i < n-1 )
	{ if( g )
	    { for( j = l; j < n; j++ )
		M(v,j,i) = ( M(u,i,j) / M(u,i,l) ) / g;
	      for( j = l; j < n; j++ )
		{ for( s = 0.0, k = l; k < n; k++ )
		    s += M(u,i,k) * M(v,k,j);
		  for( k = l; k < n; k++ )
		    M(v,k,j) += s * M(v,k,i);
		}
	    }
	  for( j = l; j < n; j++ )
	    M(v,i,j) = M(v,j,i) = 0.0;
	}
      M(v,i,i) = 1.0;
      g = V(rv1,i);
      l = i;
    }

  for( i = SYS_MIN( m-1, n-1 ); i >= 0; i-- )
    { l = i + 1;
      g = V(w,i);
      for( j = l; j < n; j++ ) M(u,i,j) = 0.0;
      if( g )
	{ g = 1.0/g;
	  for( j = l; j < n; j++ )
	    { for( s = 0.0, k = l; k < m; k++ )
		s += M(u,k,i) * M(u,k,j);
	      f = ( s/M(u,i,i) ) * g;
	      for( k =i; k < m; k++ )
		M(u,k,j) += f * M(u,k,i);
	    }
	  for( j = i; j < m; j++ ) M(u,j,i) *= g;
	}
      else
	{ for( j = i; j < m; j++ ) M(u,j,i) = 0.0;
	}
      M(u,i,i) += 1;
    }
  
  for( k = n-1; k >= 0; k-- )
    { for( its = 1; its <= 30; its++ )
	{ flag = 1;
	  for( l = k; k >= 0; l-- )
	    { nm = l - 1;
	      if( (double) (SYS_FABS(V(rv1,l)) + anorm) == anorm ) 
		{ flag = 0;
		  break;
		}
	      if( (double) (SYS_FABS(V(w,nm)) + anorm ) == anorm )
		break;
	    }
	  if( flag )
	    { c = 0.0;
	      s = 1.0;
	      for( i = l; i <= k; i++ )
		{ f = s * V(rv1,i);
		  V(rv1,i) = c * V(rv1,i);
		  if( (double) (SYS_FABS(f) + anorm) == anorm ) break;
		  g = V(w,i);
		  h = pythag( f,g );
		  V(w,i) = h;
		  h = 1.0/h;
		  c = g * h;
		  s = -f * h;
		  for( j = 0; j < m; j++ )
		    { y = M(u,j,nm);
		      z = M(u,j,i);
		      M(u,j,nm) = y * c + z * s;
		      M(u,j,i)  = z * c - y * s;
		    }
		}
	    }
	  z = V(w,k);
	  if( l == k )
	    { if( z < 0.0 )
		{ V(w,k) = -z;
		  for( j = 0; j < n; j++ ) M(v,j,k) = -M(v,j,k);
		}
	      break;
	    }
	  if( its == 30 ) 
	    { printf(" no convergence in 30 svdcmp iterations\n");
	      return( MATH_WARNING );
	    }
	  x = V(w,l);
	  nm = k-1;
	  y = V(w,nm);
	  g = V(rv1,nm);
	  h = V(rv1,k);
	  f = ( (y-z)*(y+z) + (g-h)*(g+h) ) / ( 2.0*h*y);
	  g = pythag(f,1.0);
	  f = ( (x-z)*(x+z) + h*( (y/(f+SIGN(g,f)))-h)) / x;
	  c = s = 1.0;
	  for( j = l; j <= nm; j++ )
	    { i = j+1;
	      g = V(rv1,i);
	      y = V(w,i);
	      h = s * g;
	      g = c * g;
	      z = pythag( f,h );
	      V(rv1,j) = z;
	      c = f/z;
	      s = h/z;
	      f = x*c + g*s;
	      g = g*c - x*s;
	      h = y*s;
	      y *= c;
	      for( jj = 0; jj < n; jj++ )
		{ x = M(v,jj,j);
		  z = M(v,jj,i);
		  M(v,jj,j) = x*c + z*s;
		  M(v,jj,i) = z*c - x*s;
		}
	      z = pythag( f,h );
	      V(w,j) = z;
	      if( z )
		{ z = 1.0/z;
		  c = f*z;
		  s = h*z;
		}
	      f = c*g + s*y;
	      x = c*y - s*g;
	      for( jj = 0; jj < m; jj++ )
		{ y = M(u,jj,j);
		  z = M(u,jj,i);
		  M(u,jj,j) = y*c + z*s;
		  M(u,jj,i) = z*c - y*s;
		}
	    }
	  V(rv1,l) = 0.0;
	  V(rv1,k) = f;
	  V(w,k)   = x;
	}
    }
  vector_free( rv1 );
  return( MATH_SUCCESS );
}


/*  -----------------------  end of file  ----------------------- */

