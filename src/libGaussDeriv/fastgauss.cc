/*********************************************************************/
/*                                                                   */
/* FILE         fastgauss.cc                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Optimized implementation of Gaussian derivatives     */
/*              calculated by convolving with a separable filter     */
/*              mask. The implementation is based on code written    */
/*              by Mike Heath at the University of South Florida in  */
/*              1996 and was adapted to work with the pccv Image     */
/*              class.                                               */
/*                                                                   */
/* BEGIN        FRI Jun 07 2002                                      */
/* LAST CHANGE  FRI Jun 07 2002                                      */
/*                                                                   */
/*********************************************************************/

/*********************************************************************/
/* ORIGINAL AUTHOR: Mike Heath                                       */
/*                  Computer Vision Laboratory                       */
/*                  University of South Florida                      */
/*                  heath@csee.usf.edu                               */
/*                                                                   */
/* DATE: 2/15/96                                                     */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gaussderiv.hh"

/******************/
/*   Prototypes   */
/******************/
void make_gaussian_kernel( float sigma, float **kernel, int *windowsize ); 
double angle_radians     ( double x, double y );

/* Prototypes for "GrayImage" images */
void gaussian_smooth ( const GrayImage &image, float sigma, GrayImage &res, 
											 float boostfactor=1.0 );
void derivative_x_y  ( const GrayImage &image,
											 GrayImage &img_dx, GrayImage &img_dy );
void magnitude_x_y   ( const GrayImage &img_dx, const GrayImage &img_dy,
											 GrayImage &img_mag );
void magnitude_x_y2  ( const GrayImage &img_dx, const GrayImage &img_dy,
											 GrayImage &img_mag );
void radian_direction( const GrayImage &img_dx, const GrayImage &img_dy,
                    	 GrayImage &img_dir, int xdirtag, int ydirtag );

/* Prototypes for "unsigned char*" images */
void gaussian_smooth ( unsigned char *image, int rows, int cols, float sigma,
											 short int **smoothedimg, float boostfactor=1.0 );
void derivative_x_y  ( short int *smoothedimg, int rows, int cols,
											 short int **delta_x, short int **delta_y );
void magnitude_x_y   ( short int *delta_x, short int *delta_y, int rows, 
											 int cols, short int **magnitude );
void radian_direction( short int *delta_x, short int *delta_y, int rows,
											 int cols, float **dir_radians, 
											 int xdirtag, int ydirtag );


/*========================================================================*/
/*            Auxiliary functions for all image representations           */
/*========================================================================*/

void make_gaussian_kernel(float sigma, float **kernel, int *windowsize)
	/*******************************************************************/
	/* PROCEDURE: make_gaussian_kernel                                 */
	/* PURPOSE: Create a one dimensional gaussian kernel.              */
	/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
	/* DATE: 2/15/96                                                   */
	/*******************************************************************/
{
	int i, center;
	float x, fx, sum=0.0;
	
	*windowsize = (int) ( 1 + 2 * ceil(2.5 * sigma) );
	center = (*windowsize) / 2;
	
	if(VERBOSE) printf("      The kernel has %d elements.\n", *windowsize);
	if((*kernel = (float *) calloc((*windowsize), sizeof(float))) == NULL){
		fprintf(stderr, "Error callocing the gaussian kernel array.\n");
		exit(1);
	}
	
	for(i=0;i<(*windowsize);i++){
		x = (float)(i - center);
		fx = pow(2.71828, -0.5*x*x/(sigma*sigma)) / (sigma * sqrt(6.2831853));
		(*kernel)[i] = fx;
		sum += fx;
	}
	
	for(i=0;i<(*windowsize);i++) (*kernel)[i] /= sum;
	
	if(VERBOSE){
		printf("The filter coefficients are:\n");
		for(i=0;i<(*windowsize);i++)
			printf("kernel[%d] = %f\n", i, (*kernel)[i]);
	}
}


