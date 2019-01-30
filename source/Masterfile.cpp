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
    pNewGolden = new QAction(tr("New &GoldenReference"), this);
    connect(pNewGolden,&QAction::triggered, this, &Masterfile::new_Golden);

    connect(this,SIGNAL(setOutInfo(QString, QColor)),parent->parent(), SLOT(printOutInfo(QString, QColor)));

    connect(parent->parent(), SIGNAL (generateFpga()), this, SLOT(generate_masterfile()));

    m_parent = parent;
}
Masterfile::~Masterfile()
{
    disconnect(pGenerate, SIGNAL (triggered()),this, SLOT(generate_masterfile()));
    disconnect(pNewFPGA, &QAction::triggered, this, &Masterfile::new_FPGA);
    disconnect(pNewFirmware, &QAction::triggered, this, &Masterfile::new_Firmware);
    disconnect(pNewFile, &QAction::triggered, this, &Masterfile::new_File);
    disconnect(pDelete, &QAction::triggered, this, &Node::delete_node);
    disconnect(m_parent->parent(), SIGNAL (generateFpga()), this, SLOT(generate_masterfile()));
    disconnect(this,SIGNAL(setOutInfo(QString, QColor)),m_parent->parent(), SLOT(printOutInfo(QString, QColor)));
    disconnect(pNewGolden,&QAction::triggered, this, &Masterfile::new_Golden);

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

bool Masterfile::bit_reverse()
{
    return m_bit_reverse;
}

void Masterfile::setBit_reverse(bool reverse)
{
    m_bit_reverse = reverse;
}

bool Masterfile::readJson(const QJsonObject *jsonObj)
{
    QJsonValue jsonVal;

    jsonVal = jsonObj->value("description");
    setDescription(jsonVal.toString());
    jsonVal = jsonObj->value("outputFile_name");
    setFilename(jsonVal.toString());
    jsonVal = jsonObj->value("outputFile_location");
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
    jsonVal = jsonObj->value("bit_reverse");
    setBit_reverse(jsonVal.toBool());

    return true;
}

bool Masterfile::writeJson(QJsonObject *jsonObj)
{
    jsonObj->insert("node_type",node_type());
    jsonObj->insert("description",description());
    jsonObj->insert("outputFile_name",filename());
    jsonObj->insert("outputFile_location",location().filestring);
    jsonObj->insert("ver_major",ver_major());
    jsonObj->insert("ver_minor",ver_minor());
    jsonObj->insert("ver_subminor",ver_subminor());
    jsonObj->insert("flash_size",static_cast<int>(flash_size().selectedsize));
    jsonObj->insert("bit_reverse",bit_reverse());
    return true;
}

void Masterfile::node_menue(QMenu *menu)
{


    menu->addAction(pGenerate);
    menu->addSeparator();
    menu->addAction(pNewFPGA);
    menu->addAction(pNewFirmware);
    menu->addAction(pNewFile);
    menu->addAction(pNewGolden);
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
    m->setDescription("new Firmware");
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

void Masterfile::new_Golden()
{
    file *m = new file(this);
    Model *m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("new Golden Reference");
    m->setObject_name("Golden File");
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
                setOutInfo(fiLe->object_name()+" Property filename has no value", m_errorColor);
                return false;
            }
            if(fiLe->version().isEmpty())
            {
                setOutInfo(fiLe->object_name()+" Property version has no value", m_errorColor);
            }
            if(fiLe->start_addr().isEmpty())
            {
                setOutInfo(fiLe->object_name()+" Property start_addr has no value", m_errorColor);
                return false;
            }


            srec_parameters.append(fiLe->filename().filestring);
            if(fiLe->file_type().selectedType == 1)
            {
                srec_parameters.append("--binary");
            }
            else if(fiLe->file_type().selectedType == 0)
            {
                srec_parameters.append("--intel");
            }

            if(bit_reverse())
            {
                srec_parameters.append("--bit-reverse");
            }

            srec_parameters.append("--offset");
            srec_parameters.append(fiLe->start_addr());
        }
    }


    srec_parameters.append("--o");
    srec_parameters.append(m_location.filestring + "/" + m_filename+"_notfilled"+".hex");
    srec_parameters.append("--intel");

    srec_parameters.append("--obs=16");

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

    return 0;
}

bool Masterfile::generate_masterfile()
{
    if(setSrecParameters())
    {
        if(runSrec())
        {
            return false;
        }
        else {


            QFileInfo hexFile = m_location.filestring + "/" + m_filename + "_notfilled" + ".hex";
            if (!(hexFile.exists() && hexFile.isFile()))
            {
                qDebug() << "error by hex file";
                setOutInfo("Problems while creating Masterfile", m_errorColor);
                setOutInfo(m_processOut, m_errorColor);
            }
            else {
            if(fillBlanks())
            {
                if(runSrec())
                {
                    return false;
                }
                else {
                    QFileInfo hexFile;
                    if(bit_reverse())
                    {
                        hexFile = m_location.filestring + "/" + m_filename+"_bitreverse"+".hex";
                    }
                    else {
                        hexFile = m_location.filestring + "/" + m_filename+".hex";
                    }

                    if (!(hexFile.exists() && hexFile.isFile()))
                    {
                        qDebug() << "error by hex file";
                        setOutInfo("Problems while creating Masterfile", m_errorColor);
                        setOutInfo(m_processOut, m_errorColor);
                    }
                    else {
                        setOutInfo("Created Masterfile successfully:", m_infoColor);
                        setOutInfo(m_location.filestring + "/" + m_filename+".hex", m_normalColor);
                        QFile file(m_location.filestring + "/" + m_filename + "_notfilled" + ".hex");
                        file.remove();

                    }

                    return true;
                }
            }
            else {
                return false;
            }
            }
        }
    }
    else {
        return false;
    }
    return true;

}


bool Masterfile::fillBlanks()
{
    QStringList srec_parameters;

    srec_parameters.clear();

    srec_parameters.append(m_location.filestring + "/" + m_filename+"_notfilled"+".hex");
    srec_parameters.append("--intel");
    srec_parameters.append("--fill");
    srec_parameters.append("0xFF");
    srec_parameters.append("0x00000000");
    switch (m_flashsize.selectedsize) {
        case 0:
            srec_parameters.append("0x00040000");
            break;
        case 1:
            srec_parameters.append("0x00080000");
            break;
        case 2:
            srec_parameters.append("0x00100000");
            break;
        case 3:
            srec_parameters.append("0x00200000");
            break;
        case 4:
            srec_parameters.append("0x00400000");
            break;
        case 5:
            srec_parameters.append("0x00800000");
            break;
        case 6:
            srec_parameters.append("0x01000000");
            break;
    }

    srec_parameters.append("--o");
    if(bit_reverse())
    {
        srec_parameters.append(m_location.filestring + "/" + m_filename+"_bitreverse"+".hex");
    }
    else {
        srec_parameters.append(m_location.filestring + "/" + m_filename+".hex");
    }
    srec_parameters.append("--intel");
    srec_parameters.append("--obs=16");

    m_srecParameters = srec_parameters;
    return true;
}
