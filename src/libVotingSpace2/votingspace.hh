/*********************************************************************/
/*                                                                   */
/* FILE         votingspace.hh                                       */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Fast implementation for a hough transform voting     */
/*              space, where search can be refined by Mean-Shift     */
/*              Mode Estimation (MSME).                              */
/*                                                                   */
/*                                                                   */
/* BEGIN        Tue Oct 22 2003                                      */
/* LAST CHANGE  Fri Nov 28 2003                                      */
/*                                                                   */
/*********************************************************************/

#ifndef VOTINGSPACE_HH
#define VOTINGSPACE_HH

using namespace std;

/****************/
/*   Includes   */
/****************/
#include <vector>
#include <list>
#include <cassert>

#include <featurevector.hh>

/*******************/
/*   Definitions   */
/*******************/
const float EPS_MSME       = 0.01;

const int KERNEL_HCUBE     = 0;
const int KERNEL_HSPHERE   = 1;


/*************************/
/*   Class Definitions   */
/*************************/

/*===================================================================*/
/*                          Class HoughVote                          */
/*===================================================================*/
class HoughVote 
{
public:
  HoughVote( float x, 
             float dValue, float dConfidence, 
             int nImgPointId, int nClusterId, int nOccNumber, int nOccMapId=-1,
             int nCueId=0 );
  HoughVote( float x, float y,
             float dValue, float dConfidence, 
             int nImgPointId, int nClusterId, int nOccNumber, int nOccMapId=-1,
             int nCueId=0 );
  HoughVote( float x, float y, float z,
             float dValue, float dConfidence, 
             int nImgPointId, int nClusterId, int nOccNumber, int nOccMapid=-1,
             int nCueId=0 );
  HoughVote( float x, float y, float z, float a,
             float dValue, float dConfidence, 
             int nImgPointId, int nClusterId, int nOccNumber, int nOccMapId=-1,
             int nCueId=0 );
  HoughVote( FeatureVector fvCoords, 
             float dValue, float dConfidence, 
             int nImgPointId, int nClusterId, int nOccNumber, int nOccMapId=-1,
             int nCueId=0 );
  HoughVote( const HoughVote &other );

  HoughVote& operator=( const HoughVote &other );

private:
  void createHoughVote( FeatureVector fvCoords, 
                        float dValue, float dConfidence, 
                        int nImgPointId, int nClusterId, int nOccNumber,
                        int nOccMapId=-1, int nCueId=0 );
  void copyFromOther  ( const HoughVote &other );

public:
  /*****************************/
  /*   Data Access Operators   */
  /*****************************/
  inline FeatureVector getCoords() const { return m_fvCoords; }
  inline float getValue()          const { return m_dValue; }
  inline float getConfidence()     const { return m_dConfidence; }
  inline int   getImgPointId()     const { return m_nImgPointId; }
  inline int   getClusterId()      const { return m_nClusterId; }
  inline int   getOccNumber()      const { return m_nOccNumber; } 
  inline int   getOccMapId()       const { return m_nOccMapId; } 
  inline int   getCueId()          const { return m_nCueId; }

  inline void  setCoords    ( FeatureVector fvCoords ) { m_fvCoords=fvCoords; }
  inline void  setValue     ( float dValue )      { m_dValue=dValue; }
  inline void  setConfidence( float dConfidence ) { m_dConfidence=dConfidence;}
  inline void  setImgPointId( int nImgPointId )   { m_nImgPointId=nImgPointId;}
  inline void  setClusterId ( int nClusterId )    { m_nClusterId=nClusterId; }
  inline void  setOccNumber ( int nOccNumber )    { m_nOccNumber=nOccNumber; } 
  inline void  setOccMapId  ( int nOccMapId )     { m_nOccMapId=nOccMapId; } 
  inline void  setCueId     ( int nCueId )        { m_nCueId=nCueId; }

public:
  FeatureVector m_fvCoords;
  float m_dValue;
  float m_dConfidence;

  int   m_nImgPointId;
  int   m_nClusterId;
  int   m_nOccNumber;
  int   m_nOccMapId;
  int   m_nCueId;
};

/*-------------------------------------------------------------------*/
/*                         Sorting Operators                         */
/*-------------------------------------------------------------------*/
struct compHoughValue
{
  bool operator()( HoughVote x, HoughVote y )
  { return (x.getValue() > y.getValue()); }
};

struct compHoughCluster
{
  bool operator()( HoughVote x, HoughVote y )
  { return (x.getClusterId() < y.getClusterId()); }
};

