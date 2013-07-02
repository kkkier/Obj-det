/*********************************************************************/
/*                                                                   */
/* FILE         histogram.hh                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Define a histogram class that supports histograms of */
/*              arbitrary dimensions.                                */
/*                                                                   */
/* BEGIN        Fri Aug 24 2001                                      */
/* LAST CHANGE  Wed Jul 24 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_HISTOGRAM_HH
#define LEIBE_HISTOGRAM_HH

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif

//#define NDEBUG
  
/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>
#include <cassert>
#include <fstream>

#include <qimage.h>

#include <grayimage.hh>
#include <featurevector.hh>

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                         Class Histogram                           */
/*===================================================================*/
/* Define a general histogram class */
class Histogram
{
public:
  Histogram();
  Histogram( int nBins, float min, float max );
  Histogram( int nBins_x, float min_x, float max_x,
	     int nBins_y, float min_y, float max_y );
  Histogram( int nBins_x, float min_x, float max_x,
	     int nBins_y, float min_y, float max_y,
	     int nBins_z, float min_z, float max_z );
  Histogram( int nDims, vector<int> vNumBins, 
	     vector<float> vMinValues, vector<float> vMaxValues );
  Histogram( const Histogram &other );
  ~Histogram();

  //Histogram& operator=( Histogram &other );
  Histogram& operator=( Histogram other );

protected:
  void  copyFromOther( const Histogram &other );

  void  initDimension( int dim, int nBins, float min, float max );
  void  initBins();

  int   calcTotalNumBins();

public:
  /*******************************/
  /*   Content Access Functions  */
  /*******************************/
  bool  isValid();

  int   numDims()          { assert( m_bSizeDefined ); return m_nDims; }
  int   numBins( int dim ) 
  { assert( (dim < m_nDims) && m_vDimDefined[dim] ); return m_vNumBins[dim]; }
  float minValue( int dim )
  { assert( (dim<m_nDims) && m_vDimDefined[dim] ); return m_vMinValues[dim]; }
  float maxValue( int dim )
  { assert( (dim<m_nDims) && m_vDimDefined[dim] ); return m_vMaxValues[dim]; }

  void  setNumDims  ( int nDims );
  void  setDimension( int dim, int nBins, float min, float max );

  float at ( int x )                { return m_vBins[idx(x)]; }
  float at ( int x, int y )         { return m_vBins[idx(x,y)]; }
  float at ( int x, int y, int z )  { return m_vBins[idx(x,y,z)]; }
  float at ( vector<int> index )    { return m_vBins[idx(index)]; }

  void  setData( vector<float> data );
  void  setData( vector<double> data );
  vector<float> getData() const     { return m_vBins; }

  void  clear();

  void  print();
  void  printContent();

public:
  /**************************/
  /*   Histogram Creation   */
  /**************************/
  void  createHistogram( const GrayImage &img );
  void  createHistogram( const GrayImage &img, int x1, int y1, int x2, int y2 );
  void  createHistogram( const GrayImage &img, int x1, int y1, float radius );

  void  createHistogram( vector<GrayImage> &vImgBands );
  void  createHistogram( vector<GrayImage> &vImgBands, 
			 int x1, int y1, int x2, int y2 );
  void  createHistogram( vector<GrayImage> &vImgBands, 
			 int x1, int y1, float radius );

  void  createHistogram( RGBImage &rgbImg );
  void  createHistogram( RGBImage &rgbImg, 
			 int x1, int y1, int x2, int y2 );
  void  createHistogram( RGBImage &rgbImg, 
			 int x1, int y1, float radius );

  void  insertValue( float val_x );
  void  insertValue( float val_x, float val_y );
  void  insertValue( float val_x, float val_y, float val_z );
  void  insertValue( vector<float> vValue );

  void  incrementValue( float val_x, float increment );
  void  incrementValue( float val_x, float val_y, float increment );
  void  incrementValue( float val_x, float val_y, float val_z,float increment);
  void  incrementValue( vector<float> vValue, float increment );

public:
  int   getBinNumber   ( int dim, double value );

public:
  /**************************/
  /*   Histogram File I/O   */
  /**************************/
  bool  save( string filename );
  bool  load( string filename, bool verbose=false );

  void  writeHeader( ofstream &ofile );
  void  writeData( ofstream &ofile );
  bool  readHeader( ifstream &ifile, bool &isAscii, bool verbose=false );
  bool  readData( ifstream &ifile, bool isAscii=true );

public:
  /******************************************/
  /*   Histogram Combination/Manipulation   */
  /******************************************/
  void  addHistogram   ( Histogram &other );
  void  subHistogram   ( Histogram &other );

  void  normalizeHistogram( float newSum );
  void  normalizeBins     ( vector<float> vBinFactors );

  Histogram marginalizeOver( int dim );
  Histogram getCumulativeHistogram( int dim );

public:
  /****************************/
  /*   Histogram Statistics   */
  /****************************/
  float getSum();
  float getSumAbs();
  void  getMinMax( float &min, float &max );
  
  float getMedian()         { return getQuantile(0.5); }
  float getMedian( int dim) { return getQuantile(dim, 0.5); }

  float getQuantile( float xpercent, bool fromleft=true );
  float getQuantile( int dim, float xpercent, bool fromleft=true );

  friend void computeHistogramStatistics( vector<Histogram> vHistograms, 
					  vector<float> &vMeans, 
					  vector<float> &vVariances );

public:
  /*************************************/
  /*   Histogram Comparison Measures   */
  /*************************************/
  float compSSD( Histogram &other );
  float compIntersection( Histogram &other, bool bNormalizeResult=true );
  float compChi2Measure( Histogram &other, bool bNormalize=true );
  float compChi2Significance( Histogram &other, bool bNormalize=true );
  float compBhattacharyya( Histogram &other, bool bNormalizeInputs=true );

protected:
  int   idx( int x );
  int   idx( int x, int y );
  int   idx( int x, int y, int z );
  int   idx( vector<int> index );

  vector<int> getNextIndex( const vector<int> &index, 
			    bool &endReached );

  int            m_nDims;
  vector<int>    m_vNumBins;   // contains the #bins for each dimension
  vector<float>  m_vMinValues; // contains the min value for each dimension
  vector<float>  m_vMaxValues; // contains the max value for each dimension
  vector<float>  m_vRes;       // contains the resolution for each dimension

  bool           m_bSizeDefined;
  vector<bool>   m_vDimDefined;
  bool           m_bAllDimensionsDefined;

  int            m_nTotalNumBins;
  vector<float>  m_vBins;     // contains all bins, access via idx function
};


/****************************/
/*   Associated Functions   */
/****************************/
bool saveHistogramList( string filename, vector<Histogram> &vHistograms, 
			bool gzip=true, bool verbose=false );
bool loadHistogramList( string filename, vector<Histogram> &vHistograms, 
			bool verbose=false );

extern bool readKeyWord( ifstream &ifile, string KeyWords );
extern vector<string> extractWords( string WordList );

void computeHistogramStatistics( vector<Histogram> vHistograms, 
				 vector<float> &vMeans, 
				 vector<float> &vVariances );

enum FilterType{ GAUSS, GAUSS_DX, GAUSS_DY, GAUSS_DXX, GAUSS_DXY, GAUSS_DYY};

void getBestHistogramRange( FilterType type, float sigma, 
			    float &min, float &max );

#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif

#endif // LEIBE_HISTOGRAM_HH
