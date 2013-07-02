//
// C++ Implementation: patchmatching-gui
//
// Description:
//
//
// Author: Edgar Seemann,-,-,- <edi@tahiti>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "patchmatching-gui.h"

#include <qtabwidget.h>
#include <qlayout.h>
#include <qvbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qfile.h>

#include <qtmacros.hh>

PatchMatchingGUI::PatchMatchingGUI( QWidget *parent, const char *name )
        : QWidget( parent, name )
{
    printf("Initialize PatchMatching GUI ...\n");

    QVBoxLayout *menuleftbox  = new QVBoxLayout( this );

    /***************************************/
    /*   Tab Widget for Patch Comparison   */
    /***************************************/
    QTabWidget *tabCompare = new QTabWidget( this, "tabcomp" );

    /* tabs: method, options */
    QWidget     *tabwMethod   = new QWidget( this, "tabwMethod" );
    QWidget     *tabwOptions  = new QWidget( this, "tabwOptions" );
    QVBoxLayout *tabpMethod   = new QVBoxLayout( tabwMethod );
    QVBoxLayout *tabpOptions  = new QVBoxLayout( tabwOptions );
    tabpMethod->setSpacing( 0 );
    tabpOptions->setSpacing( 0 );

    /************************************************/
    /*   parameter fields for 'Compare' function    */
    /************************************************/
    tabpMethod->addSpacing( 5 );
    QHBoxLayout  *parameterHold3 = new QHBoxLayout();

    tabpMethod->addLayout( parameterHold3 );

    QVBox        *boxReject = new QVBox( tabwMethod, "boxReject" );
    QLineEdit    *editReject = new QLineEdit( "0.80", boxReject, "editReject");
    QLabel       *labelReject = new QLabel( "Rejection\nThreshold", boxReject);
    editReject->setAlignment( Qt::AlignVCenter | Qt::AlignHCenter );
    labelReject->setAlignment( Qt::AlignVCenter | Qt::AlignLeft );
    editReject->setMaximumWidth(40);
    boxReject->setMaximumHeight(50);
    QT_CONNECT_LINEEDIT( editReject, RejectionThreshold );

    QButtonGroup *selCompMethod = new QButtonGroup( "Method:", tabwMethod,
                                  "selCM");
    selCompMethod->setColumnLayout( 1, Qt::Horizontal );

    //   QRadioButton *selNGC_fast = new QRadioButton( "NGC fast",
    //                                                 selCompMethod,"selNGCfast" );
    //   QRadioButton *selNGC_Edge = new QRadioButton( "NGC + Edge",
    //                                                 selCompMethod,"selNGCEdge" );
    selCorrel   = new QRadioButton( "Correlation",
                                    selCompMethod,"selCorrel" );
    selEuklid   = new QRadioButton( "Euklidean",
                                    selCompMethod,"selEuklid" );
    selNNSearch = new QRadioButton( "NNSearch",
                                    selCompMethod, "selNN");
    selEVMatch  = new QRadioButton( "EV Matching",
                                    selCompMethod, "selEVMatch");
    selChamfer  = new QRadioButton( "Chamfer",
                                    selCompMethod, "Chamfer");
	selChamferOrientation  = new QRadioButton( "Chamfer Orientation",
                                    selCompMethod, "ChamferOrientation");

    QHBox        *hbChamferSym   = new QHBox( selCompMethod, "hbChamferSym" );
    QLabel       *labChamferSym  = new QLabel( "Sym. Dist:", hbChamferSym );
    QCheckBox    *chkChamferSym =  new QCheckBox(hbChamferSym, "chkChamferSym" );
    
    chkChamferSym->setChecked(true);
    m_bChamferSymDist = chkChamferSym->isChecked();
    QT_CONNECT_CHECKBOX( chkChamferSym, ChamferSym );

    QHBox        *hbEVEnerg   = new QHBox( selCompMethod, "hbEVEnerg" );
    QLabel       *labEVEnerg  = new QLabel( "EV Energy:", hbEVEnerg );
    QLineEdit    *editEVEnergy= new QLineEdit( "0.90", hbEVEnerg,
                                "editEVEnerg" );
    editEVEnergy->setMaximumWidth(40);
    m_dEVEnergy = atof( editEVEnergy->text() );
    QT_CONNECT_LINEEDIT( editEVEnergy, EVEnergy );

    
    selChamfer->setChecked( true );
    m_nCompareSelect = CMP_CHAMFER;
    QT_CONNECT_RADIOBUTTON( selCompMethod, CompMethod );

    // initialize default value
    m_dRejectionThresh = atof(editReject->text() );

    parameterHold3->addWidget( boxReject );
    parameterHold3->addWidget( selCompMethod );

    /********************************/
    /*   Patch Comparison Options   */
    /********************************/
    tabpOptions->addSpacing( 5 );
    QButtonGroup *bgOptions = new QButtonGroup("Drawing Options:",
                              tabwOptions, "bgOptions");
    bgOptions->setColumnLayout( 2, Qt::Horizontal );
    tabpOptions->addWidget( bgOptions );

    /*--------------------------*/
    /*   Checkbox "Draw Maps"   */
    /*--------------------------*/
    chkMaps = new QCheckBox( "Draw Maps", bgOptions, "sMaps" );
    QLabel       *labDummy1= new QLabel( "", bgOptions );

    chkMaps->setChecked( false );
    m_bMapsOnManually = chkMaps->isChecked();
    QT_CONNECT_CHECKBOX( chkMaps, Maps );

    /*---------------------------------*/
    /*   Checkbox "Draw Confidences"   */
    /*---------------------------------*/
    chkDrawConf = new QCheckBox( "Draw Confidence", bgOptions,
                                 "chkconf" );
    QLabel       *labDummy4= new QLabel( "", bgOptions );

    chkDrawConf->setChecked( false );
    m_bDrawConfidence = chkDrawConf->isChecked();
    QT_CONNECT_CHECKBOX( chkDrawConf, DrawConf );

    /*------------------------------------*/
    /*   Checkbox "Draw Maps > x votes"   */
    /*------------------------------------*/
    chkMapThresh = new QCheckBox( "Only Draw Maps > ", bgOptions,
                                  "chkmapthresh" );
    QLineEdit    *editMapThresh = new QLineEdit( "5.0", bgOptions,
                                  "editMapThresh" );
    chkMapThresh->setChecked( false );
    m_bUseMapThresh = chkMapThresh->isChecked();
    QT_CONNECT_CHECKBOX( chkMapThresh, MapThresh );

    editMapThresh->setMinimumWidth(30);
    editMapThresh->setMaximumWidth(40);
    m_dMapThresh = atof( editMapThresh->text() );
    QT_CONNECT_LINEEDIT( editMapThresh, MapThresh );

    /*--------------------------------------*/
    /*   Checkbox "Use Patch Size Factor"   */
    /*--------------------------------------*/
    chkPSFact = new QCheckBox( "Use Patch Size Factor", bgOptions,
                               "chkpsf" );
    QLabel       *labDummy5= new QLabel( "", bgOptions );

    chkPSFact->setChecked( false );
    m_bPatchSizeFactor = chkPSFact->isChecked();
    QT_CONNECT_CHECKBOX( chkPSFact, PatchSizeFact );

    /*----------------------------------*/
    /*   Checkbox "Draw Tight BBoxes"   */
    /*----------------------------------*/
    chkTightBB = new QCheckBox( "Draw Tight BBoxes", bgOptions,
                                "sTightBB" );
    QLabel       *labDummy6= new QLabel( "", bgOptions );

    chkTightBB->setChecked( true );
    m_bDrawTightBB = chkMaps->isChecked();
    QT_CONNECT_CHECKBOX( chkTightBB, DrawTightBB );

    /* checkbox */
    //   QCheckBox    *selectFigure = new QCheckBox( "Only use figure area", this,
    //                                               "sFigure" );
    //   selectFigure->setChecked( true );
    //   m_bOnlyUseFigure = true;
    //   QT_CONNECT_CHECKBOX( selectFigure, Figure );


    //menuleftbox->addWidget( selectFigure );


    /**************************************************/
    /*   Finish the tab Widget for Patch Comparison   */
    /**************************************************/
    tabpMethod->addStretch( 50 );
    tabpOptions->addStretch( 50 );

    tabCompare->addTab( tabwMethod,  "Comparison"  );
    tabCompare->addTab( tabwOptions, "Drawing" );

    menuleftbox->addWidget( tabCompare );
}


