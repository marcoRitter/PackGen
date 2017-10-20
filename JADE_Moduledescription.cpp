#include <QDebug>
#include "JADE_Moduledescription.h"
#include <QJsonObject>
#include <QJsonDocument>
JADE_Moduledescription::JADE_Moduledescription(QObject *parent) :
    Node(parent,"JADE Moduledescription")
{

}

QString JADE_Moduledescription::filename()
{
    return m_filename;
}

void JADE_Moduledescription::setFilename(QString name)
{
    m_filename = name;
}

quint32 JADE_Moduledescription::ver_major()
{
   return m_ver_major;
}

void JADE_Moduledescription::setVer_major(quint32 ver_major)
{
    m_ver_major = ver_major;
}

quint32 JADE_Moduledescription::ver_minor()
{
    return m_ver_minor;
}

void JADE_Moduledescription::setVer_minor(quint32 ver_minor)
{
    m_ver_minor = ver_minor;
}

quint32 JADE_Moduledescription::ver_subminor()
{
    return m_ver_subminor;
}

void JADE_Moduledescription::setVer_subminor(quint32 ver_subminor)
{
    m_ver_subminor = ver_subminor;
}



bool JADE_Moduledescription::readJson(const QJsonObject *jsonObj)
{
    QJsonValue jsonVal;

    setDescription(jsonObj->value("description").toString());
    setFilename(jsonObj->value("filename").toString());

    setVer_major(jsonObj->value("ver_major").toString().toInt());
    setVer_minor(jsonObj->value("ver_minor").toString().toInt());
    setVer_subminor(jsonObj->value("ver_subminor").toString().toInt());

    return true;
}

bool JADE_Moduledescription::writeJson(QJsonObject *jsonObj)
{
    jsonObj->insert("node_type",node_type());
    jsonObj->insert("description",description());
    jsonObj->insert("filename",filename());
    jsonObj->insert("ver_major",QString::number(ver_major()));
    jsonObj->insert("ver_minor",QString::number(ver_minor()));
    jsonObj->insert("ver_subminor",QString::number(ver_subminor()));

    return true;
}

void JADE_Moduledescription::node_menue(QMenu *menu)
{
    QAction *newAct1 = new QAction(tr("Delete"), this);
    connect(newAct1, &QAction::triggered, this, &Node::delete_node);


    menu->addAction(newAct1);
    menu->addSeparator();

}


