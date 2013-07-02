/******************************************************************************
 * $Id: rgbdisplay.h,v 1.1.1.1 2004/09/08 13:50:20 mfritz Exp $
 * $Author: mfritz $
 * $Date: 2004/09/08 13:50:20 $
 *------------------------------ Description ----------------------------------
 * Header for librgbdisp, a library which provides means for displaying 
 * images of type Image<RGBPixel> in a simple and convenient manner. A 
 * RGBDisplay object encapsulates a window in which an image can be displayed.
 * Two ways of displaying an RGBImage instance exist:
 * 1. Creation of a new RGBDisplay instance which requires a reference to a 
 *    RGBImage. If every thing is o.k., a window of the image's size pops up
 *    and displays the image.
 * 2. Calling the method 'display' of an already existing RGBDisplay object
 *    replaces the image data of the instance with the data of the new image
 *    and resizes the window if necessary. 
 * Calling the additional static class method RGBDisplay::wait() waits until
 * the last of the RGBDisplay windows is closed and returns afterwards. 
 * Explicit destruction of an RGBDisplay instance which was created with the
 * 'new' operator will close its corresponding window.
 *----------------------------- Requirements ----------------------------------
 * libqt-mt.so.2.2.0 - the Qt toolkit library, version 2.2.0 (multithreaded)
 *                     compile applications with the QT_THREAD_SUPPORT flag!
 * libpthread.so     - the POSIX mutlithread library
 *------------------------------- CVS logs ------------------------------------
 * $Log: rgbdisplay.h,v $
 * Revision 1.1.1.1  2004/09/08 13:50:20  mfritz
 * initial import from bastian's code by mario typed by eDi under supervision of Bernt
 *
 * Revision 1.2  2004/04/26 15:16:33  leibe
 *
 * reinstated the original version of the library 'pccvImage'.
 *
 * Revision 1.4  2002/04/02 14:24:24  spengler
 * Two novelties for class RGBDisplay: 1. In analogy to RGBDisplay::
 * wait() there is now a class method RGBDisplay::pause() which pauses
 * the calling application until the user hits a key. 2. Each instance of
 * class RGBDisplay may now have a probe handler, i.e. a pointer to an
 * object of type RGBDisplay::QtEventFilter (or derived). Whenever the
 * user clicks into the displayed RGBImage, the function operator of the
 * handler will be called with the selected position (x,y). Usefull for
 * probing mechanisms.
 *
 * Revision 1.3  2001/12/10 14:10:38  spengler
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

#ifndef _SPENGLER_RGB_DISPLAY_H
#define _SPENGLER_RGB_DISPLAY_H

#ifdef _USE_PERSONAL_NAMESPACES
namespace Spengler {
#endif 

#include <pthread.h>
#include <qapplication.h>
#include <qstring.h>
#include <qobject.h>

#include <rgbwidget.h>
#include <rgbimage.h>

/******************************************************************************
 *                               class RGBDisplay                             *
 *****************************************************************************/
class RGBDisplay
{
  class QtEventFilter;

 public:
  RGBDisplay();
  RGBDisplay(const RGBImage&,const char* = 0);
  ~RGBDisplay();

  void display(const RGBImage&,const char* = 0);
  void setCaption(const char*);
  RGBImageWidget::Handler* setHandler(RGBImageWidget::Handler*);

  static char pause(const char * =0);
  static void wait();

 private:
  static void *guithread(void *);
  static pthread_t thread;
  static pthread_mutex_t mutex;
  
  static bool _pausing;
  static char _key;

  RGBImageWidget *widget;
  QString _caption;
  RGBImageWidget::Handler *_handler;
};

/******************************************************************************
 *                         class RGBDisplay::QtEventFilter                    *
 *****************************************************************************/
class RGBDisplay::QtEventFilter : public QObject
{
public:
  QtEventFilter(bool *,char *, const char *);

protected:
  bool eventFilter(QObject *,QEvent *);

private:
  bool *_pausing;
  char *_key;
  QString _accept;
}; 


#ifdef _USE_PERSONAL_NAMESPACES
}
#endif

#endif // _SPENGLER_RGB_DISPLAY_H
