/*********************************************************************/
/*                                                                   */
/* FILE         recoparams.cc                                        */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Class for recognition parameters.                    */
/*                                                                   */
/* BEGIN        Thu Jan 20 2005                                      */
/* LAST CHANGE  Thu Jan 20 2005                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>

#include "recoparams.hh"

/*******************/
/*   Definitions   */
/*******************/

/*===================================================================*/
/*                         Class RecoParams                          */
/*===================================================================*/

RecoParams::RecoParams( const RecoParams &other )
/* copy constructor */
{
  copyFrom( other );
}


RecoParams& RecoParams::operator=( const RecoParams &other )
  /* assignment operator */
{
  copyFrom( other );
  return *this;
}


void RecoParams::copyFrom( const RecoParams& other )
  /* copy operator */
{
  m_guiReco = other.m_guiReco;
}


RecoGUI* RecoParams::createGUI( QWidget *parent, const char* name )
{
  if( m_guiReco != 0 ) {
    cerr << "  Error in RecoParams::createGUI(): "
         << "Tried to initialize the GUI twice!" << endl;
    return 0;

  } else {
    //cout << "  Initializing reco GUI for window '" << name << "'..." 
    //     << endl;
    m_guiReco = new RecoGUI( parent, name );
  }

  return m_guiReco;
}


RecoGUI* RecoParams::params()
{
  assert( m_guiReco != 0 );

  return m_guiReco;
}