QT_IMPLEMENT_LINEEDIT_FLOAT( PatchMatchingGUI::slot, RejectionThreshold, m_dRejectionThresh, 2 )
QT_IMPLEMENT_LINEEDIT_FLOAT( PatchMatchingGUI::slot, EVEnergy, m_dEVEnergy, 2 )

QT_IMPLEMENT_LINEEDIT_FLOAT( PatchMatchingGUI::slot, MapThresh, m_dMapThresh, 1)

QT_IMPLEMENT_RADIOBUTTON( PatchMatchingGUI::slot, CompMethod, m_nCompareSelect )

QT_IMPLEMENT_CHECKBOX( PatchMatchingGUI::slot, ChamferSym, m_bChamferSymDist )
QT_IMPLEMENT_CHECKBOX( PatchMatchingGUI::slot, Maps, m_bMapsOnManually )
QT_IMPLEMENT_CHECKBOX( PatchMatchingGUI::slot, DrawConf, m_bDrawConfidence )
QT_IMPLEMENT_CHECKBOX( PatchMatchingGUI::slot, MapThresh, m_bUseMapThresh )
QT_IMPLEMENT_CHECKBOX( PatchMatchingGUI::slot, PatchSizeFact, m_bPatchSizeFactor )
QT_IMPLEMENT_CHECKBOX( PatchMatchingGUI::slot, DrawTightBB, m_bDrawTightBB )



