/*********************************************************************/
/*                                                                   */
/* FILE         canny.cc                                             */
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

/*********************************************************************/
/* This program implements a "Canny" edge detector. The processing   */
/* steps are as follows:                                             */
/*                                                                   */
/*   1) Convolve the image with a separable gaussian filter.         */
/*   2) Take the dx and dy the first derivatives using [-1,0,1] and  */
/*      [1,0,-1]'.                                                   */
/*   3) Compute the magnitude: sqrt(dx*dx+dy*dy).                    */
/*   4) Perform non-maximal suppression.                             */
/*   5) Perform hysteresis.                                          */
/*                                                                   */
/* The user must input three parameters. These are as follows:       */
/*                                                                   */
/*   sigma = The standard deviation of the gaussian smoothing        */
/*           filter.                                                 */
/*   tlow  = Specifies the low value to use in hysteresis. This is   */
/*           a fraction (0-1) of the computed high threshold edge    */
/*           strength value.                                         */
/*   thigh = Specifies the high value to use in hysteresis. This     */
/*           fraction (0-1) specifies the percentage point in a      */
/*           histogram of the gradient of the magnitude. Magnitude   */
/*           values of zero are not counted in the histogram.        */
/*                                                                   */
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

#include <gaussderiv.hh>
#include "canny.hh"

#define BOOSTBLURFACTOR  1.0 //90.0


/*========================================================================*/
/*            Canny edge detector for a "GrayImage" image class           */
/*========================================================================*/

void canny( const GrayImage &image, float sigma, float tlow, float thigh,
            GrayImage &img_edge )
/*******************************************************************/
/* PROCEDURE: canny                                                */
/* PURPOSE: To perform canny edge detection.                       */
/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
/* DATE: 2/15/96                                                   */
/*******************************************************************/
{
    FILE *fpdir=NULL;          /* File to write the gradient image to.     */
    unsigned char *nms;        /* Points that are local maximal magnitude. */
    int r, c, pos;
    float *dir_radians=NULL;   /* Gradient direction image.                */
    GrayImage img_dx, img_dy, img_mag;
    GrayImage img_nms;
    int rows, cols;

    rows = image.height();
    cols = image.width();

    /***********************************************************************
      * Perform gaussian smoothing on the image using the input standard
      * deviation and compute the first derivative in x and y direction.
      ***********************************************************************/
    if(VERBOSE)
        printf("Smoothing the image using a gaussian kernel.\n");
    if(VERBOSE)
        printf("Computing the X and Y first derivatives.\n");
    FastGaussDxDy( image, sigma, img_dx, img_dy, BOOSTBLURFACTOR );

    /*********************************************************************
     * Compute the magnitude of the gradient
     *********************************************************************/
    FastGaussMag( img_dx, img_dy, img_mag );

    /***********************************************************************
      * Perform non-maximum suppression.
      ***********************************************************************/
    if(VERBOSE)
        printf("Doing the non-maximal suppression.\n");
    //non_max_supp(img_mag, img_dx, img_dy, nms);
    non_max_supp(img_mag, img_dx, img_dy, img_nms);

    /***********************************************************************
      * Use hysteresis to mark the edge pixels.
      ***********************************************************************/
    /* intialize the edge image */
    GrayImage tmp(image);
    img_edge = tmp;

    if(VERBOSE)
        printf("Applying the hysteresis.\n");
    apply_hysteresis(img_mag, img_nms, tlow, thigh, img_edge);
}


void canny( const GrayImage &image, float sigma, float tlow, float thigh,
            GrayImage &img_edge, GrayImage &img_dir )
/*******************************************************************/
/* PROCEDURE: canny                                                */
/* PURPOSE: To perform canny edge detection.                       */
/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
/* DATE: 2/15/96                                                   */
/*******************************************************************/
{
    FILE *fpdir=NULL;          /* File to write the gradient image to.     */
    unsigned char *nms;        /* Points that are local maximal magnitude. */
    int r, c, pos;
    float *dir_radians=NULL;   /* Gradient direction image.                */
    GrayImage img_dx, img_dy;
    GrayImage img_mag;
    GrayImage img_nms;
    int rows, cols;

    rows = image.height();
    cols = image.width();

    /***********************************************************************
      * Perform gaussian smoothing on the image using the input standard
      * deviation and compute the first derivative in x and y direction.
      ***********************************************************************/
    if(VERBOSE)
        printf("Smoothing the image using a gaussian kernel.\n");
    if(VERBOSE)
        printf("Computing the X and Y first derivatives.\n");
    FastGaussDxDy( image, sigma, img_dx, img_dy, BOOSTBLURFACTOR );

    /*********************************************************************
     * Compute the magnitude and the direction up the gradient (the latter 
     * in radians that are specified counteclockwise from the positive x-axis.)
     *********************************************************************/
    FastGaussMagDir( img_dx, img_dy, img_mag, img_dir );

    /***********************************************************************
      * Perform non-maximum suppression.
      ***********************************************************************/
    if(VERBOSE)
        printf("Doing the non-maximal suppression.\n");
    // 	if((nms = (unsigned char *) calloc(rows*cols,sizeof(unsigned char)))==NULL){
    // 		fprintf(stderr, "Error allocating the nms image.\n");
    // 		exit(1);
    // 	}

    non_max_supp(img_mag, img_dx, img_dy, img_nms);

    /***********************************************************************
      * Use hysteresis to mark the edge pixels.
      ***********************************************************************/
    /* intialize the edge image */
    GrayImage tmp(image);
    img_edge = tmp;

    apply_hysteresis(img_mag, img_nms, tlow, thigh, img_edge);

    /***************************************************************************
      * Free all of the memory that we allocated except for the edge image that
      * is still being used to store out result.
      ***************************************************************************/
    //free(nms);
}


void canny( const GrayImage &image, float sigma, float tlow, float thigh,
            GrayImage &img_edge, GrayImage &img_mag, GrayImage &img_dir )
/*******************************************************************/
/* PROCEDURE: canny                                                */
/* PURPOSE: To perform canny edge detection.                       */
/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
/* DATE: 2/15/96                                                   */
/*******************************************************************/
{
    FILE *fpdir=NULL;          /* File to write the gradient image to.     */
    unsigned char *nms;        /* Points that are local maximal magnitude. */
    int r, c, pos;
    float *dir_radians=NULL;   /* Gradient direction image.                */
    GrayImage img_dx, img_dy;
    GrayImage img_nms;
    int rows, cols;

    rows = image.height();
    cols = image.width();

    /***********************************************************************
      * Perform gaussian smoothing on the image using the input standard
      * deviation and compute the first derivative in x and y direction.
      ***********************************************************************/
    if(VERBOSE)
        printf("Smoothing the image using a gaussian kernel.\n");
    if(VERBOSE)
        printf("Computing the X and Y first derivatives.\n");
    FastGaussDxDy( image, sigma, img_dx, img_dy, BOOSTBLURFACTOR );

    /*********************************************************************
     * Compute the magnitude and the direction up the gradient (the latter 
     * in radians that are specified counteclockwise from the positive x-axis.)
     *********************************************************************/
    FastGaussMagDir( img_dx, img_dy, img_mag, img_dir );

    /***********************************************************************
      * Perform non-maximum suppression.
      ***********************************************************************/
    if(VERBOSE)
        printf("Doing the non-maximal suppression.\n");
    // 	if((nms = (unsigned char *) calloc(rows*cols,sizeof(unsigned char)))==NULL){
    // 		fprintf(stderr, "Error allocating the nms image.\n");
    // 		exit(1);
    // 	}

    non_max_supp(img_mag, img_dx, img_dy, img_nms);

    /***********************************************************************
      * Use hysteresis to mark the edge pixels.
      ***********************************************************************/
    /* intialize the edge image */
    GrayImage tmp(image);
    img_edge = tmp;

    apply_hysteresis(img_mag, img_nms, tlow, thigh, img_edge);

    /***************************************************************************
      * Free all of the memory that we allocated except for the edge image that
      * is still being used to store out result.
      ***************************************************************************/
    //free(nms);
}


