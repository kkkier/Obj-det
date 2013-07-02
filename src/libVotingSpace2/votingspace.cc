/*********************************************************************/
/*                                                                   */
/* FILE         votingspace.cc                                       */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Implements functions for a generalized Hough trans-  */
/*              form and a Mean-Shift Mode Estimator (MSME).         */
/*                                                                   */
/*                                                                   */
/* BEGIN        Sun Oct 26 2003                                      */
/* LAST CHANGE  Fri Nov 28 2003                                      */
/*                                                                   */
/*********************************************************************/


/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <algorithm>

#include "votingspace.hh"


/*===================================================================*/
/*                           Class HoughVote                         */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/
HoughVote::HoughVote( float x, 
                      float dValue, float dConfidence, 
                      int nImgPointId, int nClusterId, int nOccNumber, 
                      int nOccMapId, int nCueId )
{
  FeatureVector fvCoords( 1 );
  fvCoords.setValue( 0, x );
  createHoughVote( fvCoords, dValue, dConfidence, 
                   nImgPointId, nClusterId, nOccNumber, nOccMapId, nCueId );
}


HoughVote::HoughVote( float x, float y,
                      float dValue, float dConfidence, 
                      int nImgPointId, int nClusterId, int nOccNumber, 
                      int nOccMapId, int nCueId )
{
  FeatureVector fvCoords( 2 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  createHoughVote( fvCoords, dValue, dConfidence, 
                   nImgPointId, nClusterId, nOccNumber, nOccMapId, nCueId );
}


HoughVote::HoughVote( float x, float y, float z,
                      float dValue, float dConfidence, 
                      int nImgPointId, int nClusterId, int nOccNumber,
                      int nOccMapId, int nCueId )
{
  FeatureVector fvCoords( 3 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  fvCoords.setValue( 2, z );
  createHoughVote( fvCoords, dValue, dConfidence, 
                   nImgPointId, nClusterId, nOccNumber, nOccMapId, nCueId );
}


HoughVote::HoughVote( float x, float y, float z, float s,
                      float dValue, float dConfidence, 
                      int nImgPointId, int nClusterId, int nOccNumber, 
                      int nOccMapId, int nCueId )
{
  FeatureVector fvCoords( 4 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  fvCoords.setValue( 2, z );
  fvCoords.setValue( 3, s );
  createHoughVote( fvCoords, dValue, dConfidence, 
                   nImgPointId, nClusterId, nOccNumber, nOccMapId, nCueId );
}


HoughVote::HoughVote( FeatureVector fvCoords, 
                      float dValue, float dConfidence, 
                      int nImgPointId, int nClusterId, int nOccNumber,
                      int nOccMapId, int nCueId )
{
  createHoughVote( fvCoords, dValue, dConfidence, 
                   nImgPointId, nClusterId, nOccNumber, nOccMapId, nCueId );
}


HoughVote::HoughVote( const HoughVote &other )
  /* copy constructor */
{
  copyFromOther( other );
} 


HoughVote& HoughVote::operator=( const HoughVote &other )
  /* assignment operator */
{
  copyFromOther( other );
  return *this;
}


void HoughVote::createHoughVote( FeatureVector fvCoords, 
                                 float dValue, float dConfidence, 
                                 int nImgPointId, int nClusterId, 
                                 int nOccNumber, int nOccMapId, int nCueId )
{
  m_fvCoords    = fvCoords;
  m_dValue      = dValue;
  m_dConfidence = dConfidence;
  m_nImgPointId = nImgPointId;
  m_nClusterId  = nClusterId;
  m_nOccNumber  = nOccNumber;
  m_nOccMapId   = nOccMapId;
  m_nCueId      = nCueId;
}


void HoughVote::copyFromOther( const HoughVote &other )
{
  m_fvCoords    = other.m_fvCoords;
  m_dValue      = other.m_dValue;
  m_dConfidence = other.m_dConfidence;
  m_nImgPointId = other.m_nImgPointId;
  m_nClusterId  = other.m_nClusterId;
  m_nOccNumber  = other.m_nOccNumber;
  m_nOccMapId   = other.m_nOccMapId;
  m_nCueId      = other.m_nCueId;
}


/*===================================================================*/
/*                         Class VotingSpace                         */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/
VotingSpace::VotingSpace()
  /* standard constructor */
{
  clear();
  m_nDims = 0;
  m_bSizeDefined = false;
  m_bAllDimensionsDefined = false;
  m_nKernelType = KERNEL_HCUBE;
}


VotingSpace::VotingSpace( int nBins, float min, float max )
  /* alternate constructor for 1D voting space */
{
  clear();
  m_nDims = 1;
  m_bSizeDefined = true;

  m_vNumBins.resize(1);
  m_vMinValues.resize(1);
  m_vMaxValues.resize(1);
  m_vRes.resize(1);
  m_vDimDefined.resize(1);

  initDimension( 0, nBins, min, max );
  initBins();
  m_bAllDimensionsDefined = true;
  m_nKernelType = KERNEL_HCUBE;
}


VotingSpace::VotingSpace( int nBins_x, float min_x, float max_x,
                          int nBins_y, float min_y, float max_y )
  /* alternate constructor for 2D voting space */
{
  clear();
  m_nDims = 2;
  m_bSizeDefined = true;

  m_vNumBins.resize(2);
  m_vMinValues.resize(2);
  m_vMaxValues.resize(2);
  m_vRes.resize(2);
  m_vDimDefined.resize(2);

  initDimension( 0, nBins_x, min_x, max_x );
  initDimension( 1, nBins_y, min_y, max_y );
  initBins();
  m_bAllDimensionsDefined = true;
  m_nKernelType = KERNEL_HCUBE;
}


VotingSpace::VotingSpace( int nBins_x, float min_x, float max_x,
                          int nBins_y, float min_y, float max_y,
                          int nBins_z, float min_z, float max_z )
  /* alternate constructor for 3D voting space */
{
  clear();
  m_nDims = 3;
  m_bSizeDefined = true;

  m_vNumBins.resize(3);
  m_vMinValues.resize(3);
  m_vMaxValues.resize(3);
  m_vRes.resize(3);
  m_vDimDefined.resize(3);

  initDimension( 0, nBins_x, min_x, max_x );
  initDimension( 1, nBins_y, min_y, max_y );
  initDimension( 2, nBins_z, min_z, max_z );
  initBins();
  m_bAllDimensionsDefined = true;
  m_nKernelType = KERNEL_HCUBE;
}


VotingSpace::VotingSpace( int nBins_x, float min_x, float max_x,
                          int nBins_y, float min_y, float max_y,
                          int nBins_z, float min_z, float max_z,
                          int nBins_s, float min_s, float max_s )
  /* alternate constructor for 4D voting space */
{
  clear();
  m_nDims = 4;
  m_bSizeDefined = true;

  m_vNumBins.resize(4);
  m_vMinValues.resize(4);
  m_vMaxValues.resize(4);
  m_vRes.resize(4);
  m_vDimDefined.resize(4);

  initDimension( 0, nBins_x, min_x, max_x );
  initDimension( 1, nBins_y, min_y, max_y );
  initDimension( 2, nBins_z, min_z, max_z );
  initDimension( 3, nBins_s, min_s, max_s );
  initBins();
  m_bAllDimensionsDefined = true;
  m_nKernelType = KERNEL_HCUBE;
}


VotingSpace::VotingSpace( int nDims, vector<int> vNumBins, 
                          vector<float> vMinValues, vector<float> vMaxValues )
  /* alternate constructor for voting spaces of arbitrary dimension */
{
  assert( (int)vNumBins.size()   == nDims );
  assert( (int)vMinValues.size() == nDims );
  assert( (int)vMaxValues.size() == nDims );

  clear();
  m_nDims        = nDims;
  m_bSizeDefined = true;

  m_vNumBins     = vNumBins;
  m_vMinValues   = vMinValues;
  m_vMaxValues   = vMaxValues;

  m_vDimDefined.resize( m_nDims );  
  m_vRes.resize( m_nDims );  
  for ( int i=0; i<m_nDims; i++ ) {
    m_vRes[i] = (m_vMaxValues[i] - m_vMinValues[i]) / ((float) m_vNumBins[i]);
    m_vDimDefined[i] = true;
  }
  initBins();
  m_bAllDimensionsDefined = true;
  m_nKernelType = KERNEL_HCUBE;
}


VotingSpace::VotingSpace( const VotingSpace &other )
  : m_vNumBins(other.m_vNumBins)
  , m_vMinValues(other.m_vMinValues)
  , m_vMaxValues(other.m_vMaxValues)
  , m_vRes(other.m_vRes)
  , m_vDimDefined(other.m_vDimDefined) 
  , m_vlVotes(other.m_vlVotes)
  , m_vBinScores(other.m_vBinScores)
  /* copy constructor */
{
  copyFromOther( other );
}


VotingSpace::~VotingSpace()
  /* standard destructor */
{
  clear();
}


VotingSpace& VotingSpace::operator=( const VotingSpace &other )
  /* assignment operator */
{
  copyFromOther( other );
  return *this;
}


void VotingSpace::copyFromOther( const VotingSpace &other )
  /*******************************************************************/
  /* Auxiliary function to copy from another voting space.           */
  /*******************************************************************/
{
  m_nDims        = other.m_nDims;
  m_bSizeDefined = other.m_bSizeDefined;
  m_vNumBins     = other.m_vNumBins;
  m_vMinValues   = other.m_vMinValues;
  m_vMaxValues   = other.m_vMaxValues;
  m_vRes         = other.m_vRes;
  m_vDimDefined  = other.m_vDimDefined;
  m_nTotalNumBins= other.m_nTotalNumBins;
  m_vlVotes      = other.m_vlVotes;
  m_vBinScores   = other.m_vBinScores;
  m_vBinMeans    = other.m_vBinMeans;

  m_bAllDimensionsDefined = other.m_bAllDimensionsDefined;
}


void VotingSpace::clear()
  /*******************************************************************/
  /* Completely clear this voting space. All size information will   */
  /* be reset to 0, and all bin data will be lost.                   */
  /*******************************************************************/
{
  m_nDims = 0;
  m_bSizeDefined = false;

  m_vNumBins.clear();
  m_vMinValues.clear();
  m_vMaxValues.clear();
  m_vRes.clear();
  m_vDimDefined.clear();
  m_bAllDimensionsDefined = false;

  m_nTotalNumBins = -1;
  for( int i=0; i<(int)m_vlVotes.size(); i++ )
    m_vlVotes[i].clear();
  m_vlVotes.clear();
  m_vBinScores.clear();
  m_vBinMeans.clear();
}


void  VotingSpace::print()
  /*******************************************************************/
  /* Print a text description of this voting space (useful for de-   */
  /* bugging). The content is not printed.                           */
  /*******************************************************************/
{
  cout << "  VotingSpace description:" << endl;
  if ( !m_bSizeDefined )
    cout << "    #Dimensions: not defined!" << endl;

  else {
    cout << "    #Dimensions: " << m_nDims << endl;
    
    /* print information about every dimension */
    for( int i=0; i<m_nDims; i++ )
      if ( !m_vDimDefined[i] )
        cout << "    Dimension " << i << ": not defined!" << endl;
      else {
        cout << "    Dimension " << i << ":" << endl;
        cout << "      #bins     : " << m_vNumBins[i] << endl;
        cout << "      minimum   : " << m_vMinValues[i] << endl;
        cout << "      maximum   : " << m_vMaxValues[i] << endl;
        cout << "      resolution: " << m_vRes[i] << endl;	
      }
  }
  
  cout << "  -----------------------------------" << endl;
}


void  VotingSpace::printContent()
  /*******************************************************************/
  /* Print a text description of the voting space content.           */
  /*******************************************************************/
{
  assert( isValid() );

  cout << "  VotingSpace content:" << endl;
  if( m_nDims > 2 ) {
    cout << "    Cannot display voting spaces with more than 2 dimensions." 
         << endl << endl;
    return;
  }

  if( m_nDims == 1 )
    for( int x=0; x<m_vNumBins[0]; x++ )
      cout << "    Bin " << setw(2) << x << ": " << setw(4) 
           << m_vBinScores[idx(x)] << "(" << m_vlVotes[idx(x)].size() 
           << ")" << endl;
  else
    for( int y=0; y<m_vNumBins[1]; y++ ) {
      cout << "    Row " << setw(2) << y << ": ";
      for( int x=0; x<m_vNumBins[0]; x++ )
        cout << setw(4) << m_vBinScores[idx(x,y)] 
             << "(" << m_vlVotes[idx(x,y)].size() << ")"<< "  ";
      cout << endl;
    }
  
  cout << "  -----------------------------------" << endl;
}


void VotingSpace::initDimension( int dim, int nBins, float min, float max )
  /*******************************************************************/
  /* Initialize the number of bins and bin ranges for one dimension. */
  /*******************************************************************/
{
  assert( m_bSizeDefined );
  assert( dim < m_nDims );

  m_vNumBins[dim]    = nBins;
  m_vMinValues[dim]  = min;
  m_vMaxValues[dim]  = max;
  m_vRes[dim]        = (max - min)/((float) nBins);
  if( m_vRes[dim] == 0.0 )
    m_vRes[dim] = 1.0;
  m_vDimDefined[dim] = true;

  /* invalidate the current bin count */
  m_nTotalNumBins = -1;
}


void VotingSpace::initBins()
  /*******************************************************************/
  /* Initialize all bins to 0. The voting space parameters must be   */
  /* fully defined before this function can be called.               */
  /*******************************************************************/
{
  assert( m_bSizeDefined );

  /* calculate the total number of bins in the voting space */
  int nTotalBins = calcTotalNumBins();

  for( int i=0; i<(int)m_vlVotes.size(); i++ )
    m_vlVotes[i].clear();
  m_vlVotes.clear();
  vector< list<HoughVote> > vlTemp( nTotalBins );
  m_vlVotes = vlTemp;
  //for( int i=0; i<nTotalBins; i++ )
  //  m_vlVotes[i].clear();

  m_vBinScores.clear();
  m_vBinMeans.clear();
  vector<float>         vTemp2( nTotalBins, 0.0 );
  FeatureVector fvTemp( m_nDims );
  vector<FeatureVector> vTemp3( nTotalBins, fvTemp );
  m_vBinScores = vTemp2;
  m_vBinMeans  = vTemp3;
  //for( int i=0; i<nTotalBins; i++ )
  //  m_vBinScores[i] = 0.0;
}


int VotingSpace::calcTotalNumBins()
  /*******************************************************************/
  /* Return the total number of bins in this voting space. Since     */
  /* this number is used quite often, it is calculated once and will */
  /* be saved as part of the voting space's internal information.    */
  /*******************************************************************/
{
  assert( isValid() );

  /* check if the number has already been calculated */
  if ( m_nTotalNumBins <= 0 ) {
    /* only calculate the number if it hasn't been calculated before */
    m_nTotalNumBins = 1;
    for ( int i=0; i<m_nDims; i++ )
      m_nTotalNumBins *= m_vNumBins[i];
  }
  
  return m_nTotalNumBins;  
}


/***********************************************************/
/*                    Access Functions                     */
/***********************************************************/

bool VotingSpace::isValid()
  /*******************************************************************/
  /* Check if the voting space definition is valid, that is if the   */
  /* size has been specified, and all dimensions have been defined.  */
  /* This function is mostly used in assert() statements, but it can */
  /* also be called from external programs.                          */
  /*******************************************************************/
{
  if ( !m_bSizeDefined )
    return false;

  else 
    if ( m_bAllDimensionsDefined )
      return true;
  
    else {
      /* check whether all dimensions have been defined, but the guard */
      /* variable has not been updated yet.                            */
      for ( int i=0; i < m_nDims; i++ )
        if ( !m_vDimDefined[i] )
          return false;
      
      /* if all dimensions have been defined, update the variable. */
      m_bAllDimensionsDefined = true;
      return true;
    }
}


void  VotingSpace::setNumDims  ( int nDims )
  /*******************************************************************/
  /* Set or change the number of dimensions in the voting space.     */
  /* This can be useful, for example, if the voting space has been   */
  /* instantiated with the standard constructor and the size has not */
  /* yet been specified. However, beware that all data will be       */
  /* erased when this function is called!                            */
  /*******************************************************************/
{
  clear();
  m_nDims = nDims;
  m_bSizeDefined = true;

  m_vNumBins.resize(nDims);
  m_vMinValues.resize(nDims);
  m_vMaxValues.resize(nDims);
  m_vRes.resize(nDims);
  m_vDimDefined.resize(nDims);

}


void  VotingSpace::setDimension( int dim, int nBins, float min, float max )
  /*******************************************************************/
  /* Set or change the number of bins and the bin range for one di-  */
  /* mension in the voting space. The number of dimensions must have */
  /* been specified before this function can be called! If the vo-   */
  /* ting space contained data before this function is called, this  */
  /* data will be invalidated!                                       */
  /*******************************************************************/
{
  initDimension( dim, nBins, min, max );

  if( isValid() )
    initBins();
}


/***********************************************************/
/*                  VotingSpace Creation                   */
/***********************************************************/

void VotingSpace::insertVote( const HoughVote &vote )
  /*******************************************************************/
  /* Insert a new hough vote into the voting space. The vote is      */
  /* stored in the appropriate bin location, and the bin score is    */
  /* updated.                                                        */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == vote.getCoords().numDims() );

  FeatureVector fvCoords( vote.getCoords() );

  /* reserve space for the bin indizes */
  vector<int> binidx( m_nDims );

  /* calculate the bin index */
  bool bBorder = false;
  for ( int dim = 0; dim < m_nDims; dim++ ) {
    float value = fvCoords.at(dim);

    if ( value <= m_vMinValues[dim] ) {
      binidx[dim] = 0;
      bBorder = true;
    } else if ( value >= m_vMaxValues[dim] ) {
      binidx[dim] = m_vNumBins[dim] - 1;
      bBorder = true;
    } else
      binidx[dim] = (int) floor( ((value - m_vMinValues[dim]) / m_vRes[dim]) );
    
    /* compensate for round-off errors */
    if( binidx[dim] == m_vNumBins[dim] )
      binidx[dim] = m_vNumBins[dim] - 1;
  }
  
  /* update the voting space */
  int nIdx = idx(binidx);

  m_vlVotes[nIdx].push_back( vote );
  if( !bBorder ) {
    m_vBinScores[nIdx] += vote.getValue();
    m_vBinMeans[nIdx]  += fvCoords*vote.getValue();
    //FeatureVector fvContrib = fvCoords;
    //fvContrib.multFactor( vote.getValue() );
    //m_vBinMeans[nIdx] += fvContrib;
  }
}


int   VotingSpace::getBinNumber( int dim, double value )
  /*******************************************************************/
  /* Return the number of the bin along dimension dim in which this  */
  /* value must be put.                                              */
  /*******************************************************************/
{
  assert( isValid() );
  assert( dim < m_nDims );
  
  int binidx;
  if ( value <= m_vMinValues[dim] )
    binidx = 0;
  else if ( value >= m_vMaxValues[dim] )
    binidx = m_vNumBins[dim] - 1;
  else
    binidx = (int) floor( ((value - m_vMinValues[dim]) / m_vRes[dim]) );

  return binidx;
}


/***********************************************************/
/*                     Maximum Search                      */
/***********************************************************/

void  VotingSpace::setKernelType( int nType )
  /*******************************************************************/
  /* Set the type of the search kernel. Allowed values are:          */
  /*   KERNEL_HCUBE  : A uniform hypercube (default)                 */
  /*   KERNEL_HSPHERE: A uniform hypersphere                         */
  /*******************************************************************/  
{
  switch( nType ) {
  case KERNEL_HCUBE:
  case KERNEL_HSPHERE:
    m_nKernelType = nType;
    break;

  default:
    cerr << "Error in VotingSpace::setKernelType(): "
         << "Unknown kernel type (" << nType << ")!" << endl;
  }
}


void  VotingSpace::setWindowSize ( float wx )
  /*******************************************************************/
  /* Set the window size for a 1D voting space.                      */
  /*******************************************************************/
{
  assert( m_nDims == 1 );

  FeatureVector fvSize( 1 );
  fvSize.setValue( 0, wx );
  m_fvWindowSize = fvSize;
}


void  VotingSpace::setWindowSize ( float wx, float wy )
  /*******************************************************************/
  /* Set the window size for a 2D voting space.                      */
  /*******************************************************************/
{
  assert( m_nDims == 2 );

  FeatureVector fvSize( 2 );
  fvSize.setValue( 0, wx );
  fvSize.setValue( 1, wy );
  m_fvWindowSize = fvSize;
}


void  VotingSpace::setWindowSize ( float wx, float wy, float wz )
  /*******************************************************************/
  /* Set the window size for a 3D voting space.                      */
  /*******************************************************************/
{
  assert( m_nDims == 3 );

  FeatureVector fvSize( 3 );
  fvSize.setValue( 0, wx );
  fvSize.setValue( 1, wy );
  fvSize.setValue( 2, wz );
  m_fvWindowSize = fvSize;
}


void  VotingSpace::setWindowSize ( float wx, float wy, float wz, float ws )
  /*******************************************************************/
  /* Set the window size for a 4D voting space.                      */
  /*******************************************************************/
{
  assert( m_nDims == 4 );

  FeatureVector fvSize( 4 );
  fvSize.setValue( 0, wx );
  fvSize.setValue( 1, wy );
  fvSize.setValue( 2, wz );
  fvSize.setValue( 3, ws );
  m_fvWindowSize = fvSize;
}


void  VotingSpace::setWindowSize ( const FeatureVector &fvWindowSize )
  /*******************************************************************/
  /* Set the window size for a voting space of arbitrary dimension.  */
  /*******************************************************************/
{
  assert( m_nDims == fvWindowSize.numDims() );

  m_fvWindowSize = fvWindowSize;
}


float VotingSpace::getBinVoteSum ( int idx_x )
  /*******************************************************************/
  /* Get the bin vote sum for a 1D voting space.                     */
  /*******************************************************************/
{
  assert( m_nDims == 1 );

  return m_vBinScores[idx(idx_x)];
}


float VotingSpace::getBinVoteSum ( int idx_x, int idx_y )
  /*******************************************************************/
  /* Get the bin vote sum for a 2D voting space.                     */
  /*******************************************************************/
{
  assert( m_nDims == 2 );

  return m_vBinScores[idx(idx_x,idx_y)];
}


float VotingSpace::getBinVoteSum ( int idx_x, int idx_y, int idx_z )
  /*******************************************************************/
  /* Get the bin vote sum for a 3D voting space.                     */
  /*******************************************************************/
{
  assert( m_nDims == 3 );

  return m_vBinScores[idx(idx_x,idx_y,idx_z)];
}


float VotingSpace::getBinVoteSum ( int idx_x, int idx_y, int idx_z, int idx_s )
  /*******************************************************************/
  /* Get the bin vote sum for a 4D voting space.                     */
  /*******************************************************************/
{
  assert( m_nDims == 4 );

  return m_vBinScores[idx(idx_x,idx_y,idx_z,idx_s)];
}


float VotingSpace::getBinVoteSum ( vector<int> vBinIdx )
  /*******************************************************************/
  /* Get the bin vote sum for a voting space of arbitrary dimension. */
  /*******************************************************************/
{
  assert( m_nDims == (int)vBinIdx.size() );

  return m_vBinScores[idx(vBinIdx)];
}


float VotingSpace::getVoteSum    ( float x )
  /*******************************************************************/
  /* Get the sum of all votes in a window around the given position. */
  /* Version for a 1D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 1 );
  fvCoords.setValue( 0, x );
  return getVoteSum( fvCoords );
}


float VotingSpace::getVoteSum    ( float x, float y )
  /*******************************************************************/
  /* Get the sum of all votes in a window around the given position. */
  /* Version for a 2D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 2 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  return getVoteSum( fvCoords );
}


float VotingSpace::getVoteSum    ( float x, float y, float z )
  /*******************************************************************/
  /* Get the sum of all votes in a window around the given position. */
  /* Version for a 3D voting space.                                  */
  /*******************************************************************/
{
  /* fast version optimized for 3D voting spaces */
  FeatureVector fvCoords( 3 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  fvCoords.setValue( 2, z );
  
  /* calculate the bin index */
  vector<int> vBinIdx( m_nDims );
  for ( int dim = 0; dim < m_nDims; dim++ )
    vBinIdx[dim] = getBinNumber( dim, fvCoords.at(dim) );

  /* get the indizes of all adjacent bins within the window */
  vector<int> vRange( m_nDims );
  vector<int> vMin( m_nDims );
  vector<int> vMax( m_nDims );
  for( int i=0; i<m_nDims; i++ ) {
    int nRange = (int) ceil(m_fvWindowSize.at(i) / m_vRes[i] );
    vMin[i] = max(0,vBinIdx[i]-nRange);
    vMax[i] = min(m_vNumBins[i]-1,vBinIdx[i]+nRange);
  }
  
  /* add the scores for all adjacent bins */
  float dScore = 0.0;
  for( int xx=vMin[0]; xx<vMax[0]; xx++ ) {
    vBinIdx[0] = xx;
    for( int yy=vMin[1]; yy<vMax[1]; yy++ ) {
      vBinIdx[1] = yy;
      for( int zz=vMin[2]; zz<vMax[2]; zz++ ) {
        vBinIdx[2] = zz;
        dScore += getVoteSumInWindow( vBinIdx, fvCoords );
      }
    }
  }

  return dScore;
}


float VotingSpace::getVoteSum    ( float x, float y, float z, float s )
  /*******************************************************************/
  /* Get the sum of all votes in a window around the given position. */
  /* Version for a 4D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 4 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  fvCoords.setValue( 2, z );
  fvCoords.setValue( 3, s );
  return getVoteSum( fvCoords );
}


float VotingSpace::getVoteSum    ( const FeatureVector &fvCoords )
  /*******************************************************************/
  /* Get the sum of all votes in a window around the given position. */
  /* Version for a voting space of arbitrary dimension.              */
  /*******************************************************************/
{
  /* reserve space for the bin indizes */
  vector<int> vBinIdx( m_nDims );

  /* calculate the bin index */
  for ( int dim = 0; dim < m_nDims; dim++ )
    vBinIdx[dim] = getBinNumber( dim, fvCoords.at(dim) );

  /* compute the score for the main bin */
  float dScore = 0.0;
  //dScore += getVoteSumInWindow( vBinIdx, fvCoords );

  /*prepare the indizes for all adjacent bins within the window size */
  vector<int> vRange( m_nDims );
  vector<int> vMin( m_nDims );
  vector<int> vMax( m_nDims );
  for( int i=0; i<m_nDims; i++ ) {
    int nRange = (int) ceil(m_fvWindowSize.at(i) / m_vRes[i] );
    vMin[i] = max(0,vBinIdx[i]-nRange);
    vMax[i] = min(m_vNumBins[i]-1,vBinIdx[i]+nRange);
  }

  /* initialize the running index */
  vector<int> vNewIdx( m_nDims );
  for( int i=0; i<m_nDims; i++ )
    vNewIdx[i] = vMin[i];
  
  /* add the scores for all adjacent bins */
  while( true ){
    dScore += getVoteSumInWindow( vNewIdx, fvCoords );
    
    /* increment the index */
    bool bStop = false;
    for( int i=0; i<m_nDims && !bStop; i++ ) {
      vNewIdx[i]++;
      if( vNewIdx[i] <= vMax[i] )
        bStop = true;
      else
        vNewIdx[i] = vMin[i];
    } 

    /* exit if all relevant bins have been processed */
    if( !bStop )
      break;
  }

  return dScore;
}


void VotingSpace::getMeanVote( float x, 
                               FeatureVector &fvMean, int &nNumVotes, 
                               float &dSumScore )
  /*******************************************************************/
  /* Get the sum and mean of all votes in a window around the given  */
  /* position.                                                       */
  /* Version for a 1D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 1 );
  fvCoords.setValue( 0, x );
  getMeanVote( fvCoords, fvMean, nNumVotes, dSumScore );
}


void VotingSpace::getMeanVote( float x, float y,
                               FeatureVector &fvMean, int &nNumVotes, 
                               float &dSumScore )
  /*******************************************************************/
  /* Get the sum and mean of all votes in a window around the given  */
  /* position.                                                       */
  /* Version for a 2D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 2 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  getMeanVote( fvCoords, fvMean, nNumVotes, dSumScore );
}


void VotingSpace::getMeanVote( float x, float y, float z,
                               FeatureVector &fvMean, int &nNumVotes, 
                               float &dSumScore )
  /*******************************************************************/
  /* Get the sum and mean of all votes in a window around the given  */
  /* position.                                                       */
  /* Version for a 3D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 3 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  fvCoords.setValue( 2, z );
  getMeanVote( fvCoords, fvMean, nNumVotes, dSumScore );
}


void VotingSpace::getMeanVote( float x, float y, float z, float s,
                               FeatureVector &fvMean, int &nNumVotes, 
                               float &dSumScore )
  /*******************************************************************/
  /* Get the sum and mean of all votes in a window around the given  */
  /* position.                                                       */
  /* Version for a 4D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 4 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  fvCoords.setValue( 2, z );
  fvCoords.setValue( 3, s );
  getMeanVote( fvCoords, fvMean, nNumVotes, dSumScore );
}


void VotingSpace::getMeanVote( const FeatureVector &fvCoords,
                               FeatureVector &fvMean, int &nNumVotes, 
                               float &dSumScore )
  /*******************************************************************/
  /* Get the sum and mean of all votes in a window around the given  */
  /* position.                                                       */
  /* Version for a voting space of arbitrary dimension.              */
  /*******************************************************************/
{
  /* reserve space for the bin indizes */
  vector<int> vBinIdx( m_nDims );

  /* calculate the bin index */
  for ( int dim = 0; dim < m_nDims; dim++ )
    vBinIdx[dim] = getBinNumber( dim, fvCoords.at(dim) );

  /* initialize the variables */
  FeatureVector tmp( m_nDims );
  fvMean = tmp;
  nNumVotes = 0;
  dSumScore = 0.0;

  /*prepare the indizes for all adjacent bins within the window size */
  vector<int> vRange( m_nDims );
  vector<int> vMin( m_nDims );
  vector<int> vMax( m_nDims );
  for( int i=0; i<m_nDims; i++ ) {
    int nRange = (int) ceil(m_fvWindowSize.at(i) / m_vRes[i] );
    vMin[i] = max(0,vBinIdx[i]-nRange);
    vMax[i] = min(m_vNumBins[i]-1,vBinIdx[i]+nRange);
  }

  /* initialize the running index */
  vector<int> vNewIdx( m_nDims );
  for( int i=0; i<m_nDims; i++ )
    vNewIdx[i] = vMin[i];
  
  /* add the scores for all adjacent bins */
  while( true ){
    /* get sum and mean of votes in next bin */
    FeatureVector fvBinMean;
    int   nBinNumVotes;
    float dBinSumScore;
    getMeanVoteInWindow( vNewIdx, fvCoords, 
                         fvBinMean, nBinNumVotes, dBinSumScore );

    /* update the global mean using the new information */
    if( nBinNumVotes > 0 ) {
      // mean = ((score_old * mean_old) + (score_new * mean_new))/(sum_score)
      FeatureVector fvMeanNorm    = fvMean;
      fvMeanNorm.multFactor( dSumScore );       //nNumVotes );

      FeatureVector fvBinMeanNorm = fvBinMean;
      fvBinMeanNorm.multFactor( dBinSumScore ); //nBinNumVotes );

      FeatureVector fvNewMean = fvMeanNorm;
      fvNewMean.addVector( fvBinMeanNorm );
      fvNewMean.multFactor( 1.0/(dSumScore+dBinSumScore) ); //(float)(nNumVotes+nBinNumVotes) );
      fvMean = fvNewMean;
    }
    nNumVotes += nBinNumVotes;
    dSumScore += dBinSumScore;

    /* increment the index */
    bool bStop = false;
    for( int i=0; i<m_nDims && !bStop; i++ ) {
      vNewIdx[i]++;
      if( vNewIdx[i] <= vMax[i] )
        bStop = true;
      else
        vNewIdx[i] = vMin[i];
    } 

    /* exit if all relevant bins have been processed */
    if( !bStop )
      break;
  }
}
  

void VotingSpace::getFastKernelVoteSum( int x,
                                        float &dSumScore )
  /*******************************************************************/
  /* Get the sum and mean of all votes in a window around the given  */
  /* position.                                                       */
  /* (fast approximate computation using a binned accumulator array).*/
  /* Version for a 1D voting space.                                  */
  /*******************************************************************/
{
  vector<int> vBinIdx(1);
  vBinIdx[0] = x;
  getFastKernelVoteSum( vBinIdx, dSumScore );
}


void VotingSpace::getFastKernelVoteSum( int x, int y,
                                        float &dSumScore )
  /*******************************************************************/
  /* Get the sum and mean of all votes in a window around the given  */
  /* position.                                                       */
  /* (fast approximate computation using a binned accumulator array).*/
  /* Version for a 2D voting space.                                  */
  /*******************************************************************/
{
  vector<int> vBinIdx(2);
  vBinIdx[0] = x;
  vBinIdx[1] = y;
  getFastKernelVoteSum( vBinIdx, dSumScore );
}


void VotingSpace::getFastKernelVoteSum( int x, int y, int z,
                                        float &dSumScore )
  /*******************************************************************/
  /* Get the sum and mean of all votes in a window around the given  */
  /* position.                                                       */
  /* (fast approximate computation using a binned accumulator array).*/
  /* Specially optimized version for a 3D voting space.              */
  /*******************************************************************/
{
  assert( m_nDims==3 );

  vector<int> vBinIdx(3);
  vBinIdx[0] = x;
  vBinIdx[1] = y;
  vBinIdx[2] = z;
//   getFastKernelVoteSum( vBinIdx, dSumScore );

  /*==========================*/
  /* Initialize the variables */
  /*==========================*/
  dSumScore = 0.0;

  /* get the kernel center */
  float cx    = m_vMinValues[0] + m_vRes[0]*(x+0.5);
  float cy    = m_vMinValues[1] + m_vRes[1]*(y+0.5);
  float cz    = m_vMinValues[2] + m_vRes[2]*(z+0.5);

  /* get the kernel bbox coordinates */
  float minx2 = cx - m_fvWindowSize.at(0);
  float maxx2 = cx + m_fvWindowSize.at(0);
  float miny2 = cy - m_fvWindowSize.at(1);
  float maxy2 = cy + m_fvWindowSize.at(1);
  float minz2 = cz - m_fvWindowSize.at(2);
  float maxz2 = cz + m_fvWindowSize.at(2);

  /* precompute the bin volume */
  float dBinVol = m_vRes[0]*m_vRes[1]*m_vRes[2];

  /*prepare the indizes for all adjacent bins within the window size */
  vector<int> vRange( m_nDims );
  vector<int> vMin( m_nDims );
  vector<int> vMax( m_nDims );
  for( int i=0; i<m_nDims; i++ ) {
    int nRange = (int) ceil(m_fvWindowSize.at(i) / m_vRes[i] );
    vMin[i] = max(0,vBinIdx[i]-nRange);
    vMax[i] = min(m_vNumBins[i]-1,vBinIdx[i]+nRange);
  }

  /*============================================*/
  /* Accumulate the fast approximate kernel sum */
  /*============================================*/
  for(int zz=vMin[2]; zz<vMax[2]; zz++ )
    for(int yy=vMin[1]; yy<vMax[1]; yy++ )
      for(int xx=vMin[0]; xx<vMax[0]; xx++ ) {
        int   nNewIdx = idx(xx,yy,zz);
        float dBinSumScore = m_vBinScores[nNewIdx];
  
        /*------------------------------------*/
        /* Compute the volume of intersection */
        /*------------------------------------*/
        /* get the bin coordinates */
        float minx1 = m_vMinValues[0] + m_vRes[0]* x;
        float maxx1 = m_vMinValues[0] + m_vRes[0]*(x+1);
        float miny1 = m_vMinValues[1] + m_vRes[1]* y;
        float maxy1 = m_vMinValues[1] + m_vRes[1]*(y+1);
        float minz1 = m_vMinValues[2] + m_vRes[2]* z;
        float maxz1 = m_vMinValues[2] + m_vRes[2]*(z+1);

        /* find the intersection coordinates */
        float minx = max( minx1, minx2);
        float maxx = min( maxx1, maxx2);
        float miny = max( miny1, miny2);
        float maxy = min( maxy1, maxy2);
        float minz = max( minz1, minz2);
        float maxz = min( maxz1, maxz2);
        
        /* compute the intersection volume */
        float dx = maxx - minx;
        float dy = maxy - miny;
        float dz = maxz - minz;

        float dPercInter = 0.0;
        if( (dx>0.0) && (dy>0.0) && (dz>0.0) )
          dPercInter = (dx*dy*dz)/dBinVol;

        /*-----------------------------------------------------*/
        /* Weight the bin score by the intersecting percentage */
        /*-----------------------------------------------------*/
        dSumScore += dBinSumScore*dPercInter;
      }
}


void VotingSpace::getFastKernelVoteSum( int x, int y, int z, int s,
                                        float &dSumScore )
  /*******************************************************************/
  /* Get the sum and mean of all votes in a window around the given  */
  /* position.                                                       */
  /* (fast approximate computation using a binned accumulator array).*/
  /* Version for a 4D voting space.                                  */
  /*******************************************************************/
{
  vector<int> vBinIdx(4);
  vBinIdx[0] = x;
  vBinIdx[1] = y;
  vBinIdx[2] = z;
  vBinIdx[3] = s;
  getFastKernelVoteSum( vBinIdx, dSumScore );
}


void VotingSpace::getFastKernelVoteSum( const vector<int> &vBinIdx,
                                        float &dSumScore )
  /*******************************************************************/
  /* Get the sum and mean of all votes in a window around the given  */
  /* position.                                                       */
  /* (fast approximate computation using a binned accumulator array).*/
  /* Version for a voting space of arbitrary dimension.              */
  /*******************************************************************/
{
  /* initialize the variables */
  FeatureVector tmp( m_nDims );
  dSumScore = 0.0;

  /*prepare the indizes for all adjacent bins within the window size */
  vector<int> vRange( m_nDims );
  vector<int> vMin( m_nDims );
  vector<int> vMax( m_nDims );
  for( int i=0; i<m_nDims; i++ ) {
    int nRange = (int) ceil(m_fvWindowSize.at(i) / m_vRes[i] );
    vMin[i] = max(0,vBinIdx[i]-nRange);
    vMax[i] = min(m_vNumBins[i]-1,vBinIdx[i]+nRange);
  }

  /* initialize the running index */
  vector<int> vNewIdx( m_nDims );
  for( int i=0; i<m_nDims; i++ )
    vNewIdx[i] = vMin[i];
  
  /* add the scores for all adjacent bins */
  while( true ){
    /* get sum and mean of votes in next bin */
    int   nNewIdx = idx(vNewIdx);
    float dBinSumScore = m_vBinScores[nNewIdx];

    dSumScore += dBinSumScore;

    /* increment the index */
    bool bStop = false;
    for( int i=0; i<m_nDims && !bStop; i++ ) {
      vNewIdx[i]++;
      if( vNewIdx[i] <= vMax[i] )
        bStop = true;
      else
        vNewIdx[i] = vMin[i];
    } 

    /* exit if all relevant bins have been processed */
    if( !bStop )
      break;
  }
}


void VotingSpace::getFastMeanVote( const FeatureVector &fvCoords,
                                   FeatureVector &fvMean, int &nNumVotes,
                                   float &dSumScore )
  /*******************************************************************/
  /* Get the sum and mean of all votes in a window around the given  */
  /* position.                                                       */
  /* (fast approximate computation using a binned accumulator array).*/
  /* Version for a voting space of arbitrary dimension.              */
  /*******************************************************************/
{
  /* reserve space for the bin indizes */
  vector<int> vBinIdx( m_nDims );

  /* calculate the bin index */
  for ( int dim = 0; dim < m_nDims; dim++ )
    vBinIdx[dim] = getBinNumber( dim, fvCoords.at(dim) );

  /* call the fast approximate kernel vote sum function */
  getFastMeanVote   ( vBinIdx, fvMean, nNumVotes, dSumScore );
}


void VotingSpace::getFastMeanVote( const vector<int> &vBinIdx,
                                   FeatureVector &fvMean, int &nNumVotes,
                                   float &dSumScore )
  /*******************************************************************/
  /* Get the mean of all votes in a window around the given          */
  /* position.                                                       */
  /* (fast approximate computation using a binned accumulator array).*/
  /* Version for a voting space of arbitrary dimension.              */
  /*******************************************************************/
{
  /* initialize the variables */
  FeatureVector tmp( m_nDims );
  fvMean = tmp;
  nNumVotes = 0;
  dSumScore = 0.0;

  /*prepare the indizes for all adjacent bins within the window size */
  vector<int> vRange( m_nDims );
  vector<int> vMin( m_nDims );
  vector<int> vMax( m_nDims );
  for( int i=0; i<m_nDims; i++ ) {
    int nRange = (int) ceil(m_fvWindowSize.at(i) / m_vRes[i] );
    vMin[i] = max(0,vBinIdx[i]-nRange);
    vMax[i] = min(m_vNumBins[i]-1,vBinIdx[i]+nRange);
  }

  /* initialize the running index */
  vector<int> vNewIdx( m_nDims );
  for( int i=0; i<m_nDims; i++ )
    vNewIdx[i] = vMin[i];
  
  /* add the scores for all adjacent bins */
  while( true ){
    /* get sum and mean of votes in next bin */
    int nNewIdx = idx(vNewIdx);
    FeatureVector fvBinMean    = m_vBinMeans[nNewIdx];
    int           nBinNumVotes = (int)m_vlVotes[nNewIdx].size();
    float         dBinSumScore = m_vBinScores[nNewIdx];

    /* update the global mean using the new information */
    if( nBinNumVotes > 0 ) {
      // mean = ((score_old * mean_old) + (score_new * mean_new))/(sum_score)
      FeatureVector fvMeanNorm    = fvMean;
      fvMeanNorm.multFactor( dSumScore );       //nNumVotes );

      FeatureVector fvBinMeanNorm = fvBinMean;
      //fvBinMeanNorm.multFactor( dBinSumScore ); //nBinNumVotes );

      FeatureVector fvNewMean = fvMeanNorm;
      fvNewMean.addVector( fvBinMeanNorm );
      fvNewMean.multFactor( 1.0/(dSumScore+dBinSumScore) ); //(float)(nNumVotes+nBinNumVotes) );
      fvMean = fvNewMean;
    }
    nNumVotes += nBinNumVotes;
    dSumScore += dBinSumScore;

    /* increment the index */
    bool bStop = false;
    for( int i=0; i<m_nDims && !bStop; i++ ) {
      vNewIdx[i]++;
      if( vNewIdx[i] <= vMax[i] )
        bStop = true;
      else
        vNewIdx[i] = vMin[i];
    } 

    /* exit if all relevant bins have been processed */
    if( !bStop )
      break;
  }
}


list<HoughVote> VotingSpace::getSupportingVotes( float x )
  /*******************************************************************/
  /* Get all votes in a window around the given position.            */
  /* Version for a 1D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 1 );
  fvCoords.setValue( 0, x );
  return getSupportingVotes( fvCoords );
}


list<HoughVote> VotingSpace::getSupportingVotes( float x, float y )
  /*******************************************************************/
  /* Get all votes in a window around the given position.            */
  /* Version for a 2D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 2 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  return getSupportingVotes( fvCoords );
}


list<HoughVote> VotingSpace::getSupportingVotes( float x, float y, float z )
  /*******************************************************************/
  /* Get all votes in a window around the given position.            */
  /* Version for a 3D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 3 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  fvCoords.setValue( 2, z );
  return getSupportingVotes( fvCoords );
}


list<HoughVote> VotingSpace::getSupportingVotes( float x, float y, float z,
                                                   float s )
  /*******************************************************************/
  /* Get all votes in a window around the given position.            */
  /* Version for a 4D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 4 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  fvCoords.setValue( 2, z );
  fvCoords.setValue( 3, s );
  return getSupportingVotes( fvCoords );
}


list<HoughVote> VotingSpace::getSupportingVotes( const FeatureVector &fvCoords )
  /*******************************************************************/
  /* Get all votes in a window around the given position.            */
  /* Version for a voting space of arbitrary dimension.              */
  /*******************************************************************/
{
  assert( fvCoords.numDims() == m_nDims );

  list<HoughVote> vResult;

  /* reserve space for the bin indizes */
  vector<int> vBinIdx( m_nDims );

  /* calculate the bin index */
  for ( int dim = 0; dim < m_nDims; dim++ )
    vBinIdx[dim] = getBinNumber( dim, fvCoords.at(dim) );

  /*prepare the indizes for all adjacent bins within the window size */
  vector<int> vRange( m_nDims );
  vector<int> vMin( m_nDims );
  vector<int> vMax( m_nDims );
  for( int i=0; i<m_nDims; i++ ) {
    int nRange = (int) ceil(m_fvWindowSize.at(i) / m_vRes[i] );
    vMin[i] = max(0,vBinIdx[i]-nRange);
    vMax[i] = min(m_vNumBins[i]-1,vBinIdx[i]+nRange);
  }

  /* initialize the running index */
  vector<int> vNewIdx( m_nDims );
  for( int i=0; i<m_nDims; i++ )
    vNewIdx[i] = vMin[i];
  
  /* add the scores for all adjacent bins */
  while( true ){
    list<HoughVote> vBinVotes = getVotesInWindow( vNewIdx, fvCoords );
    
    /* update the result vector */
    vResult.insert( vResult.end(), vBinVotes.begin(), vBinVotes.end() );

    /* increment the index */
    bool bStop = false;
    for( int i=0; i<m_nDims && !bStop; i++ ) {
      vNewIdx[i]++;
      if( vNewIdx[i] <= vMax[i] )
        bStop = true;
      else
        vNewIdx[i] = vMin[i];
    } 

    /* exit if all relevant bins have been processed */
    if( !bStop )
      break;
  }

  return vResult;
}


void VotingSpace::applyMSME( float x, 
                             FeatureVector &fvResult, float &dScore,
                             bool bUseFastMSME )
  /*******************************************************************/
  /* Search for a local maximum in the Hough votes using a Mean-     */
  /* Shift Mode Estimator.                                           */
  /* Version for a 1D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 1 );
  fvCoords.setValue( 0, x );
  applyMSME( fvCoords, fvResult, dScore, bUseFastMSME );
}


void VotingSpace::applyMSME( float x, float y,
                             FeatureVector &fvResult, float &dScore,
                             bool bUseFastMSME )
  /*******************************************************************/
  /* Search for a local maximum in the Hough votes using a Mean-     */
  /* Shift Mode Estimator.                                           */
  /* Version for a 2D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 2 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  applyMSME( fvCoords, fvResult, dScore, bUseFastMSME );
}


void VotingSpace::applyMSME( float x, float y, float z,
                             FeatureVector &fvResult, float &dScore,
                             bool bAdaptiveScale, float dAdaptMinScale,
                             bool bUseFastMSME )
  /*******************************************************************/
  /* Search for a local maximum in the Hough votes using a Mean-     */
  /* Shift Mode Estimator.                                           */
  /* Version for a 3D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 3 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  fvCoords.setValue( 2, z );
  applyMSME( fvCoords, fvResult, dScore, bAdaptiveScale, dAdaptMinScale,
             bUseFastMSME );
}


void VotingSpace::applyMSME( float x, float y, float z, float s,
                             FeatureVector &fvResult, float &dScore,
                             bool bAdaptiveScale, float dAdaptMinScale,
                             bool bUseFastMSME )
  /*******************************************************************/
  /* Search for a local maximum in the Hough votes using a Mean-     */
  /* Shift Mode Estimator.                                           */
  /* Version for a 4D voting space.                                  */
  /*******************************************************************/
{
  FeatureVector fvCoords( 4 );
  fvCoords.setValue( 0, x );
  fvCoords.setValue( 1, y );
  fvCoords.setValue( 2, z );
  fvCoords.setValue( 3, s );
  applyMSME( fvCoords, fvResult, dScore, bAdaptiveScale, dAdaptMinScale,
             bUseFastMSME );
}


void VotingSpace::applyMSME( const FeatureVector &fvCoords, 
                             FeatureVector &fvResult, float &dScore,
                             bool bAdaptiveScale, float dAdaptMinScale,
                             bool bUseFastMSME )
  /*******************************************************************/
  /* Search for a local maximum in the Hough votes using a Mean-     */
  /* Shift Mode Estimator.                                           */
  /* Version for a voting space of arbitrary dimension.              */
  /*******************************************************************/
{
  float dLastScore = 0.0;
  FeatureVector fvCurPos = fvCoords;
  FeatureVector fvNewMean( fvCurPos.numDims() );

  /* save the original window size */
  FeatureVector fvWindowSize = m_fvWindowSize;
  
  dScore = 2*EPS_MSME;
  //int   nIterations = 0;
  float dAdaptMinScale2 = dAdaptMinScale*dAdaptMinScale;
  bool  bStop = false;
  while( fabs(dScore - dLastScore) > EPS_MSME && !bStop ) {
    dLastScore = dScore;

    float dNorm = 1.0;
    if( bAdaptiveScale )
      if( fvCurPos.at(2) > dAdaptMinScale ) {
        /* adapt the window size to the current scale */
        m_fvWindowSize.setValue(0, fvWindowSize.at(0)*fvCurPos.at(2) );
        m_fvWindowSize.setValue(1, fvWindowSize.at(1)*fvCurPos.at(2) );
        m_fvWindowSize.setValue(2, fvWindowSize.at(2)*fvCurPos.at(2) );
        dNorm = fvCurPos.at(2)*fvCurPos.at(2);

      } else {
        /* adapt the window size to minimum current scale */
        m_fvWindowSize.setValue(0, fvWindowSize.at(0)*dAdaptMinScale );
        m_fvWindowSize.setValue(1, fvWindowSize.at(1)*dAdaptMinScale ); 
        m_fvWindowSize.setValue(2, fvWindowSize.at(2)*dAdaptMinScale ); 
        dNorm = dAdaptMinScale2;
      }

    /* collect all Hough Votes inside the current window */  
    dScore = 0.0;
    int nCount = 0;
    if( !bUseFastMSME )
      getMeanVote( fvCurPos, fvNewMean, nCount, dScore );
    else {
      getFastMeanVote( fvCurPos, fvNewMean, nCount, dScore );
      bStop = true;
    }

    /* normalize by the changed kernel volume */
    if( bAdaptiveScale )
      dScore /= dAdaptMinScale;

    /* update the current position to the new mean */
    if( dScore > dLastScore )
      fvCurPos = fvNewMean;
  }

  /* restore the original window size */
  m_fvWindowSize = fvWindowSize;

  fvResult = fvCurPos;
}


list<HoughVote> VotingSpace::getBinVotes( int idx_x )
  /*******************************************************************/
  /* Get all votes in the given bin.                                 */
  /* Version for a 1D voting space.                                  */
  /*******************************************************************/
{
  return m_vlVotes[idx(idx_x)];
}


list<HoughVote> VotingSpace::getBinVotes( int idx_x, int idx_y )
  /*******************************************************************/
  /* Get all votes in the given bin.                                 */
  /* Version for a 2D voting space.                                  */
  /*******************************************************************/
{
  return m_vlVotes[idx(idx_x,idx_y)];
}


list<HoughVote> VotingSpace::getBinVotes( int idx_x, int idx_y, int idx_z )
  /*******************************************************************/
  /* Get all votes in the given bin.                                 */
  /* Version for a 3D voting space.                                  */
  /*******************************************************************/
{
  return m_vlVotes[idx(idx_x,idx_y,idx_z)];
}


list<HoughVote> VotingSpace::getBinVotes( int idx_x, int idx_y, int idx_z, 
                                            int idx_s )
  /*******************************************************************/
  /* Get all votes in the given bin.                                 */
  /* Version for a 4D voting space.                                  */
  /*******************************************************************/
{
  return m_vlVotes[idx(idx_x,idx_y,idx_z,idx_s)];
}


list<HoughVote> VotingSpace::getBinVotes( const vector<int> &vBinIdx )
  /*******************************************************************/
  /* Get all votes in the given bin.                                 */
  /* Version for a voting space of arbitrary dimension.              */
  /*******************************************************************/
{
  return m_vlVotes[idx(vBinIdx)];
}


/***********************************************************/
/*                    Service Functions                    */
/***********************************************************/

float VotingSpace::getVoteSumInWindow( const vector<int> &vBinIdx, 
                                       const FeatureVector &fvStart )
{
  float dResult = 0.0;

  int nIdx = idx(vBinIdx);
//   for( int i=0; i<(int)m_vlVotes[nIdx].size(); i++ ) {
//     if( isInsideKernel( m_vlVotes[nIdx][i].getCoords(), fvStart ) )
  for( list<HoughVote>::iterator it=m_vlVotes[nIdx].begin();
       it!=m_vlVotes[nIdx].end(); it++ ) {
    if( isInsideKernel( it->getCoords(), fvStart ) )
      /* add the current vote to result score */
      dResult += it->getValue();
  }

  return dResult;
}


void VotingSpace::getMeanVoteInWindow( const vector<int> &vBinIdx, 
                                       const FeatureVector &fvStart,
                                       FeatureVector &fvMean, int &nNumVotes,
                                       float &dSumScore )
{
  FeatureVector tmp( m_nDims );
  fvMean    = tmp;
  nNumVotes = 0;
  dSumScore = 0.0;

  int nIdx = idx(vBinIdx);
//   for( int i=0; i<(int)m_vlVotes[nIdx].size(); i++ )
//    if( isInsideKernel( m_vlVotes[nIdx][i].getCoords(), fvStart ) ) {
  for( list<HoughVote>::iterator it=m_vlVotes[nIdx].begin();
       it!=m_vlVotes[nIdx].end(); it++ )
    if( isInsideKernel( it->getCoords(), fvStart ) ) {
      /* add the current vote to result score */
      //HoughVote &vote = m_vlVotes[nIdx][i];
      HoughVote &vote = *it;
      FeatureVector fvWeightedCoords = vote.getCoords();
      fvWeightedCoords.multFactor( vote.getValue() );
      fvMean.addVector( fvWeightedCoords );
      dSumScore += vote.getValue();
      nNumVotes++;
    }
  
  if( dSumScore > 0.0 )
    fvMean.multFactor( 1.0/dSumScore );
}


list<HoughVote> VotingSpace::getVotesInWindow( const vector<int> &vBinIdx, 
                                               const FeatureVector &fvStart )
{
  list<HoughVote> vResult;

  int nIdx = idx(vBinIdx);
//   for( int i=0; i<(int)m_vlVotes[nIdx].size(); i++ ) {
//     if( isInsideKernel( m_vlVotes[nIdx][i].getCoords(), fvStart ) )
  for( list<HoughVote>::iterator it=m_vlVotes[nIdx].begin();
       it!=m_vlVotes[nIdx].end(); it++ )
    if( isInsideKernel( it->getCoords(), fvStart ) ) {
      /* add the current vote to result score */
      //vResult.push_back( m_vlVotes[nIdx][i] );
      vResult.push_back( *it );
  }

  return vResult;
}


/***********************************************************/
/*                    Service Functions                    */
/***********************************************************/

bool  VotingSpace::isInsideKernel( const FeatureVector &fvCoords,
                                   const FeatureVector &fvCenter )
{
  bool bInside = true;

  switch( m_nKernelType ) {
  case KERNEL_HCUBE: {
    for( int k=0; (k<m_nDims) && bInside; k++ )
      if( fabs(fvCoords.at(k) - fvCenter.at(k)) > m_fvWindowSize.at(k) )
        bInside = false;
    break;
  }

  case KERNEL_HSPHERE: {
    float dDist = 0.0;
    for( int k=0; k<m_nDims; k++ ) {
      float d = (fvCoords.at(k) - fvCenter.at(k)) / m_fvWindowSize.at(k);
      dDist += d*d;
    }
    bInside = (dDist <= 1.0 );
    break;
  }

  default:
    cerr << "Error in VotingSpace::isInsideKernel(): "
         << "Unknown kernel type (" << m_nKernelType << ")!" << endl;
    return false;
  }
    
  
  return bInside;
}


int VotingSpace::idx( int x )
  /*******************************************************************/
  /* Calculate the cell index for a 1D voting space.                 */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == 1 );
  assert( x < m_vNumBins[0] );

  return x;
}


