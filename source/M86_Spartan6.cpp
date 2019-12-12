#include <QDebug>
#include "M86_Spartan6.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QProcess>
#include <output.h>
#include "version_file.h"
#include <QDir>


M86_Spartan6::M86_Spartan6(QObject *parent) :
    Node(parent,"M86")
{

    QIcon GenerateIcon;
    QIcon NewFpgaIcon;
    QIcon DeleteIcon;

    this->setObjectName("m86");

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

    delete pGenerate;
    delete pNewFirmware;
    delete pNewFPGA;
    delete pDelete;
}

ModuleType M86_Spartan6::module_type()
{
    return m_module_type;
}

void M86_Spartan6::setModule_type(ModuleType module)
{
    m_module_type = module;
}

QString M86_Spartan6::location()
{
    return m_location;
}

void M86_Spartan6::setLocation(QString foldername)
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

QString M86_Spartan6::getlocation()
{
    QString temp = m_location;

    if(temp.remove("xHOME/") == "")
    {
        return m_parent->property("working_directory").value<FileString>().filestring;
    }
    else {
        return m_parent->property("working_directory").value<FileString>().filestring + "/" + temp.remove("xHOME/");
    }
}

QString M86_Spartan6::getVerString()
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

void M86_Spartan6::setScrFileName()
{
    FileString fn = getlocation() + "/" + "temp";
    QString filenm = "";
    filenm.append(fn.filestring);
    filenm.append("/");
    filenm.append(pkgName());
    filenm.append(".mbs");
    m_scriptFileName = filenm;
}

void M86_Spartan6::setVerFileName()
{
    FileString fn = getlocation() + "/" + "temp";
    QString filenm = (fn.filestring);
    filenm.append("/");
    filenm.append(pkgName());
    filenm.append(".ver");
    m_verFileName = filenm;
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
    m->setDescription("new Firmware");
    m->setType("Firmware");
    this->setChild(this->rowCount(),m);
}



