
#ifndef FILTERBANK_HH
#define FILTERBANK_HH

#include <vector>

#include "gausskernel.hh"
#include <grayimage.hh>


// 'KERNEL_SIDE_FACTOR' is how much we multiply the largest sigma of our
// filterbank gaussians by in order to get the Kernel side.  FWHM is 2.35
// sigma, so use 3.0 to be safe (but 3.0 is multiplied by sqrt(2) to get 
// diagonal cases right..)
#define KERNEL_SIDE_FACTOR 4.2426f 

using namespace std;

class GaussFilterbank
{
public:
  GaussFilterbank();

  inline int nKernels() const          { return m_nKernels; }
  inline int nGaussScales() const     { return m_nGaussScales; }
  inline int nGaussOrientations() const { return m_nGaussOrientations; }

  void setup( const int nGaussScales,
              const int nGaussOrientations,
              const float fGaussSigmaY,
              const float fGaussX2YSigmaRatio );

  const GrayImage& getKernel( int idx ) const;

protected:
  int m_nKernels;

  vector<GrayImage> m_vKernels;

  // input parameters
  int m_nGaussScales;
  int m_nGaussOrientations;
  float m_fGaussSigmaY;
  float m_fGaussX2YSigmaRatio;

};

#endif
