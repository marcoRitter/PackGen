#include <QDebug>
#include "M86_Spartan6.h"
#include <QJsonObject>
#include <QJsonDocument>


M86_Spartan6::M86_Spartan6(QObject *parent) :
    Node(parent,"M86_Spartan6")
{

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
    QAction *newAct1 = new QAction(tr("Generate"), this);
    connect(newAct1, SIGNAL(triggered()), this, SLOT(generate_package()));
    QAction *newAct2 = new QAction(tr("&New FPGA"), this);
    connect(newAct2, &QAction::triggered, this, &M86_Spartan6::new_FPGA);
    QAction *newAct3 = new QAction(tr("&Delete"), this);
    connect(newAct3, &QAction::triggered, this, &Node::delete_node);


    menu->addAction(newAct1);
    menu->addSeparator();
    menu->addAction(newAct2);
    menu->addSeparator();
    menu->addAction(newAct3);

}

void M86_Spartan6::new_FPGA()
{
    Fpga *m = new Fpga();
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("Spartan 6 FPGA");
    this->setChild(this->rowCount(),m);

}

bool M86_Spartan6::generate_package()
{
    qDebug() << "Generate Spartan M86Package";
    return true;
}

