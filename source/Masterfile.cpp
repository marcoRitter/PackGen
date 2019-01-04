#include <QDebug>
#include "Masterfile.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QProcess>
#include <output.h>
#include "version_file.h"

Masterfile::Masterfile(QObject *parent) :
    Node(parent,"Masterfile")
{

    pGenerate = new QAction(tr("Generate"), this);
    connect(pGenerate, SIGNAL(triggered()), this, SLOT(generate_masterfile()));

    pDelete = new QAction(tr("&Delete"), this);
    connect(pDelete, &QAction::triggered, this, &Node::delete_node);

    pNewFPGA = new QAction(tr("&New FPGA"), this);
    connect(pNewFPGA, &QAction::triggered, this, &Masterfile::new_FPGA);

    pNewFirmware = new QAction(tr("New &Firmware"), this);
    connect(pNewFirmware,&QAction::triggered, this, &Masterfile::new_Firmware);

    pNewFile = new QAction(tr("New &File"), this);
    connect(pNewFile,&QAction::triggered, this, &Masterfile::new_File);

    connect(this,SIGNAL(setOutInfo(QString, QColor)),parent->parent(), SLOT(printOutInfo(QString, QColor)));

    connect(parent->parent(), SIGNAL (generateFpga()), this, SLOT(generate_masterfile()));

    m_parent = parent;
}
Masterfile::~Masterfile()
{
    disconnect(pGenerate, SIGNAL (triggered()),this, SLOT(generate_masterfile()));
    disconnect(pNewFPGA, &QAction::triggered, this, &Masterfile::new_FPGA);
    disconnect(pNewFirmware, &QAction::triggered, this, &Masterfile::new_Firmware);
    disconnect(pNewFirmware, &QAction::triggered, this, &Masterfile::new_File);
    disconnect(pDelete, &QAction::triggered, this, &Node::delete_node);
    disconnect(m_parent->parent(), SIGNAL (generateFpga()), this, SLOT(generate_masterfile()));
    disconnect(this,SIGNAL(setOutInfo(QString, QColor)),m_parent->parent(), SLOT(printOutInfo(QString, QColor)));

    delete pGenerate;
    delete pNewFirmware;
    delete pNewFPGA;
    delete pNewFile;
    delete pDelete;
}
QString Masterfile::filename()
{
    return m_filename;
}

void Masterfile::setFilename(QString name)
{
    m_filename = name;
}

FileString Masterfile::location()
{
    return m_location;
}

void Masterfile::setLocation(FileString foldername)
{
    m_location = foldername;
}

QString Masterfile::ver_major()
{
    return m_ver_major;
}

void Masterfile::setVer_major(QString ver_major)
{
    m_ver_major = ver_major;
}

QString Masterfile::ver_minor()
{
    return m_ver_minor;
}

void Masterfile::setVer_minor(QString ver_minor)
{
    m_ver_minor = ver_minor;
}

QString Masterfile::ver_subminor()
{
    return m_ver_subminor;
}

void Masterfile::setVer_subminor(QString ver_subminor)
{
    m_ver_subminor = ver_subminor;
}



bool Masterfile::readJson(const QJsonObject *jsonObj)
{
    QJsonValue jsonVal;

    jsonVal = jsonObj->value("description");
    setDescription(jsonVal.toString());
    jsonVal = jsonObj->value("filename");
    setFilename(jsonVal.toString());
    jsonVal = jsonObj->value("location");
    setLocation(jsonVal.toString());
    jsonVal = jsonObj->value("ver_major");
    setVer_major(jsonVal.toString());
    jsonVal = jsonObj->value("ver_minor");
    setVer_minor(jsonVal.toString());
    jsonVal = jsonObj->value("ver_subminor");
    setVer_subminor(jsonVal.toString());

    return true;
}

