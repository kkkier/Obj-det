/*********************************************************************/
/*                                                                   */
/* FILE         histogram.cc                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Implement a histogram class that supports histograms */
/*              of arbitrary dimensions                              */
/*                                                                   */
/* BEGIN        Fri Aug 17 2001                                      */
/* LAST CHANGE  Wed Jul 24 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif
  
/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <values.h>   // for FLT_MAX
#include <stdlib.h>   // for system()

#include <qimage.h>

#include <chisquare.hh>
#include <featurevector.hh>
#include "histogram.hh"

/*===================================================================*/
/*                         Class Histogram                           */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

Histogram::Histogram()
  /* standard constructor */
{
  clear();
  m_nDims = 0;
  m_bSizeDefined = false;
  m_bAllDimensionsDefined = false;
}


Histogram::Histogram( int nBins, float min, float max )
  /* alternate constructor for 1D histograms */
{
  clear();
  m_nDims = 1;
  m_bSizeDefined = true;

  m_vNumBins.resize(1);
  m_vMinValues.resize(1);
  m_vMaxValues.resize(1);
  m_vRes.resize(1);
  m_vDimDefined.resize(1);

  initDimension( 0, nBins, min, max );
  initBins();
  m_bAllDimensionsDefined = true;
}


Histogram::Histogram( int nBins_x, float min_x, float max_x,
		      int nBins_y, float min_y, float max_y )
  /* alternate constructor for 2D histograms */
{
  clear();
  m_nDims = 2;
  m_bSizeDefined = true;

  m_vNumBins.resize(2);
  m_vMinValues.resize(2);
  m_vMaxValues.resize(2);
  m_vRes.resize(2);
  m_vDimDefined.resize(2);

  initDimension( 0, nBins_x, min_x, max_x );
  initDimension( 1, nBins_y, min_y, max_y );
  initBins();
  m_bAllDimensionsDefined = true;
}


Histogram::Histogram( int nBins_x, float min_x, float max_x,
		      int nBins_y, float min_y, float max_y,
		      int nBins_z, float min_z, float max_z ) 
  /* alternate constructor for 3D histograms */
{
  clear();
  m_nDims = 3;
  m_bSizeDefined = true;

  m_vNumBins.resize(3);
  m_vMinValues.resize(3);
  m_vMaxValues.resize(3);
  m_vRes.resize(3);
  m_vDimDefined.resize(3);

  initDimension( 0, nBins_x, min_x, max_x );
  initDimension( 1, nBins_y, min_y, max_y );
  initDimension( 2, nBins_z, min_z, max_z );
  initBins();
  m_bAllDimensionsDefined = true;
}


Histogram::Histogram( int nDims, vector<int> vNumBins, 
		      vector<float> vMinValues, vector<float> vMaxValues )
  /* alternate constructor for histograms of arbitrary dimension */
{
  assert( vNumBins.size()   == nDims );
  assert( vMinValues.size() == nDims );
  assert( vMaxValues.size() == nDims );

  clear();
  m_nDims        = nDims;
  m_bSizeDefined = true;
  m_vNumBins     = vNumBins;
  m_vMinValues   = vMinValues;
  m_vMaxValues   = vMaxValues;

  m_vDimDefined.resize( m_nDims );  
  m_vRes.resize( m_nDims );  
  for ( int i=0; i<m_nDims; i++ ) {
    m_vRes[i] = (m_vMaxValues[i] - m_vMinValues[i]) / ((float) m_vNumBins[i]);
    m_vDimDefined[i] = true;
  }
  initBins();
  m_bAllDimensionsDefined = true;
}


Histogram::Histogram( const Histogram &other )
  : m_vNumBins(other.m_vNumBins)
  , m_vMinValues(other.m_vMinValues)
  , m_vMaxValues(other.m_vMaxValues)
  , m_vRes(other.m_vRes)
  , m_vBins(other.m_vBins)
  , m_vDimDefined(other.m_vDimDefined)
  /* copy constructor */
{
  copyFromOther( other );
}


Histogram& Histogram::operator=( Histogram other )
  /* assignment operator */
{
  //cout << "    Histogram::operator=() called." << endl;
  copyFromOther( other );
  return *this;
}


Histogram::~Histogram()
  /* standard destructor */
{
  clear();
}


void Histogram::copyFromOther( const Histogram &other )
  /*******************************************************************/
  /* Auxiliary function to copy from another histogram.              */
  /*******************************************************************/
{
  m_nDims        = other.m_nDims;
  m_bSizeDefined = other.m_bSizeDefined;
  m_vNumBins     = other.m_vNumBins;
  m_vMinValues   = other.m_vMinValues;
  m_vMaxValues   = other.m_vMaxValues;
  m_vRes         = other.m_vRes;
  m_vBins        = other.m_vBins;
  m_vDimDefined  = other.m_vDimDefined;
  m_nTotalNumBins= other.m_nTotalNumBins;

  m_bAllDimensionsDefined = other.m_bAllDimensionsDefined;
}


void Histogram::clear()
  /*******************************************************************/
  /* Completely clear this histogram. All size information will be   */
  /* reset to 0, and all bin data will be lost.                      */
  /*******************************************************************/
{
  m_nDims = 0;
  m_bSizeDefined = false;

  m_vNumBins.clear();
  m_vMinValues.clear();
  m_vMaxValues.clear();
  m_vRes.clear();
  m_vDimDefined.clear();
  m_bAllDimensionsDefined = false;

  m_nTotalNumBins = -1;
  m_vBins.clear();
}


void Histogram::initDimension( int dim, int nBins, float min, float max )
  /*******************************************************************/
  /* Initialize the number of bins and bin ranges for one dimension. */
  /*******************************************************************/
{
  assert( m_bSizeDefined );
  assert( dim < m_nDims );

  m_vNumBins[dim]    = nBins;
  m_vMinValues[dim]  = min;
  m_vMaxValues[dim]  = max;
  m_vRes[dim]        = (max - min)/((float) nBins);
  m_vDimDefined[dim] = true;

  /* invalidate the current bin count */
  m_nTotalNumBins = -1;
}


void Histogram::initBins()
  /*******************************************************************/
  /* Initialize all bins to 0. The histogram parameters must be ful- */
  /* ly defined before this function can be called.                  */
  /*******************************************************************/
{
  assert( m_bSizeDefined );

  /* calculate the total number of bins in the histogram */
  int nTotalBins = calcTotalNumBins();

  m_vBins.clear();
  m_vBins.resize( nTotalBins );
  for( int i=0; i<nTotalBins; i++ )
    m_vBins[i] = 0.0;
}


int Histogram::calcTotalNumBins()
  /*******************************************************************/
  /* Return the total number of bins in this histogram. Since this   */
  /* number is used quite often, it is calculated once and will be   */
  /* saved as part of the histogram's internal information.          */
  /*******************************************************************/
{
  assert( isValid() );

  /* check if the number has already been calculated */
  if ( m_nTotalNumBins <= 0 ) {
    /* only calculate the number if it hasn't been calculated before */
    m_nTotalNumBins = 1;
    for ( int i=0; i<m_nDims; i++ )
      m_nTotalNumBins *= m_vNumBins[i];
  }
  
  return m_nTotalNumBins;  
}


/***********************************************************/
/*                    Access Functions                     */
/***********************************************************/

bool Histogram::isValid()
  /*******************************************************************/
  /* Check if the histogram definition is valid, that is if the size */
  /* has been specified, and all dimensions have been defined. This  */
  /* function is mostly used in assert() statements, but it can also */
  /* be called from external programs.                               */
  /*******************************************************************/
{
  if ( !m_bSizeDefined )
    return false;

  else 
    if ( m_bAllDimensionsDefined )
      return true;
  
    else {
      /* check whether all dimensions have been defined, but the guard */
      /* variable has not been updated yet.                            */
      for ( int i=0; i < m_nDims; i++ )
	if ( !m_vDimDefined[i] )
	  return false;
      
      /* if all dimensions have been defined, update the variable. */
      m_bAllDimensionsDefined = true;
      return true;
    }
}


