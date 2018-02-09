#ifndef VERSION_FILE_H
#define VERSION_FILE_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QObject>
#include <QFile>
#include <QTextStream>


int versionFileCreate (QString fileName, QString versionString);
int scriptFileCreate (QString fileName, QString dataToWrite, bool rewrite);
#endif // VERSION_FILE_H
