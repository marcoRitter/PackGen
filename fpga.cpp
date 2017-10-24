#include "fpga.h"
#include "mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>

Fpga::Fpga(QObject *parent) :
    Node(parent,"FPGA")
{
    m_start_addr.hexstring = "0x";
    this->setObjectName("FPGA");
//  m_with_golden_reference ;
}

FileString Fpga::filename()
{
    return m_filename;
}

//void Fpga::setFilename(FileString filename)
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

QString Fpga::ver_major()
{
   return m_ver_major;
}

void Fpga::setVer_major(QString ver_major)
{
    m_ver_major = ver_major;
}

QString Fpga::ver_minor()
{
    return m_ver_minor;
}

void Fpga::setVer_minor(QString ver_minor)
{
    m_ver_minor = ver_minor;
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
}

bool Fpga::with_golden_reference()
{
    return m_with_golden_reference;
}

void Fpga::setWith_golden_reference(bool with_golden_reference)
{
    qDebug () << "set golden";
    if ((with_golden_reference == 0) && (m_with_golden_reference == 1))
    {
        if (m_flashsize.selectedsize == 0)
            m_start_addr.hexstring = "0x2000";
        if (m_flashsize.selectedsize == 1)
            m_start_addr.hexstring = "0x4000";
        if (m_flashsize.selectedsize == 2)
            m_start_addr.hexstring = "0x8000";
//      this->set_need_redraw();
    }
    m_with_golden_reference = with_golden_reference;
}

void Fpga::setDualBoot(DualBoot dualboot)
{
    m_dualboot = dualboot;
}

void Fpga::node_menue(QMenu *menu)
{
    QAction *newAct1 = new QAction(tr("&Delete"), this);
    connect(newAct1, &QAction::triggered, this, &Node::delete_node);

    menu->addAction(newAct1);
}

