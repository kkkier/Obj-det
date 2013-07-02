/*********************************************************************/
/*                                                                   */
/* FILE         qtclusterview.hh                                     */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      A widget that accepts a vector of images as input    */
/*              and displays them as thumbnails in an IconBox. If    */
/*              the user clicks on any of the images, a set of asso- */
/*              ciated images is displayed in a separate window.     */
/*                                                                   */
/* BEGIN        Wed Mar 12 2003                                      */
/* LAST CHANGE  Wed Mar 12 2003                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_QTCLUSTERVIEW_HH
#define LEIBE_QTCLUSTERVIEW_HH

#ifdef _USE_PERSONAL_NAMESPACES
//namespace Leibe {
#endif

/****************/
/*   Includes   */
/****************/
#include <qiconview.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qpoint.h>

#include <vector>
#include <string>

#include <opgrayimage.hh>
#include "qticonview.hh"


class QtClusterView: public QtIconView
{
  Q_OBJECT
public:
  QtClusterView( QWidget *parent=0, const char *name=0 ); 

  void loadImageSets( vector<OpGrayImage> vImages, string sImgName,
                      vector<OpGrayImage> vAssociatedImgs,
                      vector<int> vAssignment );

public slots:
  void displayAssociatedImgs( int nImgId );


private:
  void displayImgsForBrowsing( vector<OpGrayImage> &vImages,
                               int pos_x = 1150, int pos_y = 200 );

  vector<OpGrayImage> m_vAssociatedImgs;
  vector<int>         m_vAssignment;
};


#ifdef _USE_PERSONAL_NAMESPACES
//}
#endif

#endif
