/*********************************************************************/
/*                                                                   */
/* FILE         clustergui.cc                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      GUI for the clustering parameters.                   */
/*                                                                   */
/* BEGIN        Wed Mar 16 2005                                      */
/* LAST CHANGE  Wed Mar 16 2005                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <qwidget.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qcolor.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qfile.h>

#include <qtmacros.hh>

#include "clustergui.hh"

/*===================================================================*/
/*                        Class ClusterGUI                           */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/
ClusterGUI::ClusterGUI( QWidget *parent, const char *name )
  : QWidget( parent, name )
  , m_nClusterMethod( CLUSTER_RNNCAGGLO )
{
  QVBoxLayout *menuleftbox  = new QVBoxLayout( this );
  
  /*****************************/
  /* Tab Widget for Clustering */
  /*****************************/
  menuleftbox->addSpacing(5);
  QTabWidget *tabCluster = new QTabWidget( this, "intpts" );
  
  /* tabs: method, kmeans, agglo, simagglo */
  QWidget     *tabwMethod    = new QWidget( this, "tabwMethod" );
  QWidget     *tabwKMeans    = new QWidget( this, "tabwKMeans" );
  QWidget     *tabwPostAgglo = new QWidget( this, "tabwPostAgglo" );
  QWidget     *tabwReconst   = new QWidget( this, "tabwReconst" );
  QWidget     *tabwRandForest= new QWidget( this, "tabwRandForest");
  QVBoxLayout *tabpMethod    = new QVBoxLayout( tabwMethod );
  QVBoxLayout *tabpKMeans    = new QVBoxLayout( tabwKMeans );
  QVBoxLayout *tabpPostAgglo = new QVBoxLayout( tabwPostAgglo );
  QVBoxLayout *tabpReconst   = new QVBoxLayout( tabwReconst );
  QVBoxLayout *tabpRandForest= new QVBoxLayout( tabwRandForest);
  tabpMethod->setSpacing( 0 );
  tabpKMeans->setSpacing( 0 );
  tabpPostAgglo->setSpacing( 0 );
  tabpReconst->setSpacing( 0 );
  tabpRandForest->setSpacing( 0 );
  
  
  /***********************************************/
  /*   make a "Cluster Method" radio button box  */
  /***********************************************/
  tabpMethod->addSpacing( 5 );
  QButtonGroup *bgClMethod = new QButtonGroup( "Clustering Method:",
                                               tabwMethod, "bgClMethod");
  bgClMethod->setColumnLayout( 1, Qt::Horizontal );
  
  
  selKMeans      = new QRadioButton( "k-Means", bgClMethod,
                                     "selKMeans" );
  selPostAgglo   = new QRadioButton( "Agglomerative (post)",
                                     bgClMethod, "selPostAgglo");
  selRNNCAgglo   = new QRadioButton( "Agglomerative (RNNC)",
                                     bgClMethod, "selRNNCAgglo");
  selFastRNNCAgglo= new QRadioButton( "Agglomerative (RNNC) + Balltree",
                                     bgClMethod, "selFRNNCAgglo");
  selRandomForest = new QRadioButton("Random Forest", bgClMethod, "selRandomForest") ; 
  selRNNCAgglo->setChecked( true );
  
  tabpMethod->addWidget( bgClMethod );
  
  QT_CONNECT_RADIOBUTTON( bgClMethod, ClusterMethod );
  
  
  /*----------------------------------------------*/
  /*   make a checkbox 'Use PCA for Clustering'   */
  /*----------------------------------------------*/
  QHBox *hbPCA = new QHBox( tabwMethod );
  
  QCheckBox *chkUsePCA = new QCheckBox( "Use PCA with energy", hbPCA, 
                                        "usepca" );
  QLineEdit *editEnergy = new QLineEdit( "0.90", hbPCA, "editEnergy");
  
  chkUsePCA->setChecked( false );
  m_bUsePCA = chkUsePCA->isChecked();
  
  editEnergy->setMaximumWidth(50);
  m_dPCAEnergy = atof( editEnergy->text() );
  
  QT_CONNECT_CHECKBOX( chkUsePCA, PCA );
  QT_CONNECT_LINEEDIT( editEnergy, Energy );
  
  tabpMethod->addWidget( hbPCA );


  /***********************************/
  /*   parameter fields for KMeans   */
  /***********************************/
  tabpKMeans->addSpacing( 5 );
  QButtonGroup *bgKMeans = new QButtonGroup("Parameters:",
                                            tabwKMeans, "bgKMeans");
  bgKMeans->setColumnLayout( 2, Qt::Horizontal );
  
  QLabel       *labCluster  = new QLabel( "# Clusters:", bgKMeans );
  QLineEdit    *editCluster = new QLineEdit( "7", bgKMeans, "edNumCl" );
  QLabel       *labEps      = new QLabel( "Eps:", bgKMeans );
  QLineEdit    *editEps     = new QLineEdit( "0.01", bgKMeans, "edEps" );
  QLabel       *labMaxIter  = new QLabel( "Max Iter:", bgKMeans );
  QLineEdit    *editMaxIter = new QLineEdit( "25", bgKMeans, "eMaxIter" );
  
  editCluster->setMaximumWidth(50);
  editEps->setMaximumWidth(50);
  editMaxIter->setMaximumWidth(50);
  m_nNumClusters = atoi( editCluster->text() );
  m_dEps         = atof( editEps->text() );
  m_nMaxIter     = atoi( editMaxIter->text() );
  
  QT_CONNECT_LINEEDIT( editCluster, NumClusters );
  QT_CONNECT_LINEEDIT( editEps, Eps );
  QT_CONNECT_LINEEDIT( editMaxIter, MaxIter );
  
  tabpKMeans->addWidget( bgKMeans );
  /**********************************************/
  /*	Parameter fields for Random Forest	*/
  /**********************************************/
  tabpRandForest->addSpacing( 5 );
  QButtonGroup *bgRandForest = new QButtonGroup("Parameters:", tabwRandForest, "bgRandForest");
  bgRandForest->setColumnLayout( 2, Qt::Horizontal );
  
  QLabel	*labTreeNum = new QLabel("# Trees:", bgRandForest);
  QLineEdit	*editTreeNum = new QLineEdit("5" , bgRandForest, "edTreeNum");
  QLabel 	*labTreeDepth = new QLabel("Tree Depth:", bgRandForest);
  QLineEdit	*editTreeDepth = new QLineEdit("10", bgRandForest, "edTreeDep");
  QLabel	*labWeekClas	= new QLabel("Week Classifers per node", bgRandForest);
  QLineEdit	*editWeekClas	= new QLineEdit("50", bgRandForest, "edWeekClas");
  QLabel	*labMinSample	= new QLabel("Min. Sample", bgRandForest);
  QLineEdit	*editMinSample	= new QLineEdit("20", bgRandForest, "edMinSample");
  
  editTreeNum->setMaximumWidth(50);
  editTreeDepth->setMaximumWidth(50);
  editWeekClas->setMaximumWidth(50);
  editMinSample->setMaximumWidth(50);
  
  m_nTreeNumber = atoi(editTreeNum->text());
  m_nTreeDepth 	= atoi(editTreeDepth->text());
  m_nWeekClassifier = atoi(editWeekClas->text());
  m_nMinSample = atoi(editMinSample->text());
 
  QT_CONNECT_LINEEDIT(editTreeNum, TreeNum);
  QT_CONNECT_LINEEDIT(editTreeDepth, TreeDepth);
  QT_CONNECT_LINEEDIT(editWeekClas, WeekClassifier);
  QT_CONNECT_LINEEDIT(editMinSample, MinSample);

  tabpRandForest->addWidget(bgRandForest);
  /***********************************************/
  /*   parameter fields for PostAgglo Clustering */
  /***********************************************/
  tabpPostAgglo->addSpacing( 5 );
  QButtonGroup *bgPostAgglo = new QButtonGroup("Parameters:",
                                               tabwPostAgglo, "bgPostAgglo");
  bgPostAgglo->setColumnLayout( 2, Qt::Horizontal );
  
  QLabel       *labSim      = new QLabel( "Similarity:", bgPostAgglo );
  QLineEdit    *edSim       = new QLineEdit( "0.7", bgPostAgglo, "edSim" );
  QLabel       *labFeatFact = new QLabel( "Feature Factor:", bgPostAgglo );
  QLineEdit    *edFeatFact  = new QLineEdit( "800.0", bgPostAgglo, "edFF" );
  QLabel       *labNodeSize = new QLabel( "Max Node Size:", bgPostAgglo );
  QLineEdit    *edNodeSize  = new QLineEdit( "400", bgPostAgglo, "edMNS" );
  
  edSim->setMaximumWidth(60);
  edFeatFact->setMaximumWidth(60);
  edNodeSize->setMaximumWidth(60);
  m_dSimilarity     = atof( edSim->text() );
  m_dFeatureSimFact = atof( edFeatFact->text() );
  m_nMaxNodeSize    = atoi( edNodeSize->text() );
    
  QT_CONNECT_LINEEDIT( edSim, Similarity );
  QT_CONNECT_LINEEDIT( edFeatFact, FeatureSimFact );
  QT_CONNECT_LINEEDIT( edNodeSize, MaxNodeSize );
  
  tabpPostAgglo->addWidget( bgPostAgglo );
  
  
  /**************************************************/
  /*   parameter fields for Reconstitute Clustering */
  /**************************************************/
  tabpReconst->addSpacing( 5 );
  QButtonGroup *bgReconst = new QButtonGroup("Recreate up to:",
                                             tabwReconst, "bgReconst");
  bgReconst->setColumnLayout( 2, Qt::Horizontal );
  
  selCompRatio               = new QRadioButton( "Compression Ratio:", 
                                                 bgReconst, "selCR" );
  QLineEdit    *edCompRatio  = new QLineEdit( "5.0", bgReconst, "edCRatio" );
  selSimLevel                = new QRadioButton( "Similarity:", 
                                                 bgReconst, "selSim" );
  QLineEdit    *edSimLevel   = new QLineEdit( "0.6", bgReconst, "edSimLevel" );
  
  edCompRatio->setMaximumWidth(60);
  edSimLevel->setMaximumWidth(60);
  m_dCompressionRatio = atof( edCompRatio->text() );
  m_dSimLevel         = atof( edSimLevel->text() );
  
  selCompRatio->setChecked( true );
  m_nReconstMethod = RECONST_COMPRATIO;
    
  QT_CONNECT_LINEEDIT( edCompRatio, CompressionRatio );
  QT_CONNECT_LINEEDIT( edSimLevel, SimLevel );
  QT_CONNECT_RADIOBUTTON( bgReconst, ReconstMethod );
 
  tabpReconst->addWidget( bgReconst );
  
  
  /****************************************/
  /* Finish the tab Widget for Clustering */
  /****************************************/
  tabpMethod->addStretch( 50 );
  tabpKMeans->addStretch( 50 );
  tabpPostAgglo->addStretch( 50 );
  tabpReconst->addStretch( 50 );
  tabpRandForest->addStretch( 50 );
 
  tabCluster->addTab( tabwMethod,  "Method"  );
  tabCluster->addTab( tabwKMeans, "KMeans" );
  tabCluster->addTab( tabwPostAgglo, "Agglo" );
  tabCluster->addTab( tabwReconst, "RecCl" );
  tabCluster->addTab( tabwRandForest, "RandFst");  
  menuleftbox->addWidget( tabCluster );
}