double angle_radians(double x, double y)
	/*******************************************************************/
	/* FUNCTION: angle_radians                                         */
	/* PURPOSE: This procedure computes the angle of a vector with     */
	/*          components x and y. It returns this angle in radians   */
	/*          with the answer being in the range 0 <= angle <2*PI.   */
	/*******************************************************************/
{
	double xu, yu, ang;
	
	xu = fabs(x);
	yu = fabs(y);
	
	if((xu == 0) && (yu == 0)) return(0);
	
	ang = atan(yu/xu);
	
	if(x >= 0){
		if(y >= 0) return(ang);
		else return(2*M_PI - ang);
	}
	else{
		if(y >= 0) return(M_PI - ang);
		else return(M_PI + ang);
	}
}


/*========================================================================*/
/*            Gaussian derivatives for a "GrayImage" image class          */
/*========================================================================*/

void FastGauss(const GrayImage &image, float sigma, GrayImage &res, 
							 float boostfactor)
{
	gaussian_smooth( image, sigma, res, boostfactor );
}


void FastGaussDx(const GrayImage &image, float sigma, GrayImage &img_dx, 
								 float boostfactor)
{
	GrayImage img_gauss;
	GrayImage img_dy;

	gaussian_smooth( image, sigma, img_gauss, boostfactor );
	derivative_x_y ( img_gauss, img_dx, img_dy );
}


void FastGaussDy(const GrayImage &image, float sigma, GrayImage &img_dy, 
								 float boostfactor)
{
	GrayImage img_gauss;
	GrayImage img_dx;

	gaussian_smooth( image, sigma, img_gauss, boostfactor );
	derivative_x_y ( img_gauss, img_dx, img_dy );
}


void FastGaussDxDy(const GrayImage &image, float sigma, 
									 GrayImage &img_dx, GrayImage &img_dy,
									 float boostfactor)
{
	GrayImage img_gauss;

	gaussian_smooth( image, sigma, img_gauss, boostfactor );
	derivative_x_y ( img_gauss, img_dx, img_dy );
}


void FastGaussMag(const GrayImage &image, float sigma, GrayImage &img_mag,
									float boostfactor)
{
	GrayImage img_gauss;
	GrayImage img_dx, img_dy;

	gaussian_smooth( image, sigma, img_gauss, boostfactor );
	derivative_x_y ( img_gauss, img_dx, img_dy );
	magnitude_x_y  ( img_dx, img_dy, img_mag );
}


void FastGaussMag ( const GrayImage &img_dx, const GrayImage &img_dy, 
										GrayImage &img_mag )
{
	magnitude_x_y  ( img_dx, img_dy, img_mag );
}


void FastGaussMag2(const GrayImage &image, float sigma, GrayImage &img_mag,
									 float boostfactor)
{
	GrayImage img_gauss;
	GrayImage img_dx, img_dy;

	gaussian_smooth( image, sigma, img_gauss, boostfactor );
	derivative_x_y ( img_gauss, img_dx, img_dy );
	magnitude_x_y2 ( img_dx, img_dy, img_mag );
}


void FastGaussMag2( const GrayImage &img_dx, const GrayImage &img_dy, 
										GrayImage &img_mag )
{
	magnitude_x_y2 ( img_dx, img_dy, img_mag );
}


void FastGaussDir(const GrayImage &image, float sigma, GrayImage &img_dir,
									float boostfactor)
{
	GrayImage img_gauss;
	GrayImage img_dx, img_dy;

	gaussian_smooth ( image, sigma, img_gauss, boostfactor );
	derivative_x_y  ( img_gauss, img_dx, img_dy );
	radian_direction( img_dx, img_dy, img_dir, -1, -1 );
}


void FastGaussDir ( const GrayImage &img_dx, const GrayImage &img_dy, 
										GrayImage &img_dir )
{
	radian_direction( img_dx, img_dy, img_dir, -1, -1 );
}


void FastGaussMagDir(const GrayImage &img_dx, const GrayImage &img_dy,
										 GrayImage &img_mag, GrayImage &img_dir )
{
	magnitude_x_y   ( img_dx, img_dy, img_mag );
	radian_direction( img_dx, img_dy, img_dir, -1, -1 );
}


void FastGaussMagDir(const GrayImage &image, float sigma,
										 GrayImage &img_mag, GrayImage &img_dir,
										 float boostfactor)
{
	GrayImage img_gauss;
	GrayImage img_dx, img_dy;

	gaussian_smooth ( image, sigma, img_gauss, boostfactor );
	derivative_x_y  ( img_gauss, img_dx, img_dy );
	magnitude_x_y   ( img_dx, img_dy, img_mag );
	radian_direction( img_dx, img_dy, img_dir, -1, -1 );
}


