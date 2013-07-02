/*********************************************************************/
/*                                                                   */
/* FILE         edgesift.cc                                          */
/* AUTHORS      Oliver Bay, Bastian Leibe                            */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Implementation of an edge-based feature descriptor   */
/*              based on the paper:                                  */
/*                                                                   */
/*              K. Mikolajczyk, A. Zisserman, and C. Schmid          */
/*              Shape Recognition with Edge-Based Features.          */
/*              In Proc. British Machine Vision Conference (BMVC'03) */
/*              Norwich, GB, Sept. 9-11, 2003.                       */
/*                                                                   */
/* BEGIN        Fri Mar 05 2004                                      */
/* LAST CHANGE  Fri Mar 05 2004                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <math.h>

#include <dogscalespace.hh>
#include <histogram.hh>

#include "polcoe.h"

#include "gauss.hh"
#include "edgesift.hh"

/******************/
/*   Prototypes   */
/******************/
void          computeDominantAngles( OpGrayImage imgMag, OpGrayImage imgDirs,
                                     InterestPoint pt, int nRadius,
                                     vector<float> &vDomAngles );

FeatureVector createFeatureVector16 ( const OpGrayImage& imgPatch, 
                                      const OpGrayImage& imgDirs );
FeatureVector createFeatureVector128( const OpGrayImage& imgPatch, 
                                      const OpGrayImage& imgDirs );

void          calculate128dCaseI    ( const OpGrayImage& imgPatch, 
                                      const OpGrayImage& imgDirs,
                                      FeatureVector& vFeatures );
void          calculate128dCaseII   ( const OpGrayImage& imgPatch, 
                                      const OpGrayImage& imgDirs,
                                      FeatureVector& vFeatures );

int getWeight16       ( int i, int j, int x, int y, int nSize, int nWindow );
int getWeight128CaseI ( int i, int j, int x, int y, int nSize, int nWindow );
int getWeight128CaseII( int i, int j, int x, int y, int nSize, int nWindow );

int getX              ( int nWindowNr, int nSize );
int getY              ( int nWindowNr, int nSize );

void addToHistogram   ( double dDirVal, float min, float max,  int nWeight,
                        Histogram& hWindow );

// outdated:
void applyEdgeScaleSpace( OpGrayImage img, 
                          int nNumOctaves, int nLevelsPerOctave, 
                          float dScaleSigma0,
                          int nthEdgePixel, bool bInterpolate,
                          vector<OpGrayImage> &vEdgeImages,
                          vector<float> &vEdgeScales,
                          PointVector &vPoints );

/**********************/
/*   Implementation   */
/**********************/

