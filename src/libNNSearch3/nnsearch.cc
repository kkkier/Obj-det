/*********************************************************************/
/*                                                                   */
/* FILE         nnsearch.hh                                          */
/* AUTHORS      Bastian Leibe, based on code by Sameer Nene          */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Implement an efficient NN search based on the method */
/*              by Nene and Nayar, as described in the following     */
/*              paper:                                               */
/*                                                                   */
/*              S. Nene, S. Nayar,                                   */
/*              A Simple Algorithm for Nearest Neighbor Search in    */
/*              High Dimensions.                                     */
/*              IEEE Trans. PAMI, Vol. 19(9), pp. 989-1003, 1997.    */
/*                                                                   */
/*              This implementation is based on code by Sameer Nene, */
/*              as provided on the CAVE web page at Columbia Univ.   */
/*              The code has been modified to work on the Feature-   */
/*              Vector data structure.                               */
/*                                                                   */
/* BEGIN        Wed Jul 24 2002                                      */
/* LAST CHANGE  Wed Jul 24 2002                                      */
/*                                                                   */
/*********************************************************************/

  
/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <algorithm>

#include "nnsearch.hh"

/*******************/
/*   Definitions   */
/*******************/

/*===================================================================*/
/*                           Class NNSearch                          */
/*===================================================================*/

/***********************************************************/
/*                      Constructors                       */
/***********************************************************/

NNSearch::NNSearch()
  /* standard constructor */
{
  m_vData.clear();
  m_vOrigData.clear();
  m_vvBackMap.clear();
  m_vvForwardMap.clear();
  m_bIsValid = false;
}


NNSearch::NNSearch( const vector<FeatureVector> &vData )
{
  init( vData );
}


NNSearch::~NNSearch()
  /* standard destructor */
{
  m_vData.clear();
  m_vOrigData.clear();
  m_vvBackMap.clear();
  m_vvForwardMap.clear();
}


/***********************************************************/
/*                      Construction                       */
/***********************************************************/

struct compPairIncr
{
  bool operator()( const pair<float,int> &x, const pair<float,int> &y )
  { return (x.first < y.first); }
};


struct compPairDecr
{
  bool operator()( const pair<float,int> &x, const pair<float,int> &y )
  { return (x.first > y.first); }
};


void NNSearch::init( const vector<FeatureVector> &vData, bool bVerbose )
{
  assert( !vData.empty() );

  int nNumPoints = vData.size();            // j
  int nNumDims   = vData[0].numDims();      // k

  if( bVerbose ) {
    cout << "  NNSearch::init() called." << endl;
    cout << "    with " << nNumPoints << " points and " 
         << nNumDims << " dimensions." << endl;
  }

  /* set length of d_data[0] to vData.size() */
  //d_data[0].length(j = vData.size());
  vector<FeatureVector> vTmpData( nNumDims );
  m_vData = vTmpData;
  //m_vData[0].setNumDims( nNumPoints );

  /* allocate a vector of backward and forward maps */
  //d_bmaps_p = new int*[k = vData[0].numDims()];
  //d_fmaps_p = new int*[k];
  vector< vector<int> > vvTmpMap( nNumDims );
  m_vvBackMap    = vvTmpMap;
  m_vvForwardMap = vvTmpMap;

  m_vOrigData = vData;

  /*=====================*/
  /* Initialize the maps */
  /*=====================*/
  if( bVerbose ) 
    cout << "    Initializing the maps..." << endl;
  /* for all dimensions */
  //d_sa_p = new double[j];
  //for(d_i = 0; d_i < k; ++d_i) {
  for( int d_i=0; d_i<nNumDims; d_i++ ) {
    //cout << "    Processing dim " << d_i << "..." << endl;

    /* set length of d_data[d_i] to nNumPoints */
    //d_data[d_i].length(j);
    m_vData[d_i].setNumDims( nNumPoints );//
    
    /* allocate a new backward map */
    //cout << "      Allocating a new backward map..." << endl;
    //p1 = d_bmaps_p[d_i] = new int[j];
    vector<int> vTmpMap( nNumPoints );
    m_vvBackMap[d_i] = vTmpMap;
    vector<int> &vCurBMap = m_vvBackMap[d_i];

    /* initialize the backward map */
    //cout << "      Initializing the bmap..." << endl;
    //for(i = 0; i < j; ++i) {
      /* set the i'th entry to i */
      //p1[i] = i;                         // vCurBMap[i] = i;
      
      /* get the d_i'th element of point i */
      //d_sa_p[i] = va[i].element(d_i);    // vdsap[i] = vData[i].at( d_i );
    //}
    vector< pair<float,int> > vpValues( nNumPoints );
    for( int i=0; i<nNumPoints; i++ ) {
      //vCurBMap[i] = i;
      vpValues[i].first  = vData[i].at( d_i );
      vpValues[i].second = i;
    } 

    /* sort the entries in the backmap according to the original points */
    //cout << "      Sorting the bmap entries..." << endl;
    //qsort(p1, j, sizeof(int), compare);
    sort( vpValues.begin(), vpValues.end(), compPairIncr() );
    //sort( vpValues.begin(), vpValues.end(), compPairDecr() );

    /* get the sorted backward maps and elements */
    //cout << "      Retrieving the sorted elements..." << endl;
    //for(i = 0; i < j; ++i)           
    //  d_data[d_i].element(i, d_sa_p[p1[i]]); 
    for( int i=0; i<nNumPoints; i++ ) {
      m_vData[d_i].at(i) = vpValues[i].first;
      vCurBMap[i]        = vpValues[i].second;
    }

    /* allocate a new forward map */
    //cout << "      Allocating a new forward map..." << endl;
    //p2 = d_fmaps_p[d_i] = new int[j];
    m_vvForwardMap[d_i] = vTmpMap;
    vector<int> &vCurFMap = m_vvForwardMap[d_i];

    /* initialize the forward map */
    //cout << "      Initializing the fmap..." << endl;
    //for(i = 0; i < j; ++i)
    //  p2[p1[i]] = i;
    for( int i=0; i<nNumPoints; i++ )
      vCurFMap[vCurBMap[i]] = i;

    //cout << endl;
  }
  if( bVerbose )
    cout << "    done." << endl;
  
  m_bIsValid = true;
  if( bVerbose )
    cout << "  done." << endl;
}


