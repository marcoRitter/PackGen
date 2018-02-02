#ifndef VERSION_FILE_H
#define VERSION_FILE_H

#include <QObject>

class version_file : public QObject
{
    Q_OBJECT
public:
    explicit version_file(QObject *parent = nullptr);

signals:

public slots:
};

#endif // VERSION_FILE_H