void  Histogram::setNumDims  ( int nDims )
  /*******************************************************************/
  /* Set or change the number of dimensions in the histogram. This   */
  /* can be useful, for example, if the histogram has been instan-   */
  /* tiated with the standard constructor and the size has not yet   */
  /* been specified.                                                 */
  /*******************************************************************/
{
  clear();
  m_nDims = nDims;
  m_bSizeDefined = true;

  m_vNumBins.resize(nDims);
  m_vMinValues.resize(nDims);
  m_vMaxValues.resize(nDims);
  m_vRes.resize(nDims);
  m_vDimDefined.resize(nDims);

}


void  Histogram::setDimension( int dim, int nBins, float min, float max )
  /*******************************************************************/
  /* Set or change the number of bins and the bin range for one di-  */
  /* mension in the histogram. The number of dimensions must have    */
  /* been specified before this function can be called! If the his-  */
  /* togram contained data before this function is called, this date */
  /* will be invalidated!                                            */
  /*******************************************************************/
{
  initDimension( dim, nBins, min, max );

  if( isValid() )
    initBins();
}


void  Histogram::setData( vector<float> data )
  /* set the histogram data to the given float values */
{
  assert( isValid() );

  /* check if the histogram size matches the data */
  int nTotalBins = calcTotalNumBins();
  assert( nTotalBins == data.size() );
  
  /* copy the data values */
  for( int i=0; i<nTotalBins; i++ )
    m_vBins[i] = data[i];
}


void  Histogram::setData( vector<double> data )
  /* set the histogram data to the given double values */
{
  assert( isValid() );

  /* check if the histogram size matches the data */
  int nTotalBins = calcTotalNumBins();
  assert( nTotalBins == data.size() );
  
  /* copy the data values */
  for( int i=0; i<nTotalBins; i++ )
    m_vBins[i] = (float) data[i];
}


void  Histogram::print()
  /*******************************************************************/
  /* Print a text description of this histogram (useful for debug-   */
  /* ging). The histogram content is not printed.                    */
  /*******************************************************************/
{
  cout << "  Histogram description:" << endl;
  if ( !m_bSizeDefined )
    cout << "    #Dimensions: not defined!" << endl;

  else {
    cout << "    #Dimensions: " << m_nDims << endl;
    
    /* print information about every dimension */
    for( int i=0; i<m_nDims; i++ )
      if ( !m_vDimDefined[i] )
	cout << "    Dimension " << i << ": not defined!" << endl;
      else {
	cout << "    Dimension " << i << ":" << endl;
	cout << "      #bins     : " << m_vNumBins[i] << endl;
	cout << "      minimum   : " << m_vMinValues[i] << endl;
	cout << "      maximum   : " << m_vMaxValues[i] << endl;
	cout << "      resolution: " << m_vRes[i] << endl;	
      }
    }
  
  cout << "  -----------------------------------" << endl;
}


void  Histogram::printContent()
  /*******************************************************************/
  /* Print a text description of the histogram content.              */
  /*******************************************************************/
{
  assert( isValid() );

  cout << "  Histogram content:" << endl;
  if( m_nDims > 2 ) {
    cout << "    Cannot display histograms with more than 2 dimensions." 
         << endl << endl;
    return;
  }

  if( m_nDims == 1 )
    for( int x=0; x<m_vNumBins[0]; x++ )
      cout << "    Bin " << setw(2) << x << ": " << setw(4) << at(x) << endl;
  else
    for( int y=0; y<m_vNumBins[1]; y++ ) {
      cout << "    Row " << setw(2) << y << ": ";
      for( int x=0; x<m_vNumBins[0]; x++ )
        cout << setw(4) << at(x,y) << "  " << endl;
    }
  
  cout << "  -----------------------------------" << endl;
}


/***********************************************************/
/*                   Histogram Creation                    */
/***********************************************************/

void  Histogram::createHistogram( const GrayImage &img )
  /*******************************************************************/
  /* Create a 1D histogram over the whole image.                     */
  /*******************************************************************/
{ createHistogram( img, 0, 0, img.width()-1, img.height()-1 ); }


void  Histogram::createHistogram( const GrayImage &img, 
				  int x1, int y1, int x2, int y2 )
  /*******************************************************************/
  /* Create a 1D histogram over a rectangular image region.          */
  /*******************************************************************/
{
  //cout << "  Histogram::createHistogram( GrayImage&,int,int,int,int) called."
  //     << endl;
  assert( isValid() );
  assert( m_nDims == 1 );

  /* initialize the bins to 0.0 */
  initBins();

  /* enter every pixel into the histogram */
  for ( int row = y1; row <= y2; row++ )
    for ( int col = x1; col <= x2; col++ ) {

      /* calculate the bin index */
      int binidx = getBinNumber( 0, img(col,row).value() );

      /* update the histogram */
      m_vBins[idx(binidx)] += 1.0;
    }
  //cout << "  done." << endl;
}


void  Histogram::createHistogram( const GrayImage &img, 
				  int x1, int y1, float radius )
  /*******************************************************************/
  /* Create a 1D histogram over a circular image region.             */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == 1 );

  float radius2 = radius*radius;

  /* initialize the bins to 0.0 */
  initBins();

  /* enter every pixel into the histogram */
  int min_y = y1 - ((int) floor( radius ));
  int max_y = y1 + ((int) floor( radius ));
  int min_x = x1 - ((int) floor( radius ));
  int max_x = x1 + ((int) floor( radius ));
  for ( int row = min_y; row <= max_y; row++ )
    for ( int col = min_x; col <= max_x; col++ ) {

      /* if the pixel is inside the circular region */
      float dist_x = (float)( col - x1 );
      float dist_y = (float)( row - y1 );
      if ( (dist_x*dist_x + dist_y*dist_y) <= radius2 ) {
	
	/* calculate the bin index */
	int binidx = getBinNumber( 0, img(col,row).value() );

	/* update the histogram */
	m_vBins[idx(binidx)] += 1.0;
      }
    }
}


void  Histogram::createHistogram( vector<GrayImage> &vImgBands )
  /*******************************************************************/
  /* Create a multi-D histogram over the whole image.                */
  /*******************************************************************/
{ 
  assert( vImgBands.size() >= 1 );

  createHistogram( vImgBands, 0, 0, vImgBands[0].width()-1, 
		   vImgBands[0].height()-1 ); 
}


void  Histogram::createHistogram( vector<GrayImage> &vImgBands, 
				  int x1, int y1, int x2, int y2 )
  /*******************************************************************/
  /* Create a multi-D histogram over a rectangular image region.     */
  /*******************************************************************/
{
  //cout << "  Histogram::createHistogram( vector<GrayImage>&,int,int,int,int) called."
  //    << endl;
  assert( isValid() );
  assert( m_nDims == vImgBands.size() );

  /* initialize the bins to 0.0 */
  initBins();

  /* reserve space for the bin indizes */
  //cout << "    Reserving space for the bin indizes..." << endl;
  vector<int> binidx( m_nDims );

  /* enter every pixel into the histogram */
  //cout << "    Entering every pixel into the histogram..." << endl;
  for ( int row = y1; row <= y2; row++ )
    for ( int col = x1; col <= x2; col++ ) {
      
      //cout << "      Processing pixel (" << col << "," << row 
      //     << ") of (" << vImgBands[0].width() << "," 
      //     << vImgBands[0].height() << ")..." << endl;
      /* for every image band (=histogram dimension) */
      /* calculate the bin index                     */
      for ( int dim = 0; dim < m_nDims; dim++ ) {
	//GrayImage img = vImgBands[dim];

	//binidx[dim] = getBinNumber( dim, img(col,row).value() );
	binidx[dim] = getBinNumber( dim, (vImgBands[dim])(col,row).value() );
       }

      /* update the histogram */
      m_vBins[idx(binidx)] += 1.0;
    }
  //cout << "  done." << endl;
}


