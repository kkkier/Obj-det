/*********************************************************************/
/*                                                                   */
/* FILE         detector-fileio.cc                                   */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@vision.ee.ethz.ch                              */
/*                                                                   */
/* CONTENT      This file contains methods for loading and saving    */
/*              intermediate results in various formats (e.g. as     */
/*              images or ascii tables for matlab).                  */
/*                                                                   */
/* BEGIN        Thu Feb 09 2006                                      */
/* LAST CHANGE  Fri Feb 10 2006                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <string>
#include <algorithm>

#include <qfiledialog.h>

#include "detector.hh"


/***********************************************************/
/*                          Slots                          */
/***********************************************************/

void Detector::saveFVMatlab( unsigned nIdx )
  /*******************************************************************/
  /* Save the training patches to disk in plain ASCII format (to be  */
  /* read in by Matlab). Each line contains one codebook entry.      */
  /*******************************************************************/
{
  QString qsFileName = 
    QFileDialog::getSaveFileName( m_qsDirCodebooks, 
                "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this->params() );
  if ( qsFileName.isEmpty() )
    return;
  
  ofstream ofile( qsFileName );
  if( ofile == 0 ) {
    cerr << "ERROR: Couldn't open file " << qsFileName << "!" << endl;
    return;
  }

  (*m_vCodebooks[nIdx]).saveFeaturesASCII( qsFileName.latin1() );
}


void Detector::saveClustersMatlab( unsigned nIdx )
  /*******************************************************************/
  /* Save the codebook entries to disk in plain ASCII format (to be  */
  /* read in by Matlab). Each line contains one codebook entry.      */
  /*******************************************************************/
{
  QString qsFileName = 
    QFileDialog::getSaveFileName( m_qsDirCodebooks, 
             "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this->params() );
  if ( qsFileName.isEmpty() )
    return;
  
  ofstream ofile( qsFileName );
  if( ofile == 0 ) {
    cerr << "ERROR: Couldn't open file " << qsFileName << "!" << endl;
    return;
  }

  (*m_vCodebooks[nIdx]).saveClustersASCII( qsFileName.latin1() );
}


void Detector::savePatchActivationsMatlab( unsigned nIdx )
  /*******************************************************************/
  /* Save the patch activations to disk in plain ASCII format. That  */
  /* is, for each match of an interest point to a codebook cluster,  */
  /* write an entry containing the interest point coordinates, the   */
  /* cluster id, and the matching score.                             */
  /*******************************************************************/
{
  QString qsFileName = 
    QFileDialog::getSaveFileName( m_qsDirCodebooks, 
               "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this->params(), "SaveResults", 
                                  "Select name for result file");
  if ( qsFileName.isEmpty() )
    return;
  
  ofstream ofile( qsFileName );
  if( ofile == 0 ) {
    cerr << "ERROR: Couldn't open file " << qsFileName << "!" << endl;
    return;
  }

  /* Write Cluster activations to disk:                              */
  /*   Take point coordinates from m_vPointsInside                   */
  /*   Take cluster activations from m_vvAllNeighbors,               */
  /*     m_vvAllNeighborsSim                                         */

  vector<vector<int> >   &vvAllNeighbors    = 
    (*m_vMatchResults[nIdx]).getAllNeighbors();
  vector<vector<float> > &vvAllNeighborsSim = 
    (*m_vMatchResults[nIdx]).getAllNeighborsSim();
  for( int j=0; j<(int)vvAllNeighbors.size(); j++ ) 
    for( int k=0; k<(int)vvAllNeighbors[j].size(); k++ ) 
      if( vvAllNeighborsSim[j][k] > 
          m_vParMatching[nIdx].params()->m_dRejectionThresh ) {
        int clusterId = vvAllNeighbors[j][k];

        /* write entry to result file:   */
        /*   x, y, clusterid, similarity */
        ofile << m_vvPointsInside[nIdx][j].x << " " 
              << m_vvPointsInside[nIdx][j].y << " ";
        ofile << clusterId << " " << vvAllNeighborsSim[j][k] << endl;
      }
  ofile.close();
}


void Detector::saveClusterImages( unsigned nIdx )
  /*******************************************************************/
  /* Save all cluster centers and all corresponding feature vectors  */
  /* as images. The routine opens a file dialog to ask for a target  */
  /* directory. All cluster centers and their assigned image patches */
  /* are then written to this directory as images.                   */
  /*******************************************************************/
{
  cout << "saveClusterImages called..." << endl;
  if ( (*m_vCodebooks[nIdx]).getNumClusters()<=0 ) {
    cout << "You first have to load a codebook." << endl;
    return;
  }
  
  QString qsStartDir( m_qsDirCodebooks );
  
  QString qsFileName = QFileDialog::getSaveFileName( qsStartDir, 
                   "Images (*.png *.ppm *.pgm *.jpg);;All files (*.*)",  
                                                     this->params(), "save", 
                         "Select Directory for Saving Cluster Images" );
  
  if ( qsFileName.isEmpty() ) 
    return;

  (*m_vCodebooks[nIdx]).saveClusterImages( qsFileName.latin1() );
}


