#include <QDebug>
#include <QString>
#include <QProcess>
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

int srec_wrapper::runSrec(const QStringList & parameters, QString *output)
{
    qDebug() << "srec_exe " << m_srecExe;

        qDebug() << parameters;

    QProcess *process = new QProcess(0);
//  process->start(m_srecExe, parameters, QIODevice::ReadWrite);
//  process->start("echo ", parameters, QIODevice::ReadWrite);
    process->start("echo ", parameters, QIODevice::ReadWrite);


  if (!process->waitForStarted())
      qDebug() << "error by executing srec_cat";
  if (!process->waitForFinished())
      qDebug() << "srec_cat failed";

   QByteArray result = process->readAllStandardOutput();
   qDebug() << result;
   return process->exitStatus();
}
