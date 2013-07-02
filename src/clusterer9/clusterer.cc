/*********************************************************************/
/*                                                                   */
/* FILE         clusterer.cc                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Main framework to test different clustering methods  */
/*              and to create codebooks used for object              */
/*              classification (in clmanager).                       */
/*              This program uses fast codebook generation (more     */
/*              memory).                                             */
/*                                                                   */
/* BEGIN        Fri Sep 27 2002                                      */
/* LAST CHANGE  Tue Jul 15 2003                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <qapplication.h>
#include <qfiledialog.h>
#include <qcolor.h>
#include <qpainter.h>
#include <qstringlist.h>
#include <qmessagebox.h>

#include <string>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

#include <qtcoordlabel.hh>
#include <qtimgbrowser.hh>
//#include <eimagebrowser.h>
#include <qtclusterview.hh>

#include <opgrayimage.hh>
#include <visualhistogram.hh>
#include <opharrisimage.hh>
#include <featurevector.hh>
#include <pca.hh>
#include <cluster.hh>
#include <clkmeans.hh>
#include <pyramidscalespace.hh>
#include <dogscalespace.hh>
#include <edgesift.hh>
#include <chamfermatching.h>

#include <resources.hh>
//#include "evaluate.hh"
//#include <clagglomerative.hh>
//#include <clsimagglomerative.hh>
//#include <clfastagglo.hh>
//#include <cldivconagglo.hh>
#include <clpostagglo.hh>
#include <clrnncagglo.hh>

#include "clusterer.hh"

#include <imgdescrlist.hh>

/*******************/
/*   Definitions   */
/*******************/

/*===================================================================*/
/*                         Class MyWidget                            */
/*===================================================================*/

/***********************************************************/
/*                          Slots                          */
/***********************************************************/

/*---------------------------------------------------------*/
/*                      Loading Images                     */
/*---------------------------------------------------------*/

void Clusterer::loadImage()
/*******************************************************************/
/* Load a new test image.                                          */
/*******************************************************************/
{
  m_qsLastImage = QFileDialog::getOpenFileName( m_qsLastImage,
                           "Images (*.png *.jpg *.ppm *.pgm);;All files (*.*)",
                                                this);
  if ( m_qsLastImage.isEmpty() )
    return;
  
  loadImage( m_qsLastImage );
}


void Clusterer::loadImage( QString name )
/*******************************************************************/
/* Load a new test image (including its segmentation mask, if one  */
/* exists).                                                        */
/*******************************************************************/
{
  cout << "  Clusterer::loadImage() called." << endl;
  m_img.load( name );
  
  /* if possible load the corresponding segmentation maps */
  QString nameOfMap( name );
  int posDir = nameOfMap.findRev( '/' );
  QString currentDir = nameOfMap.left( posDir+1 );
  nameOfMap = nameOfMap.right( nameOfMap.length() - posDir - 1 );
  QString mapDir("maps/");
  currentDir.append( mapDir );
  posDir = nameOfMap.findRev( '.' );
  
  if ( posDir >= 0 )
    nameOfMap = nameOfMap.left( posDir );
  QString fend("-map");
  nameOfMap.append( fend );
  QString ftype(".png");
  nameOfMap.append( ftype );
  
  currentDir.append( nameOfMap );
  QFile mapfile( currentDir );
  if ( mapfile.exists() == false ) {
    cout << "No corresponding segmentation map found." << endl;
    m_bMapsOn = false;

  } else {
    cout << "Corresponding segmentation map found." << endl;
    m_bMapsOn = true;
    QImage imgMap;
    imgMap.load( currentDir );
    OpGrayImage grayImgMap( imgMap );
    m_grayImgMap = grayImgMap;
  }
  
  /* store image name (abbreviated) */
  unsigned pos;
  string tmpstr( name.latin1() );
  if ( ( pos = tmpstr.rfind( "/" )) != string::npos ) {
    if ( tmpstr.at(pos+3) != (int) string::npos )
      tmpstr = tmpstr.substr( pos+1, 3);
    else
      tmpstr = "Img";

  } else
    tmpstr = "Img";
  m_sImgNameShort = tmpstr;
  
  /* store long image name */
  string sFileName( name.latin1() );
  int    len  = (int)sFileName.length();
  int    pos1 = (int)sFileName.rfind("/");
  int    pos2 = (int)sFileName.rfind("/", pos1-1);
  m_sImgName = "";
  m_sImgPath = "";
  m_sImgFullName = sFileName;
  if( pos1 != (int)string::npos ) {
    m_sImgName = sFileName.substr( pos1+1, len-pos1 );
    if( pos2 != (int)string::npos )
      m_sImgPath = sFileName.substr( pos2+1, pos1-pos2-1 );
  }

  /* convert to OpGrayImage structure */
  OpGrayImage grayImg( m_img );
  /* convert back to QImage structure */
  //QImage test_img = grayImg.getQtImage();
  m_grayImg = grayImg;
  
  /* display */
  rsSourceImg->loadImage( m_img );
  rsSourceImg->display();
  
  if ( m_bMapsOn ) {
    /* show the segmentation map instead of the gray scale image */
    QImage timg = m_grayImgMap.getQtImage();
    rsResultImg->loadImage( timg, m_grayImgMap );
  }
  else {
    rsResultImg->loadImage( m_img, grayImg );
  }
  
  rsResultImg->display();
  
  m_bPicLoaded = true;
  
  cout << "  done." << endl;
}


