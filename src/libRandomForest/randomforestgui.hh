
/************************************************************************/
/*																		*/
/* FILE		randomforestgui.hh											*/
/* AUTHOR	Yuren Zhang													*/
/* EMAIL	kiekergaard@gmail.com;yuren.zhang@ia.ac.cn					*/
/*																		*/
/* CONTENT																*/
/* BEGIN		Mon Feb 4	2013										*/
/* LAST CHANGE	Mon Feb 4	2013										*/
/************************************************************************/
#ifndef RANDOMFORESTGUI_HH
#define RANDOMFORESTGUI_HH
/****************/
/*	Include		*/
/****************/
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
/****************/
/*	Defintions	*/
/****************/
class RandomForestGUI : public QWidget
{
	Q_OBJECT
	public:
		RandomForestGUI( QWidget *parent=0);
	public slots:
	/*----------*/
	/*	SLOTS	*/
	/*----------*/	
	void slotSetTreeNum		( const QString &text);
	void slotSetTreeDepth	( const QString &text);
	void slotSetWeekClas	( const QString &text);
	void slotSetMinSample	( const QString &text);
	
	void slotUpdateTreeNum	( );
	void slotUpdateTreeDepth( );
	void slotUpdateWeekClas	( );
	void slotUpdateMinSample( );
	
	signals:
	/*----------*/
	/*	SIGNALS	*/
	/*----------*/
	void sigTreeNumChanged	( const QString &text);
	void sigTreeDepthChanged( const QString &text);
	void sigWeekClasChanged	( const QString &text);
	void sigMinSampleChanged( const QString &text);

	private:
	/************************/
	/*	Interface Handlers	*/
	/************************/
	QLabel	*labTreeNum;
	QLabel	*labTreeDepth;
	QLabel	*labWeekClas;
	QLabel	*labMinSample;
	QLineEdit	*editTreeNum;
	QLineEdit	*editTreeDepth;
	QLineEdit	*editWeekClas;
	QLineEdit	*editMinSample;
	public:
	/****************************/
	/*	Random Forest Params	*/
	/****************************/
	int m_nTreeNum;
	int m_nTreeDepth;
	int m_nWeekClas;
	int m_nMinSample;
};
#endif
