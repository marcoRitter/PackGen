#include <QJsonObject>
#include <QJsonDocument>
#include "file.h"
#include "mainwindow.h"


file::file(QObject *parent) : Node(parent,"File")
{
    QIcon DeleteIcon;
    //this->setObjectName("File");
    DeleteIcon.addFile(":/Images/icons8-delete.png",QSize(25,25));
    pDeleteFile = new QAction (tr("&Delete"),this);

    pDeleteFile->setIcon(DeleteIcon);
    connect(pDeleteFile, &QAction::triggered, this, &Node::delete_node);

    m_parent = parent;
}

file::~file()
{
    disconnect(pDeleteFile, &QAction::triggered, this, &Node::delete_node);
    delete pDeleteFile;
}

FileString file::filename()
{
    return m_filename;
}

void file::setFilename(FileString filename)
{
    m_filename = filename;
}

QString file::description()
{
    return m_description;
}

void file::setDescription(QString description)
{
    m_description = description;
}

QString file::version()
{
    return m_version;
}

void file::setVersion(QString version)
{
    m_version = version;
}

QString file::start_addr()
{
    return m_start_addr;
}

void file::setStart_addr(QString start_addr)
{
    m_start_addr = start_addr;
}

void file::node_menue(QMenu *menu)
{
    menu->addAction(pDeleteFile);
}

QString file::object_name()
{
    return this->getType();
}

void file::setObject_name(QString object_name)
{
    this->setType(object_name);
}
