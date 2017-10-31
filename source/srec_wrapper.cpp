#include <QDebug>
#include <QString>
#include <QProcess>
#include <qiterator.h>
#include "srec_wrapper.h"


srec_wrapper::srec_wrapper()
{

}

srec_wrapper::~srec_wrapper()
{

}

void srec_wrapper::setSrecExe(QString filename)
{
    m_srecExe = filename;
}

int srec_wrapper::runSrec(const QMap<QString, QString>& srecParams, QString *output)
{
    QString srecRunCmd = "";
    qDebug() << "srec_exe " << m_srecExe;
    QMap<QString,QString>::const_iterator i;
    srecRunCmd.append(m_srecExe); srecRunCmd.append(" ");
    if (srecParams.contains("input"))
        srecRunCmd.append(srecParams.value("input"));
    srecRunCmd.append(" --binary ");
    if (srecParams.contains("--offset"))
    {
        srecRunCmd.append("--offset ");
        srecRunCmd.append(srecParams.value("--offset"));
    }
    if (srecParams.contains("--bit_reverse"))
    {
        srecRunCmd.append(" --bit_reverse ");
    }
    if (srecParams.contains("--output"))
    {
        srecRunCmd.append(" --output ");
        srecRunCmd.append(srecParams.value("--output"));
    }
    srecRunCmd.append(" --intel");


    for (i = srecParams.begin(); i != srecParams.end(); ++i)
        qDebug() << "key = " << i.key() << " value = " << i.value();
    qDebug() << srecRunCmd;

    QProcess *process = new QProcess(0);
    process->start(srecRunCmd, QIODevice::ReadWrite);

    if (!process->waitForStarted())
        qDebug() << "error by starting logichdr.exe";
    if (!process->waitForFinished())
        qDebug() << "logichdr.exe failed";

    QByteArray result = process->readAllStandardOutput();
    qDebug() << result;
    return process->exitStatus();
}
