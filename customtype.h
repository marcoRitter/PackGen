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
    inline QString get_offset (const uint& flash_size)
    {
        switch (flash_size)
        {
        case 0: return "0x1000"; break;
        case 1: return "0x2000"; break;
        case 2: return "0x4000"; break;
        case 3: return "0x8000"; break;
        case 4: return "0xC000"; break;
        case 5: return "0xE000"; break;
        case 6: return "0xF000"; break;
        default: return "0x0000"; break;
        }
    }
};

class DualBoot
{
public:
    QStringList dualboot = {"disable", "enable"};
    uint dualbootena ;
    DualBoot() :dualbootena(0){ }
    DualBoot(const uint &value) :dualbootena(value){}
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