int VotingSpace::idx( int x, int y )
  /*******************************************************************/
  /* Calculate the cell index for a 2D voting space.                 */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == 2 );
  assert( x < m_vNumBins[0] );
  assert( y < m_vNumBins[1] );

  return y*m_vNumBins[0] + x;
}


int VotingSpace::idx( int x, int y, int z )
  /*******************************************************************/
  /* Calculate the cell index for a 3D voting space.                 */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == 3 );
  assert( x < m_vNumBins[0] );
  assert( y < m_vNumBins[1] );
  assert( z < m_vNumBins[2] );

  return z*m_vNumBins[1]*m_vNumBins[0] + y*m_vNumBins[0] + x;
}


int VotingSpace::idx( int x, int y, int z, int s )
  /*******************************************************************/
  /* Calculate the cell index for a 4D voting space.                 */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == 4 );
  assert( x < m_vNumBins[0] );
  assert( y < m_vNumBins[1] );
  assert( z < m_vNumBins[2] );
  assert( s < m_vNumBins[3] );

  return ( s*m_vNumBins[2]*m_vNumBins[1]*m_vNumBins[0] + 
           z*m_vNumBins[1]*m_vNumBins[0] + y*m_vNumBins[0] + x );
}


int VotingSpace::idx( vector<int> index )
  /*******************************************************************/
  /* Calculate the cell index for a voting space of arbitrary dimen- */
  /* sion.                                                           */
  /*******************************************************************/
{
  assert( isValid() );
  assert( m_nDims == (int)index.size() );
  
  int nCurrentIdx = 0;
  int nMultFactor = 1;
  for ( int i = 0; i < (int)index.size(); i++ ) {
    assert( index[i] < m_vNumBins[i] );

    nCurrentIdx += index[i]*nMultFactor;
    nMultFactor *= m_vNumBins[i];
  }

  return nCurrentIdx;
}


