/*********************************************************************/
/*                                                                   */
/* FILE         visualhistogram.hh                                   */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Extend the histogram class by visualization routines */
/*              for Qt.                                              */
/*                                                                   */
/* BEGIN        MON Aug 27 2001                                      */
/* LAST CHANGE  MON Aug 27 2001                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_VISUALHISTOGRAM_HH
#define LEIBE_VISUALHISTOGRAM_HH

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif
  
/****************/
/*   Includes   */
/****************/
#include <vector>

#include <qimage.h>
#include <qpixmap.h>

#include <grayimage.hh>
#include <opgrayimage.hh>
#include "histogram.hh"
#include <cassert>

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                         Class VisualHistogram                     */
/*===================================================================*/
/* Define a "visual histogram" class derived from the general histogram */
class VisualHistogram : public Histogram
{
public:
  VisualHistogram() 
    : Histogram() {}
  VisualHistogram( int nBins, float min, float max ) 
    : Histogram( nBins, min, max ) {}
  VisualHistogram( int nBins_x, float min_x, float max_x,
		   int nBins_y, float min_y, float max_y )
    : Histogram( nBins_x, min_x, max_x, nBins_y, min_y, max_y ) {}
  VisualHistogram( int nBins_x, float min_x, float max_x,
		   int nBins_y, float min_y, float max_y,
		   int nBins_z, float min_z, float max_z )
    : Histogram( nBins_x, min_x, max_x, nBins_y, min_y, max_y, 
		 nBins_z, min_z, max_z ) {}
  VisualHistogram( int nDims, vector<int> vNumBins, 
		   vector<float> vMinValues, vector<float> vMaxValues )
    : Histogram( nDims, vNumBins, vMinValues, vMaxValues ) {}
  VisualHistogram( const Histogram &other )
    : Histogram( other ) {}
  VisualHistogram( const VisualHistogram &other )
    : Histogram( other ) {}

  //VisualHistogram& operator=( Histogram &other );
  //VisualHistogram& operator=( VisualHistogram &other );
  VisualHistogram& operator=( Histogram other );
  VisualHistogram& operator=( VisualHistogram other );

public:
  /*******************************/
  /*   Histogram Visualization   */
  /*******************************/
  void        drawHistogram( QImage &image, float normalizeMaxValue=-1.0 );

public:
  void        draw1DHistogram( QImage &image, float normalizeMaxValue=-1.0, 
			       bool ScaleXAxisBins=false );
  QPixmap     draw1DHistogram( float normalizeMaxValue=-1.0, 
			       bool ScaleXAxisBins=false );

  void        draw2DHistogram( QImage &image, float normalizeMaxValue=-1.0 );
  OpGrayImage draw2DHistogram();
};

#ifdef _USE_PERSONAL_NAMESPACES
}
#endif

#endif // LEIBE_VISUALHISTOGRAM_HH
