/*********************************************************************/
/*                                                                   */
/* FILE         veriparams.hh                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Class for verification parameters.                   */
/*                                                                   */
/* BEGIN        Wed Mar 23 2005                                      */
/* LAST CHANGE  Wed Mar 23 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef VERIPARAMS_HH
#define VERIPARAMS_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>

#include <qwidget.h>
#include <qstring.h>

#include <featurevector.hh>
#include <opgrayimage.hh>

#include "verigui.hh"

/*******************/
/*   Definitions   */
/*******************/


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                       Class VeriParameters                        */
/*===================================================================*/
class VeriParams
{
public:
  VeriParams() { m_guiVeri = 0; }
  VeriParams( const VeriParams &other );
  
  VeriParams& operator=( const VeriParams &other );

protected:
  void copyFrom( const VeriParams &other );

public:
  /*********************/
  /*   GUI Functions   */
  /*********************/
  VeriGUI* createGUI( QWidget *parent=0, const char* name=0 );

public:
  /**************************/
  /*   File I/O Functions   */
  /**************************/
  
public:
  /************************/
  /*   Parameter Access   */
  /************************/
  VeriGUI*  params(); 

protected:
  VeriGUI* m_guiVeri;
};


#endif