void  Histogram::createHistogram( vector<GrayImage> &vImgBands, 
				  int x1, int y1, float radius )
  /*******************************************************************/
  /* Create a multi-D histogram over a circular image region.        */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == vImgBands.size() );

  float radius2 = radius*radius;

  /* initialize the bins to 0.0 */
  initBins();

  /* reserve space for the bin indizes */
  vector<int> binidx( m_nDims );

  /* enter every pixel into the histogram */
  int min_y = y1 - ((int) floor( radius ));
  int max_y = y1 + ((int) floor( radius ));
  int min_x = x1 - ((int) floor( radius ));
  int max_x = x1 + ((int) floor( radius ));
  for ( int row = min_y; row <= max_y; row++ )
    for ( int col = min_x; col <= max_x; col++ ) {

      /* if the pixel is inside the circular region */
      float dist_x = (float)( col - x1 );
      float dist_y = (float)( row - y1 );
      if ( (dist_x*dist_x + dist_y*dist_y) <= radius2 ) {

	/* for every image band (=histogram dimension) */
	/* calculate the bin index                     */
	for ( int dim = 0; dim < m_nDims; dim++ ) {
	  GrayImage img = vImgBands[dim];
	  
	  binidx[dim] = getBinNumber( dim, img(col,row).value() );
	}

	/* update the histogram */
	m_vBins[idx(binidx)] += 1.0;
      }
    }
}


void  Histogram::createHistogram( RGBImage &rgbImg )
  /*******************************************************************/
  /* Create a 3D histogram over the whole RGB image.                 */
  /*******************************************************************/
{ createHistogram( rgbImg, 0, 0, rgbImg.width()-1, rgbImg.height()-1 ); }


void  Histogram::createHistogram( RGBImage &rgbImg, 
		       int x1, int y1, int x2, int y2 )
  /*******************************************************************/
  /* Create a 3D histogram over a rectangular image region of the    */
  /* RGB image.                                                      */
  /*******************************************************************/
{
  vector<GrayImage> vImgBands = extractImageBands( rgbImg );
  createHistogram( vImgBands, x1, y1, x2, y2 );
}


void  Histogram::createHistogram( RGBImage &rgbImg, 
		       int x1, int y1, float radius )
  /*******************************************************************/
  /* Create a 3D histogram over a circular image region of the RGB   */
  /* image.                                                          */
  /*******************************************************************/
{
  vector<GrayImage> vImgBands = extractImageBands( rgbImg );
  createHistogram( vImgBands, x1, y1, radius );
}


void  Histogram::insertValue( float val_x )
  /*******************************************************************/
  /* Insert a value into a 1D histogram.                             */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == 1 );

  /* calculate the bin index and update the histogram */
  int binidx = getBinNumber( 0, val_x );
  m_vBins[idx(binidx)] += 1.0;
}


void  Histogram::insertValue( float val_x, float val_y )
  /*******************************************************************/
  /* Insert a value into a 2D histogram.                             */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == 2 );

  /* calculate the bin index and update the histogram */
  int binidx_x = getBinNumber( 0, val_x );
  int binidx_y = getBinNumber( 1, val_y );
  m_vBins[idx(binidx_x, binidx_y)] += 1.0;
}


void  Histogram::insertValue( float val_x, float val_y, float val_z )
  /*******************************************************************/
  /* Insert a value into a 3D histogram.                             */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == 3 );

  /* calculate the bin index and update the histogram */
  int binidx_x = getBinNumber( 0, val_x );
  int binidx_y = getBinNumber( 1, val_y );
  int binidx_z = getBinNumber( 2, val_z );
  m_vBins[idx(binidx_x, binidx_y, binidx_z)] += 1.0;
}


void  Histogram::insertValue( vector<float> vValue )
  /*******************************************************************/
  /* Insert a value into a histogram of arbitrary dimension. The     */
  /* value must be a vector of the same dimension as the histogram.  */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == vValue.size() );

  /* reserve space for the bin indizes */
  vector<int> binidx( m_nDims );

  /* calculate the bin index and update the histogram */
  for ( int dim = 0; dim < m_nDims; dim++ )
    binidx[dim] = getBinNumber( dim, vValue[dim] );
    
  m_vBins[idx(binidx)] += 1.0;
}


void  Histogram::incrementValue( float val_x, float increment=1.0 )
  /*******************************************************************/
  /* Increment the bin where the value would be placed in a 1D       */
  /* histogram.                                                      */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == 1 );

  /* calculate the bin index and update the histogram */
  int binidx = getBinNumber( 0, val_x );
  m_vBins[idx(binidx)] += increment;
}


void  Histogram::incrementValue( float val_x, float val_y, 
				 float increment )
  /*******************************************************************/
  /* Increment the bin where the value would be placed in a 2D       */
  /* histogram.                                                      */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == 2 );

  /* calculate the bin index and update the histogram */
  int binidx_x = getBinNumber( 0, val_x );
  int binidx_y = getBinNumber( 1, val_y );
  m_vBins[idx(binidx_x, binidx_y)] += increment;
}


void  Histogram::incrementValue( float val_x, float val_y, float val_z, 
				 float increment )
  /*******************************************************************/
  /* Increment the bin where the value would be placed in a 3D       */
  /* histogram.                                                      */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == 3 );

  /* calculate the bin index and update the histogram */
  int binidx_x = getBinNumber( 0, val_x );
  int binidx_y = getBinNumber( 1, val_y );
  int binidx_z = getBinNumber( 2, val_z );
  m_vBins[idx(binidx_x, binidx_y, binidx_z)] += increment;
}


void  Histogram::incrementValue( vector<float> vValue, float increment )
  /*******************************************************************/
  /* Increment the bin where the value would be placed in a histo-   */
  /* gram of arbitrary dimension. The value must be a vector of the  */
  /* same dimension as the histogram.                                */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == vValue.size() );

  /* reserve space for the bin indizes */
  vector<int> binidx( m_nDims );

  /* calculate the bin index and update the histogram */
  for ( int dim = 0; dim < m_nDims; dim++ )
    binidx[dim] = getBinNumber( dim, vValue[dim] );
    
  m_vBins[idx(binidx)] += increment;
}


int   Histogram::getBinNumber( int dim, double value )
  /* return the number of the bin along dimension dim in which this  */
  /* value must be put.                                              */
{
  assert( isValid() );
  assert( dim < m_nDims );
  
  int binidx;
  if ( value <= m_vMinValues[dim] )
    binidx = 0;
  else if ( value >= m_vMaxValues[dim] )
    binidx = m_vNumBins[dim] - 1;
  else
    binidx = (int) floor( ((value - m_vMinValues[dim]) / m_vRes[dim]) );

  return binidx;
}


/***********************************************************/
/*                    Histogram File I/O                   */
/***********************************************************/

bool  Histogram::save( string filename )
  /*******************************************************************/
  /* Save this histogram to disk in text file format. The histogram  */
  /* will be saved under the given file name. If a file with this    */
  /* name already exists, it will be overwritten.                    */
  /*******************************************************************/
{
  ofstream ofile;

  /* try to open the file */
  ofile.open( filename.c_str() );
  if ( !ofile ) {
    cerr << "Error in Histogram::save(): Couldn't open file '" << filename 
	 << "'!" << endl;
    return false;
  }
 
  /* write the key word 'Histogram' */
  ofile << "Histogram" << endl;

  /* write the header information */
  writeHeader( ofile );

  /* write the data key word */
  ofile << "Data:" << endl;

  /* write the data */
  writeData( ofile );

  /* close the file */
  ofile.close();

  return true;
}


