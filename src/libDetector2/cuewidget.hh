/*********************************************************************/
/*                                                                   */
/* FILE         cuewidget.hh                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      GUI for the feature extraction and matching parame-  */
/*              ters of one cue.                                     */
/*                                                                   */
/* BEGIN        Tue Oct 11 2005                                      */
/* LAST CHANGE  Tue Oct 11 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef CUEWIDGET_HH
#define CUEWIDGET_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <qwidget.h>
#include <qstring.h>
#include <qimage.h>
#include <qradiobutton.h>
#include <featuregui.hh>
#include <matchinggui.hh>

/*******************/
/*   Definitions   */
/*******************/

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                          Class CueWidget                          */
/*===================================================================*/

class CueWidget: public QWidget
{
  Q_OBJECT
public:
  CueWidget( FeatureGUI* &guiFeatures, MatchingGUI* &guiMatching,
             QWidget *parent=0, const char *name=0 );

signals:
  void sigCuesChanged              ();

 public slots:
 void propagateCueUpdate( const QString &text ) { emit sigCuesChanged(); }
};

#endif
