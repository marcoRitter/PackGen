#ifndef JADE_Moduledescription_H
#define JADE_Moduledescription_H

#include "node.h"
#include <QFileInfo>
#include <QMenu>



class JADE_Moduledescription : public Node
{
    Q_OBJECT
    // property to identify the node type
    Q_PROPERTY(QString node_type READ node_type)

    Q_PROPERTY(QString filename READ filename WRITE setFilename)
    Q_PROPERTY(quint32 ver_major READ ver_major WRITE setVer_major)
    Q_PROPERTY(quint32 ver_minor READ ver_minor WRITE setVer_minor)
    Q_PROPERTY(quint32 ver_subminor READ ver_subminor WRITE setVer_subminor)
public:
    JADE_Moduledescription(QObject *parent = nullptr);

    QString node_type() {return"JADE_Moduledescription";}

    QString filename();
    void setFilename(QString filename);

    quint32 ver_major();
    void setVer_major(quint32 ver_major);

    quint32 ver_minor();
    void setVer_minor(quint32 ver_minor);

    quint32 ver_subminor();
    void setVer_subminor(quint32 ver_subminor);


    //    virtual bool readJson(const QJsonObject *jsonObj);
    bool readJson(const QJsonObject *jsonObj);
    bool writeJson(QJsonObject *jsonObj);

    void node_menue(QMenu *menu);

private:
//    QFileInfo m_file;
    QString m_filename ="";
    quint32 m_ver_major = 0;
    quint32 m_ver_minor = 0;
    quint32 m_ver_subminor = 0;
};

#endif // JADE_Moduledescription_H
