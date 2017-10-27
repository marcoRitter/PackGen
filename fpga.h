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

    Q_PROPERTY(FileString filename READ filename WRITE setFilename)
    Q_PROPERTY(QString designnumber READ designnumber WRITE setDesignnumber)
    Q_PROPERTY(QString ver_major READ ver_major WRITE setVer_major)
    Q_PROPERTY(QString ver_minor READ ver_minor WRITE setVer_minor)
//  Q_PROPERTY(bool with_golden_reference READ with_golden_reference WRITE setWith_golden_reference)
    Q_PROPERTY(DualBoot dualboot READ dualboot WRITE setDualBoot )

    Q_PROPERTY(FlashSize flash_size READ flash_size WRITE setFlash_size)
    Q_PROPERTY(HexString start_addr READ start_addr WRITE setStart_addr)

public:
    Fpga(QObject *parent = nullptr);
    ~Fpga();

//  QString node_type() {return"FPGA";}

    FileString filename();
    void setFilename(FileString filename);

    QString designnumber();
    void setDesignnumber(QString filename);

    QString ver_major();
    void setVer_major(QString ver_major);

    QString ver_minor();
    void setVer_minor(QString ver_minor);

    HexString start_addr();
    void setStart_addr(HexString start_addr);

    FlashSize flash_size();
    void setFlash_size(FlashSize flashsize);

//  bool with_golden_reference();
//  void setWith_golden_reference(bool with_golden_reference);

    DualBoot dualboot();
    void setDualBoot (DualBoot dualboot);

    void node_menue(QMenu *menu);

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


private:
//    QFileInfo m_file;
    FileString m_filename;
    QString m_designnumber = "";
    QString m_ver_major = "";
    QString m_ver_minor = "";
    HexString m_start_addr;
    FlashSize m_flashsize;
//  bool m_with_golden_reference;
    DualBoot m_dualboot;

    QAction *pDeleteFPGA;

};

#endif // FPGA_H
