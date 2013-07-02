/*********************************************************************/
/*                                                                   */
/* FILE         qticonview.cc                                        */
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


/****************/
/*   Includes   */
/****************/
#include <qpainter.h>
#include <math.h>

#include "qticonview.hh"

/*===================================================================*/
/*                         Class QtIconView                          */
/*===================================================================*/


QtIconView::QtIconView( QWidget *parent, const char *name ) 
  : QIconView(parent,name) 
{  
  clear();
  setResizeMode( QIconView::Adjust );
  setAutoArrange( true );
  setSorting( true );

  connect( this, SIGNAL(mouseButtonClicked(int,QIconViewItem*,const QPoint&)), 
           this, SLOT(mouseClickOnItem(int,QIconViewItem*,const QPoint&)) );  
}


void QtIconView::mouseClickOnItem( int button, QIconViewItem *item, 
                                   const QPoint &pos )
{
  if( item != 0 ) {
    int imgNo = item->key().toInt();
    
    switch(button) {
    case LeftButton:
      emit( leftClickOnImage( imgNo ) );
      break;

    case RightButton:
      emit( rightClickOnImage( imgNo ) );
      break;

    case MidButton:
      emit( middleClickOnImage( imgNo ) );
      break;

    default:
      break;
    }
  }
}


void QtIconView::loadImageSets( vector<QImage> vImages )
  /* load image sets and display them in the qIconView box. */
{
  if ( vImages.empty() ) {
    cerr << "  Error in QtIconView::loadImageSets(): " 
          << endl;
    return;
  }

  /************************************/
  /*   Initialize the qIconView box   */
  /************************************/
  clear(); //m_qIconView->clear();
  
  /* set the cursor to an hourglass */
  setCursor( waitCursor );

  /*******************************************/
  /*   Display a thumbnail for every image   */
  /*******************************************/
    
  for(int i=0; i < vImages.size(); i++) {
    QString sKey = QString::number(i);
    if ( i < 10000 ) sKey.insert(0, '0');
    if ( i <  1000 ) sKey.insert(0, '0');
    if ( i <   100 ) sKey.insert(0, '0');
    if ( i <    10 ) sKey.insert(0, '0');
     
    addImageThumbnail( vImages[i], sKey.latin1() );
  }

  /* reset the cursor back to normal */
  setCursor( arrowCursor );
}


void QtIconView::loadImageSets( vector<QImage> vImages, 
                                vector<string> vImgNames )
  /* load image sets and display them in the qIconView box. */
{
  if ( vImages.empty() || vImgNames.empty() || 
       (vImages.size()!=vImgNames.size()) ) {
    cerr << "  Error in QtIconView::loadImageSets(): " 
         << "vector dimensions must agree!" << endl;
    return;
  }

  /************************************/
  /*   Initialize the qIconView box   */
  /************************************/
  clear(); //m_qIconView->clear();
  
  /* set the cursor to an hourglass */
  setCursor( waitCursor );

  /*******************************************/
  /*   Display a thumbnail for every image   */
  /*******************************************/
    
  for(int i=0; i < vImages.size(); i++) {
    QString sKey = QString::number(i);
    if ( i < 10000 ) sKey.insert(0, '0');
    if ( i <  1000 ) sKey.insert(0, '0');
    if ( i <   100 ) sKey.insert(0, '0');
    if ( i <    10 ) sKey.insert(0, '0');
     
    addImageThumbnail( vImages[i], vImgNames[i], sKey.latin1() );
  }

  /* reset the cursor back to normal */
  setCursor( arrowCursor );
}


void QtIconView::loadImageSets( vector<QPixmap> vImages, 
                                vector<string> vImgNames )
  /* load image sets and display them in the qIconView box. */
{
  if ( vImages.empty() || vImgNames.empty() || 
       (vImages.size()!=vImgNames.size()) ) {
    cerr << "  Error in QtIconView::loadImageSets(): " 
         << "vector dimensions must agree!" << endl;
    return;
  }

  /************************************/
  /*   Initialize the qIconView box   */
  /************************************/
  clear(); //m_qIconView->clear();
  
  /* set the cursor to an hourglass */
  setCursor( waitCursor );

  /*******************************************/
  /*   Display a thumbnail for every image   */
  /*******************************************/
    
  for(int i=0; i < vImages.size(); i++) {
    QString sKey = QString::number(i);
    if ( i < 10000 ) sKey.insert(0, '0');
    if ( i <  1000 ) sKey.insert(0, '0');
    if ( i <   100 ) sKey.insert(0, '0');
    if ( i <    10 ) sKey.insert(0, '0');
     
    addImageThumbnail( vImages[i], vImgNames[i], sKey.latin1() );
  }

  /* reset the cursor back to normal */
  setCursor( arrowCursor );
}


