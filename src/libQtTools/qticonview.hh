/*********************************************************************/
/*                                                                   */
/* FILE         qticonview.hh                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      A widget that accepts a vector of images as input    */
/*              and displays them as thumbnails in an IconBox.       */
/*                                                                   */
/* BEGIN        Wed Mar 12 2003                                      */
/* LAST CHANGE  Wed Mar 12 2003                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_QTICONVIEW_HH
#define LEIBE_QTICONVIEW_HH

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif

/****************/
/*   Includes   */
/****************/
#ifdef null
#undef null
#endif

#include <qiconview.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qpoint.h>

#include <vector>
#include <string>
#include <iostream>

#include <opgrayimage.hh>

using namespace std;

class QtIconView: public QIconView
{
  Q_OBJECT
public:
  QtIconView( QWidget *parent=0, const char *name=0 ); 

  void loadImageSets( vector<QImage>      vImages );
  void loadImageSets( vector<QImage>      vImages, vector<string> vImgNames );
  void loadImageSets( vector<QPixmap>     vImages, vector<string> vImgNames );
  void loadImageSets( vector<OpGrayImage> vImages, vector<string> vImgNames );

public slots:
  void addImageThumbnail( QImage      img, string sKey );
  void addImageThumbnail( QImage      img, string sText, string sKey );
  void addImageThumbnail( QPixmap     img, string sText, string sKey );
  void addImageThumbnail( OpGrayImage img, string sText, string sKey );

signals:
  void leftClickOnImage  ( int nImgId );
  void rightClickOnImage ( int nImgId );
  void middleClickOnImage( int nImgId );

private slots:
  void mouseClickOnItem( int button, QIconViewItem* item, const QPoint &pos );

private:
  QPixmap createImagePixmap( QImage  &img, int MinSize = 50 );
  QPixmap createImagePixmap( QPixmap &img, int MinSize = 50 );
};


#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif

#endif
