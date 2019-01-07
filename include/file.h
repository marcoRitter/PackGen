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
    Q_PROPERTY(QString version READ version WRITE setVersion)
    Q_PROPERTY(QString start_addr READ start_addr WRITE setStart_addr)
    Q_PROPERTY(QString object_name READ object_name WRITE setObject_name)

public:
    file(QObject *parent = nullptr);
    ~file();

    QString node_type() {return"Fpga";}

    QString description();
    void setDescription(QString description);

    FileString filename();
    void setFilename(FileString filename);

    QString version();
    void setVersion(QString version);

    QString object_name();
    void setObject_name(QString object_name);

    QString start_addr();
    void setStart_addr(QString start_addr);

    void node_menue(QMenu *menu);

private:

    FileString m_filename;
    QString m_description;
    QString m_version;
    QString m_start_addr;

    QString m_objectName;

    QAction *pDeleteFile;
    QObject *m_parent;
};

#endif // FILE_H
