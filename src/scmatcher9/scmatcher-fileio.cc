/*********************************************************************/
/*                                                                   */
/* FILE         scmatcher-fileio.cc                                  */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Interleaved Object Categorization and Segmentation   */
/*              with an Implicit Shape Model.                        */
/*                                                                   */
/*              This file contains methods for loading and saving    */
/*              intermediate results in various formats (e.g. as     */
/*              images or ascii tables for matlab).                  */
/*                                                                   */
/* COPYRIGHT    Bastian Leibe, ETH Zurich, 2003.                     */
/*                                                                   */
/* BEGIN        Tue Nov 05 2002                                      */
/* LAST CHANGE  Fri Mar 05 2004                                      */
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

#include "scmatcher.hh"


/***********************************************************/
/*                          Slots                          */
/***********************************************************/

void ISMReco::saveFVMatlab()
  /*******************************************************************/
  /* Save the training patches to disk in plain ASCII format (to be  */
  /* read in by Matlab). Each line contains one codebook entry.      */
  /*******************************************************************/
{
  QString qsFileName = 
    QFileDialog::getSaveFileName( DIR_CL_FVECS.c_str(), 
                "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this);
  if ( qsFileName.isEmpty() )
    return;
  
  ofstream ofile( qsFileName );
  if( ofile == 0 ) {
    cerr << "ERROR: Couldn't open file " << qsFileName << "!" << endl;
    return;
  }

  m_cbCodebook.saveFeaturesASCII( qsFileName.latin1() );
}


void ISMReco::saveClustersMatlab()
  /*******************************************************************/
  /* Save the codebook entries to disk in plain ASCII format (to be  */
  /* read in by Matlab). Each line contains one codebook entry.      */
  /*******************************************************************/
{
  QString qsFileName = 
    QFileDialog::getSaveFileName( DIR_CL_FVECS.c_str(), 
             "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this);
  if ( qsFileName.isEmpty() )
    return;
  
  ofstream ofile( qsFileName );
  if( ofile == 0 ) {
    cerr << "ERROR: Couldn't open file " << qsFileName << "!" << endl;
    return;
  }

  m_cbCodebook.saveClustersASCII( qsFileName.latin1() );
}


void ISMReco::savePatchActivationsMatlab()
  /*******************************************************************/
  /* Save the patch activations to disk in plain ASCII format. That  */
  /* is, for each match of an interest point to a codebook cluster,  */
  /* write an entry containing the interest point coordinates, the   */
  /* cluster id, and the matching score.                             */
  /*******************************************************************/
{
  QString qsFileName = 
    QFileDialog::getSaveFileName( DIR_CL_FVECS.c_str(), 
               "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this, "SaveResults", 
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

  for( int j=0; j<(int)m_vvAllNeighbors.size(); j++ ) 
    for( int k=0; k<(int)m_vvAllNeighbors[j].size(); k++ ) 
      if( m_vvAllNeighborsSim[j][k] > 
          m_parMatching.params()->m_dRejectionThresh ) {
        int clusterId = m_vvAllNeighbors[j][k];

        /* write entry to result file:   */
        /*   x, y, clusterid, similarity */
        ofile << m_vPointsInside[j].x << " " << m_vPointsInside[j].y << " ";
        ofile << clusterId << " " << m_vvAllNeighborsSim[j][k] << endl;
      }
  ofile.close();
}


void ISMReco::saveImages()
  /*******************************************************************/
  /* Save the test and result image to disk in png format.           */
  /*******************************************************************/
{
  QString qsFileName = 
    QFileDialog::getSaveFileName( DIR_RESULTS.c_str(), 
                                  "Images (*.png);;All files (*.*)",
                                  this, "SaveImages", 
                                  "Select name for displayed images");
  if ( qsFileName.isEmpty() )
    return;

  string fdir = qsFileName.latin1();
  cout << "Destination path is: " << fdir << endl;
  
  int pos = fdir.rfind(".png");
  if ( pos != (int)string::npos ) fdir.erase( pos, 4 );
  string fdirSrc( fdir + "-Src.png" );
  string fdirDest( fdir + "-Dest.png" );
  string fformat( "PNG" );
  QString qSrc( fdirSrc.c_str() );
  QString qDest( fdirDest.c_str() );
  QImage srcImg  = rsSourceImg->getImage();
  QImage destImg = rsResultImg->getImage();
  
  /* save the corresponding qimages */
  bool savedOK = false;
  savedOK = srcImg.save( qSrc, fformat.c_str() );
  savedOK = savedOK && destImg.save( qDest, fformat.c_str() );
  if ( !savedOK ) {
    cerr << "Files couldn't be saved! "
         << "Please make sure to enter a valid destination path." << endl;
    return;
  }
}


void ISMReco::saveSegmentationsMatlab()
  /*******************************************************************/
  /* Open a file dialog to ask for a segmentation file name, then    */
  /* generate and save segmentation images (see below for details).  */
  /*******************************************************************/
{
  QString qsFileName = 
    QFileDialog::getSaveFileName( DIR_CL_FVECS.c_str(), 
                "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this, "SaveResults", 
                                  "Select name for result file");
  if ( qsFileName.isEmpty() )
    return;

  saveSegmentationsMatlab( qsFileName );
}


void ISMReco::saveSegmentationsMatlab( QString qsFileName )
  /*******************************************************************/
  /* Generate a segmentation from the current hypothesis (as defined */
  /* by the votes in m_vActiveVotes) and write it to disk in three   */
  /* files: one for the p(figure) probability map, one for the       */
  /* p(ground) probability map, and the final segmentation.          */
  /*******************************************************************/
{  
  /* Write segmentation maps to disk:                                 */
  /*   1st file: p(p=figure | o_n,x) - suffix "-pfig.tab"             */
  /*   2nd file: p(p=ground | o_n,x) - suffix "-pgnd.tab"             */
  /*   3rd file: final segmentation  - suffix "-seg.tab"              */

  /* replace the file extension */
  string sFileName  = qsFileName.latin1();
  string sExtension = ".tab";
  int posSlash = sFileName.rfind( "/" );
  int posDot   = sFileName.rfind( "." );
  if ( (posDot != (int)string::npos) && (posDot > posSlash) ) {
    sExtension = sFileName.substr( posDot, string::npos );
    sFileName.erase( posDot, string::npos );
  }
  
  /* set the necessary flags */
  //bool bMapsOnManually = m_bMapsOnManually;
  //slotSetMapsOnOff( 1 );

  /* generate the segmentation */
  OpGrayImage imgPFig, imgPGnd, imgSeg;
  m_ismReco.drawSegmentation(m_vActiveVotes, m_vPointsInside, m_fcCue, 
                              imgPFig, imgPGnd, imgSeg, true );
  
  //m_resultImg = drawVotePatches( m_vActiveVotes, true, false, false, true );
  saveImageAscii( imgSeg, sFileName + "-seg"  + sExtension );
  saveImage( imgSeg, sFileName + "-seg.png" );

  /* generate the probabilities for "figure" */
  //m_resultImg = drawVotePatches( m_vActiveVotes, true, true, true, true );
  saveImageAscii( imgPFig, sFileName + "-pfig" + sExtension );
  saveImage( imgPFig, sFileName + "-pfig.png" );

  /* generate the probabilities for "ground" */
  //m_resultImg = drawVotePatches( m_vActiveVotes, true, true, false, true );
  saveImageAscii( imgPGnd, sFileName + "-pgnd" + sExtension );
  saveImage( imgPGnd, sFileName + "-pgnd.png" );

  /* reset the flags top their initial values */
  //slotSetMapsOnOff( bMapsOnManually );
}


void ISMReco::saveClusterImages()
  /*******************************************************************/
  /* Save all cluster centers and all corresponding feature vectors  */
  /* as images. The routine opens a file dialog to ask for a target  */
  /* directory. All cluster centers and their assigned image patches */
  /* are then written to this directory as images.                   */
  /*******************************************************************/
{
  cout << "saveClusterImages called..." << endl;
  if ( m_cbCodebook.getNumClusters()<=0 ) {
    cout << "You first have to execute the clustering." << endl;
    return;
  }
  
  QString qsStartDir( DIR_SAVED_IMAGES.c_str() );
  
  QString qsFileName = QFileDialog::getSaveFileName( qsStartDir, 
                   "Images (*.png *.ppm *.pgm *.jpg);;All files (*.*)",  
                                                     this, "save", 
                         "Select Directory for Saving Cluster Images" );
  
  if ( qsFileName.isEmpty() ) 
    return;

  m_cbCodebook.saveClusterImages( qsFileName.latin1() );
}


void ISMReco::saveTextureMap()
{
  int TEXTURE_WIDTH = 2048;
  int MAP_SIZE      = 16;

  /*---------------------*/
  /* Ask for a file name */
  /*---------------------*/
  static QString qsFileName = DIR_RESULTS.c_str();
  qsFileName = 
    QFileDialog::getSaveFileName( qsFileName, 
                               "Images (*.png *.pgm *.jpg);;All files (*.*)",
                               this, "save", 
                               "Select name for texture map image" );
  
  if ( qsFileName.isEmpty() ) 
    return;

  /*---------------------------------------*/
  /* Create a texture map from the OccMaps */
  /*---------------------------------------*/
  vector<OpGrayImage> vOccMaps = m_ismReco.getOccMaps();

  int nNumMaps  = (int)vOccMaps.size();
  int nNumCols  = TEXTURE_WIDTH / MAP_SIZE;
  int nNumRows  = (int)ceil(nNumMaps / (float)nNumCols);

  int nTextureW = nNumCols*MAP_SIZE;
  int nTextureH = nNumRows*MAP_SIZE; 
  OpGrayImage imgTexture( nTextureW, nTextureH );
  
  // resize all OccMaps and copy them into the texture
  for( int i=0; i<nNumMaps; i++ ) {
    OpGrayImage imgMap = vOccMaps[i].opRescaleToWidth( MAP_SIZE );

    int idxx = i % nNumCols;
    int idxy = i / nNumCols;
    for( int y=idxy*MAP_SIZE, yy=0; y<nTextureH && yy<MAP_SIZE; y++, yy++ )
      for( int x=idxx*MAP_SIZE, xx=0; x<nTextureW && xx<MAP_SIZE; x++, xx++ )
        imgTexture(x,y) = imgMap(xx,yy).value();
  }
      
  /*-------------------------------------*/
  /* Save the texture map as a png image */
  /*-------------------------------------*/
  saveImage( imgTexture, qsFileName.latin1() );
}


void ISMReco::saveSegmentationData()
{
  int MAP_SIZE      = 16;

  /*---------------------*/
  /* Ask for a file name */
  /*---------------------*/
  static QString qsFileName = DIR_RESULTS.c_str();
  qsFileName = 
    QFileDialog::getSaveFileName( qsFileName, 
                                  "Votes files (*.tab);;All files (*.*)",
                                  this, "save", 
                                  "Select a name for the vote list" );
  
  if ( qsFileName.isEmpty() ) 
    return;

  /*--------------------------------------*/
  /* Write a file in the following format */
  /*--------------------------------------*/
  // image_width image_height
  // #votes
  // x_1 y_1 s_1 idx_1 w_1
  // ...
  // x_N y_N s_N idx_N w_N

  ofstream ofile( qsFileName.latin1() );
  if( !ofile ) {
    cerr << "  ERROR in ISMReco::saveSegmentationData(): "
         << "Couldn't open file '" << qsFileName << "'!" << endl;
    return;
  }

  // Write the image dimensions
  ofile << m_grayImg.width() << '\t' << m_grayImg.height() << endl;

  // Write the active votes to the file
  ofile << m_vActiveVotes.size() << endl;
  
  VecVecOccurrence vvOccurrences = m_ismReco.getOccurrences();
  float dScaleFactor = m_fcCue.params()->m_dScaleFactor;
  for(unsigned i=0; i<m_vActiveVotes.size(); i++ ) {
    int nClusterId = m_vActiveVotes[i].getClusterId();
    int nPointId   = m_vActiveVotes[i].getImgPointId();
    int nOccNumber = m_vActiveVotes[i].getOccNumber();
    int nTargetRad = (int) floor(m_vPointsInside[nPointId].scale*
                                 dScaleFactor + 0.5);
    int nTargetSize= 2*nTargetRad+1;
    
    int nOccMapIdx = vvOccurrences[nClusterId][nOccNumber].nOccMapIdx;
//     if( nOccMapIdx<0 || nOccMapIdx>=(int)vOccMaps.size() ) {
//       cerr << "  WARNING in ISMReco::saveSegmentationData(): "
//            << "OccMapIdx has invalid value (" << nOccMapIdx << ">" 
//            << vOccMaps.size() << ")!" << endl;
//       continue;
//     }

    ofile << m_vPointsInside[nPointId].x << '\t'
          << m_vPointsInside[nPointId].y << '\t'
          << (float)nTargetSize/(float)MAP_SIZE << '\t'
          << nOccMapIdx << '\t'
          << m_vActiveVotes[i].getValue() << endl;
  }

  // Close the file again
  ofile.close();
  
}


/***********************************************************/
/*                    Service Functions                    */
/***********************************************************/

QStringList ISMReco::getFileList( )
  /*******************************************************************/
  /* Open a file dialog and query the user for a file list.          */
  /*******************************************************************/
{
	return getFileList( DIR_IMAGES.c_str(), "Select Files");
}


QStringList ISMReco::getFileList( string sDir, string sCaption )
  /*******************************************************************/
  /* Open a file dialog and query the user for a file list.          */
  /*******************************************************************/
{
	QStringList qslFileList;
  qslFileList = QFileDialog::getOpenFileNames ( 
							  "Images (*.png *.ppm *.pgm *.jpg);;all files (*.*)", 
								sDir.c_str(), this, "filedia", sCaption.c_str() );

	cout << "  The following images were selected: " << endl;
  for(int i=0; i<(int)qslFileList.count(); i++ )
    cout << "    " << qslFileList[i] << endl;

	return qslFileList;
}


bool ISMReco::readVector( string sFileName, FeatureVector &fvVector )
  /*******************************************************************/
  /* Read in a vector in plain ASCII format from the given file name */
  /*******************************************************************/
{
  ifstream ifile;

  ifile.open( sFileName.c_str() );
  if( !ifile ) {
    cerr << "ERROR: Couldn't open file '" << sFileName << "'." << endl;
    return false;
  }

  /* read the featurevector dimension */
  float width;
  ifile >> width;
  int dim = (int) floor(width);
  
  /* read the featurevector */
  cout << "  Reading in the feature vector..." << endl;
  fvVector.clear();
  fvVector.setNumDims( dim );
  for( int x=0; x<dim; x++ ) {
    float val;
    ifile >> val;
    fvVector.setValue( x, val );
  }
  cout << "  done." << endl;
  return true;
}


bool ISMReco::readMatrix ( string sFileName, vector<FeatureVector> &mMatrix )
  /*******************************************************************/
  /* Read in a matrix in plain ASCII format from the given file name */
  /*******************************************************************/
{
  ifstream ifile;

  ifile.open( sFileName.c_str() );
  if( !ifile ) {
    cerr << "ERROR: Couldn't open file '" << sFileName << "'." << endl;
    return false;
  }

  /* read the matrix dimensions */
  float width, height;
  ifile >> width >> height;
  int w = (int) floor(width);
  int h = (int) floor(height);
  
  /* read the matrix */
  cout << "  Reading in the matrix..." << endl;
  mMatrix.clear();
  for( int y=0; y<h; y++ ) {
    FeatureVector fvNew( w );
    for( int x=0; x<w; x++ ) {
      float val;
      ifile >> val;
      fvNew.setValue( x, val );
    }
    mMatrix.push_back( fvNew );
  }
  cout << "  done." << endl;
  return true;
}


void ISMReco::saveImage( OpGrayImage img, string sFileName )
  /*******************************************************************/
  /* Save an image to disk in png format.                            */
  /*******************************************************************/
{
  QImage qimg = img.getQtImage();
  qimg.save( sFileName.c_str(), "PNG" );
}


void ISMReco::saveImageAscii( GrayImage img, string sFileName )
  /*******************************************************************/
  /* Save an image as a plain ASCII table (with one float value per  */
  /* pixel).                                                         */
  /*******************************************************************/
{
  ofstream ofile( sFileName.c_str() );
  if( ofile == 0 ) {
    cerr << "ERROR: Couldn't open file " << sFileName << "!" << endl;
    return;
  }

  ofile << img.width() << endl;
  ofile << img.height() << endl;
  for( int y=0; y<img.height(); y++ ) {
    for( int x=0; x<img.width(); x++ )
      ofile << img(x,y).value() << " ";
    ofile << endl;
  }
  ofile.close();
}


OpGrayImage ISMReco::loadImageAscii( string sFileName )
  /*******************************************************************/
  /* Load an image stored as a plain ASCII table (with one float     */
  /* value per pixel).                                               */
  /*******************************************************************/
{
  ifstream ifile( sFileName.c_str() );
  if( ifile == 0 ) {
    cerr << "ERROR: Couldn't open file " << sFileName << "!" << endl;
    return OpGrayImage(0,0);
  }

  int w, h;
  ifile >> w;
  ifile >> h;
  OpGrayImage img( w, h );

  for( int y=0; y<img.height(); y++ )
    for( int x=0; x<img.width(); x++ ) {
      float val;
      ifile >> val;
      img(x,y) = val;
    }
  ifile.close();

  return img;
}


void ISMReco::writeResultsToDiskUIUC( ofstream &ofile, int nImgNumber,
                                      vector<Hypothesis> vHypos )
  /*******************************************************************/
  /* Write the results to disk in the Agarwal & Roth format.         */
  /*******************************************************************/
{
  if( ofile != 0 ) {
    ofile << ' ' << nImgNumber << ":";
    
    for( int i=0; i<(int)vHypos.size(); i++ )
      ofile << " (" << vHypos[i].nBoxY1 << "," << vHypos[i].nBoxX1 << ")";
    ofile << endl;
  }
}


void ISMReco::writeResultsToDiskScore( ofstream &ofile, int nImgNumber,
                                       vector<Hypothesis> vHypos )
  /*******************************************************************/
  /* Write the results to disk including the exact value of the      */
  /* hypotheses scores.                                              */
  /*******************************************************************/
{
  if( ofile != 0 ) {
    ofile << nImgNumber << endl;

    ofile << vHypos.size() << endl;
    
    for( int i=0; i<(int)vHypos.size(); i++ )
      ofile << vHypos[i].x << "  " << vHypos[i].y << "  " 
            << vHypos[i].nBoxX1 << "  " << vHypos[i].nBoxY1 << "  "
            << vHypos[i].nBoxWidth << "  " << vHypos[i].nBoxHeight << "  "
            << vHypos[i].dScale << "  " << vHypos[i].dScore << endl;
  }
}
