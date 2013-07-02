/*********************************************************************/
/*                                                                   */
/* FILE         segmentation.cc                                      */
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

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <algorithm>

#include "segmentation.hh"

/*===================================================================*/
/*                         Class Segmentation                        */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

Segmentation::Segmentation()
/* standard constructor when the segmentation is yet unknown */
{
  m_nOffX       = 0;
  m_nOffY       = 0;
  m_nFullWidth  = -1;
  m_nFullHeight = -1;

  m_dSumPFig     = -1.0;
  m_dSumSegArea  = -1.0;
  m_bChangedPFig = true;
  m_bChangedSeg  = true;
}


Segmentation::Segmentation( OpGrayImage imgPFig, OpGrayImage imgPGnd, 
                            OpGrayImage imgSeg )
/* standard constructor for full-sized segmentations */
{
  assert( (imgPFig.width() == imgPGnd.width()) && 
          (imgPFig.width() == imgSeg.width()) );
  assert( (imgPFig.height() == imgPGnd.height()) && 
          (imgPFig.height() == imgSeg.height()) );

  m_imgPFig = imgPFig;
  m_imgPGnd = imgPGnd;
  m_imgSeg  = imgSeg;

  m_nOffX       = 0;
  m_nOffY       = 0;
  m_nFullWidth  = -1;
  m_nFullHeight = -1;

  m_dSumPFig     = -1.0;
  m_dSumSegArea  = -1.0;
  m_bChangedPFig = true;
  m_bChangedSeg  = true;
}


Segmentation::Segmentation( OpGrayImage imgPFig, OpGrayImage imgPGnd, 
                            OpGrayImage imgSeg,
                            int nOffX, int nOffY,
                            int nFullWidth, int nFullHeight )
/* standard constructor for smaller segmentations */
{
  assert( (imgPFig.width() == imgPGnd.width()) && 
          (imgPFig.width() == imgSeg.width()) );
  assert( (imgPFig.height() == imgPGnd.height()) && 
          (imgPFig.height() == imgSeg.height()) );

  m_imgPFig = imgPFig;
  m_imgPGnd = imgPGnd;
  m_imgSeg  = imgSeg;

  m_nOffX       = nOffX;
  m_nOffY       = nOffY;
  m_nFullWidth  = nFullWidth;
  m_nFullHeight = nFullHeight;

  m_dSumPFig     = -1.0;
  m_dSumSegArea  = -1.0;
  m_bChangedPFig = true;
  m_bChangedSeg  = true;
}


Segmentation::Segmentation( const Segmentation &other )
/* copy constructor */
{
  copyFrom( other );
}


Segmentation& Segmentation::operator=( const Segmentation &other )
/* assignment operator */
{
  copyFrom( other );
  return (*this);
}


void Segmentation::copyFrom( const Segmentation &other )
{
  m_imgPFig = other.m_imgPFig;
  m_imgPGnd = other.m_imgPGnd;
  m_imgSeg  = other.m_imgSeg;
  
  m_nOffX   = other.m_nOffX;
  m_nOffY   = other.m_nOffY;
  m_nFullWidth  = other.m_nFullWidth;
  m_nFullHeight = other.m_nFullHeight;

  m_dSumPFig     = other.m_dSumPFig;
  m_dSumSegArea  = other.m_dSumSegArea;
  m_bChangedPFig = other.m_bChangedPFig;
  m_bChangedSeg  = other.m_bChangedSeg;
}


/***********************************************************/
/*                    Access Functions                     */
/***********************************************************/

void Segmentation::updateImgPFig( OpGrayImage imgPFig )
{
  assert( (m_imgPFig.width()==imgPFig.width()) &&
          (m_imgPFig.height()==imgPFig.height()) );
  
  m_imgPFig = imgPFig;
  m_bChangedPFig = true;
}


void Segmentation::updateImgPGnd( OpGrayImage imgPGnd )
{
  assert( (m_imgPGnd.width()==imgPGnd.width()) &&
          (m_imgPGnd.height()==imgPGnd.height()) );
  
  m_imgPGnd = imgPGnd;
  m_bChangedPFig = true;
}


void Segmentation::updateImgSeg ( OpGrayImage imgSeg  )
{
  assert( (m_imgSeg.width()==imgSeg.width()) &&
          (m_imgSeg.height()==imgSeg.height()) );
  
  m_imgSeg = imgSeg;
  m_bChangedSeg = true;
}