void  Histogram::writeHeader( ofstream &ofile )
{
  /* write the number of dimensions */
  ofile << "Dimensions:" << endl;
  ofile << m_nDims << endl;
  
  /* write the bin sizes and ranges */
  ofile << "Bins:" << endl;
  for ( int i=0; i<m_nDims; i++ )
    ofile << m_vNumBins[i] << " " << m_vMinValues[i] << " " << m_vMaxValues[i] << endl;

  /* write the data file format */
  ofile << "Format:" << endl;
  ofile << "Ascii" << endl;
}


void  Histogram::writeData( ofstream &ofile )
{
  /* write the data key word */
  //ofile << "Data:" << endl;

  /* write the data itself in Ascii format */
  int nTotalBins = calcTotalNumBins();
  for ( int i=0; i<nTotalBins; i++ )
    ofile << m_vBins[i] << " ";
  ofile << endl;
}


bool  Histogram::load( string filename, bool verbose )
  /*******************************************************************/
  /* Load the histogram from the given file. The file must exist and */
  /* must be in text format. The function will return 'true' in case */
  /* of success, otherwise 'false'.                                  */
  /*******************************************************************/
{
  ifstream ifile;
  bool     success = false;
  bool     isAscii = true;

  /* clear this histogram */
  clear();

  /* try to open the file */
  ifile.open( filename.c_str() );
  if ( !ifile ) {
    cerr << "Error in Histogram::load(): Couldn't open file '" << filename 
	 << "'!" << endl;
    return false;
  }

  /* read the first line => determines the type of the file, must be */
  /* 'Histogram'                                                     */
  if ( !readKeyWord( ifile, "Histogram histogram HISTOGRAM" ) ) {
    cerr << "Error in Histogram::load(): Not a histogram file!" << endl;
    return false;
  }
  if ( verbose )
    cout << "  Loading histogram..." << endl;

  /* read the header information */
  success = readHeader( ifile, isAscii, verbose );
  
  if ( success )
    /* read the data */
    success = readData( ifile, isAscii );
    
  /* close the file */
  ifile.close();
  
  return success;
}


bool  Histogram::readHeader( ifstream &ifile, bool &isAscii, 
			     bool verbose )
{
  /* read the next line => must be key word 'Dimensions:' */
  if ( !readKeyWord( ifile, "Dimensions: dimensions:" ) )
    return false;

  /* read the number of dimensions */
  int nDims;
  ifile >> nDims;
  if ( verbose )
    cout << "    Dimensions: " << nDims << endl;
  setNumDims( nDims );

  /* read the next line => must be key word 'Bins:' */
  if ( !readKeyWord( ifile, "Bins: bins:" ) )
    return false;

  /* read the bin sizes and ranges */
  for ( int i=0; i<nDims; i++ ) {
    int nBins;
    float min, max;
    ifile >> nBins >> min >> max;
    if ( verbose ) {
      cout << "    Dimension " << i << ":" << endl;
      cout << "      #bins     : " << nBins << endl;
      cout << "      minimum   : " << min << endl;
      cout << "      maximum   : " << max << endl;
    }
    setDimension( i, nBins, min, max );
  }
  
  /* read the next line => must be key word 'Format:' */
  if ( !readKeyWord( ifile, "Format: format:" ) )
    return false;

  /* read the next line => must be key word 'ASCII' or 'BIN' */
  string format;
  isAscii = false;
  ifile >> format;
  if ( (format == "ascii") || (format == "Ascii") || (format == "ASCII") ) {
    if ( verbose )
      cout << "    Reading data in ASCII format..." << endl;
    isAscii = true;

  } else if ( (format == "binary") || (format == "Binary") ||
	      (format == "BINARY") ) {
    if ( verbose )
      cout << "    Reading data in binary format..." << endl;
    isAscii = false;

  } else {
    cerr << "Error in Histogram::readHeader(): "
	 << "Expected key word 'Ascii' or 'Binary'." << endl;
    return false;
  }

  return true;
}


bool  Histogram::readData( ifstream &ifile, bool isAscii )
{
  /* read the next line => must be key word 'Data:' */
  string line;
  
  /* read key word 'Data:' */
  if ( !readKeyWord( ifile, "Data: data:" ) )
    return false;

  /* read the data */
  if ( isAscii ) {
    /* read the data as ASCII values */
    int nTotalBins = calcTotalNumBins();
    for( int i=0; i<nTotalBins; i++ ) {
      float value;
      ifile >> value;
      m_vBins[i] = value;
    }
      
  } else {
    /* read the data in binary format */
    cerr << "    Sorry! Not implemented yet!" << endl;
    return false;
  }

  return true;
}


/***********************************************************/
/*           Histogram Combination/Manipulation            */
/***********************************************************/

void  Histogram::addHistogram( Histogram &other )
  /*******************************************************************/
  /* Add the content of another histogramm to this one. The other    */
  /* histogram must have the same dimension and number of bins.      */
  /*******************************************************************/
{
  /* check if the two histograms are compatible */
  assert( isValid() && other.isValid() );
  assert( m_nDims == other.m_nDims );
  for ( int dim = 0; dim<m_nDims; dim++ ) {
    assert( m_vNumBins[dim] == other.m_vNumBins[dim] );
  }

  /* for all cells of the new histogram */
  int nTotalBins = calcTotalNumBins();
  for ( int i=0; i<nTotalBins; i++ )
    /* add their content to the respective histogram cell */
    /* in this histogram.                                 */
    m_vBins[i] += other.m_vBins[i];
}


void  Histogram::subHistogram( Histogram &other )
  /*******************************************************************/
  /* Subtract the content of another histogramm to this one. The     */
  /* other histogram must have the same dimensions and number of     */
  /* bins.                                                           */
  /*******************************************************************/
{
  /* check if the two histograms are compatible */
  assert( isValid() && other.isValid() );
  assert( m_nDims == other.m_nDims );
  for ( int dim = 0; dim<m_nDims; dim++ ) {
    assert( m_vNumBins[dim] == other.m_vNumBins[dim] );
  }

  /* for all cells of the new histogram */
  int nTotalBins = calcTotalNumBins();
  for ( int i=0; i<nTotalBins; i++ )
    /* subtract their content from the respective histogram cell */
    /* in this histogram.                                        */
    m_vBins[i] -= other.m_vBins[i];
}


void Histogram::normalizeHistogram( float newSum )
  /*******************************************************************/
  /* Normalize this histogram to a fixed sum.                        */
  /*******************************************************************/
{
  assert( isValid() );

  /* calculate the current sum */
  float oldSum = getSum();
  int nTotalBins = calcTotalNumBins();

  /* and divide each histogram cell by this value */
  float factor = newSum / oldSum;
  for ( int i=0; i<nTotalBins; i++ )
    m_vBins[i] *= factor;
}


void  Histogram::normalizeBins( vector<float> vBinFactors )
  /*******************************************************************/
  /* Normalize every histogram bin with a (potentially) different    */
  /* factor. This may be useful, for example, to normalize by bin    */
  /* variances (in order to get Mahalanobis distances).              */
  /*******************************************************************/
{
  assert( isValid() );

  int nTotalBins = calcTotalNumBins();
  assert( vBinFactors.size() == nTotalBins );

  /* divide each histogram cell by the respective factor */
  for ( int i=0; i<nTotalBins; i++ )
    m_vBins[i] /= vBinFactors[i];
}


