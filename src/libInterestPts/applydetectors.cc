/*********************************************************************/
/*                                                                   */
/* FILE         applydetectors.cc                                    */
/* AUTHORS      Bastian Leibe, with some code from Thomas Hug        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Functions for easy use of interest point detectors.  */
/*                                                                   */
/* BEGIN        WED Apr 03 2002                                      */
/* LAST CHANGE  WED Apr 03 2002                                      */
/*                                                                   */
/*********************************************************************/


#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <qapplication.h>
#include <qcolor.h>
#include <q3filedialog.h>

#include <opgrayimage.hh>
#include "opharrisimage.hh"
#include "oplindebergimage.hh"
#include "ophalapimage.hh"

#include "applydetectors.hh"


bool processImages( vector<string> vFileNames, InterestPtDetector ipdType,
										double dSigma1, double dSigma2, double dAlpha,
										double dScaleFact, int nLevel,
										int nNmsSize, double dThresh1, double dThresh2 )
{
	OpGrayImage img;
	
	for( int i=0; i<vFileNames.size(); i++ ) {
		/* load the next image */
		if ( !loadImage( vFileNames[i], img ) ) {
			cerr << "Error: Couldn't load input image!" << endl;
			return false;
		}
		
		/* apply the interest point detector */
		PointVector vPoints;
		applyDetector( img, ipdType, dSigma1, dSigma2, dAlpha,
									 dScaleFact, nLevel, nNmsSize, dThresh1, dThresh2, 
									 vPoints );
		
		/* print out the interest point coordinates */
		for( int j=0; j<vPoints.size(); j++ )
			cout << "  Point " << setw(3) << ": (" << vPoints[j].x << "," 
					 << vPoints[j].y << ")" << endl;
	}
	return true;
}


bool loadImage( QString qsFileName, OpGrayImage &img )
{ return loadImage( string(qsFileName.latin1()), img ); }

bool loadImage( string sFileName, OpGrayImage &img ) 
	/* load an image into an OpGrayImage object. */
{
  QImage tmp_img;
  tmp_img.load( sFileName.c_str() );
  
  /* check for a potentially broken network connection */
  if ( tmp_img.isNull() ) {
    cout << "      Couldn't access image. Trying again... ";
    tmp_img.load( sFileName.c_str() );
    if ( !tmp_img.isNull() )
      cout << "SUCCESS" << endl;
    else {
      cout << "FAILED!" << endl;
      return false;
    }
  }
  
  /* convert to OpGrayImage structure */
  OpGrayImage grayImg( tmp_img );
  img = grayImg;

  return true;
}


bool applyDetector( OpGrayImage img, InterestPtDetector ipdType,
										double dSigma1, double dSigma2, double dAlpha,
										double dScaleFact, int nLevel,
										int nNmsSize, double dThresh1, double dThresh2,
										PointVector &vPoints ) 
	/*******************************************************************/
  /* Apply the selected interest point detector on the input image.  */
  /* Supported detectors (with their respective parameters) are:     */
  /*   IPD_HARRIS         : standard Harris detector                 */
  /*     dSigma1          : sigma for initial Gaussian filtering     */
  /*     dSigma2          : sigma for region weighting               */
  /*     dAlpha           : Harris parameter (usually 0.04-0.06)     */
  /*     dThresh1         : threshold for interest points (std. 100) */
  /*   IPD_LINDEBERG      : Lindeberg detector                       */
  /*     dSigma1          : initial sigma for lowest level           */
  /*     dScaleFact       : sigma(level_k)=dSigma1*dScaleFact^k      */
  /*     nLevel           : number of scale levels                   */
  /*     dThresh1         : threshold for interest points (std. 100) */
  /*   IPD_HARRISLAP      : Harris-Laplacian detector                */
  /*     dSigma1          : initial sigma for lowest level           */
  /*     dScaleFact       : sigma(level_k)=dSigma1*dScaleFact^k      */
  /*     dSigma2          : sigma for region weighting               */
  /*     dAlpha           : Harris parameter (usually 0.04-0.06)     */
  /*     dThresh1         : threshold for interest points (std. 100) */
  /*     dThresh2         : threshold for Harris pts at each level   */
  /*   IPD_HARRISLAP_OPT  : optimized Harris-Laplacian detector      */
  /*     dSigma2          : sigma for region weighting               */
  /*     dAlpha           : Harris parameter (usually 0.04-0.06)     */
  /*     dThresh1         : threshold for interest points (std. 100) */
  /*     dThresh2         : threshold for Harris pts at each level   */
  /* The result is a vector of interest points, returned in vPoints. */
	/*******************************************************************/
{
  if ( !(img.width() * img.height() > 0) ) {
    cerr << "Error in applyDetector(): Image is empty!" << endl;
    return false;
  }
  
  /**********************/
  /*   Apply Detector   */
  /**********************/
  OpInterestImage     interest;
  OpLindebergResult   ssres;
  OpGrayImage	        resultImage;
  OpGrayImage	        scaleImage;
   
  switch( ipdType ) {
  case IPD_HARRIS: {
    //*** Apply Harris detector...
    OpHarrisImage imgHarris(img);
    interest = imgHarris.opImprovedHarris( dSigma1, dSigma2, dAlpha );
    interest = interest.opNonMaximumSuppression( nNmsSize );
    //interest = interest.opStrongestPercentage( 0.01 );
    interest = interest.opThreshold(dThresh1);
    resultImage = interest;
    break; 
	}
    
  case IPD_LINDEBERG: {
    //*** Apply Lindeberg detector...
    OpLindebergImage lindiImg(img);
    ssres       = lindiImg.opLindeberg( dSigma1, dScaleFact, nLevel );
    interest    = ssres.resultImage();
    interest = interest.opThreshold(dThresh1);
    resultImage = interest;
    scaleImage  = ssres.scaleImage();
    break;
	}

  case IPD_HARRISLAP:
  case IPD_HARRISLAP_OPT: {
    //*** Apply Harris-Laplacian detector...
    OpHalapImage hlImg(img);
    if ( ipdType == IPD_HARRISLAP ) {
      ssres = hlImg.opHarrisLaplacian( dSigma1, dScaleFact, nLevel,
																			 dSigma2, dAlpha, dThresh2 );
    } else {
      ssres = hlImg.opFastHarrisLaplacian( nLevel, dSigma2, 
																					 dAlpha, dThresh2 );
    }
    interest    = ssres.resultImage();
    interest = interest.opThreshold(dThresh1);
    resultImage = interest;
    scaleImage  = ssres.scaleImage();
    break;
	}
	
  default:
    cerr << "Error in applyDetector(): Unknown detector type: " << ipdType 
	 << "!" << endl;
    return false; 
  }
  
  /* Collect Interest Points */
  vPoints = interest.getInterestPointVector();
    
  return true;
}


