#include <QDebug>
#include "M86_Spartan6.h"
#include <QJsonObject>
#include <QJsonDocument>


M86_Spartan6::M86_Spartan6(QObject *parent) :
    Node(parent,"M86_Spartan6")
{

    pGenerate = new QAction(tr("Generate"), this);
    connect(pGenerate, SIGNAL(triggered()), this, SLOT(generate_package()));
    pNewFPGA = new QAction(tr("&New FPGA"), this);
    connect(pNewFPGA, &QAction::triggered, this, &M86_Spartan6::new_FPGA);
    pDelete = new QAction(tr("&Delete"), this);
    connect(pDelete, &QAction::triggered, this, &Node::delete_node);

}
M86_Spartan6::~M86_Spartan6()
{
    disconnect(pGenerate, SIGNAL (triggered()),this, SLOT(generate_package()));
    disconnect(pNewFPGA, &QAction::triggered, this, &M86_Spartan6::new_FPGA);
    disconnect(pDelete, &QAction::triggered, this, &Node::delete_node);
    delete pGenerate;
    delete pNewFPGA;
    delete pDelete;
}

FileString M86_Spartan6::filename()
{
    return m_filename;
}

void M86_Spartan6::setFilename(FileString name)
{
    m_filename = name;
}

QString M86_Spartan6::ver_major()
{
   return m_ver_major;
}

void M86_Spartan6::setVer_major(QString ver_major)
{
    m_ver_major = ver_major;
}

QString M86_Spartan6::ver_minor()
{
    return m_ver_minor;
}

void M86_Spartan6::setVer_minor(QString ver_minor)
{
    m_ver_minor = ver_minor;
}

QString M86_Spartan6::ver_subminor()
{
    return m_ver_subminor;
}

void M86_Spartan6::setVer_subminor(QString ver_subminor)
{
    m_ver_subminor = ver_subminor;
}
/*
VER_STATE M86_Spartan6::ver_state()
{
    return m_ver_state;
}

void M86_Spartan6::setVer_state(VER_STATE ver_state)
{
    m_ver_state = ver_state;
}
*/

void M86_Spartan6::node_menue(QMenu *menu)
{
    menu->addAction(pGenerate);
    menu->addSeparator();
    menu->addAction(pNewFPGA);
    menu->addSeparator();
    menu->addAction(pDelete);
    menu->addSeparator();
}

void M86_Spartan6::new_FPGA()
{
    Fpga *m = new Fpga(this);
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("Spartan 6 FPGA");
    this->setChild(this->rowCount(),m);
}

bool M86_Spartan6::generate_package()
{
    qDebug() << "Generate Spartan M86Package";
    QObjectList childrenOfSpartan = this->children();
    for (auto const& childOfSpartan : childrenOfSpartan)
    {
        if (childOfSpartan->objectName()=="FPGA")
        {
            qDebug() << "Object name" << childOfSpartan->objectName();
            qDebug() << "design number" << childOfSpartan->property("designnumber").value<QString>();
            qDebug() << "flash size" << childOfSpartan->property("flash_size").value<FlashSize>().selectedsize;
            qDebug() << "filename " << childOfSpartan->property("filename").value<FileString>().filestring;
        }
    }

//  qDebug() << "ver_major : " << this->property("ver_major").toString();
    FileString itsFileName = this->filename();
//  qDebug() << "file_name = " << itsFileName.filestring;

    return true;
}

