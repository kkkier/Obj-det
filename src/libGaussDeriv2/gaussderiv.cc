/*********************************************************************/
/*                                                                   */
/* FILE         gaussderiv.cc                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Recursive implementation of Gaussian derivatives     */
/*              with Deriche's method. This code was taken from my   */
/*              Diploma thesis and adapted for the pccv GrayImage    */
/*              class.                                               */
/*                                                                   */
/* BEGIN        WED Aug 22 2001                                      */
/* LAST CHANGE  WED Aug 22 2001                                      */
/*                                                                   */
/*********************************************************************/

/*************************************************************/
/* Recursive Implementation for Gaussian derivatives.        */
/* Algorithm as described in                                 */
/*   Rachid Deriche,                                         */
/*   "Recursively Implementing the Gaussian and its          */
/*   Derivatives",                                           */
/*   INRIA Sophia Antipolis Technical Report 1893, 1993.     */
/*************************************************************/

/*************************************************************/
/* 1.  For each filter type (g, fdg, sdg), do the following: */
/*     (g=Gaussian, fdg=first derivative, sdg=second deriv.) */
/*                                                           */
/* 1.1 Set the coefficients a0, a1, b0, b1, c0, c1, w0, w1.  */
/*                                                           */
/* 1.2 Calculate n00, n11, n22, n33, d11, d22, d33, d44.     */
/*                                                           */
/* 1.3 Set n11m, n22m, n33m, n44m, d11m, d22m, d33m, d44m    */
/*                                                           */
/* 1.4 For each row I(0,j),...,I(N-1,j), calculate the       */
/*     vectors (y0p,...,yN-1p), (yN-1m,...,y0m), and from    */
/*     them the row (y0,...,yN-1) of the result image X      */
/*     using the x-dir filter type on the input image.       */
/*                                                           */
/* 1.5 For each column I(i,0),...,I(i,N-1), calculate the    */
/*     vectors (x0p,...,xN-1p), (xN-1m,...,x0m), and from    */
/*     them the column (x0,...,xN-1)T of the result image    */
/*     using the y-dir filter type on the result image X.    */
/*************************************************************/

/*************************************************************/
/* 2.  Repeat that for the following combinations            */
/*       G(x,y)   =   g_X(x,y) *   g_Y(x,y)                  */
/*       Gx(x,y)  = fdg_X(x,y) *   g_Y(x,y)                  */
/*       Gy(x,y)  =   g_X(x,y) * fdg_Y(x,y)                  */
/*       Gxx(x,y) = sdg_X(x,y) *   g_Y(x,y)                  */
/*       Gxy(x,y) = fdg_X(x,y) * fdg_Y(x,y)                  */
/*       Gyy(x,y) =   g_X(x,y) * sdg_Y(x,y)                  */
/*************************************************************/


/****************/
/*   Includes   */
/****************/
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include <grayimage.hh>

#include "gaussderiv.hh"


/*******************/
/*   Definitions   */
/*******************/
#define GAUSSDERIV_GAUSSIAN    1
#define GAUSSDERIV_FIRSTDERIV  2
#define GAUSSDERIV_SECONDDERIV 3

#define GAUSSDERIV_DIR_X       1
#define GAUSSDERIV_DIR_Y       2


/*========================================================================*/
/*                  ApplyFilter for a GrayImage image class               */
/*========================================================================*/