//****************************
// the output methods
//****************************

void harrisOutput( OpGrayImage img, 
									 double dSigma1, double dSigma2, double dAlpha, 
									 bool bDoNms, bool bSaveResult ) 
{
  OpGrayImage tmp = OpHarrisImage(img).opImprovedHarris(dSigma1, dSigma2,
                                                        dAlpha);
  tmp = OpInterestImage(tmp).opStrongestPercentage(0.01);
  colorOutput(tmp, bDoNms, bSaveResult);
}


void colorOutput( OpGrayImage refImage, bool nms, 
									bool bSaveResult ) 
{
  if (nms)
    refImage = OpInterestImage(refImage).opNonMaximumSuppression();
  //referenceImage = referenceImage.opGaussDerivGy(1.0);
  /*referenceImage = referenceImage.opGaussDerivGx(1.0);
    for (int i=0; i < referenceImage.width(); i++) {
    for (int j=0; j < referenceImage.height(); j++) {
    referenceImage(i,j) = referenceImage(i,j).value() * tmpImg(i,j).value();
    }
    }*/
  QImage tmp = refImage.getQtImage();
  tmp.setNumColors(171 * 360 + 1);
  QColor color;
  int colors = 0;
  for (int h=0; h<360; h++) {
    for (int v=85; v<256; v++) {
      color.setHsv(h, 255, v);
      tmp.setColor(colors++, color.rgb());
    }
  }
  //tmp.setColor(colors++, black.rgb());
  tmp.setColor(colors++, QColor(Qt::black).rgb() );
  
  float min, max;
  refImage.getMinMax(min, max);
  for (int i=0; i < tmp.width(); i++) {
    for (int j=0; j < tmp.height(); j++) {
      if (refImage(i,j).value() > 0) {
				int val = 255;
				if (!nms)
					val = (int)floor(refImage(i,j).value() / max * 255 / 1.5) + 85;
				color.setHsv((630 - (int)floor(refImage(i,j).value() / max * 359) 
											% 360),
										 255,  
										 //(int)floor(referenceImage(i,j).value()/max*255/2)+125,
										 //(int)floor(referenceImage(i,j).value()/max*255/1.5)+85);
										 val);
				tmp.setPixel(i,j,color.rgb());
				if (nms) {
					for (int x=1; x<3; x++) {
						if (i+x < tmp.width())
							tmp.setPixel(i+x,j,color.rgb());
						if (i-x >= 0)
							tmp.setPixel(i-x,j,color.rgb());
						if (j+x < tmp.height())
							tmp.setPixel(i,j+x,color.rgb());
						if (j-x >= 0)
							tmp.setPixel(i,j-x,color.rgb());
					}
				}
      } else {
				if (!nms)
					tmp.setPixel(i,j,QColor(Qt::black).rgb());
      }
    }
  }
  tmp.setNumColors(256);
  //rsReferenceImage->loadImage(tmp);
  //rsReferenceImage->display();
	
  if( bSaveResult ) {
    QString file = Q3FileDialog::getSaveFileName( "/home/leibe/projects/interestpoints/interestviewer/",
																								 "Images (*.png *.xpm *.jpg *.ppm *.eps);;all files (*.*)" ); //this );
    if (file.isEmpty())
      return;
    
    if (!(tmp.save(file, "JPEG", 100))) {
      cerr << "Error saving image!" << endl;
    } else {
      cout << "Image saved." << endl;
    }
		tmp.reset();
  }
}


