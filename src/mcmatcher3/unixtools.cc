/*********************************************************************/
/*                                                                   */
/* FILE         unixtools.cc                                         */
/* AUTHORS      Bastian Leibe                                        */
/* EMAIL        leibe@informatik.tu-darmstadt.de                     */
/*                                                                   */
/* CONTENT      Some tools for accessing files and directories       */
/*              under Unix.                                          */
/*                                                                   */
/* BEGIN        Tue Aug 14 2001                                      */
/* LAST CHANGE  Wed Aug 15 2001                                      */    
/*                                                                   */
/*********************************************************************/


#include <iostream>
#include <fstream>
#include <iomanip>

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#include "unixtools.hh"

bool readKeyWords( string filename, string &Type, vector<KeyWord> &vKeyWords )
  /*******************************************************************/
  /* Read a text file and return all "key words" (i.e. words that    */
  /* appear before a "=" character) and their assigned values.       */
  /*******************************************************************/
{
  ifstream ifile;

  Type = "";
  vKeyWords.clear();

  /* try to open the file */
  ifile.open( filename.c_str() );
  if ( !ifile ) {
    cerr << "Error in extractKeyWords(): Couldn't open file '" << filename << "'!" << endl;
    return false;
  }

  /* read the first line => determines the type of the file */
  ifile >> Type;
  
  /* read the rest of the file line by line */
  while( ifile.peek() != EOF ) {
    string line;
    unsigned pos;

    /* read the next line */
    getline( ifile, line );
    /* check if the line contains a '=' */
    if ( (pos=line.find("=")) != string::npos ) {
      /* divide the line into key word and value */
      string key   = line.substr(0, pos);
      string value = line.substr(pos+1);
      
      /* and add the to the result vector */
      vKeyWords.push_back( KeyWord(key, value) );
    }
  }

  return true;
}


bool keyWordDefined( string Name, vector<KeyWord> &vKeyWords, string &Value )
  /*******************************************************************/
  /* Search the key-word list for a specific entry and return its    */
  /* value.                                                          */
  /*******************************************************************/
{
  bool result = false;

  for ( vector<KeyWord>::iterator it = vKeyWords.begin(); 
	it != vKeyWords.end(); it++ )
    if ( Name.compare( it->name() ) == 0 ) {
      Value = it->value();
      result = true;
      break;
    }

  return result;
}


vector<string> extractListItems( string List )
  /*******************************************************************/
  /* Extract the words (separated by spaces) from a string.          */
  /*******************************************************************/
{
  vector<string> items;
  unsigned pos_old = 0;
  unsigned pos_new = 0;

  while ( pos_old != string::npos ) {
    /* search for the next space character */
    if ( (pos_new=List.find( " ", pos_old )) == string::npos )
      break;
    else {
      /* found a space character => extract the list item */
      items.push_back( List.substr( pos_old, pos_new - pos_old ) );

      /* skip all following spaces */
      pos_old = pos_new;
      while ( (pos_old < List.length()) && (List[pos_old] == ' ') ) 
	pos_old++;
    }
   
  }

  /* extract the last list item */
  items.push_back( List.substr(pos_old) );
  
  return items;
}


bool getDirEntries( string dir, vector<string> &vEntries )
  /*******************************************************************/
  /* Return a vector of file names in the given directory.           */
  /*******************************************************************/
{
  DIR *dp;
  struct dirent *entry;
  struct stat statbuf;

  vEntries.clear();

  /* open the directory */
  if ( (dp=opendir(dir.c_str())) == NULL ) {
    cerr << "Error: Cannot open directory '" << dir << "'!" << endl;
    return false;
  }

  /* read out the directory entries */
  chdir( dir.c_str() );
  while ( (entry=readdir(dp)) != NULL ) {
    /* copy the stat block to a buffer */
    stat( entry->d_name, &statbuf );

    string filename = entry->d_name;
    /* ignore directories */
    if ( !S_ISDIR(statbuf.st_mode) )
      /* ignore file names that begin with a dot */
      if ( (filename.length() > 0) && (filename[0] != '.') )
	vEntries.push_back( filename );
  }
  return true;
}


bool getSubDirs( string dir, vector<string> &vEntries )
  /*******************************************************************/
  /* Return a vector of subdirectories in the given directory.       */
  /*******************************************************************/
{
  DIR *dp;
  struct dirent *entry;
  struct stat statbuf;

  vEntries.clear();

  /* open the directory */
  if ( (dp=opendir(dir.c_str())) == NULL ) {
    cerr << "Error: Cannot open directory '" << dir << "'!" << endl;
    return false;
  }

  /* read out the directory entries */
  chdir( dir.c_str() );
  while ( (entry=readdir(dp)) != NULL ) {
    /* copy the stat block to a buffer */
    stat( entry->d_name, &statbuf );

    string filename = entry->d_name;
    /* take only directories */
    if ( S_ISDIR(statbuf.st_mode) )
      /* ignore '.' and '..' */
      if ( (filename.compare(".") == 0) || (filename.compare("..") == 0) )
	continue;
      else
	vEntries.push_back( filename );
  }
  return true;
}


void testKeyWords( int argc, char* argv[])
  /* test the key word functions. */
{
    string type;
    string value;
    vector<KeyWord> keys;
    if ( argc > 1 )
      if ( readKeyWords( argv[1], type, keys ) ) {
	cout << "Type: " << type << endl;
	cout << "Keywords: " << endl;
	for ( vector<KeyWord>::iterator it = keys.begin(); 
	      it != keys.end(); it++ )
	  cout << "  Key:   " << it->name() << endl
	       << "  Value: " << it->value() << endl;

	if ( keyWordDefined( "CLASSES", keys, value ) ) {
	  cout << "Key 'CLASSES' defined with value '" << value << "'." << endl;
	  vector<string> items = extractListItems( value );
	  for ( vector<string>::iterator it = items.begin(); it != items.end(); it++ )
	    cout << "  Item: " << it->c_str() << endl;
	} else
	  cout << "Key 'CLASSES' not defined." << endl;
      }
}


void testGetDirEntries( int argc, char* argv[])
  /* test the directory functions. */
{
    vector <string> direntries;
    if ( argc > 1 )
      if ( getDirEntries ( argv[1], direntries ) )
	for ( vector<string>::iterator it = direntries.begin(); 
	      it != direntries.end(); it++ )
	  cout << it->c_str() << endl;
}