Histogram Histogram::marginalizeOver( int dim )
  /*******************************************************************/
  /* Marginalize over a certain dimension. This means, the specified */
  /* dimension will be "flattened" to just one bin, and all resul-   */
  /* ting histogram cells will contain the sum over the flattened    */
  /* bins with the same remaining coordinates. The resulting histo-  */
  /* will thus have one dimension less.                              */
  /*******************************************************************/
{
  assert( isValid() );
  /* the marginalized histogram must have at least one dimension */
  assert( m_nDims > 1 );
  assert( (dim >= 0) && (dim < m_nDims) );

  /* create the result histogram with the right set of dimensions */
  Histogram result;
  result.setNumDims( m_nDims - 1 );
  int newdim = 0;
  int olddim = 0;
  for ( olddim=0; olddim<m_nDims; olddim++ )
    if ( olddim != dim ) {
      result.setDimension( newdim, m_vNumBins[olddim], 
			   m_vMinValues[olddim], m_vMaxValues[olddim] );
      newdim++;
    }

  /* Marginalize the histogram data and transfer it */
  vector<int> newindex( m_nDims - 1, 0 );  /* set to (0,0,..,0) */
  vector<int> oldindex( m_nDims );
  /* iterate through all cells of the new histogram */
  bool endReached = false;
  while ( !endReached ) {
    /* copy the other dimensions of the new index to the old index */
    for( int i=0; i<dim; i++ )
      oldindex[i] = newindex[i];
    for( int i=dim+1; i<m_nDims; i++ )
      oldindex[i] = newindex[i-1];

    /* sum over the dimension to be marginalized */
    int index = result.idx(newindex); 
    result.m_vBins[index] = 0.0;
    for( int i=0; i<m_vNumBins[dim]; i++ ) {
      oldindex[dim] = i;
      result.m_vBins[index] += m_vBins[idx(oldindex)];
    }

    /* proceed to the next cell */
    newindex = result.getNextIndex( newindex, endReached );
  }
    
  return result;
}


Histogram Histogram::getCumulativeHistogram( int dim )
  /*******************************************************************/
  /* Integrate over the given dimension in the current histogram and */
	/* return the resulting cumulative distribution.                   */
  /*******************************************************************/
{
  assert( isValid() );
  /* the histogram must have at least one dimension */
  assert( m_nDims > 0 );
  assert( (dim >= 0) && (dim < m_nDims) );

  /* create the result histogram as a copy of the given histogram */
  Histogram result( *this );

	/* create the cumulative histogram */
	vector<int> index( m_nDims, 0 );
	vector<int> lookupidx( m_nDims );
  bool endReached = false;
  while ( !endReached ) {
		/* start with the second cell in dimension dim */
		while ( (index[dim] == 0) && !endReached )
			/* proceed to the next cell */
			index = getNextIndex( index, endReached );

		if( !endReached ) {
			/* Add the content of the neighboring cell in dimension dim to   */
			/* to the current cell,                                          */
			/*   e.g. result[x,y] += result[x-1,y] */
			lookupidx = index;
			lookupidx[dim]--;
			result.m_vBins[idx(index)] += result.m_vBins[idx(lookupidx)];
		}

		/* proceed to the next cell */
		index = getNextIndex( index, endReached );
	}
    
  return result;
}


/***********************************************************/
/*                  Histogram Statistics                   */
/***********************************************************/

float Histogram::getSum()
  /*******************************************************************/
  /* Compute the sum over all histogram cells.                       */
  /*******************************************************************/
{
  assert( isValid() );

  float sum = 0.0;
  int nTotalBins = calcTotalNumBins();
  for ( int i=0; i<nTotalBins; i++ )
    sum += m_vBins[i];
  return sum;
}


float Histogram::getSumAbs()
  /*******************************************************************/
  /* Compute the sum of absolute values over all histogram cells.    */
  /*******************************************************************/
{
  assert( isValid() );

  float sum = 0.0;
  int nTotalBins = calcTotalNumBins();
  for ( int i=0; i<nTotalBins; i++ )
    sum += fabs(m_vBins[i]);
  return sum;
}


void  Histogram::getMinMax( float &min, float &max )
  /*******************************************************************/
  /* Get the minimum and maximum cell values in the histogram.       */
  /*******************************************************************/
{
  assert( isValid() );
  
  min =  99999999999.9;
  max = -99999999999.9;
  int nTotalBins = calcTotalNumBins();
  for ( int i=0; i<nTotalBins; i++ ) {
    if ( m_vBins[i] < min )
      min = m_vBins[i];
    if ( m_vBins[i] > max ) 
      max = m_vBins[i];
  }
}


float Histogram::getQuantile( float xpercent, bool fromleft )
  /*******************************************************************/
  /* Get the value (not the number) of the histogram bin up to which */
  /* x% of the data values have occurred in the histogram. This      */
  /* version of the function is for the special case of 1D histo-    */
  /* grams.                                                          */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == 1 );
  assert( (xpercent >= 0.0) && (xpercent <= 1.0) );

  float percent;
  bool  startfromleft;
  if( xpercent <= 0.5 ) {
    percent = xpercent;
    startfromleft = fromleft;
  } else {
    percent = 1.0 - xpercent;
    startfromleft = !fromleft;
  }
 
  /* calculate the number of the bin */
  float goalsum = percent*getSum();
  float current = 0.0;
  int   binidx  = 0;
  bool  foundbin = false;
  if ( startfromleft ) {
    /* traverse the bins from left to right */
    for( int x=0; x<m_vNumBins[0]; x++ )
      if( !foundbin ) {
	current += m_vBins[idx(x)];
	if ( current >= goalsum ) {
	  binidx = x;
	  foundbin = true;
	}
      }
    
    if ( !foundbin )
      binidx = m_vNumBins[0]-1;

  } else {
    /* traverse the bins from right to left */
    for( int x=m_vNumBins[0]-1; x>=0; x-- )
      if( !foundbin ) {
	current += m_vBins[idx(x)];
	if ( current >= goalsum ) {
	  binidx = x;
	  foundbin = true;
	}
      }

    if ( !foundbin )
      binidx = 0;
  }
  
  /* from the bin number, calculate the associated value */
  float value = m_vMinValues[0] + ( ((binidx)*(m_vMaxValues[0] - m_vMinValues[0]))/
				    ((float)m_vNumBins[0]) );

  return value;
}


float Histogram::getQuantile( int dim, float xpercent, bool fromleft )
  /*******************************************************************/
  /* Get the value (not the number) of the histogram bin up to which */
  /* x% of the data values have occurred in the histogram. This      */
  /* version of the function is for histograms of arbitrary dimen-   */
  /* sion.                                                           */
  /*******************************************************************/
{
  assert( isValid() );
  assert( dim < m_nDims );
  assert( (xpercent >= 0.0) && (xpercent <= 1.0) );

  /* reduce the histogram to just one dimension by marginalization */
  Histogram temp( *this );

  /* marginalize over all dimensions smaller than dim */
  for( int i=0; i<dim-1; i++ ) {
    Histogram temp2( temp.marginalizeOver(0) );
    temp = temp2; //temp.marginalizeOver(0);
  }

  /* marginalize over all dimensions greater than dim */
  while ( temp.numDims() > 1 ) {
    Histogram temp2( temp.marginalizeOver(1) );
    temp = temp2; //temp.marginalizeOver(1);
  }

  /* Now, temp is only 1-dimensional. All the other dimensions have  */
  /* been subsumed. Thus, we can now call the 1D version of this     */
  /* function.                                                       */
  return temp.getQuantile( xpercent, fromleft );
}


/***********************************************************/
/*                  Histogram Comparison                   */
/***********************************************************/

float Histogram::compSSD( Histogram &other )
  /*******************************************************************/
  /* Calculate the sum-square distance between the cells of this     */
  /* histogram and another one.                                      */
  /*******************************************************************/
{
  /* check if the two histograms have the same dimensions */
  assert( isValid() && other.isValid() );
  assert( m_nDims == other.m_nDims );
  for ( int i=0; i<m_nDims; i++ ) {
    assert ( m_vNumBins[i] == other.m_vNumBins[i] );
  }

  int nTotalBins = calcTotalNumBins();
  assert( other.calcTotalNumBins() == nTotalBins );

  /* calculate the minimum and the sums */
  float ssd = 0.0;
  for( int i=0; i<nTotalBins; i++ ) {
    float diff = m_vBins[i] - other.m_vBins[i];
    ssd       += diff*diff;
  }

  return ssd;
}


