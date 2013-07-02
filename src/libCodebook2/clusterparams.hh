/*********************************************************************/
/*                                                                   */
/* FILE         clusterparams.hh                                     */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Class for clustering parameters.                     */
/*                                                                   */
/* BEGIN        Wed Mar 16 2005                                      */
/* LAST CHANGE  Wed Mar 16 2005                                      */
/*                                                                   */
/*********************************************************************/

#ifndef CLUSTERPARAMS_HH
#define CLUSTERPARAMS_HH

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

#include "clustergui.hh"

/*******************/
/*   Definitions   */
/*******************/


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                       Class ClusterParameters                     */
/*===================================================================*/
class ClusterParams
{
public:
  ClusterParams() { m_guiCluster = 0; }
  ClusterParams( const ClusterParams &other );
  
  ClusterParams& operator=( const ClusterParams &other );

protected:
  void copyFrom( const ClusterParams &other );

public:
  /*********************/
  /*   GUI Functions   */
  /*********************/
  ClusterGUI* createGUI( QWidget *parent=0, const char* name=0 );
  void        setGUI   ( ClusterGUI* pGUI ) { m_guiCluster = pGUI; }

public:
  /**************************/
  /*   File I/O Functions   */
  /**************************/
  
public:
  /************************/
  /*   Parameter Access   */
  /************************/
  bool         isValid() const { return (m_guiCluster != 0); }
  ClusterGUI*  params() const; 

protected:
  ClusterGUI* m_guiCluster;
};


#endif
