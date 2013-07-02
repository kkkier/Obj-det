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

/****************/
/*   Includes   */
/****************/
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <cassert>

#include "matwriter.hh"

/*===================================================================*/
/*                        Class MATWriter                            */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

MATWriter::MATWriter( const char *fname, const char *comment ) 
  /**
   * Create MAT file, including header
   *
   * @param fname file name to use
   * @param comment additional comment for header file
   **/
{
  // Create 128-byte header
  char buf[128];
  memset(buf, ' ', 124);
  sprintf(buf, "MATLAB 5.0 MAT-file, %s", comment);
  buf[124] = 0x00;
  buf[125] = 0x01;
  
  // Endian indicator
  buf[126] = 'I';
  buf[127] = 'M';
  
  fp = fopen(fname, "w");
  assert(fp);
  fwrite(buf, 1, 128, fp);
}

MATWriter::~MATWriter() {
  fclose(fp);
}


/***********************************************************/
/*                      Input/Output                       */
/***********************************************************/


/***********************************************************/
/*                    Helper Routines                      */
/***********************************************************/

void MATWriter::writeTag(FILE *fp, int t, int n) 
  /**
   * Helper routine for writing a tag
   *
   * @param t type to write
   * @param n number of bytes
   **/
{
  fwrite(&t, 4, 1, fp);
  fwrite(&n, 4, 1, fp);
}


int MATWriter::getMatrixClass(int t) 
  /**
   * Helper routine returning matrix class given type
   **/
{
  switch (t) {
  case miSINGLE : return mxSINGLE_CLASS;
  case miDOUBLE : return mxDOUBLE_CLASS;
  case miINT8   : return mxINT8_CLASS;
  case miUINT8  : return mxUINT8_CLASS;
  case miINT16  : return mxINT16_CLASS;
  case miUINT16 : return mxUINT16_CLASS;
  case miINT32  : return mxINT32_CLASS;
  case miUINT32 : return mxUINT32_CLASS;
  }
  std::cerr << "Matrix type not supported in this implementation!" 
            << std::endl;
  
  return mxDOUBLE_CLASS;
}


/***********************************************************/
/*                   Auxiliary Functions                   */
/***********************************************************/
