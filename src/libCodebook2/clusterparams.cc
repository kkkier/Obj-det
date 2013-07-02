/*********************************************************************/
/*                                                                   */
/* FILE         clusterparams.cc                                     */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Class for clustering parameters.                     */
/*                                                                   */
/* BEGIN        Wed Mar 16 2005                                      */
/* LAST CHANGE  Wed Mar 16 2005                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>

#include "clusterparams.hh"

/*******************/
/*   Definitions   */
/*******************/

/*===================================================================*/
/*                         Class ClusterParams                       */
/*===================================================================*/

ClusterParams::ClusterParams( const ClusterParams &other )
/* copy constructor */
{
  copyFrom( other );
}


ClusterParams& ClusterParams::operator=( const ClusterParams &other )
  /* assignment operator */
{
  copyFrom( other );
  return *this;
}


void ClusterParams::copyFrom( const ClusterParams& other )
  /* copy operator */
{
  m_guiCluster = other.m_guiCluster;
}


ClusterGUI* ClusterParams::createGUI( QWidget *parent, const char* name )
{
  if( m_guiCluster != 0 ) {
    cerr << "  Error in ClusterParams::createGUI(): "
         << "Tried to initialize the GUI twice!" << endl;
    return 0;

  } else {
    cout << "  Initializing cluster GUI for window '" << name << "'..." 
         << endl;
    m_guiCluster = new ClusterGUI( parent, name );
  }

  return m_guiCluster;
}


ClusterGUI* ClusterParams::params() const
{
  assert( m_guiCluster != 0 );

  return m_guiCluster;
}


