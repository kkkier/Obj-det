/*********************************************************************/
/*                                                                   */
/* FILE         featurecue.cc                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Class for feature extraction.                        */
/*                                                                   */
/* BEGIN        Thu Mar 25 2004                                      */
/* LAST CHANGE  Tue Feb 20 2013(Yuren Zhang)                         */
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
#include <cassert>
#include <sys/time.h>
#include <time.h>

#include <imageoperations.h>
#include <qtimgbrowser.hh>
#include <opharrisimage.hh>
#include <dogscalespace.hh>
#include <edgesift.hh>
#include "siftdetector.hh"

#include "featurecue.hh"

/*******************/
/*   Definitions   */
/*******************/

/*===================================================================*/
/*                         Class FeatureCue                          */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

FeatureCue::FeatureCue()
{
  m_guiParams=0;
  
  /* seed the random number generator */
  timeval time;
  gettimeofday( &time, NULL );
  //cout << "    TIME: " << time.tv_sec << "s, " 
  //     << time.tv_usec << "ms" << endl;
  srandom( time.tv_usec );
}


/***********************************************************/
/*                          Slots                          */
/***********************************************************/

FeatureGUI* FeatureCue::createGUI( QWidget *parent, const char* name )
{
  if( m_guiParams != 0 )
    {
      cerr << "  Error in FeatureCue::createGUI(): "
           << "Tried to initialize the GUI twice!" << endl;
      return 0;
      
    }
  else
    {
      cout << "  Initializing feature GUI for window '" << name << "'..."
           << endl;
      m_guiParams = new FeatureGUI( parent, name );
    }
  
  /* seed the random number generator */
  timeval time;
  gettimeofday( &time, NULL );
  //cout << "    TIME: " << time.tv_sec << "s, " << time.tv_usec << "ms" 
  //     << endl;
  srandom( time.tv_usec );

  return m_guiParams;
}


FeatureGUI* FeatureCue::params() const
{
  assert( m_guiParams != 0 );
  
  return m_guiParams;
}


/*---------------------------------------------------------*/
/*                    Extracting Patches                   */
/*---------------------------------------------------------*/
void FeatureCue::processImage( const OpGrayImage& img, const OpGrayImage& map,
                               const QImage& qimg,
                               int                   &nFeatureType,
                               PointVector           &vPoints,
                               PointVector           &vPointsInside,
                               vector<OpGrayImage>   &vPatches,
                               vector<FeatureVector> &vFeatures,
                               bool bVerbose, bool bExtractPatches )
/*******************************************************************/
/* Process a new image, i.e. load it and extract patches using the */
/* methods selected in the used interface. The extracted patches   */
/* are then stored in m_vImagePatches.                             */
/*******************************************************************/
{
  //cerr << "processImage" << endl;
  
  //--- copy images to members ---//
  m_imgSrc  = img;
  m_qimgSrc = qimg;
  m_imgSrcMap = map;
  
  // verify that the map contains useful data
  int w = img.width();
  int h = img.height();
  if( (map.width()!=w) && (map.height()!=h) ) {
    OpGrayImage imgTmp( w, h );
    for( int y=0; y<h; y++ )
      for( int x=0; x<w; x++ )
        imgTmp(x,y) = 255.0;
    
    m_imgSrcMap = imgTmp;
  }

  //--- clear results from previous image ---//
  m_vPoints.clear();
  m_vPointsInside.clear();
  m_vPatches.clear();
  m_vFeatures.clear();
  m_vPtIdzs.clear();
  cout<<"    Start collecting patches..."<<endl;  
  collectPatches( "", bVerbose, bExtractPatches );
  
  nFeatureType  = m_nFeatureType;
  vPoints       = m_vPoints;
  vPointsInside = m_vPointsInside;
  vPatches      = m_vPatches;
  vFeatures     = m_vFeatures;
}


void FeatureCue::processImage( string sImgName, 
                               const OpGrayImage& img, const OpGrayImage& map,
                               const QImage& qimg,
                               int                   &nFeatureType,
                               PointVector           &vPoints,
                               PointVector           &vPointsInside,
                               vector<OpGrayImage>   &vPatches,
                               vector<FeatureVector> &vFeatures,
                               bool bVerbose, bool bExtractPatches )
/*******************************************************************/
/* Process a new image, i.e. load it and extract patches using the */
/* methods selected in the used interface. The extracted patches   */
/* are then stored in m_vImagePatches.                             */
/*******************************************************************/
{
  cerr << "processImage" << endl;
  
  //--- copy images to members ---//
  m_imgSrc  = img;
  m_qimgSrc = qimg;
  m_imgSrcMap = map;
  
  // verify that the map contains useful data
  int w = img.width();
  int h = img.height();
  cout<<" width = "<<w<<"  height"<<h<<endl;
  if( (map.width()!=w) && (map.height()!=h) ) {
    OpGrayImage imgTmp( w, h );
    for( int y=0; y<h; y++ )
      for( int x=0; x<w; x++ )
        imgTmp(x,y) = 255.0;
    
    m_imgSrcMap = imgTmp;
  }

  //--- clear results from previous image ---//
  m_vPoints.clear();
  m_vPointsInside.clear();
  m_vPatches.clear();
  m_vFeatures.clear();
  m_vPtIdzs.clear();
  cout<<"  Collecting patches"<<endl; 
  collectPatches( sImgName, bVerbose, bExtractPatches );
  
  nFeatureType  = m_nFeatureType;
  vPoints       = m_vPoints;
  vPointsInside = m_vPointsInside;
  vPatches      = m_vPatches;
  vFeatures     = m_vFeatures;
}


void FeatureCue::getMirroredFeatures( const OpGrayImage& img, 
                                      const OpGrayImage& imgMap, 
                                      const PointVector& vPointsInside,
                                      const vector<FeatureVector>& vFeatures,
                                      PointVector&           vPointsLeft,
                                      vector<FeatureVector>& vFeaturesLeft,
                                      bool bVerbose )
/*******************************************************************/
/* Collect the interest points and features from the mirrored im-  */
/* age, using the original features.                               */
/*******************************************************************/
{
  //cout << "FeatureCue::getMirroredFeatures() called... " << endl;
  //--- copy images to members ---//
  m_imgSrc    = img.flipHorizontal();
  m_qimgSrc   = m_imgSrc.getQtImage();
  if( imgMap.width()==img.width() && imgMap.height()==img.height() )
    m_imgSrcMap = imgMap.flipHorizontal();
  else {
    m_imgSrcMap = OpGrayImage(img.width(),img.height());
    m_imgSrcMap.opThresholdAbove(-1.0,255.0,0.0 );
  }
 
  if( m_guiParams->m_nPatchExtMethod==PATCHEXT_EDGELAP ) {
    // for edgelap features, call the full binary 
  m_vPoints.clear();
  m_vPointsInside.clear();
  m_vPatches.clear();
  m_vFeatures.clear();
  m_vPtIdzs.clear();
  
  //cout << "  Calling collectPatches() with mirrored image..." << endl;
  string sImgName="";
  collectPatches( sImgName, bVerbose );
  //cout << "  done." << endl;
  
  vPointsLeft   = m_vPointsInside;
  vFeaturesLeft = m_vFeatures;    

  } else {
    // mirror the interest points
    int w = img.width();
    vPointsLeft = vPointsInside;
    for(unsigned i=0; i<vPointsLeft.size(); i++ ) {
      vPointsLeft[i].x     = w - vPointsLeft[i].x - 1;
      vPointsLeft[i].angle = -vPointsLeft[i].angle;
    }
    
    vFeaturesLeft.clear();
    if( m_nFeatureType==FEATURE_SURF64 ) {
      // mirror the already extracted descriptors
      vFeaturesLeft = vFeatures;
      m_vPtIdzs.clear();
      for(unsigned i=0; i<vFeaturesLeft.size(); i++ ) {
        for( int d=0; d<vFeaturesLeft[i].numDims(); d+=4 )
          vFeaturesLeft[i].at(d) = -vFeaturesLeft[i].at(d);
        m_vPtIdzs.push_back( i );
      }
      
    } else {
      // extract the mirrored descriptors
      m_vPoints = vPointsLeft;
      m_vPointsInside.clear();
      m_vPatches.clear();
      m_vFeatures.clear();
      m_vPtIdzs.clear();
      
      extractAllPatches( false, bVerbose );
      
      vPointsLeft   = m_vPointsInside;
      vFeaturesLeft = m_vFeatures;
    }
  }
  //cout << "done." << endl;
}


void FeatureCue::getDescriptors( const OpGrayImage& img, 
                                 const OpGrayImage& imgMap, 
                                 const PointVector& vPoints,
                                 PointVector&       vPointsSampled,
                                 vector<int>&       vPtIdzs,
                                 vector<FeatureVector>& vFeatures,
                                 bool bVerbose )
/*******************************************************************/
/* Collect the descriptors from the given interest points.         */
/*******************************************************************/
{
  //--- copy images to members ---//
  m_imgSrc    = img;
  m_qimgSrc   = m_imgSrc.getQtImage();
  m_imgSrcMap = imgMap;
  
  // extract the descriptors
  m_vPoints = vPoints;
  m_vPointsInside.clear();
  m_vPatches.clear();
  m_vFeatures.clear();
  m_vPtIdzs.clear();
  
  extractAllPatches( false, bVerbose );
  
  vPointsSampled = m_vPointsInside;
  vFeatures      = m_vFeatures;
  vPtIdzs        = m_vPtIdzs;
}


/*---------------------------------------------------------*/
/*                Interest Region Extraction               */
/*---------------------------------------------------------*/

void FeatureCue::collectPatches( string sImgName, bool bVerbose, 
                                 bool bExtractPatches )
