/*********************************************************************/
/*                                                                   */
/* FILE         idlparser.hh                                         */
/* AUTHORS      Dirk Zimmer, modified by Bastian Leibe               */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Definition of an image description list structure.   */
/*                                                                   */
/* BEGIN        Thu Apr 22 2004                                      */
/* LAST CHANGE  Thu Apr 22 2004                                      */
/*                                                                   */
/*********************************************************************/

#ifndef IDLPARSER_HH
#define IDLPARSER_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "imgdescr.hh"
#include "imgdescrlist.hh"


/*******************/
/*   Definitions   */
/*******************/
const char CHAR_STRING_SEP = '"';
const char CHAR_NAME_SEP   = ':';
const char CHAR_RECT_OPEN  = '(';
const char CHAR_COORD_SEP  = ',';
const char CHAR_RECT_CLOSE = ')';
const char CHAR_RECT_SEP   = ',';
const char CHAR_SCORE_SEP  = ':';
const char CHAR_TEMPL_SEP  = '/';
const char CHAR_LINE_SEP   = ';';
const char CHAR_END_SEP    = '.';


/*************************/
/*   Class Definitions   */
/*************************/
///////////////////////////////////////////////////////////////////////
//  
// class IDLParser
//_____________________________________________________________________
// IDLParser (Image Description List Parser) is made to read in IDL Files.
// IDL Files have an EBNF Definition (see below)
// It is programmed according to the EBNF.
///////////////////////////////////////////////////////////////////////
class IDLParser 
{
public:
	IDLParser( string sFileName );
	~IDLParser();

public:
  /*************************/
  /*   Parsing Functions   */
  /*************************/
	char parse( ImgDescrList &vList );

 private:
  /*************************/
  /*   Support Functions   */
  /*************************/
  char nextChar();
	char readNumber();
	char readString();
	char readRect(); 
	char readImgDescr();

  /////////////////////////////////////////////////////////////////////
  // private members
  //___________________________________________________________________
  //  ifstream  m_ifile;				input stream to read from
  //	char      m_cCurCh; 			last char currently read
  //	int       m_nCurNumber;		last number currently read
  //	string    m_sCurString;   last string currently read
  //	Rect      m_rCurRect;			last rectangle currently read
  //	ImgDescr* m_pCurImgDescr; last image description currently read
  /////////////////////////////////////////////////////////////////////
	char      m_cCurCh;
	int       m_nCurNumber;
	string    m_sCurString;
	ImgDescr  m_idCurImgDescr;
	Rect      m_rCurRect;
	ifstream  m_ifile;
};


////////////////////////////////////////////////////////////////////////
// 
// The EBNF of an IDL File
//_____________________________________________________________________
//  File = '.' | ({ImgDescr ';'} ImgDescr '.')
//  ImgDescr = ImgName | (ImgName ':' RectList)
//  RectList = {Rectangle ',' } Rectangle
//  Rectangle = '(' Number ',' Number ',' Number ',' Number ')' [':' Score] 
//  ImgName = '"' string '"'
//  Score = c++ instream double
//  Number = digit {digit}
//  
//  Eveything after the '.' will be ignored.
//  Spaces and control signs will be ignored. 
//
// 
// Example File:
//   "img001.png":(10,10,30,30):9.1221;
//   "img002.png":(20,30,70,90):0.5361,(18,27,65,93),(18,27,65,93);
//   "subdirectory1/img003.png";
//   "img004.png":(18,27,65,93).
//
///////////////////////////////////////////////////////////////////////

#endif
