
#ifndef INTEGRALIMAGE_H
#define INTEGRALIMAGE_H

#include <qimage.h>
#include <opgrayimage.hh>

#include <array2d.h>

using namespace std;

class IntegralImage : public Array2D<double>
{
private:
    int _validPixels;
    //int _w, _h;
    // long* _data;

public:
    IntegralImage();
    IntegralImage(int w, int h);
    //IntegralImage(const IntegralImage& other);
    ~IntegralImage();
        
    //int operator()(int x,int y);
    void computeFromQImage(const QImage&);
    void computeFromQImage(const OpGrayImage&);
    void computeFromQImage(const OpGrayImage&, const OpGrayImage& mask);
    double getImageArea(int x, int y, int w, int h) const;
    double getAreaAverage(int x, int y, int w, int h) const;
    
};

#endif
