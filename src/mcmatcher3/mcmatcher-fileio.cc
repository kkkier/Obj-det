/*********************************************************************/
/*                                                                   */
/* FILE         mcmatcher-fileio.cc                                  */
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
/* LAST CHANGE  Tue Oct 11 2005                                      */
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

#include "mcmatcher.hh"


/***********************************************************/
/*                          Slots                          */
/***********************************************************/

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
  
  unsigned pos = fdir.rfind(".png");
  if ( pos != string::npos ) fdir.erase( pos, 4 );
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
    QFileDialog::getSaveFileName( DIR_CODEBOOKS.c_str(), 
                "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this, "SaveResults", 
                                  "Select name for result file");
  if ( qsFileName.isEmpty() )
    return;

  cout << "  WARNING: Functionality currently not available!" << endl;
  //saveSegmentationsMatlab( qsFileName );
}


// void ISMReco::saveSegmentationsMatlab( QString qsFileName )
//   /*******************************************************************/
//   /* Generate a segmentation from the current hypothesis (as defined */
//   /* by the votes in m_vActiveVotes) and write it to disk in three   */
//   /* files: one for the p(figure) probability map, one for the       */
//   /* p(ground) probability map, and the final segmentation.          */
//   /*******************************************************************/
// {  
//   /* Write segmentation maps to disk:                                 */
//   /*   1st file: p(p=figure | o_n,x) - suffix "-pfig.tab"             */
//   /*   2nd file: p(p=ground | o_n,x) - suffix "-pgnd.tab"             */
//   /*   3rd file: final segmentation  - suffix "-seg.tab"              */

//   /* replace the file extension */
//   string sFileName  = qsFileName.latin1();
//   string sExtension = ".tab";
//   unsigned posSlash = sFileName.rfind( "/" );
//   unsigned posDot   = sFileName.rfind( "." );
//   if ( (posDot != string::npos) && (posDot > posSlash) ) {
//     sExtension = sFileName.substr( posDot, string::npos );
//     sFileName.erase( posDot, string::npos );
//   }
  
//   /* set the necessary flags */
//   //bool bMapsOnManually = m_bMapsOnManually;
//   //slotSetMapsOnOff( 1 );

//   /* For all cues */
//   for(unsigned nIdx=0; nIdx<m_nNumCues; nIdx++ ) {
//     /* generate the segmentation */
//     OpGrayImage imgPFig, imgPGnd, imgSeg;
//     m_vISMReco[nIdx].drawSegmentation( m_vActiveVotes, 
//                                        m_vvPointsInside[nIdx], m_vCues[nIdx], 
//                                        imgPFig, imgPGnd, imgSeg, true );
    
//     string sNum="";
//     if( m_nNumCues>1 )
//       sNum = QString::number( nIdx ).latin1();

//     saveImageAscii( imgSeg, sFileName + "-seg"  + sNum + sExtension );
//     saveImage( imgSeg, sFileName + "-seg" + sNum + ".png" );
    
//     /* generate the probabilities for "figure" */
//     saveImageAscii( imgPFig, sFileName + "-pfig" + sNum + sExtension );
//     saveImage( imgPFig, sFileName + "-pfig" + sNum + ".png" );
    
//     /* generate the probabilities for "ground" */
//     saveImageAscii( imgPGnd, sFileName + "-pgnd" + sNum + sExtension );
//     saveImage( imgPGnd, sFileName + "-pgnd" + sNum + ".png" );    
//   }
// }


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
  for(unsigned i=0; i<qslFileList.count(); i++ )
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

  system( string("gzip -f " + sFileName).c_str() );
}


void ISMReco::savePFigPGndMatlab( GrayImage imgPFig, GrayImage imgPGnd,
                                  string sFileName )
  /*******************************************************************/
  /* Save an image in Matlab .mat format (with one float value per   */
  /* pixel).                                                         */
  /*******************************************************************/
{
  int tx, ty, bx, by;
  
  // Create MAT object with optional comment
  MATWriter mw( sFileName.c_str(), "ISM pfig/pgnd masks");
  
  // Write pfig
  findMinMax< float >( imgPFig.height(), imgPFig.width(), 
                       imgPFig.getData(), 0, tx, ty, bx, by );
  mw.writeMatrix< float, float >( miSINGLE, (unsigned) imgPFig.height(), 
                                  (unsigned) imgPFig.width(), 
                                  imgPFig.getData(), 
                                  tx, ty, bx, by, "pfig" );
  mw.writeVariable< int >( miINT32, tx+1, "pfig_xmn" );
  mw.writeVariable< int >( miINT32, ty+1, "pfig_ymn" );
  
  // Write pgnd
  findMinMax< float >( imgPGnd.height(), imgPGnd.width(), 
                       imgPGnd.getData(), 0, tx, ty, bx, by);
  mw.writeMatrix< float, float >(miSINGLE, (unsigned) imgPGnd.height(), 
                                 (unsigned) imgPGnd.width(),
                                 imgPGnd.getData(), 
                                 tx, ty, bx, by, "pgnd");
  mw.writeVariable< int >(miINT32, tx+1, "pgnd_xmn");
  mw.writeVariable< int >(miINT32, ty+1, "pgnd_ymn");
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
    
    for(unsigned i=0; i<vHypos.size(); i++ )
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
    
    for(unsigned i=0; i<vHypos.size(); i++ )
      ofile << vHypos[i].x << "  " << vHypos[i].y << "  " 
            << vHypos[i].nBoxX1 << "  " << vHypos[i].nBoxY1 << "  "
            << vHypos[i].nBoxWidth << "  " << vHypos[i].nBoxHeight << "  "
            << vHypos[i].dScale << "  " << vHypos[i].dScore << endl;
  }
}