void ClusterGUI::saveParams( string sFileName, bool bVerbose )
{
  QFile qfile( sFileName );
  if ( qfile.open(IO_WriteOnly | IO_Append ) )
    {
      QTextStream stream( &qfile );
      stream << "\n*** Section ClusterGUI ***\n"
             << "m_nClusterMethod: " << m_nClusterMethod << "\n"
             << "m_bUsePCA: " << m_bUsePCA << "\n"
             << "m_dPCAEnergy: " << m_dPCAEnergy << "\n"
        //-- postagglo --//
             << "m_dSimilarity: " << m_dSimilarity << "\n"
             << "m_dFeatureSimFact: " << m_dFeatureSimFact << "\n"
             << "m_nMaxNodeSize: " << m_nMaxNodeSize << "\n"
        //-- kmeans --//
             << "m_nNumClusters: " << m_nNumClusters << "\n"
             << "m_dEps: " << m_dEps << "\n"
             << "m_nMaxIter: " << m_nMaxIter << "\n"
        //-- reconstitute clustering --//
             << "m_dCompressionRatio: " << m_dCompressionRatio << "\n"
             << "m_dSimLevel: " << m_dSimLevel << "\n"
             << "m_nReconstMethod: " << m_nReconstMethod << "\n";
      qfile.close();
    }
}