void Clusterer::loadImageBBox( QString qsName, const Rect &rBBox )
{
  // load the image
  loadImage( qsName );

  if(!m_bPicLoaded)
    return;

  // rescale it so that the object has uniform size
  cout << "  Object bbox: (" << rBBox.x1 << "," << rBBox.y1 
       << "," << rBBox.x2 << "," << rBBox.y2 << ")" << endl;
  int w = abs(rBBox.x1 - rBBox.x2);
  int h = abs(rBBox.y1 - rBBox.y2);
  
  if( w==0 || h==0 ) {
    cerr << "WARNING in Clusterer::loadImageBBox(): "
         << "bbox has size zero!" << endl;
    return;
  }

  float dFactor=1.0;
  if( m_nFixObjDim==OBJDIM_WIDTH )
    dFactor = ((float)m_nObjWidth/(float)w);
  else
    dFactor = ((float)m_nObjHeight/(float)h);

  // rescale the input image
  cout << "  Rescaling the image by a factor of " << dFactor << "..." << endl;
  m_grayImg = m_grayImg.opRescaleToWidth( m_grayImg.width()*dFactor );
  m_img = m_grayImg.getQtImage();

  // compute the new bbox in the rescaled image
  Rect rBBNew( round(rBBox.x1*dFactor), round(rBBox.y1*dFactor), 
               round(rBBox.x2*dFactor), round(rBBox.y2*dFactor), 0.0, -1 );

  // limit the bbox to lie fully inside the image
  int wnew = m_grayImg.width()-1;
  int hnew = m_grayImg.height()-1;
  if( rBBNew.x1<0.0 )  rBBNew.x1=0.0;
  if( rBBNew.x1>wnew ) rBBNew.x1=wnew;
  if( rBBNew.y1<0.0 )  rBBNew.y1=0.0;
  if( rBBNew.y1>hnew ) rBBNew.y1=hnew;
  if( rBBNew.x2<0.0 )  rBBNew.x2=0.0;
  if( rBBNew.x2>wnew ) rBBNew.x2=wnew;
  if( rBBNew.y2<0.0 )  rBBNew.y2=0.0;
  if( rBBNew.y2>hnew ) rBBNew.y2=hnew;

  cout << "  New object bbox: (" << rBBNew.x1 << "," << rBBNew.y1 
       << "," << rBBNew.x2 << "," << rBBNew.y2 << ")" << endl;

  if( m_bMapsOn ) {
    // if a segmentation map is available, also rescale it
    m_grayImgMap = m_grayImgMap.opRescaleToWidth( m_grayImg.width()*dFactor );

  } else {
    // create an artificial segmentation map with a blurred elliptical prior
    m_grayImgMap = OpGrayImage( m_grayImg.width(), m_grayImg.height() );
    
    // draw an ellipse
    int a = round(abs(rBBNew.x1 - rBBNew.x2)*0.5);
    int b = round(abs(rBBNew.y1 - rBBNew.y2)*0.5);
    m_grayImgMap.drawEllipse( (rBBNew.x1+rBBNew.x2)/2, 
                              (rBBNew.y1+rBBNew.y2)/2, a, b, 1.0 );

    // fill it 
    m_grayImgMap = m_grayImgMap.opAreaDistanceTransform();
    m_grayImgMap.opThresholdAbove( 1.0, 255.0, 0.0 );

    // blur it with a Gaussian of scale 0.05*{w|h}
    float dSigma;
    if( m_nFixObjDim==OBJDIM_WIDTH )
      dSigma = m_nObjWidth*0.05;
    else
      dSigma = m_nObjHeight*0.05;
    m_grayImgMap = m_grayImgMap.opGauss( dSigma );

    m_bMapsOn = true;
  }

  // display the rescaled image
  rsSourceImg->loadImage( m_img );
  rsSourceImg->drawRect( rBBNew.x1, rBBNew.y1, 
                         rBBNew.x2-rBBNew.x1, rBBNew.y2-rBBNew.y1, 
                         Qt::green );
  rsSourceImg->display();
  QImage timg = m_grayImgMap.getQtImage();
  rsResultImg->loadImage( timg, m_grayImgMap );
  rsResultImg->display();  
}


void Clusterer::loadImageSet()
/*******************************************************************/
/* Load a set of images from several different directories.        */
/*******************************************************************/
{
  m_vImageClass.clear();
  m_vFeatureClass.clear();
  m_vFeatureLoc.clear();
  m_vPoints.clear();
  m_vPointsInside.clear();
  m_cbCodebook.clear();
  
  QStringList qslFileList;
  QStringList qslFileListSeveral;
  QString     lastDir( DIR_IMAGES );
  qslFileListSeveral = QFileDialog::getOpenFileNames(
                           "Images (*.png *.ppm *.pgm *.jpg);;All files (*.*)",
                           lastDir, this, "imageset", "Select Images" );
  
  if (qslFileListSeveral.isEmpty())
    return;
  
  lastDir = qslFileListSeveral[0];
  cout << "  ...'" << qslFileListSeveral[0] << "'... [" 
       << qslFileListSeveral.size() << "]" << endl;
  
  /* add entries for the image class */
  int nClassId = 0;
  int nCount = (int)qslFileListSeveral.size();
  for( int i=0; i<nCount; i++ )
    m_vImageClass.push_back ( nClassId );

  bool ok = true;
  while( ok ) {
    // ask for more images
    switch( QMessageBox::information( this, STR_NAME.c_str(),
                                      "Load more images?",
                                      "&Yes", "&No", "&Cancel",
                                      0, 2 ) ) {
    case 0: // Yes clicked
      qslFileList = QFileDialog::getOpenFileNames(
                       "Images (*.png *.ppm *.pgm *.jpg);;All files (*.*)",
                       lastDir, this, "filedia", "Select More Files" );
      if ( !qslFileList.isEmpty() ) {
        lastDir = qslFileList[0];
        qslFileListSeveral += qslFileList; //Append
        
        /* add entries for the image class */
        nClassId++;
        int nAdded = (int)qslFileList.size();
        for( int i=0; i<nAdded; i++ )
          m_vImageClass.push_back ( nClassId );

        cout << "  ...'" << qslFileList[0] << "'... [" 
             << qslFileList.size() << "]" << endl;

      } else
        ok = false;
      break;
      
    case 1: // No clicked
      ok = false;
      break;
      
    case 2: // Cancel clicked
      ok = false;
      return;
      break;
    }
  }
  
  for(unsigned i=0; i < qslFileListSeveral.count(); i++) {
    int nFeaturesOld = (int)m_cbCodebook.getNumFeatures();
    cout<<" Processing the "<< i <<"of "<<qslFileListSeveral.count()<<"images "<<endl;
    processImage( qslFileListSeveral[i] );

    /* add entries for the feature class */
	// Need revise to include a background class!
    int nAdded = (int)m_cbCodebook.getNumFeatures() - nFeaturesOld;
    for( int j=0; j<nAdded; j++ )
      m_vFeatureClass.push_back ( m_vImageClass[i] );
  }  

  /*------------------------*/
  /* Normalize the features */
  /*------------------------*/
  m_cbCodebook.normalizeFeatures( m_fcCue.params()->m_nFeatureType );
  m_cbCodebook.loadFeatureClass(m_vFeatureClass);
  /*-------------------------------*/
  /* Display the extracted patches */
  /*-------------------------------*/
  vector<OpGrayImage> vImagePatches = m_cbCodebook.getImagePatches();
  displayPatchesForBrowsing( vImagePatches );
}


