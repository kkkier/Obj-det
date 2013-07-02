/*********************************************************************/
/*                                                                   */
/* FILE         qtimgbrowser.hh                                      */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      A widget that accepts a vector of images as input    */
/*              displays one, together with controls to browse       */
/*              through them.                                        */
/*                                                                   */
/* BEGIN        Fri Nov 16 2001                                      */
/* LAST CHANGE  Fri Nov 16 2001                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_QTIMGBROWSER_HH
#define LEIBE_QTIMGBROWSER_HH

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif

/****************/
/*   Includes   */
/****************/
#include <qwidget.h>
#include <qlabel.h>
#include <qslider.h>
#include <qscrollbar.h>
#include <qimage.h>
#include <qstring.h>

#include <vector>

#include "qtresizeimg.hh"
#include "qtcoordlabel.hh"
#include <opgrayimage.hh>

using namespace std;

/*******************/
/*   Definitions   */
/*******************/



class QtImgBrowser: public QWidget
{
  Q_OBJECT
public:
  QtImgBrowser( QWidget *parent=0, const char *name=0, 
                const QString qsTitle="" );

  virtual void show();
  
public slots:
  void load( vector<QImage> &vImages, bool bDispCoords=false );
  void load( vector<QImage> &vImages, vector<OpGrayImage> &vValues, 
             bool bDispCoords=true );

  void display( int idx );
  void clickOnImage( int px, int py );

signals:
  void imageClicked( int idx );
  void imageClicked( int idx, int x, int y );

public:
  static int verbosity;
  
private:
  QtResizeImg  *rsImg;
  QtCoordLabel *coordPos;
  QLabel       *qScrollLabel;
  QScrollBar   *qSlider;

  vector<QImage>      m_vImages;
  vector<OpGrayImage> m_vValues;

  bool       m_bImagesLoaded;
  bool       m_bGrayImagesLoaded;
  int        m_nIndex;
};

#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif

#endif
