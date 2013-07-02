/*********************************************************************/
/*                                                                   */
/* FILE         opgrayimage.hh                                       */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Define an image class that combines the pccv         */
/*              Image class with Qt functionality for displaying,    */
/*              and that contains many useful image processing ope-  */
/*              rators.                                              */
/*                                                                   */
/* BEGIN        FRI Aug 17 2001                                      */
/* LAST CHANGE  FRI Jun 07 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_OPGRAYIMAGE_HH
#define LEIBE_OPGRAYIMAGE_HH

#ifdef _USE_PERSONAL_NAMESPACES
namespace Leibe {
#endif
  
/****************/
/*   Includes   */
/****************/
#include <qimage.h>

#include <image.h>
#include <grayimage.hh>

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                         Class OpGrayImage                         */
/*===================================================================*/
/* Define an image class combining the pccv Image class with Qt. */
class OpGrayImage : public GrayImage
{
public:
  OpGrayImage() : GrayImage() {}
  OpGrayImage( const int width, const int height ) : GrayImage(width, height) {}
  OpGrayImage( const GrayImage& other ) : GrayImage(other) {}
  OpGrayImage( const QImage& src );

  OpGrayImage& operator=( OpGrayImage other );

private:
  void copyFrom( const QImage& src );

public:
  /*******************************/
  /*   Content Access Functions  */
  /*******************************/
  QImage getQtImage( bool normalize=true ) const;
  void   getQtImage( QImage &img, bool normalize=true );

  float  getSum();
  void   getMinMax( float &min, float &max );

public:
  /***************************/
  /*   Threshold Operators   */
  /***************************/
  void opThresholdAbove  ( float t1, float valtrue=1.0, float valfalse=0.0 );
  void opThresholdBelow  ( float t1, float valtrue=1.0, float valfalse=0.0 );
  void opThresholdInside ( float t1, float t2, float valtrue=1.0, 
			   float valfalse=0.0 );
  void opThresholdOutside( float t1, float t2, float valtrue=1.0, 
			   float valfalse=0.0 );
  
  /**********************/
  /*   Clip Operators   */
  /**********************/
  void opClipAbove  ( float t1 );
  void opClipBelow  ( float t1 );
  void opClipInside ( float t1, float t2, float clipvalue=0.0 );
  void opClipOutside( float t1, float t2 );
  
  /*****************************/
  /*   Color Range Operators   */
  /*****************************/
  void opNormalizeRange( float rangeMin=0.0, float rangeMax=255.0 );
  OpGrayImage opHistEq();

  /****************************/
  /*   Derivative Operators   */
  /****************************/
  OpGrayImage opGauss        ( double sigma );
  OpGrayImage opGaussDerivGx ( double sigma );
  OpGrayImage opGaussDerivGy ( double sigma );
  OpGrayImage opGaussDerivGxx( double sigma );
  OpGrayImage opGaussDerivGxy( double sigma );
  OpGrayImage opGaussDerivGyy( double sigma );

  OpGrayImage opGaussMag     ( double sigma );
  OpGrayImage opGaussMag2    ( double sigma );
  OpGrayImage opGaussLap     ( double sigma );
    
  OpGrayImage opFastGauss    ( double sigma );
  OpGrayImage opFastGaussDx  ( double sigma );
  OpGrayImage opFastGaussDy  ( double sigma );
  OpGrayImage opFastGaussMag ( double sigma );
  OpGrayImage opFastGaussMag2( double sigma );
  OpGrayImage opFastGaussDir ( double sigma );

  void        opFastGaussDxDy      ( double sigma, 
																		 GrayImage &img_dx, GrayImage &img_dy );
  void        opFastGaussMagDir    ( double sigma, 
																		 GrayImage &img_mag, GrayImage &img_dir,
																		 float multfactor=1.0 );
  void        opFastGaussDxDyMagDir( double sigma, 
																		 GrayImage &img_dx, GrayImage &img_dy,
																		 GrayImage &img_mag, GrayImage &img_dir,
																		 float multfactor=1.0 );