void Clusterer::loadIDLImageSet()
/*******************************************************************/
/* Load a new test image.                                          */
/*******************************************************************/
{
  m_qsLastIDL = QFileDialog::getOpenFileName( m_qsLastIDL,
                           "IDL files (*.idl);;All files (*.*)",
                                                this);
  if ( m_qsLastIDL.isEmpty() )
    return;
  
  loadIDLImageSet( m_qsLastIDL );
}


void Clusterer::loadIDLImageSet( const QString& qsIdlFile )
{
  m_vPoints.clear();
  m_vPointsInside.clear();
  m_vImageClass.clear();
  m_vFeatureClass.clear();
  m_vFeatureLoc.clear();
  m_cbCodebook.clear();
 
  /*-----------------------------------------*/
  /* Extract the path from the idl file name */
  /*-----------------------------------------*/
  string sFileName( qsIdlFile.latin1() );
  string sPath;
  unsigned pos = sFileName.rfind( "/" );
  if( pos != string::npos )
    sPath = sFileName.substr( 0, pos + 1 );

  /*-------------------*/
  /* Load the idl file */
  /*-------------------*/
  ImgDescrList list;
  list.load( qsIdlFile.latin1());
  
  /*--------------------*/
  /* Process all images */
  /*--------------------*/
  for(int i=0; i <(int)list.size(); i++) {
    string sName = sPath + list[i].sName;
    cout << "Processing image '" << sName.c_str() << "'..." << endl;
    for(int j=0; j<(int)list[i].vRectList.size(); j++)
      processImageBBox( sName.c_str(), list[i].vRectList[j] );
  }
    
  /*------------------------*/
  /* Normalize the features */
  /*------------------------*/
  m_cbCodebook.normalizeFeatures( m_fcCue.params()->m_nFeatureType );

  /*-------------------------------*/
  /* Display the extracted patches */
  /*-------------------------------*/
  vector<OpGrayImage> vImagePatches = m_cbCodebook.getImagePatches();
  displayPatchesForBrowsing( vImagePatches );
}


/*---------------------------------------------------------*/
/*                    Extracting Patches                   */
/*---------------------------------------------------------*/

void Clusterer::processImage( QString qstr )
/*******************************************************************/
/* Process a new image, i.e. load it and extract patches using the */
/* methods selected in the used interface. The extracted patches   */
/* are then stored in m_vImagePatches.                             */
/*******************************************************************/
{
  loadImage( qstr );
  qApp->processEvents(); // finish drawing
  //collectPatches( true );
  
  QImage qimg = m_grayImg.getQtImage();
  PointVector vPoints, vPointsInside;
  vector<OpGrayImage> vImagePatches;
  vector<FeatureVector> vFeatures;
  cout<<"   Start Processing image..."<<endl;
  m_fcCue.processImage( m_sImgFullName, m_grayImg, m_grayImgMap, qimg, 
                        m_nFeatureType, vPoints, vPointsInside, 
                        vImagePatches, vFeatures);
  
  //m_vPoints.insert      ( m_vPoints.end(), vPoints.begin(), vPoints.end() );
  //m_vPointsInside.insert( m_vPointsInside.end(), vPointsInside.begin(), 
  //                        vPointsInside.end() );

  /*----------------------------------*/
  /* Add the features to the codebook */
  /*----------------------------------*/
  m_vPoints       = vPoints;
  m_vPointsInside = vPointsInside;
  //m_vImagePatches.insert( m_vImagePatches.end(), vImagePatches.begin(), 
  //                        vImagePatches.end() );
  //m_vFeatures.insert    ( m_vFeatures.end(), vFeatures.begin(), 
  //                        vFeatures.end() );

  if( m_bUsePatches )
    m_cbCodebook.addFeatures( vFeatures, vImagePatches );
  else
    m_cbCodebook.addFeatures( vFeatures );

  m_vFeatureLoc.insert  ( m_vFeatureLoc.end(), vPointsInside.begin(), 
                          vPointsInside.end() );
  
  /*----------------------*/
  /* Draw interest points */
  /*----------------------*/
  drawInterestPointsEllipse();
  qApp->processEvents();
}


