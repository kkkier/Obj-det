/*********************************************************************/
/*                                                                   */
/* FILE         imgdescrlist.cc                                      */
/* AUTHORS      Dirk Zimmer, modified by Bastian Leibe               */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Implementation of an image description list.         */
/*                                                                   */
/* BEGIN        Thu Apr 22 2004                                      */
/* LAST CHANGE  Thu Apr 22 2004                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <math.h>
#include <fstream>
#include <cassert>
#include <algorithm>

#include "idlparser.hh"
#include "imgdescrlist.hh"


/*===================================================================*/
/*                        Class ImgDescrList                         */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

ImgDescrList::ImgDescrList()
  /*******************************************************************/
  /* Create an empty Image Description List.                         */
  /*******************************************************************/
{
  m_nCurIdx = -1;
}


ImgDescrList::ImgDescrList( string sFileName )
  /*******************************************************************/
  /* Create an Image Description List out of an IDL-File.            */
  /*******************************************************************/
{
  m_nCurIdx = -1;
  load( sFileName );
}


ImgDescrList::ImgDescrList( const ImgDescrList &other )
  /*******************************************************************/
  /* Copy constructor.                                               */
  /*******************************************************************/
{
  m_nCurIdx   = other.m_nCurIdx;
  m_vImgDescr = other.m_vImgDescr;
}


ImgDescrList::~ImgDescrList()
  /*******************************************************************/
  /* Standard destructor.                                            */
  /*******************************************************************/
{
  clear();
}


/***********************************************************/
/*                  File I/O Functions                     */
/***********************************************************/

void ImgDescrList::load( string sFileName, bool bOverwrite, bool bDoSort )
  /*******************************************************************/
  /* Add the Image Descriptions of an IDL-File to this vector.       */
  /* Parameters:                                                     */
  /*   ifile     : ifstream of the IDL_File                          */
  /*   bOverwrite: if true the existing IDs are deleted.             */
  /*   bDoSort   : if true the descriptions will be sorted.          */
  /*******************************************************************/
{
  if( bOverwrite) 
    clear();
  
  IDLParser parser( sFileName );
	parser.parse( *this );

	if( bDoSort ) 
    sort();
}

void ImgDescrList::save( string sFileName) const
  /*******************************************************************/
  /* Save this IDL into an IDL-File.                                 */
  /* Parameters:                                                     */
  /*   ofile  : the output file stream.                              */
  /*******************************************************************/
{
  /* open the output file */
  ofstream ofile( sFileName.c_str() );
  if( !ofile ) {
    cerr << "Error in ImgDescrList::save(): "
         << "Couldn't open file '" << sFileName << "'";
    return;
  }

  for( int i=0; i<(int)m_vImgDescr.size(); i++ ) {

    /* write the image name */
    ofile << CHAR_STRING_SEP << m_vImgDescr[i].sName << CHAR_STRING_SEP;

    if( m_vImgDescr[i].vRectList.size() > 0 ) {
      /* write a name separator */
      ofile << CHAR_NAME_SEP;
    }

    /* write the rectangles, separated by commas */
    for( int j=0; j<(int)m_vImgDescr[i].vRectList.size(); j++ ) {
      /* write the next rectangle */
      ofile << " " << CHAR_RECT_OPEN 
            << m_vImgDescr[i].vRectList[j].x1 << CHAR_COORD_SEP << " "
            << m_vImgDescr[i].vRectList[j].y1 << CHAR_COORD_SEP << " "
            << m_vImgDescr[i].vRectList[j].x2 << CHAR_COORD_SEP << " "
            << m_vImgDescr[i].vRectList[j].y2 << CHAR_RECT_CLOSE;
      if (m_vImgDescr[i].vRectList[j].score != 0.0)
        ofile << CHAR_SCORE_SEP << m_vImgDescr[i].vRectList[j].score;
      if (m_vImgDescr[i].vRectList[j].nTemplateId >= 0)
        ofile << CHAR_TEMPL_SEP << m_vImgDescr[i].vRectList[j].nTemplateId;

      if( j < (int)m_vImgDescr[i].vRectList.size()-1 )
        /* write a rectangle separator */
        ofile << CHAR_RECT_SEP;
    }

    if( i < (int)m_vImgDescr.size()-1 ) {
      /* write a line separator */
      ofile << CHAR_LINE_SEP << endl;
    }
  }
  ofile << CHAR_END_SEP << endl;
  ofile.close();

}

