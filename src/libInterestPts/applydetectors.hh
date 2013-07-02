/*********************************************************************/
/*                                                                   */
/* FILE         applydetectors.hh                                    */
/* AUTHORS      Bastian Leibe, with some code from Thomas Hug        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Functions for easy use of interest point detectors.  */
/*                                                                   */
/* BEGIN        WED Apr 03 2002                                      */
/* LAST CHANGE  WED Apr 03 2002                                      */
/*                                                                   */
/*********************************************************************/

#ifndef LEIBE_APPLYDETECTORS_HH
#define LEIBE_APPLIDETECTORS_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <qstring.h>
#include <string>
#include <vector>

#include <opgrayimage.hh>
#include "oplindebergimage.hh"

/*******************/
/*   Definitions   */
/*******************/
enum InterestPtDetector{ IPD_HARRIS=1, IPD_LINDEBERG, 
                         IPD_HARRISLAP, IPD_HARRISLAP_OPT };

/******************/
/*   Prototypes   */
/******************/
bool processImages( vector<string> vFileNames, InterestPtDetector ipdType,
                    double dSigma1, double dSigma2, double dAlpha,
                    double dScaleFact, int nLevel,
                    int nNmsSize, double dThresh1, double dThresh2 );

/* auxiliary functions */
bool loadImage( QString qsFileName, OpGrayImage &img );
bool loadImage( string sFileName, OpGrayImage &img );

bool applyDetector( OpGrayImage img, InterestPtDetector ipdType,
                    double dSigma1, double dSigma2, double dAlpha,
                    double dScaleFact, int nLevel,
                    int nNmsSize, double dThresh1, double dThresh2,
                    PointVector &vPoints );

/* output functions */
void harrisOutput( OpGrayImage img, 
                   double dSigma1, double dSigma2, double dAlpha, 
                   bool bDoNms, bool bSaveResult );
void colorOutput(OpGrayImage refImage, bool nms, 
                 bool bSaveResult);
void writeEPS( OpLindebergResult lres, double threshold, bool spots );

#endif
