/*********************************************************************/
/*                                                                   */
/* FILE         featurematrix.cc                                     */
/* AUTHORS      Thomas Hug, modified by Oliver Bay, Bastian Leibe    */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Implements basic matrix operations.                  */
/*                                                                   */
/*                                                                   */
/* BEGIN        Thu Nov 08 2001                                      */
/* LAST CHANGE  Fri Jan 02 2004                                      */
/*                                                                   */
/*********************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "illegaloperationexception.hh"
#include "featurematrix.hh"

extern "C" {
#include "nr.h"
}

/*===================================================================*/
/*                        Class FeatureMatrix                        */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

FeatureMatrix::FeatureMatrix( int rows, int cols ) 
{
	m_nRows = rows; 
  m_nCols = cols;
	if (m_nRows*m_nCols > 0)
		m_vElements.resize( m_nRows*m_nCols, 0.0 );
}


FeatureMatrix::FeatureMatrix( int rows, int cols, const float *data ) 
{
	m_nRows = rows; 
  m_nCols = cols;
	if (m_nRows*m_nCols > 0)
		m_vElements.resize( m_nRows*m_nCols, 0.0 );

	for (int r=0; r<m_nRows; r++)
		for (int c=0; c<m_nCols; c++) {
			m_vElements[idx(r,c)] = data[idx(r,c)];
		}
}


FeatureMatrix::FeatureMatrix( istream& stream ) 
{
  /* read in the matrix dimensions */
	stream >> m_nRows;
	stream >> m_nCols;
	if (m_nRows*m_nCols > 0) {
		m_vElements.resize( m_nRows*m_nCols, 0.0 );
	}
	
	/* read in the matrix content */
	for (int r=0; r<m_nRows; r++)
		for (int c=0; c<m_nCols; c++) {
      float data;
      stream >> data;
      m_vElements[idx(r,c)] = data;
    }
}


FeatureMatrix::FeatureMatrix( const FeatureMatrix& other ) 
{
	m_nRows     = other.m_nRows; 
  m_nCols     = other.m_nCols;
  m_vElements = other.m_vElements;
}


FeatureMatrix createUnitMatrix( int dim )
{
  FeatureMatrix result( dim, dim );
  for( int i=0; i<dim; i++ )
    result(i,i) = 1.0;
  return result;
}


FeatureMatrix createOnesMatrix( int dim )
{
  FeatureMatrix result( dim, dim );
  for( int r=0; r<dim; r++ )
    for( int c=0; c<dim; c++ )
      result(r,c) = 1.0;
  return result;
}


FeatureMatrix createDiagMatrix( const FeatureVector& diag )
{
  int dim = diag.numDims();

  FeatureMatrix result( dim, dim );
  for( int i=0; i<dim; i++ )
    result(i,i) = diag.at(i);
  return result;
}


/***********************************************************/
/*                        Operators                        */
/***********************************************************/

/*---------------------------------------------------------*/
/*                       Assignment                        */
/*---------------------------------------------------------*/
FeatureMatrix& FeatureMatrix::operator=( const FeatureMatrix& other )
{	
  // No self assignment
  if ( this != &other ) {
    m_nRows     = other.m_nRows;
    m_nCols     = other.m_nCols;
    m_vElements = other.m_vElements;
  }
  
  return (*this);
}


/*---------------------------------------------------------*/
/*                      Element Access                     */
/*---------------------------------------------------------*/
float FeatureMatrix::operator()( int r, int c ) const 
{
	if ( (r<0) || (r>m_nRows) || (c<0) || (c>m_nCols) ) {
		cerr << "Error accessing element (" << r << "," << c << ")" << endl;
		throw IllegalOperationException();
	}
	return m_vElements[idx(r,c)];
}
	

float& FeatureMatrix::operator()( int r, int c ) 
{
	if ( (r<0) || (r>m_nRows) || (c<0) || (c>m_nCols) ) {
		cerr << "Error accessing element (" << r << "," << c << ")" << endl;
		throw IllegalOperationException();
	}
	return m_vElements[idx(r,c)];
}
		

/*---------------------------------------------------------*/
/*                 Matrix-Matrix Operations                */
/*---------------------------------------------------------*/
FeatureMatrix& FeatureMatrix::operator+=( const FeatureMatrix& other ) 
{
	if ( (m_nRows != other.m_nRows) || (m_nCols != other.m_nCols) ) {
		cerr << "Error in FeatureMatrix::operator+=(): "
         << "Matrix dimensions don't match!" << endl;
		throw IllegalOperationException();
	}

	for (int r=0; r<m_nRows; r++)
		for (int c=0; c<m_nCols; c++) {
			m_vElements[idx(r,c)] += other.m_vElements[idx(r,c)];
		}
	return *this;
}
		

