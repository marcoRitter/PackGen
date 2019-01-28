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
    Q_PROPERTY(QString start_addr READ start_addr WRITE setStart_addr)

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

    QString start_addr();
    void setStart_addr(QString start_addr);

    QString getVerFileName()
    {
        return m_verFileName;
    }
    QString getMchFileName()
    {
        return m_mchFileName;
    }
    QString getVerString();
    QString getProcessOut() {
        return m_processOut;
    }
    void setVerFileName(QString &path)
    {
        /*
        FileString fn = filename();
        QString filenm = fn.filestring.section(".",0,0).append("_FW.ver");
        QString mchfilenm = fn.filestring.section("/",0,-2);
        mchfilenm.append("/");
        mchfilenm.append(fn.filestring.section("/",-1,-1).section(".",0,0).left(8).append(".mch"));
        */
        QString verFileName = path + "/" + this->description() + ".ver";
        QString mchFileName = path + "/" + this->description() + ".mch";
        m_verFileName = verFileName;
        m_mchFileName = mchFileName;
    }
    void node_menue(QMenu *menu);
    int runLogichdr();

    QString getVariant ()
    {
        return m_variant;
    }
    QString getTypecode ()
    {
        return m_typecode;
    }
    void setVariant (QString var)
    {
        m_variant = var;
    }
    void setTypecode (QString typeCode)
    {
        m_typecode = typeCode;
    }

private:

    FileString m_filename;
    QString m_ver_major = "";
    QString m_ver_minor = "";
    QString m_ver_subminor = "";
    QString m_typecode;
    QString m_variant;
    VerState m_verstate;
    QString m_start_addr = "0x000000";

    QString m_verFileName;
    QString m_mchFileName;
    QAction *pDeleteFirmware;

    QString m_processOut;
    QObject * m_parent;
};

#endif // FIRMWARE_H