void ImgDescrList::save( string sFileName, bool bDoSort )
  /*******************************************************************/
  /* Save this IDL into an IDL-File.                                 */
  /* Parameters:                                                     */
  /*   ofile  : the output file stream.                              */
  /*   bDoSort: if true, this IDL will be sorted before saving.      */
  /*******************************************************************/
{
  if( bDoSort ) 
    sort();
  save(sFileName);
}



/***********************************************************/
/*               Content Access Functions                  */
/***********************************************************/

void ImgDescrList::clear()
  /*******************************************************************/
  /*  Free the memory of all the ImageDescr wich are referenced by   */
  /* this object. Clears the vector itself. So all the Image Des-    */
  /* criptions are presumed to be owned by this object.              */
  /*******************************************************************/
{
  m_nCurIdx = -1;
  m_vImgDescr.clear();
}


void ImgDescrList::sort()
  /*******************************************************************/
  /* Sort the Image Descriptions according to their names in ascen-  */
  /* ding order.                                                     */
  /*******************************************************************/
{
  stable_sort( m_vImgDescr.begin(), m_vImgDescr.end(), compImgDescrName() );
}


// ImgDescr  ImgDescrList::operator[]( int idx ) const
//   /*******************************************************************/
//   /* Access operator (right-sided).                                  */
//   /*******************************************************************/
// {
//   assert( (idx>=0) && (idx<(int)m_vImgDescr.size()) );

//   return m_vImgDescr[idx];
// }


ImgDescr& ImgDescrList::operator[]( int idx )
  /*******************************************************************/
  /* Access operator (left-sided).                                   */
  /*******************************************************************/
{
  assert( (idx>=0) && (idx<(int)m_vImgDescr.size()) );

  return m_vImgDescr[idx];
}


ImgDescrList& ImgDescrList::operator=( const ImgDescrList& other )
  /*******************************************************************/
  /* Assignment operator.                                            */
  /*******************************************************************/
{
  m_nCurIdx   = other.m_nCurIdx;
  m_vImgDescr = other.m_vImgDescr;

  return *this;
}


ImgDescr& ImgDescrList::addEntry( const ImgDescr &idEntry )
{
  m_vImgDescr.push_back( idEntry );

  return m_vImgDescr[ m_vImgDescr.size()-1 ];
}


void ImgDescrList::addList( ImgDescrList &other, string sPrefix )
  /*******************************************************************/
  /* Useful to merge two Image Description Lists.                    */
  /* This Object will contain all image descriptions afterwards.     */ 
  /* The RectLists of common image descriptions are added.           */
  /* Parameters:                                                     */
  /*   newIDL: The IDs to be added. This object remains untouched.   */
  /*           All values will be copied.                            */
  /*   prefix: A string that will be added to the description's      */
  /*           names of newIDL before(!) the merging process.        */
  /*******************************************************************/
{
  for( int i=0; i<(int)other.size(); i++ ) {
	  string sNewName = sPrefix + other[i].sName; 
	  ImgDescr curID = findOrCreate( sNewName );

    for( int j=0; j<(int)other[i].vRectList.size(); j++ ) {
	    curID.vRectList.push_back( other[i].vRectList[j] );
	  }
	}
}


