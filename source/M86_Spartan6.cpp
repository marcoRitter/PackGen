#include <QDebug>
#include "M86_Spartan6.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QProcess>
#include <output.h>
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

    connect(this,SIGNAL(setOutInfo(QString, QColor)),parent->parent(), SLOT(printOutInfo(QString, QColor)));
    m_parent = parent;

}
M86_Spartan6::~M86_Spartan6()
{
    disconnect(pGenerate, SIGNAL (triggered()),this, SLOT(generate_package()));
    disconnect(pNewFPGA, &QAction::triggered, this, &M86_Spartan6::new_FPGA);
    disconnect(pNewFirmware, &QAction::triggered, this, &M86_Spartan6::new_Firmware);
    disconnect(pDelete, &QAction::triggered, this, &Node::delete_node);
    disconnect(m_parent->parent(), SIGNAL (generateFpga()), this, SLOT(generate_package()));
    disconnect(this,SIGNAL(setOutInfo(QString, QColor)),m_parent->parent(), SLOT(printOutInfo(QString, QColor)));
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

QString M86_Spartan6::typecode()
{
    return m_typecode;
}

void M86_Spartan6::setTypeCode(QString typeCode)
{
    m_typecode = typeCode;
}

void M86_Spartan6::setVariant(QString variant)
{
    m_variant = variant;
}
QString M86_Spartan6::variant()
{
    return m_variant;
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
    ver.append(verstate().verstate.takeAt(verstate().selectedVersion));
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
    QStringList parameters;



    setLocation(this->property("location").value<FileString>().filestring);
    this->setVerFileName();
    this->setScrFileName();

    if (!versionFileCreate(this->getVerFileName(), this->getVerString()))
    {
        setOutInfo("Package Ver file was not created:", m_errorColor);
        return 0;
    } else {
        setOutInfo("Package Ver file created:", m_infoColor);
        setOutInfo(this->getVerFileName(), m_normalColor);
    }

    if (!scriptFileCreate(this->getScrFileName(), "ModuleType = IOFW\n", true))
    {
        setOutInfo("Script file was not created:", m_errorColor);
        return 0;
    }

    QString scriptLine = "Version = ";
    scriptLine.append('"');
    scriptLine.append(this->getVerFileName());
    scriptLine.append('"');
    scriptLine.append("\n");

    if(!scriptFileCreate(this->getScrFileName(), scriptLine, false))
    {
        setOutInfo("Error by script file creating", m_errorColor);
        return 0;
    }


    for (auto const & childsOfM86 : this->children())
    {
        if (childsOfM86->objectName()=="FPGA")
        {
            Fpga *fpga = (Fpga*) childsOfM86;
            fpga->setHexFileName(m_location.filestring);
            fpga->setSrecParameters();
            fpga->setVariant(this->variant());
            fpga->setTypecode(this->typecode());
            if (!versionFileCreate(fpga->getVerFileName(),fpga->getVerString()))
            {
                setOutInfo("Error by FPGA Ver file creating", m_errorColor);
                return 0;
            }
            setOutInfo("FPGA Ver file created:", m_infoColor);
            setOutInfo(fpga->getVerFileName(), m_normalColor);
            if (!fpga->runSrec())
            {
                setOutInfo(fpga->getProcessOut(),m_infoColor);
                QFileInfo hexFile = fpga->getHexFileName();
                if (hexFile.exists() && hexFile.isFile())
                {
                    setOutInfo("FPGA bin to hex converted successfully:", m_infoColor);
                    setOutInfo(fpga->getHexFileName(), m_normalColor);
                    fpga->runLogichdr();
                    hexFile = fpga->getMchFileName();
                    setOutInfo(fpga->getProcessOut(),m_infoColor);
                    if (hexFile.exists() && hexFile.isFile())
                    {
                        setOutInfo("FPGA mch file created:", m_infoColor);
                        setOutInfo(fpga->getMchFileName(), m_normalColor);
                    }
                }
            }

            scriptFileCreate(this->getScrFileName(), "ObjectType = LOGIC\n", false);
            QString fpgaScriptLine = "Version = \"";
            fpgaScriptLine.append(fpga->getVerFileName());
            fpgaScriptLine.append('"');
            fpgaScriptLine.append("\n");
            scriptFileCreate(this->getScrFileName(),fpgaScriptLine,false);
            fpgaScriptLine = "ObjectName = \"";
            fpgaScriptLine.append(fpga->getMchFileName());
            fpgaScriptLine.append('"');
            fpgaScriptLine.append("\n");
            scriptFileCreate(this->getScrFileName(),fpgaScriptLine,false);
        }
        if (childsOfM86->objectName()=="firmware")
        {
            firmware *fw = (firmware*)childsOfM86;
            fw->setVerFileName(m_location.filestring);
            fw->setVariant(this->variant());
            fw->setTypecode(this->typecode());
            if (!versionFileCreate(fw->getVerFileName(),fw->getVerString()))
            {
                qDebug() << "error by ver file creating";
                return 0;
            }
            fw->runLogichdr();
            QFileInfo hexFile = fw->getMchFileName();
            setOutInfo(fw->getProcessOut(),m_infoColor);

            if (hexFile.exists() && hexFile.isFile())
            {
                setOutInfo("Firmware mch file created:", m_infoColor);
                setOutInfo(fw->getMchFileName(), m_normalColor);
            }

            scriptFileCreate(this->getScrFileName(), "ObjectType = FIRMWARE\n", false);
            QString sfirmware = "Version = \"";
            sfirmware.append(fw->getVerFileName());
            sfirmware.append('"');
            sfirmware.append("\n");
            scriptFileCreate(this->getScrFileName(),sfirmware,false);
            sfirmware = "ObjectName = \"";
            sfirmware.append(fw->getMchFileName());
            sfirmware.append('"');
            sfirmware.append("\n");
            scriptFileCreate(this->getScrFileName(),sfirmware,false);
        }
    }

    setOutInfo("Script file created:", m_infoColor);
    setOutInfo(this->getScrFileName(), m_normalColor);

    this->runMbind();

    return true;
}

int M86_Spartan6::runMbind()
{
    QString mbindExe = m_parent->property("mbind").value<FileString>().filestring;
    qDebug() << "path to mbind" << mbindExe;

    QString outfilename = "";
    outfilename.append(m_location.filestring + "/");
    outfilename.append(m_pkgName);
    QStringList parameters;
    QString scritFile = "";
    parameters.clear();
    parameters << outfilename;
    parameters << "/c";
    scritFile.append(m_scriptFileName);
    parameters << scritFile;// "\"" << m_scriptFileName << "\"";

    for (auto  entity: parameters) {
        qDebug() << "mbind parameters = " << entity;
    }

    QProcess *process = new QProcess(0);
    process->start(mbindExe, parameters,QIODevice::ReadWrite);
    if (!process->waitForStarted())
        qDebug() << "error by executing srec_cat";
    if (!process->waitForFinished())
        qDebug() << "srec_cat failed";
    qDebug() << "end mbind";
    QString mbindOutput = process->readAllStandardOutput();
    qDebug() << "mbind output: " << mbindOutput;

    QFileInfo m86File = outfilename+".m86";
    if (m86File.exists() && m86File.isFile())
    {
        setOutInfo(mbindOutput,m_infoColor);
        setOutInfo("M86 file created successfully", m_infoColor);
        setOutInfo(outfilename + ".m86", m_normalColor);
    } else {

        setOutInfo("ERROR", m_errorColor);
    }


    delete process;

    return 0;
}