/*******************************************************************/
/* Collect patches from the current image, either by uniform sam-  */
/* pling, or by applying a Harris corner detector.                 */
/*******************************************************************/
{
  m_guiParams->m_dScaleFactor = SCALE_FACTOR_STD;
  if( bVerbose )
    cout << "  Collecting Patches with method #" 
         << m_guiParams->m_nPatchExtMethod << endl;

  /* enforce that SURF features are always extracted on SURF regions */
//   if( ((m_guiParams->m_nFeatureType==FEATURE_SURF64) || 
//        (m_guiParams->m_nFeatureType==FEATURE_SURF128)) &&
//       (m_guiParams->m_nPatchExtMethod!=PATCHEXT_SURF) ) {
//     cerr << "    Error in FeatureCue::CollectPatches(): "
//          << "SURF features can currently only be computed on SURF regions!"
//          << endl;
//     return;
//   }

  bool bFeaturesExtracted = false;
  switch( m_guiParams->m_nPatchExtMethod )
    {
    case PATCHEXT_UNIFORM:
      collectUniformPoints();
      break;
      
    case PATCHEXT_HARRIS:
      m_guiParams->m_dScaleFactor = SCALE_FACTOR_HARRIS;
      applyHarris( bVerbose );
      break;
      
    case PATCHEXT_HARLAP:
    case PATCHEXT_HESLAP:
    case PATCHEXT_HARHESLAP:
    case PATCHEXT_HARAFF:
    case PATCHEXT_HESAFF:
      //if( sImgName.empty() || (m_guiParams->m_nFeatureType!=FEATURE_GLOH) )
      applyMikolajczyk( m_guiParams->m_nPatchExtMethod, bVerbose );
        //else {
        //  applyMikoBoth( sImgName, 
        //                 m_guiParams->m_nPatchExtMethod,
        //                 m_guiParams->m_nFeatureType,
        //                 m_vPoints, m_vFeatures );
        //  bFeaturesExtracted = true;
        //}
      break;
   
    case PATCHEXT_LDOG:
      applyLowe( bVerbose );
      break;
      
    case PATCHEXT_EXACTDOG:
      applyExactDoG( bVerbose );
      break;
      
    case PATCHEXT_MSER:
      applyMatas( bVerbose );
      break;
      
    case PATCHEXT_MSER2:
      applyMatasUnaffine( bVerbose );
      break;

    case PATCHEXT_SURF:
      applySURF( m_guiParams->m_nFeatureType, m_vPoints, m_vFeatures, 
                 bVerbose );
      if( (m_guiParams->m_nFeatureType==FEATURE_SURF64) ||
          (m_guiParams->m_nFeatureType==FEATURE_SURF128) )
        bFeaturesExtracted = true;
      break;

    case PATCHEXT_EDGELAP:
      if( (m_guiParams->m_nFeatureType==FEATURE_SURF64) ||
          (m_guiParams->m_nFeatureType==FEATURE_SURF128) )
          applyMikolajczyk( m_guiParams->m_nPatchExtMethod, bVerbose );
        else {
          applyMikoBothNew( sImgName, 
                            m_guiParams->m_nPatchExtMethod,
                            m_guiParams->m_nFeatureType,
                            m_vPoints, m_vFeatures );
          bFeaturesExtracted = true;
        }
      break;
	case PATCHEXT_RANDOM:
	  collectRandomPoints( 250, 250 );
	  break;
   
    default:
      cerr << "    Error in FeatureCue::CollectPatches(): "
           << "Unknown patch extraction method ("
           << m_guiParams->m_nPatchExtMethod
           << ")!" << endl;
    }
  if(m_guiParams->m_nPatchExtMethod == PATCHEXT_RANDOM)
	
	  // NEW: extract from both background and the object region, currently 
	  // only used for random sampling
	  // TODO: extend to all methods
	  extractAllPatches_OBJnBKG( bFeaturesExtracted, bVerbose, bExtractPatches ); 
  else
	 extractAllPatches( bFeaturesExtracted, bVerbose, bExtractPatches );
int miny = m_guiParams->m_nPatchSize;
	int maxx = m_imgSrc.width()-m_guiParams->m_nPatchSize;
	int maxy = m_imgSrc.height()- m_guiParams->m_nPatchSize;
    
	int x = 0;
	int y = 0;
	int in = 0;
	int out = 0;

    bool bStop = false;
    if(m_guiParams->m_bExtractFromWholeImage)
	// Extract dense samples from the whole image, for testing 
	// Ignore insiders and outsiders, extract according to the 
	// average samples per area
    {
		int total = (int)(maxx - minx)*(maxy - miny)*DEFAULTSAMPLEPERAREA;
        while(!bStop)
		{
			x = random() % (maxx - minx) + minx;
			y = random() % (maxy - miny) + miny;
			InterestPoint ptNew;
			ptNew.x = x;
			ptNew.y = y;
			ptNew.value = 1.0;
			ptNew.scale = 1.0;
			ptNew.l1 = 17;
			ptNew.l2 = 17;
			m_vPoints.push_back(ptNew);
			in++;
			if( in>=total )
				bStop = true;	
		}
    }// end if UseFigureOnly
    else
    // Extract positive and negative samples from the object
    // and the background
    {
	// It is possible that there is no outsiders
	if((maxx-minx)*(maxy-miny) < m_imgSrcMap.getSum()/255.0)
		out = outsiders;
	while(!bStop)
	{
		x = random() % (maxx - minx) + minx;
		y = random() % (maxy - miny) + miny;
		InterestPoint ptNew;
		ptNew.x = x;
		ptNew.y = y;
		ptNew.value = 1.0;
		ptNew.scale = 1.0;
		ptNew.l1 = 17;
		ptNew.l2 = 17;
		if(m_imgSrcMap(x ,y).value() > 0 && in<insiders)
		{
			in++;
			m_vPoints.push_back(ptNew);
		}
		else if(m_imgSrcMap(x, y).value()==0 && out<outsiders)
		{
			out++;
			m_vPoints.push_back(ptNew);
		}
		if(in>=insiders && out>=outsiders)
			bStop = true;
	}
    }// end else UseFigureOnly
}
void FeatureCue::collectUniformPoints()
/*******************************************************************/
/* Collect points from a region of the image by sampling a uniform */
/* grid with a given step size.                                    */
/*******************************************************************/
{
  m_vPoints.clear();
  int minx = m_guiParams->m_nPatchSize;
  int miny = m_guiParams->m_nPatchSize;
  int maxx = m_imgSrc.width()-m_guiParams->m_nPatchSize;
  int maxy = m_imgSrc.height()- m_guiParams->m_nPatchSize;
  for (int y=miny; y < maxy; y+= m_guiParams->m_nStepSize)
    for (int x=minx; x < maxx; x+=m_guiParams->m_nStepSize)
      {
        InterestPoint ptNew;
        ptNew.x = x;
        ptNew.y = y;
        ptNew.value = 1.0;
        m_vPoints.push_back( ptNew );
      }
}


void FeatureCue::collectUniformPoints( int x1, int y1, int x2, int y2 )
/*******************************************************************/
/* Collect points from a region of the image by sampling a uniform */
/* grid with a given step size.                                    */
/*******************************************************************/
{
  m_vPoints.clear();
  int minx = max( x1, m_guiParams->m_nPatchSize );
  int miny = max( y1, m_guiParams->m_nPatchSize );
  int maxx = min( x2, m_imgSrc.width()-m_guiParams->m_nPatchSize );
  int maxy = min( y2, m_imgSrc.height()-m_guiParams->m_nPatchSize );
  for (int y=miny; y < maxy; y+=m_guiParams->m_nStepSize)
    for (int x=minx; x < maxx; x+=m_guiParams->m_nStepSize)
      {
        InterestPoint ptNew;
        ptNew.x = x;
        ptNew.y = y;
        ptNew.value = 1.0;
        ptNew.angle = 0.0;
        m_vPoints.push_back( ptNew );
      }
}


void FeatureCue::applyHarris( bool bVerbose )
/*******************************************************************/
/* Collect points from the image by applying a Harris corner de-   */
/* tector.                                                         */
/*******************************************************************/
{
  OpInterestImage     interest;
  
  OpHarrisImage imgHarris(m_imgSrc);
  interest = imgHarris.opImprovedHarris( m_guiParams->m_dSigma1,
                                         m_guiParams->m_dSigma2,
                                         m_guiParams->m_dAlpha );
  interest = interest.opNonMaximumSuppression( 3 );
  //interest = interest.opStrongestPercentage( 0.01 );
  interest = interest.opThreshold( m_guiParams->m_dThreshHar );
  
  m_vPoints = interest.getInterestPointVector();
}


void FeatureCue::applyMikolajczyk( int nDetector, bool bVerbose )
/*******************************************************************/
/* Collect points from the image by applying Mikolajczyk's inte-   */
/* rest point detectors (calling an external program).             */
/*******************************************************************/
{
  int nRandom = random() % 100000;
  string sRandom   = string( QString::number(nRandom).latin1() );
  string sPath     = PATH_MIKO;
  string sCommand  = "h_affine.ln ";
  string sTmpName  = string("./tmp") + sRandom + ".pgm";
  string sFileName = string("./result") + sRandom + ".key";
  switch( nDetector )
    {
    case PATCHEXT_HARLAP:
      /* apply the Multiscale Harris detector */
      sCommand = sCommand + "-harlap ";
      break;
      
    case PATCHEXT_HESLAP:
      /* apply the Fast Multiscale Harris detector */
      sCommand = sCommand + "-heslap ";
      break;
      
    case PATCHEXT_HARHESLAP:
      /* apply the Multiscale Harris+Hessian detector */
      sCommand = sCommand + "-harhes ";
      break;
      
    case PATCHEXT_HARAFF:
      /* apply the Multiscale Harris detector */
      sCommand = sCommand + "-haraff ";
      break;
      
    case PATCHEXT_HESAFF:
      /* apply the Fast Multiscale Harris detector */
      sCommand = sCommand + "-hesaff ";
      break;
      
    default:
      cerr << "  Error in FeatureCue::applyMikolajczyk(): "
           << "Unknown patch extraction method: '" << nDetector << "'!"
           << endl;
      return;
    }
  sCommand  = sPath + sCommand + "-i " + sTmpName + " -o " + sFileName + " -thres 200";
  
  if( !bVerbose )
    sCommand = sCommand + " > /dev/null";

  /* save the current image as 'tmp.pgm' */
  m_qimgSrc.save( sTmpName.c_str(), "PGM" );
  
  /* call the interest point detector */
  if( bVerbose ) {
    cout << endl;
    cout << "  Issuing command '" << sCommand << "'..." << endl;
  }
  system( sCommand.c_str() );
  if( bVerbose )
    cout << endl;
  
  /* Load the results of the interest point detector */
  /* (adjust the scales: divide by 3)                */
  m_vPoints = loadInterestFileEllipse( sFileName, 1.0/3.0, bVerbose );
  
  /* remove the temporary files again */
  system( (string("rm -f ") + sTmpName).c_str() );
  system( (string("rm -f ") + sFileName).c_str() );
}


void FeatureCue::applyLowe( bool bVerbose )
/*******************************************************************/
/* Collect points from the image by applying Lowe's interest       */
/* point detector (calling an external program).                   */
/*******************************************************************/
{
  int nRandom = random() % 100000;
  string sRandom   = string( QString::number(nRandom).latin1() );
  string sPath     = PATH_LOWE;
  string sCommand  = "keypoints ";
  string sTmpName  = string("./tmp") + sRandom + ".pgm";
  string sFileName = string("./result") + sRandom + ".key";
  sCommand  = sPath + sCommand + " < " + sTmpName + " > " + sFileName;
  
  if( !bVerbose )
    sCommand = sCommand + " > /dev/null";

  /* save the current image as 'tmp.pgm' */
  m_qimgSrc.save( sTmpName.c_str(), "PGM" );
  
  /* call the interest point detector */
  if( bVerbose ) {
    cout << endl;
    cout << "  Issuing command '" << sCommand << "'..." << endl;
  }
  system( sCommand.c_str() );
  if( bVerbose )
    cout << endl;
  
  m_vPoints = loadInterestFileLowe( sFileName );
  
  /* remove the temporary files again */
  system( (string("rm -f ") + sTmpName).c_str() );
  system( (string("rm -f ") + sFileName).c_str() );
}


