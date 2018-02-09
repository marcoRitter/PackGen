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
//  qDebug() << "file name = " << fileName.section(".", 0,0);
    return fileName.section(".",0,0);
}

int srec_wrapper::setParametersForSrec (const QObjectList & parametersParent)
{

    for (auto const & childOfFpga : parametersParent)
    {
        if (childOfFpga->objectName()=="FPGA")
        {
            Fpga *fpga = (Fpga*)childOfFpga;
//          qDebug() <<"fpga ver string = " << fpga->getVerString();
            fpga->setVerFileName();
            fpga->setHexFileName();
//          qDebug() << "ver filename = " << fpga->getVerFileName();
//          qDebug() << "h86 file = " << fpga->getHexFileName();
            if (!versionFileCreate(fpga->getVerFileName(),fpga->getVerString()))
            {
                qDebug() << "error by ver file creating";
                return 0;
            }

            QFileInfo bitFile = childOfFpga->property("filename").value<FileString>().filestring;
            if (!(bitFile.exists() && bitFile.isFile()))
            {
                qDebug() << bitFile.filePath() << "file doesn't exist";
                m_output = bitFile.filePath() + " file doesn't exist";
                return 0;
            }
            m_parameters.append(bitFile.filePath());
            m_parameters.append("--binary");
            m_parameters.append("--offset");
            m_parameters.append(childOfFpga->property("start_addr").value<QString>());
            if (childOfFpga->property("fpgatype").value<FpgaType>().selectedfpga)
                m_parameters.append("--bit_reverse");
            m_parameters.append("--output");
            QString outputFile;
            outputFile.append(childOfFpga->property("filename").value<FileString>().filestring);

            // TODO:
            // replace childOfFpga->parent()->property...
            m_parameters.append(fpga->getHexFileName());
            m_parameters.append("--intel");
        }
        if (childOfFpga->objectName()=="firmware")
        {
//          qDebug() << "firmware detected";
            firmware *fw = (firmware*)childOfFpga;
            fw->setVerFileName();
            if (!versionFileCreate(fw->getVerFileName(),fw->getVerString()))
            {
                qDebug() << "error by ver file creating";
                return 0;
            }
        }
    }
    return 1;
}

int srec_wrapper::runSrec(const QObjectList & parametersParent)
{

    if (setParametersForSrec(parametersParent))
    {
        m_output.append(m_runcmd);
    }
    else
        return 1;

    QProcess *process = new QProcess(0);
    m_runcmd.append(m_srecExe);

    for (const auto & p : m_parameters)
    {
        m_runcmd.append(" ");
        m_runcmd.append(p);
    }

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