void canny( const GrayImage &img_dx, const GrayImage &img_dy,
            float tlow, float thigh,
            GrayImage &img_edge )
/*******************************************************************/
/* PROCEDURE: canny                                                */
/* PURPOSE: To perform canny edge detection.                       */
/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
/* DATE: 2/15/96                                                   */
/*******************************************************************/
{
    FILE *fpdir=NULL;          /* File to write the gradient image to.     */
    unsigned char *nms;        /* Points that are local maximal magnitude. */
    int r, c, pos;
    float *dir_radians=NULL;   /* Gradient direction image.                */
    //GrayImage img_dx, img_dy;
    GrayImage img_mag;
    GrayImage img_nms;
    int rows, cols;

    rows = img_dx.height();
    cols = img_dx.width();

    /***********************************************************************
      * Perform gaussian smoothing on the image using the input standard
      * deviation and compute the first derivative in x and y direction.
      ***********************************************************************/
    //if(VERBOSE) printf("Smoothing the image using a gaussian kernel.\n");
    //if(VERBOSE) printf("Computing the X and Y first derivatives.\n");
    //FastGaussDxDy( image, sigma, img_dx, img_dy, BOOSTBLURFACTOR );

    /*********************************************************************
     * Compute the magnitude of the gradient
     *********************************************************************/
    FastGaussMag( img_dx, img_dy, img_mag );

    /***********************************************************************
      * Perform non-maximum suppression.
      ***********************************************************************/
    if(VERBOSE)
        printf("Doing the non-maximal suppression.\n");
    //non_max_supp(img_mag, img_dx, img_dy, nms);
    non_max_supp(img_mag, img_dx, img_dy, img_nms);

    /***********************************************************************
      * Use hysteresis to mark the edge pixels.
      ***********************************************************************/
    /* intialize the edge image */
    GrayImage tmp(cols,rows);
    img_edge = tmp;

    if(VERBOSE)
        printf("Applying the hysteresis.\n");
    apply_hysteresis(img_mag, img_nms, tlow, thigh, img_edge);
}


void canny( const GrayImage& img_mag, float sigma, float tlow, float thigh,
            int direction, GrayImage& img_edge)
{
    unsigned char *nms;        /* Points that are local maximal magnitude. */
    int r, c, pos;
    float *dir_radians=NULL;   /* Gradient direction image.                */
    GrayImage img_dx, img_dy;
    GrayImage img_nms;
    int rows, cols;

    rows = img_mag.height();
    cols = img_mag.width();

    /***********************************************************************
     * Perform non-maximum suppression.
     ***********************************************************************/
    if(VERBOSE)
        printf("Doing the non-maximal suppression.\n");
    //non_max_supp(img_mag, img_dx, img_dy, nms);
    non_max_supp(img_mag, direction, img_nms);

    /***********************************************************************
     * Use hysteresis to mark the edge pixels.
     ***********************************************************************/
    /* intialize the edge image */
    GrayImage tmp(img_mag);
    img_edge = tmp;

    if(VERBOSE)
        printf("Applying the hysteresis.\n");
    apply_hysteresis(img_mag, img_nms, tlow, thigh, img_edge);

}

void apply_hysteresis( const GrayImage &img_mag, const GrayImage &img_nms,
                       float tlow, float thigh, GrayImage &img_edge )
/*******************************************************************/
/* PROCEDURE: apply_hysteresis                                     */
/* PURPOSE: This routine finds edges that are above some high      */
/*          threshhold or are connected to a high pixel by a path  */
/*          of pixels greater than a low threshold.                */
/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
/* DATE: 2/15/96                                                   */
/********************************************************************/
{
    int r, c, pos, numedges, lowcount, highcount, lowthreshold, highthreshold,
    i, hist[32768], rr, cc;
    short int maximum_mag, sumpix;
    int rows, cols;

    rows = img_mag.height();
    cols = img_mag.width();

    /***************************************************************************
      * Initialize the edge map to possible edges everywhere the non-maximal
      * suppression suggested there could be an edge except for the border. At
      * the border we say there can not be an edge because it makes the
      * follow_edges algorithm more efficient to not worry about tracking an
      * edge off the side of the image.
      ***************************************************************************/
    for(r=0,pos=0;r<rows;r++)
    {
        for(c=0;c<cols;c++,pos++)
        {
            if(img_nms(c,r).value() == (float) POSSIBLE_EDGE)
                img_edge(c,r) = (float) POSSIBLE_EDGE;
            else
                img_edge(c,r) = (float) NOEDGE;
        }
    }

    for(r=0;r<rows;r++)
    {
        img_edge(0,r)      = (float) NOEDGE;
        img_edge(cols-1,r) = (float) NOEDGE;
    }
    for(c=0;c<cols;c++)
    {
        img_edge(c,0)      = (float) NOEDGE;
        img_edge(c,rows-1) = (float) NOEDGE;
    }

    /***************************************************************************
      * Compute the histogram of the magnitude image. Then use the histogram to
      * compute hysteresis thresholds.
      ***************************************************************************/
    for(r=0;r<32768;r++)
        hist[r] = 0;
    for(r=0;r<rows;r++)
    {
        for(c=0;c<cols;c++)
        {
            if(img_edge(c,r).value() == (float) POSSIBLE_EDGE)
                hist[(short int) img_mag(c,r).value()]++;
        }
    }

    /***************************************************************************
      * Compute the number of pixels that passed the nonmaximal suppression.
      ***************************************************************************/
    for(r=1,numedges=0;r<32768;r++)
    {
        if(hist[r] != 0)
            maximum_mag = r;
        numedges += hist[r];
    }

    highcount = (int)(numedges * thigh + 0.5);

    /***************************************************************************
      * Compute the high threshold value as the (100 * thigh) percentage point
      * in the magnitude of the gradient histogram of all the pixels that passes
      * non-maximal suppression. Then calculate the low threshold as a fraction
      * of the computed high threshold value. John Canny said in his paper
      * "A Computational Approach to Edge Detection" that "The ratio of the
      * high to low threshold in the implementation is in the range two or three
      * to one." That means that in terms of this implementation, we should
      * choose tlow ~= 0.5 or 0.33333.
      ***************************************************************************/
    r = 1;
    numedges = hist[1];
    while((r<(maximum_mag-1)) && (numedges < highcount))
    {
        r++;
        numedges += hist[r];
    }
    highthreshold = r;
    lowthreshold = (int)(highthreshold * tlow + 0.5);

    if(VERBOSE)
    {
        printf("The input low and high fractions of %f and %f computed to\n",
               tlow, thigh);
        printf("magnitude of the gradient threshold values of: %d %d\n",
               lowthreshold, highthreshold);
    }

    /***************************************************************************
      * This loop looks for pixels above the highthreshold to locate edges and
      * then calls follow_edges to continue the edge.
      ***************************************************************************/
    for(r=1;r<rows-1;r++)
    {
        for(c=1;c<cols-1;c++)
        {
            if((img_edge(c,r).value() == (float) POSSIBLE_EDGE) &&
                    (img_mag(c,r).value() >= highthreshold))
            {
                img_edge(c,r) = (float) EDGE;
                follow_edges(img_edge, img_mag, c,r, lowthreshold );
                // 				follow_edges((edge+pos), (mag+pos), lowthreshold, cols);
            }
        }
    }

    /***************************************************************************
      * Set all the remaining possible edges to non-edges.
      ***************************************************************************/
    for(r=0;r<rows;r++)
    {
        for(c=0;c<cols;c++)
            if(img_edge(c,r).value() != (float) EDGE)
                img_edge(c,r) = (float) NOEDGE;
    }
}


void apply_hysteresis( const GrayImage &img_mag, unsigned char *nms,
                       float tlow, float thigh, GrayImage &img_edge )
