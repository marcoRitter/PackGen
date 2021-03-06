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

    Q_PROPERTY(QString output_file_directory READ location WRITE setLocation)
    Q_PROPERTY(QString output_file_name READ filename WRITE setFilename)
    Q_PROPERTY(FpgaType fpgatype READ fpgatype WRITE setFpgatype)
    Q_PROPERTY(FlashSize flash_size READ flash_size WRITE setFlash_size)
    Q_PROPERTY(bool bit_reverse READ bit_reverse WRITE setBit_reverse)
public:
    Masterfile(QObject *parent = nullptr);
    ~Masterfile();

    QString node_type() {return "Masterfile";}

    QString location();
    void setLocation(QString foldername);

    QString get_location();

    FlashSize flash_size();
    void setFlash_size(FlashSize flashsize);

    FpgaType fpgatype();
    void setFpgatype (FpgaType fpgatype);

    QString filename();
    void setFilename(QString filename);

    QVariant updateBitReverse();

    bool bit_reverse();
    void setBit_reverse(bool reverse);

    bool setSrecParameters ();
    bool fillBlanks();
    int runSrec();



    bool readJson(const QJsonObject *jsonObj);
    bool writeJson(QJsonObject *jsonObj);


    void node_menue(QMenu *menu);
signals:
    void setOutInfo (const QString & textToOut, const QColor & color);
    void need_redraw(const QString &name, const QVariant a);

private slots:
    void new_FPGA();
    void new_Firmware();
    void new_File();
    void new_Golden();
    bool generate_masterfile();


private:
//    QFileInfo m_file;
    QString m_filename;
    QString m_location = static_cast<QString>("xHOME/masterfile/");
    FlashSize m_flashsize;
    bool m_bit_reverse = false;
    FpgaType m_fpgatype;

    QStringList m_srecParameters;
    QString m_processOut;

    QColor m_errorColor = QColor("red");
    QColor m_normalColor = QColor("black");
    QColor m_infoColor = QColor("green");

    QAction *pGenerate;
    QAction *pNewFPGA;
    QAction *pNewFirmware;
    QAction *pNewFile;
    QAction *pNewGolden;
    QAction *pDelete;
    QObject *m_parent;
};

#endif // M86_Spartan6_H
