#ifndef Masterfile_H
#define Masterfile_H

#include <QFileInfo>
#include <QMenu>
#include "node.h"
#include "firmware.h"
#include "fpga.h"
#include "file.h"

#include "customtype.h"

class Masterfile : public Node
{
    Q_OBJECT
    // property to identify the node type
    Q_PROPERTY(QString node_type READ node_type)

    Q_PROPERTY(QString filename READ filename WRITE setFilename)
    Q_PROPERTY(FileString location READ location WRITE setLocation)
    Q_PROPERTY(QString ver_major READ ver_major WRITE setVer_major)
    Q_PROPERTY(QString ver_minor READ ver_minor WRITE setVer_minor)
    Q_PROPERTY(QString ver_subminor READ ver_subminor WRITE setVer_subminor)
public:
    Masterfile(QObject *parent = nullptr);
    ~Masterfile();

    QString node_type() {return "Masterfile";}

    FileString location();
    void setLocation(FileString foldername);

    QString filename();
    void setFilename(QString filename);

    QString ver_major();
    void setVer_major(QString ver_major);

    QString ver_minor();
    void setVer_minor(QString ver_minor);

    QString ver_subminor();
    void setVer_subminor(QString ver_subminor);

    void setSrecParameters ();
    int runSrec();


    bool readJson(const QJsonObject *jsonObj);
    bool writeJson(QJsonObject *jsonObj);


    void node_menue(QMenu *menu);
signals:
    void setOutInfo (const QString & textToOut, const QColor & color);
private slots:
    void new_FPGA();
    void new_Firmware();
    void new_File();
    bool generate_masterfile();


private:
//    QFileInfo m_file;
    QString m_filename;
    FileString m_location;
    QString m_ver_major;
    QString m_ver_minor;
    QString m_ver_subminor;

    QStringList m_srecParameters;
    QString m_processOut;

    QColor m_errorColor = QColor("red");
    QColor m_normalColor = QColor("black");
    QColor m_infoColor = QColor("green");

    QAction *pGenerate;
    QAction *pNewFPGA;
    QAction *pNewFirmware;
    QAction *pNewFile;
    QAction *pDelete;
    QObject *m_parent;
};

#endif // M86_Spartan6_H
