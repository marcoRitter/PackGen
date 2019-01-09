#include "goldenReference.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QProcess>
#include <output.h>
#include "version_file.h"

goldenReference::goldenReference(QObject *parent) : Node(parent, "goldenReference")
{

    pGenerate = new QAction(tr("Generate"), this);
    connect(pGenerate, SIGNAL(triggered()), this, SLOT(generate_goldenRef()));

    pDelete = new QAction(tr("&Delete"), this);
    connect(pDelete, &QAction::triggered, this, &Node::delete_node);

    connect(this,SIGNAL(setOutInfo(QString, QColor)),parent->parent(), SLOT(printOutInfo(QString, QColor)));

    connect(parent->parent(), SIGNAL (generateFpga()), this, SLOT(generate_goldenRef()));

    m_parent = parent;
}

goldenReference::~goldenReference()
{
    disconnect(pGenerate, SIGNAL (triggered()),this, SLOT(generate_goldenRef()));
    disconnect(pDelete, &QAction::triggered, this, &Node::delete_node);
    disconnect(m_parent->parent(), SIGNAL (generateFpga()), this, SLOT(generate_goldenRef()));
    disconnect(this,SIGNAL(setOutInfo(QString, QColor)),m_parent->parent(), SLOT(printOutInfo(QString, QColor)));

    delete pGenerate;
    delete pDelete;
}

QString goldenReference::filename()
{
    return m_filename;
}
void goldenReference::setFilename(QString filename)
{
    m_filename = filename;
}

FileString goldenReference::location()
{
    return m_location;
}
void goldenReference::setLocation(FileString location)
{
    m_location = location;
}

FileType goldenReference::file_type()
{
    return m_file_type;
}
void goldenReference::setFile_type(FileType filetype)
{
    m_file_type = filetype;
}

QString goldenReference::version()
{
    return m_version;
}
void goldenReference::setVersion(QString version)
{
    m_version = version;
}

FlashSize goldenReference::flash_size()
{
    return m_flash_size;
}
void goldenReference::setFlash_size(FlashSize flashsize)
{
    m_flash_size = flashsize;
}

void goldenReference::node_menue(QMenu *menu)
{
    menu->addAction(pGenerate);
    menu->addSeparator();
    menu->addAction(pDelete);
}

bool goldenReference::setSrecParameters()
{
    return 0;
}

int goldenReference::runSrec()
{
    return 1;
}

bool goldenReference::generate_goldenRef()
{
    return  true;
}


