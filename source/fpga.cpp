#include "fpga.h"
#include "mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>

Fpga::Fpga(QObject *parent) :
    Node(parent,"FPGA")
{
    QIcon DeleteIcon;
    m_start_addr.hexstring = "0x";
    this->setObjectName("FPGA");

    DeleteIcon.addFile(":/Images/icons8-delete.png",QSize(25,25));
    pDeleteFPGA = new QAction(tr("&Delete"), this);
    pDeleteFPGA->setIcon(DeleteIcon);
    connect(pDeleteFPGA, &QAction::triggered, this, &Node::delete_node);

    connect(this, SIGNAL(need_redraw(QString ,  QVariant )),
           parent->parent()->parent(),SLOT(changeProperty ( QString  ,QVariant )));
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

HexString Fpga::start_addr()
{
    return m_start_addr;
}

void Fpga::setStart_addr(HexString start_addr)
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
    if (m_dualboot.dualbootena && !m_fpgatype.selectedfpga)
        m_start_addr = temp.get_offset(m_flashsize.selectedsize);
    else
        m_start_addr = temp.get_offset(255);
    a.setValue<HexString>(m_start_addr);
    return a.value<HexString>().hexstring;
}