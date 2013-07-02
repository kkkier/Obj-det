/*********************************************************************/
/*                                                                   */
/* FILE         occurrences.cc                                       */
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

/****************/
/*   Includes   */
/****************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <algorithm>

#include "occurrences.hh"

/********************************/
/*   Function Implementations   */
/********************************/

void saveOccurrences( string sFileName, VecVecOccurrence vvOccurrences,
                      bool bVerbose )
  /*******************************************************************/
  /* Save a list of occurrences to disk (using the FeatureVector     */
  /* file format).                                                   */
  /*******************************************************************/
{
  if( bVerbose )
    cout << "saveOccurrences() called." << endl;

  /* check the file name */
  int pos = sFileName.rfind( ".flz" );
  if( pos == (int)string::npos )
    sFileName.append ( ".flz" );

  /*******************************************************************/
  /*   Convert the Occurrences into Featurevectors for easy saving   */
  /*******************************************************************/
  vector<FeatureVector> vSaveOcc;
  /* for all clusters */
  for( int i=0; i<(int)vvOccurrences.size(); i++ )
    /* process all hypotheses for this cluster */
    for( int k=0; k<(int)vvOccurrences[i].size(); k++ ) {
      FeatureVector tmp( 12+1 );
      tmp.setValue( 0, (float) i );
      tmp.setValue( 1, vvOccurrences[i][k].dSimilarity );
      tmp.setValue( 2, (float) vvOccurrences[i][k].nCategory );
      tmp.setValue( 3, (float) vvOccurrences[i][k].nPose );
      tmp.setValue( 4, vvOccurrences[i][k].dScale );
      tmp.setValue( 5, vvOccurrences[i][k].dPosX );
      tmp.setValue( 6, vvOccurrences[i][k].dPosY );
      tmp.setValue( 7, (float) vvOccurrences[i][k].nImgNumber );
      tmp.setValue( 8, (float) vvOccurrences[i][k].nOccMapIdx );
      tmp.setValue( 9, vvOccurrences[i][k].dWeight );
      tmp.setValue(10, vvOccurrences[i][k].dAngle );
      tmp.setValue(11, vvOccurrences[i][k].dAxisRatio );
      tmp.setValue(12, vvOccurrences[i][k].dBBRatio );
      vSaveOcc.push_back( tmp );
    }

  /***************************************/
  /*   Save the list of featurevectors   */
  /***************************************/
  if( bVerbose )
    cout << "  Saving " << vSaveOcc.size() << " Occurrences..." << endl;
  saveFeatureVectorList( sFileName, vSaveOcc );

  if( bVerbose )
    cout << "saveOccurrences() done." << endl;
}


void saveOccurrencesMatlab( string sFileName, VecVecOccurrence vvOccurrences,
                            bool bVerbose )
  /*******************************************************************/
  /* Save a list of occurrences to disk (using a plain ASCII file    */
  /* format for use with Matlab).                                    */
  /*******************************************************************/
{
  if( bVerbose )
    cout << "saveOccurrencesMatlab() called." << endl;

  /*******************************************************************/
  /*   Convert the Occurrences into Featurevectors for easy saving   */
  /*******************************************************************/
  vector<FeatureVector> vSaveOcc;
  /* for all clusters */
  for( int i=0; i<(int)vvOccurrences.size(); i++ )
    /* process all hypotheses for this cluster */
    for( int k=0; k<(int)vvOccurrences[i].size(); k++ ) {
      FeatureVector tmp( 12+1 );
      tmp.setValue( 0, (float) i );
      tmp.setValue( 1, vvOccurrences[i][k].dSimilarity );
      tmp.setValue( 2, (float) vvOccurrences[i][k].nCategory );
      tmp.setValue( 3, (float) vvOccurrences[i][k].nPose );
      tmp.setValue( 4, vvOccurrences[i][k].dScale );
      tmp.setValue( 5, vvOccurrences[i][k].dPosX );
      tmp.setValue( 6, vvOccurrences[i][k].dPosY );
      tmp.setValue( 7, (float) vvOccurrences[i][k].nImgNumber );
      tmp.setValue( 8, (float) vvOccurrences[i][k].nOccMapIdx );
      tmp.setValue( 9, vvOccurrences[i][k].dWeight );
      tmp.setValue(10, vvOccurrences[i][k].dAngle );
      tmp.setValue(11, vvOccurrences[i][k].dAxisRatio );
      tmp.setValue(12, vvOccurrences[i][k].dBBRatio );
      vSaveOcc.push_back( tmp );
    }

  /***************************************/
  /*   Save the list of featurevectors   */
  /***************************************/
  if( bVerbose )
    cout << "  Saving " << vSaveOcc.size() << " Occurrences..." << endl;
  ofstream ofile( sFileName.c_str() );
  if( ofile == 0 ) {
    cerr << "ERROR: Couldn't open file " << sFileName << "!" << endl;
    return;
  }

  for( int i=0; i<(int)vSaveOcc.size(); i++ )
    vSaveOcc[i].writeData( ofile );
  ofile.close();

  if( bVerbose )
    cout << "saveOccurrencesMatlab() done." << endl;
}


