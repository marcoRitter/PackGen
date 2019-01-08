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

FlashSize Masterfile::flash_size()
{
    return m_flashsize;
}

void Masterfile::setFlash_size(FlashSize flashsize)
{
    m_flashsize = flashsize;
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
    jsonVal = jsonObj->value("flash_size");
    FlashSize f;
    f.selectedsize = static_cast<uint>(jsonVal.toInt());
    setFlash_size(f);

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
    jsonObj->insert("flash_size",static_cast<int>(flash_size().selectedsize));
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
    file *m = new file(this);
    Model *m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("new FPGA");
    m->setObject_name("FPGA");
    this->setChild(this->rowCount(),m);
}

void Masterfile::new_Firmware()
{
    file *m = new file(this);
    Model *m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("new firmware");
    m->setObject_name("Firmware");
    this->setChild(this->rowCount(),m);
}

void Masterfile::new_File()
{
    file *m = new file(this);
    Model *m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("new File");
    m->setObject_name("File");
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
    else if(m_filename.contains(".")||m_filename.contains(",")||m_filename.contains(";")||m_filename.contains(":")
            ||m_filename.contains("<") || m_filename.contains(">")||m_filename.contains("!")||m_filename.contains("§")
            ||m_filename.contains("$")||m_filename.contains("%")||m_filename.contains("&")||m_filename.contains("/")
            ||m_filename.contains("{") || m_filename.contains("(")||m_filename.contains("[")||m_filename.contains(")")
            ||m_filename.contains("]")||m_filename.contains("=")||m_filename.contains("}")||m_filename.contains("?")
            ||m_filename.contains("`")||m_filename.contains("´")||m_filename.contains("*"))
    {
        setOutInfo("Masterfile Property filename has an invalid value", m_errorColor);
        return false;
    }
    if(m_location.filestring.isEmpty())
    {
        setOutInfo("Masterfile Property location has no value", m_errorColor);
        return false;
    }
    if(m_ver_major.isEmpty())
    {
        setOutInfo("Masterfile Property ver_major has no value", m_errorColor);
    }
    if(m_ver_minor.isEmpty())
    {
        setOutInfo("Masterfile Property ver_minor has no value", m_errorColor);
    }
    if(m_ver_subminor.isEmpty())
    {
        setOutInfo("Masterfile Property ver_subminor has no value", m_errorColor);
    }
    if(m_flashsize.memorysize.isEmpty())
    {
        setOutInfo("Masterfile Property flashsize has no value", m_errorColor);
        return false;
    }


    QObjectList objectsMasterfile = this->children();
    for(int i = 0; i < this->children().length(); i++)
    {
        if(objectsMasterfile[i]->inherits("file"))
        {
            file *fiLe = static_cast<file*>(objectsMasterfile[i]);

            if(fiLe->filename().filestring.isEmpty())
            {
                setOutInfo("File Property filename has no value (Masterfile)", m_errorColor);
                return false;
            }
            if(fiLe->version().isEmpty())
            {
                setOutInfo("File Property version has no value (Masterfile)", m_errorColor);
            }
            if(fiLe->start_addr().isEmpty())
            {
                setOutInfo("File Property start_addr has no value (Masterfile)", m_errorColor);
                return false;
            }


            srec_parameters.append(fiLe->filename().filestring);
            if(fiLe->filename().filestring.contains(".txt") || fiLe->filename().filestring.contains(".rtf")
                    || fiLe->filename().filestring.contains(".doc") || fiLe->filename().filestring.contains(".bin")
                    || fiLe->filename().filestring.contains(".bmp") || fiLe->filename().filestring.contains(".bit"))
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
    }

    srec_parameters.append("--fill");
    srec_parameters.append("0xFF");
    srec_parameters.append("0x00000000");
    switch (m_flashsize.selectedsize) {
        case 0:
            srec_parameters.append("0x00003333");
            break;
        case 1:
            srec_parameters.append("0x00006666");
            break;
        case 2:
            srec_parameters.append("0x0000CCCC");
            break;
        case 3:
            srec_parameters.append("0x00019999");
            break;
        case 4:
            srec_parameters.append("0x00033333");
            break;
        case 5:
            srec_parameters.append("0x00066666");
            break;
        case 6:
            srec_parameters.append("0x000CCCCC");
            break;
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
