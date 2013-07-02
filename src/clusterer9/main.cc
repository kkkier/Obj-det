/*********************************************************************/
/*                                                                   */
/* FILE         main.cc                                              */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      main function - calls up a Qt window                 */
/*                                                                   */
/* BEGIN        Wed Aug 15 2001                                      */
/* LAST CHANGE  Tue Oct 01 2002                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <qapplication.h>
#include <qdir.h>
#include <qtimgbrowser.hh>

#include <codebook.hh>

#include "clusterer.hh"

const QString usage =
    "USAGE: clusterer [OPTIONS]\n\n \
    -nw: no gui\n \
    -p FILE: load parameters from FILE\n \
    -idl FILE: load images from IDL FILE\n \
    -o FILE: output file for codebook\n";


int main( int argc, char **argv )
{
    QApplication::setColorSpec( QApplication::CustomColor );
    QApplication a( argc, argv );

    Clusterer w;
    w.setGeometry( 100, 100, 1000, 800 );

    bool gui=true;
    string resultFile = "MYCODEBOOK";

    for(int i=1; i<argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);

        //--- Help ---//
        if (strcmp(argv[i],"-help")==0 || strcmp(argv[i], "--help")==0)
        {
            printf("%s", usage.latin1());
            exit(0);
        }
        //--- GUI? ---//
        if (strcmp(argv[i],"-nw")==0)
        {
            QtImgBrowser::verbosity =0;
            gui=false;
        }

        //--- Load Parameters ---//
        if (strcmp(argv[i],"-p")==0 && argc>i+1)
        {
            w.loadParams( string(argv[i+1]) );
            i++;
        }
        //--- Load Images ---//
        if (strcmp(argv[i],"-idl")==0 && argc>i+1)
        {
            w.loadIDLImageSet( QString(argv[i+1]) );
            i++;
        }
        if (strcmp(argv[i],"-o")==0 && argc>i+1)
        {
            resultFile = argv[i+1];
            i++;
        }
    }
    if (gui)
        w.show();
    else
    {
      /* do everything automatically */
      w.removeClutteredPatches();
      w.clusterPatches();
      w.saveCodebook( //string(QDir::currentDirPath().latin1()), 
                      resultFile+".flz");
      w.remove1PatchClusters();
      w.saveCodebook( //string(QDir::currentDirPath().latin1()), 
                      resultFile+"-no1pc.flz");
      exit(0);
    }

    a.setMainWidget( &w );
    return a.exec();
}