  /**********************/
  /*   Edge Operators   */
  /**********************/
	OpGrayImage opCannyEdges( float sigma, float tlow, float thigh ) const;
	void        opCannyEdges( float sigma, float tlow, float thigh, 
														GrayImage &img_edges, GrayImage &img_dir );
	void        opCannyEdges( float sigma, float tlow, float thigh, 
														GrayImage &img_edges, 
                            GrayImage &img_mag, GrayImage &img_dir );

  /************************/
  /*   Region Operators   */
  /************************/
	OpGrayImage opDistanceTransform();
	OpGrayImage opDistanceFromBorder();
	OpGrayImage opDistanceFromBorder( int x1, int y1, int x2, int y2 );
	OpGrayImage opAreaDistanceTransform( int MAX_COLORS=30000 );
	OpGrayImage opAreaDistanceTransform( int x1, int y1, int x2, int y2, 
																			 int MAX_COLORS=30000 );
	OpGrayImage opConnectedComponents( int minSize=5, int MAX_COLORS=10000 ) const;

  void        opComputeCoG( int &x, int &y );

  /********************************/
  /*   Generic Filter Operators   */
  /********************************/
	void        opCreateGaussKernel1D( float sigma );
	void        opCreateGaussKernel2D( float sigma );

	//GrayImage opConvolveWith( GrayImage &mask );

	//GrayImage opMedianFilter( int radius );

  /*****************************************/
  /*   Non-Maximum Suppression Operators   */
  /*****************************************/
  OpGrayImage opNonMaximumSuppression( float suppressval=0.0 );
  OpGrayImage opNonMaximumSuppression( int windowSize, float suppressval=0.0 );

  /****************************/
  /*   Morphology Operators   */
  /****************************/
	void        opCreateCrossMask ( int radius, float value );
	void        opCreateCircleMask( int radius, float value );

	OpGrayImage opDilate ( GrayImage &mask );
	OpGrayImage opErode  ( GrayImage &mask );
	OpGrayImage opOpening( GrayImage &mask );
	OpGrayImage opClosing( GrayImage &mask );

	OpGrayImage opDilateBlockMask ( int radius, 
																	float FG_VALUE=1.0, float BG_VALUE=0.0 );
	OpGrayImage opErodeBlockMask  ( int radius, 
																	float FG_VALUE=1.0, float BG_VALUE=0.0 );
	OpGrayImage opOpeningBlockMask( int radius, 
																	float FG_VALUE=1.0, float BG_VALUE=0.0 );
	OpGrayImage opClosingBlockMask( int radius, 
																	float FG_VALUE=1.0, float BG_VALUE=0.0 );

  /***************************/
  /*   Rescaling Operators   */
  /***************************/
  OpGrayImage opRescaleToWidth ( int w );
  OpGrayImage opRescaleToHeight( int h );

  OpGrayImage opRescaleToSize  ( int w, int h );
  OpGrayImage opDeformToWidth  ( int w );
  OpGrayImage opDeformToHeight ( int h );


private:
  float  calcIntensity( QRgb rgbval );

};


//-----------------------------------------------------------------------------
// Related Functions
//-----------------------------------------------------------------------------

GrayImage createGaussKernel1D( float sigma );
GrayImage createGaussKernel2D( float sigma );

GrayImage createCrossMask ( int radius, float value );
GrayImage createCircleMask( int radius, float value );

bool 	    isMaximum(GrayImage& img, int x, int y );
bool 	    isMaximum(GrayImage& img, int x, int y, int windowSize );

OpGrayImage cannyEdgesDxDy( const OpGrayImage &imgDx, const OpGrayImage &imgDy,
                            float tlow, float thigh );


#ifdef _USE_PERSONAL_NAMESPACES
}
#endif

#endif // LEIBE_OPGRAYIMAGE_HH
