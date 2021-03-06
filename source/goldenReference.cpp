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
#include <QDir>
#include <QDateTime>

goldenReference::goldenReference(QObject *parent) : Node(parent, "goldenReference")
{
    this->setObjectName("golden");

    pGenerate = new QAction(tr("Generate"), this);
    connect(pGenerate, SIGNAL(triggered()), this, SLOT(generate_goldenRef()));

    pDelete = new QAction(tr("&Delete"), this);
    connect(pDelete, &QAction::triggered, this, &Node::delete_node);

    connect(this,SIGNAL(setOutInfo(QString, QColor)),parent->parent(), SLOT(printOutInfo(QString, QColor)));

    connect(parent->parent(), SIGNAL (generateFpga()), this, SLOT(generate_goldenRef()));

    connect(this, SIGNAL(need_redraw(QString ,  QVariant )),
           parent->parent(),SLOT(changeProperty ( QString  ,QVariant )));

    m_parent = parent;
}

goldenReference::~goldenReference()
{
    disconnect(pGenerate, SIGNAL (triggered()),this, SLOT(generate_goldenRef()));
    disconnect(pDelete, &QAction::triggered, this, &Node::delete_node);
    disconnect(m_parent->parent(), SIGNAL (generateFpga()), this, SLOT(generate_goldenRef()));
    disconnect(this,SIGNAL(setOutInfo(QString, QColor)),m_parent->parent(), SLOT(printOutInfo(QString, QColor)));
    disconnect(this, SIGNAL(need_redraw(QString ,  QVariant )),
           m_parent->parent(),SLOT(changeProperty ( QString  ,QVariant )));

    delete pGenerate;
    delete pDelete;
}

QString goldenReference::goldenRef_file()
{
    return m_goldenRef_file;
}

void goldenReference::setGoldenRef_file(QString file)
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

QString goldenReference::location()
{
    return m_location;
}
void goldenReference::setLocation(QString location)
{
    int pos = location.lastIndexOf(QChar('/'));
    int pos2 = location.length();

    if(pos+1 == pos2)
    {
        m_location = location.left(pos);
    }
    else {
        m_location = location;
    }
}

FileType goldenReference::file_type()
{
    return m_file_type;
}
void goldenReference::setFile_type(FileType filetype)
{
    m_file_type = filetype;
}


FlashSize goldenReference::flash_size()
{
    return m_flash_size;
}
void goldenReference::setFlash_size(FlashSize flashsize)
{
    m_flash_size = flashsize;
    if(!m_parent->property("JadeProject").value<bool>())
        need_redraw("start_addr",goldenReference::updateStartAddress());
}

FpgaType goldenReference::fpgatype()
{
    return m_fpgatype;
}

void goldenReference::setFpgatype(FpgaType fpgatype)
{
    m_fpgatype = fpgatype;
    if(!m_parent->property("JadeProject").value<bool>())
        need_redraw("start_addr",goldenReference::updateStartAddress());
}

QString goldenReference::start_addr()
{
    return m_start_addr;
}

void goldenReference::setStart_addr(QString start_addr)
{
    m_start_addr = start_addr;
}

void goldenReference::node_menue(QMenu *menu)
{
    menu->addAction(pGenerate);
    menu->addSeparator();
    menu->addAction(pDelete);
}

QString goldenReference::getInPath()
{
    QString temp = m_goldenRef_file;

    return (m_parent->property("working_directory").value<FileString>().filestring + "/" + temp.remove("xHOME/"));
}

QString goldenReference::getOutPath()
{
    QString temp = m_location;

    if(temp.remove("xHOME/") == "")
    {
        return (m_parent->property("working_directory").value<FileString>().filestring);
    }
    else {
        return (m_parent->property("working_directory").value<FileString>().filestring + "/" + temp.remove("xHOME/"));
    }
}