bool ApplyFilter(int filter_type, int filter_dir, double sigma,
								 const GrayImage &image, GrayImage &res)
{
  /* additional variables used in the calculations */
  double  a0, a1, b0, b1, c0, c1, w0, w1;
  double  n00p, n11p, n22p, n33p, d11p, d22p, d33p, d44p;
  double  n11m, n22m, n33m, n44m, d11m, d22m, d33m, d44m;

  /* often used terms */
  double  sw0, sw1, cw0, cw1;   /* stands for sin(w0/sigma), cos(w0/sigma) */
  double  eb0, eb1, e2b0, e2b1; /* stands for exp(-b0/sigma),...           */
  double  eb0b1, e2b0b1, e2b1b0; /* stands for eb0*eb1,...                 */


  /* result vectors */
  double *res_p;
  double *res_m;

  /* index variables */
  int     x, y, idx, idxp, idxm, ip, im;

  /* normalization variables */
  bool    normalize = true;
  double  norm_g, norm_fdg, norm_sdg, norm_fact;

  /* reserve space for the result vectors */
  res_p = (double *) malloc ((image.width()+image.height())*sizeof(double)); 
  res_m = (double *) malloc ((image.width()+image.height())*sizeof(double)); 
  if ( (res_p==NULL) || (res_m==NULL) ) {
    std::cerr << "Error in ApplyFilter(): Out of memory!" << std::endl;
    return false;
  }

  /*************************************************************/
  /* Step 1.1: Set the coefficients a0, a1,...                 */
  /*************************************************************/
  switch(filter_type) {
  case GAUSSDERIV_GAUSSIAN: {
    a0 = 1.68;
    a1 = 3.735;
    b0 = 1.783;
    b1 = 1.723;
    c0 = -0.6803;
    c1 = -0.2598;
    w0 = 0.6318;
    w1 = 1.997;
  } break;
  
  case GAUSSDERIV_FIRSTDERIV: {
    a0 = -0.6472;
    a1 = -4.531;
    b0 = 1.527;
    b1 = 1.516;
    c0 = 0.6494;
    c1 = 0.9557;
    w0 = 0.6719;
    w1 = 2.072;
  } break;
  
  case GAUSSDERIV_SECONDDERIV: {
    a0 = -1.331;
    a1 = 3.661;
    b0 = 1.24;
    b1 = 1.314;
    c0 = 0.3225;
    c1 = -1.738;
    w0 = 0.748;
    w1 = 2.166;
  } break;
  
  default:
    std::cerr << "Error in ApplyFilter(): Unknown filter type: " << filter_type 
	 << std::endl;
    a0 = a1 = b0 = b1 = c0 = c1 = w0 = w1 = 0.0;
    return false;
  }

  /* Initialize some often used terms: */
  sw0 = sin(w0/sigma);
  sw1 = sin(w1/sigma);
  cw0 = cos(w0/sigma);
  cw1 = cos(w1/sigma);
  eb0 = exp(-b0/sigma);
  eb1 = exp(-b1/sigma);
  e2b0 = eb0*eb0;
  e2b1 = eb1*eb1;
  eb0b1 = eb0*eb1;
  e2b0b1= e2b0*eb1;
  e2b1b0= e2b1*eb0;
  
  /*************************************************************/
  /* Step 1.2: Calculate n00, n11,...                          */
  /*************************************************************/
  n00p = a0 + c0;
  n11p = eb1*(c1*sw1 - (c0+2.0*a0)*cw1) + eb0*(a1*sw0 - (2.0*c0+a0)*cw0);
  n22p = ( 2.0*eb0b1*((a0+c0)*cw1*cw0 - cw1*a1*sw0 -cw0*c1*sw1) + 
	   c0*e2b0 + a0*e2b1 ); 
  n33p = e2b0b1*(c1*sw1 - cw1*c0) + e2b1b0*(a1*sw0 - cw0*a0);
  
  d11p = -2.0*eb1*cw1 - 2.0*eb0*cw0;
  d22p =  4.0*cw1*cw0*eb0b1 + e2b1 + e2b0;
  d33p = -2.0*cw0*e2b1b0 - 2.0*cw1*e2b0b1;
  d44p = e2b0*e2b1;
  
  /*************************************************************/
  /* Step 1.3: Set n11m, n22m,...                              */
  /*************************************************************/
  d11m = d11p;
  d22m = d22p;
  d33m = d33p;
  d44m = d44p;
  
  if((filter_type == GAUSSDERIV_GAUSSIAN) || 
     (filter_type == GAUSSDERIV_SECONDDERIV)) {
    /* if the filter is symmetrical (g, sdg) */
    n11m = n11p - d11p*n00p;
    n22m = n22p - d22p*n00p;
    n33m = n33p - d33p*n00p;
    n44m =      - d44p*n00p;
    
  } else {
    /* if the filter is antisymmetrical (fdg) */
    n11m = -(n11p - d11p*n00p);
    n22m = -(n22p - d22p*n00p);
    n33m = -(n33p - d33p*n00p);
    n44m =          d44p*n00p;
  }
  
  /* Initialize the result image */
  for (y=0; y<image.height(); y++) {
    for (x=0; x<image.width(); x++) {
      res(x,y) = 0.0;
    }
  }
  norm_g   = 0.0;
  norm_fdg = 0.0;
  norm_sdg = 0.0;
  
  if (filter_dir == GAUSSDERIV_DIR_X) {
    /***********************************************************/
    /* Step 1.4a: For every row, calculate the X vector        */
    /***********************************************************/
    //for (y=4; y<image.height()-4; y++) {
    for (y=0; y<image.height(); y++) {
      
      /* Initialize the "previous results" vector */
      //for (x=0; x<image.width(); x++) {
      for (x=0; x<image.width()+8; x++) {
	res_p[x] = 0.0;
	res_m[x] = 0.0;
      }
      
      //for (x=4; x<image.width()-4; x++) {
      for (x=0; x<image.width(); x++) {
	ip   = x;
	im   = (image.width()-x-1);

	float image_ip0_y, image_ip1_y, image_ip2_y, image_ip3_y;
	float image_im1_y, image_im2_y, image_im3_y, image_im4_y;

	//idxp = y*image.width() + ip;
	//idxm = y*image.width() + im;		 
	
	image_ip3_y = image((ip-3+image.width()) % image.width(), y).value();
	image_ip2_y = image((ip-2+image.width()) % image.width(), y).value();
	image_ip1_y = image((ip-1+image.width()) % image.width(), y).value();
	image_ip0_y = image(ip-0, y).value();
	
	image_im1_y = image((im+1) % image.width(), y).value();
	image_im2_y = image((im+2) % image.width(), y).value();
	image_im3_y = image((im+3) % image.width(), y).value();
	image_im4_y = image((im+4) % image.width(), y).value();

	//res_p[ip] = (( n00p*image[idxp]   + n11p*image[idxp-1] +
	//	       n22p*image[idxp-2] + n33p*image[idxp-3] ) -
	//	     ( d11p*res_p[ip-1]   + d22p*res_p[ip-2] +
	//	       d33p*res_p[ip-3]   + d44p*res_p[ip-4] ));
	//res_p[ip] = (( n00p*image(ip-0,y).value() + n11p*image(ip-1,y).value() +
	//	       n22p*image(ip-2,y).value() + n33p*image(ip-3,y).value() ) -
	//	     ( d11p*res_p[ip-1]   + d22p*res_p[ip-2] +
	//	       d33p*res_p[ip-3]   + d44p*res_p[ip-4] ));
	res_p[4+ip] = (( n00p*image_ip0_y + n11p*image_ip1_y +
		         n22p*image_ip2_y + n33p*image_ip3_y ) -
		      ( d11p*res_p[4+ip-1] + d22p*res_p[4+ip-2] +
		        d33p*res_p[4+ip-3] + d44p*res_p[4+ip-4] ));
	
	//res_m[im] = (( n11m*image[idxm+1] + n22m*image[idxm+2] +
	//	       n33m*image[idxm+3] + n44m*image[idxm+4] ) -
	//	     ( d11m*res_m[im+1]   + d22m*res_m[im+2] +
	//	       d33m*res_m[im+3]   + d44m*res_m[im+4] ));
	//res_m[im] = (( n11m*image(im+1,y).value() + n22m*image(im+2,y).value() +
	//	       n33m*image(im+3,y).value() + n44m*image(im+4,y).value() ) -
	//	     ( d11m*res_m[im+1]   + d22m*res_m[im+2] +
	//	       d33m*res_m[im+3]   + d44m*res_m[im+4] ));
	res_m[4+im] = (( n11m*image_im1_y + n22m*image_im2_y +
		         n33m*image_im3_y + n44m*image_im4_y ) -
		      ( d11m*res_m[4+im+1] + d22m*res_m[4+im+2] +
		        d33m*res_m[4+im+3] + d44m*res_m[4+im+4] ));
      } 
      
      //for (x=4; x<image.width()-4; x++) {
      for (x=0; x<image.width(); x++) {
	//idx  = y*obj_w + x;
	
	//res[idx] = res_p[x] + res_m[x];
	res(x,y) = res_p[4+x] + res_m[4+x];

	if (normalize) {
	  //norm_g   += res[idx];
	  //norm_fdg += x*res[idx];
	  //norm_sdg += x*x*res[idx];
	  norm_g   += res(x,y).value();
	  norm_fdg += x*res(x,y).value();
	  norm_sdg += x*x*res(x,y).value();
	}
      }
    }
    
  } else if (filter_dir == GAUSSDERIV_DIR_Y) {
    /*************************************************************/
    /* Step 1.4b: For every column, calculate the Y vector       */
    /*************************************************************/
    //for (x=4; x<image.width()-4; x++) {
    for (x=0; x<image.width(); x++) {
      
      /* Initialize the "previous results" vector */
      //for (y=0; y<image.height(); y++) {
      for (y=0; y<image.height()+8; y++) {
	res_p[y] = 0.0;
	res_m[y] = 0.0;
      }
      
      //for (y=4; y<image.height()-4; y++) {
      for (y=0; y<image.height(); y++) {
	ip   = y;
	im   = (image.height()-y-1);

	//idxp = ip*obj_w + x;
	//idxm = im*obj_w + x;		 

	float image_x_ip0, image_x_ip1, image_x_ip2, image_x_ip3;
	float image_x_im1, image_x_im2, image_x_im3, image_x_im4;

	/* if the sampled pixels are outside the image border, transform the */
	/* image to a torus shape.                                           */
	image_x_ip3 = image(x,(ip-3+image.height()) % image.height()).value();
	image_x_ip2 = image(x,(ip-2+image.height()) % image.height()).value();
	image_x_ip1 = image(x,(ip-1+image.height()) % image.height()).value();
	image_x_ip0 = image(x,ip-0).value();
	
	image_x_im1 = image(x,(im+1) % image.height()).value();
	image_x_im2 = image(x,(im+2) % image.height()).value();
	image_x_im3 = image(x,(im+3) % image.height()).value();
	image_x_im4 = image(x,(im+4) % image.height()).value();
	
	//res_p[ip] = (( n00p*image[idxp]         + n11p*image[idxp-1*obj_w] +
	//	       n22p*image[idxp-2*obj_w] + n33p*image[idxp-3*obj_w] ) -
	//	     ( d11p*res_p[ip-1]   + d22p*res_p[ip-2] +
	//	       d33p*res_p[ip-3]   + d44p*res_p[ip-4] ));
	//res_p[ip] = (( n00p*image(x,ip-0).value() + n11p*image(x,ip-1).value() +
	//             n22p*image(x,ip-2).value() + n33p*image(x,ip-3).value() ) -
	//           ( d11p*res_p[ip-1]   + d22p*res_p[ip-2] +
	//             d33p*res_p[ip-3]   + d44p*res_p[ip-4] ));
	res_p[4+ip] = (( n00p*image_x_ip0 + n11p*image_x_ip1 +
			 n22p*image_x_ip2 + n33p*image_x_ip3 ) -
		       ( d11p*res_p[4+ip-1] + d22p*res_p[4+ip-2] +
			 d33p*res_p[4+ip-3] + d44p*res_p[4+ip-4] ));
	
	//res_m[im] = (( n11m*image[idxm+1*obj_w] + n22m*image[idxm+2*obj_w] +
	//	       n33m*image[idxm+3*obj_w] + n44m*image[idxm+4*obj_w] ) -
	//	     ( d11m*res_m[im+1]   + d22m*res_m[im+2] +
	//	       d33m*res_m[im+3]   + d44m*res_m[im+4] ));
	//res_m[im] = (( n11m*image(x,im+1).value() + n22m*image(x,im+2).value() +
	//	       n33m*image(x,im+3).value() + n44m*image(x,im+4).value() ) -
	//	     ( d11m*res_m[im+1]   + d22m*res_m[im+2] +
	//	       d33m*res_m[im+3]   + d44m*res_m[im+4] ));
	res_m[4+im] = (( n11m*image_x_im1 + n22m*image_x_im2 +
			 n33m*image_x_im3 + n44m*image_x_im4 ) -
		       ( d11m*res_m[4+im+1] + d22m*res_m[4+im+2] +
			 d33m*res_m[4+im+3] + d44m*res_m[4+im+4] ));
      }
      
      //for (y=4; y<image.height()-4; y++) {
      for (y=0; y<image.height(); y++) {
	//idx  = y*obj_w + x;
		  
	//res[idx] = res_p[y] + res_m[y];
	res(x,y) = res_p[4+y] + res_m[4+y];
	
	if (normalize) {
	  //norm_g   += res[idx];
	  //norm_fdg += y*res[idx];
	  //norm_sdg += y*y*res[idx];
	  norm_g   += res(x,y).value();
	  norm_fdg += y*res(x,y).value();
	  norm_sdg += y*y*res(x,y).value();
	}
      }
    }
    
  } else {
    std::cerr << "Error in ApplyFilter(): Unknown filter direction: " 
	 << filter_dir << std::endl;
    return false;
  }
  
  
  /*************************************************************/
  /* Normalize the output                                      */
  /*************************************************************/
  if (normalize) {
    switch(filter_type) {
    case GAUSSDERIV_GAUSSIAN: {
      //norm_fact = 1.0 / norm_g;
      double scale = -a0*(e2b0 - 1.0) / (2.0*cw0*eb0 - 1.0 - e2b0);
      norm_fact = 1.0 / scale;
    } break;
    
    case GAUSSDERIV_FIRSTDERIV: {
      //norm_fact = 1.0 / norm_fdg;
      double scale = ( (2.0*eb0*a1*(cw0*cw0*e2b0 - e2b0 - cw0*cw0 + 1.0)) / 
		       (sw0*(4.0*cw0*eb0 - 1.0 - 2.0*e2b0 - 4.0*cw0*cw0*e2b0 + 
			     4.0*cw0*e2b0*eb0 - e2b0*e2b0)) );
      norm_fact = 1.0 / scale;
    } break;
    
    case GAUSSDERIV_SECONDDERIV: {
      //norm_fact = 2.0 / norm_sdg;
      double scale = ( (2.0*eb0*a1*(cw0*cw0*e2b0 - e2b0 - cw0*cw0 + 1.0)) / 
		       (sw0*(4.0*cw0*eb0 - 1.0 - 2.0*e2b0 - 4.0*cw0*cw0*e2b0 + 
			     4.0*cw0*e2b0*eb0 - e2b0*e2b0)) );
      norm_fact = 2.0 / scale;
    } break;
    
    default:
      norm_fact = 1.0;
    }
    
    for (x=0; x<image.width(); x++)
      for (y=0; y<image.height(); y++) {
	//idx = y*obj_w + x;
	
	//res[idx] *= norm_fact;
	res(x,y) = res(x,y).value() * norm_fact;
      }
  }
  
  
  /*************************************************************/
  /* Return the result image res[]                             */
  /*************************************************************/
  free(res_p);
  free(res_m);
  
  return true;
}

