/*********************************************************************/
/*                                                                   */
/* FILE         sharedcontainer.hh                                   */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@vision.ee.ethz.ch                              */
/*                                                                   */
/* CONTENT      A Wrapper class for containers that allows applica-  */
/*              tions to share the same container for multiple in-   */
/*              dependent detectors.                                 */
/*                                                                   */
/* BEGIN        Thu May 18 2006                                      */
/* LAST CHANGE  Thu May 18 2006                                      */
/*                                                                   */
/*********************************************************************/

#ifndef SHAREDCONTAINER_HH
#define SHAREDCONTAINER_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>
#include <map>

/*******************/
/*   Definitions   */
/*******************/


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                      Class SharedContainer                        */
/*===================================================================*/

template<class T> class SharedContainer
{
public:
  SharedContainer();
  SharedContainer( const SharedContainer<T>& other );
  ~SharedContainer();

  SharedContainer& operator=( const SharedContainer<T> &other );

protected:
  void copyFrom( const SharedContainer<T> &other, 
                 bool bConstructor=false );

public:
  /******************/
  /* Content Access */
  /******************/
  inline T& operator->()
  { assert( _content ); return *_content; }

  inline T& content()
  { assert( _content ); return *_content; }

  void create();

  //SharedContainer<T> duplicate();

protected:
  T* _content;
};


/****************************/
/*   Class Implementation   */
/****************************/
/*---------------------------------------------------------*/
/*                       Constructors                      */
/*---------------------------------------------------------*/

template <class T> 
SharedContainer<T>::SharedContainer()
  /* standard constructor */
{
  _content = NULL;
}


template <class T> 
SharedContainer<T>::SharedContainer( const SharedContainer<T> &other )
  /* copy constructor */
{
  _content = NULL;
  copyFrom( other, true );
}


template <class T> 
SharedContainer<T>::~SharedContainer()
  /* destructor */
{
  if( _content != NULL )
    if( _content->_refcount > 1 )
      _content->_refcount--;
    else
      delete _content;
}


template <class T> 
SharedContainer<T>& SharedContainer<T>::operator=( const SharedContainer<T> &other )
{
  copyFrom( other );
  return *this;
}


template <class T> 
void SharedContainer<T>::copyFrom( const SharedContainer<T> &other, 
                                   bool bConstructor )
{
  if( !bConstructor && _content!=NULL ){
    // remove the old Container object
    if( _content->_refcount>1 )
      _content->_refcount--;
    else
      delete _content;
  }

  // copy the new object
  _content = other._content;
  if( _content!=NULL )
    _content->_refcount++;
}


/*---------------------------------------------------------*/
/*                     Content Access                      */
/*---------------------------------------------------------*/

template <class T> 
void SharedContainer<T>::create()
{
  // remove the old content...
  if( _content != NULL )
    if( _content->_refcount > 1 )
      _content->_refcount--;
    else
      delete _content;
  
  // ...and create a new one
  _content = new T();
//   if( _content!=NULL )          // don't attach here!
//     _content->_refcount++;      // (-> already done by constructor)
}


// template <class T=Container> 
// SharedContainer<T> SharedContainer<T>::duplicate()
//   /* Make a deep copy of the container and return it to the caller. */
// {
//   SharedContainer<T> res;
//   res._content = new T( *_content );
// }

#endif