// void saveOccurrenceMaps( string sFileName, 
//                          vector< vector<OpGrayImage> > vvOccMaps )
//   /*******************************************************************/
//   /* Save a list of segmentation maps with one entry for each clus-  */
//   /* ter occurrence to disk (using the FeatureVector file format).   */
//   /*******************************************************************/
// {
//   cout << "saveOccurrenceMaps() called." << endl;

//   /*******************************************************************/
//   /*   Convert the Occurrences into Featurevectors for easy saving   */
//   /*******************************************************************/
//   vector<FeatureVector> vSaveOccMaps;
//   /* for all clusters */
//   for( int i=0; i<(int)vvOccMaps.size(); i++ )
//     /* process all hypotheses for this cluster */
//     for( int k=0; k<(int)vvOccMaps[i].size(); k++ )
//       vSaveOccMaps.push_back( vvOccMaps[i][k].getData() );

//   /***************************************/
//   /*   Save the list of featurevectors   */
//   /***************************************/
//   cout << "  Saving " << vSaveOccMaps.size() << " Occurrence maps..." << endl;

//   if( !vSaveOccMaps.empty() ) {
//     /* modify the file name */
//     int pos = sFileName.rfind( ".flz" );
//     sFileName.replace( pos, 4, ".seg" );
//     sFileName.append ( ".flz" );
    
//     bool ok = saveFeatureVectorList( sFileName, vSaveOccMaps );
//     if ( !ok ) 
//       cerr << "Saving occurrence maps failed!" << endl;
//   }  

//   cout << "saveOccurrenceMaps() done." << endl;
// }


void saveOccurrenceMaps( string sFileName, 
                         /*const*/ vector<OpGrayImage> &vOccMaps,
                         bool bVerbose )
  /*******************************************************************/
  /* Save a list of segmentation maps with one entry for each clus-  */
  /* ter occurrence to disk (using the FeatureVector file format).   */
  /*******************************************************************/
{
  if( bVerbose )
    cout << "saveOccurrenceMaps() called." << endl;

  /*******************************************************************/
  /*   Convert the Occurrences into Featurevectors for easy saving   */
  /*******************************************************************/
  vector<FeatureVector> vSaveOccMaps;
  /* for all clusters */
  for( int i=0; i<(int)vOccMaps.size(); i++ )
    vSaveOccMaps.push_back( vOccMaps[i].getData() );

  /***************************************/
  /*   Save the list of featurevectors   */
  /***************************************/
  if( bVerbose )
    cout << "  Saving " << vSaveOccMaps.size() << " Occurrence maps..." 
         << endl;

  if( !vSaveOccMaps.empty() ) {
    /* modify the file name */
    int pos = sFileName.rfind( ".flz" );
    if( pos != (int)string::npos )
      sFileName.erase( pos );
    sFileName.append ( ".seg.flz" );
    
    bool ok = saveFeatureVectorList( sFileName, vSaveOccMaps );
    if ( !ok ) 
      cerr << "Saving occurrence maps failed!" << endl;
  }  

  if( bVerbose )
    cout << "saveOccurrenceMaps() done." << endl;
}


