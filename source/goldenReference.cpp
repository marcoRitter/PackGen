#include "goldenReference.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QProcess>
#include <output.h>
#include "version_file.h"
#include <iostream>
#include <fstream>

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

FileString goldenReference::goldenRef_file()
{
    return m_goldenRef_file;
}

void goldenReference::setGoldenRef_file(FileString file)
{
    m_goldenRef_file = file;
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
    if(m_filename.isEmpty())
    {
        setOutInfo("goldenReference Property filename has no value (Masterfile)", m_errorColor);
        return false;
    }
    if(m_location.filestring.isEmpty())
    {
        setOutInfo("goldenReference Property location has no value (Masterfile)", m_errorColor);
        return false;
    }
    if(m_version.isEmpty())
    {
        setOutInfo("goldenReference Property version has no value (Masterfile)", m_errorColor);
    }

    QStringList srecParameters;
    srecParameters.clear();

    srecParameters.append(m_location.filestring+"/"+m_filename+"_header.hex");
    srecParameters.append("--intel");
    srecParameters.append("--offset");
    srecParameters.append("0x0");
    srecParameters.append("--fill");
    srecParameters.append("0x00");
    srecParameters.append("0x00");
    srecParameters.append("0x10000");
    srecParameters.append("--o");
    srecParameters.append(m_location.filestring+"/"+m_filename+"_header_filled.hex");
    srecParameters.append("--intel");
    srecParameters.append("--obs=16");

    m_srecParameters = srecParameters;
    if(runSrec())
    {
        srecParameters.clear();

        srecParameters.append(m_location.filestring+"/"+m_filename+"_header_filled.hex");
        srecParameters.append("--intel");
        srecParameters.append("--offset");
        srecParameters.append("0x0");
        srecParameters.append(m_goldenRef_file.filestring);
        if(m_file_type.selectedType == 0)
        {
            srecParameters.append("--intel");
        }
        else if(m_file_type.selectedType == 1)
        {
            srecParameters.append("--binary");
        }
        srecParameters.append("--offset");
        srecParameters.append("0x10000");
        srecParameters.append("--o");
        srecParameters.append(m_location.filestring+"/"+m_filename+".hex");
        srecParameters.append("--intel");
        srecParameters.append("--obs=16");

        m_srecParameters = srecParameters;
        runSrec();
    }

    return  true;
}

int goldenReference::runSrec()
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

    return 1;
}

