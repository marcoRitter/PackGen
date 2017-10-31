#include <QDebug>
#include "JADE_History.h"
#include <QJsonObject>
#include <QJsonDocument>
JADE_History::JADE_History(QObject *parent) :
    Node(parent,"JADE History")
{

}

QString JADE_History::filename()
{
    return m_filename;
}

void JADE_History::setFilename(QString name)
{
    m_filename = name;
}

uint JADE_History::ver_major()
{
   return m_ver_major;
}

void JADE_History::setVer_major(uint ver_major)
{
    m_ver_major = ver_major;
}

uint JADE_History::ver_minor()
{
    return m_ver_minor;
}

void JADE_History::setVer_minor(uint ver_minor)
{
    m_ver_minor = ver_minor;
}

uint JADE_History::ver_subminor()
{
    return m_ver_subminor;
}

void JADE_History::setVer_subminor(uint ver_subminor)
{
    m_ver_subminor = ver_subminor;
}


#if 0
bool JADE_History::readJson(const QJsonObject *jsonObj)
{
    QJsonValue jsonVal;

    setDescription(jsonObj->value("description").toString());
    setFilename(jsonObj->value("filename").toString());

    setVer_major(jsonObj->value("ver_major").toString().toInt());
    setVer_minor(jsonObj->value("ver_minor").toString().toInt());
    setVer_subminor(jsonObj->value("ver_subminor").toString().toInt());

    return true;
}

bool JADE_History::writeJson(QJsonObject *jsonObj)
{
    jsonObj->insert("node_type",node_type());
    jsonObj->insert("description",description());
    jsonObj->insert("filename",filename());
    jsonObj->insert("ver_major",QString::number(ver_major()));
    jsonObj->insert("ver_minor",QString::number(ver_minor()));
    jsonObj->insert("ver_subminor",QString::number(ver_subminor()));

    return true;
}
#endif
void JADE_History::node_menue(QMenu *menu)
{
    QAction *newAct1 = new QAction(tr("Delete"), this);
    connect(newAct1, &QAction::triggered, this, &Node::delete_node);


    menu->addAction(newAct1);
    menu->addSeparator();

}


