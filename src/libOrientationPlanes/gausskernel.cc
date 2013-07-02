
#include "gausskernel.hh"
#include "rotate.hh"

#include "hilbert.h"

#include <cassert>
#include <math.h>


/**************/
/*   public   */
/**************/

// initialize drawing the kernel
// 'fSigmaX' - sigma in horizontal direction
// 'fSigmaY' - sigma in vertical direction
// 'nSize' - size of the image 
// 'fOrderY' - 0 = Gaussian, 1 = 1st Gauss. deriv, 2 = 2nd deriv. etc,
// (NOTE: 'fOrderX' is always 0)
// 'bHilbert' - if true, return Hilbert transform of kernel
GaussKernel::GaussKernel( const int& nSize, const float& fSigmaX, const float& fSigmaY,
                          const int& nOrderY, const float& fTheta,
                          const bool& bHilbert )
  : OpGrayImage( nSize, nSize )
{
  draw( fSigmaX, fSigmaY, nOrderY, fTheta, bHilbert );
}


// assignment operator
GaussKernel& GaussKernel::operator=( const GaussKernel& other )
{
  OpGrayImage::operator=( other );
  return *this;
}


/*****************/
/*   protected   */
/*****************/

// initialize without drawing the kernel, but do allocate the image
GaussKernel::GaussKernel( const int& nSize )
  : OpGrayImage( nSize, nSize )
{
  // deliberately empty
}


// draw the kernel with rotation 'fTheta', given new
// parameters, in an already allocated kernel
void GaussKernel::draw( const float& fSigmaX, const float& fSigmaY, const int& nOrderY,
                        const float& fTheta, const bool& bHilbert )
{
  if ( fTheta != 0.0f )
  {
    int nNewSize = F2I( 1.5f * (float)width());
    ODDIFY( nNewSize );
    const int nHalfSizeDiff = (nNewSize - width()) / 2;

    GaussKernel bigImg( nNewSize );
    GaussKernel bigImg2( nNewSize ); // wasteful
   
    // draw into a bigger image, for rotation to come out nicely
    bigImg.drawNoRot( fSigmaX, fSigmaY, nOrderY, bHilbert );
    ::rotate( bigImg, -fTheta, 0.0f, bigImg2 );
    bigImg2.extract( *this, nHalfSizeDiff, nHalfSizeDiff );
  }
  else
  {
    drawNoRot( fSigmaX, fSigmaY, nOrderY, bHilbert );
  }

  // subtract mean
  float mean = 0.0f;
  for ( int y = 0; y < height(); y++ )
    for ( int x = 0; x < width(); x++ )
    {
      float v = (*this)(x,y).value();
      mean += v;
    }
  mean /= (float)width()*height();
  for ( int y = 0; y < height(); y++ )
    for ( int x = 0; x < width(); x++ )
    {
      (*this)(x,y) = (*this)(x,y).value() - mean;
    }


  // normalize with L1 Norm
  float l1norm = 0.0f;
  for ( int y = 0; y < height(); y++ )
    for ( int x = 0; x < width(); x++ )
      {
        float v = fabs((*this)(x,y).value());
        l1norm += v;
      }
  for ( int y = 0; y < height(); y++ )
    for ( int x = 0; x < width(); x++ )
    {
      (*this)(x,y) = (*this)(x,y).value() / l1norm;
    }
}


// draw kernel without rotating
void GaussKernel::drawNoRot( const float& fSigmaX, const float& fSigmaY, 
                             const int& nOrderY, const bool& bHilbert )
{
  m_fSigmaX = fSigmaX;
  m_fSigmaY = fSigmaY;
  m_nOrderY = nOrderY;

  const int nHalfSize = width() / 2;
  const float fNorm = 1.0f / (fSigmaX * fSigmaY * CONST_2PI);
  const float fSigmaSqrX = fSigmaX * fSigmaX;
  const float fSigmaSqrY = fSigmaY * fSigmaY;
  const float fRecipTwoSigmaSqrX = 1.0f / (2.0f * fSigmaSqrX);
  const float fRecipTwoSigmaSqrY = 1.0f / (2.0f * fSigmaSqrY);
  const float fRecipSigmaSqrY = 1.0f / fSigmaSqrY;
  const float fRecipSigmaFourthY = 1.0f / (fSigmaSqrY * fSigmaSqrY);

  float* aY = new float[width()];
  float* pY = aY;
    
  for ( int y = -nHalfSize; y <= nHalfSize; y++, pY++ )
  {
    const float ySqr = (float)(y * y);
    float fy = (float)exp( -ySqr * fRecipTwoSigmaSqrY ); // zeroth derivative

    if ( nOrderY == 1 )
    { // first derivative
      fy *= -(float)y * fRecipSigmaSqrY;
    }
    else if ( nOrderY == 2 )
    { // second derivative
      fy *= fRecipSigmaSqrY * (ySqr * fRecipSigmaSqrY - 1);
    }
    else if ( nOrderY == 3 )
    { // third derivative
      fy *= fRecipSigmaFourthY * (float)y * (3 - ySqr * fRecipSigmaSqrY);
    }
    *pY = fy;
  }
  
  if ( bHilbert )
  {
    DTLib::Hilbert( aY, width() );
  }

  pY = aY;
  for ( int y = -nHalfSize; y <= nHalfSize; y++, pY++ )
    for ( int x = -nHalfSize; x <= nHalfSize; x++ )
    {
      const float xSqr = (float)(x * x);
      const float fx = (float)exp( -xSqr * fRecipTwoSigmaSqrX );
      (*this)(x + nHalfSize, y + nHalfSize) = fx * (*pY) * fNorm;
    }

  ZAP(aY);

  if (nOrderY == 0)
  { // isotropic Gaussian kernel
    opNormalizeRange( 0.0f, 1.0f );
  }

}


// Extract a portion of this image, starting at x = 'nLeft' and y =
// 'nTop' and going to the right 'destImg.width()' pixels and to the
// bottom 'destImg.height()' pixels.  The extracted region fills up
// destImg completely.
void GaussKernel::extract( OpGrayImage& destImg, const int& nLeft,
                           const int& nTop )
{
  assert( (nLeft + destImg.width()) < width() &&
          (nTop + destImg.height()) < height() );

  for ( int x = 0; x < destImg.width(); x++ )
    for ( int y = 0; y < destImg.height(); y++ )
      destImg( x, y ) = (*this)( x + nLeft, y + nTop ).value();
}