void applyEdgeScaleSpace( OpGrayImage img, 
                          int nNumOctaves, int nLevelsPerOctave, 
                          float dScaleSigma0, bool bInterpolate, 
                          float dEdgeMinSigma, float dEdgeMaxSigma, 
                          float dEdgeIncSigma,
                          float dThreshCannyLo, float dThreshCannyHi,
                          int nthEdgePixel, float dEdgeThresh,
                          float dScaleFactor, bool bNormOri,
                          PointVector &vPoints,
                          vector<OpGrayImage> &vEdgePatches,
                          vector<OpGrayImage> &vDirPatches )
  /*******************************************************************/
  /* Create an Edge Scale Space   */
  /*******************************************************************/
{
  cout << "  ::applyEdgeScaleSpace( " << nNumOctaves << ", " 
       << nLevelsPerOctave << ", " << nthEdgePixel << ", " << bInterpolate 
       << " ) called." << endl;

  vPoints.clear();
  
  /****************************/
  /* Create a DoG Scale Space */
  /****************************/
  DoGScaleSpace dogScale( img, nNumOctaves, nLevelsPerOctave, dScaleSigma0 );

  /****************************************/
  /* Create edge images at several scales */
  /****************************************/
  //double dMinSigma = sqrt(2.0)/2.0;
  //double dFactor   = sqrt(2.0);
  //double dLimit    = 8.0f;

  PointVector vEdgePoints;
  vEdgePatches.clear();
  vDirPatches.clear();
  long nNumEdgePixels = 0;
  long nNumBeforeScales = 0;
  long nNumBeforeThresh = 0;
  long nNumBeforeAngles = 0;
  for ( double dSigma=dEdgeMinSigma; dSigma<dEdgeMaxSigma; 
        dSigma*=dEdgeIncSigma ) {
    cout << "    Generating edge image with sigma=" << dSigma << "..." << endl;
    
    /* Extract edges at the current scale */
    OpGrayImage imgEdges, imgMag, imgDirs;
    img.opCannyEdges( dSigma, dThreshCannyLo, dThreshCannyHi, 
                      imgEdges, imgMag, imgDirs );

    // For Debugging only:
    //     InterestPoint ptTmp;
    //     ptTmp.x = 20;
    //     ptTmp.y = 20;
    //     ptTmp.scale=2.0;
    //     ptTmp.value=100.0;
    //     vPoints.push_back( ptTmp );
    //     vEdgePatches.push_back( imgEdges );

    /* Scan the image for edge pixels */
    for ( int y=0; y<imgEdges.height(); y++ )
      for ( int x=0; x<imgEdges.width(); x++ ) {
        
        // 0 -> Edge, 255 -> No Edge (ideal)
        if ( imgEdges(x,y).value() == 0.0 ) {
          InterestPoint ptEdge;
          ptEdge.x = x;
          ptEdge.y = y;
          //vEdgePoints.push_back( ptEdge );
          nNumEdgePixels++;

          /*-----------------------------------*/
          /* Extract the characteristic scales */
          /*-----------------------------------*/
          /* Take only every nth edge pixel */
          if ( nNumEdgePixels % nthEdgePixel == 0 ) {        
            /* get all characteristic scales for this pixel... */
            vector<float> vScales;
            vector<float> vValues;
            dogScale.getCharacteristicScales( x, y, 
                                              vScales, vValues, 
                                              bInterpolate );

            nNumBeforeScales++;

            /* ...and generate a new interest point for it */
            for ( int j=0; j<vScales.size(); j++ ) {
              nNumBeforeThresh++;

              /*-------------------*/
              /* Check a threshold */
              /*-------------------*/
              /* if the scale value exceeds a certain threshold */
              if( vValues[j] >= dEdgeThresh ) {
                InterestPoint pt( ptEdge );
                pt.value = vValues[j];
                pt.scale = vScales[j];
                
                
                /* extract the corresponding edge patch */
                int nPatchSize = (int) floor(pt.scale*dScaleFactor + 0.5);
                OpGrayImage imgEdgePatch;
                imgEdgePatch = imgEdges.extractRegion( pt.x - nPatchSize,
                                                       pt.y - nPatchSize,
                                                       pt.x + nPatchSize,
                                                       pt.y + nPatchSize );

                OpGrayImage imgDirPatch;
                imgDirPatch = imgDirs.extractRegion( pt.x - nPatchSize,
                                                     pt.y - nPatchSize,
                                                     pt.x + nPatchSize,
                                                     pt.y + nPatchSize );

                nNumBeforeAngles++;

                /*-------------------------------*/
                /* Extract dominant orientations */
                /*-------------------------------*/
                if( !bNormOri ) {
                  /* no orientation normalization */
                  InterestPoint ptAngle( pt );
                  ptAngle.angle = 0.0;
                  vPoints.push_back( ptAngle );
                  
                  vEdgePatches.push_back( imgEdgePatch );
                  vDirPatches.push_back( imgDirPatch );

                } else {
                  /* compute all dominant orientations for this patch */
                  vector<float> vAngles;
                  computeDominantAngles( imgMag, imgDirs, pt, nPatchSize,
                                         vAngles );
                  for( int k=0; k<vAngles.size(); k++ ) {
                    InterestPoint ptAngle( pt );
                    ptAngle.angle = vAngles[k];
                    
                    vPoints.push_back( ptAngle );
                    
                    /* rotate the edge patch by the dominant orientation */
                    OpGrayImage imgRotated;
                    imgRotated = imgEdgePatch.rotate( -vAngles[k] + M_PI/2.0, 
                                                      255.0 );
                    vEdgePatches.push_back( imgRotated );
                    
                    imgRotated = imgDirPatch.rotate( -vAngles[k] + M_PI/2.0, 
                                                     255.0 );
                    vDirPatches.push_back( imgRotated );
                  }
                } // end if( !bNormOri )
              } // end if( vValues[j] >= dEdgeThresh )
            } // end forall scales
          } // end if( nNumEdgePixels % nthEdgePixel == 0 )
          
          } // end if( imgEdges(x,y).value() == 0.0 )
        } // end forall edge pixels
  } // end forall edge scalespace levels
  
  /* Sort the interest point list */
  //stable_sort( vPoints.begin(), vPoints.end(), compInterestPtsScale() );
  
  /* print a short report */
  cout << "    #edge points:\t" << nNumEdgePixels << endl;
  cout << "    #before scales:\t" << nNumBeforeScales << endl;  
  cout << "    #before thresh:\t" << nNumBeforeThresh << endl;  
  cout << "    #before angles:\t" << nNumBeforeAngles << endl;  
  cout << "    #interest points:\t" << vPoints.size() << endl;  
  cout << "    #patches:    \t" << vEdgePatches.size() << endl;  
  cout << "  done." << endl;
}