/*------------------------------------------------------------------------*/

/*========================================================================*/
/*                  ApplyFilter for a double* image class                 */
/*========================================================================*/

bool ApplyFilter(int filter_type, int filter_dir, double sigma,
		 double* image, int obj_w, int obj_h,
		 double* res)
{
  /* additional variables used in the calculations */
  double  a0, a1, b0, b1, c0, c1, w0, w1;
  double  n00p, n11p, n22p, n33p, d11p, d22p, d33p, d44p;
  double  n11m, n22m, n33m, n44m, d11m, d22m, d33m, d44m;

  /* often used terms */
  double  sw0, sw1, cw0, cw1;   /* stands for sin(w0/sigma), cos(w0/sigma) */
  double  eb0, eb1, e2b0, e2b1; /* stands for exp(-b0/sigma),...           */
  double  eb0b1, e2b0b1, e2b1b0; /* stands for eb0*eb1,...                 */


  /* result vectors */
  double *res_p;
  double *res_m;

  /* index variables */
  int     x, y, idx, idxp, idxm, ip, im;

  /* normalization variables */
  bool    normalize = true;
  double  norm_g, norm_fdg, norm_sdg, norm_fact;

  /* reserve space for the result vectors */
  res_p = (double *) malloc ((obj_w+obj_h)*sizeof(double)); 
  res_m = (double *) malloc ((obj_w+obj_h)*sizeof(double)); 
  if ( (res_p==NULL) || (res_m==NULL) ) {
    std::cerr << "Error in ApplyFilter(): Out of memory!" << std::endl;
    return false;
  }

  /*************************************************************/
  /* Step 1.1: Set the coefficients a0, a1,...                 */
  /*************************************************************/
  switch(filter_type) {
  case GAUSSDERIV_GAUSSIAN: {
    a0 = 1.68;
    a1 = 3.735;
    b0 = 1.783;
    b1 = 1.723;
    c0 = -0.6803;
    c1 = -0.2598;
    w0 = 0.6318;
    w1 = 1.997;
  } break;
  
  case GAUSSDERIV_FIRSTDERIV: {
    a0 = -0.6472;
    a1 = -4.531;
    b0 = 1.527;
    b1 = 1.516;
    c0 = 0.6494;
    c1 = 0.9557;
    w0 = 0.6719;
    w1 = 2.072;
  } break;
  
  case GAUSSDERIV_SECONDDERIV: {
    a0 = -1.331;
    a1 = 3.661;
    b0 = 1.24;
    b1 = 1.314;
    c0 = 0.3225;
    c1 = -1.738;
    w0 = 0.748;
    w1 = 2.166;
  } break;
  
  default:
    std::cerr << "Error in ApplyFilter(): Unknown filter type: " << filter_type 
	 << std::endl;
    a0 = a1 = b0 = b1 = c0 = c1 = w0 = w1 = 0.0;
    return false;
  }

  /* Initialize some often used terms: */
  sw0 = sin(w0/sigma);
  sw1 = sin(w1/sigma);
  cw0 = cos(w0/sigma);
  cw1 = cos(w1/sigma);
  eb0 = exp(-b0/sigma);
  eb1 = exp(-b1/sigma);
  e2b0 = eb0*eb0;
  e2b1 = eb1*eb1;
  eb0b1 = eb0*eb1;
  e2b0b1= e2b0*eb1;
  e2b1b0= e2b1*eb0;
  
  /*************************************************************/
  /* Step 1.2: Calculate n00, n11,...                          */
  /*************************************************************/
  n00p = a0 + c0;
  n11p = eb1*(c1*sw1 - (c0+2.0*a0)*cw1) + eb0*(a1*sw0 - (2.0*c0+a0)*cw0);
  n22p = ( 2.0*eb0b1*((a0+c0)*cw1*cw0 - cw1*a1*sw0 -cw0*c1*sw1) + 
	   c0*e2b0 + a0*e2b1 ); 
  n33p = e2b0b1*(c1*sw1 - cw1*c0) + e2b1b0*(a1*sw0 - cw0*a0);
  
  d11p = -2.0*eb1*cw1 - 2.0*eb0*cw0;
  d22p =  4.0*cw1*cw0*eb0b1 + e2b1 + e2b0;
  d33p = -2.0*cw0*e2b1b0 - 2.0*cw1*e2b0b1;
  d44p = e2b0*e2b1;
  
  /*************************************************************/
  /* Step 1.3: Set n11m, n22m,...                              */
  /*************************************************************/
  d11m = d11p;
  d22m = d22p;
  d33m = d33p;
  d44m = d44p;
  
  if((filter_type == GAUSSDERIV_GAUSSIAN) || 
     (filter_type == GAUSSDERIV_SECONDDERIV)) {
    /* if the filter is symmetrical (g, sdg) */
    n11m = n11p - d11p*n00p;
    n22m = n22p - d22p*n00p;
    n33m = n33p - d33p*n00p;
    n44m =      - d44p*n00p;
    
  } else {
    /* if the filter is antisymmetrical (fdg) */
    n11m = -(n11p - d11p*n00p);
    n22m = -(n22p - d22p*n00p);
    n33m = -(n33p - d33p*n00p);
    n44m =          d44p*n00p;
  }
  
  /* Initialize the result image */
  for (y=0; y<obj_h; y++) {
    for (x=0; x<obj_w; x++) {
      idx = y*obj_w + x;
      
      res[idx] = 0.0;
    }
  }
  norm_g   = 0.0;
  norm_fdg = 0.0;
  norm_sdg = 0.0;
  
  if (filter_dir == GAUSSDERIV_DIR_X) {
    /***********************************************************/
    /* Step 1.4a: For every row, calculate the X vector        */
    /***********************************************************/
    for (y=4; y<obj_h-4; y++) {
      
      /* Initialize the "previous results" vector */
      for (x=0; x<obj_w; x++) {
	res_p[x] = 0.0;
	res_m[x] = 0.0;
      }
      
      for (x=4; x<obj_w-4; x++) {
	ip   = x;
	im   = (obj_w-x-1);
	idxp = y*obj_w + ip;
	idxm = y*obj_w + im;		 
	
	res_p[ip] = (( n00p*image[idxp]   + n11p*image[idxp-1] +
		       n22p*image[idxp-2] + n33p*image[idxp-3] ) -
		     ( d11p*res_p[ip-1]   + d22p*res_p[ip-2] +
		       d33p*res_p[ip-3]   + d44p*res_p[ip-4] ));
	
	res_m[im] = (( n11m*image[idxm+1] + n22m*image[idxm+2] +
		       n33m*image[idxm+3] + n44m*image[idxm+4] ) -
		     ( d11m*res_m[im+1]   + d22m*res_m[im+2] +
		       d33m*res_m[im+3]   + d44m*res_m[im+4] ));
      } 
      
      for (x=4; x<obj_w-4; x++) {
	idx  = y*obj_w + x;
	
	res[idx] = res_p[x] + res_m[x];

	if (normalize) {
	  norm_g   += res[idx];
	  norm_fdg += x*res[idx];
	  norm_sdg += x*x*res[idx];
	}
      }
    }
    
  } else if (filter_dir == GAUSSDERIV_DIR_Y) {
    /*************************************************************/
    /* Step 1.4b: For every column, calculate the Y vector       */
    /*************************************************************/
    for (x=4; x<obj_w-4; x++) {
      
      /* Initialize the "previous results" vector */
      for (y=0; y<obj_w; y++) {
	res_p[y] = 0.0;
	res_m[y] = 0.0;
      }
      
      for (y=4; y<obj_h-4; y++) {
	ip   = y;
	im   = (obj_h-y-1);
	idxp = ip*obj_w + x;
	idxm = im*obj_w + x;		 
	
	res_p[ip] = (( n00p*image[idxp]         + n11p*image[idxp-1*obj_w] +
		       n22p*image[idxp-2*obj_w] + n33p*image[idxp-3*obj_w] ) -
		     ( d11p*res_p[ip-1]   + d22p*res_p[ip-2] +
		       d33p*res_p[ip-3]   + d44p*res_p[ip-4] ));
	
	res_m[im] = (( n11m*image[idxm+1*obj_w] + n22m*image[idxm+2*obj_w] +
		       n33m*image[idxm+3*obj_w] + n44m*image[idxm+4*obj_w] ) -
		     ( d11m*res_m[im+1]   + d22m*res_m[im+2] +
		       d33m*res_m[im+3]   + d44m*res_m[im+4] ));
      } 
      
      for (y=4; y<obj_h-4; y++) {
	idx  = y*obj_w + x;
		  
	res[idx] = res_p[y] + res_m[y];
	
	if (normalize) {
	  norm_g   += res[idx];
	  norm_fdg += y*res[idx];
	  norm_sdg += y*y*res[idx];
	}
      }
    }
    
  } else {
    std::cerr << "Error in ApplyFilter(): Unknown filter direction: " 
	 << filter_dir << std::endl;
    return false;
  }
  
  
  /*************************************************************/
  /* Normalize the output                                      */
  /*************************************************************/
  if (normalize) {
    switch(filter_type) {
    case GAUSSDERIV_GAUSSIAN: {
      //norm_fact = 1.0 / norm_g;
      double scale = -a0*(e2b0 - 1.0) / (2.0*cw0*eb0 - 1.0 - e2b0);
      norm_fact = 1.0 / scale;
    } break;
    
    case GAUSSDERIV_FIRSTDERIV: {
      //norm_fact = 1.0 / norm_fdg;
      double scale = ( (2.0*eb0*a1*(cw0*cw0*e2b0 - e2b0 - cw0*cw0 + 1.0)) / 
		       (sw0*(4.0*cw0*eb0 - 1.0 - 2.0*e2b0 - 4.0*cw0*cw0*e2b0 + 
			     4.0*cw0*e2b0*eb0 - e2b0*e2b0)) );
      norm_fact = 1.0 / scale;
    } break;
    
    case GAUSSDERIV_SECONDDERIV: {
      //norm_fact = 2.0 / norm_sdg;
      double scale = ( (2.0*eb0*a1*(cw0*cw0*e2b0 - e2b0 - cw0*cw0 + 1.0)) / 
		       (sw0*(4.0*cw0*eb0 - 1.0 - 2.0*e2b0 - 4.0*cw0*cw0*e2b0 + 
			     4.0*cw0*e2b0*eb0 - e2b0*e2b0)) );
      norm_fact = 2.0 / scale;
    } break;
    
    default:
      norm_fact = 1.0;
    }
    
    for (x=0; x<obj_w; x++)
      for (y=0; y<obj_h; y++) {
	idx = y*obj_w + x;
	
	res[idx] *= norm_fact;
      }
  }
  
  
  /*************************************************************/
  /* Return the result image res[]                             */
  /*************************************************************/
  free(res_p);
  free(res_m);
  
  return true;
}