/*******************************************************************/
/* PROCEDURE: apply_hysteresis                                     */
/* PURPOSE: This routine finds edges that are above some high      */
/*          threshhold or are connected to a high pixel by a path  */
/*          of pixels greater than a low threshold.                */
/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
/* DATE: 2/15/96                                                   */
/********************************************************************/
{
    int r, c, pos, numedges, lowcount, highcount, lowthreshold, highthreshold,
    i, hist[32768], rr, cc;
    short int maximum_mag, sumpix;
    int rows, cols;

    rows = img_mag.height();
    cols = img_mag.width();

    /***************************************************************************
      * Initialize the edge map to possible edges everywhere the non-maximal
      * suppression suggested there could be an edge except for the border. At
      * the border we say there can not be an edge because it makes the
      * follow_edges algorithm more efficient to not worry about tracking an
      * edge off the side of the image.
      ***************************************************************************/
    for(r=0,pos=0;r<rows;r++)
    {
        for(c=0;c<cols;c++,pos++)
        {
            if(nms[pos] == POSSIBLE_EDGE)
                img_edge(c,r) = (float) POSSIBLE_EDGE;
            else
                img_edge(c,r) = (float) NOEDGE;
        }
    }

    for(r=0;r<rows;r++)
    {
        img_edge(0,r)      = (float) NOEDGE;
        img_edge(cols-1,r) = (float) NOEDGE;
    }
    for(c=0;c<cols;c++)
    {
        img_edge(c,0)      = (float) NOEDGE;
        img_edge(c,rows-1) = (float) NOEDGE;
    }

    /***************************************************************************
      * Compute the histogram of the magnitude image. Then use the histogram to
      * compute hysteresis thresholds.
      ***************************************************************************/
    for(r=0;r<32768;r++)
        hist[r] = 0;
    for(r=0;r<rows;r++)
    {
        for(c=0;c<cols;c++)
        {
            if(img_edge(c,r).value() == (float) POSSIBLE_EDGE)
                hist[(short int) img_mag(c,r).value()]++;
        }
    }

    /***************************************************************************
      * Compute the number of pixels that passed the nonmaximal suppression.
      ***************************************************************************/
    for(r=1,numedges=0;r<32768;r++)
    {
        if(hist[r] != 0)
            maximum_mag = r;
        numedges += hist[r];
    }

    highcount = (int)(numedges * thigh + 0.5);

    /***************************************************************************
      * Compute the high threshold value as the (100 * thigh) percentage point
      * in the magnitude of the gradient histogram of all the pixels that passes
      * non-maximal suppression. Then calculate the low threshold as a fraction
      * of the computed high threshold value. John Canny said in his paper
      * "A Computational Approach to Edge Detection" that "The ratio of the
      * high to low threshold in the implementation is in the range two or three
      * to one." That means that in terms of this implementation, we should
      * choose tlow ~= 0.5 or 0.33333.
      ***************************************************************************/
    r = 1;
    numedges = hist[1];
    while((r<(maximum_mag-1)) && (numedges < highcount))
    {
        r++;
        numedges += hist[r];
    }
    highthreshold = r;
    lowthreshold = (int)(highthreshold * tlow + 0.5);

    if(VERBOSE)
    {
        printf("The input low and high fractions of %f and %f computed to\n",
               tlow, thigh);
        printf("magnitude of the gradient threshold values of: %d %d\n",
               lowthreshold, highthreshold);
    }

    /***************************************************************************
      * This loop looks for pixels above the highthreshold to locate edges and
      * then calls follow_edges to continue the edge.
      ***************************************************************************/
    for(r=0,pos=0;r<rows;r++)
    {
        for(c=0;c<cols;c++,pos++)
        {
            if((img_edge(c,r).value() == (float) POSSIBLE_EDGE) &&
                    (img_mag(c,r).value() >= highthreshold))
            {
                img_edge(c,r) = (float) EDGE;
                follow_edges(img_edge, img_mag, c,r, lowthreshold );
                // 				follow_edges((edge+pos), (mag+pos), lowthreshold, cols);
            }
        }
    }

    /***************************************************************************
      * Set all the remaining possible edges to non-edges.
      ***************************************************************************/
    for(r=0,pos=0;r<rows;r++)
    {
        for(c=0;c<cols;c++,pos++)
            if(img_edge(c,r).value() != (float) EDGE)
                img_edge(c,r) = (float) NOEDGE;
    }
}


void follow_edges( GrayImage &img_edge, const GrayImage &img_mag,
                   int pos_x, int pos_y, short lowval )
// void follow_edges(unsigned char *edgemapptr, short *edgemagptr,
//             short lowval, int cols)
/*******************************************************************/
/* PROCEDURE: follow_edges                                         */
/* PURPOSE: This procedure edges is a recursive routine that tra-  */
/*          ces edgs along all paths whose magnitude values remain */
/*          above some specifyable lower threshold.                */
/* AUTHOR: Mike Heath                                              */
/* DATE: 2/15/96                                                   */
/*******************************************************************/
{
    short *tempmagptr;
    unsigned char *tempmapptr;
    int i;
    float thethresh;
    int x[8] = {1,1,0,-1,-1,-1,0,1};
    int y[8] = {0,1,1,1,0,-1,-1,-1};
    int cols;
    int tmp_x, tmp_y;

    cols = img_edge.width();

    for(i=0;i<8;i++)
    {
        tmp_x = pos_x + x[i];
        tmp_y = pos_y - y[i];
        // 		tempmapptr = edgemapptr - y[i]*cols + x[i];
        // 		tempmagptr = edgemagptr - y[i]*cols + x[i];

        if( (img_edge(tmp_x,tmp_y).value() == (float) POSSIBLE_EDGE) &&
                (img_mag(tmp_x,tmp_y).value() > (float) lowval) )
        {
            // 		if((*tempmapptr == POSSIBLE_EDGE) && (*tempmagptr > lowval)){
            img_edge(tmp_x,tmp_y) = (float) EDGE;
            follow_edges( img_edge, img_mag, tmp_x, tmp_y, lowval );
            // 			*tempmapptr = (unsigned char) EDGE;
            // 			follow_edges(tempmapptr,tempmagptr, lowval, cols);
        }
    }
}


void non_max_supp( const GrayImage &img_mag, const GrayImage &img_dx,
                   const GrayImage &img_dy, GrayImage &result )
