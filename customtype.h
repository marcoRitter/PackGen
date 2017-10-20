#ifndef CUSTOMTYPE_H
#define CUSTOMTYPE_H

#include <QMetaType>
#include <QString>
#include <QStringList>

class FileString
{
public:
    QString filestring;

    FileString() { }
    FileString(const QString &value) : filestring(value) { }
};


class HexString
{
public:
    QString hexstring;

    HexString() { }
    HexString(const QString &value) : hexstring(value) { }
};

class DualBoot
{
public:
    QStringList dualboot = {"enable", "disable"};
    uint dualbootena = 0;
    DualBoot() { }
    DualBoot(const uint &value) : dualbootena(value) {}
};

class FlashSize
{
public:
    QStringList memorysize = {"2 Mbit","4 Mbit","8 Mbit","16 Mbit","32 Mbit","64 Mbit","128 Mbit"};
    uint selectedsize = 2;
    FlashSize() { }
    FlashSize(const uint &value) : selectedsize(value) { }
};

Q_DECLARE_METATYPE(FileString)
Q_DECLARE_METATYPE(HexString)
Q_DECLARE_METATYPE(FlashSize)
Q_DECLARE_METATYPE(DualBoot)



/* hack */
static int CustomTypeFile;

class CustomType
{
public:
    static void setup();
};

#endif // CUSTOMTYPE_H