void FeatureCue::applyMatas( bool bVerbose )
/*******************************************************************/
/* Collect points from the image by applying Matas's inte-        */
/* rest point detector (calling an external program).             */
/*******************************************************************/
{
  int nRandom = random() % 100000;
  string sRandom   = string( QString::number(nRandom).latin1() );
  string sPath     = PATH_MATAS;
  string sCommand  = "mser.ln ";
  string sTmpName  = string("./tmp") + sRandom + ".pgm";
  string sFileName = string("./result") + sRandom + ".key";
  sCommand  = sPath + sCommand + "-t 2 -i " + sTmpName + " -o " + sFileName;
  
  if( !bVerbose )
    sCommand = sCommand + " > /dev/null";

  /* save the current image as 'tmp.pgm' */
  m_qimgSrc.save( sTmpName.c_str(), "PGM" );
  
  /* call the interest point detector */
  if( bVerbose ) {
    cout << endl;
    cout << "  Issuing command '" << sCommand << "'..." << endl;
  }
  system( sCommand.c_str() );
  if( bVerbose ) 
    cout << endl;
  
  m_vPoints = loadInterestFileEllipse( sFileName, 1.0, bVerbose );
  
  /* remove the temporary files again */
  system( (string("rm -f ") + sTmpName).c_str() );
  system( (string("rm -f ") + sFileName).c_str() );
}


void FeatureCue::applyMatasUnaffine( bool bVerbose )
/*******************************************************************/
/* Collect points from the image by applying Matas's inte-        */
/* rest point detector (calling an external program).             */
/*******************************************************************/
{
  int nRandom = random() % 100000;
  string sRandom   = string( QString::number(nRandom).latin1() );
  string sPath     = PATH_MATAS;
  string sCommand  = "mser.ln ";
  string sTmpName  = string("./tmp") + sRandom + ".pgm";
  string sFileName = string("./result") + sRandom + ".key";
  sCommand  = sPath + sCommand + "-t 2 -i " + sTmpName + " -o " + sFileName;
  
  if( !bVerbose )
    sCommand = sCommand + " > /dev/null";

  /* save the current image as 'tmp.pgm' */
  m_qimgSrc.save( sTmpName.c_str(), "PGM" );
  
  /* call the interest point detector */
  if( bVerbose ) {
    cout << endl;
    cout << "  Issuing command '" << sCommand << "'..." << endl;
  }
  system( sCommand.c_str() );
  if( bVerbose )
    cout << endl;
  
  m_vPoints = loadInterestFileEllipse( sFileName, 1.0, bVerbose );
  
  /* adjust the scales s.t. the regions are circular */
  for( int i=0; i<(int)m_vPoints.size(); i++ ) {
    m_vPoints[i].l1 = m_vPoints[i].scale;
    m_vPoints[i].l2 = m_vPoints[i].scale;
  }

  /* remove the temporary files again */
  system( (string("rm -f ") + sTmpName).c_str() );
  system( (string("rm -f ") + sFileName).c_str() );
}


void FeatureCue::applyExactDoG( bool bVerbose )
/*******************************************************************/
/* Collect points from the image by applying an exact DoG operator */
/* (without a Gaussian pyramid, but with scale interpolation).     */
/*******************************************************************/
{
  /* compute the scale space */
  DoGScaleSpace dogscale( m_imgSrc,
                          m_guiParams->m_nScaleOctaves,
                          m_guiParams->m_nLevsPerOctave,
                          m_guiParams->m_dScaleSigma0 );
  
  /* extract the interest points */
  PointVector vPoints = dogscale.getScaleMaxima3D( m_guiParams->m_dThreshEdog,
                                             true, //m_bInterpScales
                                             false ); // don't remove edges
  
  /* verify the scale range */
  m_vPoints.clear();
  for( unsigned i=0; i<vPoints.size(); i++ )
    if( (vPoints[i].scale>=m_guiParams->m_dMinScale) &&
        (vPoints[i].scale<=m_guiParams->m_dMaxScale) )
      m_vPoints.push_back( vPoints[i] );
}


void  FeatureCue::applySURF( int nFeatureType,
                             PointVector           &vPoints,
                             vector<FeatureVector> &vResults,
                             bool bVerbose )
/*******************************************************************/
/* Collect points from the image by applying the SURF interest     */
/* point detector and directly compute SURF descriptors            */
/* (calling an external program).                                  */
/*******************************************************************/
{
  vResults.clear();

  int nRandom = random() % 100000;
  string sRandom   = string( QString::number(nRandom).latin1() );
  string sPath     = PATH_SURF;
  string sCommand;
  if( nFeatureType==FEATURE_SURF128 )
    //sCommand = "surf128.ln ";
    sCommand = "surf.ln -e";
  else
    //sCommand = "surf64.ln";
    sCommand = "surf.ln";
  string sTmpName  = string("./tmp") + sRandom + ".pgm";
  string sFileName = string("./result") + sRandom + ".key";

  sCommand  = ( sPath + sCommand + " -i " + sTmpName + " -o " + sFileName + 
                " -thres " + 
                QString::number(m_guiParams->m_dThreshSURF,'f',1).latin1() );
  if( !m_guiParams->m_bMakeRotInv )
    sCommand  = sCommand + " -u";
  
  if( !bVerbose )
    sCommand = sCommand + " > /dev/null";

  /* save the current image as 'tmp.pgm' */
  m_qimgSrc.save( sTmpName.c_str(), "PGM" );
  
  /* call the combined detector and descriptor */
  if( bVerbose ) {
    cout << endl;
    cout << "  Issuing command '" << sCommand << "'..." << endl;
  }
  system( sCommand.c_str() );
  if( bVerbose )
    cout << endl;
  
  /* load the result file */
  loadDescriptorFileSURF( sFileName, vPoints, vResults, 1.0/3.0,
                          bVerbose );
  
  /* remove the temporary files again */
  system( (string("rm -f ") + sTmpName).c_str() );
  system( (string("rm -f ") + sFileName).c_str() );
}


/*---------------------------------------------------------*/
/*                 Computing Descriptors                   */
/*---------------------------------------------------------*/
void FeatureCue::extractFeatures_SIFT()
/***********************************************************/
{
	assert(m_imgSrc.width() == m_imgSrcMap.width() && m_imgSrc.height() == m_imgSrcMap.height());
	collectAllFeatures();
	findPointsInsideBoundingBox();
}

void FeatureCue::collectAllFeatures()
{
	PGMImage pgmimage(m_qimgSrc);
	SiftDetector siftdetector(pgmimage, m_vPoins);
	siftdetector.initializeAllPreliminaryData();
	siftdetector.calculateAllSiftDescriptor();

	vector< vector<double> > features = siftdetector.getDescr();
	m_vFeatures.clear();
	for(int i=0; i<features.size(); i++)
	{
		FeatureVector fv(features[i]);
		m_vFeatures.push_back(fv);
	}
}

/*
void FeatureCue::findPointsInsideBoundingBox()
{
	if(m_vPoints.size()!=m_vFeatures.size())
		cout<<" Error in findPointsInsideBoundingBox: interest points' size is not equal with features' size..."<<endl;
	for(int i=0; i<static_cast<int>(m_vFeatures.size()); i++)
	{
		if(isBoundingBoxCoverFeatureofIndex(i))
		{
			m_vPtIdzs.push_back(i);
			m_vPointsInside.push_back(m_vPoints[i]);
		}
	}
}*/

void FeatureCue::isBoundingBoxCoverFeatureofIndex(int index)
{
	
}

void FeatureCue::extractAllPatches( bool bFeaturesExtracted, bool bVerbose,
                                    bool bExtractPatches )
