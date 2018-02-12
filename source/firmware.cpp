#include "firmware.h"
#include "mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>

firmware::firmware(QObject *parent) :
    Node(parent,"firmware")
{
    QIcon DeleteIcon;
    this->setObjectName("firmware");
    DeleteIcon.addFile(":/Images/icons8-delete.png",QSize(25,25));
    pDeleteFirmware = new QAction(tr("&Delete"),this);

    pDeleteFirmware->setIcon(DeleteIcon);
    connect(pDeleteFirmware, &QAction::triggered, this, &Node::delete_node);

}

firmware::~firmware()
{
    disconnect(pDeleteFirmware, &QAction::triggered, this, &Node::delete_node);
    delete pDeleteFirmware;
}

FileString firmware::filename()
{
    return m_filename;
}

void firmware::setFilename(FileString filename)
{
    m_filename = filename;
}

QString firmware::ver_major()
{
   return m_ver_major;
}

void firmware::setVer_major(QString ver_major)
{
    m_ver_major = ver_major;
}

QString firmware::ver_minor()
{
    return m_ver_minor;
}

void firmware::setVer_minor(QString ver_minor)
{
    m_ver_minor = ver_minor;
}

QString firmware::ver_subminor()
{
    return m_ver_subminor;
}

void firmware::setVer_subminor(QString ver_subminor)
{
    m_ver_subminor = ver_subminor;
}

VerState firmware::verstate()
{
    return m_verstate;
}

void firmware::setVerstate(VerState verstate)
{
    m_verstate = verstate;
}

void firmware::node_menue(QMenu *menu)
{
    menu->addAction(pDeleteFirmware);
}

QString firmware::getVerString()
{
    QString ver = "";
    ver.append("\"V");
    ver.append(ver_major().section("",3,4));
    ver.append(".");
    ver.append(ver_minor().section("",3,4));
    ver.append(".");
    ver.append(ver_subminor().section("",3,4));
    ver.append(" ");
    ver.append(m_verstate.verstate.takeAt(verstate().selectedVersion));
    ver.append('"');
    return ver;
}