vector<int> VotingSpace::getNextIndex( const vector<int> &index, 
                                       bool &endReached )
  /*******************************************************************/
  /* Given a voting space index, create the index of the following   */
  /* cell. This can be used for iterating through all voting space   */
  /* cells. When the end is reached, the endReached flag will be re- */
  /* turned as true.                                                 */
  /*******************************************************************/
{
  assert( isValid() );
  assert( (int)index.size() == m_nDims );

  vector<int> result = index;
  bool done = false;
  endReached = false;

  /* find the lowest dimension that needs to be increased */
  for( int i=m_nDims-1; (i>=0) && !done; i-- )
    if( result[i] < m_vNumBins[i]-1 ) {
      /* just increase this dimension in the index -> done. */
      result[i]++;
      done = true;

    } else {
      /* set this index dimension back to zero and increase the next */
      /* higher dimension.                                           */
      result[i] = 0;
    }

  if( !done )
    /* couldn't increase a single dimension -> end reached, signal   */
    /* overflow.                                                     */
    endReached = true;

  return result;
}


float VotingSpace::getIntersectingVolume3D( int x, int y, int z,
                                            const FeatureVector &fvMean,
                                            const FeatureVector &fvWindowSize )
{
  /* Only valid for 3 dimensions */
  assert( m_nDims==3 );
  assert( fvMean.numDims()==3 );
  assert( fvWindowSize.numDims()==3 );

  /* get the bin coordinates */
  float minx1 = m_vMinValues[0] + m_vRes[0]* x;
  float maxx1 = m_vMinValues[0] + m_vRes[0]*(x+1);
  float miny1 = m_vMinValues[1] + m_vRes[1]* y;
  float maxy1 = m_vMinValues[1] + m_vRes[1]*(y+1);
  float minz1 = m_vMinValues[2] + m_vRes[2]* z;
  float maxz1 = m_vMinValues[2] + m_vRes[2]*(z+1);

  /* get the kernel bbox coordinates */
  float minx2 = fvMean.at(0) - fvWindowSize.at(0);
  float maxx2 = fvMean.at(0) + fvWindowSize.at(0);
  float miny2 = fvMean.at(1) - fvWindowSize.at(1);
  float maxy2 = fvMean.at(1) + fvWindowSize.at(1);
  float minz2 = fvMean.at(2) - fvWindowSize.at(2);
  float maxz2 = fvMean.at(2) + fvWindowSize.at(2);

  /* find the intersection coordinates */
  float minx = max( minx1, minx2);
  float maxx = min( maxx1, maxx2);
  float miny = max( miny1, miny2);
  float maxy = min( maxy1, maxy2);
  float minz = max( minz1, minz2);
  float maxz = min( maxz1, maxz2);

  /* compute the intersection volume */
  float dx = maxx - minx;
  float dy = maxy - miny;
  float dz = maxz - minz;
  if( (dx>0.0) && (dy>0.0) && (dz>0.0) )
    return dx*dy*dz;
  else
    return 0.0;
}


