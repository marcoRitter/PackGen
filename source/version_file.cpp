#include "version_file.h"

int versionFileCreate(QString fileName, QString versionString)
{
    QFile versionFile (fileName);

    if (versionFile.open(QIODevice::ReadWrite))
    {
        versionFile.resize(0);
        QTextStream stream(&versionFile);
        stream << versionString;
        versionFile.close();
    return 1;
    }
    return 0;
}
