#ifndef JADE_Package_H
#define JADE_Package_H

#include <QFileInfo>
#include <QMenu>
#include "node.h"
#include "JADE_Moduledescription.h"
#include "JADE_History.h"



class JADE_Package : public Node
{
    Q_OBJECT
    // property to identify the node type
    Q_PROPERTY(QString node_type READ node_type)

    Q_PROPERTY(QString module_name READ module_name WRITE setModule_name)

    Q_PROPERTY(QString filename READ filename WRITE setFilename)
    Q_PROPERTY(uint ver_major READ ver_major WRITE setVer_major)
    Q_PROPERTY(uint ver_minor READ ver_minor WRITE setVer_minor)
    Q_PROPERTY(uint ver_subminor READ ver_subminor WRITE setVer_subminor)
public:
    JADE_Package(QObject *parent = nullptr);

    QString node_type() {return"JADE_Package";}

    QString filename();
    void setFilename(QString filename);

    QString module_name();
    void setModule_name(QString module_name);

    uint ver_major();
    void setVer_major(uint ver_major);

    uint ver_minor();
    void setVer_minor(uint ver_minor);

    uint ver_subminor();
    void setVer_subminor(uint ver_subminor);


//    bool readJson(const QJsonObject *jsonObj);
//    bool writeJson(QJsonObject *jsonObj);

    void node_menue(QMenu *menu);

private slots:
    bool generate_package();
    void new_JADE_Moduledescription();
    void new_JADE_Historyfile();
    void new_JADE_FPGA();

private:
//    QFileInfo m_file;
    QString m_filename = "";
    QString m_module_name = "";
    uint m_ver_major = 0;
    uint m_ver_minor = 0;
    uint m_ver_subminor = 0;
};

#endif // JADE_Package_H
