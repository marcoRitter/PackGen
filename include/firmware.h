#ifndef FIRMWARE_H
#define FIRMWARE_H

#include "node.h"
#include <QFileInfo>
#include <QMenu>
#include <QDebug>
#include <QMetaProperty>

#include "customtype.h"

class firmware : public Node
{
    Q_OBJECT

    Q_PROPERTY(FileString filename READ filename WRITE setFilename)
    Q_PROPERTY(QString ver_major READ ver_major WRITE setVer_major)
    Q_PROPERTY(QString ver_minor READ ver_minor WRITE setVer_minor)
    Q_PROPERTY(QString ver_subminor READ ver_subminor WRITE setVer_subminor)
    Q_PROPERTY(VerState verstate READ verstate WRITE setVerstate)

public:

    firmware(QObject *parent = nullptr);
    ~firmware();

    QString node_type() {return"firmware";}

    FileString filename();
    void setFilename(FileString filename);

    QString designnumber();
    void setDesignnumber(QString filename);

    QString ver_major();
    void setVer_major(QString ver_major);

    QString ver_minor();
    void setVer_minor(QString ver_minor);

    QString ver_subminor();
    void setVer_subminor(QString ver_subminor);

    VerState verstate();
    void setVerstate (VerState verstate);

    QString getVerFileName()
    {
        return m_verFileName;
    }
    QString getVerString();
    void setVerFileName()
    {
        FileString fn = filename();
        QString filenm = fn.filestring.section(".",0,0).append("_FW.ver");
        m_verFileName = filenm;
    }
    void node_menue(QMenu *menu);

private:

    FileString m_filename;
    QString m_ver_major = "";
    QString m_ver_minor = "";
    QString m_ver_subminor = "";
    VerState m_verstate;

    QString m_verFileName;
    QAction *pDeleteFirmware;
};

#endif // FIRMWARE_H