void Clusterer::processImageBBox( QString qstr, const Rect &rBBox )
/*******************************************************************/
/* Process a new image, i.e. load it and extract patches using the */
/* methods selected in the used interface. The extracted patches   */
/* are then stored in m_vImagePatches.                             */
/*******************************************************************/
{
  loadImageBBox( qstr, rBBox );
  qApp->processEvents(); // finish drawing
  //collectPatches( true );
  
  QImage qimg = m_grayImg.getQtImage();
  PointVector vPoints, vPointsInside;
  vector<OpGrayImage> vImagePatches;
  vector<FeatureVector> vFeatures;
  
  m_fcCue.processImage( m_sImgFullName, m_grayImg, m_grayImgMap, qimg, 
                        m_nFeatureType, vPoints, vPointsInside, 
                        vImagePatches, vFeatures);
  
  //m_vPoints.insert      ( m_vPoints.end(), vPoints.begin(), vPoints.end() );
  //m_vPointsInside.insert( m_vPointsInside.end(), vPointsInside.begin(), 
  //                        vPointsInside.end() );

  /*----------------------------------*/
  /* Add the features to the codebook */
  /*----------------------------------*/
  m_vPoints       = vPoints;
  m_vPointsInside = vPointsInside;
  //m_vImagePatches.insert( m_vImagePatches.end(), vImagePatches.begin(), 
  //                        vImagePatches.end() );
  //m_vFeatures.insert    ( m_vFeatures.end(), vFeatures.begin(), 
  //                        vFeatures.end() );

  if( m_bUsePatches )
    m_cbCodebook.addFeatures( vFeatures, vImagePatches );
  else
    m_cbCodebook.addFeatures( vFeatures );

  m_vFeatureLoc.insert  ( m_vFeatureLoc.end(), vPointsInside.begin(), 
                          vPointsInside.end() );
  
  /*----------------------*/
  /* Draw interest points */
  /*----------------------*/
  drawInterestPointsEllipse();
  qApp->processEvents();
}


void Clusterer::collectPatches( bool process )
/*******************************************************************/
/* Collect patches from the current image, either by uniform sam-  */
/* pling, or by applying an interest point detector.               */
/*******************************************************************/
{
  QImage qimg = m_grayImg.getQtImage();
  vector<OpGrayImage> vImagePatches;
  vector<FeatureVector> vFeatures;
  m_fcCue.processImage( m_sImgFullName, m_grayImg, m_grayImgMap, qimg, 
                        m_nFeatureType, m_vPoints, m_vPointsInside, 
                        vImagePatches, vFeatures );
  
  /*----------------------------------*/
  /* Add the features to the codebook */
  /*----------------------------------*/
  m_cbCodebook.clear();
  m_cbCodebook.addFeatures( vFeatures, vImagePatches, m_vFeatureClass );

  /*----------------------*/
  /* Draw interest points */
  /*----------------------*/
  drawInterestPointsEllipse();
  qApp->processEvents();
  
  /*------------------------*/
  /* Normalize the features */
  /*------------------------*/
  m_cbCodebook.normalizeFeatures( m_fcCue.params()->m_nFeatureType );

  /*-------------------------------*/
  /* Display the extracted patches */
  /*-------------------------------*/
  if (!process) {
    vector<OpGrayImage> vImagePatches = m_cbCodebook.getImagePatches();
    displayPatchesForBrowsing( vImagePatches );
  }
  qApp->processEvents();
}


void Clusterer::drawInterestPoints()
/*******************************************************************/
/* Draw the interest points returned by the Int.Pt. detector.      */
/*******************************************************************/
{
  if ( m_vPoints.empty() )
    return;
  
  // display original image first (clean any drawings)
  rsResultImg->display();
  for( int i=0; i <(int)m_vPoints.size(); i++) {
    rsResultImg->drawCross( m_vPoints[i].x, m_vPoints[i].y,
                            QColor::QColor(255,255,0) );
    if( m_vPoints[i].scale > 1.0 )
      rsResultImg->drawCircle( m_vPoints[i].x, m_vPoints[i].y,
                               m_vPoints[i].scale*3.0,
                               QColor::QColor(255,255,0) );
  }
  qApp->processEvents();
}


void Clusterer::drawInterestPointsEllipse()
/*******************************************************************/
/* Draw the interest points returned by the Int.Pt. detector.      */
/*******************************************************************/
{
  if ( m_vPoints.empty() )
    return;
 
  cout << "  Drawing " << m_vPoints.size() << " points..." << endl;
 
  // display original image first (clean any drawings)
  rsResultImg->display();
  for( int i=0; i <(int)m_vPoints.size(); i++) {
    rsResultImg->drawCross( m_vPoints[i].x, m_vPoints[i].y,
                            QColor::QColor(255,255,0), true );
    float dScaleFactor = m_fcCue.params()->m_dScaleFactor;
    if( m_vPoints[i].scale > 1.0 )
      if( m_vPoints[i].l1 != m_vPoints[i].l2 )
        rsResultImg->drawEllipse( m_vPoints[i].x, m_vPoints[i].y,
                                  m_vPoints[i].l1*dScaleFactor, 
                                  m_vPoints[i].l2*dScaleFactor,
                                  -m_vPoints[i].angle,
                                  QColor::QColor(255,255,0), true );
    else
      rsResultImg->drawCircle( m_vPoints[i].x, m_vPoints[i].y,
                               m_vPoints[i].scale*3.0,
                               QColor::QColor(255,255,0), true );
  }
  qApp->processEvents();
}


void Clusterer::displayPatchesForBrowsing(vector<OpGrayImage> &vPatches)
/*******************************************************************/
/* Display the extracted image patches in a separate window.       */
/*******************************************************************/
{
  if ( vPatches.empty() )
    return;
  vector<QImage> vPatchImages;
  
  for( int i=0; i <(int)vPatches.size(); i++) {
    vPatchImages.push_back( vPatches[i].getQtImage() );
  }

  QtImgBrowser *qtPatchBrowser = new QtImgBrowser( 0, "patches");
  qtPatchBrowser->setGeometry( 1150, 200, 100, 150 );
  qtPatchBrowser->load( vPatchImages );
  qtPatchBrowser->show();
}


/*---------------------------------------------------------*/
/*                        Clustering                       */
/*---------------------------------------------------------*/

