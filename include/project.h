
#pragma once
#ifndef PROJECT_H
#define PROJECT_H

#include "node.h"
#include "M86_Spartan6.h"
#include "JADE_Package.h"
#include "Masterfile.h"
#include "goldenReference.h"
#include "model.h"
#include <QFileInfo>
#include <QMenu>
#include "firmware.h"
#include "fpga.h"
#include "file.h"


class Project : public Node
{
    Q_OBJECT
    // property to identify the node type
//  Q_PROPERTY(QString node_type READ node_type WRITE setNode_type)

    // Properties for this node
    Q_PROPERTY(bool JadeProject READ jade WRITE setJade)
    Q_PROPERTY(FileString working_directory READ project_directory WRITE setProject_directory)
    Q_PROPERTY(FileString srec_cat READ srec_cat WRITE setSrec_cat)
    Q_PROPERTY(FileString logichdr READ logichdr WRITE setLogichdr)
    Q_PROPERTY(FileString mbind READ mbind WRITE setMbind)

public:
    explicit Project(QObject *parent = nullptr);
    ~Project();
//#if 0
  //  explicit Project();
   // explicit Project(QObject *parent = nullptr, QString projectName = "Project");
//#endif

    QString node_type() {return"Project";}
//  void setNode_type(QString setNodeType)
//  {
//
//  };

    bool jade();
    void setJade(bool jade);

    FileString project_directory();
    void setProject_directory(FileString filename);

    FileString srec_cat();
    void setSrec_cat(FileString filename);

    FileString logichdr();
    void setLogichdr(FileString filename);

    FileString mbind();
    void setMbind(FileString filename);

//  bool readJson(const QJsonObject *jsonObj);
//  bool writeJson(QJsonObject *jsonObj);

    void node_menue(QMenu *menu);


private slots:
    void new_M86_Spartan6();
    void new_Masterfile();
    void new_goldenRef();
    void readme_all();

    /*
    void new_JADE_Package();  
    */

private:
    QString projectName;
    bool m_jade = false;
    FileString m_project_directory;
    FileString m_srecCat = static_cast<QString>("//pc011/tools/utils/srec_cat.exe");
    FileString m_logichdr =static_cast<QString>("//pc011/tools/utils/logichdr.exe");
    FileString m_mbind =static_cast<QString>("//pc011/tools/utils/mbind.exe");

    QAction *pNewSpartan;
    QAction *pNewMasterfile;
    QAction *pNewgoldenRef;
    QAction *pNewJadeFile;
};
#endif // PROJECT_H

