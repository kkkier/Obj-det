/*********************************************************************/
/*                                                                   */
/* FILE         matchingparams.cc                                    */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@inf.ethz.ch                                    */
/*                                                                   */
/* CONTENT      Class for codebook matching parameters.              */
/*                                                                   */
/* BEGIN        Tue Mar 22 2005                                      */
/* LAST CHANGE  Tue Mar 22 2005                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <cassert>

#include "matchingparams.hh"

/*******************/
/*   Definitions   */
/*******************/

/*===================================================================*/
/*                       Class MatchingParams                        */
/*===================================================================*/

MatchingParams::MatchingParams( const MatchingParams &other )
/* copy constructor */
{
  copyFrom( other );
}


MatchingParams& MatchingParams::operator=( const MatchingParams &other )
  /* assignment operator */
{
  copyFrom( other );
  return *this;
}


void MatchingParams::copyFrom( const MatchingParams& other )
  /* copy operator */
{
  m_guiMatching = other.m_guiMatching;
}


MatchingGUI* MatchingParams::createGUI( QWidget *parent, const char* name )
{
  if( m_guiMatching != 0 ) {
    cerr << "  Error in MatchingParams::createGUI(): "
         << "Tried to initialize the GUI twice!" << endl;
    return 0;

  } else {
    cout << "  Initializing matching GUI for window '" << name << "'..." 
         << endl;
    m_guiMatching = new MatchingGUI( parent, name );
  }

  return m_guiMatching;
}


MatchingGUI* MatchingParams::params() const
{
  assert( m_guiMatching != 0 );

  return m_guiMatching;
}

