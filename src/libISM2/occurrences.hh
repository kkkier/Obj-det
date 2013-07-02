/*********************************************************************/
/*                                                                   */
/* FILE         occurrences.hh                                       */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Implements functions for creating, loading, and      */
/*              saving occurrences and cooccurrences.                */
/*                                                                   */
/*                                                                   */
/* BEGIN        Tue Mar 18 2003                                      */
/* LAST CHANGE  Sat Jul 03 2004                                      */
/*                                                                   */
/*********************************************************************/

#ifndef OCCURRENCES_HH
#define OCCURRENCES_HH
 
using namespace std;
  
/****************/
/*   Includes   */
/****************/
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>

#include <featurevector.hh>
#include <opgrayimage.hh>

/*******************/
/*   Definitions   */
/*******************/
/*--------------------*/
/* Cluster Occurrence */
/*--------------------*/
typedef struct _ClusterOccurrence
{
public:
  float dSimilarity;
  int nCategory;
  int nPose;

  float dScale;
  float dPosX;
  float dPosY;
  int   nImgNumber;
  int   nOccMapIdx;
  float dWeight;
  float dAngle;
  float dAxisRatio;
  float dBBRatio;
} ClusterOccurrence;


/*-------------------------------------------------------------------*/
/*                         Sorting Operators                         */
/*-------------------------------------------------------------------*/
struct compOccCateg
{
  bool operator()( ClusterOccurrence x, ClusterOccurrence y )
  { return (x.nCategory < y.nCategory); }
};

struct compOccPose
{
  bool operator()( ClusterOccurrence x, ClusterOccurrence y )
  { return (x.nPose < y.nPose); }
};

struct compOccSim
{
  bool operator()( ClusterOccurrence x, ClusterOccurrence y )
  { return (x.dSimilarity > y.dSimilarity); }
};

struct compOccScale
{
  bool operator()( ClusterOccurrence x, ClusterOccurrence y )
  { return (x.dScale < y.dScale); }
};

struct compOccPosX
{
  bool operator()( ClusterOccurrence x, ClusterOccurrence y )
  { return (x.dPosX < y.dPosX); }
};

struct compOccPosY
{
  bool operator()( ClusterOccurrence x, ClusterOccurrence y )
  { return (x.dPosY < y.dPosY); }
};

struct compOccMapIdx
{
  bool operator()( ClusterOccurrence x, ClusterOccurrence y )
  { return (x.nOccMapIdx < y.nOccMapIdx); }
};

/*----------------------*/
/* Cluster Cooccurrence */
/*----------------------*/
typedef struct _ClusterCooccurrence
{
public:
  float dSimilarity1;
  float dSimilarity2;
  int nCategory;
  int nPose;
  
  float dAngle;
  float dDistance;

  float dScale;
  float dPosX;
  float dPosY;
} ClusterCooccurrence;

typedef vector<ClusterOccurrence>             VecOccurrence;
typedef vector< vector<ClusterOccurrence> >   VecVecOccurrence;
typedef vector< vector<ClusterCooccurrence> > VecVecCooccurrence;
typedef vector< vector<float> >               VecVecCooccDistance;


/***************************/
/*   Function Prototypes   */
/***************************/
  
void saveOccurrences      ( string sFileName, VecVecOccurrence vvOccurrences,
                            bool bVerbose=true );
void saveOccurrencesMatlab( string sFileName, VecVecOccurrence vvOccurrences,
                            bool bVerbose=true );
int  loadOccurrences      ( string sFileName, int nNumClusters,
                            VecVecOccurrence &vvOccurrences,
                            bool bVerbose=true );

void saveOccurrenceMaps   ( string sFileName, 
                            /*const*/ vector<OpGrayImage> &vOccMaps,
                            bool bVerbose=true );
void loadOccurrenceMaps   ( string sFileName, 
                            vector<OpGrayImage> &vOccMaps,
                            bool bVerbose=true );

void saveNegOccurrences   ( string sFileName, const vector<int> &vNegOccs,
                            bool bVerbose=true );
void loadNegOccurrences   ( string sFileName, int nNumClusters,
                            vector<int> &vNegOccs, bool bVerbose=true );

void saveCooccurrences    ( string sFileName, int nNumClusters,
                            VecVecCooccurrence vvCooccurrences );
void loadCooccurrences    ( string sFileName, int nNumClusters,
                            VecVecCooccurrence &vvCooccurrences );

void saveCooccDistances   ( string sFileName, int nNumClusters,
                            VecVecCooccDistance vvCooccDistances  );
void loadCooccDistances   ( string sFileName, int nNumClusters,
                            VecVecCooccDistance &vvCooccDistances );

#endif