void Clusterer::clusterPatches()
/*******************************************************************/
/* Start the clustering process.                                   */
/*******************************************************************/
{
  /* set the cursor to an hourglass */
  setCursor( waitCursor );
    
  /*********************************/
  /* Set the clustering parameters */
  /*********************************/
  m_cbCodebook.setClusterParams( m_parCluster );

  /************************/
  /* Start the clustering */
  /************************/
  m_cbCodebook.clusterPatches( m_fcCue.params()->m_nFeatureType );
    
  /************************/
  /* Display the Clusters */
  /************************/
  if( m_bUsePatches )
    m_cbCodebook.drawClusters( qClassView );
  
  
  /*******************************************/
  /* Create a Histogram of the Cluster Sizes */
  /*******************************************/
  int nNumClusters               = m_cbCodebook.getNumClusters();
  vector<int> vClusterAssignment = m_cbCodebook.getClusterAssignment();

  VisualHistogram hClSizes( 25, 1, 25 );
  vector<int> vSizes( nNumClusters );
  for( int i=0; i<(int)vClusterAssignment.size(); i++ )
    vSizes[vClusterAssignment[i]]++;
  for( int j=0; j<(int)vSizes.size(); j++ )
    hClSizes.insertValue( vSizes[j] );
  
  vector<QImage>  vQImgsSizes;
  QImage qimg;
  hClSizes.drawHistogram( qimg, 15 );
  vQImgsSizes.push_back( qimg );
  
  /* display the size histogram in a browser window */
  QtImgBrowser *qtClSizeBrowser = new QtImgBrowser( 0, "Cluster Sizes" );
  qtClSizeBrowser->setGeometry( 950, 0, 200, 250 );
  qtClSizeBrowser->load( vQImgsSizes );
  qtClSizeBrowser->show();
    
  /* reset the cursor back to normal */
  setCursor( arrowCursor );
  
  qApp->processEvents();
}


void Clusterer::removeClutteredPatches()
{
  /* set the cursor to an hourglass */
  setCursor( waitCursor );
    
  cout << "  Removing cluttered patches ..." << endl;
  cout << "    m_vFeatures.size(): " << m_cbCodebook.getNumFeatures() << endl;
  
  vector<FeatureVector> vFeatures = m_cbCodebook.getFeatures();
  vector<FeatureVector> vNewFeatures;
  vector<OpGrayImage> vImagePatches = m_cbCodebook.getImagePatches();
  vector<OpGrayImage> vNewImagePatches;;
  
  for( int i=0; i<(int)vFeatures.size(); i++) {
    float sum =0;
    FeatureVector& v = vFeatures[i];
    for(int j=0; j<v.numDims(); j++)
      sum += v.at(j);
    printf("    sum: %f\n", sum/255.0);
    if( sum/255.0>m_nClutterThresh )
      {
        vNewFeatures.push_back(v);
        vNewImagePatches.push_back(vImagePatches[i]);
      }
  }
  
  m_cbCodebook.clear();
  m_cbCodebook.addFeatures( vFeatures, vImagePatches );
  
  cout << "  done." << endl;
  cout << "    vFeatures.size(): " << vFeatures.size() << endl;

  /* reset the cursor back to normal */
  setCursor( arrowCursor );
  qApp->processEvents();
}


void Clusterer::remove1PatchClusters()
/*******************************************************************/
/* Remove all clusters that contain only a single patch.           */
/*******************************************************************/
{
  /* set the cursor to an hourglass */
  setCursor( waitCursor );
    
  m_cbCodebook.remove1PatchClusters();
  
  /*--------------------------*/
  /* Display the new clusters */
  /*--------------------------*/
  if( m_bUsePatches )
    m_cbCodebook.drawClusters( qClassView );

  /* reset the cursor back to normal */
  setCursor( arrowCursor );
  qApp->processEvents();
}


void Clusterer::loadCodebook()
/*******************************************************************/
/* Load a stored codebook and display it in the IconViewer window. */
/*******************************************************************/
{
  /*---------------------*/
  /* Ask for a file name */
  /*---------------------*/
  QString qsStartDir( DIR_CL_FVECS );
  QString qsFileName = QFileDialog::getOpenFileName( qsStartDir,
                   "Vectors (*.fls *.flz);;All files (*.*)",
                                                     this );
  if ( qsFileName.isEmpty() )
    return;

  int pos1 = qsFileName.findRev( ".fls" );
  int pos2 = qsFileName.findRev( ".flz" );
  if ( (pos1 == -1) && (pos2 == -1) ) {
    cout << "No valid file (*.fls or *.flz) selected." << endl;
    return;
  }
  
  loadCodebook( qsFileName.latin1() );
}


void Clusterer::loadCodebook( string sFileName )
/*******************************************************************/
/* Load a stored codebook and display it in the IconViewer window. */
/*******************************************************************/
{
  /* set the cursor to an hourglass */
  setCursor( waitCursor );
    
  /*-----------------------------------------------*/
  /* Prepare the file name and erase the extension */
  /*-----------------------------------------------*/
  string sRawName( sFileName );
  int pos = sRawName.rfind( "." );
  sRawName.erase( pos );

  /*-------------------------*/
  /* Load the parameter file */
  /*-------------------------*/
  cout << "  Loading parameters ..." << endl;
  string sParamName( sRawName + ".params" );
  loadParams( sParamName );
  cout << "  done." << endl;

  /*------------------------*/
  /* Load the main codebook */
  /*------------------------*/
  m_cbCodebook.loadCodebook( sFileName, m_fcCue, m_bUsePatches );

  /*--------------------------*/
  /* Load the feature classes */
  /*--------------------------*/
  string sClassFile( sRawName + ".cls" );
  loadFeatureClasses( sClassFile, m_vFeatureClass );

  /*----------------------*/
  /* Display the codebook */
  /*----------------------*/
  if( m_bUsePatches )
    m_cbCodebook.drawClusters( qClassView );

  /* reset the cursor back to normal */
  setCursor( arrowCursor );
  qApp->processEvents();
}


