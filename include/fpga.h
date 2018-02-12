#ifndef FPGA_H
#define FPGA_H

#include "node.h"
#include <QFileInfo>
#include <QMenu>
#include <QDebug>
#include <QMetaProperty>
#include <QProcess>

#include "customtype.h"

class Fpga : public Node
{
    Q_OBJECT
    // property to identify the node type
//  Q_PROPERTY(QString node_type READ node_type)

    Q_PROPERTY(FpgaType fpgatype READ fpgatype WRITE setFpgatype)
    Q_PROPERTY(FileString filename READ filename WRITE setFilename)
    Q_PROPERTY(QString designnumber READ designnumber WRITE setDesignnumber)
    Q_PROPERTY(QString revision READ revision WRITE setRevision)
    Q_PROPERTY(QString testversion READ testversion WRITE setTestversion)
    Q_PROPERTY(DualBoot dualboot READ dualboot WRITE setDualBoot )

    Q_PROPERTY(FlashSize flash_size READ flash_size WRITE setFlash_size)
    Q_PROPERTY(QString start_addr READ start_addr WRITE setStart_addr)

public:
    Fpga(QObject *parent = nullptr);
    ~Fpga();

//  QString node_type() {return"FPGA";}

    FpgaType fpgatype();
    void setFpgatype (FpgaType fpgatype);

    FileString filename();
    void setFilename(FileString filename);

    QString designnumber();
    void setDesignnumber(QString filename);

    QString revision();
    void setRevision(QString revision);

    QString testversion();
    void setTestversion(QString testversion);

//  HexString start_addr();
//  void setStart_addr(HexString start_addr);

    QString start_addr();
    void setStart_addr(QString start_addr);

    FlashSize flash_size();
    void setFlash_size(FlashSize flashsize);

    DualBoot dualboot();
    void setDualBoot (DualBoot dualboot);

    QVariant updateStartAddress();
    void node_menue(QMenu *menu);

    QString getVerString();

    QString getVerFileName()
    {
        return m_verFileName;
    }
    void setVerFileName()
    {
        FileString fn = filename();
        QString filenm = fn.filestring.section(".",0,0).append("_FPGA.ver");
        m_verFileName = filenm;
    }

    QString getHexFileName()
    {
        return m_hexFileName;
    }
    void setHexFileName()
    {
        FileString fn = filename();
        QString filenm = fn.filestring.section(".",0,0).append("_FPGA.h86");
        m_hexFileName = filenm;
    }

    void setSrecParameters ();
    QStringList getSrecParameters ();
    int runSrec();

signals:
    void need_redraw(const QString &name, const QVariant a);


    /*
    inline int getPropId (const char* propName) {
        qDebug()<< "row count = " << this->metaObject()->propertyCount();
        int cnt = this->metaObject()->propertyCount();
        for (int i = 0; i < cnt; i++)
        {
            if (!strcmp(this->metaObject()->property(i).name(), propName))
                return i;
        }
        return 0;

    }
    */

private:
//    QFileInfo m_file;
    FpgaType m_fpgatype;
    FileString m_filename;
    QString m_designnumber = "";
    QString m_revision = "";
    QString m_testversion = "";
//  HexString m_start_addr;
    QString m_start_addr;
    FlashSize m_flashsize;
//  bool m_with_golden_reference;
    DualBoot m_dualboot;

    QString m_verFileName;
    QString m_hexFileName;
    QStringList m_srecParameters;
    QAction *pDeleteFPGA;

    QObject * m_parent;

};

#endif // FPGA_H
