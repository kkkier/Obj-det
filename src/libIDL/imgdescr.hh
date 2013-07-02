/*********************************************************************/
/*                                                                   */
/* FILE         imgdescr.hh                                          */
/* AUTHORS      Dirk Zimmer, modified by Bastian Leibe               */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Definition of an image description structure.        */
/*                                                                   */
/* BEGIN        Thu Apr 22 2004                                      */
/* LAST CHANGE  Thu Apr 22 2004                                      */
/*                                                                   */
/*********************************************************************/

#ifndef IMGDESCR_HH
#define IMGDESCR_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>


/*******************/
/*   Definitions   */
/*******************/
class Rect
{
public:
  Rect() { x1 = y1 = x2 = y2 = -1; score = 0.0; nTemplateId=-1; }
  Rect( int x1, int y1, int x2, int y2, double score, int nTemplateId ) 
  { 
    this->x1 = x1; this->y1 = y1;
    this->x2 = x2; this->y2 = y2;
    this->score = score;
    this->nTemplateId = nTemplateId;
  }

public:
  int x1;
  int y1;
  int x2;
  int y2;
  double score;
  int nTemplateId;
};


/*************************/
/*   Class Definitions   */
/*************************/
class ImgDescr 
{
public:
  ImgDescr()                         { clear(); }
  ImgDescr( string name )            { sName = name; }
  ImgDescr( const ImgDescr& other )  { copy(other); } 

  ImgDescr& operator=( const ImgDescr& other ) 
  { copy(other); return *this; } 

public:
  void clear() 
  { sName = ""; vRectList.clear(); }
  
  void copy ( const ImgDescr &other ) 
  { sName = other.sName; vRectList = other.vRectList; }

  void print()
  { 
    cout << "      Name: " << sName.c_str() << endl;
    for( int i=0; i<(int)vRectList.size(); i++ ) {
      cout << "        Annot. " << i+1 << ": (" << vRectList[i].x1 << ","
           << vRectList[i].y1 << "," << vRectList[i].x2 << "," 
           << vRectList[i].y2 << ")";
      if( vRectList[i].score > 0.0 )
        cout << ":" << vRectList[i].score;
      if( vRectList[i].nTemplateId >= 0 )
        cout << "/" << vRectList[i].nTemplateId;
      cout << endl;
    }
    cout << "      ---------------------" << endl;
  }
      
public:
  string       sName;
  vector<Rect> vRectList;
};

/*-------------------------------------------------------------------*/
/*                         Sorting Operators                         */
/*-------------------------------------------------------------------*/
struct compImgDescrName
{
  bool operator()( const ImgDescr &x, const ImgDescr &y )
  { return (x.sName > y.sName); }
};

struct equalImgDescrName
{
  bool operator()( ImgDescr &x, ImgDescr &y )
  { return (x.sName == y.sName); }
};

#endif
