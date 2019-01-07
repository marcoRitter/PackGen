#include "node.h"
#include <QDebug>

#include "model.h"

Node::Node(QObject *parent, const QString &type)
    : QObject(parent),
      QStandardItem(),
      m_type(type)
{
}

QVariant Node::data(int role) const
{
    if (role == Qt::DisplayRole)
        return m_type;
    else
        return QVariant();
}

QString Node::description()
{
    return m_description;
}

void Node::setDescription(QString name)
{
    m_description = name;
}

void Node::setModel(Model *model)
{
    m_model = model;
}

Model* Node::getModel()
{
    return m_model;
}

void Node::delete_node()
{
    m_model->delete_node(this);
}

void Node::set_need_redraw()
{
    redraw_property_editor = 1;
}

int Node::get_need_redraw()
{
    if (redraw_property_editor == 0)
        return 0;
    redraw_property_editor = 0;
        return 1;
}

bool Node::readJson(const QJsonObject *jsonObj)
{
//  qDebug() << "Read Json";
    QJsonValue jsonVal;
    //int cnt = jsonObj->count();
    QStringList keys = jsonObj->keys();
//  qDebug() << keys;
    foreach (QString key, keys)
    {
        QVariant v = this->property(key.toLatin1().data());
        if (key != "childes")
        {
            QVariant v = this->property(key.toLatin1().data());
            jsonVal = jsonObj->value(key);
            if (v.type() == QVariant::UserType)
            {
                if (strcmp(v.typeName(),"FileString") == 0)
                {
                        FileString s;
                        s.filestring = jsonVal.toString();
                        QVariant a;
                        a.setValue<FileString>(s);
                        this->setProperty(key.toLatin1().data(),a);
                }
                if (strcmp(v.typeName(),"HexString") == 0)
                {
                        HexString h;
                        h.hexstring = jsonVal.toString();
                        QVariant a;
                        a.setValue<HexString>(h);
                        this->setProperty(key.toLatin1().data(),a);
                }
                if (strcmp(v.typeName(),"FlashSize") == 0)
                {
                        FlashSize f;
                        f.selectedsize = static_cast<uint>(jsonVal.toInt());
                        QVariant a;
                        a.setValue<FlashSize>(f);
                        this->setProperty(key.toLatin1().data(),a);
                }
                if (strcmp(v.typeName(), "DualBoot") == 0)
                {
                        DualBoot d;
                        d.dualbootena = static_cast<uint>(jsonVal.toInt());
                        QVariant a;
                        a.setValue<DualBoot>(d);
                        this->setProperty(key.toLatin1().data(),a);
                }
                if (strcmp(v.typeName(), "FpgaType") == 0)
                {
                        FpgaType d;
                        d.selectedfpga = static_cast<uint>(jsonVal.toInt());
                        QVariant a;
                        a.setValue<FpgaType>(d);
                        this->setProperty(key.toLatin1().data(),a);
                }
                if (strcmp(v.typeName(), "VerState") == 0)
                {
                        VerState d;
                        d.selectedVersion = static_cast<uint>(jsonVal.toInt());
                        QVariant a;
                        a.setValue<VerState>(d);
                        this->setProperty(key.toLatin1().data(),a);
                }

            }
            else
            {
                    if (v.type() == QVariant::String)
                        this->setProperty(key.toLatin1().data(),jsonVal.toString());

                    if (v.type() == QVariant::UInt)
                        this->setProperty(key.toLatin1().data(),jsonVal.toInt());
                    if (v.type() == QVariant::Bool)
                        this->setProperty(key.toLatin1().data(),jsonVal.toBool());
            }
//          qDebug() << key;
        }
    }
    return true;
}

bool Node::writeJson(QJsonObject *jsonObj)
{
    const QMetaObject *meta = this->metaObject();
    int cnt = meta->propertyCount();
    for ( int i = 1; i < cnt; i++ )
    {
        QMetaProperty prop = meta->property(i);
        auto v = this->property(prop.name());
        if (v.type() == QVariant::UserType)
        {
            if (strcmp(v.typeName(),"FileString") == 0)
                jsonObj->insert(prop.name(),v.value<FileString>().filestring);
            if (strcmp(v.typeName(),"HexString") == 0)
                jsonObj->insert(prop.name(),v.value<HexString>().hexstring);
            if (strcmp(v.typeName(),"FlashSize") == 0)
            {
                QVariant x = v.value<FlashSize>().selectedsize;
                jsonObj->insert(prop.name(),x.toInt());
            }
            if (strcmp(v.typeName(), "DualBoot") == 0)
            {
                QVariant x = v.value<DualBoot>().dualbootena;
                jsonObj->insert(prop.name(),x.toInt());
            }
            if (strcmp(v.typeName(), "FpgaType") == 0)
            {
                QVariant x = v.value<FpgaType>().selectedfpga;
                jsonObj->insert(prop.name(),x.toInt());
            }
            if (strcmp(v.typeName(), "VerState") == 0)
            {
                QVariant x = v.value<VerState>().selectedVersion;
                jsonObj->insert(prop.name(),x.toInt());
            }

        }
        else
        {
            if (v.type() == QVariant::String)
                jsonObj->insert(prop.name(),v.toString());
            if (v.type() == QVariant::UInt)
                jsonObj->insert(prop.name(),v.toInt());
            if (v.type() == QVariant::Bool)
                jsonObj->insert(prop.name(),v.toBool());
        }
    }
    return true;
}