/***********************************************************/
/*                 Nearest Neighbor Search                 */
/***********************************************************/


int NNSearch::getNN( /*const*/ FeatureVector &fvQuery, float dMaxDist, 
                     float dEps, bool bVerbose )
{
  if( !m_bIsValid ) {
    cerr << "Error in NNSearch::getNN(): "
         << "Need to initialize the NN structure first!" << endl;
    return -1;
  }

  /* get the list of all neighbors within the hypersphere */
  vector<int>   vIdzs;
  vector<float> vDists;
  getNNs( fvQuery, dMaxDist, dEps, vIdzs, vDists, bVerbose );
  
  if( vIdzs.empty() )
    return -1;

  /* search the list for the exact NN (with smallest dist) */
  int   minidx  = vIdzs[0];
  float mindist = vDists[0];
  for( int i=0; i<(int)vIdzs.size(); i++ )
    if( vDists[i] < mindist ) {
      mindist = vDists[i];
      minidx  = vIdzs[i];
    }

  return minidx;
}


void NNSearch::getNNs( /*const*/ FeatureVector &fvQuery, float dMaxDist,
                       float dEps,
                       vector<int> &vResultIdzs, vector<float> &vResultDist2,
                       bool bVerbose )
{
  if( !m_bIsValid ) {
    cerr << "Error in NNSearch::getNN(): "
         << "Need to initialize the NN structure first!" << endl;
    return;
  }

  int nNumDims   = m_vData.size();        // j
  int nNumPoints = m_vData[0].numDims();  // k

  if( bVerbose ) {
    cout << "    NNSearch::getNNs() called." << endl;
    cout << "      with " << nNumPoints << " points and " 
         << nNumDims << " dimensions." << endl;
    cout << "      max dist=" << dMaxDist << ", eps=" << dEps << endl;
  }

  /*************************************/
  /* Determine the bounds for all dims */
  /*************************************/
  if( bVerbose ) 
    cout << "      Determining the bounds for all dims..." << endl;
  //b = new int[j = d_data.getSize()];
  //t = new int[j];
  //for(i = 0, k = d_data[0].length() - 1, l = 0; i < j; ++i) {
  //  b[i] = bsearchL(i, v.element(i) - d_thresh);
  //  t[i] = bsearchR(i, v.element(i) + d_thresh);
  //}
  vector<int> vLower ( nNumDims );
  vector<int> vHigher( nNumDims );
  for( int i=0; i<nNumDims; i++ ) {
    vLower[i]  = binSearchL( i, fvQuery.at(i) - dEps );
    vHigher[i] = binSearchR( i, fvQuery.at(i) + dEps );
  }


  /***************************************************************/
  /* Sort the dimensions according to the size of their response */
  /***************************************************************/
  /* allocate an index vector for the dimensions */
  if( bVerbose )
    cout << "      Allocating an index vector for the dims..." << endl;
  //d = new int[j];
  //for(i = 0; i < j; ++i)
  //  d[i] = i;
  vector<int> vIdx( nNumDims );
  for( int i=0; i<nNumDims; i++ )
    vIdx[i] = i;

  
  /* sort the index according to the number of matching points per dim */
  if( bVerbose )
    cout << "      Sorting the index according to #matching points/dim..." 
       << endl;
  //for(i = 0, n = 1; i < j - 1 && n != 0; ++i)
  //  for(k = n = 0; k < j - i - 1; ++k)
  //    if(t[d[k + 1]] - b[d[k + 1]] < t[d[k]] - b[d[k]]) {
  //      l = d[k];
  //      d[k] = d[k + 1];
  //      d[k + 1] = l;
  //      n = 1;
  //    }
  bool bFound = true;
  for( int i=0; i<nNumDims-1 && bFound; i++ ) {
    bFound = false;
    for( int k=0; k<nNumDims-i-1; k++ )
      if( (vHigher[vIdx[k+1]] - vLower[vIdx[k+1]]) < 
          (vHigher[vIdx[k]] - vLower[vIdx[k]]) ) {
        /* swap vIdx[k] and vIdx[k+1] */
        int tmp = vIdx[k];
        vIdx[k] = vIdx[k+1];
        vIdx[k+1] = tmp;
        bFound = true;
      }
  }
   

  /***************************************************************/
  /* Create a list of all points in a hypercube around the query */
  /***************************************************************/
  /* Initialize the maps */
  if( bVerbose )
    cout << "      Initializing the maps..." << endl;
  //bmap = d_bmaps_p[d[0]];
  //p = d_fmaps_p[d[1]];
  //list = new int[t[d[0]] - b[d[0]] + 1];
  vector<int> &vCurBMap = m_vvBackMap[vIdx[0]];
  vector<int> &vCurFMap = m_vvForwardMap[vIdx[1]];
  //vector<int> vList; //( vHigher[vIdx[0]] - vLower[vIdx[0]] + 1 );
  vector<int> vList( vHigher[vIdx[0]] - vLower[vIdx[0]] + 1, -1 );

  /* construct the initial list out of matching points in dim 0 */
  if( bVerbose )
    cout << "      Constructing the initial list in dim 0..." << endl;
  //for(i = b[d[0]], k = t[d[0]], n = 0, bot = b[d[1]], top = t[d[1]]; 
  //    i <= k; ++i) {
  //  o = p[l = bmap[i]]; 
  //  if(o >= bot && o <= top)
  //    list[n++] = l;
  //}
  int nIdxBottom = vLower [vIdx[1]];
  int nIdxTop    = vHigher[vIdx[1]];
  for( int i=vLower[vIdx[0]], k=vHigher[vIdx[0]], n=0; i<=k; i++ ) {
    int nBack = vCurBMap[i];
    int nForw = vCurFMap[nBack];
    if( (nForw>=nIdxBottom) && (nForw<=nIdxTop) )
      //vList.push_back(nBack);
      vList[n++] = nBack;
  }
  if( bVerbose )
    cout << "        list size: " << vList.size() << endl;

  /* reduce the list by checking the other dimensions */
  if( bVerbose )
    cout << "      Reducing the list by checking the other dimensions..." 
         << endl;
  //for(i = 2, j = d_data.getSize(); i < j; ++i) {
  //  p = d_fmaps_p[d[i]];
  //  bot = b[d[i]];
  //  top = t[d[i]];
  //  for(k = 0, l = n, n = 0; k < l; ++k) {
  //    o = p[u = list[k]]; 
  //    if(o >= bot && o <= top)
  //      list[n++] = u;
  //  }
  //}
  vector<int> vNewList;
  for( int i=2; i<nNumDims; i++ ) {
    vector<int> &vFmap = m_vvForwardMap[vIdx[i]];
    nIdxBottom = vLower [vIdx[i]];
    nIdxTop    = vHigher[vIdx[i]];
    vNewList.clear();
    for( int k=0; k<(int)vList.size(); k++ ) {
      int nIdx1 = vList[k];
      int nIdx2 = vFmap[nIdx1];
      if( (nIdx2>=nIdxBottom) && (nIdx2<=nIdxTop) )
        vNewList.push_back( nIdx1 );
    }
    vList = vNewList;
  }
  if( bVerbose )
    cout << "        list size: " << vList.size() << endl;


  /*******************************************************************/
  /* Verify that the remaining entries in the list lie within the    */
  /* hypersphere around the query point (instead of the hypercube    */
  /* that has only been checked so far).                             */
  /*******************************************************************/
  if( bVerbose ) 
    cout << "      Verifying that the remaining points are inside "
         << "hypersphere..." << endl;
  //Vector s(j);
  //m = MAXDOUBLE;
  //for(k = 0, l = n, tsq = d_thresh * d_thresh; k < l; ++k) {
  //  o = list[k];
  //  for(i = 0; i < j; ++i)
  //    s.element(i, d_data[i].element(d_fmaps_p[i][o]));
  //  if((q = s.distance(v)) < m)
  //    if(q <= tsq) {
  //      m = q;
  //      n = list[k];
  //    }
  //}
  vResultIdzs.clear();
  vResultDist2.clear();
  float dMaxDist2 = dMaxDist*dMaxDist;
  for( int k=0; k<(int)vList.size(); k++ ) {
    int nIdx = vList[k];
    //FeatureVector fvRes( nNumDims );
    //for( int i=0; i<nNumDims; i++ )
    //  fvRes.at(i) = m_vData[i].at( m_vvForwardMap[i][nIdx] );
    //assert( nIdx>=0 );
    //assert( nIdx<(int)m_vOrigData.size() );
    FeatureVector &fvRes = m_vOrigData[nIdx];

    float dist  = fvRes.compSSD( fvQuery );
    //float dist2 = fvRes.compCorrelation( fvQuery );
    //cout << "        point " << k << ": dist=" << sqrt(dist) 
    //     << ", dist^2=" << dist << ", corr=" << dist2
    //     << endl;
    if( dist <= dMaxDist2 ) {
      vResultIdzs.push_back( nIdx );
      vResultDist2.push_back( dist );
    }
  }
  if( bVerbose ) {
    cout << "      done." << endl;
    cout << "        list size: " << vResultIdzs.size() << endl;
  }

  if( bVerbose ) 
    cout << "    done." << endl;
}