/*******************************************************************/
/* Extract an image patch for each selected point location.        */
/*******************************************************************/
{
  assert( !m_guiParams->m_bUseFigureOnly || 
          (m_imgSrc.width()==m_imgSrcMap.width() &&
           m_imgSrc.height()==m_imgSrcMap.height()) );

  if( bVerbose )
    cout << "FeatureCue::extractAllPatches() called." << endl;
  OpGrayImage         imgEdges;
  vector<OpGrayImage> vEdgePlanes;
  
  int defaultPatchSize  = m_guiParams->m_nPatchSize;
  int defaultPatchWidth = defaultPatchSize*2+1;
  int minFigurePixels   = m_guiParams->m_nMinFigurePixels;
  
  if( bVerbose )
    if ( m_guiParams->m_bUseFigureOnly )
      cout << "  Using only figure area for patch extraction ..." << endl;
  
	m_nFeatureType = m_guiParams->m_nFeatureType;
	
  /*----------------------*/
  /* Prepare the features */
  /*----------------------*/
  vector<FeatureVector> vFeatures;
  vector<FeatureVector> vFeaturesInside;
  switch( m_nFeatureType ) {
  case FEATURE_PATCH:
    break;

  case FEATURE_PATCHMIKO:
  case FEATURE_PATCHMIKO2:
  case FEATURE_STEERABLE:
  case FEATURE_SIFT:
  case FEATURE_GLOH:
  case FEATURE_MOMENTS:
  case FEATURE_DIFFINV:
  case FEATURE_COMPLEX:
  case FEATURE_SHAPECONTEXT:
  case FEATURE_SPINIMGS:
  case FEATURE_GRADIENTPCA:
    if( !bFeaturesExtracted ) 
      /* apply Mikolajczyk's descriptors */
      if( m_guiParams->m_nPatchExtMethod==PATCHEXT_EDGELAP )
        applyMikoDescriptorNew( m_nFeatureType, m_vPoints, vFeatures, 
                                bVerbose );
      else
        applyMikoDescriptor( m_nFeatureType, m_vPoints, vFeatures, bVerbose );
    else {
      vFeatures = m_vFeatures;
      //m_vFeatures.clear();
    }
    m_vFeatures.clear();
    break;

  case FEATURE_SURF64:
  case FEATURE_SURF128:
    if( !bFeaturesExtracted ) 
      applySURFDescriptor( m_nFeatureType, m_vPoints, vFeatures, bVerbose );
    else {
      vFeatures = m_vFeatures;
      //m_vFeatures.clear();
    m_vFeatures.clear();}
    break;

  default:
    cerr << "  Error in FeatureCue::extractAllPatches(): "
         << "Unknown feature type (" << m_nFeatureType << ")!" << endl;
    return;
  }
  
  //assert( vFeatures.size()==m_vPoints.size() );
   
  /*------------------------------------*/
  /* Extract all patches from the image */
  /*------------------------------------*/
  m_vPtIdzs.clear();
  for( int i=0; i<(int)m_vPoints.size(); i++) {
    // compute the image patch size
    //int nPatchSize = (int) floor( m_vPoints[i].scale*
    //                             m_guiParams->m_dScaleFactor + 0.5 )	
    int nPatchSize = m_guiParams->m_nPatchSize;
    //int patchWidth = nPatchSize*2+1;
    //int nPatchArea = patchWidth*patchWidth;
    
    
    // check if image patch is fully inside image
    if (    (m_vPoints[i].x - nPatchSize >= 0) &&
            (m_vPoints[i].y - nPatchSize >= 0) &&
            (m_vPoints[i].x + nPatchSize < m_imgSrc.width()) &&
            (m_vPoints[i].y + nPatchSize < m_imgSrc.height()) )
      {
        //m_vPointsInside.push_back( m_vPoints[i] );
        
        OpGrayImage imgPatch;        
        if( (bExtractPatches || m_nFeatureType==FEATURE_PATCH) && 
            (m_vPoints[i].l1 == m_vPoints[i].l2) ) { 
          /*-=-=-=-=-=-=-=-=-*/
          /* Circular region */
          /*-=-=-=-=-=-=-=-=-*/
          imgPatch = m_imgSrc.extractRegion( m_vPoints[i].x - nPatchSize,
                                             m_vPoints[i].y - nPatchSize,
                                             m_vPoints[i].x + nPatchSize,
                                             m_vPoints[i].y + nPatchSize );
          
          //--- rescale patch to default size
          if( nPatchSize != m_guiParams->m_nPatchSize ) {
            imgPatch = imgPatch.opRescaleToWidth( defaultPatchWidth );
          }

        } else {
          /*-=-=-=-=-=-=-=-=-=-*/
          /* Elliptical region */
          /*-=-=-=-=-=-=-=-=-=-*/
          extractAffineRegion( m_imgSrc, m_vPoints[i], 
                               2*m_guiParams->m_nPatchSize+1, 
                               imgPatch );
          
        }
        
        //--- all patches? or only those on seg mask? ---//
        if ( m_guiParams->m_bUseFigureOnly )
          {
            OpGrayImage imgMap;
            if( m_vPoints[i].l1 == m_vPoints[i].l2 ) { 
              /*-=-=-=-=-=-=-=-=-*/
              /* Circular region */
              /*-=-=-=-=-=-=-=-=-*/
              imgMap = m_imgSrcMap.extractRegion( m_vPoints[i].x - nPatchSize,
                                                  m_vPoints[i].y - nPatchSize,
                                                  m_vPoints[i].x + nPatchSize,
                                                  m_vPoints[i].y + nPatchSize);
              imgMap = imgMap.opRescaleToWidth( defaultPatchWidth );
              
            } else {
              /*-=-=-=-=-=-=-=-=-=-*/
              /* Elliptical region */
              /*-=-=-=-=-=-=-=-=-=-*/
              extractAffineRegion( m_imgSrcMap, m_vPoints[i], 
                                   2*m_guiParams->m_nPatchSize+1, 
                                   imgMap );
            }
            
            //--- only take the patch if it contains  ---//
            //--- a minimum number of figure pixels   ---//
            if( (imgMap.getSum() >= minFigurePixels*255.0) ) {
              m_vPatches.push_back( imgPatch );
              m_vPointsInside.push_back( m_vPoints[i] );
              m_vPtIdzs.push_back( i );

              /* accomodate for other feature descriptors */
              if( (m_nFeatureType!=FEATURE_PATCH) )
                vFeaturesInside.push_back( vFeatures[i] );
            }
          }
        else
          {
            /* no maps available => always take the patch */
            m_vPatches.push_back( imgPatch );
            m_vPointsInside.push_back( m_vPoints[i] );
            m_vPtIdzs.push_back( i );

            /* accomodate for other feature descriptors */
           if( (m_nFeatureType!=FEATURE_PATCH) )
              vFeaturesInside.push_back( vFeatures[i] );
          }

        
      }//end if patch is completely within image
  }// end for each interest point

  if( bVerbose )
    cout << "  Number of image patches taken: " << m_vPatches.size() << endl;
    
  /*------------------------------------------------*/
  /* If desired, filter the patches with a Gaussian */
  /*------------------------------------------------*/
  if( (bExtractPatches || m_nFeatureType==FEATURE_PATCH) && 
      m_guiParams->m_bFilterPatches )
    for( unsigned i=0; i < m_vPatches.size(); i++)
      m_vPatches[i] = m_vPatches[i].opFastGauss( 1.0 );
  
    
  /*----------------------------------------*/
  /* Convert the patches to feature vectors */
  /*----------------------------------------*/
  m_vFeatures.clear();
  switch( m_nFeatureType ) {
  case FEATURE_PATCH:
    /* convert the patches to feature vectors */
    for( int i=0; i <(int)m_vPatches.size(); i++)
      m_vFeatures.push_back( m_vPatches[i].getData() );
    break;

  case FEATURE_PATCHMIKO:
  case FEATURE_PATCHMIKO2:
  case FEATURE_STEERABLE:
  case FEATURE_SIFT:
  case FEATURE_GLOH:
  case FEATURE_MOMENTS:
  case FEATURE_DIFFINV:
  case FEATURE_COMPLEX:
  case FEATURE_SHAPECONTEXT:
  case FEATURE_SPINIMGS:
  case FEATURE_GRADIENTPCA:
  case FEATURE_SURF64:
  case FEATURE_SURF128:
    /* vFeatures already contains the features */
    m_vFeatures.insert( m_vFeatures.end(), 
                        vFeaturesInside.begin(), vFeaturesInside.end());
    break;

  default:
    cerr << "  Error in FeatureCue::extractAllPatches(): "
         << "Unknown feature type (" << m_nFeatureType << ")!" << endl;
    return;
  }
  //cout << "  Extracted patches : " << m_vPatches.size() << endl;
  //cout << "            features: " << m_vFeatures.size() << endl;
}

void FeatureCue::extractAllPatches_OBJnBKG(	bool bFeaturesExtracted, 
											bool bVerbose,
											bool bExtractPatches )
/*******************************************************************/
/* Extract an image patch for each selected point location. All pat*/
/* ches are stored in m_vPatches, all points are stored in m_vPoin */
/* ts. The insiders and outsiders are distinguished with InterestP */
/* oint.value, +1 stand for inside, -1 stands for outside.         */
/*******************************************************************/
{
  assert( !m_guiParams->m_bUseFigureOnly || 
          (m_imgSrc.width()==m_imgSrcMap.width() &&
           m_imgSrc.height()==m_imgSrcMap.height()) );

  if( bVerbose )
    cout << "FeatureCue::extractAllPatches_OBJnBKG() called." << endl;
  OpGrayImage         imgEdges;
  vector<OpGrayImage> vEdgePlanes;
  
  int defaultPatchSize  = m_guiParams->m_nPatchSize;
  int defaultPatchWidth = defaultPatchSize*2+1;
  int minFigurePixels   = m_guiParams->m_nMinFigurePixels;
  
  if( bVerbose )
    if ( m_guiParams->m_bUseFigureOnly )
      cout << "  Using only figure area for patch extraction ..." << endl;
  
	m_nFeatureType = m_guiParams->m_nFeatureType;
	
  /*----------------------*/
  /* Prepare the features */
  /*----------------------*/
  vector<FeatureVector> vFeatures;
  vector<FeatureVector> vFeaturesInside;
  switch( m_nFeatureType ) {
  case FEATURE_PATCH:
    break;

  case FEATURE_PATCHMIKO:
  case FEATURE_PATCHMIKO2:
  case FEATURE_STEERABLE:
  case FEATURE_SIFT:
  case FEATURE_GLOH:
  case FEATURE_MOMENTS:
  case FEATURE_DIFFINV:
  case FEATURE_COMPLEX:
  case FEATURE_SHAPECONTEXT:
  case FEATURE_SPINIMGS:
  case FEATURE_GRADIENTPCA:
    if( !bFeaturesExtracted ) 
      /* apply Mikolajczyk's descriptors */
      if( m_guiParams->m_nPatchExtMethod==PATCHEXT_EDGELAP )
        applyMikoDescriptorNew( m_nFeatureType, m_vPoints, vFeatures, 
                                bVerbose );
      else
        applyMikoDescriptor( m_nFeatureType, m_vPoints, vFeatures, bVerbose );
    else {
      vFeatures = m_vFeatures;
      //m_vFeatures.clear();
    }
    m_vFeatures.clear();
    break;

  case FEATURE_SURF64:
  case FEATURE_SURF128:
    if( !bFeaturesExtracted ) 
      applySURFDescriptor( m_nFeatureType, m_vPoints, vFeatures, bVerbose );
    else {
	 //TODO: I changed some, but dont know how I changed..
      vFeatures = m_vFeatures;
      //m_vFeatures.clear();
      m_vFeatures.clear();
		}
    break;

  default:
    cerr << "  Error in FeatureCue::extractAllPatches(): "
         << "Unknown feature type (" << m_nFeatureType << ")!" << endl;
    return;
  }
  
  //assert( vFeatures.size()==m_vPoints.size() );
   
  /*------------------------------------*/
  /* Extract all patches from the image */
  /*------------------------------------*/
  m_vPtIdzs.clear();
  for( int i=0; i<(int)m_vPoints.size(); i++) {
    // compute the image patch size
    //int nPatchSize = (int) floor( m_vPoints[i].scale*
    //                              m_guiParams->m_dScaleFactor + 0.5 );
    int nPatchSize = m_guiParams->m_nPatchSize;
   /* 	  cout<<"Extracting at x = "<< m_vPoints[i].x
			  <<", y = "<< m_vPoints[i].y
			  <<", Patchsize = "<<nPatchSize
			  <<", pointScale = "<<m_vPoints[i].scale<<endl;
  */
    // check if image patch is fully inside image
    if (    (m_vPoints[i].x - nPatchSize >= 0) &&
            (m_vPoints[i].y - nPatchSize >= 0) &&
            (m_vPoints[i].x + nPatchSize < m_imgSrc.width()) &&
            (m_vPoints[i].y + nPatchSize < m_imgSrc.height()) )
      {
        OpGrayImage imgPatch;        
        if( (bExtractPatches || m_nFeatureType==FEATURE_PATCH) && 
            (m_vPoints[i].l1 == m_vPoints[i].l2) ) { 
          /*-=-=-=-=-=-=-=-=-*/
          /* Circular region */
          /*-=-=-=-=-=-=-=-=-*/
	          imgPatch = m_imgSrc.extractRegion( m_vPoints[i].x - nPatchSize,
                                             m_vPoints[i].y - nPatchSize,
                                             m_vPoints[i].x + nPatchSize,
                                             m_vPoints[i].y + nPatchSize );
          
          //--- rescale patch to default size
          if( nPatchSize != m_guiParams->m_nPatchSize ) {
            imgPatch = imgPatch.opRescaleToWidth( defaultPatchWidth );
          }

        } else {
          /*-=-=-=-=-=-=-=-=-=-*/
          /* Elliptical region */
          /*-=-=-=-=-=-=-=-=-=-*/
          extractAffineRegion( m_imgSrc, m_vPoints[i], 
                               2*m_guiParams->m_nPatchSize+1, 
                               imgPatch );
          
        }
        // Judge whether it is in the box or not
         OpGrayImage imgMap;
         if( m_vPoints[i].l1 == m_vPoints[i].l2 ) { 
              /*-=-=-=-=-=-=-=-=-*/
              /* Circular region */
              /*-=-=-=-=-=-=-=-=-*/
            imgMap = m_imgSrcMap.extractRegion( m_vPoints[i].x - nPatchSize,
                                                m_vPoints[i].y - nPatchSize,
                                                m_vPoints[i].x + nPatchSize,
                                                m_vPoints[i].y + nPatchSize);
             
			imgMap = imgMap.opRescaleToWidth( defaultPatchWidth );
              
            } else {
              /*-=-=-=-=-=-=-=-=-=-*/
              /* Elliptical region */
              /*-=-=-=-=-=-=-=-=-=-*/
            extractAffineRegion( m_imgSrcMap, m_vPoints[i], 
                                 2*m_guiParams->m_nPatchSize+1, 
                                 imgMap );
            }
		  if( (imgMap.getSum() >= minFigurePixels*255.0) ) {
			  m_vPoints[i].value = 1;
		      m_vPointsInside.push_back(m_vPoints[i]);
			  m_vPatches.push_back(imgPatch);	  
		  }
		  else
		  {
			  m_vPoints[i].value = -1;
			  m_vPointsInside.push_back(m_vPoints[i]);
			  m_vPatches.push_back(imgPatch);
          }     
      }//end if patch is completely within image
  }// end for each interest point

  if( bVerbose )
    cout << "  Number of image patches taken: " << m_vPatches.size() << endl;
    
  /*------------------------------------------------*/
  /* If desired, filter the patches with a Gaussian */
  /*------------------------------------------------*/
  if( (bExtractPatches || m_nFeatureType==FEATURE_PATCH) && 
      m_guiParams->m_bFilterPatches )
    for( unsigned i=0; i < m_vPatches.size(); i++)
      m_vPatches[i] = m_vPatches[i].opFastGauss( 1.0 );
  
    
  /*----------------------------------------*/
  /* Convert the patches to feature vectors */
  /*----------------------------------------*/
  m_vFeatures.clear();
  switch( m_nFeatureType ) {
  case FEATURE_PATCH:
    /* convert the patches to feature vectors */
    for( int i=0; i <(int)m_vPatches.size(); i++)
      m_vFeatures.push_back( m_vPatches[i].getData() );
    break;

  case FEATURE_PATCHMIKO:
  case FEATURE_PATCHMIKO2:
  case FEATURE_STEERABLE:
  case FEATURE_SIFT:
  case FEATURE_GLOH:
  case FEATURE_MOMENTS:
  case FEATURE_DIFFINV:
  case FEATURE_COMPLEX:
  case FEATURE_SHAPECONTEXT:
  case FEATURE_SPINIMGS:
  case FEATURE_GRADIENTPCA:
  case FEATURE_SURF64:
  case FEATURE_SURF128:
    /* vFeatures already contains the features */
    m_vFeatures.insert( m_vFeatures.end(), 
                        vFeaturesInside.begin(), vFeaturesInside.end());
    break;

  default:
    cerr << "  Error in FeatureCue::extractAllPatches(): "
         << "Unknown feature type (" << m_nFeatureType << ")!" << endl;
    return;
  }
  //cout << "  Extracted patches : " << m_vPatches.size() << endl;
  //cout << "            features: " << m_vFeatures.size() << endl;
}