void FastGaussDxDyMagDir(const GrayImage &image, float sigma,
												 GrayImage &img_gauss,
												 GrayImage &img_dx, GrayImage &img_dy,
												 GrayImage &img_mag, GrayImage &img_dir,
												 float boostfactor)
{
	gaussian_smooth ( image, sigma, img_gauss, boostfactor );
	derivative_x_y  ( img_gauss, img_dx, img_dy );
	magnitude_x_y   ( img_dx, img_dy, img_mag );
	radian_direction( img_dx, img_dy, img_dir, -1, -1 );
}


/*------------------------------------------------------------------------*/

void MakeGaussKernel1D( float sigma, GrayImage &kernel )
	/* create a 1D Gaussian kernel */
{
	int windowsize = (int) ( 1 + 2 * ceil(2.5 * sigma) );
	int center     = windowsize / 2;

	// initialize kernel image
	GrayImage tmp( windowsize, 1 );
	kernel = tmp;

	/* Evaluate the 1D Gauss function for every pixel of the kernel:   */
  /*   p(x) = ( 1/sqrt(2*pi)*sigma)*exp(-(x-mean)^2/2*sigma^2) )   */
	float sum = 0.0;
	for( int i=0; i<windowsize; i++ ) {
		float  x = (float)(i - center);
		float fx = ( pow(2.71828, -0.5*x*x/(sigma*sigma)) / 
								 (sigma * sqrt(6.2831853)) );
		kernel(i,0) = fx;
		sum += fx;
	}
	
	for( int i=0; i<windowsize; i++ ) 
		kernel(i,0) = kernel(i,0).value() / sum;
}


void MakeGaussKernel2D( float sigma, GrayImage &kernel )
	/* create a 2D Gaussian kernel */
{
	int windowsize = (int) ( 1 + 2 * ceil(2.5 * sigma) );
	int center     = windowsize / 2;

	// initialize kernel image
	GrayImage tmp( windowsize, windowsize );
	kernel = tmp;

	/* Evaluate the 2D Gauss function for every pixel of the kernel:   */
  /*   p(x) = ( 1/(2*pi*sigma^2)*exp(-(x-mean)^2/2*sigma^2) )        */
	float sum = 0.0;
	for( int j=0; j<windowsize; j++ )
		for( int i=0; i<windowsize; i++ ) {
			float  x = (float)(i - center);
			float  y = (float)(j - center);
			float fx = ( pow(2.71828, -0.5*(x*x + y*y)/(sigma*sigma)) / 
									 (sigma*sigma * 6.2831853) );
		kernel(i,j) = fx;
		sum += fx;
	}
	
	for( int j=0; j<windowsize; j++ )
		for( int i=0; i<windowsize; i++ ) 
			kernel(i,j) = kernel(i,j).value() / sum;
}