/*------------------------------------------------------------------------*/


bool ApplyGauss( const GrayImage &image, double sigma, GrayImage &res )
{
  bool res1=false, res2=false;
  GrayImage g_x( image );
  
  /* G(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_X, sigma, 
		     image,   g_x);
  res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
		     g_x  ,   res);

  return (res1 && res2);
}


bool ApplyGaussGx( const GrayImage &image, double sigma, GrayImage &res )
{
  bool res1=false, res2=false;
  GrayImage fdg_x( image );

  /* Gx(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_X, sigma, 
		     image, fdg_x);
  res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
		     fdg_x, res);

  return (res1 && res2);
}


bool ApplyGaussGy( const GrayImage &image, double sigma, GrayImage &res )
{
  bool res1=false, res2=false;
  GrayImage g_x( image );
  
  /* Gy(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_X, sigma, 
		     image,  g_x);
  res2 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_Y, sigma,   
		     g_x,    res);

  return (res1 && res2);
}


bool ApplyGaussGxx( const GrayImage &image, double sigma, GrayImage &res )
{
  bool res1=false, res2=false;
  GrayImage sdg_x( image );
  
  /* Gxx(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_SECONDDERIV, GAUSSDERIV_DIR_X, sigma, 
		     image, sdg_x);
  res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
		     sdg_x, res);

  return (res1 && res2);
}


bool ApplyGaussGxy( const GrayImage &image, double sigma, GrayImage &res )
{
  bool res1=false, res2=false;
  GrayImage fdg_x( image );
  
  /* Gxy(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_X, sigma, 
		     image, fdg_x);
  res2 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_Y, sigma, 
		     fdg_x, res);

  return (res1 && res2);
}


bool ApplyGaussGyy( const GrayImage &image, double sigma, GrayImage &res )
{
  bool res1=false, res2=false;
  GrayImage g_x( image );
  
  /* Gyy(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_X, sigma, 
		     image, g_x);
  res2 = ApplyFilter(GAUSSDERIV_SECONDDERIV, GAUSSDERIV_DIR_Y, sigma,   
		     g_x,   res);

  return (res1 && res2);
}


bool ApplyGaussMag( const GrayImage &image, double sigma, GrayImage &res )
	/* return the gradient magnitude */
{
  bool res1=false, res2=false;
  GrayImage g_x   ( image );
  GrayImage fdg_x ( image );
  GrayImage gx_out( image );
  GrayImage gy_out( image );
  
  /* Gx(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_X, sigma, 
		     image, fdg_x);
  res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
		     fdg_x, gx_out);

  if ( res1 && res2 ) {
    /* Gy(x,y) */
    res1 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_X, sigma, 
		       image,  g_x);
    res2 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_Y, sigma,   
		       g_x,  gy_out);
  }

  if ( res1 && res2 ) {
    for( int y=0; y<image.height(); y++ )
      for( int x=0; x<image.width(); x++ )
	res(x,y) = sqrt( gx_out(x,y).value()*gx_out(x,y).value() + 
			 gy_out(x,y).value()*gy_out(x,y).value() );
    
  }

  return (res1 && res2);
}


