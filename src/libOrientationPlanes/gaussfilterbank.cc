
#include "gaussfilterbank.hh"

#include <cassert>
#include <math.h>

/**************/
/*   public   */
/**************/

GaussFilterbank::GaussFilterbank()
  : m_nKernels( 0 ),
    m_nGaussScales( 0 ),
    m_nGaussOrientations( 0 )
{
}

void GaussFilterbank::setup( const int nGaussScales,
                             const int nGaussOrientations,
                             const float fGaussSigmaY,
                             const float fGaussX2YSigmaRatio )
{
  // clear existing kernels
  m_vKernels.clear();

  // store constructor arguments
  m_nGaussScales = nGaussScales;
  m_nGaussOrientations = nGaussOrientations;
  m_fGaussSigmaY = fGaussSigmaY;
  m_fGaussX2YSigmaRatio = fGaussX2YSigmaRatio;

  m_nKernels = m_nGaussScales * m_nGaussOrientations * 2;
  m_vKernels.reserve(m_nKernels);

  // set up elongated Gaussian Kernels
  for ( int nGaussScale = 0; nGaussScale < m_nGaussScales; nGaussScale++ )
  {
    const float fSigmaY = (float)pow( (double)m_fGaussSigmaY,
                                      (double)nGaussScale );
    const float fSigmaX = fSigmaY * m_fGaussX2YSigmaRatio;
    const float fMaxSigma = MAX( fSigmaX, fSigmaY );
    int nSideLength = F2I( KERNEL_SIDE_FACTOR * fMaxSigma );
    ODDIFY( nSideLength );

    for ( int nTheta = 0; nTheta < m_nGaussOrientations; nTheta++ )
    {
      const float fTheta = CONST_PI * (float)nTheta / (float)m_nGaussOrientations;

      GaussKernel evenKernel( nSideLength, fSigmaX, fSigmaY, 2, fTheta, false );
      GaussKernel oddKernel( nSideLength, fSigmaX, fSigmaY, 2, fTheta, true );

      m_vKernels.push_back( evenKernel );
      m_vKernels.push_back( oddKernel );
    }
  }
  
  assert( m_nKernels == (int)m_vKernels.size() );
}

const GrayImage& GaussFilterbank::getKernel( int idx ) const
{
  return m_vKernels[idx];
}