void  FeatureCue::applyMikoBoth( string sImgName, 
                                 int nDetector, int nFeatureType, 
                                 PointVector           &vPoints,
                                 vector<FeatureVector> &vResults,
                                 bool bVerbose )
/*******************************************************************/
/* Collect points from the image by applying Mikolajczyk's inte-   */
/* rest point detectors and directly compute the descriptors       */
/* (calling an external program).                                  */
/*******************************************************************/
{
  vResults.clear();

  int nRandom = random() % 100000;
  string sRandom   = string( QString::number(nRandom).latin1() );
  string sPath     = PATH_MIKO;
  string sCommand  = "compute_descriptors2.ln ";
  string sFileName = string("./result") + sRandom + ".key";

  switch( nDetector ) {
  case PATCHEXT_HARLAP:
    /* apply the Multiscale Harris detector */
    sCommand = sCommand + "-harlap ";
    break;
    
  case PATCHEXT_HESLAP:
    /* apply the Fast Multiscale Harris detector */
    sCommand = sCommand + "-heslap ";
    break;
    
  case PATCHEXT_HARHESLAP:
    /* apply the Multiscale Harris+Hessian detector */
    sCommand = sCommand + "-harhes ";
    break;
    
  case PATCHEXT_HARAFF:
    /* apply the Multiscale Harris detector */
    sCommand = sCommand + "-haraff ";
    break;
    
  case PATCHEXT_HESAFF:
    /* apply the Fast Multiscale Harris detector */
    sCommand = sCommand + "-hesaff ";
    break;
    
  default:
    cerr << "  Error in FeatureCue::applyMikoBoth(): "
         << "Unknown patch extraction method: '" << nDetector << "'!"
         << endl;
    return;
  }
  
  switch( nFeatureType ) {
  case FEATURE_PATCHMIKO:
  case FEATURE_PATCHMIKO2:
      /* extract 10*10 patches */
      sCommand = sCommand + "-cc ";
      break;

  case FEATURE_STEERABLE:
      /* extract Steerable Filters */
      sCommand = sCommand + "-jla ";
      break;

  case FEATURE_SIFT:
      /* extract SIFT features */
      sCommand = sCommand + "-sift ";
      break;

  case FEATURE_GLOH:
      /* extract GLOH (gradient location-orientation histogram) */
      sCommand = sCommand + "-gloh ";
      break;

  case FEATURE_MOMENTS:
      /* extract Moments */
      sCommand = sCommand + "-mom ";
      break;

  case FEATURE_DIFFINV:
      /* extract Differential Invariants */
      sCommand = sCommand + "-koen ";
      break;

  case FEATURE_COMPLEX:
      /* extract Complex Filters */
      sCommand = sCommand + "-cf ";
      break;

  case FEATURE_SHAPECONTEXT:
      /* extract Shape Context */
      sCommand = sCommand + "-sc ";
      break;

  case FEATURE_SPINIMGS:
      /* extract Spin Images */
      sCommand = sCommand + "-spin ";
      break;

  case FEATURE_GRADIENTPCA:
      /* extract Gradient PCA */
      sCommand = sCommand + "-pca ";
      break;

  default:
    cerr << "  Error in FeatureCue::applyMikoBoth(): "
         << "Unknown feature type (" << nFeatureType << ")!"
         << endl;
    return;
  }
  sCommand  = ( sPath + sCommand + " -i " + sImgName + " -o1 " + sFileName + 
                " -thres 200" );
  if( !m_guiParams->m_bMakeRotInv )
    sCommand  = sCommand + " -noangle";
  
  if( !bVerbose )
    sCommand = sCommand + " > /dev/null";

  /* call the combined detector and descriptor */
  if( bVerbose ) {
    cout << endl;
    cout << "  Issuing command '" << sCommand << "'..." << endl;
  }
  system( sCommand.c_str() );
  if( bVerbose )
    cout << endl;
  
  /* load the result file */
  loadDescriptorFileEllipse( sFileName, vPoints, vResults, 1.2*(1.0/3.0),
                             bVerbose );
  
  /* remove the temporary files again */
  system( (string("rm -f ") + sFileName).c_str() );
}


void  FeatureCue::applyMikoBothNew( string sImgName, 
                                    int nDetector, int nFeatureType, 
                                    PointVector           &vPoints,
                                    vector<FeatureVector> &vResults,
                                    bool bVerbose )
/*******************************************************************/
/* Collect points from the image by applying Mikolajczyk's inte-   */
/* rest point detectors and directly compute the descriptors       */
/* (calling an external program).                                  */
/*******************************************************************/
{
  vResults.clear();
  bool bSaveImg = true;
  if( !sImgName.empty() && sImgName.rfind(".png")!=string::npos )
    bSaveImg = false;

  int nRandom = random() % 100000;
  string sRandom   = string( QString::number(nRandom).latin1() );
  string sPath     = PATH_MIKO;
  string sCommand  = "compute_descriptors3.ln ";
  string sTmpName  = string("./tmp") + sRandom + ".pgm";
  string sFileName = string("./result") + sRandom + ".key";

  switch( nDetector ) {
  case PATCHEXT_HARLAP:
    /* apply the Multiscale Harris detector */
    sCommand = sCommand + "-harlap ";
    break;
    
  case PATCHEXT_HESLAP:
    /* apply the Fast Multiscale Harris detector */
    sCommand = sCommand + "-heslap ";
    break;
    
  case PATCHEXT_HARHESLAP:
    /* apply the Multiscale Harris+Hessian detector */
    sCommand = sCommand + "-harhes ";
    break;
    
  case PATCHEXT_HARAFF:
    /* apply the Multiscale Harris detector */
    sCommand = sCommand + "-haraff ";
    break;
    
  case PATCHEXT_HESAFF:
    /* apply the Fast Multiscale Harris detector */
    sCommand = sCommand + "-hesaff ";
    break;
    
   case PATCHEXT_EDGELAP:
    /* apply the Edge Laplacian detector */
    sCommand = sCommand + "-sedgelap ";
    break;
    
 default:
    cerr << "  Error in FeatureCue::applyMikoBothNew(): "
         << "Unknown patch extraction method: '" << nDetector << "'!"
         << endl;
    return;
  }
  
  switch( nFeatureType ) {
  case FEATURE_PATCHMIKO:
  case FEATURE_PATCHMIKO2:
      /* extract 10*10 patches */
      sCommand = sCommand + "-cc ";
      break;

  case FEATURE_STEERABLE:
      /* extract Steerable Filters */
      sCommand = sCommand + "-jla ";
      break;

  case FEATURE_SIFT:
      /* extract SIFT features */
      sCommand = sCommand + "-sift ";
      break;

  case FEATURE_GLOH:
      /* extract GLOH (gradient location-orientation histogram) */
      sCommand = sCommand + "-gloh ";
      break;

  case FEATURE_MOMENTS:
      /* extract Moments */
      sCommand = sCommand + "-mom ";
      break;

  case FEATURE_DIFFINV:
      /* extract Differential Invariants */
      sCommand = sCommand + "-koen ";
      break;

  case FEATURE_COMPLEX:
      /* extract Complex Filters */
      sCommand = sCommand + "-cf ";
      break;

  case FEATURE_SHAPECONTEXT:
      /* extract Shape Context */
      sCommand = sCommand + "-sc ";
      break;

  case FEATURE_SPINIMGS:
      /* extract Spin Images */
      sCommand = sCommand + "-spin ";
      break;

  case FEATURE_GRADIENTPCA:
      /* extract Gradient PCA */
      sCommand = sCommand + "-pca ";
      break;

  default:
    cerr << "  Error in FeatureCue::applyMikoBothNew(): "
         << "Unknown feature type (" << nFeatureType << ")!"
         << endl;
    return;
  }
  if( bSaveImg ) 
    sCommand = sPath + sCommand + " -i " + sTmpName;
  else
    sCommand = sPath + sCommand + " -i " + sImgName;
  sCommand  = ( sCommand + " -o1 " + sFileName + " -thres 200" );

  if( !m_guiParams->m_bMakeRotInv )
    sCommand  = sCommand + " -noangle";
  
  if( !bVerbose )
    sCommand = sCommand + " > /dev/null";

  /* save the current image as 'tmp.pgm' */
  if( bSaveImg )
    m_qimgSrc.save( sTmpName.c_str(), "PGM" );
  
  /* call the combined detector and descriptor */
  if( bVerbose ) {
    cout << endl;
    cout << "  Issuing command '" << sCommand << "'..." << endl;
  }
  system( sCommand.c_str() );
  if( bVerbose )
    cout << endl;
  
  /* load the result file */
  loadDescriptorFileEllipse( sFileName, vPoints, vResults, 1.0, //1.2*(1.0/3.0),
                             bVerbose );
  
  /* remove the temporary files again */
  system( (string("rm -f ") + sFileName).c_str() );
  if( bSaveImg )
    system( (string("rm -f ") + sTmpName).c_str() );
}


