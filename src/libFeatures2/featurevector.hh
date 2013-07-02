/*********************************************************************/
/*                                                                   */
/* FILE         featurevector.hh                                     */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Define a general feature vector class derived from a */
/*              vector<float> and provide some basic functions.      */
/*                                                                   */
/* BEGIN        Wed Jul 24 2002                                      */
/* LAST CHANGE  Wed Jul 24 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_FEATUREVECTOR_HH
#define LEIBE_FEATUREVECTOR_HH

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

#include <grayimage.hh>

using namespace std;

/*************************/
/*   Class Definitions   */
/*************************/
#define USE_GZCAT USE_GZCAT

/*===================================================================*/
/*                         Class FeatureVector                       */
/*===================================================================*/
/* Define a general feature vector class */
class FeatureVector
{
public:
    FeatureVector();
    FeatureVector( int nDims );
    FeatureVector( vector<float> data );
    FeatureVector( vector<double> data );
    FeatureVector( const FeatureVector &other );
    virtual ~FeatureVector();

    FeatureVector& operator=( FeatureVector other );
    FeatureVector& operator=( vector<float> other );
    FeatureVector& operator=( vector<double> other );

protected:
    virtual void  copyFromOther( const FeatureVector &other );

    virtual void  initBins();

    virtual int   calcTotalNumBins() const;

public:
  /*******************************/
  /*   Content Access Functions  */
  /*******************************/
  virtual bool  isValid() const;

  int   numDims() const     { assert( m_bSizeDefined ); return m_nDims; }

  virtual void  setNumDims  ( int nDims );

  virtual float  at ( int x ) const          { return m_vBins[idx(x)]; }
  virtual float& at ( int x )                { return m_vBins[idx(x)]; }

  virtual void  setValue( int x, float val )  { m_vBins[idx(x)] = val; }

  virtual void  setData( vector<float> data );
  virtual void  setData( vector<double> data );
  vector<float> getData() const     { return m_vBins; }

  virtual void  clear();

  virtual void  print();
  virtual void  printContent();

  friend ostream& operator<<( ostream& os, const FeatureVector& pt );

public:
  /******************************/
  /*   FeatureVector File I/O   */
  /******************************/
  virtual bool  save( string filename );
  virtual bool  load( string filename, bool verbose=false );

  virtual void  writeHeader( ofstream &ofile );
  virtual void  writeData( ofstream &ofile ) const;
  virtual bool  readHeader( ifstream &ifile, bool &isAscii, 
														bool verbose=false );
  virtual bool  readData( ifstream &ifile, bool isAscii=true );

public:
  /**********************/
  /*   Vector Algebra   */
  /**********************/
  /*--------------------------*/
  /* Vector-Vector operations */
  /*--------------------------*/
  virtual void  addVector   ( const FeatureVector &other );
  virtual void  subVector   ( const FeatureVector &other );

  FeatureVector& operator+=( const FeatureVector &other );
  FeatureVector& operator-=( const FeatureVector &other );

  friend FeatureVector operator+( const FeatureVector &a, 
                                  const FeatureVector &b );
  friend FeatureVector operator-( const FeatureVector &a, 
                                  const FeatureVector &b );

  FeatureVector add  ( const FeatureVector &other );
  FeatureVector sub  ( const FeatureVector &other );
  float         dot  ( const FeatureVector &other );
  FeatureVector cross( const FeatureVector &other );

  /*--------------------------*/
  /* Vector-Scalar operations */
  /*--------------------------*/
  virtual void  multFactor  ( float factor );
  virtual void  divFactor   ( float factor );
  FeatureVector& operator+=( float x );
  FeatureVector& operator-=( float x );
  FeatureVector& operator*=( float x );
  FeatureVector& operator/=( float x );

  friend FeatureVector operator+( const FeatureVector& a, float x );
  friend FeatureVector operator-( const FeatureVector& a, float x );
  friend FeatureVector operator*( const FeatureVector& a, float x );
  friend FeatureVector operator/( const FeatureVector& a, float x );
  
  FeatureVector add( float x );
  FeatureVector sub( float x );
  FeatureVector mul( float x );
  FeatureVector div( float x );

public:
  /**********************************/
  /*   FeatureVector Manipulation   */
  /**********************************/
  virtual void  normalizeVector ( float newSum );
  virtual void  normalizeEntries( vector<float> vBinFactors );
  void subtractMean();
  void normalizeEnergy();
  void normalizeEnergy2();
  void normalizeZeroMeanUnitVar();
  void normalizeZeroMeanUnitStdDev();
  void normalizeZeroMeanUnitStdDev2();

public:
  /********************************/
  /*   FeatureVector Statistics   */
  /********************************/
  float getSum() const;
  void  getMinMax( float &min, float &max ) const;
  
  friend void computeFeatureStatistics( vector<FeatureVector> vFeatureVectors, 
																				vector<float> &vMeans, 
																				vector<float> &vVariances );

public:
  /*************************************/
  /*   Histogram Comparison Measures   */
  /*************************************/
  float compSSD             ( const FeatureVector &other ) const;
  float compCorrelation     ( const FeatureVector &other ) const;
  float compIntersection    ( const FeatureVector &other, 
                              bool bNormalizeResult=true ) const;
  float compChi2Measure     ( const FeatureVector &other, 
                              bool bNormalize=true ) const;
  float compChi2Significance( const FeatureVector &other, 
                              bool bNormalize=true ) const;
  float compBhattacharyya   ( const FeatureVector &other, 
                              bool bNormalizeInputs=true ) const;

protected:
    virtual int   idx( int x ) const;

    int            m_nDims;
    bool           m_bSizeDefined;

    int            m_nTotalNumBins;
    vector<float>  m_vBins;     // contains all bins, access via idx function
};


/****************************/
/*   Associated Functions   */
/****************************/
ostream& operator<<( ostream& os, const FeatureVector& pt );

bool saveFeatureVectorList( string filename,
                            const vector<FeatureVector> &vFeatureVectors,
                            bool gzip=true, bool verbose=false );
bool loadFeatureVectorList( string filename,
                            vector<FeatureVector> &vFeatureVectors,
                            bool verbose=false );

bool readKeyWord( ifstream &ifile, string KeyWords );
vector<string> extractWords( string WordList );

void computeFeatureStatistics( vector<FeatureVector> vFeatureVectors,
                               vector<float> &vMeans,
                               vector<float> &vVariances );

#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif

#endif // LEIBE_FEATUREVECTOR_HH
