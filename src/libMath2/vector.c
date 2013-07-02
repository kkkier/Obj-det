/************************************************************************
*
*       Modulename      : vector.c
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
#include "sys_const.itf"
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
extern Vector * vector_alloc ();
extern int      vector_copy ();
extern int      vector_diff ();
extern int      vector_free ();
extern int      vector_out ();
extern int      vector_prod ();
extern int      vector_scalar ( Vector*, double, Vector* );
extern int      vector_sum ();



/*********************************************************************
*
*   Body of the functions
*
********************************************************************/


/********************************************************************
*
*   Function    : vector_alloc 
*   Goal        : 
*
*    
*   input parameters   : dim : number of rows
*
*   output parameters  : 
*
*   returnted Value    :
*
********************************************************************/
extern Vector *vector_alloc ( int dim )
{
  return( matrix_alloc( dim, 1 ) );
}


/********************************************************************
*
*   Function    : vector_copy
*   Goal        : 
*
*    
*   input parameters   : v1 : original vector (source)
*
*   output parameters  : v2 : will be overwrited by v1 (destination)
*                             must be allready allocated
*
*   returnted Value    : success or error-value
*
********************************************************************/
extern int vector_copy( Vector* v1, Vector* v2 )
{
  int  size;
  char *mem_1, *mem_2;

  if( (v1 == NULL) || (v2 == NULL) )
    { printf(" vector_copy: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( VDim(v1) != VDim(v2) )
    { printf(" vector_copy: vectors have not the same size\n");
      return( MATH_WARNING );
    }

  mem_1 = (char*) v1->mem;
  mem_2 = (char*) v2->mem;
  size = VDim(v1) * sizeof(double);
  memcpy(mem_2,mem_1,size);
  /* 
  for( i = 0; i < VDim(v2); i++ )
    V(v2,i) = V(v1,i);
  */
  return( MATH_SUCCESS );
}


/********************************************************************
*
*   Function    : vector_diff
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
extern int vector_diff( Vector* v1, Vector *v2, Vector *vres )
{
  int i;

  if( (v1 == NULL) || (v2 == NULL) || (vres == NULL) )
    { printf(" vector_diff: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( (VDim(v1) != VDim(v2)) || (VDim(v1) != VDim(vres)) )
    { printf(" vector_diff: vectors have not the same size\n");
      return( MATH_WARNING );
    }
  
  for( i = 0; i < VDim(v1); i++ )
    V(vres,i) = V(v1,i) - V(v2,i);

  return( MATH_SUCCESS );
}



/********************************************************************
*
*   Function    : vector_ene
*   Goal        : 
*
*    
*   input parameters   : v: original vectors
*                        n: use only the first n dimensions
*
*   output parameters  : ene = v*v (scalarproduct - energie of vector)
*
*   returnted Value    : success or error messages
*
********************************************************************/
extern int vector_ene( Vector *v, int n, double* ene )
{
  int i, max_i;
  double sum = 0.0;

  if( v == NULL )
    { printf(" vector_ene: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }
  
  max_i = SYS_MIN( VDim(v), n );
  for( i = 0; i < max_i; i++ )
    sum += V(v,i) * V(v,i);
  
  *ene = sum;
  return( MATH_SUCCESS );
}
/********************************************************************
*
*   Function    : vector_free
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
extern int vector_free( Vector* v )
{
  return( matrix_free( v ) );
}

/********************************************************************
*
*   Function    : vector_norm
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
extern int vector_norm( Vector *vec, double *norm )
{
  int i;
  double sum = 0.0;

  if( vec == NULL )
    { printf("!! vector_norm: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }
  
  for( i = 0; i < VDim(vec); i++ ) sum += SYS_SQR( V(vec,i) );
  *norm = sqrt( sum );
  return( MATH_SUCCESS );
}
/********************************************************************
*
*   Function    : vector_out
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
extern int vector_out( Vector * vec, char* comment )
{ 
  int i;

  if( vec == NULL )
    { printf("!! vector_out: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }
  printf(" vector_out: %s \n",comment);
  for( i = 0; i < VDim(vec); i++ )
    printf(" %f , ",V(vec,i));
  printf("\n\n");
  
  return( MATH_SUCCESS );
}


/********************************************************************
*
*   Function    : vector_prod
*   Goal        : 
*
*    
*   input parameters   : v1, v2 : original vectors
*
*   output parameters  : vres = v1 * v2 (scalarproduct)
*
*   returnted Value    : success or error messages
*
********************************************************************/
extern int vector_prod( Vector *v1, Vector *v2, double* vres )
{
  int    i;
  double sum;

  if( (v1 == NULL) || (v2 == NULL) || (vres == NULL ))
    { printf(" vector_prod: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( VDim(v1) != VDim(v2) )
    { printf(" vector_prod: wrong dimensions of vectors\n");
      return( MATH_WARNING );
    }

  sum = 0.0;
  for( i = 0; i < VDim(v1); i++ )
    sum += V(v1,i) * V(v2,i);
	
  *vres = sum;

  return( MATH_SUCCESS );
}

/********************************************************************
*
*   Function    : vector_scalar
*   Goal        : 
*
*    
*   input parameters   : v1  : orignial vector 
*
*   output parameters  : vres = v1 + v2
*
*   returnted Value    : error or success messages
*
********************************************************************/
extern int vector_scalar( Vector* vec, double scal, Vector* vres )
{
  int i;

  if( (vec == NULL) || (vres == NULL) )
    { printf(" vector_scalar: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }
  
  for( i = 0; i < VDim(vec); i++ )
    V(vres,i) = scal * V(vec,i);

  return( MATH_SUCCESS );
}

/********************************************************************
*
*   Function    : vector_minmax
*   Goal        : 
*
*    
*   input parameters   : v1  : orignial vector 
*
*   output parameters  : min and max element
*
*   returnted Value    : error or success messages
*
********************************************************************/
extern int vector_minmax( Vector* vec, 
			  double* min, double* max )
{
  int i;

  if( vec == NULL )
    { printf(" vector_minmax: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }
  
  *min = *max = V(vec, 0);
  for( i = 1; i < VDim(vec); i++ )
    {
      *min = SYS_MIN( *min, V(vec,i));
      *max = SYS_MAX( *max, V(vec,i));
    }

  return( MATH_SUCCESS );
}

/********************************************************************
*
*   Function    : vector_sum
*   Goal        : 
*
*    
*   input parameters   : v1, v2 : original matrices
*
*   output parameters  : vres = v1 + v2
*
*   returnted Value    : error or success messages
*
********************************************************************/
extern int vector_sum( Vector *v1, Vector *v2, Vector *vres )
{
  int i;

  if( (v1 == NULL) || (v2 == NULL) || (vres == NULL) )
    { printf(" vector_sum: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }

  if( (VDim(v1) != VDim(vres)) || (VDim(v1) != VDim(v2)) )
    { printf(" vector_sum: wrong dimensions of vectors\n");
      return( MATH_WARNING );
    }

  for( i = 0; i < VDim(vres); i++ )
    V(vres,i) = V(v1,i) + V(v2,i) ;

  return( MATH_SUCCESS );
}


/********************************************************************
*
*   Function    : vector_zero
*   Goal        : 
*
*    
*   input parameters   : vec : original vector
*
*   output parameters  : none
*
*   returnted Value    : error or success messages
*
********************************************************************/
extern int vector_zero( Vector *vec )
{
  int ret;

  if( vec == NULL )
    { printf(" vector_zero: you gave me NULL-pointer!!! \n");
      return( MATH_FATAL_ERROR );
    }
  
  ret = matrix_zero( vec );
  return( ret );
}
/*  -----------------------  end of file  ----------------------- */














