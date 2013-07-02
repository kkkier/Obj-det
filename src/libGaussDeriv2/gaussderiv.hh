/*********************************************************************/
/*                                                                   */
/* FILE         gaussderiv.hh                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Provide functions for calculating Gaussian deriva-   */
/*              tives of 0th, 1st, and 2nd order.                    */
/*                                                                   */
/* BEGIN        WED Aug 22 2001                                      */
/* LAST CHANGE  FRI JUN 07 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_GAUSSDERIV_HH
#define LEIBE_GAUSSDERIV_HH

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif
  
/****************/
/*   Includes   */
/****************/
#include <grayimage.hh>

/*******************/
/*   Definitions   */
/*******************/
#define VERBOSE 0

/*========================================================================*/
/*            Gaussian derivatives for a "GrayImage" image class          */
/*========================================================================*/

/* in gaussderiv.cc */
bool ApplyGauss   ( const GrayImage &image, double sigma, GrayImage &res );
bool ApplyGaussGx ( const GrayImage &image, double sigma, GrayImage &res );
bool ApplyGaussGy ( const GrayImage &image, double sigma, GrayImage &res );
bool ApplyGaussGxx( const GrayImage &image, double sigma, GrayImage &res );
bool ApplyGaussGxy( const GrayImage &image, double sigma, GrayImage &res );
bool ApplyGaussGyy( const GrayImage &image, double sigma, GrayImage &res );

bool ApplyGaussMag( const GrayImage &image, double sigma, GrayImage &res );
bool ApplyGaussMag2( const GrayImage &image, double sigma, GrayImage &res );
bool ApplyGaussLap( const GrayImage &image, double sigma, GrayImage &res );

bool ApplyAllGaussFilters( const GrayImage &image, double sigma,
			   GrayImage &g_out, 
			   GrayImage &gx_out, GrayImage gy_out,
			   GrayImage &gxx_out, 
			   GrayImage &gxy_out, 
			   GrayImage &gyy_out );


/* in fastgauss.cc */
void MakeGaussKernel1D( float sigma, GrayImage &kernel );
void MakeGaussKernel2D( float sigma, GrayImage &kernel );

void FastGauss    ( const GrayImage &image, float sigma, GrayImage &res, 
										float boostfactor=1.0 );
void FastGaussDx  ( const GrayImage &image, float sigma, GrayImage &img_dx, 
										float boostfactor=1.0 );
void FastGaussDy  ( const GrayImage &image, float sigma, GrayImage &img_dy, 
										float boostfactor=1.0 );
void FastGaussDxDy( const GrayImage &image, float sigma, 
									  GrayImage &img_dx, GrayImage &img_dy,
									  float boostfactor=1.0 );
void FastGaussMag ( const GrayImage &image, float sigma, GrayImage &img_mag,
										float boostfactor=1.0 );
void FastGaussMag ( const GrayImage &img_dx, const GrayImage &img_dy, 
										GrayImage &img_mag );
void FastGaussMag2( const GrayImage &image, float sigma, GrayImage &img_mag,
										float boostfactor=1.0 );
void FastGaussMag2( const GrayImage &img_dx, const GrayImage &img_dy, 
										GrayImage &img_mag );
void FastGaussDir ( const GrayImage &image, float sigma, GrayImage &img_dir,
										float boostfactor=1.0 );
void FastGaussDir ( const GrayImage &img_dx, const GrayImage &img_dy, 
										GrayImage &img_dir );

void FastGaussMagDir ( const GrayImage &image, float sigma,
											 GrayImage &img_mag, GrayImage &img_dir, 
											 float boostfactor=1.0 );
void FastGaussMagDir ( const GrayImage &img_dx, const GrayImage &img_dy, 
											 GrayImage &img_mag, GrayImage &img_dir );
void FastGaussDxDyMagDir( const GrayImage &image, float sigma,
													GrayImage &img_gauss,
													GrayImage &img_dx, GrayImage &img_dy,
													GrayImage &img_mag, GrayImage &img_dir,
													float boostfactor=1.0 );



/*========================================================================*/
/*             Gaussian derivatives for a "double*" image class           */
/*========================================================================*/

/* in gaussderiv.cc */
bool ApplyGauss   ( double *image, int w, int h, double sigma, double *res );
bool ApplyGaussGx ( double *image, int w, int h, double sigma, double *res );
bool ApplyGaussGy ( double *image, int w, int h, double sigma, double *res );
bool ApplyGaussGxx( double *image, int w, int h, double sigma, double *res );
bool ApplyGaussGxy( double *image, int w, int h, double sigma, double *res );
bool ApplyGaussGyy( double *image, int w, int h, double sigma, double *res );

bool ApplyAllGaussFilters( double *image, int w, int h, double sigma,
			   double *g_out, 
			   double *gx_out, double *gy_out,
			   double *gxx_out, 
			   double *gxy_out, 
			   double *gyy_out );


/*========================================================================*/
/*         Gaussian derivatives for an "unsigned char*" image class       */
/*========================================================================*/

/* in fastgauss.cc */
void FastGauss    ( unsigned char *image, int rows, int cols, float sigma, 
										short int **res, float boostfactor=1.0 );
void FastGaussDx  ( unsigned char *image, int rows, int cols, float sigma, 
										short int **img_dx, float boostfactor=1.0 );
void FastGaussDy  ( unsigned char *image, int rows, int cols, float sigma, 
										short int **img_dy, float boostfactor=1.0 );
void FastGaussDxDy( unsigned char *image, int rows, int cols, float sigma, 
										short int **img_dx, short int **img_dy,
										float boostfactor=1.0 );
void FastGaussMag ( unsigned char *image, int rows, int cols, float sigma, 
										short int **img_mag, float boostfactor=1.0 );
void FastGaussMag ( short int *img_dx, short int *img_dy, int rows, int cols,
										short int **img_mag );
void FastGaussDir ( unsigned char *image, int rows, int cols, float sigma, 
										float **img_dir, float boostfactor=1.0 );
void FastGaussDir ( short int *img_dx, short int *img_dy, int rows, int cols,
										float **img_dir );

void FastGaussMagDir( short int *img_dx, short int *img_dy, int rows, int cols,
											short int **img_mag, float **img_dir );
void FastGaussDxDyMagDir( unsigned char *image, int rows, int cols, 
													float sigma, short int **img_gauss,
													short int **img_dx, short int **img_dy,
													short int **img_mag, float **img_dir,
													float boostfactor=1.0 );


#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif

#endif // #ifdef LEIBE_GAUSSDERIV_HH 
