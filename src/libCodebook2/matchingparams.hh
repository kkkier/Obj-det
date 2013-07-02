/*********************************************************************/
/*                                                                   */
/* FILE         matchingparams.hh                                    */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Class for codebook matching parameters.              */
/*                                                                   */
/* BEGIN        Tue Mar 22 2005                                      */
/* LAST CHANGE  Tue Mar 22 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef MATCHINGPARAMS_HH
#define MATCHINGPARAMS_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>

#include <qwidget.h>
#include <qstring.h>

#include "matchinggui.hh"

/*******************/
/*   Definitions   */
/*******************/


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                       Class MatchingParameters                    */
/*===================================================================*/
class MatchingParams
{
public:
  MatchingParams() { m_guiMatching = 0; }
  MatchingParams( const MatchingParams &other );
  
  MatchingParams& operator=( const MatchingParams &other );

protected:
  void copyFrom( const MatchingParams &other );

public:
  /*********************/
  /*   GUI Functions   */
  /*********************/
  MatchingGUI* createGUI( QWidget *parent=0, const char* name=0 );
  void         setGUI   ( MatchingGUI* pGUI ) { m_guiMatching = pGUI; }

public:
  /**************************/
  /*   File I/O Functions   */
  /**************************/
  
public:
  /************************/
  /*   Parameter Access   */
  /************************/
  bool         isValid() const { return (m_guiMatching != 0); }
  MatchingGUI*  params() const; 

protected:
  MatchingGUI* m_guiMatching;
};


#endif
