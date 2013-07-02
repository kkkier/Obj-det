/*********************************************************************/
/*                                                                   */
/* FILE         matchinginfo.cc                                      */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Definition of a convenience class for storing code-  */
/*              book matching results.                               */
/*                                                                   */
/* BEGIN        Mon Oct 17 2005                                      */
/* LAST CHANGE  Mon Oct 17 2005                                      */
/*                                                                   */
/*********************************************************************/

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <algorithm>

#include "matchinginfo.hh"

/*===================================================================*/
/*                         Class MatchingInfo                        */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

MatchingInfo::MatchingInfo()
/* standard constructor */
{
  m_vNN.clear();
  m_vNNSim.clear();
  m_vvAllNeighbors.clear();
  m_vvAllNeighborsSim.clear();
}


MatchingInfo::MatchingInfo( const vector<int>            &vNN,
                            const vector<float>          &vNNSim,
                            const vector<vector<int> >   &vvAllNeighbors,
                            const vector<vector<float> > &vvAllNeighborsSim )
/* standard constructor */
{
  m_vNN               = vNN;
  m_vNNSim            = vNNSim;
  m_vvAllNeighbors    = vvAllNeighbors;
  m_vvAllNeighborsSim = vvAllNeighborsSim;
}


MatchingInfo::MatchingInfo( const MatchingInfo &other )
  : Container( other )
/* copy constructor */
{
  copyFrom( other );
}


MatchingInfo& MatchingInfo::operator=( const MatchingInfo &other )
/* assignment operator */
{
  copyFrom( other );
  return (*this);
}


void MatchingInfo::copyFrom( const MatchingInfo &other )
{
  m_vNN               = other.m_vNN;
  m_vNNSim            = other.m_vNNSim;
  m_vvAllNeighbors    = other.m_vvAllNeighbors;
  m_vvAllNeighborsSim = other.m_vvAllNeighborsSim;
}


void MatchingInfo::setContent( const vector<int>            &vNN,
                               const vector<float>          &vNNSim,
                               const vector<vector<int> >   &vvAllNeighbors,
                               const vector<vector<float> > &vvAllNeighborsSim)
{
  m_vNN               = vNN;
  m_vNNSim            = vNNSim;
  m_vvAllNeighbors    = vvAllNeighbors;
  m_vvAllNeighborsSim = vvAllNeighborsSim;
}


