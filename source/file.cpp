#include <QJsonObject>
#include <QJsonDocument>
#include "file.h"
#include "mainwindow.h"


file::file(QObject *parent) : Node(parent,"File")
{
    QIcon DeleteIcon;
    this->setObjectName("File");
    DeleteIcon.addFile(":/Images/icons8-delete.png",QSize(25,25));
    pDeleteFile = new QAction (tr("&Delete"),this);

    pDeleteFile->setIcon(DeleteIcon);
    connect(pDeleteFile, &QAction::triggered, this, &Node::delete_node);

    connect(this, SIGNAL(need_redraw(QString ,  QVariant )),
           parent->parent()->parent(),SLOT(changeProperty ( QString  ,QVariant )));

    m_parent = parent;
}

file::~file()
{
    disconnect(pDeleteFile, &QAction::triggered, this, &Node::delete_node);
    delete pDeleteFile;
    disconnect(this, SIGNAL(need_redraw(QString ,  QVariant )),
           m_parent->parent()->parent(),SLOT(changeProperty ( QString  ,QVariant )));
}

FileString file::filename()
{
    return m_filename;
}

void file::setFilename(FileString filename)
{
    m_filename = filename;
}

FileType file::file_type()
{
    return m_file_type;
}

void file::setFile_type(FileType file_type)
{
    m_file_type = file_type;
}

QString file::version()
{
    return m_version;
}

void file::setVersion(QString version)
{
    m_version = version;
}

QString file::start_addr()
{
    return m_start_addr;
}

void file::setStart_addr(QString start_addr)
{
    m_start_addr = start_addr;
}

void file::node_menue(QMenu *menu)
{
    menu->addAction(pDeleteFile);
}

QString file::object_name()
{
    return this->getType();
}

void file::setObject_name(QString object_name)
{
    this->setType(object_name);
}

FpgaType file::fpgatype()
{
    return m_fpgatype;
}

void file::setFpgatype(FpgaType fpgatype)
{
    m_fpgatype = fpgatype;
    if(!m_parent->parent()->property("JadeProject").value<bool>())
        need_redraw("start_addr",file::updateStartAddress());

}

QVariant file::updateStartAddress()
{
    QVariant a;
    HexString temp;
        //QString srecExe = m_parent->property("srec_cat").value<FileString>().filestring;
    m_flash_size = m_parent->property("flash_size").value<FlashSize>();

    if(fpgatype().selectedfpga == 1)
    {
        if(m_start_addr == "0x010000" ||m_start_addr == "0x020000" || m_start_addr == "0x040000" ||
                m_start_addr == "0x080000" || m_start_addr == "0x100000" || m_start_addr == "0x200000"
                ||m_start_addr == "0x400000" || m_start_addr == "0x800000")
        {
            setStart_addr("0x010000");
        }
    }
    else if (fpgatype().selectedfpga == 2) {
        if(m_start_addr == "0x010000" ||m_start_addr == "0x020000" || m_start_addr == "0x040000" ||
                m_start_addr == "0x080000" || m_start_addr == "0x100000" || m_start_addr == "0x200000"
                ||m_start_addr == "0x400000" || m_start_addr == "0x800000")
        {
            setStart_addr("0x000000");
        }
    }
    else
    {
        if(m_start_addr == "0x010000" ||m_start_addr == "0x020000" || m_start_addr == "0x040000" ||
                m_start_addr == "0x080000" || m_start_addr == "0x100000" || m_start_addr == "0x200000"
                ||m_start_addr == "0x400000" || m_start_addr == "0x800000")
        {
            switch (m_flash_size.selectedsize) {
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
    }

    a.setValue(m_start_addr);
    return a;
}
