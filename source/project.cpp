#include "project.h"
#include <QJsonObject>
#include <QJsonDocument>

Project::Project(QObject *parent) :
    Node(parent, "Project")
{
    Q_INIT_RESOURCE(resources);
    QIcon iconAct1;
    iconAct1.addFile(":/Images/icons8-processor.png",QSize(25,25));
    pNewSpartan = new QAction("&New M86", this);
    pNewSpartan->setIcon(iconAct1);
    connect(pNewSpartan, SIGNAL(triggered()), this, SLOT(new_M86_Spartan6()));
    /*
    pNewMasterFile = new QAction(tr("&Masterfile"), this);
    connect(pNewMasterFile, SIGNAL(triggered()), this, SLOT(new_Masterfile()));
    pNewJadeFile = new QAction(tr("&New JADE Package"), this);
    connect(pNewJadeFile, SIGNAL(triggered()), this, SLOT(new_JADE_Package()));
    */

}

Project::~Project()
{
    disconnect(pNewSpartan, SIGNAL(triggered()), this, SLOT(new_M86_Spartan6()));
    /*
    disconnect(pNewMasterFile, SIGNAL(triggered()), this, SLOT(new_Masterfile()));
    disconnect(pNewJadeFile, SIGNAL(triggered()), this, SLOT(new_JADE_Package()));
    */

    delete pNewSpartan;
    /*
    delete pNewMasterFile;
    delete pNewJadeFile;
    */
}

QString Project::name()
{
    return m_name;
}

void Project::setName(QString name)
{
    m_name = name;
}

FileString Project::srec_cat()
{
    return m_srecCat;
}

void Project::setSrec_cat(FileString filename)
{
    m_srecCat = filename;
}

FileString Project::logichdr()
{
    return m_logichdr;
}

void Project::setLogichdr(FileString filename)
{
    m_logichdr = filename;
}

FileString Project::mbind()
{
    return m_mbind;
}

void Project::setMbind(FileString filename)
{
    m_mbind = filename;
}
void Project::new_M86_Spartan6()
{
    M86_Spartan6 *m = new M86_Spartan6(this);//(QObject::parent());
    Model* m_m = Node::getModel();
    m->setModel(m_m);
    m->setDescription("New M86");
    m->setType("M86 " + m->description());
//  m->setType("FPGA");
    //m->setVer_state(Alpha);
    this->setChild(this->rowCount(),m);
//  qDebug() << this->children();
}
/*
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
*/
void Project::node_menue(QMenu *menu)
{
    menu->addAction(pNewSpartan);
    /*
    menu->addAction(pNewMasterFile);
    menu->addAction(pNewJadeFile);
    */
}
