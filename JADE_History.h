#ifndef JADE_History_H
#define JADE_History_H

#include "node.h"
#include <QFileInfo>
#include <QMenu>



class JADE_History : public Node
{
    Q_OBJECT
    // property to identify the node type
    Q_PROPERTY(QString node_type READ node_type)

    Q_PROPERTY(QString filename READ filename WRITE setFilename)
    Q_PROPERTY(uint ver_major READ ver_major WRITE setVer_major)
    Q_PROPERTY(uint ver_minor READ ver_minor WRITE setVer_minor)
    Q_PROPERTY(uint ver_subminor READ ver_subminor WRITE setVer_subminor)
public:
    JADE_History(QObject *parent = nullptr);

    QString node_type() {return"JADE_History";}

    QString filename();
    void setFilename(QString filename);

    uint ver_major();
    void setVer_major(uint ver_major);

    uint ver_minor();
    void setVer_minor(uint ver_minor);

    uint ver_subminor();
    void setVer_subminor(uint ver_subminor);


    //    virtual bool readJson(const QJsonObject *jsonObj);
//    bool readJson(const QJsonObject *jsonObj);
//    bool writeJson(QJsonObject *jsonObj);

    void node_menue(QMenu *menu);

private:
//    QFileInfo m_file;
    QString m_filename ="";
    uint m_ver_major = 0;
    uint m_ver_minor = 0;
    uint m_ver_subminor = 0;
};

#endif // JADE_History_H