bool ApplyGaussMag2( const GrayImage &image, double sigma, GrayImage &res )
	/* return the square magnitude of the gradient */
{
  bool res1=false, res2=false;
  GrayImage g_x   ( image );
  GrayImage fdg_x ( image );
  GrayImage gx_out( image );
  GrayImage gy_out( image );
  
  /* Gx(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_X, sigma, 
		     image, fdg_x);
  res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
		     fdg_x, gx_out);

  if ( res1 && res2 ) {
    /* Gy(x,y) */
    res1 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_X, sigma, 
		       image,  g_x);
    res2 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_Y, sigma,   
		       g_x,  gy_out);
  }

  if ( res1 && res2 ) {
    for( int y=0; y<image.height(); y++ )
      for( int x=0; x<image.width(); x++ )
				res(x,y) = ( gx_out(x,y).value()*gx_out(x,y).value() + 
										 gy_out(x,y).value()*gy_out(x,y).value() );
    
  }

  return (res1 && res2);
}


bool ApplyGaussLap( const GrayImage &image, double sigma, GrayImage &res )
{
  bool res1=false, res2=false;
  GrayImage g_x    ( image );
  GrayImage sdg_x  ( image );
  GrayImage gxx_out( image );
  GrayImage gyy_out( image );
    
  /* Gxx(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_SECONDDERIV, GAUSSDERIV_DIR_X, sigma, 
		     image, sdg_x);
  res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
		     sdg_x, gxx_out);
  
  if ( res1 && res2 ) {    
    /* Gyy(x,y) */
    res1 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_X, sigma, 
		       image,  g_x);
    res2 = ApplyFilter(GAUSSDERIV_SECONDDERIV, GAUSSDERIV_DIR_Y, sigma,   
		       g_x, gyy_out);  
  }

  if ( res1 && res2 ) {    
    for( int y=0; y<image.height(); y++ )
      for( int x=0; x<image.width(); x++ )
	res(x,y) = gxx_out(x,y).value() + gyy_out(x,y).value();
  }

  return (res1 && res2);
}


