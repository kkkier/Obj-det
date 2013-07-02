/******************************************************************************
 * $Id: image.h,v 1.2 2005/04/26 15:20:35 leibe Exp $
 * $Author: leibe $
 * $Date: 2005/04/26 15:20:35 $
 *------------------------------ Description ----------------------------------
 * The 'Image' data type is the abstraction of images, i.e. two-dimensional
 * arrays of 'Pixel' values. Basic functionalities like copying, assignment, 
 * and access in either read-only or read-write mode.
 *------------------------------- CVS logs ------------------------------------
 * $Log: image.h,v $
 * Revision 1.2  2005/04/26 15:20:35  leibe
 * Most recent version.
 *
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.2  2004/04/26 15:16:32  leibe
 *
 * reinstated the original version of the library 'pccvImage'.
 *
 * Revision 1.11  2002/05/15 13:01:48  spengler
 * *** empty log message ***
 *
 * Revision 1.10  2002/04/02 14:14:00  spengler
 * Memory management bug removed: Memory of unused ImageData
 * (refcounter <= 1)  will now be freed in the copy function.
 *
 * Revision 1.9  2002/01/30 14:26:32  spengler
 * New method isEmpty() added. Will return 'true' if no true image data is 
 * available and otherwise 'false'. No valid image data means 
 * width()=height()=0.
 *
 * Revision 1.8  2001/12/11 15:44:09  spengler
 * Comparator for images added: Template class 'Image<Pixel>' has now
 * an implementation for operator==(). If the two Image<pixel> instances
 * use the same internal data structure, they are considered as equal.
 * If they use different data storage structures which hold the same, but
 * individually loaded, image, they are considered as not-equal!
 *
 * Revision 1.7  2001/12/10 14:05:24  spengler
 * Assignment operator bug fixed: The assignment operator of class
 * template 'Image' takes now a const Image<Pixel>& as parameter.
 * This fixes the problem with assignments like
 *
 * Image<int> img = Image<int>(w,h);
 *
 * The compiler will no longer complain about const/non-const issues
 * in this case. Beside that, declaring the source image of the assign-
 * ment operator as const seems to be logically more reasonable.
 *
 * Revision 1.6  2001/09/04 17:00:22  spengler
 * Class Image<Pixel> is now multi-thread ready. Invoke compiling with
 * preprocessor constant __THREAD_SAVE__ defined.
 *
 * Revision 1.5  2001/08/20 15:44:06  spengler
 * Version 1 of image class and related utility classes and libraries
 * finished. Ready for distribution
 *
 * Revision 1.4  2001/08/17 17:01:36  spengler
 * Running code before weekend (08/17/2001)
 *
 * Revision 1.3  2001/08/16 12:37:40  spengler
 * Image template now works!
 *
 * Revision 1.2  2001/08/15 16:27:06  spengler
 * Basic functionalities implemented but not linkable due to template
 * related errors. It seems as if the static member _ilist of class Image<>
 * cannot be found by the linker.
 *
 * Revision 1.1.1.1  2001/08/15 13:12:52  spengler
 *
 *
 *****************************************************************************/

#ifndef _SPENGLER_IMAGE_H
#define _SPENGLER_IMAGE_H

using namespace std;

#include <string.h>
#include <algorithm>

#ifdef __THREAD_SAVE__
#include <pthread.h>
#endif

template<class Pixel> class Image;

/******************************************************************************
 *                              class ImageData                               *
 *****************************************************************************/
template<class Pixel>
class ImageData
{
  ImageData();
  ImageData(const int, const int,const Pixel& =Pixel());
  ~ImageData();

  ImageData<Pixel>* clone() const;

  int _refcount;
  int _width;
  int _height;
  Pixel *_data;

#ifdef __THREAD_SAVE__
  pthread_mutex_t _mutex;
#endif

 public:
  class ObjectInUseException{};

  friend class Image<Pixel>;
};

//============================== Implementation ===============================