void ClusterGUI::loadParams( string sFileName, bool bVerbose )
{
  QFile qfile( sFileName.c_str() );
  if ( qfile.open( IO_ReadOnly ) ) {
    QTextStream stream( &qfile );
    QString line;
    bool started=false;
    
    while(!(line = stream.readLine()).isNull())
      {
        if (!started || line.isEmpty())
          {
            if(line.compare("*** Section ClusterGUI ***")==0)
              started=true;
            continue;
          }
        else if(line.left(1).compare("*")==0)
          {
            started=false;//stop if section is over
            break;
          }
        
        int len = line.length();
        int pos = line.find(':');
        if( bVerbose )
          printf("line: %s\n",line.latin1());
        
        QString name = line.left(pos);
        QString val = line.right(len-pos-2);
        
        if (name.compare("m_nClusterMethod")==0) {
          m_nClusterMethod = val.toInt();
          switch (m_nClusterMethod) {
          case CLUSTER_KMEANS:
            selKMeans->setChecked(true);
            break;
          case CLUSTER_POSTAGGLO:
            selPostAgglo->setChecked(true);
            break;
          case CLUSTER_RNNCAGGLO:
            selRNNCAgglo->setChecked(true);
            break;
	  case CLUSTER_RANDOMFOREST:
	    selRandomForest->setChecked(true);
	    break;
          default:
            cerr << "********* ERROR: Cluster Method unknown !!! *********" 
                 << endl;
          }
          if( bVerbose )
            cerr << "   val: " << val.latin1() << " -> setting " 
                 << val.toInt() << endl;
        }
        //-- agglo --//
        else if (name.compare("m_dSimilarity")==0)
          emit sigSimilarityChanged(val);
        else if (name.compare("m_dFeatureSimFact")==0)
          emit sigFeatureSimFactChanged(val);
        else if (name.compare("m_nMaxNodeSize")==0)
          emit sigMaxNodeSizeChanged(val);
        //-- kmeans --//
        else if (name.compare("m_nNumClusters")==0)
          emit sigNumClustersChanged(val);
        else if (name.compare("m_dEps")==0)
          emit sigEpsChanged(val);
        else if (name.compare("m_nMaxIter")==0)
          emit sigMaxIterChanged(val);
        //-- reconstitute clustering  --//
        else if (name.compare("m_nReconstMethod")==0) {
          m_nReconstMethod = val.toInt();
          switch (m_nReconstMethod) {
          case RECONST_COMPRATIO:
            selCompRatio->setChecked(true);
            break;
          case RECONST_SIMLEVEL:
            selSimLevel->setChecked(true);
            break;
          default:
            cerr << "*** ERROR: Cluster Reconstitution Method unknown !!! ***" 
                 << endl;
          }
          if( bVerbose )
            cerr << "   val: " << val.latin1() << " -> setting " 
                 << val.toInt() << endl;
        }
        else if (name.compare("m_dCompressionRatio")==0)
          emit sigCompressionRatioChanged(val);
        else if (name.compare("m_dSimLevel")==0)
          emit sigSimLevelChanged(val);
        else
          cerr << "XXXXXXXXX     WARNING: variable " << name.latin1()
               << " unknown !!!     XXXXXXXXXXXX" << endl;
      }
    qfile.close();
  }
}


