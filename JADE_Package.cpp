#include <QDebug>
#include "JADE_Package.h"
#include <QJsonObject>
#include <QJsonDocument>
JADE_Package::JADE_Package(QObject *parent) :
    Node(parent,"JADE Package")
{

}

QString JADE_Package::filename()
{
    return m_filename;
}

void JADE_Package::setFilename(QString name)
{
    m_filename = name;
}

QString JADE_Package::module_name()
{
    return m_module_name;
}

void JADE_Package::setModule_name(QString module_name)
{
    m_module_name = module_name;
}

uint JADE_Package::ver_major()
{
   return m_ver_major;
}

void JADE_Package::setVer_major(uint ver_major)
{
    m_ver_major = ver_major;
}

uint JADE_Package::ver_minor()
{
    return m_ver_minor;
}

void JADE_Package::setVer_minor(uint ver_minor)
{
    m_ver_minor = ver_minor;
}

uint JADE_Package::ver_subminor()
{
    return m_ver_subminor;
}

void JADE_Package::setVer_subminor(uint ver_subminor)
{
    m_ver_subminor = ver_subminor;
}


#if 0
bool JADE_Package::readJson(const QJsonObject *jsonObj)
{
    QJsonValue jsonVal;

    setDescription(jsonObj->value("description").toString());
    setFilename(jsonObj->value("filename").toString());

    setModule_name(jsonObj->value("module_name").toString());

    setVer_major(jsonObj->value("ver_major").toString().toInt());
    setVer_minor(jsonObj->value("ver_minor").toString().toInt());
    setVer_subminor(jsonObj->value("ver_subminor").toString().toInt());
    return true;
}

bool JADE_Package::writeJson(QJsonObject *jsonObj)
{
    jsonObj->insert("node_type",node_type());
    jsonObj->insert("description",description());
    jsonObj->insert("module_name",module_name());
    jsonObj->insert("filename",filename());
    jsonObj->insert("ver_major",QString::number(ver_major()));
    jsonObj->insert("ver_minor",QString::number(ver_minor()));
    jsonObj->insert("ver_subminor",QString::number(ver_subminor()));

    return true;
}
#endif
void JADE_Package::node_menue(QMenu *menu)
{
    QAction *newAct1 = new QAction(tr("Generate"), this);
    connect(newAct1, SIGNAL(triggered()), this, SLOT(generate_package()));
    QAction *newAct2 = new QAction(tr("Add History File"), this);
    connect(newAct2, SIGNAL(triggered()), this, SLOT(new_JADE_Historyfile()));
    QAction *newAct3 = new QAction(tr("Add Module Description"), this);
    connect(newAct3, SIGNAL(triggered()), this, SLOT(new_JADE_Moduledescription()));
    QAction *newAct4 = new QAction(tr("Add FPGA"), this);
    connect(newAct4, SIGNAL(triggered()), this, SLOT(new_JADE_FPGA()));
    QAction *newAct5 = new QAction(tr("Delete"), this);
    connect(newAct5, &QAction::triggered, this, &Node::delete_node);


    menu->addAction(newAct1);
    menu->addSeparator();
    menu->addAction(newAct2);
    menu->addAction(newAct3);
    menu->addAction(newAct4);
    menu->addSeparator();
    menu->addAction(newAct5);

}

bool JADE_Package::generate_package()
{
    qDebug() << "Generate JADE Package";
    return true;
}

void JADE_Package::new_JADE_Moduledescription()
{
    JADE_Moduledescription *m = new JADE_Moduledescription();
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("JADE Moduledescription");
    this->setChild(this->rowCount(),m);
}

void JADE_Package::new_JADE_Historyfile()
{
    JADE_History *m = new JADE_History();
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("JADE History");
    this->setChild(this->rowCount(),m);
}

void JADE_Package::new_JADE_FPGA()
{

}

