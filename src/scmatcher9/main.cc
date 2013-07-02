/*********************************************************************/
/*                                                                   */
/* FILE         main.cc                                              */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      main function - calls up a Qt window                 */
/*                                                                   */
/* BEGIN        Wed Aug 15 2001                                      */
/* LAST CHANGE  Tue Non 05 2002                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <qapplication.h>

#include "scmatcher.hh"


int main( int argc, char **argv )
{
    QApplication::setColorSpec( QApplication::CustomColor );
    QApplication a( argc, argv );

    ISMReco w;
    w.setGeometry( 100, 100, 1000, 800 );
    a.setMainWidget( &w );
    w.show();

    return a.exec();
}

