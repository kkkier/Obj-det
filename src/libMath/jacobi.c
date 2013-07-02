/************************************************************************
*
*       Modulename      : jacobi.c
*
*       Author          : Bernt Schiele
*
*       Date            : 19 April 1994
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

#define ROTATE(a, i, j, k, l) { g = MVal(a,i,j) ; h = MVal(a,k,l) ; \
				MVal(a,i,j) = g - s*(h + g * tau); \
				MVal(a,k,l) = h + s*(g - h * tau); }

/*********************************************************************
*
*   Functions implemented in that module
*
********************************************************************/
extern int jacobi ( );
extern void eigsort ();


/*********************************************************************
*
*   Body of the functions
*
********************************************************************/


/********************************************************************
*
*   Function    : eigsort
*   Goal        : 
*
*    
*   input parameters   : eig_v : eigenvectors as a matrix
*                        eig_w : eigenvalues as a vector
*
*   output parameters  : eig_v : the sorted eigenvectors
*                        eig_w : the sorted eigenvalues
*
*   returnted Value    : none
*
********************************************************************/
extern void eigsort( Matrix *eig_v, Vector *eig_w )
{
  int      i, k, j;
  double   p;
  int      n = VDim( eig_w );

  for( i = 0; i < n; i++ )
    { k = i;
      p = V(eig_w,k);
      for( j = i+1; j < n; j++ )
	if( V(eig_w,j) >= p ) 
	  { k = j;
	    p = V( eig_w, k );
	  }
      if( k != i )
	{ V(eig_w,k) = V(eig_w,i);
	  V(eig_w,i) = p;
	  for( j = 0; j < n; j++ )
	    { p            = M(eig_v,j,i);
	      M(eig_v,j,i) = M(eig_v,j,k);
	      M(eig_v,j,k) = p;
	    }
	}
    }
}

/********************************************************************
*
*   Function    : jacobi
*   Goal        : 
*
*    
*   input parameters   : mat   : original Matrix
*
*   output parameters  : d     : contains the eigenvalues
*                        eig_v : contains the eigenvectors as columna
*
*   returnted Value    : success or error messages
*
********************************************************************/
extern int jacobi( Matrix* mat, Vector *d, Matrix *eig_v )
{
  int    j, iq, ip, i, n, nrot;
  double tresh, theta, tau, t, sm, s, h, g, c;
  Vector *b, 
         *z;       /* this vector accumulate terms */
  double **mval, **vval, **bval, **dval, **zval;
  Matrix *msave;

  if( (mat == NULL) || (d == NULL) || (eig_v == NULL) )
    { printf(" jacobi: you gave me a NULL-pointer\n");
      return( MATH_FATAL_ERROR );
    }

  if( (MDim1(mat) != MDim2(mat)) || (VDim(d) != MDim1(mat)) ||
      (MDim1(eig_v) != MDim2(eig_v)) || (MDim1(mat) != MDim1(eig_v)) )
    { printf(" jacobi: wrong dimensions of the matrices or the vector\n");
      return( MATH_WARNING );
    }
  
  msave = matrix_alloc( MDim1(mat), MDim2(mat) );
  matrix_copy( mat, msave );

  n = MDim1(msave);
  mval = msave->val;
  vval = eig_v->val;
  dval = d->val;
  b = vector_alloc( n );   bval = b->val;
  z = vector_alloc( n );   zval = z->val;

  /* Initialize */
  for( ip = 0; ip < n; ip++ )      /* initialize to the identiy matrix */
    { for( iq = 0; iq < n; iq ++ )
	MVal(vval,ip,iq) = 0.0;
      MVal(vval,ip,ip) = 1.0;
    }
  for( ip = 0; ip < n; ip++ )              /* initialize b and d to the */
    { VVal(bval,ip) = VVal(dval,ip) = MVal(mval,ip,ip) ; /* diagonal of msave */
      VVal(zval,ip) = 0.0;
    }
  nrot = 0;

  for( i = 0; i < 50; i++ )
    { 
      sm = 0.0;
      for( ip = 0; ip < n-1; ip++) 
	for( iq = ip+1; iq < n; iq++) sm += SYS_FABS(MVal(mval,ip,iq));

      if( sm == 0.0) 
	{ vector_free( b );
	  vector_free( z );
	  matrix_free( msave );
	  eigsort( eig_v, d );
	  return( MATH_SUCCESS );
	}

      if (i < 4) tresh = 0.2 * sm / (n * n);
      else       tresh = 0.0;

      for( ip = 0; ip < n-1;ip++) 
	for( iq = ip+1; iq < n; iq++) 
	  { g = 100.0 * SYS_FABS( MVal(mval,ip,iq) );

	    if ( (i > 4) && 
		((double) SYS_FABS(VVal(dval,ip))+g)==(double) SYS_FABS(VVal(dval,ip)) &&
		((double) SYS_FABS(VVal(dval,iq))+g)==(double) SYS_FABS(VVal(dval,iq)) )
	      { MVal(mval,ip,iq) = 0.0;
	      }
	    else if( SYS_FABS( MVal(mval,ip,iq) ) > tresh )
	      { h = VVal(dval,iq) - VVal(dval,ip);
                if( (double) (SYS_FABS(h) + g) == (double)SYS_FABS(h)) 
		  { t = ( MVal(mval,ip,iq) ) / h;
		  }
                else 
		  {  theta = 0.5 * h / ( MVal(mval,ip,iq) );
                     t     = 1.0 / (SYS_FABS(theta) + sqrt(1.0 + theta*theta));
                     if( theta < 0.0 ) t = -t;
		   }
                c   = 1.0 / sqrt( 1 + t*t);
                s   = t * c;
                tau = s / (1.0 + c);
                h   = t * MVal(mval,ip,iq);
                VVal(zval,ip) -= h;
                VVal(zval,iq) += h;
                VVal(dval,ip) -= h;
                VVal(dval,iq) += h;
                MVal( mval,ip,iq) = 0.0;

                for (j = 0;    j < ip; j++) ROTATE( mval,j, ip,j, iq);
                for (j = ip+1; j < iq; j++) ROTATE( mval,ip,j, j, iq);
                for (j = iq+1; j < n;  j++) ROTATE( mval,ip,j, iq,j);
                for (j = 0;    j < n;  j++) ROTATE( vval,j, ip,j, iq);
                ++nrot;
           }
        }
      
      for( ip = 0; ip < n; ip++ )
	{ VVal(bval,ip) += VVal(zval,ip);
	  VVal(dval,ip)  = VVal(bval,ip);
	  VVal(zval,ip)  = 0.0;
	}
    }
  printf(" jacobi: to many iterations in routine\n");
  return( MATH_WARNING );
}



/*  -----------------------  end of file  ----------------------- */



