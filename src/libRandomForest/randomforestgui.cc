/************************************************************************/
/*																		*/
/* FILE		randomforestgui.cc											*/
/* AUTHOR	Yuren Zhang													*/
/* EMAIL	kiekergaard@gmail.com;yuren.zhang@ia.ac.cn					*/
/*																		*/
/* CONTENT	GUI for the random forest parameters						*/
/* BEGIN		Mon Feb 4 2013											*/
/* LAST CHANGE	Mon Feb 4 2013											*/
/************************************************************************/

/****************/
/*	Include		*/
/****************/

#include <QFormLayout>
#include <QString>

#include "randomforestgui.hh"
#include <qtmacros.hh>
/****************/
/*	Defintions	*/
/****************/

/*==========================================================*/
/*					Class RandomForestGUI					*/
/*==========================================================*/

RandomForestGUI::RandomForestGUI( QWidget *parent):QWidget( parent )
{
	
	/*------------------------------*/
	/*	Make the labels and inputs	*/
	/*------------------------------*/
	
  QFormLayout	*bgRandForest = new QFormLayout( this );

  labTreeNum = new QLabel(QString("# Trees:"));
  editTreeNum = new QLineEdit("5");
  labTreeDepth = new QLabel("Tree Depth:");
  editTreeDepth = new QLineEdit("10");
  labWeekClas	= new QLabel("Week Classifers per node");
  editWeekClas	= new QLineEdit("50");
  labMinSample	= new QLabel("Min. Sample");
  editMinSample	= new QLineEdit("20");

  bgRandForest->addRow(labTreeNum, editTreeNum);
  bgRandForest->addRow(labTreeDepth, editTreeDepth);
  bgRandForest->addRow(labWeekClas, editWeekClas);
  bgRandForest->addRow(labMinSample, editMinSample);
  bgRandForest->setVerticalSpacing(10);
  bgRandForest->setHorizontalSpacing(10);
  

  // Initial params 
  m_nTreeNum =  editTreeNum->text().toInt();
  m_nTreeDepth = editTreeDepth->text().toInt();
  m_nWeekClas = editWeekClas->text().toInt(); 
  m_nMinSample = editMinSample->text().toInt();
  
  // Make connections
  QT_CONNECT_LINEEDIT( editTreeNum, TreeNum );
  QT_CONNECT_LINEEDIT( editTreeDepth, TreeDepth );
  QT_CONNECT_LINEEDIT( editWeekClas, WeekClas );
  QT_CONNECT_LINEEDIT( editMinSample, MinSample );
}

QT_IMPLEMENT_LINEEDIT_INT_QT4( RandomForestGUI::slot, TreeNum, m_nTreeNum)
QT_IMPLEMENT_LINEEDIT_INT_QT4( RandomForestGUI::slot, TreeDepth, m_nTreeDepth)
QT_IMPLEMENT_LINEEDIT_INT_QT4( RandomForestGUI::slot, WeekClas, m_nWeekClas)
QT_IMPLEMENT_LINEEDIT_INT_QT4( RandomForestGUI::slot, MinSample, m_nMinSample)
