/*********************************************************************/
/*                                                                   */
/* FILE         clustergui.hh                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      GUI for the clustering parameters.                   */
/*                                                                   */
/* BEGIN        Wed Mar 16 2005                                      */
/* LAST CHANGE  Wed Mar 16 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef CLUSTERGUI_HH
#define CLUSTERGUI_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <qwidget.h>
#include <qstring.h>
#include <qimage.h>
#include <qradiobutton.h>
#include <qapplication.h>

/*******************/
/*   Definitions   */
/*******************/
const int CLUSTER_KMEANS        = 0;
const int CLUSTER_POSTAGGLO     = 1;
const int CLUSTER_RNNCAGGLO     = 2;
const int CLUSTER_FASTRNNCAGGLO = 3;
const int CLUSTER_RANDOMFOREST  = 4;

const int RECONST_COMPRATIO     = 0;
const int RECONST_SIMLEVEL      = 1;

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                           Class ClusterGUI                        */
/*===================================================================*/

class ClusterGUI: public QWidget
{
  Q_OBJECT
public:
  ClusterGUI( QWidget *parent=0, const char *name=0 );
  
public slots:
  /**************************/
  /*   Interface Handlers   */
  /**************************/
  void slotSetEnergy            ( const QString &text );
  void slotSetNumClusters       ( const QString &text );
  void slotSetEps               ( const QString &text );
  void slotSetMaxIter           ( const QString &text );
  void slotSetSimilarity        ( const QString &text );
  void slotSetFeatureSimFact    ( const QString &text );
  void slotSetMaxNodeSize       ( const QString &text );
  void slotSetCompressionRatio  ( const QString &text );
  void slotSetSimLevel          ( const QString &text );
  void slotUpdateEnergy();
  void slotUpdateNumClusters();
  void slotUpdateEps();
  void slotUpdateMaxIter();
  void slotUpdateSimilarity();
  void slotUpdateFeatureSimFact();
  void slotUpdateMaxNodeSize();
  void slotUpdateCompressionRatio();
  void slotUpdateSimLevel();

  void slotSelectClusterMethod   ( int   id );
  void slotSelectReconstMethod   ( int   id );

  void slotSetPCAOnOff           ( int   state );

  void slotSetTreeNum		(const QString &text);
  void slotSetTreeDepth		(const QString &text);
  void slotSetWeekClassifier	(const QString &text);
  void slotSetMinSample		(const QString &text);
 
  void slotUpdateTreeNum();
  void slotUpdateTreeDepth();
  void slotUpdateWeekClassifier();
  void slotUpdateMinSample();

  

signals:
  /**************************/
  /*   Interface Handlers   */
  /**************************/
  void sigNumClustersChanged       ( const QString& );
  void sigEpsChanged               ( const QString& );
  void sigMaxIterChanged           ( const QString& );
  void sigVarianceChanged          ( const QString& );
  void sigSimilarityChanged        ( const QString& );
  void sigFeatureSimFactChanged    ( const QString& );
  void sigMaxNodeSizeChanged       ( const QString& );
  void sigCompressionRatioChanged  ( const QString& );
  void sigSimLevelChanged          ( const QString& );
  void sigFileNameChanged          ( const QString& );
  void sigEnergyChanged            ( const QString& );

  void sigTreeNumChanged	(const QString& );
  void sigTreeDepthChanged	(const QString& );
  void sigWeekClassifierChanged	(const QString& );
  void sigMinSampleChanged	(const QString& );
public:
  void processEvents() { qApp->processEvents(); }

public:
  /*********************/
  /*   Parameter I/O   */
  /*********************/
  void saveParams( string sFileName, bool bVerbose=false );
  void loadParams( string sFileName, bool bVerbose=false );

public:
  /*****************************/
  /*   Clustering Parameters   */
  /*****************************/
	QRadioButton *selKMeans;
  QRadioButton *selPostAgglo;
  QRadioButton *selRNNCAgglo;
  QRadioButton *selFastRNNCAgglo;
  QRadioButton *selCompRatio;
  QRadioButton *selSimLevel;
  QRadioButton *selRandomForest;

  int     m_nClusterMethod;

  /* kmeans parameters */
  int     m_nNumClusters;
  double  m_dEps;
  int     m_nMaxIter;

  /* avglink clustering parameters */
  double  m_dSimilarity;
  float   m_dFeatureSimFact;
  int     m_nMaxNodeSize;

  /* reconstitution parameters */
  float   m_dCompressionRatio;
  float   m_dSimLevel;
  int     m_nReconstMethod;

  /* pca parameters */
  bool    m_bUsePCA;
  float   m_dPCAEnergy;

  /*	Randon Forest parameters 	*/
  int 	m_nTreeNumber;
  int 	m_nTreeDepth;
  int 	m_nWeekClassifier;
  int 	m_nMinSample;
};

#endif