int ImgDescrList::findName( const string &sName )
  /*******************************************************************/
  /* Find an Image Description by its name.                          */
  /* Parameters:                                                     */
  /*   name: the name of the Image Description searched for.         */
  /* Return Value:                                                   */
  /*   If the entry exists, its index is returned, else -1.          */ 
  /*******************************************************************/
{
  /* search for the entry in the current list */
  for( int i=0; i<(int)m_vImgDescr.size(); i++ )
    if( m_vImgDescr[i].sName == sName ) {
      return i;
    }

  return -1;
}


ImgDescr& ImgDescrList::findOrCreate( const string &sName )
  /*******************************************************************/
  /* Find an Image Description by its name (closely related to       */
  /* findName). If such a Description doesn't exist, one will be     */
  /* created.                                                        */
  /* Parameters:                                                     */
  /*   name: the name of the Image Description searched for.         */
  /*******************************************************************/
{
  /* search for the entry in the current list */
  for( int i=0; i<(int)m_vImgDescr.size(); i++ )
    if( m_vImgDescr[i].sName == sName ) {
      return m_vImgDescr[i];
    }

  return addEntry( ImgDescr(sName) );
}


void ImgDescrList::findAndRemove( const string &sName )
  /*******************************************************************/
  /* Find an Image Description by its name and remove it.            */
  /* Parameters:                                                     */
  /*   name: the name of the Image Description searched for.         */
  /*******************************************************************/
{
  /* search for the entry in the current list */
  for( vector<ImgDescr>::iterator it=m_vImgDescr.begin(); 
       it<m_vImgDescr.end(); it++ )
    if( it->sName == sName ) {
      m_vImgDescr.erase(it);
    }
}


void ImgDescrList::getRectInfo( int &nCount, double &dMinScore, 
                                double &dMaxScore )
  /*******************************************************************/
  /* Return some info about the rectangles in this IDL.              */
  /* Return Parameters:                                              */
  /*   nCount   : the number of Rectangles found.                    */
  /*   dMinScore: the minimum score found.                           */
  /*   dMaxScore: the maximum score found.                           */
  /*   (If nCount==0, dMinScore and dMaxScore remain unchanged).     */
  /*******************************************************************/
{
  bool first = true;
  double s;
  nCount = 0;
  for( int i=0; i<(int)m_vImgDescr.size(); i++ ) {
    for( int j=0; j<(int)m_vImgDescr[i].vRectList.size(); j++ ) {
      s = m_vImgDescr[i].vRectList[j].score;

      if( first ) {
        dMinScore = s;
        dMaxScore = s;
        first = false;
        
      } else {
        if (s < dMinScore) dMinScore = s;
        if (s > dMaxScore) dMaxScore = s;
      }
    }
    nCount += m_vImgDescr[i].vRectList.size();
  }
}	


void ImgDescrList::print()
  /*******************************************************************/
  /* Print the content of the image description list.                */
  /*******************************************************************/
{
  cout << "    --------------------------------" << endl;
  cout << "    Image Description List:" << endl;
  cout << "      #entries: " << size() << endl;
  for( int i=0; i<size(); i++ )
    m_vImgDescr[i].print();
  cout << "    --------------------------------" << endl;
}


/***********************************************************/
/*                   External Functions                    */
/***********************************************************/

void ImgDescrList::sortCoords( Rect& r )
  /*******************************************************************/
  /* Sort the coordinates of an rectangle r such that                */
  /*   (x1,y1) is the upper left corner, and                         */
  /*   (x2,y2) is lower right corner (meaning x2>x1, y2>y1)          */  
  /* Parameters:                                                     */
  /*   r: r is the rectangle to be sorted.                           */
  /*******************************************************************/
{
  int h;
  if( r.x1 > r.x2 ) {
     h    = r.x1; 
     r.x1 = r.x2; 
     r.x2 = h;
  }
  if( r.y1 > r.y2 ) {
     h    = r.y1; 
     r.y1 = r.y2; 
     r.y2 = h;
  }
  return;
}


