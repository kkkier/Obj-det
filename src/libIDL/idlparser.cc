/*********************************************************************/
/*                                                                   */
/* FILE         idlparser.cc                                         */
/* AUTHORS      Dirk Zimmer, modified by Bastian Leibe               */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Definition of an image description list structure.   */
/*                                                                   */
/* BEGIN        Thu Apr 22 2004                                      */
/* LAST CHANGE  Thu Apr 22 2004                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <ctype.h>
#include <vector>

#include "idlparser.hh"


/*===================================================================*/
/*                         Class IDLParser                           */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

IDLParser::IDLParser( string sFileName )
  : m_idCurImgDescr()
  /*******************************************************************/
  /* Construct an object of type IDLParser.                          */
  /* Parameters:                                                     */
  /*   ifile: The input stream to be parsed.                         */
  /*******************************************************************/
{
  m_ifile.open( sFileName.c_str() );
  if( !m_ifile ) {
    cerr << "Error in IDLParser::IDLParser(): "
         << "Couldn't open file '" << sFileName << "'!" << endl;
  }

  m_cCurCh       = 0;
  m_nCurNumber   = 0;
  m_sCurString   = "";
  m_idCurImgDescr.sName = "";
  m_idCurImgDescr.vRectList.clear();
}


IDLParser::~IDLParser()
{
  m_ifile.close();
}


/***********************************************************/
/*                    Parsing Functions                    */
/***********************************************************/

char IDLParser::parse( ImgDescrList &vList )
  /*******************************************************************/
  /* Parse an IDL-File on the instream that was given by the obj's   */
  /* constructor. It's rarely meaningful to call this routine twice  */
  /* for the same object.                                            */
  /* Parameters:                                                     */
  /*   data: a container for the result.                             */ 
  /* Preconditions:                                                  */
  /*   data must be a valid container.                               */
  /* Return Values and Side-effects:                                 */
  /*   The return value is true if the try was succesful, otherwise  */
  /*   false. If succesful, all Image Descriptions are added to the  */
  /*   vector. If not succesful all those Image Descr. are added,    */
  /*   which could be parsed. Existing elements of the vector won't  */
  /*   be deleted.                                                   */
  /*   m_cCurCh, m_nCurString, m_rCurRect and m_nCurNumber are updated.          */
  /*******************************************************************/
{
  /* clear the image description list */
  vList.clear();

  nextChar();
   
  if( m_cCurCh == CHAR_END_SEP ) 
    return true;
  
  if( !readImgDescr() )  
    return false;
  vList.addEntry( m_idCurImgDescr );
   
  while( m_cCurCh == CHAR_LINE_SEP ) {
    nextChar();
    if( !readImgDescr() )
      return false; 
    
    vList.addEntry( m_idCurImgDescr );
  }
  
  if( m_cCurCh != CHAR_END_SEP ) 
    return false;

  return true;
}


/***********************************************************/
/*                    Support Functions                    */
/***********************************************************/

char IDLParser::nextChar()
  /*******************************************************************/
  /* Read in the next character that isn't a space or a control cha- */
  /* racter.                                                         */
  /* Return Values and Side-effects:                                 */
  /*   the next valid character or EOF                               */
  /*   the return value is also stored in m_cCurCh                      */
  /*******************************************************************/
{
  char c;
  m_ifile >> c;
  while( !m_ifile.eof() && ((c == ' ') || (iscntrl(c))) ) 
    m_ifile >> c;

  m_cCurCh = c;		
  return c;
}


char IDLParser::readNumber()
  /*******************************************************************/
  /* Try to read in a number.                                        */
  /* Return Values and Side-effects:                                 */
  /*   The return value is true if the try was succesful, otherwise  */
  /*   false. If succesful the number is stored in m_nCurNumber.        */
  /*   m_cCurCh is updated as well.                                     */
  /*******************************************************************/
{
  int nSign = 1;
  char c = m_cCurCh;
  if( !isdigit(c) && !(c=='-') ) 
    return false;
  
  if( c == '-' ) {
    nSign = -1;
    m_ifile >> c;
  }
    
  int n = 0;
  while( isdigit(c) ) {
    n = 10*n + c - (int)'0';
    m_ifile >> c;
  }

  /* clear all whitespace */
  while( !m_ifile.eof() && ((c == ' ') || (iscntrl(c))) ) 
    m_ifile >> c;

  m_cCurCh     = c;		
  m_nCurNumber = nSign * n;
  return true; 
}


