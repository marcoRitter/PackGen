#ifndef SREC_WRAPPER_H
#define SREC_WRAPPER_H
#include <QString>
#include <QMap>

class srec_wrapper
{
public:
    srec_wrapper();
    ~srec_wrapper();

    void setSrecExe (QString filename);
    QString getSrecExe () {return m_srecExe;}

    int runSrec (const QStringList & parameters);

    QString getOutput () {return m_output;}
    QString getRuncmd () {return m_runcmd;}
    QString getOutputFileName (const QString& inFile);

private:
    QString m_srecExe;
    QString m_output;
    QString m_runcmd;

    QMap<QString, QString> m_inputKeys;

};

#endif // SREC_WRAPPER_H
