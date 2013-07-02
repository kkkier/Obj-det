/*********************************************************************/
/*                                                                   */
/* FILE         featurematrix.hh                                     */
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

#ifndef LEIBE_FEATUREMATRIX_HH
#define LEIBE_FEATUREMATRIX_HH

#include <iostream>
#include <fstream>
#include <vector>

#include <featurevector.hh>

class FeatureMatrix 
{
public:
  FeatureMatrix () { m_nCols = 0; m_nRows = 0; }
  FeatureMatrix ( int rows, int cols );
  FeatureMatrix ( const FeatureMatrix& other );
  FeatureMatrix ( istream& stream );
  FeatureMatrix ( int rows, int cols, const float *data );

  friend FeatureMatrix createUnitMatrix( int dim );
  friend FeatureMatrix createOnesMatrix( int dim );
  friend FeatureMatrix createDiagMatrix( const FeatureVector& diag );

public:  
  /*****************/
  /*   Operators   */ 
  /*****************/
  /*------------*/
  /* Assignment */
  /*------------*/
  FeatureMatrix& operator=( const FeatureMatrix& other );

  /*----------------*/
  /* Element access */
  /*----------------*/
  float  operator()( int r, int c ) const;
  float& operator()( int r, int c );
  
  /*--------------------------*/
  /* Matrix-Matrix operations */
  /*--------------------------*/
  FeatureMatrix& operator+=( const FeatureMatrix& other );
  FeatureMatrix& operator-=( const FeatureMatrix& other );
  FeatureMatrix& operator*=( const FeatureMatrix& other );

  friend FeatureMatrix operator+( const FeatureMatrix& a, 
                                  const FeatureMatrix& b );
  friend FeatureMatrix operator-( const FeatureMatrix& a, 
                                  const FeatureMatrix& b );
  friend FeatureMatrix operator*( const FeatureMatrix& a, 
                                  const FeatureMatrix& b );

  /*--------------------------*/
  /* Matrix-Vector operations */
  /*--------------------------*/
  FeatureVector operator*  ( const FeatureVector& x ) const;
  FeatureVector rightMulVec( const FeatureVector& x ) const;
  FeatureMatrix leftMulVecT( const FeatureVector& x ) const;
  
  FeatureVector getVector() const;

  /*--------------------------*/
  /* Matrix-Scalar operations */
  /*--------------------------*/
  FeatureMatrix& operator+=( float x );
  FeatureMatrix& operator-=( float x );
  FeatureMatrix& operator*=( float x );
  FeatureMatrix& operator/=( float x );
  
  // Binary
  friend FeatureMatrix operator+( const FeatureMatrix& a, float x );
  friend FeatureMatrix operator-( const FeatureMatrix& a, float x );
  friend FeatureMatrix operator*( const FeatureMatrix& a, float x );
  friend FeatureMatrix operator/( const FeatureMatrix& a, float x );
  
  /*--------------------------*/
  /* Vector-Vector operations */
  /*--------------------------*/
  friend FeatureMatrix mulVecTVec( const FeatureVector& x, 
                                   const FeatureVector& y );
  friend FeatureMatrix mulVecVecT( const FeatureVector& x, 
                                   const FeatureVector& y );

  /*--------------*/
  /* Input/Output */
  /*--------------*/
  friend ostream& operator<< ( ostream& s, const FeatureMatrix& c );
  friend ofstream& operator<<( ofstream& s, const FeatureMatrix& c );
  
public:
  /************************/
  /*   Member functions   */
  /************************/  
  int rows() const { return m_nRows; }
  int cols() const { return m_nCols; }
  
  FeatureMatrix transpose() const;
  FeatureMatrix invert() const;
  float  det() const;
  
protected:
  inline int idx( int r, int c ) const { return r*m_nCols + c; }

  int  m_nRows;
  int  m_nCols;
  
  vector<float> m_vElements;  
};  
FeatureMatrix mulVecVecT( const FeatureVector& x,
                                   const FeatureVector& y );
 FeatureMatrix createUnitMatrix( int dim );
 FeatureMatrix createOnesMatrix( int dim );
 FeatureMatrix createDiagMatrix( const FeatureVector& diag );
#endif