int loadOccurrences( string sFileName, int nNumClusters,
                     VecVecOccurrence &vvOccurrences, bool bVerbose )
  /*******************************************************************/
  /* Load a list of occurrences from disk (using the FeatureVector   */
  /* file format).                                                   */
  /*******************************************************************/
{ 
  if( bVerbose )
    cout << "loadOccurrences() called." << endl;

  /********************************************************/
  /*   Load the occurrences as a list of featurevectors   */
  /********************************************************/
  vector<FeatureVector> vLoadOcc;
  loadFeatureVectorList( sFileName, vLoadOcc );
  if( bVerbose )
    cout << "  " << vLoadOcc.size() << " Occurrences loaded..." << endl;

  /***************************************************/
  /*   Convert the Featurevectors into Occurrences   */
  /***************************************************/
  /* initialize the Occurrences vector */
  /* !!! The occurrences must fit to the current clustering!!! */
  vvOccurrences.clear();
  VecVecOccurrence tmp( nNumClusters );
  vvOccurrences = tmp;
  
  unsigned nNumOccs = vLoadOcc.size();
  for(unsigned k=0; k<nNumOccs; k++ ) {
    int clusterId = (int) vLoadOcc[k].at(0);
    if( (clusterId<0) || (clusterId>=(int)nNumClusters) ) {
      cerr << "  Error loading Occurrences: reference to invalid cluster '"
           << clusterId << "'!" << endl;
      return -1;
    }

    ClusterOccurrence occ;
    occ.dSimilarity = vLoadOcc[k].at(1);
    occ.nCategory   = (int)vLoadOcc[k].at(2);
    occ.nPose       = (int)vLoadOcc[k].at(3);
    occ.dScale      = vLoadOcc[k].at(4);
    occ.dPosX       = vLoadOcc[k].at(5);
    occ.dPosY       = vLoadOcc[k].at(6);
    if( vLoadOcc[k].numDims() >= 8 )
      occ.nImgNumber  = (int)vLoadOcc[k].at(7);
    else
      occ.nImgNumber  = 0;

    if( vLoadOcc[k].numDims() >= 9 )
      occ.nOccMapIdx = (int)vLoadOcc[k].at(8);
    else
      occ.nOccMapIdx  = k;

    if( vLoadOcc[k].numDims() >= 10 )
      occ.dWeight  = vLoadOcc[k].at(9);
    else
      occ.dWeight  = 1.0;

    if( vLoadOcc[k].numDims() >= 11 )
      occ.dAngle  = vLoadOcc[k].at(10);
    else
      occ.dAngle  = 0.0;

    if( vLoadOcc[k].numDims() >= 12 )
      occ.dAxisRatio  = vLoadOcc[k].at(11);
    else
      occ.dAxisRatio  = 1.0;

    if( vLoadOcc[k].numDims() >= 13 )
      occ.dBBRatio  = vLoadOcc[k].at(12);
    else
      occ.dBBRatio  = 1.0;

    vvOccurrences[clusterId].push_back( occ );
  }
  if( bVerbose )
    cout << "loadOccurrences() done." << endl;

  return (int)nNumOccs;
}


// void loadOccurrenceMaps( string sFileName, int nNumClusters,
//                          VecVecOccurrence vvOccurrences,
//                          vector< vector<OpGrayImage> > &vvOccMaps )
//   /*******************************************************************/
//   /* Load a list of segmentation maps with one entry for each clus-  */
//   /* ter occurrence from disk (using the FeatureVector file format). */
//   /*******************************************************************/
// { 
//   cout << "loadOccurrenceMaps() called." << endl;

//   /********************************************************/
//   /*   Load the occurrences as a list of featurevectors   */
//   /********************************************************/
//   /* modify the file name */
//   string sSuffix = ".flz";
//   int pos = sFileName.rfind( ".flz" );
//   if( pos == string::npos ) {
//     /* check if the file has the suffix *.fls */
//     pos = sFileName.rfind( ".fls" );
//     sSuffix = ".fls";
//   }

//   if( pos == string::npos ) {
//     cerr << "  Error in loadOccurrenceMaps: "
//          << "Unknown file extension (not .flz or .fls)!" << endl;
//     vvOccMaps.clear();
//     return;

//   } else {   
//     sFileName.replace( pos, 4, ".seg" );
//     sFileName.append ( sSuffix );
//   } 
      

//   vector<FeatureVector> vLoadOccMaps;
//   bool ok = loadFeatureVectorList( sFileName, vLoadOccMaps );
//   if( !ok ) {
//     cerr << "  No file containing occurrence maps found!" << endl;
//     vvOccMaps.clear();
//     return;
//   }
//   cout << "  " << vLoadOccMaps.size() << " Occurrence maps loaded..." << endl;

//   /*******************************************************/
//   /*   Convert the Featurevectors into Occurrence Maps   */
//   /*******************************************************/
//   /* assign the maps according to the known occurrence vector */
//   vvOccMaps.clear();
//   vector< vector<OpGrayImage> > tmp( nNumClusters );
//   vvOccMaps = tmp;