bool ApplyAllGaussFilters( const GrayImage &image, double sigma,
			   GrayImage &g_out, 
			   GrayImage &gx_out, GrayImage gy_out,
			   GrayImage &gxx_out, 
			   GrayImage &gxy_out, 
			   GrayImage &gyy_out )
{
  bool res1=false, res2=false;
  GrayImage g_x  ( image );
  GrayImage fdg_x( image );
  GrayImage sdg_x( image );
  
  /*************************************************************/
  /* Step 1: Calculate the result images for all filter types  */
  /*************************************************************/
  
  /* G(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_X, sigma, 
		     image,  g_x);
  res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
		     g_x  , g_out);
  
  if ( res1 && res2 ) {
    /* Gx(x,y) */
    res1 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_X, sigma, 
		       image, fdg_x);
    res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
		       fdg_x, gx_out);
  }

  if ( res1 && res2 ) {
    /* Gy(x,y) */
    res1 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_Y, sigma,   
		       g_x,  gy_out);
  }
  
  if ( res1 && res2 ) {
    /* Gxx(x,y) */
    res1 = ApplyFilter(GAUSSDERIV_SECONDDERIV, GAUSSDERIV_DIR_X, sigma, 
		       image, sdg_x);
    res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
		       sdg_x, gxx_out);
  }
  
  if ( res1 && res2 ) {
    /* Gxy(x,y) */
    res1 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_Y, sigma, 
		       fdg_x, gxy_out);
    
    /* Gyy(x,y) */
    res2 = ApplyFilter(GAUSSDERIV_SECONDDERIV, GAUSSDERIV_DIR_Y, sigma,   
		       g_x, gyy_out);  
  }

  return (res1 && res2);
}