FeatureMatrix& FeatureMatrix::operator-=( const FeatureMatrix& other ) 
{
	if ( (m_nRows != other.m_nRows) || (m_nCols != other.m_nCols) ) {
		cerr << "Error in FeatureMatrix::operator-=(): "
         << "Matrix dimensions don't match!" << endl;
		throw IllegalOperationException();
	}

	for (int r=0; r<m_nRows; r++)
		for (int c=0; c<m_nCols; c++) {
			m_vElements[idx(r,c)] -= other.m_vElements[idx(r,c)];
		}
	return *this;
}


FeatureMatrix& FeatureMatrix::operator*=( const FeatureMatrix& other ) 
{
	if (m_nCols != other.m_nRows) {
		cerr << "Error in FeatureMatrix::operator*=(): "
         << "Matrix dimensions don't match!" << endl;
		throw IllegalOperationException();
	}
	
	FeatureMatrix result( m_nRows, other.m_nCols );
	for (int r=0; r<m_nRows; r++)
    for (int c=0; c<other.m_nCols; c++)
      for (int k=0; k<m_nCols; k++) {
        result.m_vElements[idx(r,c)] += ( m_vElements[idx(r,k)] *
                                          other.m_vElements[idx(k,c)] );
      }
  
  *this = result; // Valid with new '='-operator    
	return *this;
}


// Binary
FeatureMatrix operator+( const FeatureMatrix& a, const FeatureMatrix& b ) 
{
	if ( (a.m_nRows != b.m_nRows) || (a.m_nCols != b.m_nCols) ) {
		cerr << "Error in FeatureMatrix::operator+(): "
         << "Matrix dimensions don't match!" << endl;
		throw IllegalOperationException();
	}

	FeatureMatrix result(a.m_nRows, a.m_nCols);
	for (int r=0; r<a.m_nRows; r++)
		for (int c=0; c<a.m_nCols; c++) {
			result(r,c) = a(r,c) + b(r,c);
		}
	return result;
}


FeatureMatrix operator-( const FeatureMatrix& a, const FeatureMatrix& b ) 
{
	if ( (a.m_nRows != b.m_nRows) || (a.m_nCols != b.m_nCols) ) {
		cerr << "Error in FeatureMatrix::operator-(): "
         << "Matrix dimensions don't match!" << endl;
		throw IllegalOperationException();
	}

	FeatureMatrix result(a.m_nRows, a.m_nCols);
	for (int r=0; r<a.m_nRows; r++)
		for (int c=0; c<a.m_nCols; c++) {
			result(r,c) = a(r,c) - b(r,c);
		}
	return result;
}


FeatureMatrix operator*( const FeatureMatrix& a, const FeatureMatrix& b ) 
{
	if (a.m_nCols != b.m_nRows) {
		cerr << "Error in FeatureMatrix::operator*=(): "
         << "Matrix dimensions don't match!" << endl;
		throw IllegalOperationException();
	}
	
	FeatureMatrix result( a.m_nRows, b.m_nCols );
	for (int r=0; r<a.m_nRows; r++)
    for (int c=0; c<b.m_nCols; c++)
      for (int k=0; k<a.m_nCols; k++) {
        result(r,c) += a(r,k) * b(k,c);
      }
	return result;
}


/*---------------------------------------------------------*/
/*                 Matrix-Vector Operations                */
/*---------------------------------------------------------*/

FeatureVector FeatureMatrix::operator*( const FeatureVector& x ) const
{
	if (m_nCols != x.numDims()) {
		cerr << "Error in FeatureMatrix::operator*(): "
         << "Matrix dimensions don't match!" << endl;
		throw IllegalOperationException();
	}
	
	FeatureVector result( m_nCols );
  for (int r=0; r<m_nRows; r++)
    for (int c=0; c<m_nCols; c++)
      result.at(r) += m_vElements[idx(r,c)] * x.at(c);

	return result;
}


FeatureVector FeatureMatrix::rightMulVec( const FeatureVector& x ) const
{
	return (*this)*x;
}


FeatureMatrix FeatureMatrix::leftMulVecT( const FeatureVector& x ) const
{
	if (m_nRows != x.numDims()) {
		cerr << "Error in FeatureMatrix::operator*(): "
         << "Matrix dimensions don't match!" << endl;
		throw IllegalOperationException();
	}
	
	FeatureMatrix result( 1, m_nCols );
  for (int c=0; c<m_nCols; c++)
    for (int r=0; r<m_nRows; r++)
      result.m_vElements[idx(1,c)] += m_vElements[idx(r,c)] * x.at(c);

	return result;
}


