#ifndef FPGA_H
#define FPGA_H

#include "node.h"
#include <QFileInfo>
#include <QMenu>
#include <QDebug>
#include <QMetaProperty>

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
    Q_PROPERTY(HexString start_addr READ start_addr WRITE setStart_addr)

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

    HexString start_addr();
    void setStart_addr(HexString start_addr);

    FlashSize flash_size();
    void setFlash_size(FlashSize flashsize);

    DualBoot dualboot();
    void setDualBoot (DualBoot dualboot);

    QVariant updateStartAddress();
    void node_menue(QMenu *menu);

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
    HexString m_start_addr;
    FlashSize m_flashsize;
//  bool m_with_golden_reference;
    DualBoot m_dualboot;

    QAction *pDeleteFPGA;

};

#endif // FPGA_H