void computeDominantAngles( OpGrayImage imgMag, OpGrayImage imgDirs,
                            InterestPoint pt, int nRadius,
                            vector<float> &vDomAngles )
{
  int   nBins   = 36;
  float dOffset = (M_PI/(float)nBins); // width of half a bin in radians
  float dMinOri = dOffset;
  float dMaxOri = 2.0*M_PI + dOffset;
  float dBinSize= (dMaxOri - dMinOri)/(float)nBins;
  Histogram   hOrientations( 36, dMinOri, dMaxOri );
  //VotingSpace vsOrientations( 36, dMinOri, dMaxOri );

  /*----------------------------------------*/
  /* Create a histogram of all orientations */
  /*----------------------------------------*/
  int minx = (pt.x-nRadius < 0)? 0 : pt.x-nRadius;
  int miny = (pt.y-nRadius < 0)? 0 : pt.y-nRadius;
  int maxx = (pt.x+nRadius>=imgDirs.width())? imgDirs.width(): pt.x+nRadius;
  int maxy = (pt.y+nRadius>=imgDirs.height())? imgDirs.height(): pt.y+nRadius;
  int nRadius2 = nRadius*nRadius;
  for( int y=miny; y<maxy; y++ )
    for( int x=minx; x<maxx; x++ ) {
      float r2 = (x-pt.x)*(x-pt.x) + (y-pt.y)*(y-pt.y);
      if( r2 <= (float) nRadius2 ) {
        /* formula according to [Lowe, IJCV, 04] */
//         float val = ( imgDirs(x,y).value()*imgMag(x,y).value() *
//                       evaluateUnnormalizedGauss( 2, r2, (float) nRadius ) );
        float val = ( imgDirs(x,y).value()*imgMag(x,y).value() );
        /* add the pixel orientation to the histogram */
        hOrientations.incrementValue( imgDirs(x,y).value(), val );
        //vsOrientations.insertVote( HoughVote( imgDirs(x,y).value(), val, 
        //                                      1.0,0,0,0 ) );
      }
    }

  /*------------------------------------------------*/
  /* Search for maxima in the orientation histogram */
  /*------------------------------------------------*/
  vector<float> vAngles;
  vector<float> vValues;
  vector<int>   vBinIdzs;
  /* version for histograms */
  for( int i=1; i<nBins-1; i++ )
    if( (hOrientations.at(i) > hOrientations.at(i-1)) &&
        (hOrientations.at(i) > hOrientations.at(i+1)) ) {
      /* local maximum */
      vAngles.push_back( 1.5*dOffset + i*dBinSize );
      vValues.push_back( hOrientations.at(i) );
      vBinIdzs.push_back( i );
    }
  if( (hOrientations.at(0) > hOrientations.at(nBins-1)) &&
      (hOrientations.at(0) > hOrientations.at(1)) ) {
    vAngles.push_back( 1.5*dOffset );
    vValues.push_back( hOrientations.at(0) );
    vBinIdzs.push_back( 0 );
  }
  if( (hOrientations.at(nBins-1) > hOrientations.at(nBins-2)) &&
      (hOrientations.at(nBins-1) > hOrientations.at(0)) ) {
    vAngles.push_back( 1.5*dOffset + (nBins-1)*dBinSize );
    vValues.push_back( hOrientations.at(nBins-1) );
    vBinIdzs.push_back( nBins-1 );
  }

  /* version for MSME search */
  //   float dWindowSize = 20.0;
  //   vAngles.clear();
  //   vValues.clear();
  //   vsOrientations.setWindowSize( dWindowSize );
  //   for( int i=1; i<nBins-1; i++ )
  //     ...

  /*------------------------------------------------------*/
  /* Keep only the orientations within 80% of the maximum */
  /*------------------------------------------------------*/
  float dMaxVal = 0;
  for( int i=0; i<vValues.size(); i++ )
    if( vValues[i] > dMaxVal )
      dMaxVal = vValues[i];
  
  vector<int> vFinalBins;
  vDomAngles.clear();
  for( int i=0; i<vValues.size(); i++ )
    if( vValues[i] >= 0.8*dMaxVal ) {
      vDomAngles.push_back( vAngles[i] );
      vFinalBins.push_back( vBinIdzs[i] );
    }

  /*-------------------------------------------------------*/
  /* Interpolate with a parabola to find the exact maximum */
  /*-------------------------------------------------------*/
  for( int i=0; i<vDomAngles.size(); i++ ) {
    /* interpolate the orientation using a 2nd order polynomial      */
    float idx[3];
    float val[3];

    idx[0] = vDomAngles[i] - dBinSize;
    idx[1] = vDomAngles[i];
    idx[2] = vDomAngles[i] + dBinSize;

    val[0] = ( (vFinalBins[i]>=1)? 
               hOrientations.at(vFinalBins[i]-1): hOrientations.at(nBins-1) );
    val[1] = hOrientations.at(vFinalBins[i]);
    val[2] = ( (vFinalBins[i]<nBins-1)? 
               hOrientations.at(vFinalBins[i]-1) : hOrientations.at(0) );
    
    float c[3];
    /* interpolate the polynomial */
    polcoe( idx, val, 2, c );
  
    /* find the maximum: the parabole's apex is at           */
    /*   ( -c1/2c2 | -(c1^2-4c0c2)/4c2 )                     */
    float dAngle = -c[1]/(2.0*c[2]);
    float dValue = -(c[1]*c[1]-4.0*c[0]*c[2])/(4.0*c[2]);
    
    if( dAngle < 0.0 )
      dAngle += 2*M_PI;
    if( dAngle > 2*M_PI )
      dAngle -= 2*M_PI;

    vDomAngles[i] = dAngle;
  }
}


/***************************************************************************/
/*                         Feature Vector 16D, 128D                        */
/***************************************************************************/

void createEdgeFeatures( /*const*/ PointVector &vPoints,
                         /*const*/ vector<OpGrayImage> &vEdgePatches,
                         /*const*/ vector<OpGrayImage> &vDirPatches,
                                   vector<FeatureVector> &vFeatures16d,
                                   vector<FeatureVector> &vFeatures128d )
{
  // Debug information
  cout << "  createEdgeFeatures() called." << endl;
  
  // Clear feature vectors
  vFeatures16d.clear();
  vFeatures128d.clear();
  
  cout << "    Calculating feature vectors..." << endl;
  
  for ( int i=0; i<vEdgePatches.size() && i<vDirPatches.size(); i++ ) {
    FeatureVector fv16, fv128;
    
    // Create feature vectors
     fv16  = createFeatureVector16 ( vEdgePatches[i], vDirPatches[i] );
     fv128 = createFeatureVector128( vEdgePatches[i], vDirPatches[i] );
    
    // Normalize sum
    fv16.normalizeVector ( 100.0 );
    fv128.normalizeVector( 100.0 );
    
    vFeatures16d.push_back ( fv16  );
    vFeatures128d.push_back( fv128 );   
  }
  
  cout << "    #patches      :\t" << vEdgePatches.size() << endl;  
  cout << "    #features 16D :\t" << vFeatures16d.size() << endl;
  cout << "    #features 128D:\t" << vFeatures128d.size() << endl;  
  cout << "  done." << endl;
}


/***************************************************************************/
/*                            Feature Vector 16D                           */
/***************************************************************************/

