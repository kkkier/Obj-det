/**
 * MAT file writing routines, based on description found in
 * http://www.mathworks.com/access/helpdesk/help/pdf_doc/matlab/matfile_format.pdf
 *
 * Only single variables and 2D matrices are supported in this
 * simple implementation. Writing routine automatically transposes
 * and casts matrix into other data types (e.g. double to float)
 *
 * July 2007, Andreas Ess
 **/

#ifndef MATWRITER_HH
#define MATWRITER_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <cassert>
#include <vector>
#include <cstring>
/*******************************/
/*   Constants for MAT types   */
/*******************************/
// Constants for most important, basic MAT types
const int miINT8   = 1;
const int miUINT8  = 2;
const int miINT16  = 3;
const int miUINT16 = 4;
const int miINT32  = 5;
const int miUINT32 = 6;
const int miSINGLE = 7;
const int miDOUBLE = 9;
const int miMATRIX = 14;

// Matlab array types (classes)
const int mxCHAR_CLASS   = 4;
const int mxDOUBLE_CLASS = 6;
const int mxSINGLE_CLASS = 7;
const int mxINT8_CLASS   = 8;
const int mxUINT8_CLASS  = 9;
const int mxINT16_CLASS  = 10;
const int mxUINT16_CLASS = 11;
const int mxINT32_CLASS  = 12;
const int mxUINT32_CLASS = 13;


class MATWriter {
	public:
	public:
  /********************/
  /*   Constructors   */
  /********************/
  MATWriter(const char *fname, const char *comment = NULL);
  
  ~MATWriter();
  
	public:
  /********************/
  /*   Input/Output   */
  /********************/
  template< typename T, typename Tn >
  void writeMatrix( int t, unsigned m, unsigned n, T *a, const char *name)
    /**
     * Write a 2D matrix into MAT file. T is the array's
     * actual type, Tn the type that is written to the file.
     *
     * @param t type of matrix elements
     * @param m rows of matrix
     * @param n columns of matrix
     * @param a pointer to (C-style, column-first) matrix
     * @param name name of the matrix
     **/
  {
    int szb = writeMatrix< Tn >(t, m, n, name);
    
    // Transpose & convert matrix...
    for (unsigned j = 0; j < n; j++) {
      for (unsigned i = 0; i < m; i++) {
        Tn v = *(a + i*n + j);
        fwrite(&v, sizeof(Tn), 1, fp);
      }
    }
    
    // Padding bytes
    int pad = (8 - (szb % 8)) % 8;
    while (pad--)
      fputc(0, fp);
  }


  template< typename T, typename Tn >
  void writeMatrix( int t, unsigned m, unsigned n, const vector<T> &a, 
                    const char *name)
    /**
     * Write a 2D matrix into MAT file. T is the array's
     * actual type, Tn the type that is written to the file.
     *
     * @param t type of matrix elements
     * @param m rows of matrix
     * @param n columns of matrix
     * @param a vector of (C-style, column-first) matrix
     * @param name name of the matrix
     **/
  {
    int szb = writeMatrix< Tn >(t, m, n, name);
    
    // Transpose & convert matrix...
    for (unsigned j = 0; j < n; j++) {
      for (unsigned i = 0; i < m; i++) {
        Tn v = a[i*n + j];
        fwrite(&v, sizeof(Tn), 1, fp);
      }
    }
    
    // Padding bytes
    int pad = (8 - (szb % 8)) % 8;
    while (pad--)
      fputc(0, fp);
  }
  

  template< typename T, typename Tn >
  void writeMatrix( int t, unsigned m, unsigned n, T *a, 
                    int tx, int ty, int bx, int by, const char *name)
    /**
     * Write a 2D sub-matrix into MAT file. T is the array's
     * actual type, Tn the type that is written to the file.
     *
     * @param t type of matrix elements
     * @param m rows of matrix
     * @param n columns of matrix
     * @param a pointer to (C-style, column-first) matrix
     * @param tx x-coordinate to start writing from
     * @param ty y-coordinate to start writing from
     * @param bx x-coordinate to end writing (inclusive)
     * @param by y-coordinate to end writing
     * @param name name of the matrix
     **/
  {
    int szb = writeMatrix< Tn >(t, by-ty+1, bx-tx+1, name);
    
    // Transpose & convert sub-matrix...
    for (unsigned j = tx; j <= bx; j++) {
      for (unsigned i = ty; i <= by; i++) {
        Tn v = *(a + i*n + j);
        fwrite(&v, sizeof(Tn), 1, fp);
      }
    }
    
    // Padding bytes
    int pad = (8 - (szb % 8)) % 8;
    while (pad--)
      fputc(0, fp);
  }
		

  template< typename T, typename Tn >
  void writeMatrix( int t, unsigned m, unsigned n, const vector<T> &a, 
                    int tx, int ty, int bx, int by, const char *name)
  /**
   * Write a 2D sub-matrix into MAT file. T is the array's
   * actual type, Tn the type that is written to the file.
   *
   * @param t type of matrix elements
   * @param m rows of matrix
   * @param n columns of matrix
   * @param a vector of (C-style, column-first) matrix
   * @param tx x-coordinate to start writing from
   * @param ty y-coordinate to start writing from
   * @param bx x-coordinate to end writing (inclusive)
   * @param by y-coordinate to end writing
   * @param name name of the matrix
   **/
  {
    int szb = writeMatrix< Tn >(t, by-ty+1, bx-tx+1, name);
    
    // Transpose & convert sub-matrix...
    for (int j = tx; j <= bx; j++) {
      for (int i = ty; i <= by; i++) {
        Tn v = a[i*n + j];
        fwrite(&v, sizeof(Tn), 1, fp);
      }
    }
    
    // Padding bytes
    int pad = (8 - (szb % 8)) % 8;
    while (pad--)
      fputc(0, fp);
  }
  