void writeEPS( OpLindebergResult lres, double threshold, bool spots ) 
{
  QString file = Q3FileDialog::getSaveFileName( "/home/leibe/projects/interestpoints/interestviewer/",
																							 "Images (*.png *.xpm *.jpg *.ppm *.eps);;all files (*.*)" ); //, this );
  if (file.isEmpty())
    return;
	
  ofstream psFile(file);
  // calculate bounding box
  OpInterestImage resultImage = lres.resultImage();
  OpInterestImage scaleImage  = lres.scaleImage();
  int minx = 0, maxx = resultImage.width();
  int miny = 0, maxy = resultImage.height();
  for (int i = 0; i < resultImage.width(); i++) {
    for (int j = 0; j < resultImage.height(); j++) {
      if (i - (int)ceil(scaleImage(i,j).value()) < minx) {
				minx = i - (int)ceil(scaleImage(i,j).value());
      } else if (i + (int)ceil(scaleImage(i,j).value()) > maxx) {
				maxx = i + (int)ceil(scaleImage(i,j).value());
      }
      if (j - (int)ceil(scaleImage(i,j).value()) < miny) {
				miny = j - (int)ceil(scaleImage(i,j).value());
      } else if (j + (int)ceil(scaleImage(i,j).value()) > maxy) {
				maxy = j + (int)ceil(scaleImage(i,j).value());
      }
    }
  }
	
  int offX = -minx + 1;
  int offY = -miny + 1;
  int width = resultImage.width();
  int height = resultImage.height();
  // write opening sequence...
  psFile << "%!PS-Adobe-3.0 EPSF-3.0" << endl;
  psFile << "%%BoundingBox: " << 0 << " " << 0 << " "
				 << maxx+1+offX << " " << maxy+1+offY << endl;
  psFile << "%%LanguageLevel: 2" << endl;
  psFile << "%%Pages: 1" << endl;
  psFile << "%%Title: " << file.toStdString() << endl;
  psFile << "%%EndComments" << endl;
  psFile << "1 setlinewidth" << endl;
	
  // mark image boundaries for postprocessing
  psFile << "newpath" << endl;
  psFile << offX << " " << maxy - (offY+4) << " moveto" << endl;
  psFile << offX << " " << maxy - (offY-4) << " lineto stroke" << endl;
  psFile << "newpath" << endl;
  psFile << offX+4 << " " << maxy - offY << " moveto" << endl;
  psFile << offX-4 << " " << maxy - offY << " lineto stroke" << endl;
  psFile << "newpath" << endl;
  psFile << offX+width << " " << maxy - (offY+height+4) << " moveto" << endl;
  psFile << offX+width << " " << maxy - (offY+height-4) << " lineto stroke" 
				 << endl;
  psFile << "newpath" << endl;
  psFile << offX+width+4 << " " << maxy - (offY+height) << " moveto" << endl;
  psFile << offX+width-4 << " " << maxy - (offY+height) << " lineto stroke" 
				 << endl;
	
  // write circles
  for (int i = 0; i < resultImage.width(); i++) {
    for (int j = 0; j < resultImage.height(); j++) {
      if (resultImage(i,j).value() > threshold) {
				float radius = qRound(scaleImage(i,j).value());
				psFile << "newpath" << endl;
				psFile << i+offX << " " << maxy - (j+offY) << " " << radius 
	       << " 0 360 arc stroke" << endl;
				if (spots) {
					psFile << "newpath" << endl;
					psFile << i+offX << " " << maxy - (j+offY-2) << " moveto" << endl;
					psFile << i+offX << " " << maxy - (j+offY+2) << " lineto stroke" 
								 << endl;
					psFile << "newpath" << endl;
					psFile << i+offX-2 << " " << maxy - (j+offY) << " moveto" << endl;
					psFile << i+offX+2 << " " << maxy - (j+offY) << " lineto stroke" 
								 << endl;
				}
      }
    }
  }
	
  // write closing sequence...
  psFile << "%%EOF" << endl;
  psFile << "%%EndDocument" << endl << endl;
  psFile << "EndEPSF" << endl;
  psFile << "stacks" << endl;
  psFile << "pgsave restore" << endl;
  psFile << "showpage" << endl;
  psFile << "%%EOF" << endl;
}