/*******************************************************************/
/* PROCEDURE: non_max_supp                                         */
/* PURPOSE: applies non-maximal suppression to the magnitude of    */
/*          the gradient image.                                    */
/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
/* DATE: 2/15/96                                                   */
/********************************************************************/
{
    //int rowcount, colcount;
    int count;
    //short *magrowptr,*magptr;
    //short *gxrowptr,*gxptr;
    //short *gyrowptr,*gyptr,z1,z2;
    float z1, z2;
    float m00, gx, gy;
    //short m00,gx,gy;
    float mag1,mag2,xperp,yperp;
    //unsigned char *resultrowptr, *resultptr;
    int ncols, nrows;
    int c,r;
    int x,y;

    nrows = img_mag.height();
    ncols = img_mag.width();

    /* initialize the result image */
    GrayImage tmp( img_mag.width(), img_mag.height() );
    result = tmp;

    /***********************************************************************
      * Zero the edges of the result image.
      ***********************************************************************/
    for(c=0; c<ncols; c++)
    {
        result(c,0)       = (float) NOEDGE;  //0.0;
        result(c,nrows-1) = (float) NOEDGE;  //0.0;
    }

    for(r=0; r<nrows; r++)
    {
        result(0,r)       = (float) NOEDGE;  //0.0;
        result(ncols-1,r) = (float) NOEDGE;  //0.0;
    }

    /***********************************************************************
      * Suppress non-maximum points.
      ***********************************************************************/
    for( y=1; y<nrows-1; y++ )
    {
        // 	for(rowcount=1,magrowptr=mag+ncols+1,gxrowptr=gradx+ncols+1,
        // 				gyrowptr=grady+ncols+1,resultrowptr=result+ncols+1;
        //       rowcount<nrows-2;
        //       rowcount++,magrowptr+=ncols,gyrowptr+=ncols,gxrowptr+=ncols,
        // 				resultrowptr+=ncols){
        for( x=1; x<ncols-1; x++ )
        {
            // 		for(colcount=1,magptr=magrowptr,gxptr=gxrowptr,gyptr=gyrowptr,
            // 					resultptr=resultrowptr;colcount<ncols-2;
            // 				colcount++,magptr++,gxptr++,gyptr++,resultptr++){
            m00 = img_mag(x,y).value();
            // 			m00 = *magptr;
            if( m00 == 0.0 )
            {
                result(x,y) = (float) NOEDGE;
                //*resultptr = (unsigned char) NOEDGE;
            }
            else
            {
                xperp = -(gx = img_dx(x,y).value())/m00;
                yperp =  (gy = img_dy(x,y).value())/m00;
                // 				xperp = -(gx = *gxptr)/((float)m00);
                // 				yperp =  (gy = *gyptr)/((float)m00);
            }

            if(gx >= 0.0)
            {
                if(gy >= 0.0)
                {
                    if (gx >= gy)
                    {
                        /* 111 */
                        /* Left point */
                        z1 = img_mag(x-1,y).value();
                        z2 = img_mag(x-1,y-1).value();
                        // 							z1 = *(magptr - 1);
                        // 							z2 = *(magptr - ncols - 1);

                        mag1 = (m00 - z1)*xperp + (z2 - z1)*yperp;

                        /* Right point */
                        z1 = img_mag(x+1,y).value();
                        z2 = img_mag(x+1,y+1).value();
                        // 							z1 = *(magptr + 1);
                        // 							z2 = *(magptr + ncols + 1);

                        mag2 = (m00 - z1)*xperp + (z2 - z1)*yperp;
                    }
                    else
                    {
                        /* 110 */
                        /* Left point */
                        z1 = img_mag(x  ,y-1).value();
                        z2 = img_mag(x-1,y-1).value();
                        // 							z1 = *(magptr - ncols);
                        // 							z2 = *(magptr - ncols - 1);

                        mag1 = (z1 - z2)*xperp + (z1 - m00)*yperp;

                        /* Right point */
                        z1 = img_mag(x  ,y+1).value();
                        z2 = img_mag(x+1,y+1).value();
                        // 							z1 = *(magptr + ncols);
                        // 							z2 = *(magptr + ncols + 1);

                        mag2 = (z1 - z2)*xperp + (z1 - m00)*yperp;
                    }
                }
                else
                {
                    if (gx >= -gy)
                    {
                        /* 101 */
                        /* Left point */
                        z1 = img_mag(x-1,y).value();
                        z2 = img_mag(x-1,y+1).value();
                        // 								z1 = *(magptr - 1);
                        // 								z2 = *(magptr + ncols - 1);

                        mag1 = (m00 - z1)*xperp + (z1 - z2)*yperp;

                        /* Right point */
                        z1 = img_mag(x+1,y).value();
                        z2 = img_mag(x+1,y-1).value();
                        // 								z1 = *(magptr + 1);
                        // 								z2 = *(magptr - ncols + 1);

                        mag2 = (m00 - z1)*xperp + (z1 - z2)*yperp;
                    }
                    else
                    {
                        /* 100 */
                        /* Left point */
                        z1 = img_mag(x  ,y+1).value();
                        z2 = img_mag(x-1,y+1).value();
                        // 								z1 = *(magptr + ncols);
                        // 								z2 = *(magptr + ncols - 1);

                        mag1 = (z1 - z2)*xperp + (m00 - z1)*yperp;

                        /* Right point */
                        z1 = img_mag(x  ,y-1).value();
                        z2 = img_mag(x+1,y-1).value();
                        // 								z1 = *(magptr - ncols);
                        // 								z2 = *(magptr - ncols + 1);

                        mag2 = (z1 - z2)*xperp  + (m00 - z1)*yperp;
                    }
                }
            }
            else
            {
                if (gy >= 0)
                    // 					if ((gy = *gyptr) >= 0)
                {
                    if (-gx >= gy)
                    {
                        /* 011 */
                        /* Left point */
                        z1 = img_mag(x+1,y).value();
                        z2 = img_mag(x+1,y-1).value();
                        // 									z1 = *(magptr + 1);
                        // 									z2 = *(magptr - ncols + 1);

                        mag1 = (z1 - m00)*xperp + (z2 - z1)*yperp;

                        /* Right point */
                        z1 = img_mag(x-1,y).value();
                        z2 = img_mag(x-1,y+1).value();
                        // 									z1 = *(magptr - 1);
                        // 									z2 = *(magptr + ncols - 1);

                        mag2 = (z1 - m00)*xperp + (z2 - z1)*yperp;
                    }
                    else
                    {
                        /* 010 */
                        /* Left point */
                        z1 = img_mag(x  ,y-1).value();
                        z2 = img_mag(x+1,y-1).value();
                        // 									z1 = *(magptr - ncols);
                        // 									z2 = *(magptr - ncols + 1);

                        mag1 = (z2 - z1)*xperp + (z1 - m00)*yperp;

                        /* Right point */
                        z1 = img_mag(x  ,y+1).value();
                        z2 = img_mag(x-1,y+1).value();
                        // 									z1 = *(magptr + ncols);
                        // 									z2 = *(magptr + ncols - 1);

                        mag2 = (z2 - z1)*xperp + (z1 - m00)*yperp;
                    }
                }
                else
                {
                    if (-gx >= -gy)
                    {
                        /* 001 */
                        /* Left point */
                        z1 = img_mag(x+1,y).value();
                        z2 = img_mag(x+1,y+1).value();
                        // 									z1 = *(magptr + 1);
                        // 									z2 = *(magptr + ncols + 1);

                        mag1 = (z1 - m00)*xperp + (z1 - z2)*yperp;

                        /* Right point */
                        z1 = img_mag(x-1,y).value();
                        z2 = img_mag(x-1,y-1).value();
                        // 									z1 = *(magptr - 1);
                        // 									z2 = *(magptr - ncols - 1);

                        mag2 = (z1 - m00)*xperp + (z1 - z2)*yperp;
                    }
                    else
                    {
                        /* 000 */
                        /* Left point */
                        z1 = img_mag(x  ,y+1).value();
                        z2 = img_mag(x+1,y+1).value();
                        // 									z1 = *(magptr + ncols);
                        // 									z2 = *(magptr + ncols + 1);

                        mag1 = (z2 - z1)*xperp + (m00 - z1)*yperp;

                        /* Right point */
                        z1 = img_mag(x  ,y-1).value();
                        z2 = img_mag(x-1,y-1).value();
                        // 									z1 = *(magptr - ncols);
                        // 									z2 = *(magptr - ncols - 1);

                        mag2 = (z2 - z1)*xperp + (m00 - z1)*yperp;
                    }
                }
            }

            /* Now determine if the current point is a maximum point */

            if ((mag1 > 0.0) || (mag2 > 0.0))
            {
                result(x,y) = (float) NOEDGE;
                //*resultptr = (unsigned char) NOEDGE;
            }
            else
            {
                if (fabs(mag2) <= EPS)
                    result(x,y) = (float) NOEDGE;
                //*resultptr = (unsigned char) NOEDGE;
                else
                    result(x,y) = (float) POSSIBLE_EDGE;
                //*resultptr = (unsigned char) POSSIBLE_EDGE;
            }
        }
    }
}

