/*********************************************************************/
/*                                                                   */
/* FILE         detectorgui.cc                                       */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@vision.ee.ethz.ch                              */
/*                                                                   */
/* CONTENT      GUI for the detector parameters.                     */
/*                                                                   */
/* BEGIN        Thu Feb 09 2006                                      */
/* LAST CHANGE  Thu Feb 09 2006                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <string>

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
#include <qfile.h>
#include <qapplication.h>
#include <qfileinfo.h>
#include <qfiledialog.h>

#include <qtmacros.hh>

#include "detectorgui.hh"

/*===================================================================*/
/*                        Class DetectorGUI                          */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/
DetectorGUI::DetectorGUI( QWidget *parent, const char *name )
  : QWidget( parent, name )
{
  QVBoxLayout *menuleftbox  = new QVBoxLayout( this );

  /***************************/
  /*   Make a Group 'Cues'   */
  /***************************/
  menuleftbox->addSpacing( 10 );
  QButtonGroup *bgCues = new QButtonGroup( "Detector:", this, "cues" );
  bgCues->setColumnLayout( 1, Qt::Horizontal );
  bgCues->setInsideSpacing( 0 );
 
  /*-------------------------------------*/
  /*   Make fields for 'Category/Pose'   */
  /*-------------------------------------*/
  QButtonGroup *bgInfo = new QButtonGroup( bgCues, "info" );
  bgInfo->setColumnLayout( 2, Qt::Horizontal );
  bgInfo->setInsideSpacing( 0 );
  bgInfo->setFrameStyle( QFrame::NoFrame );

  QLabel      *labCateg  = new QLabel( "Category:", bgInfo );
  QLineEdit   *edCateg   = new QLineEdit( "", bgInfo, "edCateg" );
 
  QLabel      *labPose   = new QLabel( "View/Pose:", bgInfo );
  QLineEdit   *edPose    = new QLineEdit( "", bgInfo, "edPose" );

  QLabel      *labArea   = new QLabel( "Area factor:", bgInfo );
  QLineEdit   *edArea    = new QLineEdit( "1.00", bgInfo, "edArea" );

  QLabel      *labHeight = new QLabel( "Object height (m):", bgInfo );
  QLineEdit   *edHeight  = new QLineEdit( "1.50", bgInfo, "edHeight" );

  chkHeightVar = new QCheckBox( " Use height var.:", bgInfo, "chkHVar" );
  QLineEdit   *edHVar    = new QLineEdit( "0.05", bgInfo, "edHVar" );

  QLabel      *labDist   = new QLabel( "Dist to center (m):", bgInfo );
  QLineEdit   *edDist    = new QLineEdit( "2.70", bgInfo, "edDist" );

  edCateg->setMaximumWidth(80);
  edPose->setMaximumWidth(80);
  edArea->setMaximumWidth(80);
  edHeight->setMaximumWidth(80);
  edHVar->setMaximumWidth(80);
  edDist->setMaximumWidth(80);
  m_qsCategName = edCateg->text();
  m_qsPoseName  = edPose->text();
  m_dAreaFactor = atof(edArea->text());
  m_dRealHeight = atof(edHeight->text());
  m_dHeightVar  = atof(edHVar->text());
  m_dDistCenter = atof(edDist->text());

  QT_CONNECT_LINEEDIT( edCateg, CategName );
  QT_CONNECT_LINEEDIT( edPose,  PoseName );
  QT_CONNECT_LINEEDIT( edArea,  AreaFactor );
  QT_CONNECT_LINEEDIT( edHeight, RealHeight );
  QT_CONNECT_LINEEDIT( edHVar, HeightVar );
  QT_CONNECT_LINEEDIT( edDist, DistCenter );
 
  chkHeightVar->setChecked( false );
  m_bUseHeightVar = chkHeightVar->isChecked();
  QT_CONNECT_CHECKBOX( chkHeightVar, UseHeightVar );

  /*----------------------------------*/
  /* Checkbox 'Process Flipped Image' */
  /*----------------------------------*/
  chkBothDir = new QCheckBox( "Process Flipped Image", bgCues, "chkBothDir" );
  chkBothDir->setChecked( false );
  m_bProcessBothDir = chkBothDir->isChecked();
  QT_CONNECT_CHECKBOX( chkBothDir, BothDir );

 /*----------------------------*/
  /* Checkbox 'Apply GP Filter' */
  /*----------------------------*/
  chkApplyGP = new QCheckBox( "Apply Ground Plane Filter", bgCues, 
                              "chkApplyGP" );
  chkApplyGP->setChecked( false );
  m_bApplyGPFilter = chkApplyGP->isChecked();
  QT_CONNECT_CHECKBOX( chkApplyGP, ApplyGP );

  /*-----------------------------*/
  /*   Make a button 'Add Cue'   */
  /*-----------------------------*/
  QPushButton  *addCue = new QPushButton( "Add Cue", bgCues, "loadCl" );
  
  connect( addCue, SIGNAL(clicked()), this, SLOT(addCue()) );
    
  /*-------------------------------*/
  /*   Make a table for all cues   */
  /*-------------------------------*/
  bgCues->addSpace( 5 );
  m_tblCues = new QTable( 1, 4, bgCues, "CueTable" );
  m_tblCues->setSelectionMode( QTable::SingleRow );

  m_tblCues->horizontalHeader()->setLabel( 0, "Det." );
  m_tblCues->horizontalHeader()->setLabel( 1, "Feat." );
  m_tblCues->horizontalHeader()->setLabel( 2, "#Clust." );
  m_tblCues->horizontalHeader()->setLabel( 3, "#Occs." );

  m_tblCues->setText( 0, 0, "Salient" );
  m_tblCues->setText( 0, 1, "Color" );
  m_tblCues->setText( 0, 2, "12498" );
  m_tblCues->setText( 0, 3, "3134198" );
  for( int i=0; i<4; i++ )
    m_tblCues->adjustColumn( i );
  m_tblCues->setNumRows( 0 );

  connect( m_tblCues, SIGNAL(doubleClicked(int,int,int,const QPoint&)),
           this, SLOT(cueTableClicked(int,int,int,const QPoint&)) );

  menuleftbox->addWidget( bgCues );

  /*--------------------------------------*/
  /*   Make a button 'Collect Features'   */
  /*--------------------------------------*/
//   bgCues->addSpace(10);
//   QPushButton *colPatches = new QPushButton( "Collect Features", bgCues, 
//                                              "colpatches" );

//   connect( colPatches, SIGNAL(clicked()), this, SLOT(collectPatches()) );
  
  /*---------------------------*/
  /*   Make a button 'Match'   */
  /*---------------------------*/
//   QPushButton *compare = new QPushButton( "Match to Codebooks", bgCues, 
//                                           "compare" );
  
//   connect( compare, SIGNAL(clicked()), this, SLOT(compareFeatures()) );
     
}