bool goldenReference::setSrecParameters()
{
    QStringList srecParameters;
    srecParameters.clear();

    if(jumpCommend_addr.isEmpty())
    {
        srecParameters.append(getOutPath()+"/"+"temp"+"/"+m_filename+"_header.hex");
        srecParameters.append("--intel");
        srecParameters.append("--offset");
        srecParameters.append("0x0");
        srecParameters.append("--fill");
        srecParameters.append("0x00");
        srecParameters.append("0x00");
        srecParameters.append(m_start_addr);
        srecParameters.append("--o");
        srecParameters.append(getOutPath()+"/"+"temp"+"/"+m_filename+"_header_filled.hex");
        srecParameters.append("--intel");
        srecParameters.append("--obs=16");

        m_srecParameters = srecParameters;
        if(runSrec())
        {
            srecParameters.clear();

            srecParameters.append(getOutPath()+"/"+"temp"+"/"+m_filename+"_header_filled.hex");
            srecParameters.append("--intel");
            srecParameters.append("--offset");
            srecParameters.append("0x0");
            srecParameters.append(getInPath());
            if(m_file_type.selectedType == 0)
            {
                srecParameters.append("--intel");
            }
            else if(m_file_type.selectedType == 1)
            {
                srecParameters.append("--binary");
            }
            srecParameters.append("--offset");
            srecParameters.append(m_start_addr);
            srecParameters.append("--o");
            srecParameters.append(getOutPath()+"/"+m_filename+".hex");
            srecParameters.append("--intel");
            srecParameters.append("--obs=16");

            m_srecParameters = srecParameters;
            runSrec();
        }
    }
    else {
        srecParameters.append(getOutPath()+"/"+"temp"+"/"+m_filename+"_header.hex");
        srecParameters.append("--intel");
        srecParameters.append("--bit-reverse");
        srecParameters.append("--offset");
        srecParameters.append(jumpCommend_addr);
        srecParameters.append("--o");
        srecParameters.append(getOutPath()+"/"+"temp"+"/"+m_filename+"_header_filled.hex");
        srecParameters.append("--intel");
        srecParameters.append("--obs=16");

        m_srecParameters = srecParameters;
        if(runSrec())
        {
            srecParameters.clear();

            srecParameters.append(getOutPath()+"/"+"temp"+"/"+m_filename+"_header_filled.hex");
            srecParameters.append("--intel");
            //srecParameters.append("--offset");
            //srecParameters.append(jumpCommend_addr);
            srecParameters.append(getInPath());
            if(m_file_type.selectedType == 0)
            {
                srecParameters.append("--intel");
            }
            else if(m_file_type.selectedType == 1)
            {
                srecParameters.append("--binary");
                srecParameters.append("--bit-reverse");
            }
            srecParameters.append("--offset");
            srecParameters.append(start_addr()+"0000");

            srecParameters.append("--o");
            srecParameters.append(getOutPath()+"/"+m_filename+".hex");
            srecParameters.append("--intel");
            srecParameters.append("--obs=16");

            m_srecParameters = srecParameters;
            runSrec();
        }
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

bool goldenReference::creatHeader()
{

    if(m_filename.isEmpty())
    {
        setOutInfo("goldenReference Property filename has no value", m_errorColor);
        return false;
    }
    if(getOutPath() == "")
    {
        setOutInfo("goldenReference Property location has no value", m_errorColor);
        return false;
    }
    QString tempfile = m_goldenRef_file;
    QFileInfo  dir_file = m_parent->property("working_directory").value<FileString>().filestring + "/" + tempfile.remove("xHOME/");

    if(!dir_file.exists())
    {
        setOutInfo("goldenReference Property goldenRef_file doesn`t exist", m_errorColor);
        return false;
    }

    if(jumpCommend_addr.isEmpty())
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

        QString filenameHeader = getOutPath() + "/" + "temp" + "/" + m_filename + "_header.hex";
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

            return true;
        }
        else {
            setOutInfo("can`t create header file", m_errorColor);
            return false;
        }
    }

    else if(jumpCommend_addr == "0x000000"){
        /*QString header_dummy[] = {"0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF"};
        const uint8_t header_dumy_length = 16;
        QString header_preamble[] = {"0xBD", "0xB3"};
        const uint8_t header_preamble_length = 2;
        QString control_register[] = {"0xC4", "0x00", "0x00", "0x00", "0x00", "0x00", "0x00", "0x00"};
        const uint8_t control_register_length = 8;
        QString jump_command[] = {"0xFE", "0x00", "0x00", "0x00", "0x03", arrayLattice[0][2]};
        const uint8_t jump_command_length = 6;
        QString frame_end[] = {"0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF"};
        const uint8_t frame_end_length = 16;*/

        QString row1[] = {"0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF"};
        const uint8_t row1_length = 16;
        QString row2[] = {"0xBD", "0xB3", "0xC4", "0x00", "0x00", "0x00", "0x00", "0x00", "0x00", "0x00", "0xFE", "0x00", "0x00", "0x00", "0x03", m_start_addr.remove("0000")};
        const uint8_t row2_length = 16;
        QString row3[] = {"0x00","0x00","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF"};
        const uint8_t row3_length = 16;
        QString row4[] = {"0xFF","0xFF"};
        const uint8_t row4_length = 2;

        QString filenameHeader = getOutPath() + "/" + "temp" + "/" + m_filename + "_header.hex";
        QFile file(filenameHeader);

        if(file.open(QFile::WriteOnly|QFile::Text))
        {
            QTextStream stream(&file);

            stream << ":10000000";
            for(int i = 0; i < row1_length; i++)
            {
                stream << row1[i].remove("0x");
            }

            stream << "00";
            stream << "\n";
            stream << ":10001000";


            for(int i = 0; i < row2_length; i++)
            {
                stream << row2[i].remove("0x");
            }


            if(m_start_addr.contains("0x02"))
            {
                stream << "A9";
            }
            else if(m_start_addr.contains("0x04"))
            {
                stream << "A7";
            }
            else if(m_start_addr.contains("0x08"))
            {
                stream << "A3";
            }
            else if(m_start_addr.contains("0x10"))
            {
                stream << "9B";
            }
            else if(m_start_addr.contains("0x20"))
            {
                stream << "8B";
            }
            else if(m_start_addr.contains("0x40"))
            {
                stream << "6B";
            }
            else if(m_start_addr.contains("0x80"))
            {
                stream << "2B";
            }

            stream << "\n";
            stream << ":10002000";

            for(int i = 0; i < row3_length; i++)
            {
                stream << row3[i].remove("0x");
            }

            stream << "DE";
            stream << "\n";
            stream << ":02003000";

            for(int i = 0; i < row4_length; i++)
            {
                stream << row4[i].remove("0x");
            }

            stream << "D0";
            file.flush();
            file.close();
            return true;
        }
        else {
            return false;
        }

    }
    else {
        QString row1[] = {"0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF"};
        const uint8_t row1_length = 16;
        QString row2[] = {"0xFF","0xFF","0xBD", "0xB3", "0xFF", "0xFF", "0xFF", "0xFF", "0x7E", "0x00", "0x00", "0x00", "0x03", m_start_addr.remove("0000"), "0x00","0x00"};
        const uint8_t row2_length = 16;
        QString row3[] = {"0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF","0xFF"};
        const uint8_t row3_length = 16;
        QString row4[] = {"0xFF","0xFF"};
        const uint8_t row4_length = 2;

        QString filenameHeader = getOutPath() + "/" + "temp" + "/" + m_filename + "_header.hex";
        QFile file(filenameHeader);

        if(file.open(QFile::WriteOnly|QFile::Text))
        {
            QTextStream stream(&file);

            stream << ":10000000";
            for(int i = 0; i < row1_length; i++)
            {
                stream << row1[i].remove("0x");
            }

            stream << "00";
            stream << "\n";
            stream << ":10001000";


            for(int i = 0; i < row2_length; i++)
            {
                stream << row2[i].remove("0x");
            }

            if(m_start_addr.contains("0x00"))
            {
                stream << "F5";
            }

            if(m_start_addr.contains("0x02"))
            {
                stream << "F3";
            }
            else if(m_start_addr.contains("0x04"))
            {
                stream << "F1";
            }
            else if(m_start_addr.contains("0x08"))
            {
                stream << "ED";
            }
            else if(m_start_addr.contains("0x10"))
            {
                stream << "E5";
            }
            else if(m_start_addr.contains("0x20"))
            {
                stream << "D5";
            }
            else if(m_start_addr.contains("0x40"))
            {
                stream << "B5";
            }
            else if(m_start_addr.contains("0x80"))
            {
                stream << "75";
            }

            stream << "\n";
            stream << ":10002000";

            for(int i = 0; i < row3_length; i++)
            {
                stream << row3[i].remove("0x");
            }

            stream << "E0";
            stream << "\n";
            stream << ":02003000";

            for(int i = 0; i < row4_length; i++)
            {
                stream << row4[i].remove("0x");
            }

            stream << "D0";
            file.flush();
            file.close();
            return true;
    }
    }

}

