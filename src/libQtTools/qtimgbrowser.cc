/*********************************************************************/
/*                                                                   */
/* FILE         qtimgbrowser.cc                                      */
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


/****************/
/*   Includes   */
/****************/
#include <cassert>
#include <qapplication.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qstring.h>

#include "qtcoordlabel.hh"
#include "qtimgbrowser.hh"


/*******************/
/*   Definitions   */
/*******************/


/*===================================================================*/
/*                         Class QtImgBrowser                        */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/


QtImgBrowser::QtImgBrowser( QWidget *parent, const char *name, 
                            const QString qsTitle )
  : QWidget( parent, name )
{
  /*******************************/
  /*   create an image display   */
  /*******************************/
  QHBox        *hbImg  = new QHBox( this, "himg");
  rsImg = new QtResizeImg (hbImg, "current image");
    
  /***********************************/
  /*   make a "coordinate display"   */
  /***********************************/
  QHBox        *hbCoord  = new QHBox( this, "hbcoord");
  coordPos = new QtCoordLabel( "Pos:", hbCoord, "coord_pos" );
  
  // display the coordinates when the mouse is moved over the image
  rsImg->setMouseTracking( TRUE );
  connect ( rsImg, SIGNAL(mouseMovedOnPixel(int, int, QRgb)), 
	    coordPos, SLOT(setCoordValue( int, int, QRgb )) );
  connect ( rsImg, SIGNAL(mouseMovedOnPixel(int, int, QRgb, float)), 
	    coordPos, SLOT(setCoordValue( int, int, QRgb, float )) );
  connect ( rsImg, SIGNAL(leftClickOnPixel(int, int)), 
	    this, SLOT(clickOnImage( int, int)) );
  
  // set the cursor shape to a crosshair
  rsImg->setCursor( crossCursor );

  /***********************************/
  /*   create a scrolling facility   */
  /***********************************/
  QHBox  *hbScrollLab  = new QHBox( this, "hbscroll");
  qScrollLabel = new QLabel( " 1/ 1", hbScrollLab );

  QHBox  *hbScroll  = new QHBox( this, "hbscroll");
  //qSlider = new QSlider( Horizontal, hbScroll, "slider" );
  qSlider = new QScrollBar( Horizontal, hbScroll, "slider" );

  /* initialize the slider */
  qSlider->setRange( 1, 1 );
  qSlider->setValue( 1 );
  m_nIndex = 1;
  connect( qSlider, SIGNAL(valueChanged(int)), this, SLOT(display(int)) );

  /****************************/
  /*   do the window layout   */
  /****************************/
  QGridLayout *grid = new QGridLayout( this, 4, 1, 10 );
  grid->addWidget( hbCoord , 0, 0, Qt::AlignHCenter);
  grid->addWidget( hbImg   , 1, 0 );
  grid->addWidget( hbScrollLab, 2, 0, Qt::AlignHCenter);
  grid->addWidget( hbScroll, 3, 0, Qt::AlignTop);
  //grid->setColStretch( 1, 10 );

  /****************************/
  /*   Set the window title   */
  /****************************/
  if( parent == 0 )
    setCaption( qsTitle );

  /***************************************/
  /*   Initialize the global variables   */
  /***************************************/
  m_bImagesLoaded = false;
  m_bGrayImagesLoaded = false;
}

//--- init static member ---//
int QtImgBrowser::verbosity = 1;

void QtImgBrowser::show()
{
	printf("Verbosity: %d\n", verbosity);
	if (verbosity>0) QWidget::show();
}

/***********************************************************/
/*                          Slots                          */
/***********************************************************/

void QtImgBrowser::load( vector<QImage> &vImages, bool bDispCoords )
  /* Load the browser with a vector of Qt images.                    */
{
  assert( vImages.size() > 0 );

  m_vImages = vImages;
  m_vValues.clear();
  qSlider->setRange( 1, vImages.size() );
  qSlider->setValue( 1 );
  m_bImagesLoaded     = true;
  m_bGrayImagesLoaded = false;

  if( !bDispCoords ) {
    disconnect ( rsImg, SIGNAL(mouseMovedOnPixel(int, int, QRgb)) );
    disconnect ( rsImg, SIGNAL(mouseMovedOnPixel(int, int, QRgb, float)) );
  } else {
    disconnect ( rsImg, SIGNAL(mouseMovedOnPixel(int, int, QRgb)) );
    disconnect ( rsImg, SIGNAL(mouseMovedOnPixel(int, int, QRgb, float)) );
    connect    ( rsImg, SIGNAL(mouseMovedOnPixel(int, int, QRgb)), 
                 coordPos, SLOT(setCoordValue( int, int, QRgb )) );
  }

  display(1);
}


void QtImgBrowser::load( vector<QImage> &vImages, 
                         vector<OpGrayImage> &vValues, bool bDispCoords )
  /* Load the browser with a vector of Qt images, and their unscaled */
  /* value images.                                                   */
{
  assert( vImages.size() > 0 );
  assert( vValues.size() == vImages.size() );

  m_vImages = vImages;
  m_vValues = vValues;
  qSlider->setRange( 1, m_vImages.size() );
  qSlider->setValue( 1 );
  m_bImagesLoaded     = true;
  m_bGrayImagesLoaded = true;

  if( !bDispCoords ) {
    disconnect ( rsImg, SIGNAL(mouseMovedOnPixel(int, int, QRgb)) );
    disconnect ( rsImg, SIGNAL(mouseMovedOnPixel(int, int, QRgb,float)) );
  } else {
    disconnect ( rsImg, SIGNAL(mouseMovedOnPixel(int, int, QRgb)) );
    disconnect ( rsImg, SIGNAL(mouseMovedOnPixel(int, int, QRgb,float)) );

    connect    ( rsImg, SIGNAL(mouseMovedOnPixel(int, int, QRgb, float)), 
                 coordPos, SLOT(setCoordValue( int, int, QRgb, float )) );
  }
  display(1);
}


void QtImgBrowser::display( int idx )
{
  if ( m_bImagesLoaded && (idx > 0) && (idx <= m_vImages.size()) ) {
    m_nIndex = idx;

    if ( !m_bGrayImagesLoaded )
      rsImg->loadImage( m_vImages[idx-1] );
    else
      rsImg->loadImage( m_vImages[idx-1], m_vValues[idx-1] );

    rsImg->display();
    qScrollLabel->setText( QString::number(idx) + "/" + 
			   QString::number(m_vImages.size()) );
  }
}


void QtImgBrowser::clickOnImage( int px, int py )
{
  emit imageClicked( m_nIndex-1 );
  emit imageClicked( m_nIndex-1, px, py );
}