FeatureVector createFeatureVector16( const OpGrayImage& imgPatch, 
                                     const OpGrayImage& imgDirs )
{  
  FeatureVector vFeatures(16);

  // Scan window
  int nSize   = imgPatch.width()/2 + 1;
  int nOffset = imgPatch.width()/2;

  // Histogram settings
  int   nNumBins = 4;
  float dBinSize = (M_PI / (float)nNumBins);
  float min      = (float) ( -dBinSize/2.0 );
  float max      = (float) ( -dBinSize/2.0 + M_PI );
  
  // Index in feature vector
  int index       = 0;

  // Scan window (0=upper left, 1=upper right, 2=lower left, 3=lower right )
  int nWindow     = 0;

  // Scan image
  for ( int y=0; y<imgPatch.height()-1; y+=nOffset )
    for ( int x=0; x<imgPatch.width()-1; x+=nOffset ) {
      // Histogram of scan window
      Histogram hWindow( nNumBins, min, max );
      
      // Scan window
      for ( int j=y; j<y+nSize; j++ )
        for ( int i=x; i<x+nSize; i++ ) {
          // compensate for bin overlaps
          int nWeight = getWeight16( i, j, x, y, nSize, nWindow );

          // if the pixel contains an edge
          if( imgPatch(i,j).value() <= THRESH_ROT_EDGE )
            // add the edge direction to the histogram
            addToHistogram( imgDirs(i,j).value(), min, max, nWeight,
                            hWindow );
        }

      // Set corresponding values in feature vector
      for ( int k=index; k<index+nNumBins; k++ ) {
        if ( k < vFeatures.numDims() ) {
          vFeatures.setValue( k, hWindow.at( k % nNumBins ) );
        }
        else {
          cerr << "    Error: Invalid feature vector index: " << k 
               << " (size: " << vFeatures.numDims() << ")." << endl;
        }
      }
      // Set current position
      index += nNumBins;
      
      // Set next window
      nWindow++;
    }

  return vFeatures;
}


int getWeight16( int i, int j, int x, int y, int nSize, int nWindow )
{
  int nWeight = -1;

  switch( nWindow ) {
  case 0: // Upper left window
    if      ( (i == (x+nSize-1)) && (j == (y+nSize-1)) ) 
      nWeight = 1;
    else if ( (i == (x+nSize-1)) || (j == (y+nSize-1)) ) 
      nWeight = 2;
    else                                               
      nWeight = 4;
    break;

  case 1: // Upper left window
    if      ( (i == x) && (j == (y+nSize-1)) )
      nWeight = 1;
    else if ( (i == x) || (j == (y+nSize-1)) ) 
      nWeight = 2;
    else
      nWeight = 4;
    break;

  case 2: // Lower left window
    if      ( (i == (x+nSize-1)) && (j == y) ) 
      nWeight = 1;
    else if ( (i == (x+nSize-1)) || (j == y) )
      nWeight = 2;
    else
      nWeight = 4;
    break;

  case 3: // Lower right window
    if      ( (i == x) && (j == y) )
      nWeight = 1;
    else if ( (i == x) || (j == y) )
      nWeight = 2;
    else
      nWeight = 4;
    break;

  default:
    cerr << "    Error in getWeight16(): "
         << "Illegal window index (" << nWindow << ")!" << endl;
  }

  return nWeight;
}


void addToHistogram( double dDirVal, float min, float max,  int nWeight,
                     Histogram& hWindow )
{
    float dGradient = dDirVal;
    float dEdge     = dGradient - (M_PI / 2.0);
    
    if ( dEdge < min ) {
      dEdge += 2*M_PI;
    }
    
    if ( dEdge < max ) {
      hWindow.incrementValue( dEdge, (float) nWeight );
    }
    else if ( (max < dEdge) && (dEdge < (max + M_PI)) ) {
      hWindow.incrementValue( dEdge - M_PI, (float) nWeight );
    }
    else {
      hWindow.incrementValue( dEdge - 2*M_PI, (float) nWeight );
    }
}


/***************************************************************************/
/*                            Feature Vector 128D                          */
/***************************************************************************/

FeatureVector createFeatureVector128( const OpGrayImage& imgPatch, 
                             const OpGrayImage& imgDirs ) 
{
  FeatureVector vFeatures(128);

  if ( imgPatch.width() % 4 == 1 ) {
    calculate128dCaseI ( imgPatch, imgDirs, vFeatures );
  }
  else if ( imgPatch.width() % 4 == 3 ) {
    calculate128dCaseII( imgPatch, imgDirs, vFeatures );    
  }
  else {
    // Error
    cerr << "    Error in createFeatureVector128(): "
         << " Invalid patch size!" << endl;
  } 
  
  return vFeatures;
}


void calculate128dCaseI ( const OpGrayImage& imgPatch, 
                          const OpGrayImage& imgDirs,
                          FeatureVector& vFeatures )
{
  // Scan window
  int nSize   = ( imgPatch.width() + 3 ) / 4;
  int nOffset = nSize - 1;

  // Histogram settings
  int   nNumBins = 8;
  float nBinSize = (M_PI / nNumBins);
  float min      = (float) ( -nBinSize/2.0 );
  float max      = (float) ( -nBinSize/2.0 + M_PI );

  // Index in feature vector
  int index       = 0;
  
  // Scan window 
  int nWindow      = 0;

  for ( int y=0; y<imgPatch.height()-1; y+=nOffset )
    for ( int x=0; x<imgPatch.width()-1; x+=nOffset ) {
      // Histogram of scan window
      Histogram hWindow( nNumBins, min, max );

      // Scan window
      for ( int j=y; j<y+nSize; j++ )
        for ( int i=x; i<x+nSize; i++ ) {
          // compensate for bin overlaps
          int nWeight = getWeight128CaseI( i, j, x, y, nSize, nWindow );

          // if the pixel contains an edge
          if( imgPatch(i,j).value() <= THRESH_ROT_EDGE )
            // add the edge direction to the histogram
            addToHistogram( imgDirs(i,j).value(), min, max, nWeight,
                            hWindow );
        }

      // Set corresponding values in feature vector
      for ( int k=index; k<index+nNumBins; k++) {
        if ( k < vFeatures.numDims() ) {
          vFeatures.setValue( k, hWindow.at( k % nNumBins ) );
        }
        else {
          cerr << "    Error: invalid feature vector index: " << k 
               << " (size: " << vFeatures.numDims() << ")." << endl;
        }
      }
      // Set current position
      index += nNumBins;

      // Set next window
      nWindow++;      
    }
}