float VotingSpace::getIntersectingVolume3D( const vector<int>   &vBinIdx,
                                            const FeatureVector &fvMean,
                                            const FeatureVector &fvWindowSize )
{
  /* Only valid for 3 dimensions */
  assert( m_nDims==3 );
  assert( vBinIdx.size()==3 );
  assert( fvMean.numDims()==3 );
  assert( fvWindowSize.numDims()==3 );

  /* get the bin coordinates */
  float minx1 = m_vMinValues[0] + m_vRes[0]* vBinIdx[0];
  float maxx1 = m_vMinValues[0] + m_vRes[0]*(vBinIdx[0]+1);
  float miny1 = m_vMinValues[1] + m_vRes[1]* vBinIdx[1];
  float maxy1 = m_vMinValues[1] + m_vRes[1]*(vBinIdx[1]+1);
  float minz1 = m_vMinValues[2] + m_vRes[2]* vBinIdx[2];
  float maxz1 = m_vMinValues[2] + m_vRes[2]*(vBinIdx[2]+1);

  /* get the kernel bbox coordinates */
  float minx2 = fvMean.at(0) - fvWindowSize.at(0);
  float maxx2 = fvMean.at(0) + fvWindowSize.at(0);
  float miny2 = fvMean.at(1) - fvWindowSize.at(1);
  float maxy2 = fvMean.at(1) + fvWindowSize.at(1);
  float minz2 = fvMean.at(2) - fvWindowSize.at(2);
  float maxz2 = fvMean.at(2) + fvWindowSize.at(2);

  /* find the intersection coordinates */
  float minx = max( minx1, minx2);
  float maxx = min( maxx1, maxx2);
  float miny = max( miny1, miny2);
  float maxy = min( maxy1, maxy2);
  float minz = max( minz1, minz2);
  float maxz = min( maxz1, maxz2);

  /* compute the intersection volume */
  float dx = maxx - minx;
  float dy = maxy - miny;
  float dz = maxz - minz;
  if( (dx>0.0) && (dy>0.0) && (dz>0.0) )
    return dx*dy*dz;
  else
    return 0.0;
}