FeatureVector FeatureMatrix::getVector() const
{
	if (m_nRows == 1) {
    FeatureVector result( m_nCols );
		for (int c=0; c<m_nCols; c++)
      result.at(c) = m_vElements[idx(1,c)];
    return result;

  } else if (m_nCols == 1) {
    FeatureVector result( m_nRows );
		for (int r=0; r<m_nRows; r++)
      result.at(r) = m_vElements[idx(r,1)];
    return result;

  } else {  
		cerr << "Error in FeatureMatrix::getVector(): "
         << "At least one dimension must be 1!" << endl;
		throw IllegalOperationException();
	}
}
		

/*---------------------------------------------------------*/
/*                 Matrix-Scalar Operations                */
/*---------------------------------------------------------*/
FeatureMatrix& FeatureMatrix::operator+=( float x ) 
{
	for (int r=0; r<m_nRows; r++)
		for (int c=0; c<m_nCols; c++) {
			m_vElements[idx(r,c)] += x;
		}
	return *this;
}

		
FeatureMatrix& FeatureMatrix::operator-=( float x ) 
{
	for (int r=0; r<m_nRows; r++)
		for (int c=0; c<m_nCols; c++) {
			m_vElements[idx(r,c)] -= x;
		}
	return *this;
}

		
FeatureMatrix& FeatureMatrix::operator*=( float x ) 
{
	for (int r=0; r<m_nRows; r++)
		for (int c=0; c<m_nCols; c++) {
			m_vElements[idx(r,c)] *= x;
		}
	return *this;
}

		
FeatureMatrix& FeatureMatrix::operator/=( float x ) 
{
	if (x == 0.0) {
		cerr << "Error in FeatureMatrix::operator/=(): "
         << "Division by Zero!" << endl;
		throw IllegalOperationException();
	}
	
	for (int r=0; r<m_nRows; r++)
		for (int c=0; c<m_nCols; c++) {
			m_vElements[idx(r,c)] /= x;
		}
	return *this;
}


// Binary		
FeatureMatrix operator+( const FeatureMatrix& a, float x ) 
{
	FeatureMatrix result(a.m_nRows, a.m_nCols);
	for (int r=0; r<a.m_nRows; r++)
		for (int c=0; c<a.m_nCols; c++) {
			result(r,c) = a(r,c) + x;
		}
	return result;
}
		

FeatureMatrix operator-( const FeatureMatrix& a, float x ) 
{
	FeatureMatrix result(a.m_nRows, a.m_nCols);
	for (int r=0; r<a.m_nRows; r++)
		for (int c=0; c<a.m_nCols; c++) {
			result(r,c) = a(r,c) - x;
		}
	return result;
}
		

FeatureMatrix operator*( const FeatureMatrix& a, float x ) 
{
	FeatureMatrix result(a.m_nRows, a.m_nCols);
	for (int r=0; r<a.m_nRows; r++)
		for (int c=0; c<a.m_nCols; c++) {
			result(r,c) = a(r,c) * x;
		}
	return result;
}
		

FeatureMatrix operator/( const FeatureMatrix& a, float x ) 
{
	if (x == 0.0) {
		cerr << "Error in FeatureMatrix::operator/(): "
         << "Division by Zero!" << endl;
		throw IllegalOperationException();
	}
	
	FeatureMatrix result(a.m_nRows, a.m_nCols);
	for (int r=0; r<a.m_nRows; r++)
		for (int c=0; c<a.m_nCols; c++) {
			result(r,c) = a(r,c) / x;
		}
	return result;
}
		

/*---------------------------------------------------------*/
/*                 Vector-Vector Operations                */
/*---------------------------------------------------------*/

FeatureMatrix mulVecTVec( const FeatureVector& x, const FeatureVector& y ) 
{
  assert( x.isValid() && y.isValid() );

	if (x.numDims() != y.numDims()) {
		cerr << "Error in mulVecTVec(): "
         << "Vector dimensions must agree!" << endl;
		throw IllegalOperationException();
	}
	
  int N = x.numDims();
	FeatureMatrix result(N,N);
	for (int r=0; r<N; r++)
		for (int c=0; c<N; c++) {
			result(r,c) = x.at(c) * y.at(r);
		}
	return result;
}
		

