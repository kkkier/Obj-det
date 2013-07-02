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
extern int tqli( double d[], double e[], int n, Matrix *z)
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
	      if (iter++ == 30) nrerror("Too many iterations in TQLI");
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
}