  template< typename T >
  void writeVariable(int t, T v, const char *name)
  /**
   * Write a single variable into MAT file
   *
   * @param t type of variable
   * @param v actual variable
   * @param name name of variable
   **/
  {
    writeMatrix< T, T >(t, 1, 1, &v, name);
  }
  

	private:
  /***********************/
  /*   Helper Routines   */
  /***********************/
  void writeTag(FILE *fp, int t, int n);

  int getMatrixClass(int t);

  template< typename Tn >
  int writeMatrix(int t, unsigned m, unsigned n, const char *name)
  /**
   * Helper routine for preparing matrix write: writes all
   * except actual matrix and its padding bytes
   **/
  {
    int nlen = ::strlen(name);
    int szb = n * m * sizeof(Tn);
    
    // Size in bytes---includes padding to 64bit boundary
    int sz = 16 + 16 + 8 + nlen + (8 - nlen % 8)%8 + 8 + szb + (8 - szb % 8)%8;
    writeTag(fp, miMATRIX, sz);
    
    // Write array flags
    writeTag(fp, miUINT32, 8);
    char buf[8];
    ::memset(buf, 0, 8);
    buf[0] = getMatrixClass(t);
    fwrite(buf, 8, 1, fp);
    
    // Dimensions array (2 dimensions only)
    writeTag(fp, miINT32, 8);
    fwrite(&m, 1, 4, fp);
    fwrite(&n, 1, 4, fp);
    
    // Name of matrix
    writeTag(fp, miINT8, nlen);
    while (*name != 0)
      fputc(*name++, fp);
    // (Padding)
    nlen = (8 - nlen % 8) % 8;
    while (nlen--)
      fputc(0, fp);
    
    // Actual array
    writeTag(fp, t, szb);
    
    return szb;
  }

	private:
  /************************/
  /*   Member Variables   */
  /************************/
  //! Handle to MAT file
  FILE *fp;
};


/***********************************************************/
/*                   Auxiliary Functions                   */
/***********************************************************/
template< typename T >
void findMinMax( int m, int n, T* a, T eps, 
                 int &tx, int &ty, int &bx, int &by )
 /**
  * Find smallest rectangle of values > eps in a matrix
  *
  * @param m rows of original matrix
  * @param n columsn of original matrix
  * @param a array of actual values (column-first)
  * @param eps threshold value
  * @param tx (return) top x coordinate
  * @param ty (return) top y coordinate
  * @param bx (return) bottom x coordinate (inclusive)
  * @param by (return) bottom y coordinate (inclusive)
  **/
{
	tx = n - 1;
	ty = m - 1;
	bx = 0;
	by = 0;
	for (int y = 0; y < m; y++) {
		for (int x = 0; x < n; x++) {
			if (a[y * n + x] > eps) {
				if (x < tx) tx = x;
				if (y < ty) ty = y;
				if (x > bx) bx = x;
				if (y > by) by = y;
			}
		}
	}
}


template< typename T >
void findMinMax( int m, int n, const vector<T> &a, T eps, 
                 int &tx, int &ty, int &bx, int &by )
 /**
  * Find smallest rectangle of values > eps in a matrix
  *
  * @param m rows of original matrix
  * @param n columsn of original matrix
  * @param a array of actual values (column-first)
  * @param eps threshold value
  * @param tx (return) top x coordinate
  * @param ty (return) top y coordinate
  * @param bx (return) bottom x coordinate (inclusive)
  * @param by (return) bottom y coordinate (inclusive)
  **/
{
	tx = n - 1;
	ty = m - 1;
	bx = 0;
	by = 0;
	for (int y = 0; y < m; y++) {
		for (int x = 0; x < n; x++) {
			if (a[y * n + x] > eps) {
				if (x < tx) tx = x;
				if (y < ty) ty = y;
				if (x > bx) bx = x;
				if (y > by) by = y;
			}
		}
	}
}



// USAGE:
#if 0
int main(int argc, char **argv) {
	MATWriter mw("test.mat", "blabla");
	double a[4*4] = { 0, 0, 0, 0, 0, 1, 2, 0, 0, 3, 0, 0, 0, 0, 0, 0 };
	double b[3*2] = { 9, 8, 7, 6, 5, 4 };
	
	int tx, ty, bx, by;
	findMinMax< double >(4, 4, a, 0, tx, ty, bx, by);
	mw.writeMatrix< double, float >(miSINGLE, 3, 2, b, "b");
	mw.writeMatrix< double, float >(miSINGLE, 4, 4, a, tx, ty, bx, by, "a");
	mw.writeVariable< int >(miINT32, tx, "tx");
	mw.writeVariable< float >(miSINGLE, a[0], "ty");
}
#endif


#endif
