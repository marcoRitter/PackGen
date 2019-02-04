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

    Q_PROPERTY(QString outputFile_name READ name WRITE setName)
    Q_PROPERTY(FileString inputFile_directory READ filename WRITE setFilename)
    Q_PROPERTY(FpgaType fpgatype READ fpgatype WRITE setFpgatype)
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

    QString name();
    void setName(QString name);

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

    QString getHexFileName()
    {
        return m_hexFileName;
    }
    QString getMchFileName()
    {
        return m_mchFileName;
    }
    void setHexFileName(QString &path)
    {
        /*
        FileString fn = filename();
        QString filenm = fn.filestring.section(".",0,0).append("_FPGA.h86");
        QString mchfilenm = fn.filestring.section("/",0,-2);
        mchfilenm.append("/");
        mchfilenm.append(fn.filestring.section("/",-1,-1).section(".",0,0).left(8).append(".mch"));
        */
        QString h86FileName;

        if(m_parent->property("module_type").value<ModuleType>().selectedmodultype == 0)
        {
            h86FileName = path + "/" + this->name() + ".h86";
        }
        else {
            h86FileName = path + "/" + this->name() + ".bin";
        }
        QString mchFileName = path + "/" + this->name() + ".mch";
        QString verFileName = path + "/" + this->name() +"_FPGA"+ ".ver";

        qDebug() << "h86 file = " << h86FileName;
        qDebug() << "mch file = " << mchFileName;

        m_hexFileName = h86FileName;
        m_mchFileName = mchFileName;
        m_verFileName = verFileName;
    }
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

    QString getProcessOut () {
        return m_processOut;
    }
    void setSrecParameters ();
    QStringList getSrecParameters ();
    int runSrec();
    int runLogichdr();

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
    QString m_name;
    // input fpga bit file
    FileString m_filename;
    QString m_designnumber = "";
    QString m_revision = "";
    QString m_testversion = "";
    QString m_typecode;
    QString m_variant;
//  HexString m_start_addr;
    QString m_start_addr = "0x000000";
    FlashSize m_flashsize;
//  bool m_with_golden_reference;
    DualBoot m_dualboot;

    // version file name to create
    QString m_verFileName;
    // output file name for srec
    QString m_hexFileName;
    // output file name for logichdr
    QString m_mchFileName;

    QStringList m_srecParameters;
    QAction *pDeleteFPGA;

    QObject * m_parent;
    QString m_processOut;

};

#endif // FPGA_H