//   /* for all clusters */
//   int nCount = 0;
//   for( int i=0; i<(int)vvOccurrences.size(); i++ )
//     /* process all hypotheses for this cluster */
//     for( int k=0; k<(int)vvOccurrences[i].size(); k++ ) {
//       //vvOccMaps[i].push_back( vLoadOccMaps[nCount++] );
//       OpGrayImage imgTmp;
//       int imgsize = (int) sqrt( vLoadOccMaps[nCount].numDims() );
//       imgTmp.loadFromData( imgsize, imgsize, vLoadOccMaps[nCount++].getData());
//       vvOccMaps[i].push_back( imgTmp );
//     }

//   cout << "loadOccurrenceMaps() done." << endl;
// }


void loadOccurrenceMaps( string sFileName,
                          vector<OpGrayImage> &vOccMaps, bool bVerbose )
  /*******************************************************************/
  /* Load a list of segmentation maps with one entry for each clus-  */
  /* ter occurrence from disk (using the FeatureVector file format). */
  /*******************************************************************/
{ 
  if( bVerbose )
    cout << "loadOccurrenceMaps() called." << endl;

  /********************************************************/
  /*   Load the occurrences as a list of featurevectors   */
  /********************************************************/
  /* modify the file name */
  string sSuffix = ".flz";
  int pos = sFileName.rfind( ".flz" );
  if( pos == (int)string::npos ) {
    /* check if the file has the suffix *.fls */
    pos = sFileName.rfind( ".fls" );
    sSuffix = ".fls";
  }

  if( pos == (int)string::npos ) {
    cerr << "  Error in loadOccurrenceMaps: "
         << "Unknown file extension (not .flz or .fls)!" << endl;
    //vvOccMaps.clear();
    vOccMaps.clear();
    return;

  } else {
    if( pos != (int)string::npos )
      sFileName.erase( pos );
    sFileName.append ( ".seg"+sSuffix );
  } 
      

  vector<FeatureVector> vLoadOccMaps;
  bool ok = loadFeatureVectorList( sFileName, vLoadOccMaps );
  if( !ok ) {
    cerr << "  No file containing occurrence maps found!" << endl;
    //vvOccMaps.clear();
    vOccMaps.clear();
    return;
  }
  if( bVerbose )
    cout << "  " << vLoadOccMaps.size() << " Occurrence maps loaded..." 
         << endl;

  /*******************************************************/
  /*   Convert the Featurevectors into Occurrence Maps   */
  /*******************************************************/
  /* assign the maps according to the known occurrence vector */
  vOccMaps.clear();

  /* for all occurrence maps */
  for( int i=0; i<(int)vLoadOccMaps.size(); i++ ) {
    OpGrayImage imgTmp;
    int imgsize = (int) sqrt( (double)vLoadOccMaps[i].numDims() );
    imgTmp.loadFromData( imgsize, imgsize, vLoadOccMaps[i].getData());
    vOccMaps.push_back( imgTmp );
  }
    
  if( bVerbose )
    cout << "loadOccurrenceMaps() done." << endl;
}


void saveNegOccurrences( string sFileName, const vector<int> &vNegOccs, 
                         bool bVerbose )
  /*******************************************************************/
  /* Save a list of negative occurrences to disk (using the          */
  /* FeatureVector file format).                                     */
  /*******************************************************************/
{ 
  if( bVerbose )
    cout << "saveNegOccurrences() called." << endl;

  /*************************************/
  /*   Load the negative occurrences   */
  /*************************************/
  ofstream ofile( sFileName.c_str() );
  if ( !ofile ) {
    cerr << "  Error: Could not open file '" << sFileName << "'!" << endl;
    return;
  }

  /* write the number of entries */
  ofile << vNegOccs.size() << endl;

  /* save the neg occurrences */
  int nCount = 0;
  for(int i=0; i<(int)vNegOccs.size(); i++) {
    ofile << vNegOccs[i] << "  ";
    nCount += vNegOccs[i];
  }
  ofile << endl;
  if( bVerbose )
    cout << "  Saved " << nCount << " negative occurrences." << endl;

  /* close the neg occurrence file again. */
  ofile.close();

  if( bVerbose )
    cout << "saveNegOccurrences() done." << endl;
}