float Histogram::compIntersection( Histogram &other, 
				   bool bNormalizeResult )
  /*******************************************************************/
  /* Calculate the intersection between the two histograms.          */
  /* The intersection int(h1, h2) is defined as                      */
  /*   int'(h1,h2) = sum( min(p1 from h1, p2 from h2) )              */
  /*   int(h1,h2)  = ( int'(h1,h2)/sum(h1) + int'(h1,h2)/sum(h2) )/2 */
  /* Unless otherwise specified, the normalized value will be        */
  /* returned.                                                       */
  /*******************************************************************/
{
  /* check if the two histograms have the same dimensions */
  assert( isValid() && other.isValid() );
  assert( m_nDims == other.m_nDims );
  for ( int i=0; i<m_nDims; i++ ) {
    assert ( m_vNumBins[i] == other.m_vNumBins[i] );
  }

  int nTotalBins = calcTotalNumBins();
  assert( other.calcTotalNumBins() == nTotalBins );

  /* calculate the minimum and the sums */
  float sum1 = 0.0;
  float sum2 = 0.0;
  float summin = 0.0; 
  for ( int i=0; i<nTotalBins; i++ ) {
    sum1 += m_vBins[i];
    sum2 += other.m_vBins[i];
    if ( m_vBins[i] < other.m_vBins[i] )
      summin += m_vBins[i];
    else
      summin += other.m_vBins[i];
  }

  if ( bNormalizeResult )
    return 0.5*(summin/sum1 + summin/sum2);
  else
    return summin;
}


float Histogram::compChi2Measure( Histogram &other, bool bNormalize )
  /*******************************************************************/
  /* Calculate the chi-square statistic to compare two histograms.   */
  /* The chi-square measure X(h1, h2) is defined as                  */
  /*   X(h1, h2) = sum( (h1[i]-h2[i])^2/(h1[i]+h2[i]) )              */
  /*******************************************************************/
{
  /* check if the two histograms have the same dimensions */
  assert( isValid() && other.isValid() );
  assert( m_nDims == other.m_nDims );
  for ( int i=0; i<m_nDims; i++ ) {
    assert ( m_vNumBins[i] == other.m_vNumBins[i] );
  }

  int nTotalBins = calcTotalNumBins();
  assert( other.calcTotalNumBins() == nTotalBins );

  /* calculate the minimum and the sums */
  double df, chisq, prob;
  if ( bNormalize )
    chstwo_measure( m_vBins, other.m_vBins, 0, getSum(), other.getSum(), 
	    &df, &chisq );
  else
    chstwo_measure( m_vBins, other.m_vBins, 0, 1.0, 1.0, &df, &chisq );

  return chisq;
}


float Histogram::compChi2Significance( Histogram &other, 
				       bool bNormalize )
  /*******************************************************************/
  /* Calculate the chi-square statistic to compare two histograms.   */
  /* The chi-square measure X(h1, h2) is defined as                  */
  /*   X(h1, h2) = sum( (h1[i]-h2[i])^2/(h1[i]+h2[i]) )              */
  /* From this measure, the function calculates a significance esti- */
  /* mate in order to decide whether the measure gives sufficient    */
  /* proof to reject the null hypothesis that both histograms are    */
  /* drawn from the same distribution. A return value close to zero  */
  /* indicates a significant difference in the histogram distribu-   */
  /* tions.                                                          */
  /*******************************************************************/
{
  /* check if the two histograms have the same dimensions */
  assert( isValid() && other.isValid() );
  assert( m_nDims == other.m_nDims );
  for ( int i=0; i<m_nDims; i++ ) {
    assert ( m_vNumBins[i] == other.m_vNumBins[i] );
  }

  int nTotalBins = calcTotalNumBins();
  assert( other.calcTotalNumBins() == nTotalBins );

  /* calculate the minimum and the sums */
  double df, chisq, prob;
  if ( bNormalize )
    chstwo( m_vBins, other.m_vBins, 0, getSum(), other.getSum(), 
	    &df, &chisq, &prob );
  else
    chstwo( m_vBins, other.m_vBins, 0, 1.0, 1.0, &df, &chisq, &prob );

  return prob;
}


float Histogram::compBhattacharyya( Histogram &other, 
				    bool bNormalizeInputs )
  /*******************************************************************/
  /* Calculate the Bhattacharyya distance between the two histograms.*/
  /* The distance B(h1, h2) is defined as                            */
  /*   B(h1,h2) = - ln( sum( sqrt(h1(i)*h2(i)) ) )                   */
  /* The Bhattacharyya distance is only defined for normalized his-  */
  /* tograms. If they are not normalized yet, the last parameter     */
  /* should be set to 'true'. The routine will then compensate.      */
  /*******************************************************************/
{
  /* check if the two histograms have the same dimensions */
  assert( isValid() && other.isValid() );
  assert( m_nDims == other.m_nDims );
  for ( int i=0; i<m_nDims; i++ ) {
    assert ( m_vNumBins[i] == other.m_vNumBins[i] );
  }

  int nTotalBins = calcTotalNumBins();
  assert( other.calcTotalNumBins() == nTotalBins );

  /* compensate for non-normalized inputs */
  float factor = 1.0;
  if( bNormalizeInputs ) {
    float sum1 = getSum();
    float sum2 = other.getSum();
    factor = 1.0/(sum1*sum2);
  }

  /* calculate the minimum and the Bhattacharyya distance */
  float sum = 0.0;
  for ( int i=0; i<nTotalBins; i++ ) {
    float fact = sqrt(m_vBins[i]*other.m_vBins[i]*factor);
    sum += fact;
  }

  if ( sum > 0.0 )
    return -log(sum);
  else
    return FLT_MAX; //numeric_limits<float>::max();
}


/***********************************************************/
/*                    Service Functions                    */
/***********************************************************/

int Histogram::idx( int x )
  /* calculate the cell index for a 1D histogram */
{
  assert( isValid() );
  assert( m_nDims == 1 );
  assert( x < m_vNumBins[0] );

  return x;
}


int Histogram::idx( int x, int y )
  /* calculate the cell index for a 2D histogram */
{
  assert( isValid() );
  assert( m_nDims == 2 );
  assert( x < m_vNumBins[0] );
  assert( y < m_vNumBins[1] );

  return y*m_vNumBins[0] + x;
}


int Histogram::idx( int x, int y, int z )
  /* calculate the cell index for a 3D histogram */
{
  assert( isValid() );
  assert( m_nDims == 3 );
  assert( x < m_vNumBins[0] );
  assert( y < m_vNumBins[1] );
  assert( z < m_vNumBins[2] );

  return z*m_vNumBins[1]*m_vNumBins[0] + y*m_vNumBins[0] + x;
}


int Histogram::idx( vector<int> index )
  /* calculate the cell index for a histogram of arbitrary dimension */
{
  assert( isValid() );
  assert( m_nDims == index.size() );
  
  int nCurrentIdx = 0;
  int nMultFactor = 1;
  for ( int i = 0; i < index.size(); i++ ) {
    assert( index[i] < m_vNumBins[i] );

    nCurrentIdx += index[i]*nMultFactor;
    nMultFactor *= m_vNumBins[i];
  }

  return nCurrentIdx;
}


vector<int> Histogram::getNextIndex( const vector<int> &index, 
				     bool &endReached )
  /*******************************************************************/
  /* Given a histogram index, create the index of the following his- */
  /* togram cell. This can be used for iterating through all histo-  */
  /* gram cells. When the end is reached, the endReached flag will   */
  /* be returned as true.                                            */
  /*******************************************************************/
{
  assert( isValid() );
  assert( index.size() == m_nDims );

  vector<int> result = index;
  bool done = false;
  endReached = false;

  /* find the lowest dimension that needs to be increased */
  for( int i=m_nDims-1; (i>=0) && !done; i-- )
    if( result[i] < m_vNumBins[i]-1 ) {
      /* just increase this dimension in the index -> done. */
      result[i]++;
      done = true;

    } else {
      /* set this index dimension back to zero and increase the next */
      /* higher dimension.                                           */
      result[i] = 0;
    }

  if( !done )
    /* couldn't increase a single dimension -> end reached, signal   */
    /* overflow.                                                     */
    endReached = true;

  return result;
}


