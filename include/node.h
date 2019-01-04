#ifndef NODE_H
#define NODE_H

#include <QStandardItem>
#include <QMenu>
//#include "customtype.h"
#include <QDebug>
#include <QMetaProperty>

class Model;

class Node : public QObject, public QStandardItem
{


    Q_OBJECT
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(QString node_type READ node_type)
public:
    explicit Node(QObject *parent, const QString &type = QString());

    virtual QVariant data(int role) const;
    virtual QString node_type() {return m_type;}

    QString description();
    void setDescription(QString description);
    void setModel(Model *model);
    void setType (QString type) {m_type = type;}
    QString getType() {return m_type;}

//    QString m_type;

    virtual void node_menue(QMenu *menu) = 0;
//    virtual bool readJson(const QJsonObject *jsonObj) = 0;
//    virtual bool writeJson(QJsonObject *jsonObj) = 0;
    bool readJson(const QJsonObject *jsonObj);
    bool writeJson(QJsonObject *jsonObj);


    Model *getModel();

    void set_need_redraw();
    int get_need_redraw();

public slots:
    void delete_node();

protected:
    Model *m_model;

private:
    int redraw_property_editor = 0;
    QString m_description;
    QString m_type;
};

#endif // NODE_H