float Segmentation::getSumPFig()
{
  if( m_bChangedPFig ) {
    m_bChangedPFig = false;
    // segmentation has been updated => compute new value
    //float dSumPFig = 0.0;
    //for( int y=0; y<(int)imgPFig.height(); y++ )
    //  for( int x=0; x<(int)imgPFig.width(); x++ ) {
    //    float val = imgPFig(x,y).value();
    //    if( val > imgPGnd(x,y).value() )
    //      dSumPFig += val;
    //  }
    m_dSumPFig = m_imgPFig.getSum();
  }
  return m_dSumPFig;
}


float Segmentation::getSumSegArea()
{
  if( m_bChangedSeg ) {
    // segmentation has been updated => compute new value
    m_bChangedSeg = false;
    m_dSumSegArea = m_imgSeg.getSum()/255.0;
  }
  return m_dSumSegArea;
}


OpGrayImage Segmentation::getFullImg( OpGrayImage imgSrc )
{
  if( (m_nFullWidth<0) || (m_nFullHeight<0) ) {
    // image already has the full size
    cout << "  Warning: image size not yet defined: (" << m_nFullWidth
         << "," << m_nFullHeight << ")" << endl;
    return imgSrc;

  } else {
    OpGrayImage imgRes( m_nFullWidth, m_nFullHeight );
    return imgRes.copy( imgSrc, m_nOffX, m_nOffY );
  }
}

  
/***********************************************************/
/*                  Algebraic Operations                   */
/***********************************************************/

Segmentation Segmentation::add( const Segmentation &other ) const
{
  /* check if the two segmentations are compatible */
  assert( m_nFullWidth==other.m_nFullWidth );
  assert( m_nFullHeight==other.m_nFullHeight );
  assert( m_nOffX==other.m_nOffX && m_nOffY==other.m_nOffY );

  OpGrayImage imgPFig = m_imgPFig + other.m_imgPFig;
  OpGrayImage imgPGnd = m_imgPGnd + other.m_imgPGnd;
  OpGrayImage imgSeg  = (m_imgSeg + other.m_imgSeg)/2.0;

  Segmentation segResult( imgPFig, imgPGnd, imgSeg,
                          m_nOffX, m_nOffY, m_nFullWidth, m_nFullHeight );

  return segResult;
}


Segmentation Segmentation::sub( const Segmentation &other ) const
{
  /* check if the two segmentations are compatible */
  assert( m_nFullWidth==other.m_nFullWidth );
  assert( m_nFullHeight==other.m_nFullHeight );
  assert( m_nOffX==other.m_nOffX && m_nOffY==other.m_nOffY );

  OpGrayImage imgPFig = m_imgPFig - other.m_imgPFig;
  OpGrayImage imgPGnd = m_imgPGnd - other.m_imgPGnd;
  OpGrayImage imgSeg  = (m_imgSeg - other.m_imgSeg)*2.0;

  Segmentation segResult( imgPFig, imgPGnd, imgSeg,
                          m_nOffX, m_nOffY, m_nFullWidth, m_nFullHeight );

  return segResult;
}


Segmentation& Segmentation::operator+=( const Segmentation &other )
{
  *this = add( other );
  return *this;
}


Segmentation& Segmentation::operator-=( const Segmentation &other )
{
  *this = sub( other );
  return *this;
}


/***********************************************************/
/*                  Associated Functions                   */
/***********************************************************/
Segmentation operator+( const Segmentation &a, const Segmentation &b )
{
  return a.add( b );
}


Segmentation operator-( const Segmentation &a, const Segmentation &b )
{
  return a.sub( b );
}


Segmentation add( const vector<Segmentation> &vSeg )
{
  assert( vSeg.size() > 0 );
  int nOffX       = vSeg[0].m_nOffX;
  int nOffY       = vSeg[0].m_nOffY;
  int nWidth      = vSeg[0].m_imgPFig.width();
  int nHeight     = vSeg[0].m_imgPFig.height();
  int nFullWidth  = vSeg[0].m_nFullWidth;
  int nFullHeight = vSeg[0].m_nFullHeight;

  OpGrayImage imgPFig( vSeg[0].m_imgPFig );
  OpGrayImage imgPGnd( vSeg[0].m_imgPGnd );
  OpGrayImage imgSeg ( vSeg[0].m_imgSeg );
  
  for( unsigned k=1; k<vSeg.size(); k++ ) {
    /* check if all segmentations are compatible */
    assert( vSeg[k].m_nFullWidth ==nFullWidth );
    assert( vSeg[k].m_nFullHeight==nFullHeight );
    assert( vSeg[k].m_imgPFig.width() ==nWidth );
    assert( vSeg[k].m_imgPFig.height()==nHeight );
    assert( vSeg[k].m_nOffX==nOffX && vSeg[k].m_nOffY==nOffY );

    imgPFig = imgPFig + vSeg[k].m_imgPFig;
    imgPGnd = imgPGnd + vSeg[k].m_imgPGnd;
    imgSeg  = imgSeg  + vSeg[k].m_imgSeg;
  }
  imgSeg = imgSeg / (float)(vSeg.size() );

  Segmentation segResult( imgPFig, imgPGnd, imgSeg,
                          nOffX, nOffY, nFullWidth, nFullHeight );

  return segResult;
}