void loadNegOccurrences( string sFileName, int nNumClusters,
                         vector<int> &vNegOccs, bool bVerbose )
  /*******************************************************************/
  /* Load a list of negative occurrences from disk (using the        */
  /* FeatureVector file format).                                     */
  /*******************************************************************/
{ 
  if( bVerbose )
    cout << "loadNegOccurrences() called." << endl;

  /*************************************/
  /*   Load the negative occurrences   */
  /*************************************/
  ifstream ifile( sFileName.c_str() );
  if ( !ifile ) {
    cerr << "  Error: Could not open file '" << sFileName << "'!" << endl;
    return;
  }

  /* initialize the Negative Occurrences vector */
  vNegOccs.clear();
  vector<int> tmp( nNumClusters );
  vNegOccs = tmp;

  int nEntries;;
  ifile >> nEntries;

  /* !!! The occurrences must fit to the current clustering!!! */
  if( nEntries != nNumClusters ) {
    cerr << "  Error loading Negative Occurrences: "
         << "Number of clusters doesn't match!" << endl;
    return;
  }

  /* load the neg occurrences */
  int nCount = 0;
  for(int i=0; i<nEntries; i++) {
    int num;
    ifile >> num;
    vNegOccs[i] = num;
    nCount += num;
  }
  if( bVerbose )
    cout << "  Loaded " << nCount << " negative occurrences." << endl;

  /* close the neg occurrence file again. */
  ifile.close();

  if( bVerbose )
    cout << "loadNegOccurrences() done." << endl;
}


void saveCooccurrences( string sFileName, int nNumClusters,
                        VecVecCooccurrence vvCooccurrences )
  /*******************************************************************/
  /* Save a list of cooccurrences to disk (using the          */
  /* FeatureVector file format).                                     */
  /*******************************************************************/
{
  cout << "saveCooccurrences() called." << endl;

  /*********************************************************************/
  /*   Convert the Cooccurrences into Featurevectors for easy saving   */
  /*********************************************************************/
  vector<FeatureVector> vSaveCoocc;
  /* for all clusters */
  for( int i=0; i<(int)nNumClusters; i++ )
    for( int j=0; j<(int)nNumClusters; j++ )
      if( i > j ) {
        int idx = i*nNumClusters + j;
        
        /* process all hypotheses for this combination of clusters */
        for( int k=0; k<(int)vvCooccurrences[idx].size(); k++ ) {
          FeatureVector tmp( 8+2 );
          tmp.setValue( 0, (float) i );
          tmp.setValue( 1, (float) j );
          tmp.setValue( 2, vvCooccurrences[idx][k].dSimilarity1 );
          tmp.setValue( 3, vvCooccurrences[idx][k].dSimilarity2 );
          tmp.setValue( 4, (float) vvCooccurrences[idx][k].nCategory );
          tmp.setValue( 5, (float) vvCooccurrences[idx][k].nPose );
          tmp.setValue( 6, vvCooccurrences[idx][k].dAngle );
          tmp.setValue( 7, vvCooccurrences[idx][k].dDistance );
          tmp.setValue( 8, vvCooccurrences[idx][k].dScale );
          tmp.setValue( 9, vvCooccurrences[idx][k].dPosX );
          tmp.setValue(10, vvCooccurrences[idx][k].dPosY );
          vSaveCoocc.push_back( tmp );
        }
      }
  
  /***************************************/
  /*   Save the list of featurevectors   */
  /***************************************/
  cout << "  Saving " << vSaveCoocc.size() << " Cooccurrences..." << endl;
  saveFeatureVectorList( sFileName, vSaveCoocc );

  cout << "saveCooccurrences() done." << endl;
}