struct compHoughOcc
{
  bool operator()( HoughVote x, HoughVote y )
  { return (x.getOccNumber() < y.getOccNumber()); }
};

struct compHoughOccMap
{
  bool operator()( HoughVote x, HoughVote y )
  { return (x.getOccMapId() < y.getOccMapId()); }
};

struct compHoughPoint : public binary_function<HoughVote,HoughVote,bool>
{
  bool operator()( HoughVote x, HoughVote y )
  { return (x.getImgPointId() < y.getImgPointId()); }
};

struct compHoughPointIsEqual
{
  bool operator()( HoughVote x, HoughVote y )
  { return (x.getImgPointId() == y.getImgPointId()); }
};




/*===================================================================*/
/*                         Class VotingSpace                         */
/*===================================================================*/
class VotingSpace
{
public:
  VotingSpace();
  VotingSpace( int nBins, float min, float max );
  VotingSpace( int nBins_x, float min_x, float max_x,
               int nBins_y, float min_y, float max_y );
  VotingSpace( int nBins_x, float min_x, float max_x,
               int nBins_y, float min_y, float max_y,
               int nBins_z, float min_z, float max_z );
  VotingSpace( int nBins_x, float min_x, float max_x,
               int nBins_y, float min_y, float max_y,
               int nBins_z, float min_z, float max_z,
               int nBins_s, float min_s, float max_s );
  VotingSpace( int nDims, vector<int> vNumBins, 
               vector<float> vMinValues, vector<float> vMaxValues );
  VotingSpace( const VotingSpace &other );
  ~VotingSpace();

  VotingSpace& operator=( const VotingSpace &other );

private:
  void copyFromOther  ( const VotingSpace &other );

  void  initDimension ( int dim, int nBins, float min, float max );
  void  initBins();

  int   calcTotalNumBins();
  
public:
  /*******************************/
  /*   Content Access Operators  */
  /*******************************/
  bool  isValid();

  int   numDims()          { assert( m_bSizeDefined ); return m_nDims; }
  int   numBins( int dim ) 
  { assert( (dim < m_nDims) && m_vDimDefined[dim] ); return m_vNumBins[dim]; }
  float minValue( int dim )
  { assert( (dim<m_nDims) && m_vDimDefined[dim] ); return m_vMinValues[dim]; }
  float maxValue( int dim )
  { assert( (dim<m_nDims) && m_vDimDefined[dim] ); return m_vMaxValues[dim]; }

  void  setNumDims  ( int nDims );
  void  setDimension( int dim, int nBins, float min, float max );

  void  clear();

  void  print();
  void  printContent();

public:
  /*****************************/
  /*   Voting Space Creation   */
  /*****************************/

  void  insertVote    ( const HoughVote &vote );

protected:
  int   getBinNumber  ( int dim, double value );

public:
  /**********************/
  /*   Maximum Search   */
  /**********************/
  float getBinVoteSum ( int idx_x );
  float getBinVoteSum ( int idx_x, int idx_y );
  float getBinVoteSum ( int idx_x, int idx_y, int idx_z );
  float getBinVoteSum ( int idx_x, int idx_y, int idx_z, int idx_s );
  float getBinVoteSum ( vector<int> vBinIdx );

  void  setWindowSize ( float wx );
  void  setWindowSize ( float wx, float wy );
  void  setWindowSize ( float wx, float wy, float wz );
  void  setWindowSize ( float wx, float wy, float wz, float ws );
  void  setWindowSize ( const FeatureVector &fvWindowSize );

  static const int   KERNEL_HCUBE   = 0;
  static const int   KERNEL_HSPHERE = 1;
  void  setKernelType ( int nType );

  float getVoteSum    ( float x );
  float getVoteSum    ( float x, float y );
  float getVoteSum    ( float x, float y, float z );
  float getVoteSum    ( float x, float y, float z, float s );
  float getVoteSum    ( const FeatureVector &fvCoords );

  void  getMeanVote   ( float x, 
                        FeatureVector &fvMean, int &nNumVotes, 
                        float &dSumScore );
  void  getMeanVote   ( float x, float y, 
                        FeatureVector &fvMean, int &nNumVotes, 
                        float &dSumScore );
  void  getMeanVote   ( float x, float y, float z,
                        FeatureVector &fvMean, int &nNumVotes, 
                        float &dSumScore );
  void  getMeanVote   ( float x, float y, float z, float s,
                        FeatureVector &fvMean, int &nNumVotes, 
                        float &dSumScore );
  void  getMeanVote   ( const FeatureVector &fvCoords,
                        FeatureVector &fvMean, int &nNumVotes, 
                        float &dSumScore );