char IDLParser::readString()
  /*******************************************************************/
  /* Try to read in a string that starts and ends with: '"'          */ 
  /* WARNING: If the string itself contains a '"', this routine      */
  /* won't work.                                                     */
  /* Return Values and Side-effects:                                 */
  /*   The return value is true if the try was succesful, otherwise  */
  /*   false. If succesful, the string is stored in m_nCurString.    */
  /*   m_cCurCh is updated as well.                                  */
  /*******************************************************************/
{
  
  if( m_cCurCh != CHAR_STRING_SEP ) 
    return false;
  
  string buffer =  "";
  m_ifile >> m_cCurCh;
  while( !m_ifile.eof() && (m_cCurCh != CHAR_STRING_SEP) ) {
    buffer += m_cCurCh;
    m_ifile >> m_cCurCh;
  }	

  if( m_cCurCh != CHAR_STRING_SEP )
    return false;

  nextChar();
  m_sCurString = buffer;  
  return true;
}


char IDLParser::readRect()
  /*******************************************************************/
  /* Try to read in a rectangle according to the EBNF.               */
  /* Return Values and Side-effects:                                 */
  /*   The return value is true if the try was succesful, otherwise  */
  /*   false. If succesful the rectangle is stored in m_rCurRect.    */
  /*   m_cCurCh and m_nCurNumber are updated as well.                */
  /*******************************************************************/
{
  Rect R;
  if( m_cCurCh != CHAR_RECT_OPEN ) 
    return false;
  nextChar(); 
  if( !readNumber() ) 
    return false;
  R.x1 = m_nCurNumber;
  
  if( m_cCurCh != CHAR_COORD_SEP ) 
    return false;
  nextChar();
  if( !readNumber() ) 
    return false;
  R.y1 = m_nCurNumber;

  if( m_cCurCh != CHAR_COORD_SEP ) 
    return false;
  nextChar();
  if( !readNumber() ) 
    return false;
  R.x2= m_nCurNumber;

  if( m_cCurCh != CHAR_COORD_SEP ) 
    return false;
  nextChar();
  if( !readNumber() ) 
    return false;
  R.y2 = m_nCurNumber;

  if( m_cCurCh != CHAR_RECT_CLOSE ) 
    return false;
  nextChar();

  if( m_cCurCh == CHAR_SCORE_SEP) {
    m_ifile >> R.score;
    nextChar();
  } else 
    R.score = 0.0;

  if( m_cCurCh == CHAR_TEMPL_SEP) {
    m_ifile >> R.nTemplateId;
    nextChar();
  } else 
    R.nTemplateId = -1;
  
  m_rCurRect = R;
  return true;
}


char IDLParser::readImgDescr()
  /*******************************************************************/
  /*  Try to read in an image description according to the EBNF.     */
  /* Return Values and Side-effects:                                 */
  /*   The return value is true if the try was succesful, otherwise  */
  /*   false. If succesful the pointer to the newly created ImgDescr */
  /*   is stored in m_pCurImgDescr.                                  */
  /*   m_cCurCh, m_nCurString, m_rCurRect and m_nCurNumber are upda- */
  /*   ted as well.                                                  */
  /*******************************************************************/
{
  if( !readString() ) 
    return false;

  ImgDescr idRead;
  idRead.sName = m_sCurString;	

  if( m_cCurCh == CHAR_NAME_SEP ) { 
    do { 
      nextChar();
      if (!readRect()) {
        return false;
      }
      idRead.vRectList.push_back( m_rCurRect );
    } while (m_cCurCh == CHAR_RECT_SEP && !m_ifile.eof() );
  }
  m_idCurImgDescr = idRead;
  return true;
}