void  FeatureCue::applyMikoDescriptor( int nFeatureType, 
                                       const PointVector     &vPoints,
                                       vector<FeatureVector> &vResults,
                                       bool bVerbose )
/*******************************************************************/
/* Apply one of Mikolajczyk's descriptors on a given set of inte-  */
/* rest point (calling an external program).                       */
/*******************************************************************/
{
  vResults.clear();

  int nRandom = random() % 100000;
  string sRandom   = string( QString::number(nRandom).latin1() );
  string sPath     = PATH_MIKO;
  string sCommand  = "compute_descriptors.ln ";
  string sTmpName  = string("./tmp") + sRandom + ".pgm";
  string sIntPtName= string("./tmp") + sRandom + ".key";
  string sFileName = string("./result") + sRandom + ".key";
  switch( nFeatureType ) {
  case FEATURE_PATCHMIKO:
  case FEATURE_PATCHMIKO2:
      /* extract 10*10 patches */
      sCommand = sCommand + "-cc ";
      break;

  case FEATURE_STEERABLE:
      /* extract Steerable Filters */
      sCommand = sCommand + "-jla ";
      break;

  case FEATURE_SIFT:
      /* extract SIFT features */
      sCommand = sCommand + "-sift ";
      break;

  case FEATURE_GLOH:
      /* extract GLOH (gradient location-orientation histogram) */
      sCommand = sCommand + "-gloh ";
      break;

  case FEATURE_MOMENTS:
      /* extract Moments */
      sCommand = sCommand + "-mom ";
      break;

  case FEATURE_DIFFINV:
      /* extract Differential Invariants */
      sCommand = sCommand + "-koen ";
      break;

  case FEATURE_COMPLEX:
      /* extract Complex Filters */
      sCommand = sCommand + "-cf ";
      break;

  case FEATURE_SHAPECONTEXT:
      /* extract Shape Context */
      sCommand = sCommand + "-sc ";
      break;

  case FEATURE_SPINIMGS:
      /* extract Spin Images */
      sCommand = sCommand + "-spin ";
      break;

  case FEATURE_GRADIENTPCA:
      /* extract Gradient PCA */
      sCommand = sCommand + "-pca ";
      break;

  default:
    cerr << "  Error in FeatureCue::applyMikoDescriptor(): "
         << "Unknown feature type (" << nFeatureType << ")!"
         << endl;
    return;
  }
  sCommand  = ( sPath + sCommand + " -i " + sTmpName + " -p1 " + sIntPtName +
                " -o1 " + sFileName);
  if( !m_guiParams->m_bMakeRotInv )
    sCommand  = sCommand + " -noangle";
  
  if( !bVerbose )
    sCommand = sCommand + " > /dev/null";

  /* save the current image as 'tmp.pgm' */
  m_qimgSrc.save( sTmpName.c_str(), "PGM" );
  
  /* write out the given interest points */
  writeInterestFileEllipse( sIntPtName, vPoints, bVerbose );

  /* call the descriptor */
  if( bVerbose ) {
    cout << endl;
    cout << "  Issuing command '" << sCommand << "'..." << endl;
  }
  int ret = system( sCommand.c_str() );
  bool bErrorOccurred = false;
  if( ret > 0 ) {
    //-----------------------------------------------------------------
    bErrorOccurred = true;
    cerr << "    FAILED... (return value: " << ret << ")" << endl;
    cerr << "    Appending one line to interest file... " << flush;
    ofstream ofile;
    ofile.open( sIntPtName.c_str(), ios_base::out | ios_base::app );
    if( !ofile ) {
      cerr << "      Error: Couldn't open file '" << sIntPtName << "'!" 
           << endl;
      exit(0);
    }

    // duplicate the last interest point in the file
    InterestPoint pt = vPoints[ vPoints.size()-1 ];
    float l1 = pt.l1 * 3.0;
    float l2 = pt.l2 * 3.0;
    float e1 = 1.0/(l1*l1);
    float e2 = 1.0/(l2*l2);

    float si   = sin(-pt.angle);
    float si2  = si*si;
    float co   = cos(-pt.angle);
    float co2  = co*co;
    float sico = si*co;

    float a = e1*co2 + e2*si2;
    float b = (e1-e2)*sico;
    float c = e1*si2 + e2*co2;
    
    ofile << pt.x << " " << pt.y << " " <<  a << " " <<  b << " " << c << endl;
    ofile.close();

    // Try again...
    cerr << "Retrying..." << endl;
    ret = system( sCommand.c_str() );
    if( ret > 0 ) {
      cerr << "    FAILED AGAIN! (return value: " << ret << ")" << endl;
      exit(0);
    }
    //-----------------------------------------------------------------
  }
  if( bVerbose )
    cout << endl;
  
  /* load the result file */
  if( bVerbose )
    cout << "  Loading the result file..." << endl;
  loadDescriptorFileEllipse( sFileName, vResults, bVerbose );
  
  if( bErrorOccurred ) {
    //-----------------------------------------------------------------
    // remove the last descriptor again to make the results consistent
    vResults.erase( vResults.end()-- );
    cerr << "  Corrected #descriptors to " << (int)vResults.size() << "." 
         << endl;
    //-----------------------------------------------------------------
  }

  /* remove the temporary files again */
  system( (string("rm -f ") + sTmpName).c_str() );
  system( (string("rm -f ") + sIntPtName).c_str() );
  system( (string("rm -f ") + sFileName).c_str() );
}


void  FeatureCue::applyMikoDescriptorNew( int nFeatureType, 
                                          const PointVector     &vPoints,
                                          vector<FeatureVector> &vResults,
                                          bool bVerbose )
/*******************************************************************/
/* Apply one of Mikolajczyk's descriptors on a given set of inte-  */
/* rest point (calling an external program).                       */
/*******************************************************************/
{
  vResults.clear();

  int nRandom = random() % 100000;
  string sRandom   = string( QString::number(nRandom).latin1() );
  string sPath     = PATH_MIKO;
  string sCommand  = "compute_descriptors3.ln ";
  string sTmpName  = string("./tmp") + sRandom + ".pgm";
  string sIntPtName= string("./tmp") + sRandom + ".key";
  string sFileName = string("./result") + sRandom + ".key";
  switch( nFeatureType ) {
  case FEATURE_PATCHMIKO:
  case FEATURE_PATCHMIKO2:
      /* extract 10*10 patches */
      sCommand = sCommand + "-cc ";
      break;

  case FEATURE_STEERABLE:
      /* extract Steerable Filters */
      sCommand = sCommand + "-jla ";
      break;

  case FEATURE_SIFT:
      /* extract SIFT features */
      sCommand = sCommand + "-sift ";
      break;

  case FEATURE_GLOH:
      /* extract GLOH (gradient location-orientation histogram) */
      sCommand = sCommand + "-gloh ";
      break;

  case FEATURE_MOMENTS:
      /* extract Moments */
      sCommand = sCommand + "-mom ";
      break;

  case FEATURE_DIFFINV:
      /* extract Differential Invariants */
      sCommand = sCommand + "-koen ";
      break;

  case FEATURE_COMPLEX:
      /* extract Complex Filters */
      sCommand = sCommand + "-cf ";
      break;

  case FEATURE_SHAPECONTEXT:
      /* extract Shape Context */
      sCommand = sCommand + "-sc ";
      break;

  case FEATURE_SPINIMGS:
      /* extract Spin Images */
      sCommand = sCommand + "-spin ";
      break;

  case FEATURE_GRADIENTPCA:
      /* extract Gradient PCA */
      sCommand = sCommand + "-pca ";
      break;

  default:
    cerr << "  Error in FeatureCue::applyMikoDescriptorNew(): "
         << "Unknown feature type (" << nFeatureType << ")!"
         << endl;
    return;
  }

  sCommand = ( sPath + sCommand + " -i " + sTmpName + 
               " -p1 " + sIntPtName + " -o1 " + sFileName + "-thresh 200" );

  if( !m_guiParams->m_bMakeRotInv )
    sCommand  = sCommand + " -noangle";
  
  if( !bVerbose )
    sCommand = sCommand + " > /dev/null";

  /* save the current image as 'tmp.pgm' */
  m_qimgSrc.save( sTmpName.c_str(), "PGM" );
  
  /* write out the given interest points */
  writeInterestFileEllipse( sIntPtName, vPoints, bVerbose );

  /* call the descriptor */
  if( bVerbose ) {
    cout << endl;
    cout << "  Issuing command '" << sCommand << "'..." << endl;
  }
  int ret = system( sCommand.c_str() );
  bool bErrorOccurred = false;
  if( ret > 0 ) {
    //-----------------------------------------------------------------
    bErrorOccurred = true;
    cerr << "    FAILED... (return value: " << ret << ")" << endl;
    cerr << "    Appending one line to interest file... " << flush;
    ofstream ofile;
    ofile.open( sIntPtName.c_str(), ios_base::out | ios_base::app );
    if( !ofile ) {
      cerr << "      Error: Couldn't open file '" << sIntPtName << "'!" 
           << endl;
      exit(0);
    }

    // duplicate the last interest point in the file
    InterestPoint pt = vPoints[ vPoints.size()-1 ];
    float l1 = pt.l1 * 3.0;
    float l2 = pt.l2 * 3.0;
    float e1 = 1.0/(l1*l1);
    float e2 = 1.0/(l2*l2);

    float si   = sin(-pt.angle);
    float si2  = si*si;
    float co   = cos(-pt.angle);
    float co2  = co*co;
    float sico = si*co;

    float a = e1*co2 + e2*si2;
    float b = (e1-e2)*sico;
    float c = e1*si2 + e2*co2;
    
    ofile << pt.x << " " << pt.y << " " <<  a << " " <<  b << " " << c << endl;
    ofile.close();

    // Try again...
    cerr << "Retrying..." << endl;
    ret = system( sCommand.c_str() );
    if( ret > 0 ) {
      cerr << "    FAILED AGAIN! (return value: " << ret << ")" << endl;
      exit(0);
    }
    //-----------------------------------------------------------------
  }
  if( bVerbose )
    cout << endl;
  
  /* load the result file */
  if( bVerbose )
    cout << "  Loading the result file..." << endl;
  loadDescriptorFileEllipse( sFileName, vResults, bVerbose );
  
  if( bErrorOccurred ) {
    //-----------------------------------------------------------------
    // remove the last descriptor again to make the results consistent
    vResults.erase( vResults.end()-- );
    cerr << "  Corrected #descriptors to " << (int)vResults.size() << "." 
         << endl;
    //-----------------------------------------------------------------
  }

  /* remove the temporary files again */
  system( (string("rm -f ") + sTmpName).c_str() );
  system( (string("rm -f ") + sIntPtName).c_str() );
  system( (string("rm -f ") + sFileName).c_str() );
}