void DetectorGUI::clear()
{
  m_vCBFiles.clear();
  m_vOccFiles.clear();
  m_tblCues->setNumRows(0);
}


void DetectorGUI::saveParams( string sFileName )
{
  QFile qfile( sFileName );
  if ( qfile.open(IO_WriteOnly ) ) // always overwrite the file
    {
      QTextStream stream( &qfile );
      stream << "\n*** Section DetectorGUI ***\n"
        //-- Detector parameters --//
             << "m_qsCategName: " << m_qsCategName << "\n"
             << "m_qsPoseName: " << m_qsPoseName << "\n"
             << "m_dAreaFactor: " << m_dAreaFactor << "\n"
             << "m_dRealHeight: " << m_dRealHeight << "\n"
             << "m_bUseHeightVar: " << m_bUseHeightVar << "\n"
             << "m_dHeightVar: " << m_dHeightVar << "\n"
             << "m_dDistCenter: " << m_dDistCenter << "\n"
             << "m_bApplyGPFilter: " << m_bApplyGPFilter << "\n"
             << "m_bProcessBothDir: " << m_bProcessBothDir << "\n"
        //-- Cue parameters --//
             << "m_nNumCues: " << m_vCBFiles.size() << "\n";
      for(unsigned k=0; k<m_vCBFiles.size(); k++ )
        stream << "m_sCBFile: " << m_vCBFiles[k] << "\n"
               << "m_sOccFile: " << m_vOccFiles[k] << "\n";
      qfile.close();
    }
}


void DetectorGUI::loadParams( string sFileName, bool bVerbose )
{
  unsigned nNumCues = 0;
  clear();
  QFile qfile( sFileName.c_str() );
  if ( qfile.open( IO_ReadOnly ) ) {
    QTextStream stream( &qfile );
    QString line;
    bool started=false;
    
    while(!(line = stream.readLine()).isNull()) {
      if (!started || line.isEmpty()) {
        if(line.compare("*** Section DetectorGUI ***")==0)
          started=true;
        continue;

      } else if(line.left(1).compare("*")==0) {
        started=false;//stop if section is over
        break;
      }
        
      int len = line.length();
      int pos = line.find(':');
        
      QString name = line.left(pos);
      QString val = line.right(len-pos-2);
      
      //-- Detector parameters --//
      if (name.compare("m_qsCategName")==0)
        emit sigCategNameChanged(val);
      else if (name.compare("m_qsPoseName")==0)
        emit sigPoseNameChanged(val);
      else if (name.compare("m_dAreaFactor")==0)
        emit sigAreaFactorChanged(val);
      else if (name.compare("m_dRealHeight")==0)
        emit sigRealHeightChanged(val);
       else if (name.compare("m_dHeightVar")==0)
        emit sigHeightVarChanged(val);
      else if (name.compare("m_bUseHeightVar")==0) {
        chkHeightVar->setChecked((bool)val.toInt());
        slotSetUseHeightVarOnOff(val.toInt());
      }
       else if (name.compare("m_dDistCenter")==0)
        emit sigDistCenterChanged(val);
     else if (name.compare("m_bApplyGPFilter")==0) {
        chkApplyGP->setChecked((bool)val.toInt());
        slotSetApplyGPOnOff(val.toInt());
      }
      else if (name.compare("m_bProcessBothDir")==0) {
        chkBothDir->setChecked((bool)val.toInt());
        slotSetBothDirOnOff(val.toInt());
      }
      //-- Cue parameters --//
      else if (name.compare("m_nNumCues")==0)
        nNumCues = atoi(val);
      else if (name.compare("m_sCBFile")==0)
        m_vCBFiles.push_back(val.latin1());
      else if (name.compare("m_sOccFile")==0)
        m_vOccFiles.push_back(val.latin1());     
      else
        cerr << "XXXXXXXXX     WARNING: variable " << name.latin1()
             << " unknown !!!     XXXXXXXXXXXX" << endl;
    }
    qfile.close();
  }

  /* check if the correct number of files has been loaded */
  if( !((m_vCBFiles.size()==m_vOccFiles.size()) &&
        (m_vCBFiles.size()==nNumCues)) ) {
    cerr <<  "XXXXXXXXX     WARNING: wrong number of cue files!!! "
         << "     XXXXXXXXXXXX" << endl;
    m_vCBFiles.clear();
    m_vOccFiles.clear();
    nNumCues=0;
  }

  /* load the cues */
  for(unsigned k=0; k<nNumCues; k++ ) {
    emit sigAddCueClicked( m_vCBFiles[k], m_vOccFiles[k], bVerbose );
    qApp->processEvents();
  }
}


