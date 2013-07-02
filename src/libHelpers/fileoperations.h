

#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <qstring.h>
#include <qstringlist.h>

///////////////////////////////////////////////////////////////////////////////
//
// Filename stuff
//
//

using namespace std;

class FileOperations
{
  public:
	static QString getFileExtension(const QString& filename);
	static QString getBasename(const QString& filename);
	static bool fileExists(const QString& filename);
	static void parseFilename(const QString& filename, QString& dir, QString& imgName);
	static QStringList contentsByLine(const QString& filename);    

	//static void getMapName(const QString& imgName, QString& mapName);
};

#endif