bool M86_Spartan6::generate_package()
{
    QStringList parameters;

    QDir dir(getlocation());
    QDir dir_temp(getlocation()+"/temp");
    if(dir.exists())
    {
    }
    else {
        dir.mkpath(getlocation());
    }
    if(dir_temp.exists())
    {
        dir_temp.removeRecursively();
        dir_temp.mkpath(getlocation()+"/temp");
    }
    else {
        dir_temp.mkpath(getlocation()+"/temp");
    }


   // setLocation(this->property("outputFile_location").value<FileString>().filestring);
    setVerFileName();
    setScrFileName();

    if(m_pkgName.isEmpty())
    {
        setOutInfo("M86 Property pkgName has no value", m_errorColor);
        return false;
    }
    else if(m_pkgName.contains(".")||m_pkgName.contains(",")||m_pkgName.contains(";")||m_pkgName.contains(":")
            ||m_pkgName.contains("<") || m_pkgName.contains(">")||m_pkgName.contains("!")||m_pkgName.contains("§")
            ||m_pkgName.contains("$")||m_pkgName.contains("%")||m_pkgName.contains("&")||m_pkgName.contains("/")
            ||m_pkgName.contains("{") || m_pkgName.contains("(")||m_pkgName.contains("[")||m_pkgName.contains(")")
            ||m_pkgName.contains("]")||m_pkgName.contains("=")||m_pkgName.contains("}")||m_pkgName.contains("?")
            ||m_pkgName.contains("`")||m_pkgName.contains("´")||m_pkgName.contains("*"))
    {
        setOutInfo("M86 Property pkgName has an invalid value", m_errorColor);
        return false;
    }
    //if(m_location.isEmpty())
    //{
    //    setOutInfo("M86 Property location has no value", m_errorColor);
    //    return false;
   // }
    if(m_typecode.isEmpty())
    {
        setOutInfo("M86 Property typecode has no value", m_errorColor);
    }
    if(m_variant.isEmpty())
    {
        setOutInfo("M86 Property variant has no value", m_errorColor);
    }
    if(m_ver_major.isEmpty())
    {
        setOutInfo("M86 Property ver_major has no value", m_errorColor);
    }
    if(m_ver_minor.isEmpty())
    {
        setOutInfo("M86 Property ver_minor has no value", m_errorColor);
    }
    if(m_ver_subminor.isEmpty())
    {
        setOutInfo("M86 Property ver_minor has no value", m_errorColor);
    }


    QObjectList objectsM86 = this->children();
    for(int i = 0; i < this->children().length(); i++)
    {
        if(objectsM86[i]->inherits("firmware"))
        {
            firmware *firm = static_cast<firmware*>(objectsM86[i]);
            QString temp_file = firm->filename();
            QFileInfo  dir_file = m_parent->property("working_directory").value<FileString>().filestring + "/" + temp_file.remove("xHOME/");

            if(firm->filename().isEmpty())
            {
                setOutInfo("Firmware Property filename has no value (M86)", m_errorColor);
                return false;
            }

            if(!dir_file.exists())
            {
                setOutInfo("Firmware Property inputFile_directory doesn`t exist (M86)", m_errorColor);
                return false;
            }

            if(firm->ver_major().isEmpty())
            {
                setOutInfo("Firmware Property ver_major has no value (M86)", m_errorColor);
                return false;
            }
            if(firm->ver_minor().isEmpty())
            {
                setOutInfo("Firmware Property ver_minor has no value (M86)", m_errorColor);
                return false;
            }
            if(firm->ver_subminor().isEmpty())
            {
                setOutInfo("Firmware Property ver_subminor has no value (M86)", m_errorColor);
                return false;
            }
            if(firm->start_addr().isEmpty())
            {
                setOutInfo("Firmware Property start_addr has no value (M86)", m_errorColor);
                return false;
            }
        }
        else if(objectsM86[i]->inherits("Fpga"))
        {
            Fpga *fpga = static_cast<Fpga*>(objectsM86[i]);
            QString temp_file = fpga->filename();
            QFileInfo  dir_file = m_parent->property("working_directory").value<FileString>().filestring + "/" + temp_file.remove("xHOME/");
            if(fpga->filename().isEmpty())
            {
                setOutInfo("FPGA Property filename has no value (M86)", m_errorColor);
                return false;
            }

            if(!dir_file.exists())
            {
                setOutInfo("FPGA Property inputFile_directory doesn`t exist (M86)", m_errorColor);
                return false;
            }
            if(fpga->designnumber().isEmpty())
            {
                setOutInfo("File Property designnumber has no value (M86)", m_errorColor);
                return false;
            }
            if(fpga->revision().isEmpty())
            {
                setOutInfo("File Property revision has no value (M86)", m_errorColor);
                return false;
            }
            if(fpga->testversion().isEmpty())
            {
                setOutInfo("File Property testversion has no value (M86)", m_errorColor);
                return false;
            }
            if(fpga->start_addr().isEmpty())
            {
                setOutInfo("FPGA Property start_addr has no value (M86)", m_errorColor);
                return false;
            }
        }
    }


    if (!versionFileCreate(getVerFileName(), getVerString()))
    {
        setOutInfo("Package Ver file was not created:", m_errorColor);
        return 0;
    } else {
        setOutInfo("Package Ver file created:", m_infoColor);
        setOutInfo(this->getVerFileName(), m_normalColor);
    }

    if (!scriptFileCreate(getScrFileName(), "ModuleType = IOFW\n", true))
    {
        setOutInfo("Script file was not created:", m_errorColor);
        return 0;
    }

    QString scriptLine = "Version = ";
    scriptLine.append('"');
    scriptLine.append(getVerFileName());
    scriptLine.append('"');
    scriptLine.append("\n");

    if(!scriptFileCreate(getScrFileName(), scriptLine, false))
    {
        setOutInfo("Error by script file creating", m_errorColor);
        return 0;
    }


    for (auto const & childsOfM86 : this->children())
    {
        if (childsOfM86->objectName()=="FPGA")
        {
            Fpga *fpga = static_cast<Fpga*>(childsOfM86);
            QString temp_hex = getlocation();
            fpga->setHexFileName(temp_hex);
            fpga->setSrecParameters();
            fpga->setVariant(variant());
            fpga->setTypecode(typecode());
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
            firmware *fw = static_cast<firmware*>(childsOfM86);
            QString temp_fw = getlocation();
            fw->setVerFileName(temp_fw);
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
    outfilename.append(getlocation() + "/");
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

    QProcess *process = new QProcess(nullptr);
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