//-----------------------------------------------------------------------------
// Default constructor
//-----------------------------------------------------------------------------
template<class Pixel>
ImageData<Pixel>::ImageData(){
  _refcount = 1;
  _width = 0;
  _height = 0;
  _data = 0;

#ifdef __THREAD_SAVE__
  pthread_mutex_init(&_mutex,0);
#endif
}

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
template<class Pixel>
ImageData<Pixel>::ImageData(const int width, const int height,
			    const Pixel& value)
  : _width(width), _height(height)
{
  _refcount = 1;
  if(_width*_height > 0){
    _data = new Pixel[width*height];
    for(int i=0;i<_width*_height;i++) _data[i] = value;
  }
  else{
    _width = 0;
    _height = 0;
    _data = 0;
  }

#ifdef __THREAD_SAVE__
  pthread_mutex_init(&_mutex,0);
#endif
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
template<class Pixel>
ImageData<Pixel>::~ImageData(){
#ifdef __THREAD_SAVE__
  pthread_mutex_lock(&_mutex);
#endif

  if(_refcount > 1)
    throw ObjectInUseException();
  
  if(_data != 0)
    delete[] _data;

#ifdef __THREAD_SAVE__
  pthread_mutex_unlock(&_mutex);
  pthread_mutex_destroy(&_mutex);
#endif
}

//-----------------------------------------------------------------------------
// Clone: creates an identical duplicate except for the reference counter
// _refcount which is set to 1.
//-----------------------------------------------------------------------------
template<class Pixel>
ImageData<Pixel>* ImageData<Pixel>::clone() const {
  ImageData<Pixel> *tmp = new ImageData<Pixel>(_width,_height);
  memcpy(tmp->_data,_data,sizeof(Pixel)*tmp->_width*tmp->_height);
  tmp->_refcount = 1;
  return tmp;
}

/******************************************************************************
 *                                  class Image                               *
 *****************************************************************************/
template<class Pixel>
class Image
{
 public:
  Image();
  Image(const int, const int,const Pixel& =Pixel());
  Image(const Image<Pixel>&);
  ~Image();

  int width() const;
  int height() const;

  bool isEmpty() const {return _img==0;}
  bool isValid(const int x,const int y) const;

  Image<Pixel>& operator=(const Image<Pixel>&);
  Pixel operator()(const int x, const int y) const;
  Pixel& operator()(const int x, const int y);

  void drawEllipse(const int, const int, const int, const int, const Pixel&);
  void drawLine(const int,const int,const int, const int,const Pixel&);

  bool operator==(const Image<Pixel>&);

 protected:
  void copy(const Image<Pixel>&,bool constructor=false);

 private:
  void drawEllipsePoint(const int,const int,const int,const int,const Pixel&);

  ImageData<Pixel> *_img;

 public:
  class OutOfBoundaryException{};
};

//================================ Implementation =============================

//-----------------------------------------------------------------------------
// Default constructor
//-----------------------------------------------------------------------------
template<class Pixel>
Image<Pixel>::Image(){
  _img = 0;
}

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
template<class Pixel>
Image<Pixel>::Image(const int width, const int height,const Pixel& value){
  _img = new ImageData<Pixel>(width,height,value);
}

//-----------------------------------------------------------------------------
// Copy constructor
//-----------------------------------------------------------------------------
template<class Pixel>
Image<Pixel>::Image(const Image<Pixel>& src){
  _img = 0;
  copy(src,true);
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
template<class Pixel>
Image<Pixel>::~Image(){
  if(_img != 0){
#ifdef __THREAD_SAVE__
    pthread_mutex_lock(&_img->_mutex);
#endif
    if(_img->_refcount > 1){
      _img->_refcount--;
#ifdef __THREAD_SAVE__
      pthread_mutex_unlock(&_img->_mutex);
#endif
    }
    else{
#ifdef __THREAD_SAVE__
      pthread_mutex_unlock(&_img->_mutex);
#endif
      delete _img;
    }
  }
}

//-----------------------------------------------------------------------------
// Image width
//-----------------------------------------------------------------------------
template<class Pixel>
int Image<Pixel>::width() const {
  int result;
  if(_img == 0)
    result = 0;
  else{
#ifdef __THREAD_SAVE__
    pthread_mutex_lock(&_img->_mutex);
#endif
    result = _img->_width;
#ifdef __THREAD_SAVE__
    pthread_mutex_unlock(&_img->_mutex);
#endif
  }
  return result;
}

//-----------------------------------------------------------------------------
// Image height
//-----------------------------------------------------------------------------
template<class Pixel>
int Image<Pixel>::height() const {
  int result;
  if(_img == 0)
    result = 0;
  else{
#ifdef __THREAD_SAVE__
    pthread_mutex_lock(&_img->_mutex);
#endif
    result = _img->_height;
#ifdef __THREAD_SAVE__
    pthread_mutex_unlock(&_img->_mutex);
#endif
  }
  return result;
}

//-----------------------------------------------------------------------------
// Coordinate validity check.
//-----------------------------------------------------------------------------
template<class Pixel>
bool Image<Pixel>::isValid(const int x,const int y) const {
  return (_img && x>=0 && x<width() && y>=0 && y<height());
}

//-----------------------------------------------------------------------------
// Assignment operator
//-----------------------------------------------------------------------------
template<class Pixel>
Image<Pixel>& Image<Pixel>::operator=(const Image<Pixel>& src){
  if(_img != src._img)
    copy(src);
  return *this;
}

//-----------------------------------------------------------------------------
// Read-only access
//-----------------------------------------------------------------------------
template<class Pixel>
Pixel Image<Pixel>::operator()(const int x, const int y) const {
#ifdef __THREAD_SAVE__
  if(_img != 0)
    pthread_mutex_lock(&_img->_mutex);
#endif
  if(_img == 0 || _img->_data == 0 ||
     x<0 || x>=_img->_width ||
     y<0 || y>=_img->_height){
#ifdef __THREAD_SAVE__
    if(_img != 0)
      pthread_mutex_unlock(&_img->_mutex);
#endif
    throw OutOfBoundaryException();
  }
  
  Pixel result = _img->_data[y*_img->_width+x];
#ifdef __THREAD_SAVE__
  pthread_mutex_unlock(&_img->_mutex);
#endif
  return result;
}

//-----------------------------------------------------------------------------
// Read-write access
//-----------------------------------------------------------------------------
template<class Pixel>
Pixel& Image<Pixel>::operator()(const int x, const int y){
#ifdef __THREAD_SAVE__
  if(_img != 0 && _img->_refcount > 1)
    pthread_mutex_lock(&_img->_mutex);
#endif
  if(_img == 0 || _img->_data == 0 ||
     x<0 || x>=_img->_width ||
     y<0 || y>=_img->_height){
#ifdef __THREAD_SAVE__
    if(_img != 0)
      pthread_mutex_unlock(&_img->_mutex);
#endif
    throw OutOfBoundaryException();
  }

  // make an identical copy if necessary
  if(_img->_refcount > 1){
    ImageData<Pixel> *tmp = _img->clone();
    _img->_refcount--;
#ifdef __THREAD_SAVE__
    pthread_mutex_unlock(&_img->_mutex);
#endif
    _img = tmp;
  }

  return _img->_data[y*_img->_width+x];
}

//-----------------------------------------------------------------------------
// draw an ellipse with center (x,y) and radii d1 & d2.
//-----------------------------------------------------------------------------
template<class Pixel>
void Image<Pixel>::drawEllipse(const int x,const int y,const int a, 
			       const int b, const Pixel& color)
{
  if(_img == 0)
    return;

  double q;
  double d1 = a;
  double d2 = b;
  int u=0;
  int v=(int)d2;
  double r=d2*d2-(d1*d1*d2)+0.25*d1*d1;
  drawEllipsePoint(x,y,u,v,color);
  
  // region 1 
  while(d1*d1*(v-0.5)>d2*d2*(u+1.0)){
    if(r < 0.0)
      r += d2*d2*(2.0*u+3.0);
    else{
      r += d2*d2*(2.0*u+3.0)+d1*d1*(-2.0*v+2.0);
      v--;
    }
    u++;
    drawEllipsePoint(x,y,u,v,color);
  }

  // region 2
  q = d2*d2*(u+0.5)*(u+0.5)+d1*d1*(v-1.0)*(v-1.0)-d1*d1*d2*d2;
  while(v > 0){
    if(q < 0.0){
      q += d2*d2*(2.0*u+2.0)+d1*d1*(-2.0*v+3.0);
      u++;
    }
    else
      q += d1*d1*(-2.0*v+3.0);
    v--;
    drawEllipsePoint(x,y,u,v,color);
  }
}

//-----------------------------------------------------------------------------
// Draw line with start point (x0,y0) and end point (x1,y1).
//-----------------------------------------------------------------------------
template<class Pixel>
void Image<Pixel>::drawLine(const int x0,const int y0,
			    const int x1,const int y1,
			    const Pixel& value)
{
  int X0=x0,Y0=y0,X1=x1,Y1=y1;
  int dy = Y1-Y0;
  int dx = X1-X0;
  int G,DeltaG1,DeltaG2;
  int swap;
  int inc = 1;
  int x = X0;
  int y = Y0;
  
  if(x>=0 && x<width() && y>=0 && y<height()) (*this)(x,y) = value;
  
  if(abs(dy) < abs(dx)){
    // Mostly horizontal
    if(dx < 0){
      dx = -dx;
      dy = -dy;
      swap = Y1;
      Y1 = Y0;
      Y0 = swap;
      swap = X1;
      X1 = X0;
      X0 = swap;
    }
    if(dy < 0){
      dy = -dy;
      inc = -1;
    }
    y = Y0;
    x = X0 + 1;
    G = 2 * dy - dx;
    DeltaG1 = 2 * ( dy - dx );
    DeltaG2 = 2 * dy;
    while ( x <= X1 )
      {
        if ( G > 0 )
	  {
            G += DeltaG1;
            y += inc;
            
	  }
        else
	  G += DeltaG2;
	if(x>=0 && x<width() && y>=0 && y<height()) (*this)(x,y) = value;
        x++;
      }
  }
  else
    {
      // Mostly vertical
      if ( dy < 0 )
        {
	  dx = -dx;
	  dy = -dy;
	  swap = Y1;
	  Y1 = Y0;
	  Y0 = swap;
	  swap = X1;
	  X1 = X0;
	  X0 = swap;
        }
      if ( dx < 0 )
        {
	  dx = -dx;
	  inc = -1;
        }
      x = X0;
      y = Y0 + 1;
      G = 2 * dx - dy;
      int minG, maxG;
      minG = maxG = G;
      DeltaG1 = 2 * ( dx - dy );
      DeltaG2 = 2 * dx;
      while ( y <= Y1 )
	{
	  if ( G > 0 )
        {
	  G += DeltaG1;
	  x += inc;
        }
	  else
	    G += DeltaG2;
	  if(x>=0 && x<width() && y>=0 && y<height()) (*this)(x,y) = value;
	  y++;
	}
    }
  
}

//-----------------------------------------------------------------------------
// Comparator. Checks the two objects for equality.
//-----------------------------------------------------------------------------
template<class Pixel>
bool Image<Pixel>::operator==(const Image<Pixel>& other){
  return (_img == other._img);
}

//-----------------------------------------------------------------------------
// Overwrites the object's data with the argument's data.
//-----------------------------------------------------------------------------
template<class Pixel>
void Image<Pixel>::copy(const Image<Pixel>& src, bool constr) {
  if(!constr && _img != 0){
#ifdef __THREAD_SAVE__
    pthread_mutex_lock(&_img->_mutex);
#endif
    if(_img->_refcount > 1){
      _img->_refcount--;
#ifdef __THREAD_SAVE__
      pthread_mutex_unlock(&_img->_mutex);
#endif
    }
    else{
#ifdef __THREAD_SAVE__
      pthread_mutex_unlock(&_img->_mutex);
#endif
      delete _img;
    }
  }

  _img = src._img;
  if(_img != 0){
#ifdef __THREAD_SAVE__
    pthread_mutex_lock(&_img->_mutex);
#endif
    _img->_refcount++;
#ifdef __THREAD_SAVE_
    pthread_mutex_unlock(&_img->_mutex);
#endif
  }
}
 
//-----------------------------------------------------------------------------
// Draw Ellipse Points. Draws four axially symetric points on an ellipse.
//-----------------------------------------------------------------------------
template<class Pixel>
void Image<Pixel>::drawEllipsePoint(const int x,const int y,const int dx,
				    const int dy, const Pixel& color)
{
  int u,v;
  
  u = x+dx; v = y+dy;
  if(u>=0 && u<width() && v>=0 && v<height()) (*this)(u,v) = color;
  u = x+dx; v = y-dy;
  if(u>=0 && u<width() && v>=0 && v<height()) (*this)(u,v) = color;
  u = x-dx; v = y-dy;
  if(u>=0 && u<width() && v>=0 && v<height()) (*this)(u,v) = color;
  u = x-dx; v = y+dy;
  if(u>=0 && u<width() && v>=0 && v<height()) (*this)(u,v) = color;
}

#ifdef _USE_PERSONAL_NAMESPACES
}
#endif

#endif // _SPENGLER_IMAGE_H
