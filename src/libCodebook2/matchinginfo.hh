/*********************************************************************/
/*                                                                   */
/* FILE         matchinginfo.hh                                      */
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

#ifndef MATCHINGINFO_HH
#define MATCHINGINFO_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <cassert>

#include <container.hh>

/*******************/
/*   Definitions   */
/*******************/

/*************************/
/*   Class Definitions   */
/*************************/
/*===================================================================*/
/*                        Class MatchingInfo                         */
/*===================================================================*/
class MatchingInfo : public Container
{
public:
  MatchingInfo();
  MatchingInfo( const vector<int>            &vNN,
                const vector<float>          &vNNSim,
                const vector<vector<int> >   &vvAllNeighbors,
                const vector<vector<float> > &vvAllNeighborsSim );
  MatchingInfo( const MatchingInfo &other );

  MatchingInfo& operator=( const MatchingInfo &other );

protected:
  void copyFrom( const MatchingInfo &other );

public:
  /*******************************/
  /*   Content Access Operators  */
  /*******************************/
  vector<int>&            getNN()               { return m_vNN; }
  vector<float>&          getNNSim()            { return m_vNNSim; }
  vector<vector<int> >&   getAllNeighbors()     { return m_vvAllNeighbors; }
  vector<vector<float> >& getAllNeighborsSim()  { return m_vvAllNeighborsSim; }
  
  void setContent( const vector<int>            &vNN,
                   const vector<float>          &vNNSim,
                   const vector<vector<int> >   &vvAllNeighbors,
                   const vector<vector<float> > &vvAllNeighborsSim );

protected:
  vector<int>            m_vNN;
  vector<float>          m_vNNSim;
  vector<vector<int> >   m_vvAllNeighbors;
  vector<vector<float> > m_vvAllNeighborsSim;
};

typedef SharedContainer<MatchingInfo> SharedMatchingInfo;

#endif