void non_max_supp( const GrayImage &img_mag, int direction, GrayImage &result )
/*******************************************************************/
/* PROCEDURE: non_max_supp                                         */
/* PURPOSE: applies non-maximal suppression to the magnitude of    */
/*          the gradient image.                                    */
/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
/* DATE: 2/15/96                                                   */
/********************************************************************/
{
 
    int count;
    float z1, z2;
    float m00, gx, gy;
    float mag1,mag2,xperp,yperp;
    int ncols, nrows;
    int c,r;
    int x,y;

    nrows = img_mag.height();
    ncols = img_mag.width();

    /* initialize the result image */
    GrayImage tmp( img_mag.width(), img_mag.height() );
    result = tmp;

    /***********************************************************************
      * Zero the edges of the result image.
      ***********************************************************************/
    for(c=0; c<ncols; c++)
    {
        result(c,0)       = (float) NOEDGE;  //0.0;
        result(c,nrows-1) = (float) NOEDGE;  //0.0;
    }

    for(r=0; r<nrows; r++)
    {
        result(0,r)       = (float) NOEDGE;  //0.0;
        result(ncols-1,r) = (float) NOEDGE;  //0.0;
    }

    switch(direction)
    {
    case 0:
        gx =0;
        gy=1;
        break;
    case 45:
        gx=0.5;
        gy=0.5;
        break;
    case 90:
        gx =1;
        gy=0;
        break;
    case 135:
        gx=-0.5;
        gy=0.5;
    }
    //printf("direction: %d: gx=%.1f gy=%.1f\n", direction, gx, gy);
    
    /***********************************************************************
      * Suppress non-maximum points.
      ***********************************************************************/
    for( y=1; y<nrows-1; y++ )
    {
        for( x=1; x<ncols-1; x++ )
        {
            m00 = img_mag(x,y).value();
            if( m00 == 0.0 )
            {
                result(x,y) = (float) NOEDGE;
            }
            else
            {
                xperp = -(gx)/m00;
                yperp =  (gy)/m00;
            }

            if(gx >= 0.0)
            {
                if(gy >= 0.0)
                {
                    if (gx > gy)
                    {
                        /* 111 */
                        /* Left point */
                        z1 = img_mag(x-1,y).value();
                        z2 = img_mag(x-1,y-1).value();
                        mag1 = (m00 - z1)*xperp + (z2 - z1)*yperp;

                        /* Right point */
                        z1 = img_mag(x+1,y).value();
                        z2 = img_mag(x+1,y+1).value();
                        mag2 = (m00 - z1)*xperp + (z2 - z1)*yperp;
                    }
                    else
                    {
                        /* 110 */
                        /* Left point */
                        z1 = img_mag(x  ,y-1).value();
                        z2 = img_mag(x-1,y-1).value();
                        mag1 = (z1 - z2)*xperp + (z1 - m00)*yperp;

                        /* Right point */
                        z1 = img_mag(x  ,y+1).value();
                        z2 = img_mag(x+1,y+1).value();
                        mag2 = (z1 - z2)*xperp + (z1 - m00)*yperp;
                    }
                }
                else
                {
                    if (gx >= -gy)
                    {
                        /* 101 */
                        /* Left point */
                        z1 = img_mag(x-1,y).value();
                        z2 = img_mag(x-1,y+1).value();
                        mag1 = (m00 - z1)*xperp + (z1 - z2)*yperp;

                        /* Right point */
                        z1 = img_mag(x+1,y).value();
                        z2 = img_mag(x+1,y-1).value();
                        mag2 = (m00 - z1)*xperp + (z1 - z2)*yperp;
                    }
                    else
                    {
                        /* 100 */
                        /* Left point */
                        z1 = img_mag(x  ,y+1).value();
                        z2 = img_mag(x-1,y+1).value();
                        mag1 = (z1 - z2)*xperp + (m00 - z1)*yperp;

                        /* Right point */
                        z1 = img_mag(x  ,y-1).value();
                        z2 = img_mag(x+1,y-1).value();
                        mag2 = (z1 - z2)*xperp  + (m00 - z1)*yperp;
                    }
                }
            }
            else
            {
                if (gy >= 0)
                {
                    if (-gx >= gy)
                    {
                        /* 011 */
                        /* Left point */
                        z1 = img_mag(x+1,y).value();
                        z2 = img_mag(x+1,y-1).value();
                        mag1 = (z1 - m00)*xperp + (z2 - z1)*yperp;

                        /* Right point */
                        z1 = img_mag(x-1,y).value();
                        z2 = img_mag(x-1,y+1).value();
                        mag2 = (z1 - m00)*xperp + (z2 - z1)*yperp;
                    }
                    else
                    {
                        /* 010 */
                        /* Left point */
                        z1 = img_mag(x  ,y-1).value();
                        z2 = img_mag(x+1,y-1).value();
                        mag1 = (z2 - z1)*xperp + (z1 - m00)*yperp;

                        /* Right point */
                        z1 = img_mag(x  ,y+1).value();
                        z2 = img_mag(x-1,y+1).value();
                        mag2 = (z2 - z1)*xperp + (z1 - m00)*yperp;
                    }
                }
                else
                {
                    if (-gx >= -gy)
                    {
                        /* 001 */
                        /* Left point */
                        z1 = img_mag(x+1,y).value();
                        z2 = img_mag(x+1,y+1).value();
                        mag1 = (z1 - m00)*xperp + (z1 - z2)*yperp;

                        /* Right point */
                        z1 = img_mag(x-1,y).value();
                        z2 = img_mag(x-1,y-1).value();
                        mag2 = (z1 - m00)*xperp + (z1 - z2)*yperp;
                    }
                    else
                    {
                        /* 000 */
                        /* Left point */
                        z1 = img_mag(x  ,y+1).value();
                        z2 = img_mag(x+1,y+1).value();
                        mag1 = (z2 - z1)*xperp + (m00 - z1)*yperp;

                        /* Right point */
                        z1 = img_mag(x  ,y-1).value();
                        z2 = img_mag(x-1,y-1).value();
                        mag2 = (z2 - z1)*xperp + (m00 - z1)*yperp;
                    }
                }
            }

            /* Now determine if the current point is a maximum point */

            if ((mag1 > 0.0) || (mag2 > 0.0))
            {
                result(x,y) = (float) NOEDGE;            }
            else
            {
                if (fabs(mag2) <= EPS)
                    result(x,y) = (float) NOEDGE;
                else
                    result(x,y) = (float) POSSIBLE_EDGE;
            }
        }
    }
}


void non_max_supp( const GrayImage &img_mag, const GrayImage &img_dx,
                   const GrayImage &img_dy, unsigned char *result )