bool goldenReference::generate_goldenRef()
{
    QDir dir(getOutPath());
    QDir dir_temp(getOutPath()+"/temp");

    if(dir.exists())
    {
    }
    else {
        dir.mkpath(getOutPath());
    }
    if(dir_temp.exists())
    {
        dir_temp.removeRecursively();
        dir_temp.mkpath(getOutPath()+"/temp");
    }
    else {
        dir_temp.mkpath(getOutPath()+"/temp");
    }


    if(m_fpgatype.selectedfpga == 0)
    {
        jumpCommend_addr.clear();
    }

    else if(m_fpgatype.selectedfpga == 1)
    {
        jumpCommend_addr = "0x000000";
        jumpCommend_fill = "0x000100";
    }

    else if(m_fpgatype.selectedfpga == 2)
    {
        switch (flash_size().selectedsize) {
        case 0:
            jumpCommend_addr = "0x03FF00";
            jumpCommend_fill = "0x040000";
            break;
        case 1:
            jumpCommend_addr = "0x07FF00";
            jumpCommend_fill = "0x080000";
            break;
        case 2:
            jumpCommend_addr = "0x0FFF00";
            jumpCommend_fill = "0x100000";
            break;
        case 3:
            jumpCommend_addr = "0x1FFF00";
            jumpCommend_fill = "0x200000";
            break;
        case 4:
            jumpCommend_addr = "0x3FFF00";
            jumpCommend_fill = "0x400000";
            break;
        case 5:
            jumpCommend_addr = "0x7FFF00";
            jumpCommend_fill = "0x800000";
            break;
        case 6:
            jumpCommend_addr = "0xFFFF00";
            jumpCommend_fill = "0x1000000";
            break;
        }
    }


    if(!creatHeader())
        return false;

    setSrecParameters();

    QFileInfo hexFile = getOutPath()+"/"+m_filename+".hex";
    if (!(hexFile.exists() && hexFile.isFile()))
    {
        qDebug() << "error by hex file";
        setOutInfo("Problems while creating Golden Reference", m_errorColor);
        setOutInfo(m_processOut, m_errorColor);
    }
    else {
        setOutInfo("Created Golden Reference successfully:", m_infoColor);
        setOutInfo(getOutPath()+"/"+m_filename+".hex", m_normalColor);
//        QFile fileHeader(getOutPath() + "/" + m_filename + "_header.hex");
//        fileHeader.remove();
//        QFile headerfilled(getOutPath()+"/"+m_filename+"_header_filled.hex");
//        headerfilled.remove();
    }
    return  true;
}


QVariant goldenReference::updateStartAddress()
{
    QVariant a;
    HexString temp;

    if(fpgatype().selectedfpga == 0)
    {
        setStart_addr("0x010000");
    }

    else
    {
        switch (flash_size().selectedsize) {
        case 0:
            setStart_addr("0x020000");
            break;
        case 1:
            setStart_addr("0x040000");
            break;
        case 2:
            setStart_addr("0x080000");
            break;
        case 3:
            setStart_addr("0x100000");
            break;
        case 4:
            setStart_addr("0x200000");
            break;
        case 5:
            setStart_addr("0x400000");
            break;
        case 6:
            setStart_addr("0x800000");
            break;
        }
    }

    a.setValue(m_start_addr);
    return a;
}
