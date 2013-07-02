/*********************************************************************/
/*                                                                   */
/* FILE         matchinggui.cc                                       */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      GUI for the codebook matching parameters.            */
/*                                                                   */
/* BEGIN        Tue Mar 22 2005                                      */
/* LAST CHANGE  Tue Mar 22 2005                                      */
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

#include "matchinggui.hh"

/*===================================================================*/
/*                        Class MatchingGUI                          */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/
MatchingGUI::MatchingGUI( QWidget *parent, const char *name )
  : QWidget( parent, name )
{
  QVBoxLayout *menuleftbox  = new QVBoxLayout( this );
  
  /************************************************/
  /*   parameter fields for 'Compare' function    */
  /************************************************/
  menuleftbox->addSpacing( 5 );
  QHBoxLayout  *hblParameters = new QHBoxLayout();
  
  menuleftbox->addLayout( hblParameters );
  
  QVBox        *boxReject = new QVBox( this, "boxReject" );
  QLineEdit    *editReject = new QLineEdit( "0.70", boxReject, "editReject");
  QLabel       *labelReject = new QLabel( "Rej. Thresh", boxReject);
  editReject->setAlignment( Qt::AlignVCenter | Qt::AlignHCenter );
  labelReject->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
  editReject->setMaximumWidth(40);
  boxReject->setMaximumHeight(50);

  m_dRejectionThresh = atof(editReject->text() );
  QT_CONNECT_LINEEDIT( editReject, RejectionThreshold );
  
  QButtonGroup *selCompMethod = new QButtonGroup( "Method:", this, 
                                                  "selCM");
  selCompMethod->setColumnLayout( 1, Qt::Horizontal );
  
  selCorrel   = new QRadioButton( "Correlation", 
                                  selCompMethod,"selCorrel" );
  selEuclid   = new QRadioButton( "Euclidean", 
                                  selCompMethod,"selEuclid" );

  QHBox        *hbSimFact   = new QHBox( selCompMethod, "hbSimFact" );
  QLabel       *labSimFact  = new QLabel( "Sim Fact:", hbSimFact );
  QLineEdit    *editSimFact = new QLineEdit( "800", hbSimFact, 
                                             "edSimFact" );
  editSimFact->setMaximumWidth(50);
  m_dFeatureSimFact = atof( editSimFact->text() );
  QT_CONNECT_LINEEDIT( editSimFact, FeatureSimFact );
  
  selCorrel->setChecked( true );
  m_nCompareSelect = CMP_CORRELATION;
  QT_CONNECT_RADIOBUTTON( selCompMethod, CompMethod );
  
  hblParameters->addWidget( boxReject );
  hblParameters->addWidget( selCompMethod );
}


void MatchingGUI::saveParams( string sFileName, bool bVerbose )
{
  QFile qfile( sFileName);
  if ( qfile.open( IO_WriteOnly | IO_Append ) ) {
    QTextStream stream( &qfile );
    
    stream << "\n*** Section MatchingGUI ***\n"
           << "m_nCompareSelect: " << m_nCompareSelect << "\n"
           << "m_dRejectionThresh: " << m_dRejectionThresh << "\n"
           << "m_dFeatureSimFact: " << m_dFeatureSimFact << "\n";
    qfile.close();
  }
}


void MatchingGUI::loadParams( string sFileName, bool bVerbose )
{
  if( bVerbose )
    printf("  MatchingGUI::loadParams() ...\n");

  QFile qfile( sFileName );
  if ( qfile.open( IO_ReadOnly ) ) {
    QTextStream stream( &qfile );
    QString line;
    bool started = false;
    
    while(!(line = stream.readLine()).isNull()) {
      if (!started || line.isEmpty()) {
        if(line.compare("*** Section MatchingGUI ***")==0)
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
        printf("  line: %s\n",line.latin1());
      
      QString name = line.left(pos);
      QString val = line.right(len-pos-2);
      
      if (name.compare("m_nCompareSelect")==0) {
        m_nCompareSelect = val.toInt();
        switch (m_nCompareSelect)
          {            
          case CMP_CORRELATION:
            selCorrel->setChecked(true);
            break;
          case CMP_EUKLIDEAN:
            selEuclid->setChecked(true);
            break;

          default:
            cerr << "*********   ERROR: Patch Matching Method unknown ("
                 << val << ")!!!    *********" << endl;
          }
        if( bVerbose )
          printf("   val: %s -> setting %d\n", val.latin1(), val.toInt());
      }
      else if (name.compare("m_dRejectionThresh")==0)
        emit sigRejectionThresholdChanged(val);
      else if (name.compare("m_dFeatureSimFact")==0)
        emit sigFeatureSimFactChanged(val);
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

QT_IMPLEMENT_LINEEDIT_FLOAT( MatchingGUI::slot, RejectionThreshold, 
                             m_dRejectionThresh, 2 )
QT_IMPLEMENT_LINEEDIT_FLOAT( MatchingGUI::slot, FeatureSimFact, 
                             m_dFeatureSimFact, 1 )

QT_IMPLEMENT_RADIOBUTTON( MatchingGUI::slot, CompMethod, m_nCompareSelect )

