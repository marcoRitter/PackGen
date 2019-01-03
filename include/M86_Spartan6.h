#ifndef M86_Spartan6_H
#define M86_Spartan6_H

#include <QFileInfo>
#include <QMenu>
#include "node.h"
#include "fpga.h"
#include "firmware.h"

#include "customtype.h"

/*

enum VER_STATE
{
    Alpha,
    Beta,
    Release
};

Q_DECLARE_METATYPE(VER_STATE)
*/
class M86_Spartan6 : public Node
{
    Q_OBJECT
    // property to identify the node type
//  Q_PROPERTY(QString node_type READ node_type)

    Q_PROPERTY(QString pkgName READ pkgName WRITE setPkgName)
    Q_PROPERTY(FileString location READ location WRITE setLocation)
    Q_PROPERTY(QString typecode READ typecode WRITE setTypeCode)
    Q_PROPERTY(QString variant READ variant WRITE setVariant)
    Q_PROPERTY(QString ver_major READ ver_major WRITE setVer_major)
    Q_PROPERTY(QString ver_minor READ ver_minor WRITE setVer_minor)
    Q_PROPERTY(QString ver_subminor READ ver_subminor WRITE setVer_subminor)
    Q_PROPERTY(VerState verstate READ verstate WRITE setVerstate)
//    Q_PROPERTY(VER_STATE ver_state READ ver_state WRITE setVer_state)
public:
    M86_Spartan6(QObject *parent = nullptr);
    ~M86_Spartan6();

    QString node_type() {return"M86";}

    FileString location();
    void setLocation(FileString foldername);

    QString pkgName();
    void setPkgName (QString pkgname);

    QString ver_major();
    void setVer_major(QString ver_major);

    QString ver_minor();
    void setVer_minor(QString ver_minor);

    QString ver_subminor();
    void setVer_subminor(QString ver_subminor);

    QString typecode();
    void setTypeCode (QString typeCode);

    QString variant();
    void setVariant (QString variant);

    VerState verstate();
    void setVerstate (VerState verstate);

/*
    VER_STATE ver_state();
    void setVer_state(VER_STATE ver_state);
*/

    QString getVerFileName()
    {
        return m_verFileName;
    }

    QString getVerString();
    void setVerFileName()
    {
        FileString fn = location();
        QString filenm = (fn.filestring);
        filenm.append("/");
        filenm.append(pkgName());
        filenm.append(".ver");
        m_verFileName = filenm;
    }

    QString getScrFileName ()
    {
        return m_scriptFileName;
    }

    void setScrFileName()
    {
        FileString fn = location();
        QString filenm = "";
        filenm.append(fn.filestring);
        filenm.append("/");
        filenm.append(pkgName());
        filenm.append(".mbs");
        m_scriptFileName = filenm;
    }

    void node_menue(QMenu *menu);
    int runMbind ();
signals:
    void setOutInfo (const QString & textToOut, const QColor & color);

private slots:
    void new_FPGA();
    void new_Firmware();
    bool generate_package();


private:
//    QFileInfo m_file;
    QColor m_errorColor = QColor("red");
    QColor m_normalColor = QColor("black");
    QColor m_infoColor = QColor("green");

    QString m_pkgName;
    QString m_outPkgName;
    FileString m_location;
    QString m_ver_major = "";
    QString m_ver_minor = "";
    QString m_ver_subminor = "";
    QString m_typecode = "";
    QString m_variant = "";
    VerState m_verstate;
    QString m_verFileName;
    QString m_scriptFileName;

    // menu Actions
    QAction * pGenerate;
    QAction * pNewFPGA;
    QAction * pNewFirmware;
    QAction * pDelete;
    QObject * m_parent;
};

#endif // M86_Spartan6_H
