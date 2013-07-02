
#include "fileoperations.h"

#include <qfile.h>
#include <iostream>

QString FileOperations::getFileExtension(const QString& filename)
{
    int pos = filename.findRev('.');
    return filename.right( filename.length() - pos);
}

QString FileOperations::getBasename(const QString& filename)
{
    int pos = filename.findRev('.');
    return filename.left(pos);
}

bool FileOperations::fileExists(const QString& filename)
{
    QFile file( filename );
    if (!file.exists())
    {
	    cerr << "File not found: " << filename.ascii() << endl;
        return false;
    }
    return true;
}

/*void FileOperations::getMapName(const QString& imgName, QString& mapName)
{
    // get map name and map dir //
    mapName = imgName;
    int pos = mapName.findRev( '.' );
    if ( pos >= 0 )
        mapName.insert(pos, "-map");

}*/

void FileOperations::parseFilename(const QString& filename, QString& dir, QString& imgName)
{
    int len = filename.length();

    // get imgName and dir //
    int pos = filename.findRev( '/' );
    dir = filename.left( pos+1 );
    imgName = filename.right(len - pos -1);

    cout << filename << " "  << dir << " "  << imgName << " " << endl;
}

QStringList FileOperations::contentsByLine(const QString& filename)
{
    QStringList list;
    QString line;
    if (!fileExists(filename))
        return list;

    QFile f(filename);
    f.open(IO_ReadOnly);
    QTextStream tstream(&f);

    while(!(line=tstream.readLine()).isNull())
    {
        list.push_back(line);
    }
    f.close();
    return list;

}
