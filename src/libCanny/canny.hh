/*********************************************************************/
/*                                                                   */
/* FILE         canny.hh                                             */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Provides an implementation of the Canny edge detec-  */
/*              tor. This file is based on code written by Mike      */
/*              Heath at the University of South Florida in 96       */
/*              (heath@csee.usf.edu). See the file 'canny.src.txt'   */
/*              for details.                                         */
/*                                                                   */
/* BEGIN        FRI Jun 07 2002                                      */
/* LAST CHANGE  FRI Jun 07 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_CANNY_HH
#define LEIBE_CANNY_HH

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

#define NOEDGE          255
#define POSSIBLE_EDGE   128
#define EDGE              0

const float EPS = 0.0001;

/***************************/
/*   Function Prototypes   */
/***************************/
/******************/
/*   Prototypes   */
/******************/
void apply_hysteresis( const GrayImage &img_mag, unsigned char *nms,
                       float tlow, float thigh, GrayImage &img_edge );
void apply_hysteresis( const GrayImage &img_mag, const GrayImage &img_nms,
                       float tlow, float thigh, GrayImage &img_edge );
void follow_edges    ( GrayImage &img_edge, const GrayImage &img_mag,
                       int pos_x, int pos_y, short lowval );
void non_max_supp    ( const GrayImage &img_mag, const GrayImage &img_dx,
                       const GrayImage &img_dy, GrayImage &result );
void non_max_supp    ( const GrayImage &img_mag, const GrayImage &img_dx,
                       const GrayImage &img_dy, unsigned char *result );
void non_max_supp( const GrayImage &img_mag, int direction, GrayImage &result );

void apply_hysteresis( short int *mag, unsigned char *nms, int rows, int cols,
                       float tlow, float thigh, unsigned char *edge );
void follow_edges    ( unsigned char *edgemapptr, short *edgemagptr,
                       short lowval, int cols );
void non_max_supp    ( short *mag, short *gradx, short *grady,
                       int nrows, int ncols,
                       unsigned char *result );
void non_max_supp    ( short *mag, int nrows, int ncols, int direction, unsigned char* result);


/*========================================================================*/
/*            Canny edge detector for a "GrayImage" image class           */
/*========================================================================*/
void canny( const GrayImage &image, float sigma, float tlow, float thigh,
            GrayImage &img_edge );
void canny( const GrayImage &image, float sigma, float tlow, float thigh,
            GrayImage &img_edge, GrayImage &img_dir );
void canny( const GrayImage &image, float sigma, float tlow, float thigh,
            GrayImage &img_edge, GrayImage &img_mag, GrayImage &img_dir );

void canny( const GrayImage &img_dx, const GrayImage &img_dy,
            float tlow, float thigh,
            GrayImage &img_edge );
void canny( const GrayImage& img_mag, float sigma, float tlow, float thigh,
            int direction, GrayImage& img_edge);
            
/*========================================================================*/
/*         Canny edge detector for an "unsigned char*" image class        */
/*========================================================================*/
void canny( unsigned char *image, int rows, int cols, float sigma,
            float tlow, float thigh, unsigned char **edge );
void canny( unsigned char *image, int rows, int cols, float sigma,
            float tlow, float thigh, unsigned char **edge,
            float **dir_radians );


#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif

#endif // #ifdef LEIBE_CANNY_HH
