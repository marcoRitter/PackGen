#include "version_file.h"

int versionFileCreate(QString fileName, QString versionString)
{
    QFile versionFile (fileName);

    if (versionFile.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&versionFile);
        stream << versionString << endl;
    return 1;
    }
    return 0;
}