void QtIconView::loadImageSets( vector<OpGrayImage> vImages, 
                                vector<string> vImgNames )
  /* load image sets and display them in the qIconView box. */
{
  if ( vImages.empty() || vImgNames.empty() || 
       (vImages.size()!=vImgNames.size()) ) {
    cerr << "  Error in QtIconView::loadImageSets(): " 
         << "vector dimensions must agree!" << endl;
    return;
  }

  /************************************/
  /*   Initialize the qIconView box   */
  /************************************/
  clear(); //m_qIconView->clear();
  
  /* set the cursor to an hourglass */
  setCursor( waitCursor );

  /*******************************************/
  /*   Display a thumbnail for every image   */
  /*******************************************/
    
  for(int i=0; i < vImages.size(); i++) {
    QString sKey = QString::number(i);
    if ( i < 10000 ) sKey.insert(0, '0');
    if ( i <  1000 ) sKey.insert(0, '0');
    if ( i <   100 ) sKey.insert(0, '0');
    if ( i <    10 ) sKey.insert(0, '0');
     
    addImageThumbnail( vImages[i], vImgNames[i], sKey.latin1() );
  }

  /* reset the cursor back to normal */
  setCursor( arrowCursor );
}


void QtIconView::addImageThumbnail( OpGrayImage img, string sText, 
                                    string sKey )
  /* load an image, convert it to a thumbnail, and add it to the box */
{
  /* load the image into a QImage structure */
  QImage qimg = img.getQtImage();
  
  /* Create the thumbnail image */
  QPixmap pmThumb = createImagePixmap( qimg, 50 );
  QIconViewItem *newItem = new QIconViewItem( this, sText.c_str(), pmThumb );
  newItem->setKey( sKey.c_str() );
}


void QtIconView::addImageThumbnail( QImage img, 
                                    string sKey )
  /* load an image, convert it to a thumbnail, and add it to the box */
{
  /* Create the thumbnail image */
  QPixmap pmThumb = createImagePixmap( img, 50 );
  QIconViewItem *newItem = new QIconViewItem( this, "", pmThumb );
  newItem->setKey( sKey.c_str() );
}

void QtIconView::addImageThumbnail( QImage img, string sText, 
                                    string sKey )
  /* load an image, convert it to a thumbnail, and add it to the box */
{
  /* Create the thumbnail image */
  QPixmap pmThumb = createImagePixmap( img, 50 );
  QIconViewItem *newItem = new QIconViewItem( this, sText.c_str(), pmThumb );
  newItem->setKey( sKey.c_str() );
}


void QtIconView::addImageThumbnail( QPixmap img, string sText, 
                                    string sKey )
  /* load an image, convert it to a thumbnail, and add it to the box */
{
  /* Create the thumbnail image */
  QPixmap pmThumb = createImagePixmap( img, 50 );
  QIconViewItem *newItem = new QIconViewItem( this, sText.c_str(), pmThumb );
  newItem->setKey( sKey.c_str() );
}


QPixmap QtIconView::createImagePixmap( QImage &img, int MinSize )
{
  /* determine the right scale factors */
  float fScaleX = 1.0;
  float fScaleY = 1.0;
  if ( img.width() < MinSize )
    fScaleX = MinSize / ((float) img.width());
  if ( img.height() < MinSize )
    fScaleY = MinSize / ((float) img.height());

  /* create a pixmap with this size */
  int nNewSizeX = (int) fScaleX*img.width();
  int nNewSizeY = (int) fScaleY*img.height();
  QPixmap  pmScaled( nNewSizeX, nNewSizeY );
  QPainter p( &pmScaled );
  
  /* copy the pixels into the new pixmap */
  float fIncX = 1.0 / fScaleX;
  float fIncY = 1.0 / fScaleY;
  float fCountY = 0.0;
  for( int y=0; y<nNewSizeY; y++ ) {
    int idx_y = (int) floor( fCountY );

    float fCountX = 0.0;
    for( int x=0; x<nNewSizeX; x++ ) {
      int idx_x = (int) floor( fCountX );
      
      p.setPen( QColor(img.pixel(idx_x, idx_y)) );
      p.drawPoint( x, y );

      fCountX += fIncX;
    }

    fCountY += fIncY;
  }
  p.end();

  return pmScaled;
}


QPixmap QtIconView::createImagePixmap( QPixmap &pm, int MinSize )
{
  /* convert the qpixmap to a qimage */
  QImage img = pm.convertToImage();

  /* determine the right scale factors */
  float fScaleX = 1.0;
  float fScaleY = 1.0;
  if ( img.width() < MinSize )
    fScaleX = MinSize / ((float) img.width());
  if ( img.height() < MinSize )
    fScaleY = MinSize / ((float) img.height());

  /* create a pixmap with this size */
  int nNewSizeX = (int) fScaleX*img.width();
  int nNewSizeY = (int) fScaleY*img.height();
  QPixmap  pmScaled( nNewSizeX, nNewSizeY );
  QPainter p( &pmScaled );
  
  /* copy the pixels into the new pixmap */
  float fIncX = 1.0 / fScaleX;
  float fIncY = 1.0 / fScaleY;
  float fCountY = 0.0;
  for( int y=0; y<nNewSizeY; y++ ) {
    int idx_y = (int) floor( fCountY );

    float fCountX = 0.0;
    for( int x=0; x<nNewSizeX; x++ ) {
      int idx_x = (int) floor( fCountX );
      
      p.setPen( QColor(img.pixel(idx_x, idx_y)) );
      p.drawPoint( x, y );

      fCountX += fIncX;
    }

    fCountY += fIncY;
  }
  p.end();

  return pmScaled;
}

