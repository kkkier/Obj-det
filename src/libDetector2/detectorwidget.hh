/*********************************************************************/
/*                                                                   */
/* FILE         detectorwidget.hh                                    */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@vision.ee.ethz.ch                              */
/*                                                                   */
/* CONTENT      GUI for the detector and its recognition parameters. */
/*                                                                   */
/* BEGIN        Fri Feb 10 2006                                      */
/* LAST CHANGE  Fri Feb 10 2006                                      */
/*                                                                   */
/*********************************************************************/

#ifndef DETECTORWIDGET_HH
#define DETECTORWIDGET_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <qwidget.h>
#include <qstring.h>
#include <qimage.h>
#include <qradiobutton.h>
#include <recogui.hh>

#include "detectorgui.hh"

/*******************/
/*   Definitions   */
/*******************/

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                       Class DetectorWidget                        */
/*===================================================================*/

class DetectorWidget: public QWidget
{
  Q_OBJECT
public:
  DetectorWidget( DetectorGUI* &guiDetect, RecoGUI* &guiReco,
                  QWidget *parent=0, const char *name=0 );

signals:
  void sigCuesChanged    ();
  void sigDetectorChanged();

  void sigLoadDetectorClicked();
  void sigSaveDetectorClicked();
  void sigClearDetectorClicked();

public slots:
  void propagateCueUpdate     (const QString&) { emit sigCuesChanged(); }
  void propagateDetectorUpdate(const QString&) { emit sigDetectorChanged(); }

  void hideEvent (QHideEvent *e);
  void closeEvent(QCloseEvent *e);

  void loadDetector();
  void saveDetector();
  void clearDetector();

public:
  DetectorGUI *m_guiDetect;
  RecoGUI     *m_guiReco;
};

#endif