void calculate128dCaseII( const OpGrayImage& imgPatch, 
                          const OpGrayImage& imgDirs,
                          FeatureVector& vFeatures )
{
  // Scan window
  int nSize   = ( imgPatch.width() + 1 ) / 4;

  // Histogram settings
  int   nNumBins = 8;
  float dBinSize = (M_PI / nNumBins);
  float min       = (float) ( -dBinSize/2.0 );
  float max       = (float) ( -dBinSize/2.0 + M_PI );

  // Index in feature vector
  int index       = 0;

  // Position of scan window
  int x = 0;
  int y = 0;
  for ( int nWindow=0; nWindow<16; nWindow++ ) {
    x = getX( nWindow, nSize );
    y = getY( nWindow, nSize );

    // Histogram of scan window
    Histogram hWindow( nNumBins, min, max );

    // Scan window
    for ( int j=y; j<y+nSize; j++ )
      for ( int i=x; i<x+nSize; i++ ) {
        // compensate bin overlaps
        int nWeight = getWeight128CaseII( i, j, x, y, nSize, nWindow );

        // if the pixel contains an edge
        if( imgPatch(i,j).value() <= THRESH_ROT_EDGE )
          // add the edge direction to the histogram
          addToHistogram( imgDirs(i,j).value(), min, max, nWeight,
                        hWindow );
      }
    
    // Set corresponding values in feature vector
    for ( int k=index; k<index+nNumBins; k++) {
      if ( k < vFeatures.numDims() ) {
        vFeatures.setValue( k, hWindow.at( k % nNumBins ) );
      } else {
        cerr << "    Error: invalid feature vector index: " << k 
             << " (size: " << vFeatures.numDims() << ")." << endl;
      }
    }
    // Set current position
    index += nNumBins;    
  }
}


int getWeight128CaseI ( int i, int j, int x, int y, int nSize, int nWindow )
{
  // Window numbering
  //
  //  0  1  2  3
  //  4  5  6  7
  //  8  9 10 11
  // 12 13 14 15
  //
  int nWeight = -1;
  
  switch( nWindow ) {
  case 0: // UPPER LEFT CORNER
    nWeight = getWeight16( i, j, x, y, nSize, 0 );
    break;

  case 4:
  case 8: // LEFT SIDE
    if      ( (i==(x+nSize-1)) && ((j==y || j==(y+nSize-1))) )       
      nWeight = 1;
    else if ( (i==(x+nSize-1)) ||   j==y || j==(y+nSize-1)   )       
      nWeight = 2;
    else                                                           
      nWeight = 4;
    break;

  case 12: // LOWER LEFT CORNER
    return getWeight16( i, j, x, y, nSize, 2 );
    break;

  case 1:
  case 2: // UPPER SIDE
    if      ( (i==x || i==(x+nSize-1)) && j==(y+nSize-1) )           
      { return 1; }
    else if (  i==x || i==(x+nSize-1)  || j==(y+nSize-1) )           
      { return 2; }
    else                                                           
      { return 4; }
    break;

  case 5:
  case 6:
  case 9:
  case 10: // MIDDLE SQUARES
    if      ( (i==x || i==(x+nSize-1)) && (j==y || j==(y+nSize-1)) ) 
      { return 1; }
    else if (  i==x || i==(x+nSize-1)  ||  j==y || j==(y+nSize-1)  ) 
      { return 2; }
    else                                                           
      { return 4; }
    break;
    
  case 13:
  case 14: // LOWER SIDE
    if      ( (i==x || i==(x+nSize-1)) && j==y )                    
      { return 1; }
    else if (  i==x || i==(x+nSize-1)  || j==y )                    
      { return 2; }
    else                                                           
      { return 4; }
    break;

  case 3: // UPPER RIGHT CORNER
    return getWeight16( i, j, x, y, nSize, 1 );
    break;

  case 7:
  case 11: // RIGHT SIDE
    if      ( i==x && (j==y || j==(y+nSize-1)) )                    
      { return 1; }
    else if ( i==x ||  j==y || j==(y+nSize-1)  )                    
      { return 2; }
    else                                                           
      { return 4; }  
    break;

  case 15: // LOWER RIGHT CORNER
    return getWeight16( i, j, x, y, nSize, 3 );
    break;

  default:
    cerr << "    Error in getWeight128CaseI(): "
         << "Illegal window index (" << nWindow << ")!" << endl;
  }

  return nWeight;
}


