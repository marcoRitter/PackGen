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

    int runSrec (const QMap<QString, QString>& srecParams, QString *output);

private:
    QString m_srecExe;
    QString m_keys;
    QString m_output;

    QMap<QString, QString> m_inputKeys;

};

#endif // SREC_WRAPPER_H
