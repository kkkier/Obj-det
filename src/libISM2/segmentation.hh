/*********************************************************************/
/*                                                                   */
/* FILE         segmentation.hh                                      */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Definition of a convenience class for storing seg-   */
/*              mentations.                                          */
/*                                                                   */
/* BEGIN        Fri Aug 05 2005                                      */
/* LAST CHANGE  Fri Aug 05 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef SEGMENTATION_HH
#define SEGMENTATION_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <cassert>

#include <opgrayimage.hh>

/*******************/
/*   Definitions   */
/*******************/

/*************************/
/*   Class Definitions   */
/*************************/
/*===================================================================*/
/*                        Class Segmentation                         */
/*===================================================================*/
class Segmentation
{
public:
  Segmentation();
  Segmentation( OpGrayImage imgPFig, OpGrayImage imgPGnd, OpGrayImage imgSeg );
  Segmentation( OpGrayImage imgPFig, OpGrayImage imgPGnd, OpGrayImage imgSeg,
                int nOffX, int nOffY, int nFullWidth, int nFullHeight );
  Segmentation( const Segmentation &other );

  Segmentation& operator=( const Segmentation &other );

protected:
  void copyFrom( const Segmentation &other );

public:
  /*******************************/
  /*   Content Access Operators  */
  /*******************************/
  OpGrayImage getImgPFig()     { return m_imgPFig; }
  OpGrayImage getImgPGnd()     { return m_imgPGnd; }
  OpGrayImage getImgSeg ()     { return m_imgSeg; }
  
  OpGrayImage getFullImgPFig() { return getFullImg( m_imgPFig ); }
  OpGrayImage getFullImgPGnd() { return getFullImg( m_imgPGnd ); }
  OpGrayImage getFullImgSeg () { return getFullImg( m_imgSeg );  }
  
  int   getOffsetX() { return m_nOffX; }
  int   getOffsetY() { return m_nOffY; }

  void  updateImgPFig( OpGrayImage imgPFig );
  void  updateImgPGnd( OpGrayImage imgPGnd );
  void  updateImgSeg ( OpGrayImage imgSeg  );

  float getSumPFig();
  float getSumSegArea();

protected:
  OpGrayImage getFullImg( OpGrayImage imgSrc );
  
public:
  /****************************/
  /*   Algebraic Operations   */
  /****************************/
  Segmentation add( const Segmentation &other ) const;
  Segmentation sub( const Segmentation &other ) const;

  Segmentation& operator+=( const Segmentation &other );
  Segmentation& operator-=( const Segmentation &other );

  friend Segmentation operator+( const Segmentation &a, 
                                 const Segmentation &b );
  friend Segmentation operator-( const Segmentation &a, 
                                 const Segmentation &b );

  friend Segmentation add   ( const vector<Segmentation> &vSeg );
  friend Segmentation avg   ( const vector<Segmentation> &vSeg );
  friend Segmentation max   ( const vector<Segmentation> &vSeg );
  friend Segmentation argmax( const vector<Segmentation> &vSeg );

protected:
  OpGrayImage m_imgPFig;
  OpGrayImage m_imgPGnd;
  OpGrayImage m_imgSeg;
  
  int   m_nOffX;
  int   m_nOffY;
  int   m_nFullWidth;
  int   m_nFullHeight;

  float m_dSumPFig;
  float m_dSumSegArea;  
  bool  m_bChangedPFig;
  bool  m_bChangedSeg;
};


/****************************/
/*   Associated Functions   */
/****************************/
Segmentation operator+( const Segmentation &a, const Segmentation &b );
Segmentation operator-( const Segmentation &a, const Segmentation &b );

Segmentation add   ( const vector<Segmentation> &vSeg );
Segmentation avg   ( const vector<Segmentation> &vSeg );
Segmentation max   ( const vector<Segmentation> &vSeg );
Segmentation argmax( const vector<Segmentation> &vSeg );


#endif
