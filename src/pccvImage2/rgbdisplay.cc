/******************************************************************************
 * $Id: rgbdisplay.cc,v 1.1.1.1 2004/09/08 13:50:20 mfritz Exp $
 * $Author: mfritz $
 * $Date: 2004/09/08 13:50:20 $
 *------------------------------ Description ----------------------------------
 * Source code for librgbdisp, a library which provides means for displaying 
 * images of type Image<RGBPixel> in a simple and convenient manner. A 
 * RGBDisplay object encapsulates a window in which an image can be displayed.
 * For details see 'rgbdisplay.h'.
 *----------------------------- Requirements ----------------------------------
 * libqt-mt.so.2.2.0 - the Qt toolkit library version 2.2.0 (multithreaded)
 * libpthread.so  - the POSIX mutlithread library
 *------------------------------- CVS logs ------------------------------------
 * $Log: rgbdisplay.cc,v $
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.2  2004/04/26 15:16:33  leibe
 *
 * reinstated the original version of the library 'pccvImage'.
 *
 * Revision 1.4  2002/04/02 14:24:23  spengler
 * Two novelties for class RGBDisplay: 1. In analogy to RGBDisplay::
 * wait() there is now a class method RGBDisplay::pause() which pauses
 * the calling application until the user hits a key. 2. Each instance of
 * class RGBDisplay may now have a probe handler, i.e. a pointer to an
 * object of type RGBDisplay::QtEventFilter (or derived). Whenever the
 * user clicks into the displayed RGBImage, the function operator of the
 * handler will be called with the selected position (x,y). Usefull for
 * probing mechanisms.
 *
 * Revision 1.3  2001/12/10 14:10:37  spengler
 * RGBDisplay has now the capability of displaying captions for the indi-
 * vidual screens. Captions can either be set at creation time of instances
 * of type 'RGBDisplay' or later by calling the new method setCaption()
 * which takes a array of char as argument.
 *
 * Revision 1.2  2001/08/20 15:44:06  spengler
 * Version 1 of image class and related utility classes and libraries
 * finished. Ready for distribution
 *
 *****************************************************************************/

#ifdef _USE_PERSONAL_NAMESPACES
namespace Spengler {
#endif 

#include "rgbdisplay.h"

#include <iostream>
#include <unistd.h>
#include <qwidcoll.h>

/******************************************************************************
 *                               class RGBDisplay                             *
 *****************************************************************************/

  // Initialization of two class variables (static class members).
pthread_t RGBDisplay::thread = (pthread_t)0;
pthread_mutex_t RGBDisplay::mutex = PTHREAD_MUTEX_INITIALIZER;
bool RGBDisplay::_pausing = false;
char RGBDisplay::_key = 0;

//-----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------
RGBDisplay::RGBDisplay(){
  // check whether there is already a QApplication instance
  if(RGBDisplay::thread == 0)
    pthread_create(&RGBDisplay::thread,0,RGBDisplay::guithread,0);
  
  widget = 0;
  _handler = 0;
}

//-----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------
RGBDisplay::RGBDisplay(const RGBImage& image,const char* caption){
  // check whether there is already a QApplication instance
  if(RGBDisplay::thread == 0)
    pthread_create(&RGBDisplay::thread,0,RGBDisplay::guithread,0);

   widget = 0;
  _handler = 0;

  display(image,caption);
}
   
//-----------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------
RGBDisplay::~RGBDisplay(){
  if(widget != 0 && !qApp->closingDown()){
    qApp->lock();
    QWidgetList *list = QApplication::topLevelWidgets();
    if(list != 0){
      if(list->find(widget) != -1)
	widget->close();
      delete list;
    }
    qApp->unlock();
  }
}

//-----------------------------------------------------------------------------
// Displays a new image in the current RGBDisplay instance.
//-----------------------------------------------------------------------------
void RGBDisplay::display(const RGBImage& image,const char* caption){
  // create new RGBWidget
  while(qApp->startingUp());
  qApp->lock();
  if(widget == 0){
    widget = new RGBImageWidget(image);
    if(caption != 0)
      _caption = QString(caption);
    widget->setCaption(_caption);
    widget->setHandler(_handler);
    widget->show();
  }
  else{
    if(caption != 0)
      _caption = QString(caption);
    widget->display(image);
    widget->setCaption(_caption);
  }
  qApp->unlock();
}

//-----------------------------------------------------------------------------
// Sets a new caption for the display window.
//-----------------------------------------------------------------------------
void RGBDisplay::setCaption(const char* caption){
  _caption = QString(caption);
  qApp->lock();
  if(widget != 0)
    widget->setCaption(_caption);
  qApp->unlock();
}

//-----------------------------------------------------------------------------
// Sets a new pixel selection handler.
//-----------------------------------------------------------------------------
RGBImageWidget::Handler* RGBDisplay::setHandler(RGBImageWidget::Handler *h){
  RGBImageWidget::Handler *result = _handler;
  _handler = h;

  if(widget != 0){
    qApp->lock();
    result = widget->setHandler(_handler);
    qApp->unlock();
  }

  return result;
}    
    
//-----------------------------------------------------------------------------
// Waits for a key stroke. If no keys/characters are specified as argument,
// this function will return when an arbitrary keystroke has been detected.
//-----------------------------------------------------------------------------
char RGBDisplay::pause(const char *accept){
  QtEventFilter filter(&_pausing,&_key,accept);
  _key = 0;
  _pausing = true;
  qApp->lock();
  qApp->installEventFilter(&filter);
  qApp->unlock();

  while(_pausing);

  qApp->lock();
  qApp->removeEventFilter(&filter);
  qApp->unlock();

  return _key;
}

//-----------------------------------------------------------------------------
// Waits until the last window/RGBDisplay instance is closed. [static]
//-----------------------------------------------------------------------------
void RGBDisplay::wait(){
  pthread_join(RGBDisplay::thread,0);
}

//-----------------------------------------------------------------------------
// GUI/Qt thread which contains the QApplication instance. [static]
//-----------------------------------------------------------------------------
void *RGBDisplay::guithread(void *){
  int argc = 0; char **argv = 0;

  QApplication app(argc,argv);
  QObject::connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
  app.exec();
  return 0;
}

/******************************************************************************
 *                         class RGBDisplay::QtEventFilter                    *
 *****************************************************************************/

//-----------------------------------------------------------------------------
// Constructor.
//-----------------------------------------------------------------------------
RGBDisplay::QtEventFilter::QtEventFilter(bool *pausing,char *key, 
					 const char *accept)
{
  _pausing = pausing;
  _key = key;
  _accept = QString(accept);
}

//-----------------------------------------------------------------------------
// Qt Event Filter.
//-----------------------------------------------------------------------------
bool RGBDisplay::QtEventFilter::eventFilter(QObject *o,QEvent *e){
  if(e->type() == QEvent::KeyPress){
    QKeyEvent *k = (QKeyEvent *)e;
    int idx;
    if(_accept.isEmpty()){
      *_key = 0;
      *_pausing = false;
      return TRUE;
    }
    else if((idx = _accept.find(k->key())) > -1){
      *_key = (char)QChar(_accept[idx]);
      *_pausing = false;
      return TRUE;
    }
  }
  return QObject::eventFilter(o,e);
}


#ifdef _USE_PERSONAL_NAMESPACES
}
#endif