void  FeatureCue::applySURFDescriptor( int nFeatureType,
                                       PointVector           &vPoints,
                                       vector<FeatureVector> &vResults,
                                       bool bVerbose )
/*******************************************************************/
/* Collect SURF descriptors from a given set of interest points    */
/* (calling an external program).                                  */
/*******************************************************************/
{
  vResults.clear();

  int nRandom = random() % 100000;
  string sRandom   = string( QString::number(nRandom).latin1() );
  string sPath     = PATH_SURF;
  string sCommand;
  if( nFeatureType==FEATURE_SURF128 )
    //sCommand = "surf128.ln ";
    sCommand = "surf.ln -e";
  else
    //sCommand = "surf64.ln";
    sCommand = "surf.ln";
  string sTmpName  = string("./tmp") + sRandom + ".pgm";
  string sIntPtName= string("./tmp") + sRandom + ".key";
  string sFileName = string("./result") + sRandom + ".key";

  sCommand  = ( sPath + sCommand + " -i " + sTmpName + " -o " + sFileName + 
                " -thres " + 
                QString::number(m_guiParams->m_dThreshSURF,'f',1).latin1() +
                " -p1 " + sIntPtName );
  if( !m_guiParams->m_bMakeRotInv )
    sCommand  = sCommand + " -u";
  
  if( bVerbose )
    sCommand  = sCommand + " -v";
  else
    sCommand  = sCommand + " -q > /dev/null";

  /* save the current image as 'tmp.pgm' */
  m_qimgSrc.save( sTmpName.c_str(), "PGM" );
  
  /* write out the given interest points */
  writeInterestFileEllipse( sIntPtName, vPoints, bVerbose );

  /* call the descriptor */
  if( bVerbose ) {
    cout << endl;
    cout << "  Issuing command '" << sCommand << "'..." << endl;
  }
  system( sCommand.c_str() );
  if( bVerbose )
    cout << endl;
  
  /* load the result file */
  loadDescriptorFileSURF( sFileName, vPoints, vResults, 1.0/3.0, bVerbose );
  
  /* remove the temporary files again */
  system( (string("rm -f ") + sTmpName).c_str() );
  system( (string("rm -f ") + sIntPtName).c_str() );
  system( (string("rm -f ") + sFileName).c_str() );
}


/*---------------------------------------------------------*/
/*                   Service Functions                     */
/*---------------------------------------------------------*/

PointVector FeatureCue::loadInterestFileMiko( string sFileName, bool bVerbose )
/*******************************************************************/
/* Load the result file from Mikolajczyk's interest point detec-   */
/* tor program.                                                    */
/*******************************************************************/
{
  PointVector vResults;
  
  ifstream ifile( sFileName.c_str() );
  if( !ifile )
    {
      cerr << "  Error: Couldn't open file '" << sFileName << "'!" << endl;
      return vResults;
    }
  
  /* read the number of interest points */
  int   nNumPoints;
  float dummy;
  ifile >> dummy;
  ifile >> nNumPoints;
  
  for( int i=0; i<nNumPoints; i++ )
    {
      float x, y, value, scale, angle;
      float m11, m12, m21, m22;

      ifile >> x >> y >> value >> scale >> angle >> dummy
            >> dummy >> dummy >> m11 >> m12 >> m21 >> m22;
      
      if( (scale >= m_guiParams->m_dMinScale) &&
          (scale <= m_guiParams->m_dMaxScale) )
        {
          InterestPoint pt;
          pt.x     = (int)floor(x + 0.5);
          pt.y     = (int)floor(y + 0.5);
          pt.value = value;
          pt.scale = scale;
          pt.angle = 0.0;
          pt.l1    = pt.scale;
          pt.l2    = pt.scale;
          
          vResults.push_back( pt );
        }
    }
  
    /* close the file again */
  ifile. close();
  
  if( bVerbose )
    cout << "  Found " << (int)vResults.size() << " points." << endl;

  return vResults;
}


PointVector FeatureCue::loadInterestFileEllipse( string sFileName,
                                                 float dRescaleFact,
                                                 bool bVerbose )
/*******************************************************************/
/* Load the result file from Mikolajczyk's interest point detec-   */
/* tor program (in the newer ellipse format).                      */
/*******************************************************************/
{
  PointVector vResults;
  
  ifstream ifile( sFileName.c_str() );
  if( !ifile )
    {
      cerr << "  Error: Couldn't open file '" << sFileName << "'!" << endl;
      return vResults;
    }
  
  /* read the number of interest points */
  int   nNumPoints;
  float dummy;
  ifile >> dummy;
  ifile >> nNumPoints;
  
  for( int i=0; i<nNumPoints; i++ )
    {
      float x, y, a, b, c;
      float scale, angle;

      //ifile >> x >> y >> value >> scale >> angle >> dummy
      //      >> dummy >> dummy >> m11 >> m12 >> m21 >> m22;
      ifile >> x >> y >> a >> b >> c;
      
      float e1, e2;
      float l1, l2;
      float det = sqrt((a-c)*(a-c) + 4.0*b*b);
      e1 = 0.5*(a+c + det);
      e2 = 0.5*(a+c - det);
      if( e1>e2 ) {     // ensure a canonical orientation
        float e = e1;
        e1 = e2;
        e2 = e;
      }
      l1 = (1.0/sqrt(e1)) * dRescaleFact;
      l2 = (1.0/sqrt(e2)) * dRescaleFact;
      scale = sqrt( l1*l2 );
      
      angle = 0.5 * atan2( (double)2.0*b, (double)c-a );

      if( (scale >= m_guiParams->m_dMinScale) &&
          (scale <= m_guiParams->m_dMaxScale) )
        {
          InterestPoint pt;
          pt.x     = (int)floor(x + 0.5);
          pt.y     = (int)floor(y + 0.5);
          pt.value = 1.0;
          pt.scale = scale;
          pt.angle = angle;
          pt.l1    = l1;
          pt.l2    = l2;
          
          vResults.push_back( pt );
        }
    }
  
    /* close the file again */
  ifile. close();
  
  if( bVerbose )
    cout << "  Found " << (int)vResults.size() << " points." << endl;

  return vResults;
}


PointVector FeatureCue::loadInterestFileLowe( string sFileName, 
                                              bool bVerbose )
/*******************************************************************/
/* Load the result file from Lowe's interest point detector pro-   */
/* gram.                                                           */
/*******************************************************************/
{
  // File format (Lowe)
  //
  // The file format starts with 2 integers giving the total number of
  // keypoints and the size of the descriptor vector for each keypoint
  // (128 by default). Then each keypoint is specified by 4 floating point
  // numbers giving subpixel row and column location, scale, and orientation
  // (in radians from -PI to PI).
  // Then the descriptor vector for the keypoint is given as a list
  // of integers in range [0,255].
  PointVector vResults;
  
  ifstream ifile( sFileName.c_str() );
  if( !ifile )
    {
      cerr << "  Error: Couldn't open file '" << sFileName << "'!" << endl;
      return vResults;
    }
  
  /* read the number of interest points */
  int   nNumPoints;
  int   nDescriptorSize;
  float dummy;
  ifile >> nNumPoints;
  ifile >> nDescriptorSize;
  
  for( int i=0; i<nNumPoints; i++ )
    {
      float x, y, scale, angle;
      
      /* read in the point coordinates */
      ifile >> y >> x >> scale >> angle;
      
      /* skip the descriptor */
      for( int j=0; j<nDescriptorSize; j++ )
        ifile >> dummy ;
      
      if( (scale >= m_guiParams->m_dMinScale) &&
          (scale <= m_guiParams->m_dMaxScale) )
        {
          InterestPoint pt;
          pt.x     = (int)floor(x + 0.5);
          pt.y     = (int)floor(y + 0.5);
          pt.value = 1.0;
          pt.scale = scale;
          pt.angle = angle;
          pt.l1    = pt.scale;
          pt.l2    = pt.scale;
          
          vResults.push_back( pt );
        }
    }
  
  /* close the file again */
  ifile. close();
  
  return vResults;
}


bool FeatureCue::writeInterestFileEllipse( string sFileName, 
                                           const PointVector &vPoints,
                                           bool bVerbose )
/*******************************************************************/
/* Write a file with interest point locations for Mikolajczyk's    */
/* descriptor program (in the newer ellipse format).               */
/*******************************************************************/
{
  ofstream ofile( sFileName.c_str() );
  if( !ofile ) {
      cerr << "  Error: Couldn't open file '" << sFileName << "'!" << endl;
      return false;
    }
  
  /* write the number of interest points */
  int   nNumPoints = (int)vPoints.size();
  ofile << 1.0 << endl;
  ofile << nNumPoints << endl;
  
  for( int i=0; i<nNumPoints; i++ ) {    
    InterestPoint pt = vPoints[i];
        
    /* adjust the scales: multiply by 3 */
    float l1 = pt.l1 * 3.0;
    float l2 = pt.l2 * 3.0;
    float e1, e2;
    e1 = 1.0/(l1*l1);
    e2 = 1.0/(l2*l2);
    
    //angle = 0.5 * atan2( (double)2.0*b, (double)c-a );

    float a, b, c;
    float si   = sin(-pt.angle);
    float si2  = si*si;
    float co   = cos(-pt.angle);
    float co2  = co*co;
    float sico = si*co;
    //a2 = a*co2 - 2.0*b*sico + c*si2;
    //b2 = b*(co2 - si2) + (a-c)*sico;
    //c2 = a*si2 + 2.0*b*sico + c*co2;
    a = e1*co2 + e2*si2;
    b = (e1-e2)*sico;
    c = e1*si2 + e2*co2;
    
    //cout << "    read: a=" << a << ", b=" << b << ", c=" << c << endl;
    //cout << "      => l1=" << l1 << ", l2=" << l2 << ", sc="
    //    << scale << ", angle=" << angle << endl;
    //cout << "        (a2=" << a2 << ", b2=" << b2 << ", c2=" << c2 << ")"
    //     << endl;

    ofile << pt.x << " " << pt.y << " " <<  a << " " <<  b << " " << c << endl;
  }
  
  /* close the file again */
  ofile << endl << flush;
  ofile. close();
  
  if( bVerbose )
    cout << "  Wrote " << nNumPoints << " points." << endl;
  
  return true;
}


void FeatureCue::loadDescriptorFileEllipse( string sFileName,
                                            vector<FeatureVector> &vResults,
                                            bool bVerbose )