/*------------------------------------------------------------------------*/


bool ApplyGauss( double *image, int w, int h, double sigma, double *res )
{
  bool res1=false, res2=false;
  double *g_x;

  /* reserve space for the intermediate result image */
  g_x = (double *) malloc (w*h*sizeof(double)); 
  
  /* G(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_X, sigma, 
		     image, w, h, g_x);
  res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
		     g_x  , w, h, res);

  /* clean up the reserved space again */
  free(g_x);

  return (res1 && res2);
}


bool ApplyGaussGx( double *image, int w, int h, double sigma, double *res )
{
  bool res1=false, res2=false;
  double *fdg_x;

  /* reserve space for the intermediate result image */
  fdg_x = (double *) malloc (w*h*sizeof(double)); 
  
  /* Gx(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_X, sigma, 
		     image, w, h, fdg_x);
  res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
		     fdg_x, w, h, res);

  /* clean up the reserved space again */
  free(fdg_x);

  return (res1 && res2);
}


bool ApplyGaussGy( double *image, int w, int h, double sigma, double *res )
{
  bool res1=false, res2=false;
  double *g_x;
  
  /* reserve space for the intermediate result image */
  g_x = (double *) malloc (w*h*sizeof(double)); 
  
  /* Gy(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_X, sigma, 
		     image, w, h,  g_x);
  res2 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_Y, sigma,   
		     g_x, w, h,   res);

  /* clean up the reserved space again */
  free(g_x);

  return (res1 && res2);
}


