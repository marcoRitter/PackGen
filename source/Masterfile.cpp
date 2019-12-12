#include <QDebug>
#include "Masterfile.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QProcess>
#include <output.h>
#include "version_file.h"
#include <QDir>
#include <QDateTime>
Masterfile::Masterfile(QObject *parent) :
    Node(parent,"Masterfile")
{
    this->setObjectName("master");

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

    connect(this, SIGNAL(need_redraw(QString ,  QVariant )),
           parent->parent(),SLOT(changeProperty ( QString  ,QVariant )));

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
    disconnect(this, SIGNAL(need_redraw(QString ,  QVariant )),
           m_parent->parent(),SLOT(changeProperty ( QString  ,QVariant )));

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

FpgaType Masterfile::fpgatype()
{
    return m_fpgatype;
}
void Masterfile::setFpgatype(FpgaType fpgatype)
{
    m_fpgatype = fpgatype;
    if(m_fpgatype.selectedfpga == 0)
    {
        QVariant a = false;
        need_redraw("bit_reverse",a);
    }
}

FlashSize Masterfile::flash_size()
{
    return m_flashsize;
}

void Masterfile::setFlash_size(FlashSize flashsize)
{
    m_flashsize = flashsize;
}

QString Masterfile::location()
{
    return m_location;
}

void Masterfile::setLocation(QString foldername)
{
    int pos = foldername.lastIndexOf(QChar('/'));
    int pos2 = foldername.length();

    if(pos+1 == pos2)
    {
        m_location = foldername.left(pos);
    }
    else {
        m_location = foldername;
    }
}


bool Masterfile::bit_reverse()
{
    return m_bit_reverse;
}

void Masterfile::setBit_reverse(bool reverse)
{
    m_bit_reverse = reverse;

    if(m_fpgatype.selectedfpga == 0)
    {
        QVariant a = false;
        need_redraw("bit_reverse",a);
    }

}

bool Masterfile::readJson(const QJsonObject *jsonObj)
{
    QJsonValue jsonVal;

    jsonVal = jsonObj->value("description");
    setDescription(jsonVal.toString());
    jsonVal = jsonObj->value("output_file_name");
    setFilename(jsonVal.toString());
    jsonVal = jsonObj->value("output_file_directory");
    setLocation(jsonVal.toString());
    jsonVal = jsonObj->value("flash_size");
    FlashSize f;
    f.selectedsize = static_cast<uint>(jsonVal.toInt());
    setFlash_size(f);
    jsonVal = jsonObj->value("bit_reverse");
    setBit_reverse(jsonVal.toBool());
    jsonVal = jsonObj->value("fpgatype");
    FpgaType fp;
    fp.selectedfpga = static_cast<uint>(jsonVal.toInt());
    setFpgatype(fp);

    return true;
}

bool Masterfile::writeJson(QJsonObject *jsonObj)
{
    jsonObj->insert("node_type",node_type());
    jsonObj->insert("description",description());
    jsonObj->insert("output_file_name",filename());
    jsonObj->insert("output_file_directory",location());
    jsonObj->insert("flash_size",static_cast<int>(flash_size().selectedsize));
    jsonObj->insert("bit_reverse",bit_reverse());
    jsonObj->insert("fpgatype",static_cast<int>(fpgatype().selectedfpga));
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

QString Masterfile::get_location()
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
    if(m_location.isEmpty())
    {
        setOutInfo("Masterfile Property location has no value", m_errorColor);
        return false;
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
            QString temp_file = fiLe->filename();
            QFileInfo  dir_file = m_parent->property("working_directory").value<FileString>().filestring + "/" + temp_file.remove("xHOME/");
            if(temp_file.remove("xHOME/") == "")
            {
                setOutInfo("Masterfile Property: " + fiLe->object_name() + ":" +" Property filename has no value", m_errorColor);
                return false;
            }
            if(!dir_file.exists())
            {
                setOutInfo("Masterfile Property: " + fiLe->object_name() + ":" +" Input file doesn`t exist", m_errorColor);
                return false;
            }
            if(fiLe->start_addr().isEmpty())
            {
                setOutInfo("Masterfile Property: " + fiLe->object_name() + ":" +" Property start_addr has no value", m_errorColor);
                return false;
            }

            //if(temp_file.remove("xHOME/") == 0)
           // {
           //     srec_parameters.append(m_parent->property("project_directory").value<FileString>().filestring);
           // }
           // else {
                srec_parameters.append(m_parent->property("working_directory").value<FileString>().filestring + "/" + temp_file.remove("xHOME/"));
          //  }

            if(fiLe->file_type().selectedType == 1)
            {
                srec_parameters.append("--binary");
                if(m_fpgatype.selectedfpga)
                {
                    srec_parameters.append("--bit-reverse");
                }

            }
            else if(fiLe->file_type().selectedType == 0)
            {
                srec_parameters.append("--intel");
                if(m_fpgatype.selectedfpga && (fiLe->getType().toLower() == "file" || fiLe->getType().toLower() == "firmware"))
                {
                    srec_parameters.append("--bit-reverse");
                }
            }

            srec_parameters.append("--offset");
            srec_parameters.append(fiLe->start_addr());
        }
    }


    srec_parameters.append("--o");
    srec_parameters.append(get_location() + "/" + "temp" + "/" + m_filename+"_notfilled"+".hex");
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
    //QFileInfo check = get_location();
    QDir dir(get_location());
    QDir dir_temp(get_location()+"/temp");
    if(dir.exists())
    {
    }
    else {
        dir.mkpath(get_location());
    }
    if(dir_temp.exists())
    {
        dir_temp.removeRecursively();
        dir_temp.mkpath(get_location()+"/temp");
    }
    else {
        dir_temp.mkpath(get_location()+"/temp");
    }
    if(setSrecParameters())
    {
        if(runSrec())
        {
            return false;
        }
        else {


            QFileInfo hexFile = get_location() + "/" + "temp" + "/" + m_filename + "_notfilled" + ".hex";
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
                        hexFile = get_location() + "/" + m_filename+"_bitreverse"+".hex";
                    }
                    else {
                        hexFile = get_location() + "/" + m_filename+".hex";
                    }

                    if (!(hexFile.exists() && hexFile.isFile()))
                    {
                        qDebug() << "error by hex file";
                        setOutInfo("Problems while creating Masterfile", m_errorColor);
                        setOutInfo(m_processOut, m_errorColor);
                    }
                    else {
                        setOutInfo("Created Masterfile successfully:", m_infoColor);
                        setOutInfo(get_location() + "/" + m_filename+".hex", m_normalColor);
                        //QFile file(get_location() + "/" + "temp" + "/" + m_filename + "_notfilled" + ".hex");
                        //file.remove();

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

    srec_parameters.append(get_location() + "/" + "temp" + "/" + m_filename+"_notfilled"+".hex");
    srec_parameters.append("--intel");
    if(!m_bit_reverse && m_fpgatype.selectedfpga)
        srec_parameters.append("--bit-reverse");
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
        srec_parameters.append(get_location() + "/" + m_filename+"_bitreverse"+".hex");
    }
    else {
        srec_parameters.append(get_location() + "/" + m_filename+".hex");
    }
    srec_parameters.append("--intel");
    srec_parameters.append("--obs=16");

    m_srecParameters = srec_parameters;
    return true;
}