  void getFastKernelVoteSum( int x,
                             float &dSumScore );
  void getFastKernelVoteSum( int x, int y,
                             float &dSumScore );
  void getFastKernelVoteSum( int x, int y, int z,
                             float &dSumScore );
  void getFastKernelVoteSum( int x, int y, int z, int s,
                             float &dSumScore );
  void getFastKernelVoteSum( const vector<int> &vBinIdx,
                             float &dSumScore );

  void getFastMeanVote     ( const FeatureVector &fvCoords,
                             FeatureVector &fvMean, int &nNumVotes, 
                             float &dSumScore );
  void getFastMeanVote     ( const vector<int> &vBinIdx,
                             FeatureVector &fvMean, int &nNumVotes, 
                             float &dSumScore );

  list<HoughVote> getSupportingVotes( float x );
  list<HoughVote> getSupportingVotes( float x, float y );
  list<HoughVote> getSupportingVotes( float x, float y, float z );  
  list<HoughVote> getSupportingVotes( float x, float y, float z, float s );  
  list<HoughVote> getSupportingVotes( const FeatureVector &fvCoords );

  void applyMSME( float x, 
                  FeatureVector &fvResult, float &dScore,
                  bool bUseFastMSME=false );
  void applyMSME( float x, float y, 
                  FeatureVector &fvResult, float &dScore,
                  bool bUseFastMSME=false );
  void applyMSME( float x, float y, float z, 
                  FeatureVector &fvResult, float &dScore, 
                  bool bAdaptiveScale=false, float dAdaptMinScale=1.0,
                  bool bUseFastMSME=false );
  void applyMSME( float x, float y, float z, float s,
                  FeatureVector &fvResult, float &dScore, 
                  bool bAdaptiveScale=false, float dAdaptMinScale=1.0,
                  bool bUseFastMSME=false );
  void applyMSME( const FeatureVector &fvCoords, 
                  FeatureVector &fvResult, float &dScore,
                  bool bAdaptiveScale=false, float dAdaptMinScale=1.0,
                  bool bUseFastMSME=false );

  list<HoughVote> getBinVotes     ( int idx_x );
  list<HoughVote> getBinVotes     ( int idx_x, int idx_y );
  list<HoughVote> getBinVotes     ( int idx_x, int idx_y, int idx_z );
  list<HoughVote> getBinVotes     ( int idx_x, int idx_y, int idx_z, 
                                    int idx_s );
  list<HoughVote> getBinVotes     ( const vector<int> &vBinIdx );

  //protected:
public:
  float getVoteSumInWindow          ( const vector<int> &vBinIdx, 
                                      const FeatureVector &fvStart );
  void  getMeanVoteInWindow         ( const vector<int> &vBinIdx, 
                                      const FeatureVector &fvStart,
                                      FeatureVector &fvMean, int &nNumVotes,
                                      float &dSumScore );
  list<HoughVote> getVotesInWindow( const vector<int> &vBinIdx, 
                                      const FeatureVector &fvStart );

  bool  isInsideKernel              ( const FeatureVector &fvCoords,
                                      const FeatureVector &fvCenter );

protected:
  int   idx( int x );
  int   idx( int x, int y );
  int   idx( int x, int y, int z );
  int   idx( int x, int y, int z, int s );
  int   idx( vector<int> index );

  vector<int> getNextIndex( const vector<int> &index, bool &endReached );

  float getIntersectingVolume3D( int x, int y, int z,
                                 const FeatureVector &fvMean,
                                 const FeatureVector &fvWindowSize );
  float getIntersectingVolume3D( const vector<int>   &vBinIdx,
                                 const FeatureVector &fvMean,
                                 const FeatureVector &fvWindowSize );

  int            m_nDims;
  vector<int>    m_vNumBins;   // contains the #bins for each dimension
  vector<float>  m_vMinValues; // contains the min value for each dimension
  vector<float>  m_vMaxValues; // contains the max value for each dimension
  vector<float>  m_vRes;       // contains the resolution for each dimension
  int            m_nTotalNumBins;
  int            m_nKernelType;

  bool           m_bSizeDefined;
  vector<bool>   m_vDimDefined;
  bool           m_bAllDimensionsDefined;

  vector< list<HoughVote> > m_vlVotes;
  vector<float>               m_vBinScores;
  vector<FeatureVector>       m_vBinMeans;

  FeatureVector  m_fvWindowSize;
};


#endif   // VOTINGSPACE_HH
