/*********************************************************************/
/*                                                                   */
/* FILE         oplindebergimage.hh                                  */
/* AUTHORS      Thomas Hug, modified by Bastian Leibe                */
/* EMAIL        thug@student.ethz.ch                                 */
/*                                                                   */
/* CONTENT      Implementation of the Lindeberg Interest Point       */
/*              Detector.                                            */
/*                                                                   */
/* BEGIN        Mon Nov 05 2001                                      */
/* LAST CHANGE  Wed Jan 23 2002                                      */
/*                                                                   */
/*********************************************************************/
/***************************************************************************
    copyright            : (C) 2001 by Thomas Hug and ETH Zurich
    email                : thug@student.ethz.ch
 ***************************************************************************/

#ifndef OPLINDEBERGIMAGE_HH
#define OPLINDEBERGIMAGE_HH

//#ifdef _USE_PERSONAL_NAMESPACES
//namespace Hug {
//#endif

/****************/
/*   Includes   */
/****************/
#include <scalespace.hh>
#include "opinterestimage.hh"

/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                      Class OpLindebergResult                      */
/*===================================================================*/
/* Define a result class for the Lindeberg detector. */
class OpLindebergResult : public OpInterestResult  {
public: 
  OpLindebergResult() {}
  OpLindebergResult(GrayImage resImg, GrayImage sclImg) 
    : OpInterestResult(resImg) { m_scaleImage = sclImg; }
  ~OpLindebergResult() {}
  
  OpInterestImage scaleImage() { return m_scaleImage; }

private:
  OpInterestImage		m_scaleImage;
};


/*===================================================================*/
/*                       Class OpInterestImage                       */
/*===================================================================*/
/* Define an image class for calculating Lindeberg interest points. */
class OpLindebergImage : public OpInterestImage  {
public:
  OpLindebergImage() : OpInterestImage() {}
  OpLindebergImage(const int width, const int height) 
    : OpInterestImage(width, height) {}
  OpLindebergImage(const GrayImage& other) : OpInterestImage(other) {}
  OpLindebergImage(const QImage& src) : OpInterestImage(src) {}
  
  OpLindebergResult opLindeberg(double sigma=1.0, float scale=1.15, 
				int level=25);

private:
  ScaleSpace m_sspace;
};

//#ifdef _USE_PERSONAL_NAMESPACES
//}
//#endif

#endif