void Clusterer::appendCodebook()
/*******************************************************************/
/* Load a stored codebook and display it in the IconViewer window. */
/*******************************************************************/
{
  /*---------------------*/
  /* Ask for a file name */
  /*---------------------*/
  QString qsStartDir( DIR_CL_FVECS );
  QString qsFileName = QFileDialog::getOpenFileName( qsStartDir,
                   "Vectors (*.fls *.flz);;All files (*.*)",
                                                     this );
  if ( qsFileName.isEmpty() )
    return;

  int pos1 = qsFileName.findRev( ".fls" );
  int pos2 = qsFileName.findRev( ".flz" );
  if ( (pos1 == -1) && (pos2 == -1) ) {
    cout << "No valid file (*.fls or *.flz) selected." << endl;
    return;
  }
  appendCodebook( qsFileName.latin1() );
}


void Clusterer::appendCodebook( string sFileName )
/*******************************************************************/
/* Load a stored codebook and display it in the IconViewer window. */
/*******************************************************************/
{
  /* set the cursor to an hourglass */
  setCursor( waitCursor );
    
  /*-----------------------------------------------*/
  /* Prepare the file name and erase the extension */
  /*-----------------------------------------------*/
  string sRawName( sFileName );
  int pos = sRawName.rfind( "." );
  sRawName.erase( pos );

  /*-------------------------*/
  /* Load the parameter file */
  /*-------------------------*/
  cout << "  Loading parameters ..." << endl;
  string sParamName( sRawName + ".params" );
  loadParams( sParamName );
  cout << "  done." << endl;

  /*--------------------------*/
  /* Append the main codebook */
  /*--------------------------*/
  m_cbCodebook.appendCodebook( sFileName, m_fcCue, m_bUsePatches );

  /*----------------------------*/
  /* Append the feature classes */
  /*----------------------------*/
  string sClassFile( sRawName + ".cls" );
  vector<int> vFeatureClass;
  loadFeatureClasses( sClassFile, vFeatureClass );
  m_vFeatureClass.insert( m_vFeatureClass.end(), 
                          vFeatureClass.begin(), vFeatureClass.end() );

  /* reset the cursor back to normal */
  setCursor( arrowCursor ); 
  qApp->processEvents();
}


void Clusterer::saveCodebook()
/*******************************************************************/
/* Save the current codebook to disk (producing 2-3 files: a list. */
/* of feature vectors (*.flz), an ASCII file containing the clus-  */
/* ter assignments (*.ass), and an optional list of patch segmen-  */
/* tation masks (*.seg.flz, will only be saved if segmentations    */
/* were available for the training images!).                       */
/*******************************************************************/
{
  /*verify the current clustering state */
  int nNumFeatures = m_cbCodebook.getNumFeatures();
  cout << "Saving Codebook()... (" << nNumFeatures << " features)" <<endl;
  if ( nNumFeatures <= 0 ) {
    cerr << "  Saving codebook failed because current state isn't ok!"
         << endl;
    return;
  }
  
  /*---------------------*/
  /* Ask for a file name */
  /*---------------------*/
  QString qsStartDir( DIR_CL_FVECS );
  QString qsFileName;
  qsFileName = QFileDialog::getSaveFileName( qsStartDir,
                                       "Vectors (*.flz);;All files (*.*)",
                                           this );

  if( qsFileName.isEmpty() )
    return;
  
  saveCodebook( qsFileName.latin1() );
}


void Clusterer::saveCodebook( string sFileName )
/*******************************************************************/
/* Save the current codebook to disk (producing 2-3 files: a list. */
/* of feature vectors (*.flz), an ASCII file containing the clus-  */
/* ter assignments (*.ass), and an optional list of patch segmen-  */
/* tation masks (*.seg.flz, will only be saved if segmentations    */
/* were available for the training images!).                       */
/*******************************************************************/
{
  /* set the cursor to an hourglass */
  setCursor( waitCursor );
    
  /*-----------------------------------------------*/
  /* Prepare the file name and erase the extension */
  /*-----------------------------------------------*/
  string sRawName( sFileName );
  int pos = sRawName.rfind( "." );
  if( pos != (int)string::npos )
    sRawName.erase( pos );

  /*-------------------------*/
  /* Save the parameter file */
  /*-------------------------*/
  cout << "  Saving parameters ..." << endl;
  string sParamName( sRawName + ".params" );
  saveParams( sParamName );
  cout << "  done." << endl;

  /*------------------------*/
  /* Save the main codebook */
  /*------------------------*/
  m_cbCodebook.saveCodebook( sFileName, m_fcCue, m_bUsePatches );

  /*--------------------------*/
  /* Save the feature classes */
  /*--------------------------*/
  string sClassFile( sRawName + ".cls" );
  saveFeatureClasses( sClassFile );

  /* reset the cursor back to normal */
  setCursor( arrowCursor );
  qApp->processEvents();
}
  

void Clusterer::recreateCodebookFromTrace()
{
  /* set the cursor to an hourglass */
  setCursor( waitCursor );
    
  /* recreate the codebook */
  m_cbCodebook.recreateCodebookFromTrace( m_fcCue.params()->m_nFeatureType );

  /* display the codebook */
  if( m_bUsePatches )
    m_cbCodebook.drawClusters( qClassView );

  /* reset the cursor back to normal */
  setCursor( arrowCursor );
  qApp->processEvents();
}


/*---------------------------------------------------------*/
/*                    Eigenspace Functions                 */
/*---------------------------------------------------------*/

void Clusterer::computeFVEigenspace()
/*******************************************************************/
/* Compute an eigenspace from the original feature vectors.        */
/*******************************************************************/
{
  vector<FeatureVector> vFeatures = m_cbCodebook.getFeatures();
  
  vector<float> vEigVals;
  calcEigenVectors( vFeatures,
                    m_vEigenVectors, vEigVals, m_fvAvgImage );
  m_fvEigenValues = vEigVals;
  
  displayEigenspace( m_vEigenVectors, m_fvEigenValues, m_fvAvgImage );
}


