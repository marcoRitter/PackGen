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

    m_parent = parent;

}

firmware::~firmware()
{
    disconnect(pDeleteFirmware, &QAction::triggered, this, &Node::delete_node);
    delete pDeleteFirmware;
}

QString firmware::name()
{
    return m_name;
}

void firmware::setName(QString name)
{
    m_name = name;
}

QString firmware::filename()
{
    return m_filename;
}

void firmware::setFilename(QString filename)
{
    int pos = filename.lastIndexOf(QChar('/'));
    int pos2 = filename.length();

    if(pos+1 == pos2)
    {
        m_filename = filename.left(pos);
    }
    else {
        m_filename = filename;
    }
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


QString firmware::start_addr()
{
    return m_start_addr;
}

void firmware::setStart_addr(QString start_addr)
{
    m_start_addr = start_addr;
}

QString firmware::getVerString()
{
    QString ver = "";
    ver.append("\"V");
    ver.append(ver_major());
    ver.append(".");
    ver.append(ver_minor());
    ver.append(".");
    if (verstate().verstate.takeAt(static_cast<int>(verstate().selectedVersion)) == "Release")
        ver.append("00");
    else
        ver.append(ver_subminor());
    ver.append(" ");
    ver.append(verstate().verstate.takeAt(static_cast<int>(verstate().selectedVersion)));
    ver.append('"');
    return ver;
}

int firmware::runLogichdr()
{
    QString logichdr = m_parent->parent()->property("logichdr").value<FileString>().filestring;
    qDebug() << "logichdr EXE = " << logichdr;
    QStringList parameters;
    parameters.clear();
    QString temp = m_filename;
    parameters << "-i" << m_parent->parent()->property("working_directory").value<FileString>().filestring + "/" + temp.remove("xHOME/") << "-o" << m_mchFileName;
    parameters << "-c" << m_typecode;
    parameters << "-v" << m_variant;
    parameters << "-d" << "0x"+m_ver_major+m_ver_minor;
    parameters << "-r" << "0x"+m_ver_subminor;
    parameters << "-t" << "0x01";
    parameters << "-f" << "H86";
    parameters << "-e" << "FW1";
    qDebug() << "logichdr parameters = \n" << parameters;

    QProcess *process = new QProcess(nullptr);
    process->start(logichdr, parameters,QIODevice::ReadWrite);
    if (!process->waitForStarted())
        qDebug() << "error by executing srec_cat";
    if (!process->waitForFinished())
        qDebug() << "srec_cat failed";
    m_processOut = process->readAllStandardOutput();

    delete process;
    return 0;
}