/*******************************************************************/
/* PROCEDURE: non_max_supp                                         */
/* PURPOSE: applies non-maximal suppression to the magnitude of    */
/*          the gradient image.                                    */
/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
/* DATE: 2/15/96                                                   */
/********************************************************************/
{
    //int rowcount, colcount;
    int count;
    //short *magrowptr,*magptr;
    //short *gxrowptr,*gxptr;
    //short *gyrowptr,*gyptr,z1,z2;
    float z1, z2;
    float m00, gx, gy;
    //short m00,gx,gy;
    float mag1,mag2,xperp,yperp;
    unsigned char *resultrowptr, *resultptr;
    int ncols, nrows;
    int x,y;

    nrows = img_mag.height();
    ncols = img_mag.width();

    /***********************************************************************
      * Zero the edges of the result image.
      ***********************************************************************/
    for(count=0,resultrowptr=result,resultptr=result+ncols*(nrows-1);
            count<ncols; resultptr++,resultrowptr++,count++)
    {
        *resultrowptr = *resultptr = (unsigned char) 0;
    }

    for(count=0,resultptr=result,resultrowptr=result+ncols-1;
            count<nrows; count++,resultptr+=ncols,resultrowptr+=ncols)
    {
        *resultptr = *resultrowptr = (unsigned char) 0;
    }

    /***********************************************************************
      * Suppress non-maximum points.
      ***********************************************************************/
    for( y=1,resultrowptr=result+ncols+1; y<nrows-2; y++,resultrowptr+=ncols )
    {
        // 	for(rowcount=1,magrowptr=mag+ncols+1,gxrowptr=gradx+ncols+1,
        // 				gyrowptr=grady+ncols+1,resultrowptr=result+ncols+1;
        //       rowcount<nrows-2;
        //       rowcount++,magrowptr+=ncols,gyrowptr+=ncols,gxrowptr+=ncols,
        // 				resultrowptr+=ncols){
        for( x=1,resultptr=resultrowptr; x<ncols-2; x++,resultptr++ )
        {
            // 		for(colcount=1,magptr=magrowptr,gxptr=gxrowptr,gyptr=gyrowptr,
            // 					resultptr=resultrowptr;colcount<ncols-2;
            // 				colcount++,magptr++,gxptr++,gyptr++,resultptr++){
            m00 = img_mag(x,y).value();
            // 			m00 = *magptr;
            if( m00 == 0.0 )
            {
                *resultptr = (unsigned char) NOEDGE;
            }
            else
            {
                xperp = -(gx = img_dx(x,y).value())/m00;
                yperp =  (gy = img_dy(x,y).value())/m00;
                // 				xperp = -(gx = *gxptr)/((float)m00);
                // 				yperp =  (gy = *gyptr)/((float)m00);
            }

            if(gx >= 0.0)
            {
                if(gy >= 0.0)
                {
                    if (gx >= gy)
                    {
                        /* 111 */
                        /* Left point */
                        z1 = img_mag(x-1,y).value();
                        z2 = img_mag(x-1,y-1).value();
                        // 							z1 = *(magptr - 1);
                        // 							z2 = *(magptr - ncols - 1);

                        mag1 = (m00 - z1)*xperp + (z2 - z1)*yperp;

                        /* Right point */
                        z1 = img_mag(x+1,y).value();
                        z2 = img_mag(x+1,y+1).value();
                        // 							z1 = *(magptr + 1);
                        // 							z2 = *(magptr + ncols + 1);

                        mag2 = (m00 - z1)*xperp + (z2 - z1)*yperp;
                    }
                    else
                    {
                        /* 110 */
                        /* Left point */
                        z1 = img_mag(x  ,y-1).value();
                        z2 = img_mag(x-1,y-1).value();
                        // 							z1 = *(magptr - ncols);
                        // 							z2 = *(magptr - ncols - 1);

                        mag1 = (z1 - z2)*xperp + (z1 - m00)*yperp;

                        /* Right point */
                        z1 = img_mag(x  ,y+1).value();
                        z2 = img_mag(x+1,y+1).value();
                        // 							z1 = *(magptr + ncols);
                        // 							z2 = *(magptr + ncols + 1);

                        mag2 = (z1 - z2)*xperp + (z1 - m00)*yperp;
                    }
                }
                else
                {
                    if (gx >= -gy)
                    {
                        /* 101 */
                        /* Left point */
                        z1 = img_mag(x-1,y).value();
                        z2 = img_mag(x-1,y+1).value();
                        // 								z1 = *(magptr - 1);
                        // 								z2 = *(magptr + ncols - 1);

                        mag1 = (m00 - z1)*xperp + (z1 - z2)*yperp;

                        /* Right point */
                        z1 = img_mag(x+1,y).value();
                        z2 = img_mag(x+1,y-1).value();
                        // 								z1 = *(magptr + 1);
                        // 								z2 = *(magptr - ncols + 1);

                        mag2 = (m00 - z1)*xperp + (z1 - z2)*yperp;
                    }
                    else
                    {
                        /* 100 */
                        /* Left point */
                        z1 = img_mag(x  ,y+1).value();
                        z2 = img_mag(x-1,y+1).value();
                        // 								z1 = *(magptr + ncols);
                        // 								z2 = *(magptr + ncols - 1);

                        mag1 = (z1 - z2)*xperp + (m00 - z1)*yperp;

                        /* Right point */
                        z1 = img_mag(x  ,y-1).value();
                        z1 = img_mag(x+1,y-1).value();
                        // 								z1 = *(magptr - ncols);
                        // 								z2 = *(magptr - ncols + 1);

                        mag2 = (z1 - z2)*xperp  + (m00 - z1)*yperp;
                    }
                }
            }
            else
            {
                if ((gy = img_dy(x,y).value()) >= 0)
                    // 					if ((gy = *gyptr) >= 0)
                {
                    if (-gx >= gy)
                    {
                        /* 011 */
                        /* Left point */
                        z1 = img_mag(x+1,y).value();
                        z2 = img_mag(x+1,y-1).value();
                        // 									z1 = *(magptr + 1);
                        // 									z2 = *(magptr - ncols + 1);

                        mag1 = (z1 - m00)*xperp + (z2 - z1)*yperp;

                        /* Right point */
                        z1 = img_mag(x-1,y).value();
                        z2 = img_mag(x-1,y+1).value();
                        // 									z1 = *(magptr - 1);
                        // 									z2 = *(magptr + ncols - 1);

                        mag2 = (z1 - m00)*xperp + (z2 - z1)*yperp;
                    }
                    else
                    {
                        /* 010 */
                        /* Left point */
                        z1 = img_mag(x  ,y-1).value();
                        z2 = img_mag(x+1,y-1).value();
                        // 									z1 = *(magptr - ncols);
                        // 									z2 = *(magptr - ncols + 1);

                        mag1 = (z2 - z1)*xperp + (z1 - m00)*yperp;

                        /* Right point */
                        z1 = img_mag(x  ,y+1).value();
                        z2 = img_mag(x-1,y+1).value();
                        // 									z1 = *(magptr + ncols);
                        // 									z2 = *(magptr + ncols - 1);

                        mag2 = (z2 - z1)*xperp + (z1 - m00)*yperp;
                    }
                }
                else
                {
                    if (-gx > -gy)
                    {
                        /* 001 */
                        /* Left point */
                        z1 = img_mag(x+1,y).value();
                        z2 = img_mag(x+1,y+1).value();
                        // 									z1 = *(magptr + 1);
                        // 									z2 = *(magptr + ncols + 1);

                        mag1 = (z1 - m00)*xperp + (z1 - z2)*yperp;

                        /* Right point */
                        z1 = img_mag(x-1,y).value();
                        z2 = img_mag(x-1,y-1).value();
                        // 									z1 = *(magptr - 1);
                        // 									z2 = *(magptr - ncols - 1);

                        mag2 = (z1 - m00)*xperp + (z1 - z2)*yperp;
                    }
                    else
                    {
                        /* 000 */
                        /* Left point */
                        z1 = img_mag(x  ,y+1).value();
                        z2 = img_mag(x+1,y+1).value();
                        // 									z1 = *(magptr + ncols);
                        // 									z2 = *(magptr + ncols + 1);

                        mag1 = (z2 - z1)*xperp + (m00 - z1)*yperp;

                        /* Right point */
                        z1 = img_mag(x  ,y-1).value();
                        z2 = img_mag(x-1,y-1).value();
                        // 									z1 = *(magptr - ncols);
                        // 									z2 = *(magptr - ncols - 1);

                        mag2 = (z2 - z1)*xperp + (m00 - z1)*yperp;
                    }
                }
            }

            /* Now determine if the current point is a maximum point */

            if ((mag1 > 0.0) || (mag2 > 0.0))
            {
                *resultptr = (unsigned char) NOEDGE;
            }
            else
            {
                if (mag2 == 0.0)
                    *resultptr = (unsigned char) NOEDGE;
                else
                    *resultptr = (unsigned char) POSSIBLE_EDGE;
            }
        }
    }
}


/*========================================================================*/
/*         Canny edge detector for an "unsigned char*" image class        */
/*========================================================================*/

void canny( unsigned char *image, int rows, int cols, float sigma,
            float tlow, float thigh, unsigned char **edge )