FeatureMatrix mulVecVecT( const FeatureVector& x, const FeatureVector& y ) 
{
  assert( x.isValid() && y.isValid() );

	if (x.numDims() != y.numDims()) {
		cerr << "Error in mulVecTVec(): "
         << "Vector dimensions must agree!" << endl;
		throw IllegalOperationException();
	}
	
  int N = x.numDims();
	FeatureMatrix result(N,N);
	for (int r=0; r<N; r++)
		for (int c=0; c<N; c++) {
			result(r,c) = x.at(r) * y.at(c);
		}
	return result;
}
		

/*---------------------------------------------------------*/
/*                       Input/Output                      */
/*---------------------------------------------------------*/
ostream& operator<<( ostream& s, const FeatureMatrix& a ) 
{
	for (int r=0; r<a.m_nRows; r++) {
		s << "\t";
		for (int c=0; c<a.m_nCols; c++) {
			s << a(r,c) << "\t";
		}
		s << endl;
	}
	return s;
}


ofstream& operator<<( ofstream& s, const FeatureMatrix& a ) 
{
	s << a.m_nRows << " " << a.m_nCols << endl;
	for (int r=0; r<a.m_nRows; r++) {
		for (int c=0; c<a.m_nCols; c++) {
			s << a(r,c) << " ";
		}
    s << endl;
  }
	return s;
}
		

//****************************************************
// Member functions
//****************************************************


FeatureMatrix FeatureMatrix::transpose() const
{
	FeatureMatrix result( m_nCols, m_nRows );
	for (int r=0; r<m_nRows; r++)
		for (int c=0; c<m_nCols; c++) {
			result.m_vElements[idx(c,r)] = m_vElements[idx(r,c)];
		}
  return result;
}


FeatureMatrix FeatureMatrix::invert() const
{
	if( m_nRows != m_nCols ) {
		cerr << "Error in FeatureMatrix::invert(): "
         << "Matrix must be square!" << endl;
		throw IllegalOperationException();
	}
	
	FeatureMatrix result( m_nRows, m_nCols );
	
  // alloc arrays for calculations
  float **a = new float*[m_nRows+1];
  float **y = new float*[m_nRows+1];
  for( int r=0; r<m_nRows+1; r++) {
  	a[r] = new float[m_nCols+1];
  	y[r] = new float[m_nCols+1];
  }

	float d, *col;
	int   *indx;
	
	col = new float[m_nRows+1];
	indx = new int[m_nRows+1];

  for( int r=0; r<m_nRows; r++)
    for( int c=0; c<m_nCols; c++) {
			a[r+1][c+1] = m_vElements[idx(r,c)];
		}
	ludcmp(a, m_nRows, indx, &d);
	for (int j=1; j<=m_nRows; j++) {
		for (int i=1; i<=m_nRows; i++) {
			col[i] = 0.0;
		}
		col[j] = 1.0;
		lubksb(a, m_nRows, indx, col);
		for (int i=1; i<=m_nRows; i++) {
			y[i][j] = col[i];
		}
	}

  for( int r=0; r<m_nRows; r++)
    for( int c=0; c<m_nCols; c++) {
			result.m_vElements[idx(r,c)] = y[r+1][c+1];
		}
	
	// dealloc a and y, col and indx
	for (int i=0; i<m_nCols+1; i++) {
		delete[] a[i];
		delete[] y[i];
	}
	delete[] a;
	delete[] y;
	delete[] col;
	delete[] indx;

  return result;
}


float FeatureMatrix::det() const
{
	if( m_nRows != m_nCols ) {
		cerr << "Error in FeatureMatrix::det(): "
         << "Matrix must be square!" << endl;
		throw IllegalOperationException();
	}
	
  // alloc arrays for calculations
  float **a = new float*[m_nRows+1];
  float **y = new float*[m_nRows+1];
  for( int r=0; r<m_nRows+1; r++) {
  	a[r] = new float[m_nCols+1];
  	y[r] = new float[m_nCols+1];
  }

	float d;
	int *indx = new int[m_nRows+1];

  for( int r=0; r<m_nRows; r++)
    for( int c=0; c<m_nCols; c++) {
			a[r+1][c+1] = m_vElements[idx(r,c)];
		}
	ludcmp(a, m_nRows, indx, &d);
  for (int j=1; j<=m_nRows; j++) {
    d *= a[j][j];
  }
	float result = d;

	// dealloc a and y, col and indx
	for (int i=0; i<m_nCols+1; i++) {
		delete[] a[i];
		delete[] y[i];
	}
	delete[] a;
	delete[] y;
	delete[] indx;

  return result;
}


