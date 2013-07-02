/*********************************************************************/
/*                                                                   */
/* FILE         unixtools.hh                                         */
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


#ifndef UNIXTOOLS_HH
#define UNIXTOOLS_HH

using namespace std;

#include <vector>
#include <string>


class KeyWord
{
public:
  KeyWord( string Name, string Value ) { m_sName = Name; m_sValue = Value; }
  
  string name()          { return m_sName; }
  string value()         { return m_sValue; }

private:
  string m_sName;
  string m_sValue;
};


/*************************************/
/*   Key Word Extraction Functions   */
/*************************************/
bool readKeyWords( string filename, string &Type, vector<KeyWord> &vKeyWords );
/* read a text file and return all "key words" (i.e. words that    */
/* appear before a "=" character) and their assigned values.       */

bool keyWordDefined( string Name, vector<KeyWord> &vKeyWords, string &Value );
  /* search the key-word list for a specific entry and return its    */
  /* value.                                                          */

vector<string> extractListItems( string List );
  /* extract the words (separated by spaces) from a string.          */

void testKeyWords( int argc, char* argv[]);
  /* test the key word functions. */


/**********************************/
/*   Directory Access Functions   */
/**********************************/
bool getDirEntries( string dir, vector<string> &vEntries );
  /* return a vector of file names in the given directory.           */

bool getSubDirs( string dir, vector<string> &vEntries );
  /* return a vector of subdirectories in the given directory.       */

void testGetDirEntries( int argc, char* argv[]);
  /* test the directory functions. */

#endif
