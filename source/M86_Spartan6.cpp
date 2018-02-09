#include <QDebug>
#include "M86_Spartan6.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QProcess>
#include "srec_wrapper.h"
#include "version_file.h"


M86_Spartan6::M86_Spartan6(QObject *parent) :
    Node(parent,"M86")
{

    QIcon GenerateIcon;
    QIcon NewFpgaIcon;
    QIcon DeleteIcon;

    GenerateIcon.addFile(":/Images/icons8-robot.png",QSize(25,25));
    pGenerate = new QAction(tr("Generate"), this);
    pGenerate->setIcon(GenerateIcon);
    connect(pGenerate, SIGNAL(triggered()), this, SLOT(generate_package()));

    NewFpgaIcon.addFile(":/Images/integrated-circuit-256.png",QSize(25,25));
    pNewFPGA = new QAction(tr("&New FPGA"), this);
    pNewFPGA->setIcon(NewFpgaIcon);
    connect(pNewFPGA, &QAction::triggered, this, &M86_Spartan6::new_FPGA);

    pNewFirmware = new QAction(tr("New &Firmware"), this);
    connect(pNewFirmware,&QAction::triggered, this, &M86_Spartan6::new_Firmware);

    DeleteIcon.addFile(":/Images/icons8-delete.png", QSize(25,25));
    pDelete = new QAction(tr("&Delete"), this);
    pDelete->setIcon(DeleteIcon);
    connect(pDelete, &QAction::triggered, this, &Node::delete_node);

    connect(parent->parent(), SIGNAL (generateFpga()), this, SLOT(generate_package()));
    m_parent = parent;

}
M86_Spartan6::~M86_Spartan6()
{
    disconnect(pGenerate, SIGNAL (triggered()),this, SLOT(generate_package()));
    disconnect(pNewFPGA, &QAction::triggered, this, &M86_Spartan6::new_FPGA);
    disconnect(pNewFirmware, &QAction::triggered, this, &M86_Spartan6::new_Firmware);
    disconnect(pDelete, &QAction::triggered, this, &Node::delete_node);
    disconnect(m_parent->parent(), SIGNAL (generateFpga()), this, SLOT(generate_package()));
//  qDebug()<< "M86_Spartan object removed";
    delete pGenerate;
    delete pNewFirmware;
    delete pNewFPGA;
    delete pDelete;
}

FileString M86_Spartan6::location()
{
    return m_location;
}

void M86_Spartan6::setLocation(FileString foldername)
{
    m_location = foldername;
}

QString M86_Spartan6::pkgName()
{
    return m_pkgName;
}

void M86_Spartan6::setPkgName(QString pkgname)
{
    m_pkgName = pkgname;
}

QString M86_Spartan6::ver_major()
{
   return m_ver_major;
}

void M86_Spartan6::setVer_major(QString ver_major)
{
    m_ver_major = ver_major;
}

QString M86_Spartan6::ver_minor()
{
    return m_ver_minor;
}

void M86_Spartan6::setVer_minor(QString ver_minor)
{
    m_ver_minor = ver_minor;
}

QString M86_Spartan6::ver_subminor()
{
    return m_ver_subminor;
}

void M86_Spartan6::setVer_subminor(QString ver_subminor)
{
    m_ver_subminor = ver_subminor;
}

VerState M86_Spartan6::verstate()
{
    return m_verstate;
}

QString M86_Spartan6::getVerString()
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

void M86_Spartan6::setVerstate(VerState verstate)
{
    m_verstate = verstate;
}
void M86_Spartan6::node_menue(QMenu *menu)
{
    menu->addAction(pGenerate);
    menu->addSeparator();
    menu->addAction(pNewFPGA);
    menu->addAction(pNewFirmware);
    menu->addSeparator();
    menu->addAction(pDelete);
    menu->addSeparator();
}

void M86_Spartan6::new_FPGA()
{
    Fpga *m = new Fpga(this);
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("FPGA");
    m->setType("FPGA");
    this->setChild(this->rowCount(),m);
}

void M86_Spartan6::new_Firmware()
{
    firmware *m = new firmware(this);
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("new firmware");
    m->setType("firmware");
    this->setChild(this->rowCount(),m);
}


bool M86_Spartan6::generate_package()
{
    srec_wrapper srecRun;
    QStringList parameters;
    srecRun.setSrecExe(m_parent->property("srec_cat").value<FileString>().filestring);
    setLocation(this->property("location").value<FileString>().filestring);
//  qDebug() << (QString)location().filestring;
//  qDebug () << this->getVerString();
    this->setVerFileName();
//  qDebug() <<"M86 ver file name is " << this->getVerFileName();

    if (!versionFileCreate(this->getVerFileName(), this->getVerString()))
    {
        qDebug() << "error by ver file creating";
        return 0;
    }

    QObjectList childrenOfSpartan = this->children();
    QString dlgOut;
    QMessageBox msgBox;

        if (!srecRun.runSrec(childrenOfSpartan))
        {
            dlgOut.append(srecRun.getRuncmd());
            dlgOut.append(srecRun.getOutput());
            qDebug() << dlgOut;
            msgBox.setText("Generate m86 with following parameters");
            msgBox.setIcon(QMessageBox::Information);
        }
        else
        {
            dlgOut.append("error by generating hex file: ");
            dlgOut.append(srecRun.getRuncmd());
            dlgOut.append(srecRun.getOutput());
            msgBox.setText("error by generating hex file");
            msgBox.setIcon(QMessageBox::Critical);
        }

//      if (versionFileCreate())

    msgBox.setDetailedText(dlgOut);
//  msgBox.setStyleSheet("QLabel{min-width: 500px}");
    msgBox.exec();

//  qDebug() << "ver_major : " << this->property("ver_major").toString();
//  FileString itsFileName = this->location();
//  qDebug() << "file_name = " << itsFileName.filestring;

    return true;
}

