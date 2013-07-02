/*********************************************************************/
/*                                                                   */
/* FILE         imgdescrlist.hh                                      */
/* AUTHORS      Dirk Zimmer, modified by Bastian Leibe               */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Definition of an image description list structure.   */
/*                                                                   */
/* BEGIN        Thu Apr 22 2004                                      */
/* LAST CHANGE  Thu Apr 22 2004                                      */
/*                                                                   */
/*********************************************************************/

#ifndef IMGDESCRLIST_HH
#define IMGDESCRLIST_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "imgdescr.hh"


/*************************/
/*   Class Definitions   */
/*************************/
///////////////////////////////////////////////////////////////////////
//
// class ImgDescrList inherits a vector of ImgDescr*
//_____________________________________________________________________
//
// First of all: This class inherits a vector. So don't forget you have 
// all the vector functionality avaible.
// ImgDescrList (Image Description List or short IDL) adds two major
// functionalities:
//   1) It allows you to load, save, edit and examine IDLs comfortably
//	 2) You have static toolkit functions to handle rectangles 
// Something important:
//   The ImgDescrList purposes to own its Image Descriptions.
//   So by executing the destructor or by calling clear() not only the 
//   pointers are deleted also the ImgDescr they point to. Be aware of 
//   that!
///////////////////////////////////////////////////////////////////////
class ImgDescrList
{
public:

  ImgDescrList (); 
  ImgDescrList ( string sFileName );
  ImgDescrList ( const ImgDescrList &other );

  ~ImgDescrList();
  
public:
  /*********************/
  /*   I/O Functions   */
  /*********************/
	void load ( string sFileName, bool bOverwrite=true, bool bDoSort=false );
	void save ( string sFileName) const;
	void save ( string sFileName, bool bDoSort );

public:
  /********************************/
  /*   Content Access Functions   */
  /********************************/
	void          clear();
  int           size ()     { return m_vImgDescr.size(); }
  void          sort ();

  //ImgDescr      operator[]  ( int idx ) const;
  ImgDescr&     operator[]  ( int idx );
  ImgDescr&     addEntry    ( const ImgDescr &idEntry );
	void          addList     ( ImgDescrList &other, string sPrefix = "" );

  ImgDescrList& operator=   ( const ImgDescrList& other );
  

  int           findName     ( const string& sName );
  ImgDescr&     findOrCreate ( const string& sName );    
  void          findAndRemove( const string& sName );

  void          getRectInfo  ( int& nCount, 
                               double& dMinScore, double& dMaxScore );

	void print();

public:
  /**************************/
  /*   External Functions   */
  /**************************/
  enum FixDimType{ FIX_OBJWIDTH = 0, FIX_OBJHEIGHT = 1};

	static void   sortCoords    ( Rect& r );
  static double compCover     ( Rect r1, Rect r2 );
  static double compInterUnion( Rect r1, Rect r2 );
  static double compRelDist   ( const Rect& r1, const Rect& r2 );
  static double compRelDist   ( const Rect& r1, const Rect& r2, 
                                float dAspectRatio, 
                                FixDimType eFixObjDim=FIX_OBJHEIGHT );
  static bool   isMatching    ( Rect& r1, Rect& r2, 
                                double dTDist, 
                                double dTCover, double dTOverlap);
  static bool   isMatching    ( Rect& r1, Rect& r2, 
                                double dTDist, 
                                double dTCover, double dTOverlap,
                                float dAspectRatio, 
                                FixDimType eFixObjDim=FIX_OBJHEIGHT );
  
private:
  vector<ImgDescr> m_vImgDescr;
  int              m_nCurIdx;
};

#endif