bool Masterfile::writeJson(QJsonObject *jsonObj)
{
    jsonObj->insert("node_type",node_type());
    jsonObj->insert("description",description());
    jsonObj->insert("filename",filename());
    jsonObj->insert("ver_major",ver_major());
    jsonObj->insert("ver_minor",ver_minor());
    jsonObj->insert("ver_subminor",ver_subminor());

    return true;
}

void Masterfile::node_menue(QMenu *menu)
{


    menu->addAction(pGenerate);
    menu->addSeparator();
    menu->addAction(pNewFPGA);
    menu->addAction(pNewFirmware);
    menu->addAction(pNewFile);
    menu->addSeparator();
    menu->addAction(pDelete);

}


void Masterfile::new_FPGA()
{
    Fpga *m = new Fpga(this);
    Model *m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("new FPGA");
    m->setType("FPGA");
    this->setChild(this->rowCount(),m);
}

void Masterfile::new_Firmware()
{
    firmware *m = new firmware(this);
    Model *m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("new firmware");
    m->setType("firmware");
    this->setChild(this->rowCount(),m);
}

void Masterfile::new_File()
{
    file *m = new file(this);
    Model *m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("new File");
    m->setType("File");
    this->setChild(this->rowCount(),m);
}

bool Masterfile::setSrecParameters()
{
    QStringList srec_parameters;

    srec_parameters.clear();


    if(m_filename == "")
    {
        setOutInfo("Masterfile Property filename has no value", m_errorColor);
        return false;
    }
    if(m_filename.contains(".")||m_filename.contains(",")||m_filename.contains(";")||m_filename.contains(":")
            ||m_filename.contains("<") || m_filename.contains(">")||m_filename.contains("!")||m_filename.contains("§")
            ||m_filename.contains("$")||m_filename.contains("%")||m_filename.contains("&")||m_filename.contains("/")
            ||m_filename.contains("{") || m_filename.contains("(")||m_filename.contains("[")||m_filename.contains(")")
            ||m_filename.contains("]")||m_filename.contains("=")||m_filename.contains("}")||m_filename.contains("?")
            ||m_filename.contains("`")||m_filename.contains("´")||m_filename.contains("*"))
    {
        setOutInfo("Masterfile Property filename has an invalid value", m_errorColor);
        return false;
    }
    if(m_location.filestring == "")
    {
        setOutInfo("Masterfile Property location has no value", m_errorColor);
        return false;
    }
    if(m_ver_major == "")
    {
        setOutInfo("Masterfile Property ver_major has no value", m_errorColor);
    }
    if(m_ver_minor == "")
    {
        setOutInfo("Masterfile Property ver_minor has no value", m_errorColor);
    }
    if(m_ver_subminor == "")
    {
        setOutInfo("Masterfile Property ver_subminor has no value", m_errorColor);
    }


    QObjectList objectsMasterfile = this->children();
    for(int i = 0; i < this->children().length(); i++)
    {
        if(objectsMasterfile[i]->inherits("firmware"))
        {
            firmware *firm = static_cast<firmware*>(objectsMasterfile[i]);


            if(firm->filename().filestring == "")
            {
                setOutInfo("Firmware Property filename has no value (Masterfile)", m_errorColor);
                return false;
            }
            if(firm->ver_major() == "")
            {
                setOutInfo("Firmware Property ver_major has no value (Masterfile)", m_errorColor);
            }
            if(firm->ver_minor() == "")
            {
                setOutInfo("Firmware Property ver_minor has no value (Masterfile)", m_errorColor);
            }
            if(firm->ver_subminor() == "")
            {
                setOutInfo("Firmware Property ver_subminor has no value (Masterfile)", m_errorColor);
            }
            if(firm->start_addr() == "")
            {
                setOutInfo("Firmware Property start_addr has no value (Masterfile)", m_errorColor);
                return false;
            }

            srec_parameters.append(firm->filename().filestring);
            srec_parameters.append("--binary");
            srec_parameters.append("--offset");
            srec_parameters.append(firm->start_addr());
        }
        else if(objectsMasterfile[i]->inherits("file"))
        {
            file *fiLe = static_cast<file*>(objectsMasterfile[i]);

            if(fiLe->filename().filestring == "")
            {
                setOutInfo("File Property filename has no value (Masterfile)", m_errorColor);
                return false;
            }
            if(fiLe->version() == "")
            {
                setOutInfo("File Property version has no value (Masterfile)", m_errorColor);
            }
            if(fiLe->start_addr() == "")
            {
                setOutInfo("File Property start_addr has no value (Masterfile)", m_errorColor);
                return false;
            }


            srec_parameters.append(fiLe->filename().filestring);
            if(fiLe->filename().filestring.contains(".txt") || fiLe->filename().filestring.contains(".rtf")
                    || fiLe->filename().filestring.contains(".doc") || fiLe->filename().filestring.contains(".bin")
                    || fiLe->filename().filestring.contains(".bmp"))
            {
                srec_parameters.append("--binary");
            }
            else if(fiLe->filename().filestring.contains(".hex") || fiLe->filename().filestring.contains(".h86"))
            {
                srec_parameters.append("--intel");
            }
            srec_parameters.append("--offset");
            srec_parameters.append(fiLe->start_addr());
        }
        else if(objectsMasterfile[i]->inherits("Fpga"))
        {
            Fpga *fpga = static_cast<Fpga*>(objectsMasterfile[i]);

            if(fpga->filename().filestring == "")
            {
                setOutInfo("FPGA Property filename has no value (Masterfile)", m_errorColor);
                return false;
            }
            if(fpga->designnumber() == "")
            {
                setOutInfo("File Property designnumber has no value (Masterfile)", m_errorColor);
            }
            if(fpga->revision() == "")
            {
                setOutInfo("File Property revision has no value (Masterfile)", m_errorColor);
            }
            if(fpga->testversion() == "")
            {
                setOutInfo("File Property testversion has no value (Masterfile)", m_errorColor);
            }
            if(fpga->start_addr() == "")
            {
                setOutInfo("FPGA Property start_addr has no value (Masterfile)", m_errorColor);
                return false;
            }

            srec_parameters.append(fpga->filename().filestring);
            srec_parameters.append("--binary");
            srec_parameters.append("--offset");
            srec_parameters.append(fpga->start_addr());
        }

    }

    srec_parameters.append("--o");
    srec_parameters.append(m_location.filestring + "/" + m_filename+".hex");
    srec_parameters.append("--intel");

    m_srecParameters = srec_parameters;
    return true;
}

