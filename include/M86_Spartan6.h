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
    Q_PROPERTY(ModuleType module_type READ module_type WRITE setModule_type)
    Q_PROPERTY(QString output_file_directory READ location WRITE setLocation)
    Q_PROPERTY(QString output_file_name READ pkgName WRITE setPkgName)
    Q_PROPERTY(QString typecode READ typecode WRITE setTypeCode)
    Q_PROPERTY(QString variant READ variant WRITE setVariant)
    Q_PROPERTY(QString required_driver_version READ required_driver_version WRITE setRequired_driver_version)
    Q_PROPERTY(QString ver_major READ ver_major WRITE setVer_major)
    Q_PROPERTY(QString ver_minor READ ver_minor WRITE setVer_minor)
    Q_PROPERTY(QString ver_subminor READ ver_subminor WRITE setVer_subminor)
    Q_PROPERTY(VerState verstate READ verstate WRITE setVerstate)

//    Q_PROPERTY(VER_STATE ver_state READ ver_state WRITE setVer_state)
public:
    M86_Spartan6(QObject *parent = nullptr);
    ~M86_Spartan6();

    QString node_type() {return"M86";}

    ModuleType module_type();
    void setModule_type(ModuleType module);

    QString location();
    void setLocation(QString foldername);

    QString pkgName();
    void setPkgName (QString pkgname);

    QString required_driver_version();
    void setRequired_driver_version (QString required_driver_version);

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


    QString getlocation();
/*
    VER_STATE ver_state();
    void setVer_state(VER_STATE ver_state);
*/

    QString getVerFileName()
    {
        return m_verFileName;
    }

    QString getVerString();
    void setVerFileName();

    QString getScrFileName ()
    {
        return m_scriptFileName;
    }

    void setScrFileName();

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
    QString m_location= static_cast<QString>("xHOME/m86/");
    QString m_ver_major = "";
    QString m_ver_minor = "";
    QString m_ver_subminor = "";
    QString m_typecode = "";
    QString m_variant = "";
    VerState m_verstate;
    QString m_verFileName;
    QString m_scriptFileName;
    ModuleType m_module_type;
    QString m_required_driver_version = static_cast<QString>("V0.00.00 Alpha");

    // menu Actions
    QAction * pGenerate;
    QAction * pNewFPGA;
    QAction * pNewFirmware;
    QAction * pDelete;
    QObject * m_parent;
};

#endif // M86_Spartan6_H
