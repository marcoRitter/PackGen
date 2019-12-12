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

    Q_PROPERTY(QString input_file READ filename WRITE setFilename)
    Q_PROPERTY(FileType file_type READ file_type WRITE setFile_type)
    Q_PROPERTY(QString start_addr READ start_addr WRITE setStart_addr)
    Q_PROPERTY(QString object_name READ object_name WRITE setObject_name)


public:
    file(QObject *parent = nullptr);
    ~file();

    QString node_type() {return"File";}

    QString filename();
    void setFilename(QString filename);

    FileType file_type();
    void setFile_type(FileType file_type);


    QString object_name();
    void setObject_name(QString object_name);

    QString start_addr();
    void setStart_addr(QString start_addr);

    QVariant updateStartAddress();
    void node_menue(QMenu *menu);

signals:
    void need_redraw(const QString &name, const QVariant a);

private:

    QString m_filename= static_cast<QString>("xHOME/");
    FileType m_file_type;
    QString m_description;
    QString m_start_addr = "0x000000";
    FlashSize m_flash_size;


    QString m_objectName;

    QAction *pDeleteFile;
    QObject *m_parent;
};

#endif // FILE_H