void gaussian_smooth( const GrayImage &image, float sigma, GrayImage &res, 
											float boostfactor)
	/*******************************************************************/
	/* PROCEDURE: gaussian_smooth                                      */
	/* PURPOSE: Blur an image with a gaussian filter by convolving     */
	/*          with two separate filter masks for x and y direction.  */
	/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */    
	/* DATE: 2/15/96                                                   */
	 /******************************************************************/
{
	int r, c, rr, cc,    /* Counter variables. */
		windowsize,        /* Dimension of the gaussian kernel. */
		center;            /* Half of the windowsize. */
	float *tempim,       /* Buffer for separable filter gaussian smoothing. */
		*kernel,        /* A one dimensional gaussian kernel. */
		dot,            /* Dot product summing variable. */
		sum;            /* Sum of the kernel weights variable. */
	int rows, cols; 

	rows = image.height();
	cols = image.width();
	
	/***********************************************************************
   * Create a 1-dimensional gaussian smoothing kernel.
   ***********************************************************************/
	if(VERBOSE) printf("   Computing the gaussian smoothing kernel.\n");
	make_gaussian_kernel(sigma, &kernel, &windowsize);
	center = windowsize / 2;
	
	/***********************************************************************
   * Allocate a temporary buffer image and the smoothed image.
   ***********************************************************************/
	if((tempim = (float *) calloc(rows*cols, sizeof(float))) == NULL){
		fprintf(stderr, "Error allocating the buffer image.\n");
		exit(1);
	}

	/* initialize the result image */
	GrayImage tmp( image );
	res = tmp;
	
	/***********************************************************************
   * Blur in the x - direction.
   ***********************************************************************/
	if(VERBOSE) printf("   Bluring the image in the X-direction.\n");
	for(r=0;r<rows;r++){
		for(c=0;c<cols;c++){
			dot = 0.0;
			sum = 0.0;
			for(cc=(-center);cc<=center;cc++){
				if(((c+cc) >= 0) && ((c+cc) < cols)){
					dot += (float)image(c+cc,r).value() * kernel[center+cc];
					sum += kernel[center+cc];
				}
			}
			tempim[r*cols+c] = dot/sum;
		}
	}
	
	/***********************************************************************
   * Blur in the y - direction.
   ***********************************************************************/
	if(VERBOSE) printf("   Bluring the image in the Y-direction.\n");
	for(c=0;c<cols;c++){
		for(r=0;r<rows;r++){
			sum = 0.0;
			dot = 0.0;
			for(rr=(-center);rr<=center;rr++){
				if(((r+rr) >= 0) && ((r+rr) < rows)){
					dot += tempim[(r+rr)*cols+c] * kernel[center+rr];
					sum += kernel[center+rr];
				}
			}
			res(c,r) = (dot*boostfactor/sum);
			//(*smoothedimg)[r*cols+c] = (short int)(dot*BOOSTBLURFACTOR/sum + 0.5);
		}
	}
	
	free(tempim);
	free(kernel);
}


void derivative_x_y(const GrayImage &image,
										 GrayImage &img_dx, GrayImage &img_dy)
	/*******************************************************************/
	/* PROCEDURE: derivative_x_y                                       */
	/* PURPOSE: Compute the first derivative of the image in both the  */
	/*          x and y directions. The differential filters that are  */
	/*          used are:                                              */
	/*                                                                 */
	/*                                          -1                     */
	/*         dx =  -1 0 +1     and       dy =  0                     */
	/*                                          +1                     */
	/*                                                                 */
	/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
	/* DATE: 2/15/96                                                   */
	/*******************************************************************/
{
	int r, c, pos;
	int rows, cols;

	rows = image.height();
	cols = image.width();
	
	/***********************************************************************
   * Allocate images to store the derivatives.
   ***********************************************************************/
	/* initialize the result images */
	GrayImage tmp( image );
	img_dx = tmp;
	img_dy = tmp;
	
	/***********************************************************************
   * Compute the x-derivative. Adjust the derivative at the borders to avoid
   * losing pixels.
   ***********************************************************************/
	if(VERBOSE) printf("   Computing the X-direction derivative.\n");
	for(r=0;r<rows;r++){
		//pos = r * cols;
		//(*delta_x)[pos] = smoothedimg[pos+1] - smoothedimg[pos];
		pos = 0;
		img_dx(pos,r) = image(pos+1,r).value() - image(pos,r).value();
		pos++;
		for(c=1;c<(cols-1);c++,pos++){
			//(*delta_x)[pos] = smoothedimg[pos+1] - smoothedimg[pos-1];
			img_dx(pos,r) = image(pos+1,r).value() - image(pos-1,r).value();
		}
		//(*delta_x)[pos] = smoothedimg[pos] - smoothedimg[pos-1];
		img_dx(pos,r) = image(pos,r).value() - image(pos-1,r).value();
	}
	
	/***********************************************************************
   * Compute the y-derivative. Adjust the derivative at the borders to avoid
   * losing pixels.
   ***********************************************************************/
	if(VERBOSE) printf("   Computing the Y-direction derivative.\n");
	for(c=0;c<cols;c++){
		//pos = c;
		//(*delta_y)[pos] = smoothedimg[pos+cols] - smoothedimg[pos];
		pos = 0;
		img_dy(c,pos) = image(c, pos+1).value() - image(c, pos).value();
		pos++;
		for(r=1;r<(rows-1);r++,pos++){
			//(*delta_y)[pos] = smoothedimg[pos+cols] - smoothedimg[pos-cols];
			img_dy(c,pos) = image(c, pos+1).value() - image(c, pos-1).value();
		}
		//(*delta_y)[pos] = smoothedimg[pos] - smoothedimg[pos-cols];
		img_dy(c,pos) = image(c, pos).value() - image(c, pos-1).value();
	}
}


