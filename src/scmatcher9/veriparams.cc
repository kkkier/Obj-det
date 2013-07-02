/*********************************************************************/
/*                                                                   */
/* FILE         veriparams.cc                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Class for verification parameters.                   */
/*                                                                   */
/* BEGIN        Wed Mar 23 2005                                      */
/* LAST CHANGE  Wed Mar 23 2005                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>

#include "veriparams.hh"

/*******************/
/*   Definitions   */
/*******************/

/*===================================================================*/
/*                         Class VeriParams                          */
/*===================================================================*/

VeriParams::VeriParams( const VeriParams &other )
/* copy constructor */
{
  copyFrom( other );
}


VeriParams& VeriParams::operator=( const VeriParams &other )
  /* assignment operator */
{
  copyFrom( other );
  return *this;
}


void VeriParams::copyFrom( const VeriParams& other )
  /* copy operator */
{
  m_guiVeri = other.m_guiVeri;
}


VeriGUI* VeriParams::createGUI( QWidget *parent, const char* name )
{
  if( m_guiVeri != 0 ) {
    cerr << "  Error in VeriParams::createGUI(): "
         << "Tried to initialize the GUI twice!" << endl;
    return 0;

  } else {
    cout << "  Initializing reco GUI for window '" << name << "'..." 
         << endl;
    m_guiVeri = new VeriGUI( parent, name );
  }

  return m_guiVeri;
}


VeriGUI* VeriParams::params()
{
  assert( m_guiVeri != 0 );

  return m_guiVeri;
}