/*******************************************************************/
/* PROCEDURE: canny                                                */
/* PURPOSE: To perform canny edge detection.                       */
/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
/* DATE: 2/15/96                                                   */
/*******************************************************************/
{
    FILE *fpdir=NULL;          /* File to write the gradient image to.     */
    unsigned char *nms;        /* Points that are local maximal magnitude. */
    short int *smoothedim,     /* The image after gaussian smoothing.      */
    *delta_x,        /* The first devivative image, x-direction. */
    *delta_y,        /* The first derivative image, y-direction. */
    *magnitude;      /* The magnitude of the gadient image.      */
    int r, c, pos;
    //float *dir_radians=NULL;   /* Gradient direction image.                */

    /***********************************************************************
      * Perform gaussian smoothing on the image using the input standard
      * deviation and compute the first derivative in x and y direction.
      ***********************************************************************/
    if(VERBOSE)
        printf("Smoothing the image using a gaussian kernel.\n");
    if(VERBOSE)
        printf("Computing the X and Y first derivatives.\n");
    FastGaussDxDy( image, rows, cols, sigma, &delta_x, &delta_y );

    /*********************************************************************
     * Compute the magnitude of the gradient
     *********************************************************************/
    FastGaussMag( delta_x, delta_y, rows, cols, &magnitude );

    /***********************************************************************
      * Perform non-maximum suppression.
      ***********************************************************************/
    if(VERBOSE)
        printf("Doing the non-maximum suppression.\n");
    if((nms = (unsigned char *) calloc(rows*cols,sizeof(unsigned char)))==NULL)
    {
        fprintf(stderr, "Error allocating the nms image.\n");
        exit(1);
    }

    non_max_supp(magnitude, delta_x, delta_y, rows, cols, nms);

    /***********************************************************************
      * Use hysteresis to mark the edge pixels.
      ***********************************************************************/
    if(VERBOSE)
        printf("Doing hysteresis thresholding.\n");
    if((*edge=(unsigned char *)calloc(rows*cols,sizeof(unsigned char))) ==NULL)
    {
        fprintf(stderr, "Error allocating the edge image.\n");
        exit(1);
    }

    apply_hysteresis(magnitude, nms, rows, cols, tlow, thigh, *edge);

    /***********************************************************************
      * Free all of the memory that we allocated except for the edge image that
      * is still being used to store out result.
      ***********************************************************************/
    free(smoothedim);
    free(delta_x);
    free(delta_y);
    free(magnitude);
    free(nms);
}


void canny( unsigned char *image, int rows, int cols, float sigma,
            float tlow, float thigh, unsigned char **edge,
            float **dir_radians )
/*******************************************************************/
/* PROCEDURE: canny                                                */
/* PURPOSE: To perform canny edge detection.                       */
/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
/* DATE: 2/15/96                                                   */
/*******************************************************************/
{
    FILE *fpdir=NULL;          /* File to write the gradient image to.     */
    unsigned char *nms;        /* Points that are local maximal magnitude. */
    short int *smoothedim,     /* The image after gaussian smoothing.      */
    *delta_x,        /* The first devivative image, x-direction. */
    *delta_y,        /* The first derivative image, y-direction. */
    *magnitude;      /* The magnitude of the gadient image.      */
    int r, c, pos;
    //float *dir_radians=NULL;   /* Gradient direction image.                */

    /***********************************************************************
      * Perform gaussian smoothing on the image using the input standard
      * deviation and compute the first derivative in x and y direction.
      ***********************************************************************/
    if(VERBOSE)
        printf("Smoothing the image using a gaussian kernel.\n");
    if(VERBOSE)
        printf("Computing the X and Y first derivatives.\n");
    FastGaussDxDy( image, rows, cols, sigma, &delta_x, &delta_y,
                   BOOSTBLURFACTOR );

    /*********************************************************************
     * Compute the magnitude and the direction up the gradient, (the latter
     * in radians that are specified counteclockwise from the positive x-axis.)
     *********************************************************************/
    FastGaussMagDir( delta_x, delta_y, rows, cols, &magnitude, dir_radians );

    /***********************************************************************
      * Perform non-maximum suppression.
      ***********************************************************************/
    if(VERBOSE)
        printf("Doing the non-maximum suppression.\n");
    if((nms = (unsigned char *) calloc(rows*cols,sizeof(unsigned char)))==NULL)
    {
        fprintf(stderr, "Error allocating the nms image.\n");
        exit(1);
    }

    non_max_supp(magnitude, delta_x, delta_y, rows, cols, nms);

    /***********************************************************************
      * Use hysteresis to mark the edge pixels.
      ***********************************************************************/
    if(VERBOSE)
        printf("Doing hysteresis thresholding.\n");
    if((*edge=(unsigned char *)calloc(rows*cols,sizeof(unsigned char))) ==NULL)
    {
        fprintf(stderr, "Error allocating the edge image.\n");
        exit(1);
    }

    apply_hysteresis(magnitude, nms, rows, cols, tlow, thigh, *edge);

    /***********************************************************************
      * Free all of the memory that we allocated except for the edge image that
      * is still being used to store out result.
      ***********************************************************************/
    free(smoothedim);
    free(delta_x);
    free(delta_y);
    free(magnitude);
    free(nms);
}


void follow_edges(unsigned char *edgemapptr, short *edgemagptr, short lowval,
                  int cols)
/*******************************************************************/
/* PROCEDURE: follow_edges                                         */
/* PURPOSE: This procedure edges is a recursive routine that       */
/*          traces edgs along all paths whose magnitude values     */
/*          remain above some specifyable lower threshhold.        */
/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
/* DATE: 2/15/96                                                   */
/*******************************************************************/
{
    short *tempmagptr;
    unsigned char *tempmapptr;
    int i;
    float thethresh;
    int x[8] = {1,1,0,-1,-1,-1,0,1},
               y[8] = {0,1,1,1,0,-1,-1,-1};

    for(i=0;i<8;i++)
    {
        tempmapptr = edgemapptr - y[i]*cols + x[i];
        tempmagptr = edgemagptr - y[i]*cols + x[i];

        if((*tempmapptr == POSSIBLE_EDGE) && (*tempmagptr > lowval))
        {
            *tempmapptr = (unsigned char) EDGE;
            follow_edges(tempmapptr,tempmagptr, lowval, cols);
        }
    }
}


void apply_hysteresis(short int *mag, unsigned char *nms, int rows, int cols,
                      float tlow, float thigh, unsigned char *edge)
/*******************************************************************/
/* PROCEDURE: apply_hysteresis                                     */
/* PURPOSE: This routine finds edges that are above some high      */
/*          threshhold or are connected to a high pixel by a path  */
/*          of pixels greater than a low threshold.                */
/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
/* DATE: 2/15/96                                                   */
/********************************************************************/
{
    int r, c, pos, numedges, lowcount, highcount, lowthreshold, highthreshold,
    i, hist[32768], rr, cc;
    short int maximum_mag, sumpix;

    /***********************************************************************
      * Initialize the edge map to possible edges everywhere the non-maximal
      * suppression suggested there could be an edge except for the border. At
      * the border we say there can not be an edge because it makes the
      * follow_edges algorithm more efficient to not worry about tracking an
      * edge off the side of the image.
      ***********************************************************************/
    for(r=0,pos=0;r<rows;r++)
    {
        for(c=0;c<cols;c++,pos++)
        {
            if(nms[pos] == POSSIBLE_EDGE)
                edge[pos] = POSSIBLE_EDGE;
            else
                edge[pos] = NOEDGE;
        }
    }

    for(r=0,pos=0;r<rows;r++,pos+=cols)
    {
        edge[pos] = NOEDGE;
        edge[pos+cols-1] = NOEDGE;
    }
    pos = (rows-1) * cols;
    for(c=0;c<cols;c++,pos++)
    {
        edge[c] = NOEDGE;
        edge[pos] = NOEDGE;
    }

    /***********************************************************************
      * Compute the histogram of the magnitude image. Then use the histogram to
      * compute hysteresis thresholds.
      ***********************************************************************/
    for(r=0;r<32768;r++)
        hist[r] = 0;
    for(r=0,pos=0;r<rows;r++)
    {
        for(c=0;c<cols;c++,pos++)
        {
            if(edge[pos] == POSSIBLE_EDGE)
                hist[mag[pos]]++;
        }
    }

    /***********************************************************************
      * Compute the number of pixels that passed the nonmaximal suppression.
      ***********************************************************************/
    for(r=1,numedges=0;r<32768;r++)
    {
        if(hist[r] != 0)
            maximum_mag = r;
        numedges += hist[r];
    }

    highcount = (int)(numedges * thigh + 0.5);

    /***********************************************************************
      * Compute the high threshold value as the (100 * thigh) percentage point
      * in the magnitude of the gradient histogram of all the pixels that passes
      * non-maximal suppression. Then calculate the low threshold as a fraction
      * of the computed high threshold value. John Canny said in his paper
      * "A Computational Approach to Edge Detection" that "The ratio of the
      * high to low threshold in the implementation is in the range two or three
      * to one." That means that in terms of this implementation, we should
      * choose tlow ~= 0.5 or 0.33333.
      ***********************************************************************/
    r = 1;
    numedges = hist[1];
    while((r<(maximum_mag-1)) && (numedges < highcount))
    {
        r++;
        numedges += hist[r];
    }
    highthreshold = r;
    lowthreshold = (int)(highthreshold * tlow + 0.5);

    if(VERBOSE)
    {
        printf("The input low and high fractions of %f and %f computed to\n",
               tlow, thigh);
        printf("magnitude of the gradient threshold values of: %d %d\n",
               lowthreshold, highthreshold);
    }

    /***********************************************************************
      * This loop looks for pixels above the highthreshold to locate edges and
      * then calls follow_edges to continue the edge.
      ***********************************************************************/
    for(r=0,pos=0;r<rows;r++)
    {
        for(c=0;c<cols;c++,pos++)
        {
            if((edge[pos] == POSSIBLE_EDGE) && (mag[pos] >= highthreshold))
            {
                edge[pos] = EDGE;
                follow_edges((edge+pos), (mag+pos), lowthreshold, cols);
            }
        }
    }

    /***********************************************************************
      * Set all the remaining possible edges to non-edges.
      ***********************************************************************/
    for(r=0,pos=0;r<rows;r++)
    {
        for(c=0;c<cols;c++,pos++)
            if(edge[pos] != EDGE)
                edge[pos] = NOEDGE;
    }
}