vector<int> NNSearch::getNNIdzs( /*const*/ FeatureVector &fvQuery, 
                                 float dMaxDist, float dEps, bool bVerbose )
{
  /* get the list of all neighbors within the hypersphere */
  vector<int>   vIdzs;
  vector<float> vDists;
  getNNs( fvQuery, dMaxDist, dEps, vIdzs, vDists, bVerbose );
  
  return vIdzs;
}


/***********************************************************/
/*                    Service Functions                    */
/***********************************************************/

int NNSearch::binSearchL( int nCol, float dMinVal ) const
{
  /* binary search for the lower bound */
  //while(t - b > 1) {
  //  c = (b + t) >> 1;
  //  q = v.element(c);
  //  if(d < q)
  //    t = c;
  //  else if(d > q)
  //    b = c;
  //  else
  //    return c;
  //}
  int nBottom = 0;
  int nTop    = m_vData[nCol].numDims()-1;
  while( nTop - nBottom > 1 ) {
    int   nMiddle = (nBottom + nTop) >> 1;  // (nBottom + nTop)/2
    float dVal    = m_vData[nCol].at(nMiddle);
    if( dMinVal < dVal )
      nTop = nMiddle;
    else if( dMinVal > dVal )
      nBottom = nMiddle;
    else
      return nMiddle;
  }

  /* return the lower bound */
  //return ((d <= v.element(b)) ? b : t);
  if( dMinVal <= m_vData[nCol].at( nBottom ) )
    return nBottom;
  else
    return nTop; 
}


int NNSearch::binSearchR( int nCol, float dMaxVal ) const
{
  /* binary search for the upper bound */
  //while(t - b > 1) {
  //  c = (b + t) >> 1;
  //  q = v.element(c);
  //  if(d < q)
  //    t = c;
  //  else if(d > q)
  //    b = c;
  //  else
  //    return c;
  //}
  int nBottom = 0;
  int nTop    = m_vData[nCol].numDims()-1;
  while( nTop - nBottom > 1 ) {
    int   nMiddle = (nBottom + nTop) >> 1;  // (nBottom + nTop)/2
    float dVal    = m_vData[nCol].at(nMiddle);
    if( dMaxVal < dVal )
      nTop = nMiddle;
    else if( dMaxVal > dVal )
      nBottom = nMiddle;
    else
      return nMiddle;
  }

  /* return the lower bound */
  //return ((d >= v.element(t)) ? t : b);
  if( dMaxVal >= m_vData[nCol].at( nTop ) )
    return nTop;
  else
    return nBottom; 
}