/***********************************************************/
/*                  Associated Functions                   */
/***********************************************************/

bool saveHistogramList( string filename, vector<Histogram> &vHistograms,
			bool gzip, bool verbose )
  /*******************************************************************/
  /* Save the given vector of histograms to disk in text file format.*/
  /* The histograms will be saved under the given file name. If a    */
  /* file with this name already exists, it will be overwritten.     */
  /* The parameter gzip specifies if the file shall be gzipped       */
  /* after saving.                                                   */
  /*******************************************************************/
{
  assert( vHistograms.size() > 0 );
  for( int i=0; i<vHistograms.size(); i++ ) {
    assert( vHistograms[i].isValid() );
  }

  ofstream ofile;

  /* check for the .hlz extension and change it into .hls */
  int pos;
  if ( gzip )
    if ( (pos=filename.rfind( ".hlz" )) != string::npos ) {
      filename = filename.substr(0, pos) + ".hls";
    }

  /* check for the .gz file extension and remove it if it's there */
  if ( gzip )
    if ( (pos=filename.rfind( ".gz" )) != string::npos )
      filename = filename.substr(0, pos);

  /* try to open the file */
  ofile.open( filename.c_str() );
  if ( !ofile ) {
    /* try to compensate for broken network connections */
    cerr << "Couldn't open output file, trying again... ";
    ofile.open( filename.c_str() );
    if ( ofile )
      cerr << "SUCCESS!" << endl;
    else {
      cerr << "FAILED!" << endl;
      cerr << "Error in saveHistogramList(): Couldn't open file '" << filename 
	   << "'!" << endl;
      return false;
    }
  }
 
  /* write the key word 'HistogramList' */
  ofile << "HistogramList" << endl;

  /* write the number of histograms */
  ofile << "Histograms:" << endl;
  ofile << vHistograms.size() << endl;

  /* write the number of dimensions */
  ofile << "Dimensions:" << endl;
  ofile << vHistograms[0].numDims() << endl;
  
  /* write the bin sizes and ranges */
  ofile << "Bins:" << endl;
  for ( int i=0; i<vHistograms[0].numDims(); i++ )
    ofile << vHistograms[0].numBins(i) << " " << vHistograms[0].minValue(i) 
	  << " " << vHistograms[0].maxValue(i) << endl;

  /* write the data file format */
  ofile << "Format:" << endl;
  ofile << "Ascii" << endl;

  /* write the data for all histograms */
  for( int i=0; i<vHistograms.size(); i++ ) {
    /* write the data key word */
    ofile << "Data:" << endl;

    vHistograms[i].writeData( ofile );
  }

  /* close the file */
  ofile.close();

  if ( gzip ) {
    /* gzip the file */
    if ( verbose )
      cout << "  gzipping..." << endl;
    system( string("gzip -f " + filename).c_str() );
    filename = filename + ".gz";
  }

  /* check for the .hls.gz file extension and change it into .hlz */
  if ( (pos=filename.rfind( ".hls.gz" )) != string::npos ) {
    if ( verbose )
      cout << "  shortening extension .hls.gz into .hlz..." << endl;
    filename = filename.substr(0, pos);
    system( string("mv " + filename+".hls.gz " + filename+".hlz").c_str() );
  }

  return true;
}


bool loadHistogramList( string filename, vector<Histogram> &vHistograms, 
			bool verbose )
  /*******************************************************************/
  /* Load the histograms from the given file. The file must exist,   */
  /* must contain a histogram list in text format. The function will */
  /* return a vector of histograms and the function value 'true' in  */
  /* case of success, otherwise 'false'.                             */
  /*******************************************************************/
{
 ifstream ifile;

  /* clear this histogram */
  vHistograms.clear();

  /* check for the .hlz file extension and change it into .hls.gz */
  bool shortname = false;
  string originalname = filename;
  int  pos;
  if ( (pos=filename.rfind( ".hlz" )) != string::npos ) {
    if ( verbose )
      cout << "  found suffix .hlz: changing into .hls.gz..." << endl;
    system( string("mv " + filename + " " + filename.substr(0,pos) + 
		   ".hls.gz").c_str() );
    shortname = true;
    filename = filename.substr(0, pos) + ".hls.gz";
  }

  /* check for the .gz file extension and gunzip the file if it's there */
  bool gunzipped = false;
  if ( (pos=filename.rfind( ".gz" )) != string::npos ) {
    if ( verbose )
      cout << "  found suffix .gz: gunzipping..." << endl;
    system( string("gunzip -f " + filename).c_str() );
    gunzipped = true;
    filename = filename.substr(0, pos);
  }

  /* try to open the file */
  ifile.open( filename.c_str() );
  if ( !ifile ) {
    /* try to compensate for broken network connections */
    cerr << "Couldn't open input file, trying again... ";
    ifile.open( filename.c_str() );
    if ( ifile )
      cout << "SUCCESS!" << endl;
    else {
      cout << "FAILED!" << endl;
      cerr << "Error in loadHistogramList(): Couldn't open file '" << filename 
	   << "'!" << endl;
      return false;
    }
  }

  /* read the first line => determines the type of the file, */
  /* must be 'HistogramList' */
  if ( !readKeyWord( ifile, "HistogramList histogramlist HISTOGRAMLIST" ) ) {
    cerr << "Error in loadHistogramList: Not a histogram list file!" << endl;
    return false;
  }
  if ( verbose )
    cout << "  Loading header..." << endl;

  /* read the next line => must be key word 'Histograms:' */
  if ( !readKeyWord( ifile, "Histograms: histograms:" ) )
    return false;

  /* read the number of histograms */
  int nHistos;
  ifile >> nHistos;
  if ( verbose )
    cout << "    Histograms: " << nHistos << endl;
  vHistograms.reserve( nHistos );

  /* read the next line => must be key word 'Dimensions:' */
  if ( !readKeyWord( ifile, "Dimensions: dimensions:" ) )
    return false;

  /* read the number of dimensions */
  int nDims;
  ifile >> nDims;
  if ( verbose )
    cout << "    Dimensions: " << nDims << endl;
  for( int j=0; j<nHistos; j++ ) {
    Histogram hist;
    hist.setNumDims( nDims );
    //vHistograms[j] = hist;
    vHistograms.push_back(hist);
    //vHistograms[j].setNumDims( nDims );
  }

  /* read the next line => must be key word 'Bins:' */
  if ( !readKeyWord( ifile, "Bins: bins:" ) )
    return false;

  /* read the bin sizes and ranges */
  for ( int i=0; i<nDims; i++ ) {
    int nBins;
    float min, max;
    ifile >> nBins >> min >> max;
    if ( verbose ) {
      cout << "    Dimension " << i << ":" << endl;
      cout << "      #bins     : " << nBins << endl;
      cout << "      minimum   : " << min << endl;
      cout << "      maximum   : " << max << endl;
    }
    for( int j=0; j<nHistos; j++ )
      vHistograms[j].setDimension( i, nBins, min, max );
  }
  
  /* read the next line => must be key word 'Format:' */
  if ( !readKeyWord( ifile, "Format: format:" ) )
    return false;

  /* read the next line => must be key word 'ASCII' or 'BIN' */
  string format;
  bool isAscii = false;
  ifile >> format;
  if ( (format == "ascii") || (format == "Ascii") || (format == "ASCII") ) {
    if ( verbose )
      cout << "    Reading data in ASCII format..." << endl;
    isAscii = true;

  } else if ( (format == "binary") || (format == "Binary") ||
	      (format == "BINARY") ) {
    if ( verbose )
      cout << "    Reading data in binary format..." << endl;
    isAscii = false;

  } else {
    cerr << "Error in Histogram::load(): Expected key word 'Ascii' or 'Binary'." 
	 << endl;
    return false;
  }
  
  /* read the data */
  if ( verbose )
    cout << "  Loading histograms..." << endl;
  bool success = true;
  for( int j=0; j<nHistos; j++ )
    if( success )
      success = vHistograms[j].readData( ifile, isAscii );
  if ( verbose )
    cout << "    found " << vHistograms.size() << " histograms." << endl;

  /* close the file */
  ifile.close();
  
  if ( gunzipped ) {
    /* gzip the file */
    if ( verbose )
      cout << "  gzipping again..." << endl;
    system( string("gzip -f " + filename).c_str() );
  }

  if ( shortname ) {
    /* change the filename from .hls.gz to .hlz */
    if ( verbose )
      cout << "  restoring the original filename with extension .hlz..." 
	   << endl;
    system( string("mv " + filename + ".gz " + originalname).c_str() );
  }

  return success;
}


