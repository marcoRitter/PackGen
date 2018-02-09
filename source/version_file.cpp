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

int scriptFileCreate (QString fileName, QString dataToWrite, bool rewrite)
{
    QFile scriptFile (fileName);
    if (scriptFile.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        if (rewrite)
            scriptFile.resize(0);
        QTextStream stream(&scriptFile);
        stream << dataToWrite;
        scriptFile.close();
        return 1;
    }
    return 0;
}