void Clusterer::loadESMatlab()
/*******************************************************************/
/* Read in an Eigenspace in plain ASCII format (for use with Mat-  */
/* lab).                                                           */
/*******************************************************************/
{
  /***************************/
  /*   Read in eigenimages   */
  /***************************/
  QString qsFileName =
    QFileDialog::getOpenFileName( DIR_CL_FVECS,
                "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this, "diag", "Select 'Eigenimages' file" );
  if ( qsFileName.isEmpty() )
    return;
  
  if( !readMatrix( qsFileName.latin1(), m_vEigenVectors ) )
    {
      cerr << "ERROR: Couldn't open 'eigenimages' file!" << endl;
      return;
    }

  /***************************/
  /*   Read in eigenvalues   */
  /***************************/
  qsFileName =
    QFileDialog::getOpenFileName( qsFileName,
              "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this, "diag", "Select 'Eigenvalues' file" );
  if ( qsFileName.isEmpty() )
    return;
  
  if( !readVector( qsFileName.latin1(), m_fvEigenValues ) )
    {
      cerr << "ERROR: Couldn't open 'eigenvalues' file!" << endl;
      return;
    }
  
  /*************************/
  /*   Read in avg image   */
  /*************************/
  qsFileName =
    QFileDialog::getOpenFileName( qsFileName,
                "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this, "diag", "Select 'Average Image' file" );
  if ( qsFileName.isEmpty() )
    return;
  
  if( !readVector( qsFileName.latin1(), m_fvAvgImage ) )
    {
      cerr << "ERROR: Couldn't open 'avg image' file!" << endl;
      return;
    }
  
  /******************************/
  /*   Display the Eigenspace   */
  /******************************/
  displayEigenspace( m_vEigenVectors, m_fvEigenValues, m_fvAvgImage );
}


void Clusterer::displayEigenspace( const vector<FeatureVector> &vEigenVectors,
                                   const FeatureVector &fvEigenValues,
                                   const FeatureVector &fvAvgImage )
/*******************************************************************/
/* Display an eigenspace on the user interface.                    */
/*******************************************************************/
{
  /* create images from the eigenvectors */
  vector<QImage>      vQImgsEigVecs;
  vector<OpGrayImage> vImgsEigVecs;
  OpGrayImage img;
  for( int i=0; i<min(100,(int)vEigenVectors.size()); i++ ) {
    int nSize = (int) sqrt( (float) vEigenVectors[i].numDims() );
    img.loadFromData( nSize, nSize, vEigenVectors[i].getData() );
    vQImgsEigVecs.push_back( img.getQtImage() );
    vImgsEigVecs.push_back( img );
  }
  
  /* create a plot from the eigenvalues */
  vector<QImage>  vQImgsEigVals;
  QImage qimg;
  VisualHistogram hEigVals( fvEigenValues.numDims(), 0.0,
                            fvEigenValues.numDims() );
  hEigVals.setData( fvEigenValues.getData() );
  hEigVals.drawHistogram( qimg );
  vQImgsEigVals.push_back( qimg );
  
  /* create an image from the avg image */
  vector<QImage>      vQImgsAvgImg;
  vector<OpGrayImage> vImgsAvgImg;
  int nSize = (int) sqrt( (float) fvAvgImage.numDims() );
  img.loadFromData( nSize, nSize, fvAvgImage.getData() );
  vQImgsAvgImg.push_back( img.getQtImage() );
  vImgsAvgImg.push_back( img );
  
  
  /* display the eigenvectors in a browser window */
  QtImgBrowser *qtEigVecBrowser = new QtImgBrowser( 0, "Eigenvectors" );
  qtEigVecBrowser->setGeometry( 950, 0, 200, 250 );
  qtEigVecBrowser->load( vQImgsEigVecs, vImgsEigVecs );
  qtEigVecBrowser->show();
  
  /* display the eigenvalues in a browser window */
  QtImgBrowser *qtEigValBrowser = new QtImgBrowser( 0, "Eigenvalues" );
  qtEigValBrowser->setGeometry( 950, 250, 200, 250 );
  qtEigValBrowser->load( vQImgsEigVals );
  qtEigValBrowser->show();
  
  /* display the eigenvectors in a browser window */
  QtImgBrowser *qtAvgImgBrowser = new QtImgBrowser( 0, "Avg Image" );
  qtAvgImgBrowser->setGeometry( 950, 500, 200, 250 );
  qtAvgImgBrowser->load( vQImgsAvgImg, vImgsAvgImg );
  qtAvgImgBrowser->show();
}


/*---------------------------------------------------------*/
/*                          File I/O                       */
/*---------------------------------------------------------*/

void Clusterer::savePatchesMatlab()
/*******************************************************************/
/* Save the extracted patches to disk in plain ASCII format (to be */
/* read in by Matlab). Each line contains one codebook entry.      */
/*******************************************************************/
{
  QString qsFileName =
    QFileDialog::getSaveFileName( DIR_CL_FVECS,
                "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this);
  if ( qsFileName.isEmpty() )
    return;
  
  m_cbCodebook.savePatchesASCII( qsFileName.latin1() );
}


void Clusterer::saveFeaturesMatlab()
/*******************************************************************/
/* Save the extracted features to disk in plain ASCII format (to   */
/* be read in by Matlab). Each line contains one codebook entry.   */
/*******************************************************************/
{
  QString qsFileName =
    QFileDialog::getSaveFileName( DIR_CL_FVECS,
                "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this);
  if ( qsFileName.isEmpty() )
    return;
  
  m_cbCodebook.saveFeaturesASCII( qsFileName.latin1() );
}


void Clusterer::saveFeatureLocations()
/*******************************************************************/
/* Save the feature locations to disk in plain ASCII format (to    */
/* be read in by Matlab). Each line contains one codebook entry.   */
/*******************************************************************/
{
  QString qsFileName =
    QFileDialog::getSaveFileName( DIR_CL_FVECS,
                "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this);
  if ( qsFileName.isEmpty() )
    return;
  
  saveFeatureLocations( qsFileName.latin1() );
}


void Clusterer::saveFeatureLocations( string sFileName )
/*******************************************************************/
/* Save the extracted features to disk in plain ASCII format (to   */
/* be read in by Matlab). Each line contains one codebook entry.   */
/*******************************************************************/
{
  cout << "  Saving feature locations..." << endl;

  ofstream ofile( sFileName.c_str() );
  if( !ofile ) {
    cerr << "ERROR: Couldn't open file " << sFileName << "!" << endl;
    return;
  }
  
  ofile << m_vFeatureLoc.size() << endl;
  for( int i=0; i<(int)m_vFeatureLoc.size(); i++ )
    ofile << m_vFeatureLoc[i].x << " " << m_vFeatureLoc[i].y << " "
          << m_vFeatureLoc[i].scale << " " << m_vFeatureLoc[i].angle << " "
          << m_vFeatureLoc[i].l1 << " " << m_vFeatureLoc[i].l2 << endl;
  ofile.close();

  cout << "  done." << endl;
}


void Clusterer::loadFeatureClasses( string sFileName, 
                                    vector<int> &vFeatureClass )
/*******************************************************************/
/* Load the feature-class assignments from disk (in plain ASCII    */
/* format).                                                        */
/*******************************************************************/
{
  cout << "  Loading feature-class assignments..." << endl;

  ifstream ifile( sFileName.c_str() );
  if ( !ifile ) {
    cerr <<"Can't open input file: " << sFileName << endl;
    return;
  } 
  
  vFeatureClass.clear();
  int nNumClassEntries;
  ifile >> nNumClassEntries;
  for(int i=0; i <nNumClassEntries; i++) {
    int nClass;
    ifile >> nClass;
    vFeatureClass.push_back( nClass );
  }
  ifile.close();

  cout << "  done." << endl;
}


void Clusterer::saveFeatureClasses()
/*******************************************************************/
/* Save the feature-class assignments to disk in plain ASCII for-  */
/* mat (to be read in by Matlab).                                  */
/*******************************************************************/
{
  QString qsFileName =
    QFileDialog::getSaveFileName( DIR_CL_FVECS,
                "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this);
  if ( qsFileName.isEmpty() )
    return;
  
  saveFeatureClasses( qsFileName.latin1() );
}


void Clusterer::saveFeatureClasses( string sFileName )
/*******************************************************************/
/* Save the feature-class assignments to disk (in plain ASCII for- */
/* mat).                                                           */
/*******************************************************************/
{
  cout << "  Saving feature-class assignments..." << endl;

  ofstream ofile( sFileName.c_str() );
  if( !ofile ) {
    cerr << "ERROR: Couldn't open file " << sFileName << "!" << endl;
    return;
  }
  
  ofile << m_vFeatureClass.size() << endl;
  for( int i=0; i<(int)m_vFeatureClass.size(); i++ )
    ofile << m_vFeatureClass[i]<< " ";
  ofile << endl;
  ofile.close();

  cout << "  done." << endl;
}


void Clusterer::saveClustersMatlab()
  /*******************************************************************/
  /* Save the codebook entries to disk in plain ASCII format (to be  */
  /* read in by Matlab). Each line contains one codebook entry.      */
  /*******************************************************************/
{
  QString qsFileName =
    QFileDialog::getSaveFileName( DIR_CL_FVECS,
                                  "Matlab Files (*.tab);;Vectors (*.fls *.flz);;All files (*.*)",
                                  this);
  if ( qsFileName.isEmpty() )
    return;

  m_cbCodebook.saveClustersASCII( qsFileName.latin1() );
}


void Clusterer::saveClusterTrace()
{
  /*---------------------*/
  /* Ask for a file name */
  /*---------------------*/
  QString qsStartDir( DIR_CL_FVECS );
  QString qsFileName;
  qsFileName = QFileDialog::getSaveFileName( qsStartDir,
                                     "Cluster Traces (*.trc);;All files (*.*)",
                                           this );

  if( qsFileName.isEmpty() )
    return;
  
  m_cbCodebook.saveClusterTrace( qsFileName.latin1(), true );
}


void Clusterer::saveClusteredImages()
/*******************************************************************/
/* Save all cluster centers and all corresponding feature vectors  */
/* as images. The routine opens a file dialog to ask for a target  */
/* directory. All cluster centers and their assigned image patches */
/* are then written to this directory as images.                   */
/*******************************************************************/
{  
  /*---------------------*/
  /* Ask for a file name */
  /*---------------------*/
  QString qsStartDir( QDir::homeDirPath() );

  QString qsFileName = QFileDialog::getSaveFileName( qsStartDir,
                   "Images (*.png *.ppm *.pgm *.jpg);;All files (*.*)");

  if ( qsFileName.isEmpty() ) 
    return;


  m_cbCodebook.saveClusterImages( qsFileName.latin1() );
}


/***********************************************************/
/*                    Service Functions                    */
/***********************************************************/

bool Clusterer::readVector( string sFileName, FeatureVector &fvVector )
/*******************************************************************/
/* Read in a vector in plain ASCII format from the given file name */
/*******************************************************************/
{
  ifstream ifile;
  
  ifile.open( sFileName.c_str() );
  if( !ifile )
    {
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
  for( int x=0; x<dim; x++ )
    {
      float val;
      ifile >> val;
      fvVector.setValue( x, val );
    }
  cout << "  done." << endl;
  return true;
}


bool Clusterer::readMatrix ( string sFileName, vector<FeatureVector> &mMatrix )
/*******************************************************************/
/* Read in a matrix in plain ASCII format from the given file name */
/*******************************************************************/
{
  ifstream ifile;
  
  ifile.open( sFileName.c_str() );
  if( !ifile )
    {
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
  for( int y=0; y<h; y++ )
    {
      FeatureVector fvNew( w );
      for( int x=0; x<w; x++ )
        {
          float val;
          ifile >> val;
          fvNew.setValue( x, val );
        }
      mMatrix.push_back( fvNew );
    }
  cout << "  done." << endl;
  return true;
}

