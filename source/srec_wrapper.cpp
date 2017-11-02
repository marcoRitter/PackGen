#include <QDebug>
#include <QString>
#include <QProcess>
#include <QTextCodec>
#include <QFileInfo>
#include "srec_wrapper.h"
#include "customtype.h"


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

QString srec_wrapper::getOutputFileName(const QFile &inFile)
{
    QFileInfo fileinfo(inFile.fileName());
    QString fileName = (QString)fileinfo.fileName();
    return fileName.section(".",0,0);
}

int srec_wrapper::setParametersForSrec (const QObjectList & parametersParent)
{

    for (auto const & childOfFpga : parametersParent)
    {
        if (childOfFpga->objectName()=="FPGA")
        {
            m_parameters.append(childOfFpga->property("filename").value<FileString>().filestring);
            m_parameters.append("--binary");
            m_parameters.append("--offset");
            m_parameters.append(childOfFpga->property("start_addr").value<HexString>().hexstring);
            if (childOfFpga->property("fpgatype").value<FpgaType>().selectedfpga)
                m_parameters.append("--bit_reverse");
            m_parameters.append("--output");
            QString outputFile;
            outputFile.append(childOfFpga->property("filename").value<FileString>().filestring);
            // TODO:
            // replace childOfFpga->parent()->property...
            m_parameters.append(childOfFpga->parent()->property("location").value<FileString>().filestring + "/" +
                              getOutputFileName((QFile)outputFile) + ".hex");
            m_parameters.append("--intel");
        }
    }
    return 1;
}

int srec_wrapper::runSrec()
{
    QProcess *process = new QProcess(0);
    m_runcmd.append(m_srecExe);
/*
    for (const auto & p : m_parameters)
    {
        m_runcmd.append(" ");
        m_runcmd.append(p);
    }
*/

    process->start(m_srecExe, m_parameters, QIODevice::ReadWrite);
//  process->start("echo", parameters, QIODevice::ReadWrite);

    if (!process->waitForStarted())
        qDebug() << "error by executing srec_cat";
    if (!process->waitForFinished())
        qDebug() << "srec_cat failed";

    int ret = process->exitCode();
    delete process;
    return ret;
}
