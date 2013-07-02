#include "hsiimage.h"

#include <algorithm>
#include <math.h>

const double HSIPixel::UNDEFINED = -1.0;

HSIPixel::HSIPixel(const double h,const double s,const double i){
  if((h>=0.0 && h<=360.0) || h==UNDEFINED)
    _h = h;
  else
    throw OutOfRangeException();
  if(s>=0.0 && s<=1.0)
    _s = s;
  else
    throw OutOfRangeException();
  if(i>=0.0 && i<=1.0)
    _i = i;
  else
    throw OutOfRangeException();
  if(_s==0.0 && _h!=UNDEFINED)
    throw OutOfRangeException();
}

HSIPixel::HSIPixel(const HSIPixel& src){
  this->operator=(src);
}

HSIPixel::HSIPixel(const RGBPixel& src){
  this->operator=(src);
}

HSIPixel::operator RGBPixel() const {
  double r,g,b;

  if(_s == 0.0)               // color is on black&white center line.
    if(_h == UNDEFINED)       // achromatic color: no hue.
      r = g = b = _i;
    else
      throw InternalErrorException();
  else{                       // chromatic color: s!=0, h!=undef.
    double h,f,p,q,t;
    int i;
    
    h = _h/60.0;
    i = (int)floor(h);
    f = h-(double)i;
    p = _i*(1.0-_s);
    q = _i*(1.0-_s*f);
    t = _i*(1.0-_s*(1.0-f));
    switch(i){
    case 0: r = _i; g = t; b = p; break;
    case 1: r = q; g = _i; b = p; break;
    case 2: r = p; g = _i; b = t; break;
    case 3: r = p; g = q; b = _i; break;
    case 4: r = t; g = p; b = _i; break;
    case 5: r = _i; g = p; b = q; break;
    }
  }

  return RGBPixel((int)(255.0*r),(int)(255.0*g),(int)(255.0*b));
}

HSIPixel& HSIPixel::operator=(const HSIPixel& src){
  _h = src._h;
  _s = src._s;
  _i = src._i;

  return *this;
}

HSIPixel& HSIPixel::operator=(const RGBPixel& src){
  double r = src.red()/255.0;
  double g = src.green()/255.0;
  double b = src.blue()/255.0;
  double max = std::max(r,std::max(g,b));
  double min = std::min(r,std::min(g,b));

  _i = max;
  _s = (max != 0.0)?((max-min)/max):0.0;
  if(_s == 0.0)
    _h = UNDEFINED;
  else{
    double delta = max-min;
    if(r == max)
      _h = (g-b)/delta;
    else if(g == max)
      _h=2.0+(b-r)/delta;
    else if(b == max)
      _h = 4.0+(r-g)/delta;
    _h *= 60.0;
    if(_h<0.0)
      _h+=360.0;
    if(_h==360.0)
      _h = 0.0;
  }

  return *this;
}


