/*********************************************************************/
/*                                                                   */
/* FILE         qtclusterview.cc                                     */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      A widget that accepts a vector of images as input    */
/*              and displays them as thumbnails in an IconBox. If    */
/*              the user clicks on any of the thumbnailss, a set of  */
/*              associated images is displayed in a separate window. */
/*                                                                   */
/* BEGIN        Wed Mar 12 2003                                      */
/* LAST CHANGE  Wed Mar 12 2003                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include "qtimgbrowser.hh"

#include "qtclusterview.hh"

/*===================================================================*/
/*                       Class QtClusterView                         */
/*===================================================================*/

QtClusterView::QtClusterView( QWidget *parent, const char *name ) 
  : QtIconView(parent,name) 
{  
  /* make the connection for displaying associated images */
  connect( this, SIGNAL(leftClickOnImage(int)), 
           this, SLOT(displayAssociatedImgs(int)) );
}


void QtClusterView::loadImageSets( vector<OpGrayImage> vImages, 
                                   string sImgName,
                                   vector<OpGrayImage> vAssociatedImgs,
                                   vector<int> vAssignment )
{
  if( vImages.empty() || (vAssociatedImgs.size() != vAssignment.size()) ) {
    cerr << "  Error in QtClusterView::loadImageSets(): " 
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
    if ( i < 100 ) sKey.insert(0, '0');
    if ( i <  10 ) sKey.insert(0, '0');
    int belong = 0;

    for(int j=0; j < vAssignment.size(); j++) {
      if ( vAssignment[j] == i ) {
        ++belong;
      }
    }  
    QString sImgstr = QString::number( belong );
    string imgstr( sImgstr.latin1() );
    imgstr.insert(0, "\n");
    imgstr.insert(0, sKey.latin1());
    imgstr.insert(0, "_");
    imgstr.insert(0, sImgName);
     
    addImageThumbnail( vImages[i], imgstr, sKey.latin1() );
  }

  /* copy the associated images */
  m_vAssociatedImgs = vAssociatedImgs;
  m_vAssignment     = vAssignment;

  /* reset the cursor back to normal */
  setCursor( arrowCursor );
}


void QtClusterView::displayAssociatedImgs( int nImgId )
  /* Display the corresponding images belonging to the cluster image */
  /* just clicked.                                                   */
{
  if( !m_vAssociatedImgs.empty() && !m_vAssignment.empty() ) {
    vector<OpGrayImage> vCorresponding;

    for ( int i=0; i < m_vAssignment.size(); i++ )
      if ( m_vAssignment[i] == nImgId )
        vCorresponding.push_back( m_vAssociatedImgs[i] );
 
    displayImgsForBrowsing( vCorresponding );
  }
  
}


void QtClusterView::displayImgsForBrowsing( vector<OpGrayImage> &vImages,
                                            int pos_x, int pos_y )
  /* display images in a separate window */
{
  if ( vImages.empty() ) 
    return;

  /* convert into QImages */
  vector<QImage> vQImages;
  for (int i=0; i < vImages.size(); i++) {
    vQImages.push_back( vImages[i].getQtImage() );
  }

  /* display the images in a browser window */
  QtImgBrowser *qtBrowser = new QtImgBrowser( 0, "images" );
  qtBrowser->setGeometry( pos_x, pos_y, 100, 150 );
  qtBrowser->load( vQImages );
  qtBrowser->show();
}




