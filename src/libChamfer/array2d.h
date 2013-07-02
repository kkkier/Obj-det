
#ifndef UTIL_H
#define UTIL_H

#include <cassert>

template <class T>
class Array2D
{
protected:
    int _w, _h;
    T* _data;

protected:
    T& value(int x, int y)
    {
        assert(x>=0 && y>=0 && x<_w && y<_h);
        return _data[x+y*_w];
    }
    
    T& value(int x, int y) const
    {
        assert(x>=0 && y>=0 && x<_w && y<_h);
        return _data[x+y*_w];
    }
public:
    Array2D()
    {
        _w=-1;
        _h=-1;
        _data=0;
    }
    
	Array2D(int w, int h)
    {
        _w = w;
        _h = h;
        _data = new T[w*h];
    }

    Array2D(const Array2D& other)
    {
        _w = other._w;
        _h = other._h;
        _data = new T[_w*_h];
        for(int i=0; i<_w*_h; i++)
            _data[i]=other._data[i];
    }
    
    ~Array2D() { delete [] _data ; }
    
    void resize(int w, int h)
    {
        _w = w;
        _h = h;
        delete [] _data;
        _data = new T[w*h];
    }
    
    void toZero()
    {
        bzero(_data, sizeof(T)*_w*_h);
    }
    
    T& operator()(int x, int y)
    { 
        assert(x>=0 && y>=0 && x<_w && y<_h);
        return _data[x+y*_w];
    }

};

#endif

