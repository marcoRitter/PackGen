#ifndef GOLDEN_REF_H
#define GOLDEN_REF_H

#include <QFileInfo>
#include <QMenu>
#include "node.h"
#include "customtype.h"

class goldenReference : public Node
{
    Q_OBJECT

    //TODO Properties anpassen (Namensgebung, entfernene,..)
    Q_PROPERTY(QString output_file_directory READ location WRITE setLocation)
    Q_PROPERTY(QString output_file_name READ filename WRITE setFilename)
    Q_PROPERTY(QString input_file READ goldenRef_file WRITE setGoldenRef_file)
    Q_PROPERTY(FileType inputFile_type READ file_type WRITE setFile_type)
    Q_PROPERTY(FlashSize flash_size READ flash_size WRITE setFlash_size)
    Q_PROPERTY(FpgaType fpgatype READ fpgatype WRITE setFpgatype)
    Q_PROPERTY(QString start_addr READ start_addr WRITE setStart_addr)

signals:
    void setOutInfo(const QString & textToOut, const QColor & color);
private slots:
    bool generate_goldenRef();

public:
    goldenReference(QObject * parent = nullptr);
    ~goldenReference();

    QString node_type() {return "goldenReference";}

    FpgaType fpgatype();
    void setFpgatype (FpgaType fpgatype);

    QString goldenRef_file();
    void setGoldenRef_file(QString file);

    QString filename();
    void setFilename(QString filename);

    QString location();
    void setLocation(QString location);

    FileType file_type();
    void setFile_type(FileType filetype);

    FlashSize flash_size();
    void setFlash_size(FlashSize flashsize);

    QString start_addr();
    void setStart_addr(QString start_addr);

    QVariant updateStartAddress();
    void node_menue(QMenu *menu);

    QString getInPath();
    QString getOutPath();

    bool setSrecParameters ();
    int runSrec();
    bool creatHeader();


signals:
    void need_redraw(const QString &name, const QVariant a);

private:
    QString m_goldenRef_file = static_cast<QString>("xHOME/");
    QString m_filename;
    QString m_location = static_cast<QString>("xHOME/golden/");
    FileType m_file_type;
    FlashSize m_flash_size;
    FpgaType m_fpgatype;
    QString m_start_addr = "0x000000";
    QString jumpCommend_addr;
    QString jumpCommend_fill;

    QStringList m_srecParameters;
    QString m_processOut;

    QColor m_errorColor = QColor("red");
    QColor m_normalColor = QColor("black");
    QColor m_infoColor = QColor("green");


    QAction *pGenerate;
    QAction *pDelete;
    QObject *m_parent;
};

#endif // GOLDEN_REF_H