void non_max_supp(short *mag, short *gradx, short *grady, int nrows, int ncols,
                  unsigned char *result)
/*******************************************************************/
/* PROCEDURE: non_max_supp                                         */
/* PURPOSE: This routine applies non-maximal suppression to the    */
/*          magnitude of the gradient image.                       */
/* AUTHOR: Mike Heath, modified by Bastian Leibe                   */
/* DATE: 2/15/96                                                   */
/********************************************************************/
{
    int rowcount, colcount,count;
    short *magrowptr,*magptr;
    short *gxrowptr,*gxptr;
    short *gyrowptr,*gyptr,z1,z2;
    short m00,gx,gy;
    float mag1,mag2,xperp,yperp;
    unsigned char *resultrowptr, *resultptr;


    /***********************************************************************
      * Zero the borders of the result image.
      ***********************************************************************/
    /* 000000
       IIIIII
       000000
    */
    for(count=0,resultrowptr=result,resultptr=result+ncols*(nrows-1);
            count<ncols; resultptr++,resultrowptr++,count++)
    {
        *resultrowptr = *resultptr = (unsigned char) 0;
    }
    /* 0 I 0
       0 I 0
       0 I 0
    */
    for(count=0,resultptr=result,resultrowptr=result+ncols-1;
            count<nrows; count++,resultptr+=ncols,resultrowptr+=ncols)
    {
        *resultptr = *resultrowptr = (unsigned char) 0;
    }

    /***********************************************************************
      * Suppress non-maximum points.
      ***********************************************************************/
    for(rowcount=1,magrowptr=mag+ncols+1,gxrowptr=gradx+ncols+1,
            gyrowptr=grady+ncols+1,resultrowptr=result+ncols+1;
            rowcount<nrows-2;
            rowcount++,magrowptr+=ncols,gyrowptr+=ncols,gxrowptr+=ncols,
            resultrowptr+=ncols)
    {
        for(colcount=1,magptr=magrowptr,gxptr=gxrowptr,gyptr=gyrowptr,
                resultptr=resultrowptr;colcount<ncols-2;
                colcount++,magptr++,gxptr++,gyptr++,resultptr++)
        {
            m00 = *magptr;
            if(m00 == 0)
            {
                *resultptr = (unsigned char) NOEDGE;
            }
            else
            {
                xperp = -(gx = *gxptr)/((float)m00);
                yperp = (gy = *gyptr)/((float)m00);
            }

            if(gx >= 0)
            {
                if(gy >= 0)
                {
                    if (gx >= gy)
                    {
                        /* 111 */
                        /* Left point */
                        z1 = *(magptr - 1);
                        z2 = *(magptr - ncols - 1);

                        mag1 = (m00 - z1)*xperp + (z2 - z1)*yperp;

                        /* Right point */
                        z1 = *(magptr + 1);
                        z2 = *(magptr + ncols + 1);

                        mag2 = (m00 - z1)*xperp + (z2 - z1)*yperp;
                    }
                    else
                    {
                        /* 110 */
                        /* Left point */
                        z1 = *(magptr - ncols);
                        z2 = *(magptr - ncols - 1);

                        mag1 = (z1 - z2)*xperp + (z1 - m00)*yperp;

                        /* Right point */
                        z1 = *(magptr + ncols);
                        z2 = *(magptr + ncols + 1);

                        mag2 = (z1 - z2)*xperp + (z1 - m00)*yperp;
                    }
                }
                else
                {
                    if (gx >= -gy)
                    {
                        /* 101 */
                        /* Left point */
                        z1 = *(magptr - 1);
                        z2 = *(magptr + ncols - 1);

                        mag1 = (m00 - z1)*xperp + (z1 - z2)*yperp;

                        /* Right point */
                        z1 = *(magptr + 1);
                        z2 = *(magptr - ncols + 1);

                        mag2 = (m00 - z1)*xperp + (z1 - z2)*yperp;
                    }
                    else
                    {
                        /* 100 */
                        /* Left point */
                        z1 = *(magptr + ncols);
                        z2 = *(magptr + ncols - 1);

                        mag1 = (z1 - z2)*xperp + (m00 - z1)*yperp;

                        /* Right point */
                        z1 = *(magptr - ncols);
                        z2 = *(magptr - ncols + 1);

                        mag2 = (z1 - z2)*xperp  + (m00 - z1)*yperp;
                    }
                }
            }
            else
            {
                if ((gy = *gyptr) >= 0)
                {
                    if (-gx >= gy)
                    {
                        /* 011 */
                        /* Left point */
                        z1 = *(magptr + 1);
                        z2 = *(magptr - ncols + 1);

                        mag1 = (z1 - m00)*xperp + (z2 - z1)*yperp;

                        /* Right point */
                        z1 = *(magptr - 1);
                        z2 = *(magptr + ncols - 1);

                        mag2 = (z1 - m00)*xperp + (z2 - z1)*yperp;
                    }
                    else
                    {
                        /* 010 */
                        /* Left point */
                        z1 = *(magptr - ncols);
                        z2 = *(magptr - ncols + 1);

                        mag1 = (z2 - z1)*xperp + (z1 - m00)*yperp;

                        /* Right point */
                        z1 = *(magptr + ncols);
                        z2 = *(magptr + ncols - 1);

                        mag2 = (z2 - z1)*xperp + (z1 - m00)*yperp;
                    }
                }
                else
                {
                    if (-gx > -gy)
                    {
                        /* 001 */
                        /* Left point */
                        z1 = *(magptr + 1);
                        z2 = *(magptr + ncols + 1);

                        mag1 = (z1 - m00)*xperp + (z1 - z2)*yperp;

                        /* Right point */
                        z1 = *(magptr - 1);
                        z2 = *(magptr - ncols - 1);

                        mag2 = (z1 - m00)*xperp + (z1 - z2)*yperp;
                    }
                    else
                    {
                        /* 000 */
                        /* Left point */
                        z1 = *(magptr + ncols);
                        z2 = *(magptr + ncols + 1);

                        mag1 = (z2 - z1)*xperp + (m00 - z1)*yperp;

                        /* Right point */
                        z1 = *(magptr - ncols);
                        z2 = *(magptr - ncols - 1);

                        mag2 = (z2 - z1)*xperp + (m00 - z1)*yperp;
                    }
                }
            }

            /* Now determine if the current point is a maximum point */

            if ((mag1 > 0.0) || (mag2 > 0.0))
            {
                *resultptr = (unsigned char) NOEDGE;
            }
            else
            {
                if (mag2 == 0.0)
                    *resultptr = (unsigned char) NOEDGE;
                else
                    *resultptr = (unsigned char) POSSIBLE_EDGE;
            }
        }
    }
}
