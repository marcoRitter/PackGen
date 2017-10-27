#include "project.h"
#include <QJsonObject>
#include <QJsonDocument>

Project::Project(QObject *parent) :
    Node(parent, "Project")
{
    QIcon iconAct1;
    iconAct1.addFile("new.png",QSize(20,20));
    pNewSpartan = new QAction(tr("&New M86 Spartan"), this);
    pNewSpartan->setIcon(iconAct1);
    connect(pNewSpartan, SIGNAL(triggered()), this, SLOT(new_M86_Spartan6()));
    pNewMasterFile = new QAction(tr("&Masterfile"), this);
    connect(pNewMasterFile, SIGNAL(triggered()), this, SLOT(new_Masterfile()));
    pNewJadeFile = new QAction(tr("&New JADE Package"), this);
    connect(pNewJadeFile, SIGNAL(triggered()), this, SLOT(new_JADE_Package()));

}

Project::~Project()
{
    disconnect(pNewSpartan, SIGNAL(triggered()), this, SLOT(new_M86_Spartan6()));
    disconnect(pNewMasterFile, SIGNAL(triggered()), this, SLOT(new_Masterfile()));
    disconnect(pNewJadeFile, SIGNAL(triggered()), this, SLOT(new_JADE_Package()));

    delete pNewSpartan;
    delete pNewMasterFile;
    delete pNewJadeFile;
}

QString Project::name()
{
    return m_name;
}

QString Project::Property1()
{
    return Node::getType();
}

void Project::setProperty1(QString property1)
{
    m_Property1 = property1;
}
void Project::setName(QString name)
{
    m_name = name;
}

bool Project::readJson(const QJsonObject *jsonObj)
{
    setName(jsonObj->value("name").toString());
    setDescription(jsonObj->value("description").toString());

    return true;
}

bool Project::writeJson(QJsonObject *jsonObj)
{
    jsonObj->insert("node_type",node_type());
    jsonObj->insert("name",name());
    jsonObj->insert("description",description());

    return true;
}

void Project::new_M86_Spartan6()
{
    M86_Spartan6 *m = new M86_Spartan6(this);//(QObject::parent());
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("M86 for SCT202");
//  m->setType("FPGA");
    //m->setVer_state(Alpha);
    this->setChild(this->rowCount(),m);
//  qDebug() << this->children();
}

void Project::new_JADE_Package()
{
    JADE_Package *m = new JADE_Package();
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("JADE Package");
    this->setChild(this->rowCount(),m);
}

void Project::new_Masterfile()
{
    Masterfile *m = new Masterfile();
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("Masterfile");
    this->setChild(this->rowCount(),m);
}

void Project::node_menue(QMenu *menu)
{
    menu->addAction(pNewSpartan);
    menu->addAction(pNewMasterFile);
    menu->addAction(pNewJadeFile);
}
