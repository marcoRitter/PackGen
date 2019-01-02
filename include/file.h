#ifndef FILE_H
#define FILE_H

#include "node.h"
#include <QFileInfo>
#include <QMenu>
#include <QDebug>
#include <QMetaProperty>
#include "customtype.h"

class file : public Node
{

    Q_OBJECT

    Q_PROPERTY(FileString filename READ filename WRITE setFilename)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QString version READ version WRITE setVersion)
    Q_PROPERTY(QString start_addr READ start_addr WRITE setStart_addr)

public:
    file(QObject *parent = nullptr);
    ~file();

    QString node_type() {return"File";}

    QString description();
    void setDescription(QString description);

    FileString filename();
    void setFilename(FileString filename);

    QString version();
    void setVersion(QString version);

    QString start_addr();
    void setStart_addr(QString start_addr);

    void node_menue(QMenu *menu);

private:

    FileString m_filename;
    QString m_description;
    QString m_version;
    QString m_start_addr;

    QAction *pDeleteFile;
    QObject *m_parent;
};

#endif // FILE_H