void magnitude_x_y(const GrayImage &img_dx, const GrayImage &img_dy,
									 GrayImage &img_mag )
	/*******************************************************************/
	/* PROCEDURE: magnitude_x_y                                        */
	/* PURPOSE: Compute the magnitude of the gradient. This is the     */
	/*          square root of the sum of the squared derivative       */
	/*          values.                                                */
	/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
	/* DATE: 2/15/96                                                   */
/*********************************************************************/
{
	int   r, c, pos;
	float sq1, sq2;
	int   rows, cols;

	rows = img_dx.height();
	cols = img_dy.width();
	
	/************************************************************************
   * Allocate an image to store the magnitude of the gradient.
   ***********************************************************************/
	/* initialize the magnitude image */
	GrayImage tmp( img_dx );
	img_mag = tmp;
	
	for(r=0;r<rows;r++){
		for(c=0;c<cols;c++){
			sq1 = img_dx(c,r).value() * img_dx(c,r).value();
			sq2 = img_dy(c,r).value() * img_dy(c,r).value();
			img_mag(c,r) = sqrt(sq1 + sq2);
		}
  }
	
}


void magnitude_x_y2(const GrayImage &img_dx, const GrayImage &img_dy,
									 GrayImage &img_mag )
	/*******************************************************************/
	/* PROCEDURE: magnitude_x_y2                                       */
	/* PURPOSE: Compute the square magnitude of the gradient. This is  */
	/*          the sum of the squared derivative values.              */
	/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
	/* DATE: 2/15/96                                                   */
/*********************************************************************/
{
	int   r, c, pos;
	float sq1, sq2;
	int   rows, cols;

	rows = img_dx.height();
	cols = img_dy.width();
	
	/************************************************************************
   * Allocate an image to store the magnitude of the gradient.
   ***********************************************************************/
	/* initialize the magnitude image */
	GrayImage tmp( img_dx );
	img_mag = tmp;
	
	for(r=0;r<rows;r++){
		for(c=0;c<cols;c++){
			sq1 = img_dx(c,r).value() * img_dx(c,r).value();
			sq2 = img_dy(c,r).value() * img_dy(c,r).value();
			img_mag(c,r) = (sq1 + sq2);
		}
  }
	
}


void radian_direction( const GrayImage &img_dx, const GrayImage &img_dy,
                    	 GrayImage &img_dir, int xdirtag, int ydirtag )
	/*******************************************************************/
	/* PROCEDURE: radian_direction                                     */
	/* PURPOSE: To compute a direction of the gradient image from com- */
	/*          ponent dx and dy images. Because not all deriviatives  */
	/*          are computed in the same way, this code allows for dx  */
	/*          or dy to have been calculated in different ways.       */
	/*                                                                 */
	/* FOR X:  xdirtag = -1  for  [-1 0  1]                            */
	/*         xdirtag =  1  for  [ 1 0 -1]                            */
	/*                                                                 */
	/* FOR Y:  ydirtag = -1  for  [-1 0  1]'                           */
	/*         ydirtag =  1  for  [ 1 0 -1]'                           */
	/*                                                                 */
	/* The resulting angle is in radians measured counterclockwise     */
	/* from the xdirection. The angle points "up the gradient".        */
	/*******************************************************************/
{
	int r, c, pos;
	float *dirim=NULL;
	double dx, dy;
	int rows, cols;

	rows = img_dx.height();
	cols = img_dx.width();
	
	/***********************************************************************
   * Allocate an image to store the direction of the gradient.
   ***********************************************************************/
	/* initialize the direction image */
	GrayImage tmp( img_dx );
	img_dir = tmp;
	
	for(r=0;r<rows;r++){
		for(c=0;c<cols;c++){
			dx = (double) img_dx(c,r).value(); 
			dy = (double) img_dy(c,r).value(); 
			
			if(xdirtag == 1) dx = -dx;
			if(ydirtag == -1) dy = -dy;
			
			img_dir(c,r) = (float)angle_radians(dx, dy);
		}
	}
}


