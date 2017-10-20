#include "project.h"
#include <QJsonObject>
#include <QJsonDocument>

Project::Project(QObject *parent) :
    Node(parent, "Project")
{

}
#if 0
Project::Project():
    Node(null,"Project")
{

}

Project::Project (QObject *parent, QString name) :
    Node(parent, name)
{

}
#endif

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
    M86_Spartan6 *m = new M86_Spartan6();
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("M86 for SCT202");
    //m->setVer_state(Alpha);
    this->setChild(this->rowCount(),m);
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
    QIcon iconAct1;
    iconAct1.addFile("C:/Users/GRPA/Documents/WORK/C_Projects/PackGen-NG/new.png",QSize(100,100));

    QAction *newAct1 = new QAction(tr("&New M86 Spartan"), this);
    newAct1->setIcon(iconAct1);
    connect(newAct1, SIGNAL(triggered()), this, SLOT(new_M86_Spartan6()));
    QAction *newAct2 = new QAction(tr("&Masterfile"), this);
    connect(newAct2, SIGNAL(triggered()), this, SLOT(new_Masterfile()));
    QAction *newAct3 = new QAction(tr("&New JADE Package"), this);
//  connect(newAct3, SIGNAL(triggered()), this, SLOT(new_JADE_Package()));
//  QAction *newProject = new QAction(tr("New Project"),this);


//  menu->addAction(newProject);
    menu->addAction(newAct1);
    menu->addAction(newAct2);
    menu->addAction(newAct3);
}