void loadCooccurrences( string sFileName, int nNumClusters,
                        VecVecCooccurrence &vvCooccurrences )
{ 
  cout << "loadCooccurrences() called." << endl;

  /********************************************************/
  /*   Load the occurrences as a list of featurevectors   */
  /********************************************************/
  vector<FeatureVector> vLoadCoocc;
  loadFeatureVectorList( sFileName, vLoadCoocc );
  cout << "  " << vLoadCoocc.size() << " Cooccurrences loaded..." << endl;

  /*****************************************************/
  /*   Convert the Featurevectors into Cooccurrences   */
  /*****************************************************/
  /* initialize the Cooccurrences vector */
  /* !!! The cooccurrences must fit to the current clustering!!! */
  vvCooccurrences.clear();
  VecVecCooccurrence tmp( nNumClusters*nNumClusters );
  vvCooccurrences = tmp;

  for( int k=0; k<(int)vLoadCoocc.size(); k++ ) {
    int clusterId1 = (int) vLoadCoocc[k].at(0);
    int clusterId2 = (int) vLoadCoocc[k].at(1);
    if( (clusterId1<0) || (clusterId2<0) ||
        (clusterId1>=nNumClusters) || 
        (clusterId1>=nNumClusters) ) {
      cerr << "  Error loading Cooccurrences: reference to invalid clusters '"
           << clusterId1 << "/" << clusterId2 << "'!" << endl;
      return;
    }

    ClusterCooccurrence coocc;
    coocc.dSimilarity1 = vLoadCoocc[k].at(2);
    coocc.dSimilarity2 = vLoadCoocc[k].at(3);
    coocc.nCategory   = (int)vLoadCoocc[k].at(4);
    coocc.nPose       = (int)vLoadCoocc[k].at(5);
    coocc.dAngle      = vLoadCoocc[k].at(6);
    coocc.dDistance   = vLoadCoocc[k].at(7);
    coocc.dScale      = vLoadCoocc[k].at(8);
    coocc.dPosX       = vLoadCoocc[k].at(9);
    coocc.dPosY       = vLoadCoocc[k].at(10);
    int idx = clusterId1*nNumClusters + clusterId2;
    vvCooccurrences[idx].push_back( coocc );
  }
  cout << "loadCooccurrences() done." << endl;
}


void saveCooccDistances( string sFileName, int nNumClusters,
                         VecVecCooccDistance vvCooccDistances  )
{
  cout << "saveCooccDistances() called." << endl;

  /*********************************************************************/
  /*   Convert the Cooccurrences into Featurevectors for easy saving   */
  /*********************************************************************/
  vector<FeatureVector> vSaveCoocc;
  /* for all cooccurrences */
  for( int i=0; i<(int)nNumClusters; i++ )
    for( int j=0; j<(int)nNumClusters; j++ )
      if( i > j ) {
        int idx = i*nNumClusters + j;
        
        /* process all hypotheses for this combination of clusters */
        for( int k=0; k<(int)vvCooccDistances[idx].size(); k++ ) {
          FeatureVector tmp( 3 );
          tmp.setValue( 0, (float) i );
          tmp.setValue( 1, (float) j );
          tmp.setValue( 2, vvCooccDistances[idx][k] );
          vSaveCoocc.push_back( tmp );
        }
      }
  
  /***************************************/
  /*   Save the list of featurevectors   */
  /***************************************/
  cout << "  Saving " << vSaveCoocc.size() << " Cooccurrences..." << endl;
  saveFeatureVectorList( sFileName, vSaveCoocc );
  
  cout << "saveCooccDistances() done." << endl;
}


void loadCooccDistances( string sFileName, int nNumClusters,
                         VecVecCooccDistance &vvCooccDistances )
{ 
  cout << "loadCooccDistances() called." << endl;

  /***********************************************************/
  /*   Load the cooccdistances as a list of featurevectors   */
  /***********************************************************/
  vector<FeatureVector> vLoadCoocc;
  loadFeatureVectorList( sFileName, vLoadCoocc );
  cout << "  " << vLoadCoocc.size() << " CooccDistances loaded..." << endl;

  /******************************************************/
  /*   Convert the Featurevectors into CooccDistances   */
  /******************************************************/
  /* initialize the CooccDistances vector */
  /* !!! The cooccdistances must fit to the current clustering!!! */
  vvCooccDistances.clear();
  VecVecCooccDistance tmp( nNumClusters*nNumClusters );
  vvCooccDistances = tmp;

  for( int k=0; k<(int)vLoadCoocc.size(); k++ ) {
    int clusterId1 = (int) vLoadCoocc[k].at(0);
    int clusterId2 = (int) vLoadCoocc[k].at(1);
    if( (clusterId1<0) || (clusterId2<0) ||
        (clusterId1>=nNumClusters) || 
        (clusterId1>=nNumClusters) ) {
      cerr << "  Error loading CooccDistances: reference to invalid clusters '"
           << clusterId1 << "/" << clusterId2 << "'!" << endl;
      return;
    }

    float dDistance   = vLoadCoocc[k].at(2);
    int idx;
    if( clusterId1 > clusterId2 )
      idx = clusterId1*nNumClusters + clusterId2;
    else
      idx = clusterId2*nNumClusters + clusterId1;
    vvCooccDistances[idx].push_back( dDistance );
  }
  cout << "loadCooccDistances() done." << endl;
}