/*========================================================================*/
/*         Gaussian derivatives for an "unsigned char*" image class       */
/*========================================================================*/

void FastGauss( unsigned char *image, int rows, int cols, float sigma, 
								short int **res, float boostfactor)
{
	gaussian_smooth( image, rows, cols, sigma, res, boostfactor );
}


void FastGaussDx( unsigned char *image, int rows, int cols, float sigma, 
									short int **img_dx, float boostfactor)
{
	short int *img_gauss;
	short int *img_dy;

	gaussian_smooth( image, rows, cols, sigma, &img_gauss, boostfactor );
	derivative_x_y ( img_gauss, rows, cols, img_dx, &img_dy );

	free( img_gauss );
	free( img_dy );
}


void FastGaussDy( unsigned char *image, int rows, int cols, float sigma, 
										short int **img_dy, float boostfactor)
{
	short int *img_gauss;
	short int *img_dx;

	gaussian_smooth( image, rows, cols, sigma, &img_gauss, boostfactor );
	derivative_x_y ( img_gauss, rows, cols, &img_dx, img_dy );

	free( img_gauss );
	free( img_dx );
}


void FastGaussDxDy( unsigned char *image, int rows, int cols, float sigma, 
										short int **img_dx, short int **img_dy,
										float boostfactor)
{
	short int *img_gauss;

	gaussian_smooth( image, rows, cols, sigma, &img_gauss, boostfactor );
	derivative_x_y ( img_gauss, rows, cols, img_dx, img_dy );

	free( img_gauss );
}


void FastGaussMag( unsigned char *image, int rows, int cols, float sigma, 
									 short int **img_mag, float boostfactor)
{
	short int *img_gauss;
	short int *img_dx, *img_dy;

	gaussian_smooth( image, rows, cols, sigma, &img_gauss, boostfactor );
	derivative_x_y ( img_gauss, rows, cols, &img_dx, &img_dy );
	magnitude_x_y  ( img_dx, img_dy, rows, cols, img_mag );

	free( img_gauss );
	free( img_dx );
	free( img_dy );
}


void FastGaussMag( short int *img_dx, short int *img_dy, int rows, int cols,
									 short int **img_mag )
{
	magnitude_x_y   ( img_dx, img_dy, rows, cols, img_mag );
}


void FastGaussDir( unsigned char *image, int rows, int cols, float sigma, 
									 float **img_dir, float boostfactor)
{
	short int *img_gauss;
	short int *img_dx, *img_dy;

	gaussian_smooth ( image, rows, cols, sigma, &img_gauss, boostfactor );
	derivative_x_y  ( img_gauss, rows, cols, &img_dx, &img_dy );
	radian_direction( img_dx, img_dy, rows, cols, img_dir, -1, -1 );

	free( img_gauss );
	free( img_dx );
	free( img_dy );
}


void FastGaussDir( short int *img_dx, short int *img_dy, int rows, int cols,
									 float **img_dir )
{
	radian_direction( img_dx, img_dy, rows, cols, img_dir, -1, -1 );
}


void FastGaussMagDir( short int *img_dx, short int *img_dy, int rows, int cols,
											short int **img_mag, float **img_dir )
{
	magnitude_x_y   ( img_dx, img_dy, rows, cols, img_mag );
	radian_direction( img_dx, img_dy, rows, cols, img_dir, -1, -1 );
}


void FastGaussDxDyMagDir( unsigned char *image, int rows, int cols, 
													float sigma, short int **img_gauss,
													short int **img_dx, short int **img_dy,
													short int **img_mag, float **img_dir,
													float boostfactor)
{
	gaussian_smooth ( image, rows, cols, sigma, img_gauss, boostfactor );
	derivative_x_y  ( *img_gauss, rows, cols, img_dx, img_dy );
	magnitude_x_y   ( *img_dx, *img_dy, rows, cols, img_mag );
	radian_direction( *img_dx, *img_dy, rows, cols, img_dir, -1, -1 );
}


/*------------------------------------------------------------------------*/