bool ApplyGaussGxx( double *image, int w, int h, double sigma, double *res )
{
  bool res1=false, res2=false;
  double *sdg_x;
  
  /* reserve space for the intermediate result image */
  sdg_x = (double *) malloc (w*h*sizeof(double)); 
  
  /* Gxx(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_SECONDDERIV, GAUSSDERIV_DIR_X, sigma, 
	      image, w, h, sdg_x);
  res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
	      sdg_x, w, h, res);

  /* clean up the reserved space again */
  free(sdg_x);

  return (res1 && res2);
}


bool ApplyGaussGxy( double *image, int w, int h, double sigma, double *res )
{
  bool res1=false, res2=false;
  double *fdg_x;
  
  /* reserve space for the intermediate result image */
  fdg_x = (double *) malloc (w*h*sizeof(double)); 
  
  /* Gxy(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_X, sigma, 
	      image, w, h, fdg_x);
  res2 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_Y, sigma, 
	      fdg_x, w, h, res);

  /* clean up the reserved space again */
  free(fdg_x);

  return (res1 && res2);
}


bool ApplyGaussGyy( double *image, int w, int h, double sigma, double *res )
{
  bool res1=false, res2=false;
  double *g_x;
  
  /* reserve space for the intermediate result image */
  g_x = (double *) malloc (w*h*sizeof(double)); 
  
  /* Gyy(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_X, sigma, 
	      image, w, h, g_x);
  res2 = ApplyFilter(GAUSSDERIV_SECONDDERIV, GAUSSDERIV_DIR_Y, sigma,   
	      g_x,   w, h, res);

  /* clean up the reserved space again */
  free(g_x);

  return (res1 && res2);
}


bool ApplyAllGaussFilters( double *image, int w, int h, double sigma,
			   double *g_out, 
			   double *gx_out, double *gy_out,
			   double *gxx_out, 
			   double *gxy_out, 
			   double *gyy_out )
{
  bool res1=false, res2=false;
  double *g_x;
  double *fdg_x;
  double *sdg_x;
  
  /* reserve space for the intermediate result images */
  g_x   = (double *) malloc (w*h*sizeof(double)); 
  fdg_x = (double *) malloc (w*h*sizeof(double)); 
  sdg_x = (double *) malloc (w*h*sizeof(double)); 
  
  /*************************************************************/
  /* Step 1: Calculate the result images for all filter types  */
  /*************************************************************/
  
  /* G(x,y) */
  res1 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_X, sigma, 
		     image, w, h,  g_x);
  res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
		     g_x  , w, h, g_out);
  
  if ( res1 && res2 ) {
    /* Gx(x,y) */
    res1 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_X, sigma, 
		       image, w, h, fdg_x);
    res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
		       fdg_x, w, h, gx_out);
  }
  
  if ( res1 && res2 ) {
    /* Gy(x,y) */
    res1 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_Y, sigma,   
		       g_x, w, h,  gy_out);
  }

  if ( res1 && res2 ) {
    /* Gxx(x,y) */
    res1 = ApplyFilter(GAUSSDERIV_SECONDDERIV, GAUSSDERIV_DIR_X, sigma, 
		       image, w, h, sdg_x);
    res2 = ApplyFilter(GAUSSDERIV_GAUSSIAN   , GAUSSDERIV_DIR_Y, sigma, 
		       sdg_x, w, h, gxx_out);
  }
  
  if ( res1 && res2 ) {
    /* Gxy(x,y) */
    res1 = ApplyFilter(GAUSSDERIV_FIRSTDERIV , GAUSSDERIV_DIR_Y, sigma, 
		       fdg_x, w, h, gxy_out);
    
    /* Gyy(x,y) */
    res2 = ApplyFilter(GAUSSDERIV_SECONDDERIV, GAUSSDERIV_DIR_Y, sigma,   
		       g_x, w, h, gyy_out);
  }
  
  /* clean up the reserved space again */
  free(g_x);
  free(fdg_x);
  free(sdg_x);

  return (res1 && res2);
}

/*------------------------------------------------------------------------*/

