#ifndef M86_Spartan6_H
#define M86_Spartan6_H

#include <QFileInfo>
#include <QMenu>
#include "node.h"
#include "fpga.h"
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
    Q_PROPERTY(QString ver_major READ ver_major WRITE setVer_major)
    Q_PROPERTY(QString ver_minor READ ver_minor WRITE setVer_minor)
    Q_PROPERTY(QString ver_subminor READ ver_subminor WRITE setVer_subminor)
//    Q_PROPERTY(VER_STATE ver_state READ ver_state WRITE setVer_state)
public:
    M86_Spartan6(QObject *parent = nullptr);
    ~M86_Spartan6();

    QString node_type() {return"M86_Spartan6";}

    FileString location();
    void setLocation(FileString filename);

    QString pkgName();
    void setPkgName (QString pkgname);

    QString ver_major();
    void setVer_major(QString ver_major);

    QString ver_minor();
    void setVer_minor(QString ver_minor);

    QString ver_subminor();
    void setVer_subminor(QString ver_subminor);
/*
    VER_STATE ver_state();
    void setVer_state(VER_STATE ver_state);
*/


    void node_menue(QMenu *menu);

private slots:
    void new_FPGA();
    bool generate_package();


private:
//    QFileInfo m_file;
    QString m_pkgName;
    FileString m_location;
    QString m_ver_major = "";
    QString m_ver_minor = "";
    QString m_ver_subminor = "";
    //VER_STATE m_ver_state = Alpha;

    // menu Actions
    QAction * pGenerate;
    QAction * pNewFPGA;
    QAction * pDelete;

    QObject * m_parent;
};

#endif // M86_Spartan6_H
