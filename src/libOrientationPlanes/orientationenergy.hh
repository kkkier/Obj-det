#ifndef ORIENTATIONENERGY_HH
#define ORIENTATIONENERGY_HH

#include <grayimage.hh>

#include "gaussfilterbank.hh"

void computeOrientationEnergy( const GrayImage& sourceImg, const GaussFilterbank& filterbank,
                               GrayImage& energyImg, GrayImage& scaleImg,
                               GrayImage& orientationImg );

vector<OpGrayImage> computeOrientationPlanes( const OpGrayImage& sourceImg, const GaussFilterbank& filterbank);

vector<OpGrayImage> orientationCanny( const OpGrayImage& img, float sigma, float tlow, float thigh, const GaussFilterbank& filterbank);

#endif