/*******************************************************************/
/* Load the result file from Mikolajczyk's descriptor program      */
/* (in the newer ellipse format).                                  */
/*******************************************************************/
{
  vResults.clear();
  
  ifstream ifile( sFileName.c_str() );
  if( !ifile ) {
      cerr << "  Error: Couldn't open file '" << sFileName << "'!" << endl;
      return;
    }
  
  /* read the number of points and dimensionality */
  int   nDim;
  int   nNumPoints;
  ifile >> nDim;
  ifile >> nNumPoints;
  
  for( int i=0; i<nNumPoints; i++ ) {
    float x, y, a, b, c;
    
    ifile >> x >> y >> a >> b >> c;

    FeatureVector fvNew( nDim );
    for( int j=0; j<nDim; j++ )
      ifile >> fvNew.at(j);

    vResults.push_back( fvNew );
  }
  
  /* close the file again */
  ifile. close();
  
  if( bVerbose )
    cout << "  Read in " << (int)vResults.size() << " descriptors." << endl;
}


void FeatureCue::loadDescriptorFileEllipse( string sFileName,
                                            PointVector           &vPoints,
                                            vector<FeatureVector> &vResults,
                                            float dRescaleFact,
                                            bool bVerbose )
/*******************************************************************/
/* Load the result file from Mikolajczyk's descriptor program      */
/* (in the newer ellipse format).                                  */
/*******************************************************************/
{
  vPoints.clear();
  vResults.clear();
  
  ifstream ifile( sFileName.c_str() );
  if( !ifile ) {
      cerr << "  Error: Couldn't open file '" << sFileName << "'!" << endl;
      return;
    }
  
  /* read the number of points and dimensionality */
  int   nDim;
  int   nNumPoints;
  ifile >> nDim;
  ifile >> nNumPoints;
  
  for( int i=0; i<nNumPoints; i++ ) {
    /*-------------------------*/
    /* Load the interest point */
    /*-------------------------*/
    float x, y, a, b, c;    
    float scale, angle;

    //ifile >> x >> y >> value >> scale >> angle >> dummy
    //      >> dummy >> dummy >> m11 >> m12 >> m21 >> m22;
    ifile >> x >> y >> a >> b >> c;
    
    float e1, e2;
    float l1, l2;
    float det = sqrt((a-c)*(a-c) + 4.0*b*b);
    e1 = 0.5*(a+c + det);
    e2 = 0.5*(a+c - det);
    if( e1>e2 ) {     // ensure a canonical orientation
      float e = e1;
      e1 = e2;
      e2 = e;
    }
    l1 = (1.0/sqrt(e1)) * dRescaleFact;
    l2 = (1.0/sqrt(e2)) * dRescaleFact;
    scale = sqrt( l1*l2 );
    
    angle = 0.5 * atan2( (double)2.0*b, (double)c-a );
    
    bool bAcceptPoint = false;
    if( (scale >= m_guiParams->m_dMinScale) &&
        (scale <= m_guiParams->m_dMaxScale) ) {
      InterestPoint pt;
      pt.x     = (int)floor(x + 0.5);
      pt.y     = (int)floor(y + 0.5);
      pt.value = 1.0;
      pt.scale = scale;
      pt.angle = angle;
      pt.l1    = l1;
      pt.l2    = l2;
    
      bAcceptPoint = true;
      vPoints.push_back( pt );
    }
       
    /*---------------------*/
    /* Load the descriptor */
    /*---------------------*/
    FeatureVector fvNew( nDim );
    for( int j=0; j<nDim; j++ )
      ifile >> fvNew.at(j);

    if( bAcceptPoint )
      vResults.push_back( fvNew );
  }
  
  /* close the file again */
  ifile. close();
  
  if( bVerbose )
    cout << "  Read in " << (int)vResults.size() << " descriptors." << endl;
}


void FeatureCue::loadDescriptorFileSURF( string sFileName,
                                         vector<FeatureVector> &vResults,
                                         float dRescaleFact,
                                         bool bVerbose )
/*******************************************************************/
/* Load the result file from Mikolajczyk's descriptor program      */
/* (in the newer ellipse format).                                  */
/*******************************************************************/
{
  vResults.clear();
  
  ifstream ifile( sFileName.c_str() );
  if( !ifile ) {
      cerr << "  Error: Couldn't open file '" << sFileName << "'!" << endl;
      return;
    }
  
  /* read the number of points and dimensionality */
  int   nDim;
  int   nNumPoints;
  ifile >> nDim;
  ifile >> nNumPoints;
  
  for( int i=0; i<nNumPoints; i++ ) {
    /*-------------------------*/
    /* Load the interest point */
    /*-------------------------*/
    float x, y, a, b, c;    
    float scale, angle;

    //ifile >> x >> y >> value >> scale >> angle >> dummy
    //      >> dummy >> dummy >> m11 >> m12 >> m21 >> m22;
    ifile >> x >> y >> a >> b >> c;
    
//     float e1, e2;
    float l1, l2;
//     float det = sqrt((a-c)*(a-c) + 4.0*b*b);
//     e1 = 0.5*(a+c + det);
//     e2 = 0.5*(a+c - det);
//     if( e1>e2 ) {     // ensure a canonical orientation
//       float e = e1;
//       e1 = e2;
//       e2 = e;
//     }
//     l1 = (1.0/sqrt(e1)) * dRescaleFact;
//     l2 = (1.0/sqrt(e2)) * dRescaleFact;
//     scale = sqrt( l1*l2 );
    scale = (1.0/sqrt(a))*dRescaleFact;
    l1    = scale;
    l2    = scale; 
    
//     angle = 0.5 * atan2( (double)2.0*b, (double)c-a );
    angle = 0.0;
    
    bool bAcceptPoint = false;
    if( (scale >= m_guiParams->m_dMinScale) &&
        (scale <= m_guiParams->m_dMaxScale) ) {
      bAcceptPoint = true;
    }
    
    // load the sign of the laplacian
    float dummy;
    ifile >> dummy;

    /*---------------------*/
    /* Load the descriptor */
    /*---------------------*/
    FeatureVector fvNew( nDim-1 );
    for( int j=0; j<nDim-1; j++ )
      ifile >> fvNew.at(j);

    if( bAcceptPoint )
      vResults.push_back( fvNew );
  }
  
  /* close the file again */
  ifile. close();
  
  if( bVerbose ) 
    cout << "  Read in " << (int)vResults.size() << " descriptors." << endl;
}


void FeatureCue::loadDescriptorFileSURF( string sFileName,
                                         PointVector           &vPoints,
                                         vector<FeatureVector> &vResults,
                                         float dRescaleFact,
                                         bool bVerbose )
/*******************************************************************/
/* Load the result file from Mikolajczyk's descriptor program      */
/* (in the newer ellipse format).                                  */
/*******************************************************************/
{
  vPoints.clear();
  vResults.clear();
  
  ifstream ifile( sFileName.c_str() );
  if( !ifile ) {
      cerr << "  Error: Couldn't open file '" << sFileName << "'!" << endl;
      return;
    }
  
  /* read the number of points and dimensionality */
  int   nDim;
  int   nNumPoints;
  ifile >> nDim;
  ifile >> nNumPoints;
  
  for( int i=0; i<nNumPoints; i++ ) {
    /*-------------------------*/
    /* Load the interest point */
    /*-------------------------*/
    float x, y, a, b, c;    
    float scale, angle;

    //ifile >> x >> y >> value >> scale >> angle >> dummy
    //      >> dummy >> dummy >> m11 >> m12 >> m21 >> m22;
    ifile >> x >> y >> a >> b >> c;
    
//     float e1, e2;
    float l1, l2;
//     float det = sqrt((a-c)*(a-c) + 4.0*b*b);
//     e1 = 0.5*(a+c + det);
//     e2 = 0.5*(a+c - det);
//     if( e1>e2 ) {     // ensure a canonical orientation
//       float e = e1;
//       e1 = e2;
//       e2 = e;
//     }
//     l1 = (1.0/sqrt(e1)) * dRescaleFact;
//     l2 = (1.0/sqrt(e2)) * dRescaleFact;
//     scale = sqrt( l1*l2 );
    scale = (1.0/sqrt(a))*dRescaleFact;
    l1    = scale;
    l2    = scale; 
    
//     angle = 0.5 * atan2( (double)2.0*b, (double)c-a );
    angle = 0.0;
    
    bool bAcceptPoint = false;
    if( (scale >= m_guiParams->m_dMinScale) &&
        (scale <= m_guiParams->m_dMaxScale) ) {
      InterestPoint pt;
      pt.x     = (int)floor(x + 0.5);
      pt.y     = (int)floor(y + 0.5);
      pt.value = 1.0;
      pt.scale = scale;
      pt.angle = angle;
      pt.l1    = l1;
      pt.l2    = l2;
    
      bAcceptPoint = true;
      vPoints.push_back( pt );
    }
    
    // load the sign of the laplacian
    float dummy;
    ifile >> dummy;

    /*---------------------*/
    /* Load the descriptor */
    /*---------------------*/
    FeatureVector fvNew( nDim-1 );
    for( int j=0; j<nDim-1; j++ )
      ifile >> fvNew.at(j);

    if( bAcceptPoint )
      vResults.push_back( fvNew );
  }
  
  /* close the file again */
  ifile. close();
  
  if( bVerbose ) 
    cout << "  Read in " << (int)vResults.size() << " descriptors." << endl;
}


void FeatureCue::extractAffineRegion( OpGrayImage img, InterestPoint pt,
                                      int nPatchSize,
                                      //OpGrayImage &imgBig,
                                      //OpGrayImage &imgRot,
                                      //OpGrayImage &imgRegion,
                                      OpGrayImage &imgResult )
/*********************************************************************/
/* Extract an affine region (defined by an ellipse) from the image,  */
/* rescale it to a square, and return it.                            */
/*********************************************************************/
{
  OpGrayImage imgBig, imgRot, imgRegion;

  int   x     = (int)pt.x;
  int   y     = (int)pt.y;
  float l1    = pt.l1;
  float l2    = pt.l2;
  float angle = pt.angle;

  /*-------------------------*/
  /* Extract a bigger region */
  /*-------------------------*/
  float scfact = m_guiParams->m_dScaleFactor;
  float cosa = cos(angle)*scfact;
  float sina = sin(angle)*scfact;
  float rx = fabs(l1*cosa) + fabs(l2*sina);
  float ry = fabs(l1*sina) + fabs(l2*cosa);
  int   r  = (int)floor( max( rx, max( ry, max(l1,l2) ) ) + 0.5);

  imgBig = img.extractRegion( x-r, y-r, x+r, y+r );
  
  /*-----------------------------*/
  /* Rotate the region by -alpha */
  /*-----------------------------*/
  imgRot = imgBig.rotate( -angle );

  /*--------------------------------*/
  /* Extract the rectangular region */
  /*--------------------------------*/
  int mx = imgRot.width()/2;
  int my = mx;
  imgRegion = imgRot.extractRegion( (int)(mx-l1), (int)(my-l2), 
                                    (int)(mx+l1*scfact), (int)(my+l2*scfact) );

  /*---------------------------*/
  /* Rescale it to square size */
  /*---------------------------*/
  imgResult = imgRegion.opRescaleToSize( nPatchSize, nPatchSize );
}
