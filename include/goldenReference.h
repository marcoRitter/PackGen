#ifndef GOLDEN_REF_H
#define GOLDEN_REF_H

#include <QFileInfo>
#include <QMenu>
#include "node.h"
#include "customtype.h"

class goldenReference : public Node
{
    Q_OBJECT

    Q_PROPERTY(QString goldenRef_name READ filename WRITE setFilename)
    Q_PROPERTY(FileString location READ location WRITE setLocation)
    Q_PROPERTY(FileString goldenRef_file READ goldenRef_file WRITE setGoldenRef_file)
    Q_PROPERTY(FileType file_type READ file_type WRITE setFile_type)
    Q_PROPERTY(QString version READ version WRITE setVersion)
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

    FileString goldenRef_file();
    void setGoldenRef_file(FileString file);

    QString filename();
    void setFilename(QString filename);

    FileString location();
    void setLocation(FileString location);

    FileType file_type();
    void setFile_type(FileType filetype);

    QString version();
    void setVersion(QString version);

    FlashSize flash_size();
    void setFlash_size(FlashSize flashsize);

    QString start_addr();
    void setStart_addr(QString start_addr);

    QVariant updateStartAddress();
    void node_menue(QMenu *menu);

    bool setSrecParameters ();
    int runSrec();
    bool creatHeader();


signals:
    void need_redraw(const QString &name, const QVariant a);

private:
    FileString m_goldenRef_file;
    QString m_filename;
    FileString m_location;
    FileType m_file_type;
    QString m_version;
    FlashSize m_flash_size;
    FpgaType m_fpgatype;
    QString m_start_addr;
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
