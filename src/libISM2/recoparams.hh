/*********************************************************************/
/*                                                                   */
/* FILE         recoparams.hh                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Class for recognition parameters.                    */
/*                                                                   */
/* BEGIN        Thu Jan 20 2005                                      */
/* LAST CHANGE  Thu Jan 20 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef RECOPARAMS_HH
#define RECOPARAMS_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>

#include <qwidget.h>
#include <qstring.h>

#include "recogui.hh"

/*******************/
/*   Definitions   */
/*******************/


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                       Class RecoParameters                        */
/*===================================================================*/
class RecoParams
{
public:
  RecoParams() { m_guiReco = 0; }
  RecoParams( const RecoParams &other );
  
  RecoParams& operator=( const RecoParams &other );

protected:
  void copyFrom( const RecoParams &other );

public:
  /*********************/
  /*   GUI Functions   */
  /*********************/
  RecoGUI* createGUI( QWidget *parent=0, const char* name=0 );
  void     setGUI( RecoGUI* pGUI ) { m_guiReco = pGUI; }

public:
  /**************************/
  /*   File I/O Functions   */
  /**************************/
  
public:
  /************************/
  /*   Parameter Access   */
  /************************/
  bool      isValid() const { return (m_guiReco != 0); }
  RecoGUI*  params(); 

protected:
  RecoGUI* m_guiReco;
};


#endif
