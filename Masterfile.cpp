#include <QDebug>
#include "Masterfile.h"
#include <QJsonObject>
#include <QJsonDocument>


Masterfile::Masterfile(QObject *parent) :
    Node(parent,"Masterfile")
{

}

QString Masterfile::filename()
{
    return m_filename;
}

void Masterfile::setFilename(QString name)
{
    m_filename = name;
}

QString Masterfile::ver_major()
{
   return m_ver_major;
}

void Masterfile::setVer_major(QString ver_major)
{
    m_ver_major = ver_major;
}

QString Masterfile::ver_minor()
{
    return m_ver_minor;
}

void Masterfile::setVer_minor(QString ver_minor)
{
    m_ver_minor = ver_minor;
}

QString Masterfile::ver_subminor()
{
    return m_ver_subminor;
}

void Masterfile::setVer_subminor(QString ver_subminor)
{
    m_ver_subminor = ver_subminor;
}



bool Masterfile::readJson(const QJsonObject *jsonObj)
{
    QJsonValue jsonVal;

    jsonVal = jsonObj->value("description");
    setDescription(jsonVal.toString());
    jsonVal = jsonObj->value("filename");
    setFilename(jsonVal.toString());
    jsonVal = jsonObj->value("ver_major");
    setVer_major(jsonVal.toString());
    jsonVal = jsonObj->value("ver_minor");
    setVer_minor(jsonVal.toString());
    jsonVal = jsonObj->value("ver_subminor");
    setVer_subminor(jsonVal.toString());

    return true;
}

bool Masterfile::writeJson(QJsonObject *jsonObj)
{
    jsonObj->insert("node_type",node_type());
    jsonObj->insert("description",description());
    jsonObj->insert("filename",filename());
    jsonObj->insert("ver_major",ver_major());
    jsonObj->insert("ver_minor",ver_minor());
    jsonObj->insert("ver_subminor",ver_subminor());

    return true;
}

void Masterfile::node_menue(QMenu *menu)
{
    QAction *newAct1 = new QAction(tr("Generate"), this);
    connect(newAct1, SIGNAL(triggered()), this, SLOT(generate_masterfile()));
//    QAction *newAct2 = new QAction(tr("&New FPGA"), this);
//    connect(newAct2, &QAction::triggered, this, &M86_Spartan6::new_FPGA);
    QAction *newAct3 = new QAction(tr("&Delete"), this);
    connect(newAct3, &QAction::triggered, this, &Node::delete_node);


    menu->addAction(newAct1);
    menu->addSeparator();
//    menu->addAction(newAct2);
//    menu->addSeparator();
    menu->addAction(newAct3);

}

//void Masterfile::new_FPGA()
//{
//    Fpga *m = new Fpga();
//    Model* m_m = Node::getModel();
//    m->setModel(m_m);
//    m->setDescription("Spartan 6 FPGA");
//    this->setChild(this->rowCount(),m);

//}

bool Masterfile::generate_masterfile()
{
    qDebug() << "Generate Masterfile";
    return true;
}