void gaussian_smooth(unsigned char *image, int rows, int cols, float sigma,
										 short int **smoothedimg, float boostfactor)
	/*******************************************************************/
	/* PROCEDURE: gaussian_smooth                                      */
	/* PURPOSE: Blur an image with a gaussian filter by convolving     */
	/*          with two separate filter masks for x and y direction.  */
	/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */    
	/* DATE: 2/15/96                                                   */
	 /******************************************************************/
{
	int r, c, rr, cc,     /* Counter variables. */
		windowsize,        /* Dimension of the gaussian kernel. */
		center;            /* Half of the windowsize. */
	float *tempim,        /* Buffer for separable filter gaussian smoothing. */
		*kernel,        /* A one dimensional gaussian kernel. */
		dot,            /* Dot product summing variable. */
		sum;            /* Sum of the kernel weights variable. */
	
	/***********************************************************************
   * Create a 1-dimensional gaussian smoothing kernel.
   ***********************************************************************/
	if(VERBOSE) printf("   Computing the gaussian smoothing kernel.\n");
	make_gaussian_kernel(sigma, &kernel, &windowsize);
	center = windowsize / 2;
	
	/***********************************************************************
   * Allocate a temporary buffer image and the smoothed image.
   ***********************************************************************/
	if((tempim = (float *) calloc(rows*cols, sizeof(float))) == NULL){
		fprintf(stderr, "Error allocating the buffer image.\n");
		exit(1);
	}
	if(((*smoothedimg) = (short int *) calloc(rows*cols,
																					 sizeof(short int))) == NULL){
		fprintf(stderr, "Error allocating the smoothed image.\n");
		exit(1);
	}
	
	/***********************************************************************
   * Blur in the x - direction.
   ***********************************************************************/
	if(VERBOSE) printf("   Bluring the image in the X-direction.\n");
	for(r=0;r<rows;r++){
		for(c=0;c<cols;c++){
			dot = 0.0;
			sum = 0.0;
			for(cc=(-center);cc<=center;cc++){
				if(((c+cc) >= 0) && ((c+cc) < cols)){
					dot += (float)image[r*cols+(c+cc)] * kernel[center+cc];
					sum += kernel[center+cc];
				}
			}
			tempim[r*cols+c] = dot/sum;
		}
	}
	
	/***********************************************************************
   * Blur in the y - direction.
   ***********************************************************************/
	if(VERBOSE) printf("   Bluring the image in the Y-direction.\n");
	for(c=0;c<cols;c++){
		for(r=0;r<rows;r++){
			sum = 0.0;
			dot = 0.0;
			for(rr=(-center);rr<=center;rr++){
				if(((r+rr) >= 0) && ((r+rr) < rows)){
					dot += tempim[(r+rr)*cols+c] * kernel[center+rr];
					sum += kernel[center+rr];
				}
			}
			(*smoothedimg)[r*cols+c] = (short int)(dot*boostfactor/sum + 0.5);
		}
	}
	
	free(tempim);
	free(kernel);
}


