#include "fpga.h"
#include "mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>

Fpga::Fpga(QObject *parent) :
    Node(parent,"FPGA")
{
    QIcon DeleteIcon;
//  m_start_addr.hexstring = "0x";
    this->setObjectName("FPGA");

    DeleteIcon.addFile(":/Images/icons8-delete.png",QSize(25,25));
    pDeleteFPGA = new QAction(tr("&Delete"), this);
    pDeleteFPGA->setIcon(DeleteIcon);
    connect(pDeleteFPGA, &QAction::triggered, this, &Node::delete_node);

    connect(this, SIGNAL(need_redraw(QString ,  QVariant )),
           parent->parent()->parent(),SLOT(changeProperty ( QString  ,QVariant )));
    m_parent = parent;
}

Fpga::~Fpga()
{
    disconnect(pDeleteFPGA, &QAction::triggered, this, &Node::delete_node);
    delete pDeleteFPGA;
}

FpgaType Fpga::fpgatype()
{
    return m_fpgatype;
}

void Fpga::setFpgatype(FpgaType fpgatype)
{
    m_fpgatype = fpgatype;
    need_redraw("start_addr",Fpga::updateStartAddress());
}

FileString Fpga::filename()
{
    return m_filename;
}

void Fpga::setFilename(FileString filename)
{
    m_filename = filename;
}

QString Fpga::designnumber()
{
    return m_designnumber;
}

void Fpga::setDesignnumber(QString name)
{
    m_designnumber = name;
}

QString Fpga::revision()
{
   return m_revision;
}

void Fpga::setRevision(QString revision)
{
    m_revision = revision;
}

QString Fpga::testversion()
{
    return m_testversion;
}

void Fpga::setTestversion(QString testversion)
{
    m_testversion = testversion;
}

QString Fpga::start_addr()
{
    return m_start_addr;
}

void Fpga::setStart_addr(QString start_addr)
{
    m_start_addr = start_addr;
}

FlashSize Fpga::flash_size()
{
    return m_flashsize;
}

void Fpga::setFlash_size(FlashSize flashsize)
{
    m_flashsize = flashsize;
    /*
    QVariant a;
    HexString temp;
    if (m_dualboot.dualbootena && !m_fpgatype.selectedfpga)
        m_start_addr = temp.get_offset(m_flashsize.selectedsize);
    else
        m_start_addr = temp.get_offset(255);
    a.setValue<HexString>(m_start_addr);
    this->setProperty("start_addr",a);
    need_redraw("start_addr",a.value<HexString>().hexstring);
    */
    need_redraw("start_addr",Fpga::updateStartAddress());

}

DualBoot Fpga::dualboot()
{
    return m_dualboot;
}

void Fpga::setDualBoot(DualBoot dualboot)
{
    m_dualboot = dualboot;
    /*
    QVariant a;
    HexString temp;
    if (m_dualboot.dualbootena && !m_fpgatype.selectedfpga)
        m_start_addr = temp.get_offset(m_flashsize.selectedsize);
    else
        m_start_addr = temp.get_offset(255);
    a.setValue<HexString>(m_start_addr);
    this->setProperty("start_addr",a);
    need_redraw("start_addr",a.value<HexString>().hexstring);
    */
    need_redraw("start_addr",Fpga::updateStartAddress());
}

void Fpga::node_menue(QMenu *menu)
{
    menu->addAction(pDeleteFPGA);
}

QVariant Fpga::updateStartAddress()
{
    QVariant a;
    HexString temp;
    if (m_dualboot.dualbootena && m_fpgatype.selectedfpga == 0)// && m_fpgatype.selectedfpga)
    {
        switch (m_flashsize.selectedsize)
        {
            case 0: m_start_addr = "0x020000"; break;
            case 1: m_start_addr = "0x040000"; break;
            case 2: m_start_addr = "0x080000"; break;
            case 3: m_start_addr = "0x100000"; break;
            case 4: m_start_addr = "0x200000"; break;
            case 5: m_start_addr = "0x400000"; break;
            case 6: m_start_addr = "0x800000"; break;
        }
    }
    else if(m_dualboot.dualbootena)
    {
      if(m_fpgatype.selectedfpga == 1)
      {
            m_start_addr = "0x010000";
      }
      else if(m_fpgatype.selectedfpga == 2)
      {
            m_start_addr = "0x000000";
      }
    }
//      m_start_addr = temp.get_offset(m_flashsize.selectedsize);
//  else
//      m_start_addr = "0x0";//temp.get_offset(255);
    a.setValue(m_start_addr);
    return a;
}

QString Fpga::getVerString()
{
    QString ver = "";
    ver.append("\"V");
    ver.append(designnumber());
    ver.append(".");
    ver.append(designnumber());
    ver.append(".");
    ver.append(revision());
    ver.append('"');
    return ver;
}

void Fpga::setSrecParameters()
{
    QStringList parameters;
    parameters.clear();
    parameters.append(m_filename.filestring);
    parameters.append("--binary");
    parameters.append("--offset");
    parameters.append(m_start_addr);
    if (m_fpgatype.selectedfpga)
        parameters.append("--bit_reverse");
    parameters.append("--output");
    parameters.append(m_hexFileName);
    parameters.append("--intel");

    m_srecParameters = parameters;
//  runSrec();
//  qDebug() << "fpga srec parameters = " << m_srecParameters;
}

int Fpga::runSrec()
{
    QString srecExe = m_parent->parent()->property("srec_cat").value<FileString>().filestring;

    qDebug() << "srec EXE = " << srecExe;

    QProcess *process = new QProcess(nullptr);

    qDebug() << "srec startet with " << m_srecParameters;

    process->start(srecExe,m_srecParameters,QIODevice::ReadWrite);

    if (!process->waitForStarted())
        qDebug() << "error by executing srec_cat";
    if (!process->waitForFinished())
        qDebug() << "srec_cat failed";

    m_processOut = process->readAllStandardOutput();
//  qDebug() <<"srec returned " << process->errorString();

    delete process;
    QFileInfo hexFile = m_hexFileName;
    if (!(hexFile.exists() && hexFile.isFile()))
    {
        qDebug() << "error by hex file";
        return 1;
    }

    return 0;

}

int Fpga::runLogichdr()
{
    QString logichdr = m_parent->parent()->property("logichdr").value<FileString>().filestring;
    qDebug() << "logichdr EXE = " << logichdr;
    QStringList parameters;
    parameters.clear();
    parameters << "-i" << m_hexFileName << "-o" << m_mchFileName;
    parameters << "-c" << m_typecode;
    parameters << "-v" << m_variant;
    parameters << "-d" << m_designnumber;
    parameters << "-r" << m_revision;
    parameters << "-t" << m_testversion;
    parameters << "-f" << "MCS";
    parameters << "-e" << "FPGA";
    qDebug() << "logichdr parameters = \n" << parameters;

    QProcess *process = new QProcess(nullptr);
    process->start(logichdr, parameters,QIODevice::ReadWrite);
    if (!process->waitForStarted())
        qDebug() << "error by executing srec_cat";
    if (!process->waitForFinished())
        qDebug() << "srec_cat failed";

    m_processOut = process->readAllStandardOutput();
//  qDebug() <<"srec returned " << process->errorString();
    delete process;
    return 0;
}
