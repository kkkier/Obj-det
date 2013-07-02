
#include "stringoperations.h"

#include <qfile.h>
#include <qtextstream.h>
#include <iostream>


string StringOperations::floatToString(float val)
{
    char tmp[256];
    sprintf(tmp, "%f", val);
    //strstream s;
    //s << val <<'\0';
    //return s.str();
    return string(tmp);
}

float StringOperations::stringToFloat(const char* s)
{
    float val;
    sscanf(s, "%f", &val);
    return val;
}
