#include <QDebug>
#include <QString>
#include <QProcess>
#include <QTextCodec>
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

QString srec_wrapper::getOutputFileName(const QString &inFile)
{
    QString a = "a";
    return a;
}

int srec_wrapper::runSrec(const QStringList & parameters)
{
    QProcess *process = new QProcess(0);
    m_runcmd.append(m_srecExe);
    for (const auto & p : parameters)
    {
        m_runcmd.append(" ");
        m_runcmd.append(p);
    }

    process->start(m_srecExe, parameters, QIODevice::ReadWrite);
//  process->start("echo", parameters, QIODevice::ReadWrite);

    if (!process->waitForStarted())
        qDebug() << "error by executing srec_cat";
    if (!process->waitForFinished())
        qDebug() << "srec_cat failed";

    int ret = process->exitCode();
    delete process;
    return ret;
}
