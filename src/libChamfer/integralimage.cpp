
#include "integralimage.h"
#include <iostream>
#include <cassert>

IntegralImage::IntegralImage()
{
}

IntegralImage::IntegralImage(int w, int h)
{
    bzero(_data, sizeof(long)*w*h);
    _validPixels=0;
}

IntegralImage::~IntegralImage()
{

}

void IntegralImage::computeFromQImage(const QImage& img)
{
    //cout << "IntegralImage::computeFromQImage()\n";
    
    // check dimensions and resize if necessary //
    if (_w!=img.width() || _h!=img.height())
    {
        //cout << "Resizing integral image\n";
        this->resize(img.width(), img.height());
        this->toZero();
    }
    
    Array2D<double> s(_w, _h);
    s.toZero();
        
    // compute integralimage see viola&jones //
    for(int x=0; x<_w; x++)
    {
        for(int y=0; y<_h; y++)
        {
            QRgb val = qGray(img.pixel(x,y));
            if (y>0)
            {
                //if (y<10 && x<10) cout << "y>0: " << s(x,y-1) << " " << qGray(img.pixel(x,y)) <<endl;
                s(x,y) = s(x, y-1) + val;
            }    
            else         s(x,y) = val;
            if (x>0) {
                //if (x<10 && y<10) cout << "x>0: " << this->value(x-1,y)<< " " << s(x,y) <<endl;
                this->value(x,y) = this->value(x-1,y) + s(x,y);
            }
            else     this->value(x,y) = s(x,y);
            //if (x<10 && y<10) cout << "("<<x<<","<<y<<"): " << s(x,y) << "\t"<<this->value(x,y) << endl;
        }
    }
}

void IntegralImage::computeFromQImage(const OpGrayImage& img)
{
    //cout << "IntegralImage::computeFromQImage()\n";
    
    // check dimensions and resize if necessary //
    if (_w!=img.width() || _h!=img.height())
    {
        //cout << "Resizing integral image\n";
        this->resize(img.width(), img.height());
        this->toZero();
    }
    
    Array2D<double> s(_w, _h);
    s.toZero();
        
    // compute integralimage see viola&jones //
    for(int x=0; x<_w; x++)
    {
        for(int y=0; y<_h; y++)
        {
            float val = img(x,y).value();
            _validPixels++;
            if (y>0)
            {
                //if (y<10 && x<10) cout << "y>0: " << s(x,y-1) << " " << qGray(img.pixel(x,y)) <<endl;
                s(x,y) = s(x, y-1) + val;
            }    
            else         s(x,y) = val;
            if (x>0) {
                //if (x<10 && y<10) cout << "x>0: " << this->value(x-1,y)<< " " << s(x,y) <<endl;
                this->value(x,y) = this->value(x-1,y) + s(x,y);
            }
            else     this->value(x,y) = s(x,y);
            //if (x<10 && y<10) cout << "("<<x<<","<<y<<"): " << s(x,y) << "\t"<<this->value(x,y) << endl;
        }
    }
}

void IntegralImage::computeFromQImage(const OpGrayImage& img, const OpGrayImage& mask)
{
    cout << "IntegralImage::computeFromQImage()\n";
    
    // check dimensions and resize if necessary //
    if (_w!=img.width() || _h!=img.height())
    {
        cout << "Resizing integral image\n";
        this->resize(img.width(), img.height());
        this->toZero();
    }
    
    Array2D<double> s(_w, _h);
    s.toZero();
        
    // compute integralimage see viola&jones //
    for(int x=0; x<_w; x++)
    {
        for(int y=0; y<_h; y++)
        {
            float val;
            if (mask(x,y).value()==0)
            {
                val = img(x,y).value();
                _validPixels++;
            }
            else val=0;
            
            if (y>0)
            {
                //if (y<10 && x<10) cout << "y>0: " << s(x,y-1) << " " << qGray(img.pixel(x,y)) <<endl;
                s(x,y) = s(x, y-1) + val;
            }    
            else         s(x,y) = val;
            if (x>0) {
                //if (x<10 && y<10) cout << "x>0: " << this->value(x-1,y)<< " " << s(x,y) <<endl;
                this->value(x,y) = this->value(x-1,y) + s(x,y);
            }
            else     this->value(x,y) = s(x,y);
            //if (x<10 && y<10) cout << "("<<x<<","<<y<<"): " << s(x,y) << "\t"<<this->value(x,y) << endl;
        }
    }
}

double IntegralImage::getImageArea(int x, int y, int w, int h) const
{
    if (_data==0) {cout << "Integral image has yet to be computed -> use computeFromImage"<< endl; return -1;}
    else
    {
        //printf("width: %d height: %d x: %d y: %d w:%d h:%d\n", _w, _h, x, y, w, h);
        if (x<0) {w+=x; x=0;}
        if (y<0) {h+=y; y=0;}
        if (x+w>=_w) w=_w-x-1;
        if (y+h>=_h) h=_h-y-1;
        if (w<=0 || h<=0) return 0;
        //printf("width: %d height: %d x: %d y: %d w:%d h:%d\n", _w, _h, x, y, w, h);
        return this->value(x+w,y+h) - this->value(x+w,y) - this->value(x,y+h) + this->value(x,y);
    }
}

double IntegralImage::getAreaAverage(int x, int y, int w, int h) const
{
    return getImageArea(x,y,w,h)/(double)_validPixels;
}
