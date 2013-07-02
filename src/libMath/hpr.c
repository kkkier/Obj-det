/************************************************************************
*
*       Modulename      : hpr.c
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


/*********************************************************************
*
*   liste of Includes   
*
********************************************************************/
#include <stdio.h>
#include <math.h>
#include "sys_const.itf"
#include "libMath.h"
#include "sys_proto.h"


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



/*********************************************************************
*
*   Body of the functions
*
********************************************************************/


/********************************************************************
*
*   Function    : hqr
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
extern int hpr( Matrix *mat, Vector *w_real, Vector *w_imag )
{
  int    n, nn, m, l, k, j, its, i, mmin;
  double p, q, r, s, t, u, v, w, x, y, z, anorm;
  double **val;

  if( (mat == NULL) || (w_real == NULL) || (w_imag == NULL) )
    { printf(" hpr: you gave me a NULL-pointer\n");
      return( MATH_FATAL_ERROR );
    }

  if( (MDim1(mat) != MDim2(mat)) || (MDim1(w_real) != MDim1(mat)) ||
      (MDim1(w_imag) != MDim1(mat)) )
    { printf(" hpr: wrong dimensions of the matrix or the vectors\n");
      return( MATH_WARNING );
    }

  n = MDim1(mat);
  val = mat->val;

  anorm = SYS_FABS( MVal(val,0,0) );
  for( i = 1; i < n; i++ )
    for( j = (i-1); j < n; j++ )
      anorm += SYS_FABS( MVal(val,i,j) );

  nn = n-1;
  t = 0.0;
  while( nn >= 0 )
    { 
      its = 0;
      do{ 
	  for( l = nn; l >=1; l-- )
	    { s = SYS_FABS( MVal(val,l-1,l-1) ) + SYS_FABS( MVal(val,l,l) );
	      if( s == 0.0 ) s = anorm;
	      if( (double) (SYS_FABS( MVal(val,l,l-1)) + s ) == s ) break;
	    }
	  x = MVal(val,nn,nn);
	  if( l == nn )
	    { w_real->MVal(val,0,nn) = x + t;
	      w_imag->MVal(val,0,nn) = 0.0;
	      nn--;
	    }
	  else
	    { y = MVal(val,nn-1,nn-1);
	      w = MVal(val,nn,nn-1) * MVal(val,nn-1,nn);
	      if( l == (nn-1) )
		{ p = 0.5 * (y-x);
		  q = p*p + w;
		  z = sqrt( SYS_FABS( q ) );
		  x += t;
		  if( q >= 0.0 )  /* real pair */
		    { z = p + SIGN( z, p );
		      w_real->MVal(val,0,nn-1) = w_real->MVal(val,0,nn) = x + z;
		      if( z ) w_real->MVal(val,0,nn) = x - w/z;
		      w_imag->MVal(val,0,nn-1) = w_imag->MVal(val,0,nn) = 0.0;
		    }
		  else            /* complex pair */
		    { w_real->MVal(val,0,nn-1) = w_real->MVal(val,0,nn) = x + p;
		      w_imag->MVal(val,0,nn-1) = -( w_imag->MVal(val,0,nn) = z );
		    }
		  nn -= 2;
		}
	      else
		{ if( its == 30 ) 
		    { printf(" hpr: too many iterations\n");
		      return( MATH_WARNING );
		    }
		  if( (its == 10) || (its == 20) )
		    { t += x;
		      for( i = 0; i <= nn; i++ ) MVal(val,i,i) -= x;
		      s = SYS_FABS( MVal(val,nn,nn-1) ) + SYS_FABS( MVal(val,nn-1,nn-2) );
		      y = x = 0.75 * s;
		      w = -0.4375 * s * s;
		    }
		  ++its;
		  for( m = (nn-2); m >= 0; m-- )
		    { z = MVal(val,m,m);
		      r = x - z;
		      s = y - z;
		      p = ( r * s - w )/ MVal(val,m+1,m) + MVal(val,m,m+1);
		      q = MVal(val,m+1,m+1) - z - r - s;
		      r = MVal(val,m+2,m+1);
		      s = SYS_FABS( p ) + SYS_FABS( q ) + SYS_FABS( r );
		      p /= s;
		      q /= s;
		      r /= s;
		      if( m == 0 ) break;
		      u = SYS_FABS( MVal(val,m,m-1) ) * (SYS_FABS( q ) + SYS_FABS( r ));
		      v = SYS_FABS( p ) * ( SYS_FABS( MVal(val,m-1,m-1)) + SYS_FABS( z ) +
				        SYS_FABS( MVal(val,m+1,m+1)));
		      if( (double) (u + v) == v) break;
		    }
		  for( i = m+2; i <= nn; i++ )
		    { MVal(val,i,i-2) = 0.0;
		      if( i != (m+2)) MVal(val,i,i-3) = 0.0;
		    }
		  for( k = m; k <= nn-1; k++ )
		    { if( k!= m )
			{ p = MVal(val,k,k-1);
			  q = MVal(val,k+1,k-1);
			  r = 0.0;
			  if( k != (nn-1) ) r = MVal(val,k+2,k-1);
			  if( (x = SYS_FABS( p ) + SYS_FABS( q ) + SYS_FABS( r )) != 0.0 )
			    { p /= x;
			      q /= x;
			      r /= x;
			    }
			}
		      if( (s = SIGN( sqrt( p*p + q*q + r*r), p)) != 0.0 )
			{ if( k == m )
			    { if( l != m ) MVal(val,k,k-1) = -MVal(val,k,k-1);
			    }
			  else
			    MVal(val,k,k-1) = -s*x;
			    
			  p += s;
			  x = p/s;
			  y = q/s;
			  z = r/s;
			  q /= p;
			  r /= p;
			  for( j = k; j <= nn; j++ )
			    { p = MVal(val,k,j) + q * MVal(val,k+1,j);
			      if( k!= (nn-1) )
				{ p += r * MVal(val,k+2,j);
				  MVal(val,k+2,j) -= p*z;
				}
			      MVal(val,k+1,j) -= p*y;
			      MVal(val,k,j)   -= p*x;
			    }
			  mmin = nn < k+3 ? nn : k+3;
			  for( i = 0; i <= mmin; i++ )
			    { p = x * MVal(val,i,k) + y * MVal(val,i,k+1);
			      if( k != (nn-1) )
				{ p += z * MVal(val,i,k+2);
				  MVal(val,i,k+2) -= p * r;
				}
			      MVal(val,i,k+1) -= p * q;
			      MVal(val,i,k) -= p;
			    }
			}
		    }
		}
	    }
	} while( l < nn -1 );
    }
  return( MATH_SUCCESS );
}



/*  -----------------------  end of file  ----------------------- */