void derivative_x_y(short int *smoothedimg, int rows, int cols,
										 short int **delta_x, short int **delta_y)
	/*******************************************************************/
	/* PROCEDURE: derivative_x_y                                       */
	/* PURPOSE: Compute the first derivative of the image in both the  */
	/*          x and y directions. The differential filters that are  */
	/*          used are:                                              */
	/*                                                                 */
	/*                                          -1                     */
	/*         dx =  -1 0 +1     and       dy =  0                     */
	/*                                          +1                     */
	/*                                                                 */
	/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
	/* DATE: 2/15/96                                                   */
	/*******************************************************************/
{
	int r, c, pos;
	
	/***********************************************************************
   * Allocate images to store the derivatives.
   ***********************************************************************/
	if(((*delta_x) = (short *) calloc(rows*cols, sizeof(short))) == NULL){
		fprintf(stderr, "Error allocating the delta_x image.\n");
		exit(1);
	}
	if(((*delta_y) = (short *) calloc(rows*cols, sizeof(short))) == NULL){
		fprintf(stderr, "Error allocating the delta_x image.\n");
		exit(1);
	}
	
	/***********************************************************************
   * Compute the x-derivative. Adjust the derivative at the borders to avoid
   * losing pixels.
   ***********************************************************************/
	if(VERBOSE) printf("   Computing the X-direction derivative.\n");
	for(r=0;r<rows;r++){
		pos = r * cols;
		(*delta_x)[pos] = smoothedimg[pos+1] - smoothedimg[pos];
		pos++;
		for(c=1;c<(cols-1);c++,pos++){
			(*delta_x)[pos] = smoothedimg[pos+1] - smoothedimg[pos-1];
		}
		(*delta_x)[pos] = smoothedimg[pos] - smoothedimg[pos-1];
	}
	
	/***********************************************************************
   * Compute the y-derivative. Adjust the derivative at the borders to avoid
   * losing pixels.
   ***********************************************************************/
	if(VERBOSE) printf("   Computing the Y-direction derivative.\n");
	for(c=0;c<cols;c++){
		pos = c;
		(*delta_y)[pos] = smoothedimg[pos+cols] - smoothedimg[pos];
		pos += cols;
		for(r=1;r<(rows-1);r++,pos+=cols){
			(*delta_y)[pos] = smoothedimg[pos+cols] - smoothedimg[pos-cols];
		}
		(*delta_y)[pos] = smoothedimg[pos] - smoothedimg[pos-cols];
	}
}


void magnitude_x_y(short int *delta_x, short int *delta_y, int rows, int cols,
									 short int **magnitude)
	/*******************************************************************/
	/* PROCEDURE: magnitude_x_y                                        */
	/* PURPOSE: Compute the magnitude of the gradient. This is the     */
	/*          square root of the sum of the squared derivative       */
	/*          values.                                                */
	/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
	/* DATE: 2/15/96                                                   */
/*********************************************************************/
{
	int r, c, pos, sq1, sq2;
	
	/************************************************************************
   * Allocate an image to store the magnitude of the gradient.
   ***********************************************************************/
	if((*magnitude = (short *) calloc(rows*cols, sizeof(short))) == NULL){
		fprintf(stderr, "Error allocating the magnitude image.\n");
		exit(1);
	}
	
	for(r=0,pos=0;r<rows;r++){
		for(c=0;c<cols;c++,pos++){
			sq1 = (int)delta_x[pos] * (int)delta_x[pos];
			sq2 = (int)delta_y[pos] * (int)delta_y[pos];
			(*magnitude)[pos] = (short)(0.5 + sqrt((float)sq1 + (float)sq2));
		}
	}
	
}


void radian_direction( short int *delta_x, short int *delta_y, int rows,
											 int cols, float **dir_radians, int xdirtag, int ydirtag)
	/*******************************************************************/
	/* PROCEDURE: radian_direction                                     */
	/* PURPOSE: To compute a direction of the gradient image from com- */
	/*          ponent dx and dy images. Because not all deriviatives  */
	/*          are computed in the same way, this code allows for dx  */
	/*          or dy to have been calculated in different ways.       */
	/*                                                                 */
	/* FOR X:  xdirtag = -1  for  [-1 0  1]                            */
	/*         xdirtag =  1  for  [ 1 0 -1]                            */
	/*                                                                 */
	/* FOR Y:  ydirtag = -1  for  [-1 0  1]'                           */
	/*         ydirtag =  1  for  [ 1 0 -1]'                           */
	/*                                                                 */
	/* The resulting angle is in radians measured counterclockwise     */
	/* from the xdirection. The angle points "up the gradient".        */
	/*******************************************************************/
{
	int r, c, pos;
	float *dirim=NULL;
	double dx, dy;
	
	/***********************************************************************
   * Allocate an image to store the direction of the gradient.
   ***********************************************************************/
	if((dirim = (float *) calloc(rows*cols, sizeof(float))) == NULL){
		fprintf(stderr, "Error allocating the gradient direction image.\n");
		exit(1);
	}
	*dir_radians = dirim;
	
	for(r=0,pos=0;r<rows;r++){
		for(c=0;c<cols;c++,pos++){
			dx = (double)delta_x[pos];
			dy = (double)delta_y[pos];
			
			if(xdirtag == 1) dx = -dx;
			if(ydirtag == -1) dy = -dy;
			
			dirim[pos] = (float)angle_radians(dx, dy);
		}
	}
}


