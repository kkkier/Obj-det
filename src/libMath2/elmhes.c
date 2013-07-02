/************************************************************************
*
*       Modulename      : elmhes.c
*
*       Author          : Bernt Schiele
*
*       Date            : 17 April 1994
*       Version         : 0.0
*       Goal            : Reduction of a (nonsymetric) matrix to 
*                         Hessenberg form by the elimination
*                         methode 
*
*                A Hessenberg form is:
*                  1. all values with i > j+1 are zero
*                  2. the rest non-zero: 'x'
*                  Ex: 5x5 matrix in Hessenbergform:
*                           x x x x x   
*                           x x x x x   
*                           0 x x x x 
*                           0 0 x x x 
*                           0 0 0 x x
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


#define SWAP(g,h)    { y = (g); (g) = (h); (h) = y; }

/*********************************************************************
*
*   Functions implemented in that module
*
********************************************************************/
extern int elmhes( Matrix *mat );

/*********************************************************************
*
*   Body of the functions
*
********************************************************************/


/********************************************************************
*
*   Function    : elmhes
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
extern int elmhes( Matrix *mat )
{
  int      m, n, i, j;
  double   x, y;
  double   **value;

  
  if( mat == NULL )
    { printf(" elmhes: you gave me a NULL-pointer\n");
      return( MATH_FATAL_ERROR );
    }

  if( MDim1(mat) != MDim2(mat) )
    { printf(" elmhes: not a quadratic matrix \n");
      return( MATH_WARNING );
    }

  n = MDim2(mat);
  value = mat->val;

  for( m = 1; m < n-1; m++ )
    {
      x = 0.0;
      i = m;
      for( j = m; j < n; j++ )     /* find the pivot */
	{ if( SYS_FABS( MVal(value,j,m-1)) > SYS_FABS( x ) )
	    { x = MVal(value,j,m-1);
	      i = j;
	    }
	}

      if( i != m )    /* interchange rows and columns */
	{ for( j = m-1; j < n; j++ ) SWAP( MVal(value,i,j), MVal(value,m,j) );
	  for( j = 0; j < n; j++ )   SWAP( MVal(value,j,i), MVal(value,j,m) );
	}

      if( x )
	{ for( i = m+1; i < n; i++ )
	    { if( (y = MVal(value,i,m-1)) != 0.0 )
		{ y /= x;
		  MVal(value,i,m-1) = y;
		  for( j = m; j < n; j++ ) MVal(value,i,j) -= y * MVal(value,m,j);
		  for( j = 0; j < n; j++ ) MVal(value,j,m) += y * MVal(value,j,i);
		}
	    }
	}
    }

  /* the values with i > j+1 are to be thought of as zero. */
  /* Up to now they are random values */
  for( i = 2; i < MDim1(mat); i++ ) 
    for( j = 0; j < i-1; j++ )
      M(mat,i,j) = 0.0;

  return( MATH_SUCCESS );
}

/*  -----------------------  end of file  ----------------------- */