Segmentation avg( const vector<Segmentation> &vSeg )
{
  assert( vSeg.size() > 0 );
  int nOffX       = vSeg[0].m_nOffX;
  int nOffY       = vSeg[0].m_nOffY;
  int nWidth      = vSeg[0].m_imgPFig.width();
  int nHeight     = vSeg[0].m_imgPFig.height();
  int nFullWidth  = vSeg[0].m_nFullWidth;
  int nFullHeight = vSeg[0].m_nFullHeight;

  OpGrayImage imgPFig ( nWidth, nHeight );
  OpGrayImage imgPGnd ( nWidth, nHeight );
  OpGrayImage imgSeg  ( nWidth, nHeight );
  OpGrayImage imgCount( nWidth, nHeight );
  
  for( unsigned k=0; k<vSeg.size(); k++ ) {
    /* check if all segmentations are compatible */
    assert( vSeg[k].m_nFullWidth ==nFullWidth );
    assert( vSeg[k].m_nFullHeight==nFullHeight );
    if( vSeg[k].m_imgPFig.width() != nWidth ) {
      cerr << "ERROR in Segmentation::avg(): "
           << "Segmentations have inconsistent size at element(" << k 
           << ")! " << endl;
      for(unsigned i=0; i<vSeg.size(); i++ )
        cerr << "    Seg " << i << ": " << vSeg[i].m_imgPFig.width() << "x"
             << vSeg[i].m_imgPFig.height() << endl;
      cerr << endl;
      //exit(0);
    }
    assert( vSeg[k].m_imgPFig.width() ==nWidth );
    assert( vSeg[k].m_imgPFig.height()==nHeight );
    assert( vSeg[k].m_nOffX==nOffX && vSeg[k].m_nOffY==nOffY );

    for( int y=0; y<nHeight; y++ )
      for( int x=0; x<nWidth; x++ )
        if( vSeg[k].m_imgPFig(x,y).value()>0.0 ||
            vSeg[k].m_imgPGnd(x,y).value()>0.0 ) {
          imgPFig (x,y)= imgPFig(x,y).value() + vSeg[k].m_imgPFig(x,y).value();
          imgPGnd (x,y)= imgPGnd(x,y).value() + vSeg[k].m_imgPGnd(x,y).value();
          imgSeg  (x,y)= imgSeg (x,y).value() + vSeg[k].m_imgSeg(x,y).value();
          imgCount(x,y)= imgCount(x,y).value() + 1.0;
        }
  }
  for( int y=0; y<nHeight; y++ )
    for( int x=0; x<nWidth; x++ ) {
      float dNormVal = imgCount(x,y).value();
      if( dNormVal > 0.0 ) {
        imgPFig(x,y) = imgPFig(x,y).value() / dNormVal;
        imgPGnd(x,y) = imgPGnd(x,y).value() / dNormVal;
        imgSeg (x,y) = imgSeg (x,y).value() / dNormVal;
      } else {
        imgPFig(x,y) = 0.0;
        imgPGnd(x,y) = 0.0;
        imgSeg (x,y) = 0.0;
      }
    }

  Segmentation segResult( imgPFig, imgPGnd, imgSeg,
                          nOffX, nOffY, nFullWidth, nFullHeight );

  return segResult;
}