int Masterfile::runSrec()
{
    QString srecExe = m_parent->property("srec_cat").value<FileString>().filestring;
    //QString srecExe = "//pc011/tools/utils/srec_cat.exe";

    qDebug() << "srec EXE = " << srecExe;

    QProcess *process = new QProcess();

    qDebug() << "srec startet with " << m_srecParameters;

    process->start(srecExe,m_srecParameters,QIODevice::ReadWrite);

    if (!process->waitForStarted())
        qDebug() << "error by executing srec_cat";
    if (!process->waitForFinished())
        qDebug() << "srec_cat failed";

    m_processOut = process->readAllStandardOutput();
//  qDebug() <<"srec returned " << process->errorString();

    delete process;
    QFileInfo hexFile = m_location.filestring + "/" + m_filename + ".hex";
    if (!(hexFile.exists() && hexFile.isFile()))
    {
        qDebug() << "error by hex file";
        setOutInfo("Problems while creating Masterfile", m_errorColor);
        return 1;
    }
    else {
        setOutInfo("Created Masterfile successfully:", m_infoColor);
        setOutInfo(m_location.filestring + "/" + m_filename + ".hex", m_normalColor);
    }

    return 0;
}

bool Masterfile::generate_masterfile()
{
    if(setSrecParameters())
    {
        if(!runSrec())
        {
            return false;
        }
    }
    else {
        return false;
    }
    return true;

}

