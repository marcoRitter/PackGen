#ifndef Masterfile_H
#define Masterfile_H

#include <QFileInfo>
#include <QMenu>
#include "node.h"

class Masterfile : public Node
{
    Q_OBJECT
    // property to identify the node type
    Q_PROPERTY(QString node_type READ node_type)

    Q_PROPERTY(QString filename READ filename WRITE setFilename)
    Q_PROPERTY(QString ver_major READ ver_major WRITE setVer_major)
    Q_PROPERTY(QString ver_minor READ ver_minor WRITE setVer_minor)
    Q_PROPERTY(QString ver_subminor READ ver_subminor WRITE setVer_subminor)
public:
    Masterfile(QObject *parent = nullptr);

    QString node_type() {return "Masterfile";}

    QString filename();
    void setFilename(QString filename);

    QString ver_major();
    void setVer_major(QString ver_major);

    QString ver_minor();
    void setVer_minor(QString ver_minor);

    QString ver_subminor();
    void setVer_subminor(QString ver_subminor);


    //    virtual bool readJson(const QJsonObject *jsonObj);
    bool readJson(const QJsonObject *jsonObj);
    bool writeJson(QJsonObject *jsonObj);


    void node_menue(QMenu *menu);

private slots:
//    void new_FPGA();
    bool generate_masterfile();


private:
//    QFileInfo m_file;
    QString m_filename;
    QString m_ver_major;
    QString m_ver_minor;
    QString m_ver_subminor;
};

#endif // M86_Spartan6_H
