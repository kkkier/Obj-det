#include "orientationenergy.hh"

#include "convolve.hh"
#include <canny.hh>
#include <iostream>

//#include <math.h>

void computeOrientationEnergy( const GrayImage& sourceImg, const GaussFilterbank& filterbank,
                               GrayImage& energyImg, GrayImage& scaleImg,
                               GrayImage& orientationImg )
{
  int nScales = filterbank.nGaussScales();
  int nOrientations = filterbank.nGaussOrientations();

  int w = sourceImg.width();
  int h = sourceImg.height();

  GrayImage energy( w, h );
  GrayImage scale( w, h );
  GrayImage orientation( w, h );

  cerr << "computing orientation energy [";

  // convolve with all filters
  vector<GrayImage> conImgs1;
  vector<GrayImage> conImgs2;
  for ( int s = 0; s < nScales; s++ )
  {
    for ( int r = 0; r < nOrientations; r++ )
    {
      GrayImage tmp;
      int idx = 2*(s*nOrientations + r);
      convolve( sourceImg, filterbank.getKernel( idx ), tmp );
      conImgs1.push_back( tmp );
      convolve( sourceImg, filterbank.getKernel( idx+1 ), tmp );
      conImgs2.push_back( tmp );
      cerr << '*';
    }
  }

  // orientation energy
  for ( int s = 0; s < nScales; s++ )
  {
    for ( int r = 0; r < nOrientations; r++ )
    {
      int idx = s*nOrientations + r;
      for ( int y = 0; y < h; y++ )
      {
        for ( int x = 0; x < w; x++ )
        {
          float v1 = conImgs1[idx](x,y).value();
          float v2 = conImgs2[idx](x,y).value();
          float oe = v1*v1 + v2*v2;
          if ( oe > energy(x,y).value() )
          {
            energy(x,y) = oe;
            scale(x,y) = (float)s;
            orientation(x,y) = (float)r;
          }
        }
      }
    }
  }
  cerr << ']' << endl;

  energyImg = energy;
  scaleImg = scale;
  orientationImg = orientation;
}

vector<OpGrayImage> computeOrientationPlanes(const OpGrayImage& sourceImg, const GaussFilterbank& filterbank)
{
  int nScales = filterbank.nGaussScales();
  int nOrientations = filterbank.nGaussOrientations();

  int w = sourceImg.width();
  int h = sourceImg.height();

  cerr << "computing orientation energy [";

  // convolve with all filters
  vector<OpGrayImage> conImgs1;
  vector<OpGrayImage> conImgs2;
  for ( int s = 0; s < nScales; s++ )
  {
    for ( int r = 0; r < nOrientations; r++ )
    {
      GrayImage tmp;
      int idx = 2*(s*nOrientations + r);
      convolve( sourceImg, filterbank.getKernel( idx ), tmp );
      conImgs1.push_back( tmp );
      convolve( sourceImg, filterbank.getKernel( idx+1 ), tmp );
      conImgs2.push_back( tmp );
      cerr << '*';
    }
  }

  vector<OpGrayImage> planes;
  // orientation energy
  for ( int s = 0; s < nScales; s++ )
  {
    for ( int r = 0; r < nOrientations; r++ )
    {
      int idx = s*nOrientations + r;
      OpGrayImage tmp(w,h);
      for ( int y = 0; y < h; y++ )
      {
        for ( int x = 0; x < w; x++ )
        {
          float v1 = conImgs1[idx](x,y).value();
          float v2 = conImgs2[idx](x,y).value();
          float oe = v1*v1 + v2*v2;
          tmp(x,y)=oe;
        }
      }
      planes.push_back(tmp);
    }
  }
  cerr << ']' << endl;

  return planes;
}

vector<OpGrayImage> orientationCanny(const OpGrayImage& sourceImg, float sigma, float tlow, float thigh, const GaussFilterbank& filterbank)
{
    vector<OpGrayImage> planes = computeOrientationPlanes(sourceImg, filterbank);
    vector<OpGrayImage> results;
    results.insert(results.begin(), planes.begin(), planes.end());
    canny(planes[0], sigma, tlow, thigh, 0, results[0]);
    canny(planes[1], sigma, tlow, thigh, 135, results[1]);
    canny(planes[2], sigma, tlow, thigh, 90, results[2]);
    canny(planes[3], sigma, tlow, thigh, 45, results[3]);
    return results;
}