int getWeight128CaseII( int i, int j, int x, int y, int nSize, int nWindow )
{
  // Window numbering
  //
  //  0  1  2  3
  //  4  5  6  7
  //  8  9 10 11
  // 12 13 14 15
  //

  int nWeight = -1;

  switch( nWindow ) {
  case 0: 
  case 3: 
  case 12: 
  case 15: // CORNERS
    nWeight = 4;
    break;

  case 4: 
  case 7: // LEFT/RIGHT SIDE UP
    if   ( j==(y+nSize-1) ) 
      nWeight = 2;
    else
      nWeight = 4;
    break;

  case 8: 
  case 11: // LEFT/RIGHT SIDE LOW
    if   ( j==y )
      nWeight = 2;
    else
      nWeight = 4;
    break;

  case 1:
  case 13: // UPPER/LOWER SIDE LEFT
    if   ( i==(x+nSize-1) )
      nWeight = 2;
    else
      nWeight = 4;
    break;

  case 2:
  case 14: // UPPER/LOWER SIDE RIGHT
    if   ( i==x )
      nWeight = 2;
    else
      nWeight = 4;
    break;

  case 5: // MIDDLE UPPER LEFT
    nWeight = getWeight16( i, j, x, y, nSize, 0 );
    break;

  case 6: // MIDDLE UPPER RIGHT
    nWeight = getWeight16( i, j, x, y, nSize, 1 );
    break;

  case 9: // MIDDLE LOWER LEFT
    nWeight = getWeight16( i, j, x, y, nSize, 2 );
    break;

  case 10: // MIDDLE LOWER RIGHT
    nWeight = getWeight16( i, j, x, y, nSize, 3 );
    break;

  default:
    cerr << "    Error in getWeight128CaseII(): "
         << "Illegal window index (" << nWindow << ")!" << endl;
  }

  return nWeight;
}


int getX( int nWindowNr, int nSize )
{
  // Window numbering
  //
  //  0  1  2  3
  //  4  5  6  7
  //  8  9 10 11
  // 12 13 14 15
  //
  switch ( nWindowNr ) {
  case 0: case 4: case 8: case 12:   
    return 0;
  case 1: case 5: case 9: case 13:   
    return nSize;
  case 2: case 6: case 10: case 14: 
    return 2*nSize-1;
  case 3: case 7: case 11: case 15: 
    return 3*nSize-1;
  }
}


int getY( int nWindowNr, int nSize )
{
  // Window numbering
  //
  //  0  1  2  3
  //  4  5  6  7
  //  8  9 10 11
  // 12 13 14 15
  //
  switch (nWindowNr) {
  case 0: case 1: case 2: case 3: 
    return 0;
  case 4: case 5: case 6: case 7: 
    return nSize;
  case 8: case 9: case 10: case 11: 
    return 2*nSize-1;
  case 12: case 13: case 14: case 15: 
    return 3*nSize-1;
  }
}

/***************************************************************************/
/*                             Visualization                               */
/***************************************************************************/

OpGrayImage drawEdgeFeature( FeatureVector fvFeature )
{
  if( fvFeature.numDims() == 16 )
    return drawEdgeFeature16d( fvFeature );
  else
    return drawEdgeFeature128d( fvFeature );
}


OpGrayImage drawEdgeFeature16d( FeatureVector fvFeature )
{
  OpGrayImage imgResult(9,9);

  if( fvFeature.numDims() != 16 ) {
    cerr << "    Error in drawEdgeFeature16d(): "
         << "Feature vector dimensions don't match!" << endl;
    return imgResult;
  }
  
  /* set markers for the cell positions */
  float dColMarker = 100.0/16.0;
  imgResult(2,2) = dColMarker;
  imgResult(6,2) = dColMarker;
  imgResult(2,6) = dColMarker;
  imgResult(6,6) = dColMarker;

  /* draw the histogram values */
  int nBinIdx = 0;
  for( int y=0; y<2; y++ )
    for( int x=0; x<2; x++ ) {
      int nPosX = x*4 + 2;
      int nPosY = y*4 + 2;

      imgResult(nPosX+1,nPosY  ) = fvFeature.at( nBinIdx );
      imgResult(nPosX  ,nPosY-1) = fvFeature.at( nBinIdx+1 );
      imgResult(nPosX-1,nPosY  ) = fvFeature.at( nBinIdx+2 );
      imgResult(nPosX  ,nPosY+1) = fvFeature.at( nBinIdx+3 );
        
      nBinIdx += 4;
    }

  return imgResult;
}


OpGrayImage drawEdgeFeature128d( FeatureVector fvFeature )
{
  OpGrayImage imgResult(17,17);

  if( fvFeature.numDims() != 128 ) {
    cerr << "    Error in drawEdgeFeature128d(): "
         << "Feature vector dimensions don't match!" << endl;
    return imgResult;
  }
  
  /* set markers for the cell positions */
  float dColMarker = 100.0/128.0;
  imgResult( 2, 2) = dColMarker;
  imgResult( 6, 2) = dColMarker;
  imgResult(10, 2) = dColMarker;
  imgResult(14, 2) = dColMarker;
  imgResult( 2, 6) = dColMarker;
  imgResult( 6, 6) = dColMarker;
  imgResult(10, 6) = dColMarker;
  imgResult(14, 6) = dColMarker;
  imgResult( 2,10) = dColMarker;
  imgResult( 6,10) = dColMarker;
  imgResult(10,10) = dColMarker;
  imgResult(14,10) = dColMarker;
  imgResult( 2,14) = dColMarker;
  imgResult( 6,14) = dColMarker;
  imgResult(10,14) = dColMarker;
  imgResult(14,14) = dColMarker;

  /* draw the histogram values */
  int nBinIdx = 0;
  for( int y=0; y<4; y++ )
    for( int x=0; x<4; x++ ) {
      int nPosX = x*4 + 2;
      int nPosY = y*4 + 2;

      imgResult(nPosX+1,nPosY  ) = fvFeature.at( nBinIdx );
      imgResult(nPosX+1,nPosY-1) = fvFeature.at( nBinIdx+1 );
      imgResult(nPosX  ,nPosY-1) = fvFeature.at( nBinIdx+2 );
      imgResult(nPosX-1,nPosY-1) = fvFeature.at( nBinIdx+3 );
      imgResult(nPosX-1,nPosY  ) = fvFeature.at( nBinIdx+4 );
      imgResult(nPosX-1,nPosY+1) = fvFeature.at( nBinIdx+5 );
      imgResult(nPosX  ,nPosY+1) = fvFeature.at( nBinIdx+6 );
      imgResult(nPosX+1,nPosY+1) = fvFeature.at( nBinIdx+7 );
        
      nBinIdx += 8;
    }

  return imgResult;
}