void goldenReference::creatHeader()
{
   /* QString head13[] = {"0x00", "0x09", "0x0F", "0xF0", "0x0F", "0xF0", "0x0F", "0xF0", "0x0F", "0xF0", "0x00", "0x00", "0x01"};
    const uint8_t head13_length = 13;*/
    QString sync16[] = {"0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF"};
    const uint8_t sync16_length = 16;
    QString header4[] = { "0xAA", "0x99", "0x55", "0x66" };
    const uint8_t header4_length = 4;
    QString info1_4[] = { "0x31", "0xE1", "0xFF", "0xFF" };
    const uint8_t info1_4_length = 4;
    QString Bootaddr1_2MB_8[] = { "0x32", "0x61", "0x00", "0x00", "0x32", "0x81", "0x03", "0x02" };	    // Working
    const uint8_t Bootaddr1_2MB_8_length = 8;
    QString Bootaddr1_4MB_8[] = { "0x32", "0x61", "0x00", "0x00", "0x32", "0x81", "0x03", "0x04" };	    // Working
    const uint8_t Bootaddr1_4MB_8_length = 8;
    QString Bootaddr1_8MB_8[] = { "0x32", "0x61", "0x00", "0x00", "0x32", "0x81", "0x03", "0x08" };	    // Working
    const uint8_t Bootaddr1_8MB_8_length = 8;
    QString Bootaddr1_16MB_8[] = { "0x32", "0x61", "0x00", "0x00", "0x32", "0x81", "0x03", "0x10" };	    // Working
    const uint8_t Bootaddr1_16MB_8_length = 8;
    QString Bootaddr1_32MB_8[] = { "0x32", "0x61", "0x00", "0x00", "0x32", "0x81", "0x03", "0x20" };	    // Working
    const uint8_t Bootaddr1_32MB_8_length = 8;
    QString Bootaddr1_64MB_8[] = { "0x32", "0x61", "0x00", "0x00", "0x32", "0x81", "0x03", "0x40" };	    // Working
    const uint8_t Bootaddr1_64MB_8_length = 8;
    QString Bootaddr1_128MB_8[] = { "0x32", "0x61", "0x00", "0x00", "0x32", "0x81", "0x03", "0x80" };	    // Working
    const uint8_t Bootaddr1_128MB_8_length = 8;
    QString Bootaddr2_8[] = { "0x32", "0xA1", "0x00", "0x00", "0x32", "0xC1", "0x03", "0x01" };	    // Referenz Design
    const uint8_t Bootaddr2_8_length = 8;
    QString info2_4[] = { "0x32", "0xE1", "0x00", "0x00" };
    const uint8_t info2_4_length = 4;
    QString info3_4[] = { "0x30", "0xA1", "0x00", "0x00" };
    const uint8_t info3_4_length = 4;
    QString info4_4[] = { "0x33", "0x01", "0x21", "0x00" };
    const uint8_t info4_4_length = 4;
    QString cor2_4[] = { "0x31", "0x61", "0x89", "0xEE" };								// Reboot on CRC
    const uint8_t cor2_4_length = 4;
    QString info5_4[] = { "0x32", "0x01", "0x00", "0x1F" };
    const uint8_t info5_4_length = 4;
    QString info6_4[] = { "0x33", "0x81", "0x3C", "0x12" };							    // Auslesetakt 22 MHz
    const uint8_t info6_4_length = 4;
    QString info7_4[] = { "0x30", "0xA1", "0x00", "0x0E" };							    // Restart
    const uint8_t info7_4_length = 4;
    QString NOP_8[] = { "0x20", "0x00", "0x20", "0x00", "0x20", "0x00", "0x20", "0x00" };			// 5*NOP
    const uint8_t NOP_8_lenght = 8;

    QString filenameHeader = m_location.filestring + "/" + m_filename + "_header.hex";
    QFile file(filenameHeader);

    if(file.open(QFile::WriteOnly|QFile::Text))
    {
        QTextStream stream(&file);

        stream << ":20000000";
        for(int i = 0; i < sync16_length; i++)
        {
            stream << sync16[i].remove("0x");
        }
        for(int i = 0; i < header4_length; i++)
        {
            stream << header4[i].remove("0x");
        }
        for(int i = 0; i < info1_4_length; i++)
        {
            stream << info1_4[i].remove("0x");
        }
        QString checkSumLine1;
        switch (m_flash_size.selectedsize) {
        case 0:
            for(int i = 0; i < Bootaddr1_2MB_8_length; i++)
            {
                stream << Bootaddr1_2MB_8[i].remove("0x");
                checkSumLine1 = "97";
            }
            break;
        case 1:
            for(int i = 0; i < Bootaddr1_4MB_8_length; i++)
            {
                stream << Bootaddr1_4MB_8[i].remove("0x");
                checkSumLine1 = "95";
            }
            break;
        case 2:
            for(int i = 0; i < Bootaddr1_8MB_8_length; i++)
            {
                stream << Bootaddr1_8MB_8[i].remove("0x");
                checkSumLine1 = "91";
            }
            break;
        case 3:
            for(int i = 0; i < Bootaddr1_16MB_8_length; i++)
            {
                stream << Bootaddr1_16MB_8[i].remove("0x");
                checkSumLine1 = "89";
            }
            break;
        case 4:
            for(int i = 0; i < Bootaddr1_32MB_8_length; i++)
            {
                stream << Bootaddr1_32MB_8[i].remove("0x");
                checkSumLine1 = "79";
            }
            break;
        case 5:
            for(int i = 0; i < Bootaddr1_64MB_8_length; i++)
            {
                stream << Bootaddr1_64MB_8[i].remove("0x");
                checkSumLine1 = "59";
            }
            break;
        case 6:
            for(int i = 0; i < Bootaddr1_128MB_8_length; i++)
            {
                stream << Bootaddr1_128MB_8[i].remove("0x");
                checkSumLine1 = "19";
            }
            break;
        }
        stream << checkSumLine1;
        stream << "\n";


        stream << ":20002000";
        for(int i = 0; i < Bootaddr2_8_length; i++)
        {
            stream << Bootaddr2_8[i].remove("0x");
        }
        for(int i = 0; i < info2_4_length; i++)
        {
            stream << info2_4[i].remove("0x");
        }
        for(int i = 0; i < info3_4_length; i++)
        {
            stream << info3_4[i].remove("0x");
        }
        for(int i = 0; i < info4_4_length; i++)
        {
            stream << info4_4[i].remove("0x");
        }
        for(int i = 0; i < cor2_4_length; i++)
        {
            stream << cor2_4[i].remove("0x");
        }
        for(int i = 0; i < info5_4_length; i++)
        {
            stream << info5_4[i].remove("0x");
        }
        for(int i = 0; i < info6_4_length; i++)
        {
            stream << info6_4[i].remove("0x");
        }
        stream << "60\n";

        stream << ":20004000";

        for(int i = 0; i < info7_4_length; i++)
        {
            stream << info7_4[i].remove("0x");
        }
        for(int i = 0; i < NOP_8_lenght; i++)
        {
            stream << NOP_8[i].remove("0x");
        }
        stream << "000000000000000000000000000000000000000041";
        file.flush();
        file.close();
    }


}

bool goldenReference::generate_goldenRef()
{
    creatHeader();
    setSrecParameters();

    QFileInfo hexFile = m_location.filestring+"/"+m_filename+".hex";
    if (!(hexFile.exists() && hexFile.isFile()))
    {
        qDebug() << "error by hex file";
        setOutInfo("Problems while creating Golden Reference", m_errorColor);
        setOutInfo(m_processOut, m_errorColor);
    }
    else {
        setOutInfo("Created Golden Reference successfully:", m_infoColor);
        setOutInfo(m_location.filestring+"/"+m_filename+".hex", m_normalColor);
        QFile fileHeader(m_location.filestring + "/" + m_filename + "_header.hex");
        fileHeader.remove();
        QFile headerfilled(m_location.filestring+"/"+m_filename+"_header_filled.hex");
        headerfilled.remove();
    }
    return  true;
}