// bool  readKeyWord( ifstream &ifile, string KeyWords )
// {
//   /* extract the key words to look for */
//   vector<string> vWords = extractWords( KeyWords );
//   string line;

//   assert( vWords.size() > 0 );

//   ifile >> line;
//   bool success = false;
//   for ( int i=0; i<vWords.size(); i++ )
//     if ( !success )
//       if ( line == vWords[i] )
// 	success = true;

//   if ( !success )
//     cerr << "Error loading Histogram: Expected key word '" << vWords[0] 
// 	 << "', found '" << line << "' instead!" << endl;

//   return success;
// }


// vector<string> extractWords( string WordList )
//   /* extract the words (separated by spaces) from a string.          */
// {
//   vector<string> items;
//   int pos_old = 0;
//   int pos_new = 0;

//   while ( pos_old != string::npos ) {
//     /* search for the next space character */
//     if ( (pos_new=WordList.find( " ", pos_old )) == string::npos )
//       break;
//     else {
//       /* found a space character => extract the list item */
//       items.push_back( WordList.substr( pos_old, pos_new - pos_old ) );

//       /* skip all following spaces */
//       pos_old = pos_new;
//       while ( (pos_old < WordList.length()) && (WordList[pos_old] == ' ') ) 
// 	pos_old++;
//     }   
//   }

//   /* extract the last list item */
//   items.push_back( WordList.substr(pos_old) );
  
//   return items;
// }


void computeHistogramStatistics( vector<Histogram> vHistograms, 
				 vector<float> &vMeans, 
				 vector<float> &vVariances )
  /*******************************************************************/
  /* Compute the per-bin means and variances from a collection of    */
  /* histograms.                                                     */
  /*******************************************************************/
{
  /* check if the histogram collection is valid */
  assert( vHistograms.size() >= 1 );
  assert( vHistograms[0].isValid() );

  int nHist      = vHistograms.size();
  int nDims      = vHistograms[0].numDims();
  int nTotalBins = vHistograms[0].calcTotalNumBins();
  for ( int i=1; i<nHist; i++ ) {
    assert( vHistograms[i].isValid() );
    assert( vHistograms[i].numDims() == nDims );
    assert( vHistograms[i].calcTotalNumBins() == nTotalBins );
  }

  /* initialize the means and variances */
  vMeans.clear();
  vVariances.clear();
  vMeans.resize( nTotalBins );
  vVariances.resize( nTotalBins );

  /* calculate the means */
  for ( int i=0; i<nTotalBins; i++ ) {
    vMeans[i] = 0.0;

    for ( int hist=0; hist < nHist; hist++ )
      vMeans[i] += vHistograms[hist].m_vBins[i];
    vMeans[i] /= nHist;
  }

  /* calculate the variances */
  for ( int i=0; i<nTotalBins; i++ ) {
    vVariances[i] = 0.0;

    for ( int hist=0; hist < nHist; hist++ ) {
      float diff = (vHistograms[hist].m_vBins[i] - vMeans[i]);
      vVariances[i] += diff*diff;
    }
    vVariances[i] /= nHist;
  }
}


void getBestHistogramRange( FilterType type, float sigma, 
			    float &min, float &max )
  /* Return the best histogram range for every filter type and sigma */
  /* The ranges returned are calculated to include roughly the 99.9% */
  /* quantile of data values obtained in an empirical study on a     */
  /* test set of 2208 greyvalue images.                              */
{
  switch( type ) {
  case GAUSS:
    min = 0.0;
    max = 255.0;
    break;

  case GAUSS_DX:
  case GAUSS_DY:
    if ( sigma < 1.0 ) {         // sigma smaller than 1.0
      min =  -80.0; //150.0;
      max =   80.0; //150.0;
    } else if ( sigma < 2.0 ) {  // sigma between 1.0 and 2.0
      min =  -61.4; //110.0;
      max =   61.4; //110.0;
    } else if ( sigma < 4.0 ) {  // sigma between 2.0 and 4.0
      min =  -44.6; //66.0;
      max =   44.6; //66.0;
    } else if ( sigma < 8.0 ) {  // sigma between 4.0 and 8.0
      min =  -26.8; //35.0;
      max =   26.8; //35.0;
    } else if ( sigma < 16.0 ) { // sigma between 8.0 and 16.0
      min =  -14.5; //20.0;
      max =   14.5; //20.0;
    } else {                        // sigma greater than 16.0
      min =   -7.2; //10.0;
      max =    7.2; //10.0;
    }
    break;

  case GAUSS_DXX:
  case GAUSS_DYY:
    if ( sigma < 1.0 ) {         // sigma smaller than 1.0
      min =  -70.0; //200.0;
      max =   70.0; //200.0;
    } else if ( sigma < 2.0 ) {  // sigma between 1.0 and 2.0
      min =  -57.1; //170.0
      max =   57.1; //170.0;
    } else if ( sigma < 4.0 ) {  // sigma between 2.0 and 4.0
      min =  -44.5; //100.0;
      max =   44.5; //100.0;
    } else if ( sigma < 8.0 ) {  // sigma between 4.0 and 8.0
      min =  -29.1; //60.0;
      max =   29.1; //60.0;
    } else if ( sigma < 16.0 ) { // sigma between 8.0 and 16.0
      min =  -17.6; //32.0;
      max =   17.6; //32.0;
    } else {                        // sigma greater than 16.0
      min =   -9.1; //16.0;
      max =    9.1; //16.0;
    }
    break;
    
  case GAUSS_DXY:
    if ( sigma < 1.0 ) {         // sigma smaller than 1.0
      min =  -30.0; //55.0;
      max =   30.0; //55.0;
    } else if ( sigma < 2.0 ) {  // sigma between 1.0 and 2.0
      min =  -17.23; //45.0;
      max =   17.23; //45.0;
    } else if ( sigma < 4.0 ) {  // sigma between 2.0 and 4.0
      min =   -5.51; //15.0;
      max =    5.51; //15.0;
    } else if ( sigma < 8.0 ) {  // sigma between 4.0 and 8.0
      min =   -1.68; //5.0;
      max =    1.68; //5.0;
    } else if ( sigma < 16.0 ) { // sigma between 8.0 and 16.0
      min =   -0.516; //1.0;
      max =    0.516; //1.0;
    } else {                        // sigma greater than 16.0
      min =   -0.1336; //3;
      max =    0.1336; //3;
    }
    break;
    
  default:
    cerr << "Error in getBestHistogramRange(): Unknown filter type: " 
	 << type << endl;
    min = 0.0;
    max = 255.0;
  }
}

#ifdef _USE_PERSONAL_NAMESPACES
}
#endif
