/************************************************************************/
/* 									*/
/* FILE		main.cc							*/
/* AUTHOR	Yuren Zhang						*/
/* EMAIL	kiekergaard@gmail.com;yuren.zhang@ia.ac.cn		*/
/* 									*/
/* CONTENT	The start point of the Qt app				*/
/* BEGIN	2013.2.1						*/
/* LAST CHANGE	2013.2.1						*/
/************************************************************************/

/************************/
/*	Include		*/
/************************/
#include "rf_ismreco.hh"
#include <QApplication>
/************************/
/*	Defintions	*/
/************************/


int main(int argc, char* argv[])
{

	QApplication a(argc, argv);
	RFISMReco w;
	w.show();
	
	return  a.exec();


}