double ImgDescrList::compCover( Rect r1, Rect r2 )
  /*******************************************************************/
  /* Determine the Fraction of r1 which is covered by r2.            */
  /* Parameters:                                                     */
  /*   r1, r2: The rectangles. Attention: the order does matter!     */
  /* Return Value:                                                   */
  /*   The fraction wich is coverd (always between 0 and 1).         */
  /*******************************************************************/
{
  sortCoords(r1);
  sortCoords(r2);

  int nWidth  = r1.x2 - r1.x1;
  int nHeight = r1.y2 - r1.y1;
  int iWidth  = max(0,min(max(0,r2.x2-r1.x1),nWidth )-max(0,r2.x1-r1.x1));
  int iHeight = max(0,min(max(0,r2.y2-r1.y1),nHeight)-max(0,r2.y1-r1.y1));
  return ((double)iWidth * (double)iHeight)/((double)nWidth * (double)nHeight);
}	


double ImgDescrList::compInterUnion( Rect r1, Rect r2 )
  /*******************************************************************/
  /* Determine the score "intersection/union" between r1 and r2.     */
  /* Parameters:                                                     */
  /*   r1, r2: The rectangles.                                       */
  /* Return Value:                                                   */
  /*   The "intersection/union" score (always between 0 and 1).      */
  /*******************************************************************/
{
  sortCoords(r1);
  sortCoords(r2);

  int nWidth  = abs(r1.x2 - r1.x1);
  int nHeight = abs(r1.y2 - r1.y1);
  int iWidth  = ( max(0,min(max(0,abs(r2.x2-r1.x1)),nWidth ) - 
                      max(0,abs(r2.x1-r1.x1))) );
  int iHeight = ( max(0,min(max(0,abs(r2.y2-r1.y1)),nHeight) -
                      max(0,abs(r2.y1-r1.y1))) );
  int uWidth  = ( max(0,max(r1.x2,r2.x2) - min(r1.x1,r2.x1)) );
  int uHeight = ( max(0,max(r1.y2,r2.y2) - min(r1.y1,r2.y1)) );
  
  float dInter = (double)iWidth * (double)iHeight;
  float dUnion = (double)uWidth * (double)uHeight;
  return dInter / dUnion;
}	


double ImgDescrList::compRelDist( const Rect& r1, const Rect& r2 )
  /*******************************************************************/
  /* Determine the distance between the centers of r1 and r2.        */
  /* How the distance is measured depends on r1:                     */
  /*   If the center of r2 is on the largest inlying ellipse, the    */
  /*   distance is defined to be exactly 1.0. Outside it is greater; */
  /*   inside it is smaller.                                         */
  /* Parameters:                                                     */
  /*   r1, r2: The rectangles. Attention: the order does matter!     */
  /* Return Value:                                                   */
  /*   the relative distance.                                        */
  /*******************************************************************/
{
  double dWidth  = r1.x2 - r1.x1;
  double dHeight = r1.y2 - r1.y1;
  double xdist   = (double)(r1.x1 + r1.x2 - r2.x1 - r2.x2) / dWidth;
  double ydist   = (double)(r1.y1 + r1.y2 - r2.y1 - r2.y2) / dHeight;
  return sqrt(xdist*xdist + ydist*ydist);
}


