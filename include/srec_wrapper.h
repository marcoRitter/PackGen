#ifndef SREC_WRAPPER_H
#define SREC_WRAPPER_H
#include <QString>
#include <QStringList>
#include <QMap>
#include <QObject>
#include "fpga.h"
#include "firmware.h"
#include "version_file.h"

class srec_wrapper
{
public:
    srec_wrapper();
    ~srec_wrapper();

    void setSrecExe (QString filename);
    QString getSrecExe () {return m_srecExe;}

    int runSrec (const QObjectList & parametersParent);

    QString getOutput () {return m_output;}
    QString getRuncmd () {return m_runcmd;}
    QString getOutputFileName(const QFile &inFile);

    int setParametersForSrec (const QObjectList & parametersParent);

private:
    QString m_srecExe;
    QString m_inputFile;
    QString m_outputFile;
    QString m_output;
    QString m_runcmd;
    QStringList m_parameters;


    QMap<QString, QString> m_inputKeys;

};

#endif // SREC_WRAPPER_H