Segmentation max( const vector<Segmentation> &vSeg )
{
  assert( vSeg.size() > 0 );
  int nOffX       = vSeg[0].m_nOffX;
  int nOffY       = vSeg[0].m_nOffY;
  int nWidth      = vSeg[0].m_imgPFig.width();
  int nHeight     = vSeg[0].m_imgPFig.height();
  int nFullWidth  = vSeg[0].m_nFullWidth;
  int nFullHeight = vSeg[0].m_nFullHeight;

  /* check if all segmentations are compatible */
  for( unsigned k=0; k<vSeg.size(); k++ ) {
    assert( vSeg[k].m_nFullWidth ==nFullWidth );
    assert( vSeg[k].m_nFullHeight==nFullHeight );
    assert( vSeg[k].m_imgPFig.width() ==nWidth );
    assert( vSeg[k].m_imgPFig.height()==nHeight );
    assert( vSeg[k].m_nOffX==nOffX && vSeg[k].m_nOffY==nOffY );
  }

  OpGrayImage imgPFig( nWidth, nHeight );
  OpGrayImage imgPGnd( nWidth, nHeight );
  OpGrayImage imgSeg ( nWidth, nHeight );
  
  /* find the maximum value for each pixel */
  for(int y=0; y<nHeight; y++ )
    for( int x=0; x<nWidth; x++ ) {
      float dMaxPFig = vSeg[0].m_imgPFig(x,y).value();
      float dMaxPGnd = vSeg[0].m_imgPGnd(x,y).value();
      float dMaxSeg  = vSeg[0].m_imgSeg(x,y).value();
      for( unsigned k=1; k<vSeg.size(); k++ ) {
        if( vSeg[k].m_imgPFig(x,y).value() > dMaxPFig ) 
          dMaxPFig = vSeg[k].m_imgPFig(x,y).value();
        if( vSeg[k].m_imgPGnd(x,y).value() > dMaxPGnd ) 
          dMaxPGnd = vSeg[k].m_imgPGnd(x,y).value();
        if( vSeg[k].m_imgSeg(x,y).value() > dMaxSeg ) 
          dMaxSeg  = vSeg[k].m_imgSeg(x,y).value();
      }
      imgPFig(x,y) = dMaxPFig;
      imgPGnd(x,y) = dMaxPGnd;
      imgSeg (x,y) = dMaxSeg;
  }

  Segmentation segResult( imgPFig, imgPGnd, imgSeg,
                          nOffX, nOffY, nFullWidth, nFullHeight );

  return segResult;
}


Segmentation argmax( const vector<Segmentation> &vSeg )
{
  assert( vSeg.size() > 0 );
  int nOffX       = vSeg[0].m_nOffX;
  int nOffY       = vSeg[0].m_nOffY;
  int nWidth      = vSeg[0].m_imgPFig.width();
  int nHeight     = vSeg[0].m_imgPFig.height();
  int nFullWidth  = vSeg[0].m_nFullWidth;
  int nFullHeight = vSeg[0].m_nFullHeight;

  /* check if all segmentations are compatible */
  for( unsigned k=0; k<vSeg.size(); k++ ) {
    assert( vSeg[k].m_nFullWidth ==nFullWidth );
    assert( vSeg[k].m_nFullHeight==nFullHeight );
    assert( vSeg[k].m_imgPFig.width() ==nWidth );
    assert( vSeg[k].m_imgPFig.height()==nHeight );
    assert( vSeg[k].m_nOffX==nOffX && vSeg[k].m_nOffY==nOffY );
  }

  OpGrayImage imgPFig( nWidth, nHeight );
  OpGrayImage imgPGnd( nWidth, nHeight );
  OpGrayImage imgSeg ( nWidth, nHeight );
  
  /* find the maximum idx for each pixel */
  for(int y=0; y<nHeight; y++ )
    for( int x=0; x<nWidth; x++ ) {
      float dMaxPFig = 0.0;
      float dMaxPGnd = 0.0;
      float dMaxSeg  = 0.0;
      int   nIdxPFig = -1;
      int   nIdxPGnd = -1;
      int   nIdxSeg  = -1;
      for( unsigned k=0; k<vSeg.size(); k++ ) {
        if( vSeg[k].m_imgPFig(x,y).value() > dMaxPFig ) {
          dMaxPFig = vSeg[k].m_imgPFig(x,y).value();
          nIdxPFig = k;
        }
        if( vSeg[k].m_imgPGnd(x,y).value() > dMaxPGnd ) {
          dMaxPGnd = vSeg[k].m_imgPGnd(x,y).value();
          nIdxPGnd = k;
        }
        if( vSeg[k].m_imgSeg(x,y).value() > dMaxSeg ) {
          dMaxSeg  = vSeg[k].m_imgSeg(x,y).value();
          nIdxSeg  = k;
        }
      }
      imgPFig(x,y) = (float)nIdxPFig;
      imgPGnd(x,y) = (float)nIdxPGnd;
      imgSeg (x,y) = (float)nIdxSeg;
  }

  Segmentation segResult( imgPFig, imgPGnd, imgSeg,
                          nOffX, nOffY, nFullWidth, nFullHeight );

  return segResult;
}

