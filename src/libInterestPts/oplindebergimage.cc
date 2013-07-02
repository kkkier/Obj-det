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

/****************/
/*   Includes   */
/****************/
#include <iostream>

#include <scalespace.hh>
#include "oplindebergimage.hh"


/*===================================================================*/
/*                       Class OpLindebergImage                      */
/*===================================================================*/

/***********************************************************/
/*                     Harris Operators                    */
/***********************************************************/

OpLindebergResult OpLindebergImage::opLindeberg(double sigma, float scale, 
                                                int level) 
  /* The Lindeberg interest point detector                           */
  /* Computes the scale space of given image and applies Laplacian   */
  /* on each level of the scale space. Searches maximum in scale     */
  /* space (characteristic scale). Returns an OpGrayImage containing */
  /* the value at the characteristic scale of each point.            */
{
  cout << "OpLindebergImage::opLindeberg called" << endl;
  cout << "  Creating scale space..." << endl;
  //OpScaleSpace ssp(*this);
  //ScaleSpace sspace = ssp.opBuildScaleSpace(sigma, scale, level);
  m_sspace = buildScaleSpace(*this, sigma, scale, level);
  ScaleSpace sspace(m_sspace);
  cout << "  Applying Laplacian on scale space." << endl;
  sspace.opLaplacian();
  
  OpGrayImage result(this->width(), this->height());
  OpGrayImage scales(result);
  
  cout << "  Searching maxima..." << endl;
  for (int i=0; i<this->width(); i++) {
    for (int j=0; j<this->height(); j++) {
      result(i,j) = 0.0;
      scales(i,j) = 0.0;
      //** search for a maximum in scale space
        for (int z=1; z<=sspace.getLevel()-1; z++) {
          if (sspace.isMaximum3D(i,j,z)) {
            result(i,j) = sspace(i,j,z);
            scales(i,j) = sspace.getScaleAtLevel(z);
            break;
          }
        }
    }
  }
  cout << "OpLindebergImage::opLindeberg exit." << endl;
  return OpLindebergResult(result, scales);
}