void DetectorGUI::setDefaultDir( const string &sDirCodebooks )
{
  m_qsDirCodebooks = sDirCodebooks.c_str();
}


/*---------------------------------------------------------*/
/*                   Loading/Adding Cues                   */
/*---------------------------------------------------------*/

void DetectorGUI::addCue()
{
  /*--------------------------------------*/
  /* Ask for a file name for the codebook */
  /*--------------------------------------*/
  QString qsStartDir( m_qsDirCodebooks );
  QString qsCBName = QFileDialog::getOpenFileName( qsStartDir,
                                      "Vectors (*.fls *.flz);;All files (*.*)",
                                                   this, 
                                                   "LoadCodebook",
                                                   "Select a codebook" );
  if ( qsCBName.isEmpty() )
    return;

  /*-----------------------------------------*/
  /* Ask for a file name for the occurrences */
  /*-----------------------------------------*/
  QString qsOccName = QFileDialog::getOpenFileName( qsCBName,
                                      "Vectors (*.fls *.flz);;All files (*.*)",
                                                    this, 
                                                    "LoadOccurrences",
                                                 "Select an occurrence file");
  if ( qsOccName.isEmpty() )
    return;

  emit sigAddCueClicked( qsCBName.latin1(), qsOccName.latin1(), true );
  //addCue( qsCBName.latin1(), qsOccName.latin1() );
  m_qsDirCodebooks = QFileInfo( qsCBName ).dirPath();

  m_vCBFiles.push_back( qsCBName );
  m_vOccFiles.push_back( qsOccName );
}


void DetectorGUI::cueTableClicked( int nRow, int nCol, int button, 
                               const QPoint& mousePos )
{
  showCueWindow( nRow );
}


void DetectorGUI::showCueWindow( int nIdx )
{
  if( nIdx<0 || nIdx>=(int)m_vpCueWindows.size() )
    return;
  
  m_vpCueWindows[nIdx]->show();
}


void DetectorGUI::addCueTableEntry( string sDetector, string sFeature, 
                                    string sNumCl, string sNumOcc )
{
  unsigned nNewIdx = m_tblCues->numRows();
  m_tblCues->setNumRows( nNewIdx+1 );
  m_tblCues->setText( nNewIdx, 0, sDetector );
  m_tblCues->setText( nNewIdx, 1, sFeature );
  m_tblCues->setText( nNewIdx, 2, sNumCl );
  m_tblCues->setText( nNewIdx, 3, sNumOcc );
  m_tblCues->adjustSize();
}


/***********************************************************/
/*                   Interface Handlers                    */
/***********************************************************/

QT_IMPLEMENT_LINEEDIT_STRING( DetectorGUI::slot, CategName, m_qsCategName )
QT_IMPLEMENT_LINEEDIT_STRING( DetectorGUI::slot, PoseName, m_qsPoseName )
QT_IMPLEMENT_LINEEDIT_FLOAT( DetectorGUI::slot, AreaFactor, m_dAreaFactor, 2 )
QT_IMPLEMENT_LINEEDIT_FLOAT( DetectorGUI::slot, RealHeight, m_dRealHeight, 2 )
QT_IMPLEMENT_LINEEDIT_FLOAT( DetectorGUI::slot, HeightVar, m_dHeightVar, 2 )
QT_IMPLEMENT_LINEEDIT_FLOAT( DetectorGUI::slot, DistCenter, m_dDistCenter, 2 )

QT_IMPLEMENT_CHECKBOX( DetectorGUI::slot, UseHeightVar, m_bUseHeightVar )
QT_IMPLEMENT_CHECKBOX( DetectorGUI::slot, ApplyGP, m_bApplyGPFilter )
QT_IMPLEMENT_CHECKBOX( DetectorGUI::slot, BothDir, m_bProcessBothDir )