void PatchMatchingGUI::saveParams(const QString& filename)
{
    QFile file(filename);
    if ( file.open( IO_WriteOnly | IO_Append ) )
    {
        QTextStream stream( &file );

        stream << "\n*** Section PatchMatching ***\n"
        << "m_nCompareSelect: " << m_nCompareSelect << "\n"
        << "m_dEVEnergy: " << m_dEVEnergy << "\n"
        << "m_dRejectionThresh: " << m_dRejectionThresh << "\n"
        /* Segmentation parameters */
        << "m_bMapsOnManually: " << m_bMapsOnManually << "\n"
        << "m_bDrawConfidence: " << m_bDrawConfidence << "\n"
        << "m_bUseMapThresh: " << m_bUseMapThresh << "\n"
		<< "m_dMapThresh: " << m_dMapThresh << "\n"
        << "m_bPatchSizeFactor: " << m_bPatchSizeFactor << "\n"
        << "m_bDrawTightBB: " << m_bDrawTightBB << "\n";
        file.close();
    }
}

void PatchMatchingGUI::loadParams(const QString& filename)
{
  cout << "  PatchMatchingGUI::loadParams() ..." << endl;
  QFile file(filename);
  if ( file.open( IO_ReadOnly ) )
    {
      QTextStream stream( &file );
      QString line;
      bool started = false;
      
      while(!(line = stream.readLine()).isNull())
        {
          if (!started || line.isEmpty())
            {
              if(line.compare("*** Section PatchMatching ***")==0)
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
          printf("  line: %s\n",line.latin1());
          
          QString name = line.left(pos);
          QString val = line.right(len-pos-2);
          
          if (name.compare("m_nCompareSelect")==0)
            {
              m_nCompareSelect = val.toInt();
              switch (m_nCompareSelect)
                {
                  
                  
                case CMP_CORRELATION:
                  selCorrel->setChecked(true);
                  break;
                case CMP_EUKLIDEAN:
                  selEuklid->setChecked(true);
                  break;
                case CMP_NNSEARCH:
                  selNNSearch->setChecked(true);
                  break;
                case CMP_EV_DIST:
                  selEVMatch->setChecked(true);
                  break;
                case CMP_CHAMFER:
                  selChamfer->setChecked(true);
                  break;
                default:
                  cout << "*********   ERROR: "
                       << "Patch Matching Method unknown !!!    *********" 
                       << endl;
                }
              cout << "   val: " << val 
                   << " -> setting " << val.toInt() << endl;
            }
          else if (name.compare("m_dEVEnergy")==0)
            emit sigEVEnergyChanged(val);
          else if (name.compare("m_dRejectionThresh")==0)
            emit sigRejectionThresholdChanged(val);
          else if (name.compare("m_bMapsOnManually")==0)
            {
              chkMaps->setChecked((bool)val.toInt());
              emit slotSetMapsOnOff(val.toInt());
            }
          else if (name.compare("m_bDrawConfidence")==0)
            {
              chkDrawConf->setChecked((bool)val.toInt());
              emit slotSetDrawConfOnOff(val.toInt());
            }
          else if (name.compare("m_bUseMapThresh")==0)
            {
              chkMapThresh->setChecked((bool)val.toInt());
              emit slotSetMapThreshOnOff(val.toInt());
            }
          else if (name.compare("m_dMapThresh")==0)
            emit sigMapThreshChanged(val);
          else if (name.compare("m_bPatchSizeFactor")==0)
            {
              chkPSFact->setChecked((bool)val.toInt());
              emit slotSetPatchSizeFactOnOff(val.toInt());
            }
          else if (name.compare("m_bDrawTightBB")==0)
            {
              chkTightBB->setChecked((bool)val.toInt());
              emit slotSetDrawTightBBOnOff(val.toInt());
            }
          else
            cout << "XXXXXXXXXXXXXXX     WARNING: variable " << name 
                 << " unknown!!!     XXXXXXXXXXXXXXXXXX" << endl;
        }
      file.close();
    }
}