double ImgDescrList::compRelDist( const Rect& r1, const Rect& r2, 
                                  float dAspectRatio, FixDimType eFixObjDim )
  /*******************************************************************/
  /* Determine the distance between the centers of r1 and r2.        */
  /* How the distance is measured depends on r1:                     */
  /*   If the center of r2 is on the largest inlying ellipse, the    */
  /*   distance is defined to be exactly 1.0. Outside it is greater; */
  /*   inside it is smaller.                                         */
  /* This version of the function uses a ground truth box with fixed */
  /* aspect ratio as reference.                                      */
  /* Parameters:                                                     */
  /*   r1, r2: The rectangles. Attention: the order does matter!     */
  /*   dAspectRatio: the fixed aspect ratio for the first rect.      */
  /*   eFixObjDim  : the information whether the object width or     */
  /*                 height shall be fixed for the computation.      */
  /*                 (allowed values: FIX_OBJWIDTH, FIX_OBJHEIGHT)   */
  /* Return Value:                                                   */
  /*   the relative distance.                                        */
  /*******************************************************************/
{
  double dWidth, dHeight;

  switch( eFixObjDim ) {
  case FIX_OBJWIDTH:
    dWidth  = r1.x2 - r1.x1;
    dHeight = dWidth / dAspectRatio;
    break;

  case FIX_OBJHEIGHT:
    dHeight = r1.y2 - r1.y1;
    dWidth  = dHeight * dAspectRatio;
    break;

  default: 
    cerr << "Error in ImgDescrList::compRelDist(): "
         << "Unknown type for parameter ('which obj dimension to fix?'): "
         << eFixObjDim << "!" << endl;
    return -1.0;
  }

  double xdist = (double)(r1.x1+r1.x2-r2.x1-r2.x2) / dWidth;
  double ydist = (double)(r1.y1+r1.y2-r2.y1-r2.y2) / dHeight;
  return sqrt(xdist*xdist + ydist*ydist);
}


bool ImgDescrList::isMatching( Rect& r1, Rect& r2, 
                               double dTDist, double dTCover, double dTOverlap)
  /*******************************************************************/
  /* Check if r2 matches with r1 according to 3 threshold values.    */
  /* Parameters:                                                     */
  /*   r1, r2: The rectangles. Attention: the order does matter!     */
  /*   TDist   : maximal relative distance r2 is allowed to have     */
  /*             from r1 (always >= 0.0).                            */
  /*             If TDist is > (1 - TCover/2 - TOverlap/2), then     */
  /*             TDist is redundant.                                 */
  /*   TCover  : minimal fraction of r1 that has to be covered by r2 */ 
  /*             (meaningful values are between 0 and 1).            */
  /*   TOverlap: minimal fraction of r2 that has to be covered by r1 */
  /*             (meaningful values are between 0 and 1).            */
  /* Return Value:                                                   */
  /*   true iff r2 passes all 3 threshold tests.                     */
  /*******************************************************************/
{
  return ( (compRelDist(r1,r2) <= dTDist) && 
           (compCover(r1,r2) >= dTCover) && 
           (compCover(r2,r1) >= dTOverlap) );
}


bool ImgDescrList::isMatching( Rect& r1, Rect& r2, 
                               double dTDist, double dTCover, double dTOverlap,
                               float dAspectRatio, FixDimType eFixObjDim )
  /*******************************************************************/
  /* Check if r2 matches with r1 according to 3 threshold values.    */
  /* This version of the function uses a ground truth box with fixed */
  /* aspect ratio as reference.                                      */
  /* Parameters:                                                     */
  /*   r1, r2: The rectangles. Attention: the order does matter!     */
  /*   TDist   : maximal relative distance r2 is allowed to have     */
  /*             from r1 (always >= 0.0).                            */
  /*             If TDist is > (1 - TCover/2 - TOverlap/2), then     */
  /*             TDist is redundant.                                 */
  /*   TCover  : minimal fraction of r1 that has to be covered by r2 */ 
  /*             (meaningful values are between 0 and 1).            */
  /*   TOverlap: minimal fraction of r2 that has to be covered by r1 */
  /*             (meaningful values are between 0 and 1).            */
  /*   dAspectRatio: the fixed aspect ratio for the first rect.      */
  /*   eFixObjDim  : the information whether the object width or     */
  /*                 height shall be fixed for the computation.      */
  /*                 (allowed values: FIX_OBJWIDTH, FIX_OBJHEIGHT)   */
  /* Return Value:                                                   */
  /*   true iff r2 passes all 3 threshold tests.                     */
  /*******************************************************************/
{
  return ( (compRelDist(r1,r2,dAspectRatio,eFixObjDim) <= dTDist) && 
           (compCover(r1,r2) >= dTCover) && 
           (compCover(r2,r1) >= dTOverlap) );
}