OpGrayImage drawEdgeFeature16d_2( FeatureVector fvFeature )
{
  OpGrayImage imgResult(9,7);

  if( fvFeature.numDims() != 16 ) {
    cerr << "    Error in drawEdgeFeature16d_2(): "
         << "Feature vector dimensions don't match!" << endl;
    return imgResult;
  }
  
  /* set markers for the cell positions */
  float dColMarker = 100.0/16.0;
  imgResult(2,2) = dColMarker;
  imgResult(6,2) = dColMarker;
  imgResult(2,5) = dColMarker;
  imgResult(6,5) = dColMarker;

  /* draw the histogram values */
  int nBinIdx = 0;
  for( int y=0; y<2; y++ )
    for( int x=0; x<2; x++ ) {
      int nPosX = x*4 + 2;
      int nPosY = y*3 + 2;

      imgResult(nPosX+1,nPosY  ) = fvFeature.at( nBinIdx );
      imgResult(nPosX+1,nPosY-1) = fvFeature.at( nBinIdx+1 );
      imgResult(nPosX  ,nPosY-1) = fvFeature.at( nBinIdx+2 );
      imgResult(nPosX-1,nPosY-1) = fvFeature.at( nBinIdx+3 );
        
      nBinIdx += 4;
    }

  return imgResult;
}


OpGrayImage drawEdgeFeature128d_2( FeatureVector fvFeature )
{
  OpGrayImage imgResult(25,17);

  if( fvFeature.numDims() != 128 ) {
    cerr << "    Error in drawEdgeFeature128d(): "
         << "Feature vector dimensions don't match!" << endl;
    return imgResult;
  }
  
  /* set markers for the cell positions */
  float dColMarker = 100.0/64.0;
  imgResult( 3, 3) = dColMarker;
  imgResult( 9, 3) = dColMarker;
  imgResult(15, 3) = dColMarker;
  imgResult(21, 3) = dColMarker;
  imgResult( 3, 7) = dColMarker;
  imgResult( 9, 7) = dColMarker;
  imgResult(15, 7) = dColMarker;
  imgResult(21, 7) = dColMarker;
  imgResult( 3,11) = dColMarker;
  imgResult( 9,11) = dColMarker;
  imgResult(15,11) = dColMarker;
  imgResult(21,11) = dColMarker;
  imgResult( 3,15) = dColMarker;
  imgResult( 9,15) = dColMarker;
  imgResult(15,15) = dColMarker;
  imgResult(21,15) = dColMarker;

  /* draw the histogram values */
  int nBinIdx = 0;
  for( int y=0; y<4; y++ )
    for( int x=0; x<4; x++ ) {
      int nPosX = x*6 + 3;
      int nPosY = y*4 + 3;

      imgResult(nPosX+2,nPosY  ) = fvFeature.at( nBinIdx );
      imgResult(nPosX+2,nPosY-1) = fvFeature.at( nBinIdx+1 );
      imgResult(nPosX+2,nPosY-2) = fvFeature.at( nBinIdx+2 );
      imgResult(nPosX+1,nPosY-2) = fvFeature.at( nBinIdx+3 );
      imgResult(nPosX  ,nPosY-2) = fvFeature.at( nBinIdx+4 );
      imgResult(nPosX-1,nPosY-2) = fvFeature.at( nBinIdx+5 );
      imgResult(nPosX-2,nPosY-2) = fvFeature.at( nBinIdx+6 );
      imgResult(nPosX-2,nPosY-1) = fvFeature.at( nBinIdx+7 );
        
      nBinIdx += 8;
    }

  return imgResult;
}


OpGrayImage drawEdgeFeatureBoth( FeatureVector fv16, FeatureVector fv128 )
{
  OpGrayImage imgResult(25,17);

  if( (fv16.numDims() != 16) || (fv128.numDims() != 128) ) {
    cerr << "    Error in drawEdgeFeatureBoth: "
         << "Feature vector dimensions don't match!" << endl;
    return imgResult;
  }
  
  /* set markers for the cell positions */
  float dColMarker = 100.0/64.0;
  imgResult( 3, 3) = dColMarker;
  imgResult( 9, 3) = dColMarker;
  imgResult(15, 3) = dColMarker;
  imgResult(21, 3) = dColMarker;
  imgResult( 3, 7) = dColMarker;
  imgResult( 9, 7) = dColMarker;
  imgResult(15, 7) = dColMarker;
  imgResult(21, 7) = dColMarker;
  imgResult( 3,11) = dColMarker;
  imgResult( 9,11) = dColMarker;
  imgResult(15,11) = dColMarker;
  imgResult(21,11) = dColMarker;
  imgResult( 3,15) = dColMarker;
  imgResult( 9,15) = dColMarker;
  imgResult(15,15) = dColMarker;
  imgResult(21,15) = dColMarker;

  /* draw the histogram values */
  int nBinIdx16 = 0;
  int nBinIdx128 = 0;
  for( int y=0; y<4; y++ )
    for( int x=0; x<4; x++ ) {
      int nPosX = x*6 + 3;
      int nPosY = y*4 + 3;

      nBinIdx16 = (y/2)*8 + (x/2)*4;
      imgResult(nPosX+1,nPosY  ) = fv16.at( nBinIdx16 )/4.0;
      imgResult(nPosX+1,nPosY-1) = fv16.at( nBinIdx16+1 )/4.0;
      imgResult(nPosX  ,nPosY-1) = fv16.at( nBinIdx16+2 )/4.0;
      imgResult(nPosX-1,nPosY-1) = fv16.at( nBinIdx16+3 )/4.0;
        
      imgResult(nPosX+2,nPosY  ) = fv128.at( nBinIdx128 );
      imgResult(nPosX+2,nPosY-1) = fv128.at( nBinIdx128+1 );
      imgResult(nPosX+2,nPosY-2) = fv128.at( nBinIdx128+2 );
      imgResult(nPosX+1,nPosY-2) = fv128.at( nBinIdx128+3 );
      imgResult(nPosX  ,nPosY-2) = fv128.at( nBinIdx128+4 );
      imgResult(nPosX-1,nPosY-2) = fv128.at( nBinIdx128+5 );
      imgResult(nPosX-2,nPosY-2) = fv128.at( nBinIdx128+6 );
      imgResult(nPosX-2,nPosY-1) = fv128.at( nBinIdx128+7 );
        
      nBinIdx128 += 8;
    }

  return imgResult;
}


