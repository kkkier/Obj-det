

#ifndef STRINGOPERATIONS_H
#define STRINGOPERATIONS_H

#include <qstring.h>
#include <qstringlist.h>
#include <string>
///////////////////////////////////////////////////////////////////////////////
//
// Filename stuff
//
//

using namespace std;

class StringOperations
{
  public:
    static string floatToString(float val);
    static float stringToFloat(const char* s);
};

#endif