/***********************************************************/
/*                   Interface Handlers                    */
/***********************************************************/

QT_IMPLEMENT_LINEEDIT_INT( ClusterGUI::slot, NumClusters, m_nNumClusters )
QT_IMPLEMENT_LINEEDIT_FLOAT( ClusterGUI::slot, Eps, m_dEps, 2 )
QT_IMPLEMENT_LINEEDIT_INT( ClusterGUI::slot, MaxIter, m_nMaxIter )

QT_IMPLEMENT_RADIOBUTTON( ClusterGUI::slot, ClusterMethod, m_nClusterMethod )

QT_IMPLEMENT_LINEEDIT_FLOAT( ClusterGUI::slot, Similarity, m_dSimilarity, 7 )
QT_IMPLEMENT_LINEEDIT_FLOAT( ClusterGUI::slot, FeatureSimFact, m_dFeatureSimFact, 1 )
QT_IMPLEMENT_LINEEDIT_INT( ClusterGUI::slot, MaxNodeSize, m_nMaxNodeSize )

QT_IMPLEMENT_LINEEDIT_FLOAT( ClusterGUI::slot, CompressionRatio, m_dCompressionRatio, 1 )
QT_IMPLEMENT_LINEEDIT_FLOAT( ClusterGUI::slot, SimLevel, m_dSimLevel, 7 )
QT_IMPLEMENT_RADIOBUTTON( ClusterGUI::slot, ReconstMethod, m_nReconstMethod )

QT_IMPLEMENT_CHECKBOX( ClusterGUI::slot, PCA, m_bUsePCA )

QT_IMPLEMENT_LINEEDIT_FLOAT( ClusterGUI::slot, Energy, m_dPCAEnergy, 2 )

QT_IMPLEMENT_LINEEDIT_INT(ClusterGUI::slot, TreeNum, m_nTreeNumber )
QT_IMPLEMENT_LINEEDIT_INT(ClusterGUI::slot, TreeDepth, m_nTreeDepth)
QT_IMPLEMENT_LINEEDIT_INT(ClusterGUI::slot, WeekClassifier, m_nWeekClassifier)
QT_IMPLEMENT_LINEEDIT_INT(ClusterGUI::slot, MinSample, m_nMinSample)