// -------------------------------------------------------------------------
//             Old Stuff (Outdated)
// -------------------------------------------------------------------------


const float TLOW  = 0.4;
const float THIGH = 0.8;

void applyEdgeScaleSpace( OpGrayImage img, 
                          int nNumOctaves, int nLevelsPerOctave, 
                          float dScaleSigma0,
                          int nthEdgePixel, bool bInterpolate,
                          vector<OpGrayImage> &vEdgeImages,
                          vector<float> &vEdgeScales,
                          PointVector &vPoints )
  /*******************************************************************/
  /* Create an Edge Scale Space   */
  /*******************************************************************/
{
  cout << "  ::applyEdgeScaleSpace( " << nNumOctaves << ", " 
       << nLevelsPerOctave << ", " << nthEdgePixel << ", " << bInterpolate 
       << " ) called." << endl;

  vPoints.clear();
  
  /****************************/
  /* Create a DoG Scale Space */
  /****************************/
  DoGScaleSpace dogScale( img, nNumOctaves, nLevelsPerOctave, dScaleSigma0 );

  /****************************************/
  /* Create edge images at several scales */
  /****************************************/
  double dMinSigma = sqrt(2.0)/2.0;
  double dFactor   = sqrt(2.0);
  double dLimit    = 8.0f;

  PointVector vEdgePoints;
  vEdgeImages.clear();
  vEdgeScales.clear();
  long nNumEdgePixels = 0;
  for ( double dSigma=dMinSigma; dSigma<dLimit; dSigma*=dFactor ) {
    cout << "    Generating edge image with sigma=" << dSigma << "..." << endl;
    
    /* Extract edges at the current scale */
    OpGrayImage imgEdges;
    imgEdges = img.opCannyEdges( dSigma, TLOW, THIGH );
    vEdgeImages.push_back( imgEdges );
    vEdgeScales.push_back( dSigma );

    /* Scan the image for edge pixels */
    for ( int y = 0; y < imgEdges.height(); y ++ )
      for ( int x = 0; x < imgEdges.width(); x ++ ) {
        
        // 0 -> Edge, 255 -> No Edge (ideal)
        if ( imgEdges(x,y).value() == 0 ) {
          InterestPoint ptEdge;
          ptEdge.x = x;
          ptEdge.y = y;
          vEdgePoints.push_back( ptEdge );
          nNumEdgePixels++;
        }
      }
  }
  
  /***************************/
  /* Take only unique points */
  /***************************/
  /* sort the interest point list */
  stable_sort( vEdgePoints.begin(), vEdgePoints.end(), compInterestPtsX() );
  stable_sort( vEdgePoints.begin(), vEdgePoints.end(), compInterestPtsY() );

  /* transfer all unique points to a new list */
 PointVector vUniquePoints;
  int nLastX       = vEdgePoints[0].x;
  int nLastY       = vEdgePoints[0].y;
  InterestPoint ptCurrent = vEdgePoints[0];
  for( int i=1; i<(int)vEdgePoints.size(); i++ )
    if( (vEdgePoints[i].x != nLastX) || (vEdgePoints[i].y != nLastY) ) {
      vUniquePoints.push_back( ptCurrent );

      nLastX     = vEdgePoints[i].x;
      nLastY     = vEdgePoints[i].y;
      ptCurrent  = vEdgePoints[i];
    }
  vUniquePoints.push_back( ptCurrent );
  

  /*************************************/
  /* Extract the characteristic scales */
  /*************************************/
  for( int i=0; i<(int)vUniquePoints.size(); i++ ) {

    /* Take only every nth edge pixel */
    if ( i % nthEdgePixel == 0 ) {        
      /* get all characteristic scales for this pixel... */
      vector<float> vScales;
      vector<float> vValues;
      dogScale.getCharacteristicScales( vUniquePoints[i].x, 
                                        vUniquePoints[i].y, 
                                        vScales, vValues, 
                                        bInterpolate );

      /* ...and generate a new interest point for it */
      for ( int j=0; j<vScales.size(); j++ ) {
        InterestPoint pt( vUniquePoints[i] );
        pt.value = vValues[j];
        pt.scale = vScales[j];
        
        vPoints.push_back( pt );
      }
    }
  }

  /* Sort the interest point list */
  stable_sort( vPoints.begin(), vPoints.end(), compInterestPtsScale() );
  
  /* print a short report */
  cout << "    #edge points:\t" << nNumEdgePixels << endl;
  cout << "    #unique points:\t" << vUniquePoints.size() << endl;
  cout << "    #interest points:\t" << vPoints.size() << endl;  
  cout << "  done." << endl;
}


