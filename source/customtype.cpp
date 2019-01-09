#include "customtype.h"

void CustomType::setup()
{
    CustomTypeFile = qRegisterMetaType<FileString>("FileString");
    CustomTypeFile = qRegisterMetaType<HexString>("HexString");
    CustomTypeFile = qRegisterMetaType<DualBoot>("DualBoot");
    CustomTypeFile = qRegisterMetaType<FlashSize>("FlashSize");
    CustomTypeFile = qRegisterMetaType<FpgaType>("FpgaType");
    CustomTypeFile = qRegisterMetaType<VerState>("VerState");
    CustomTypeFile = qRegisterMetaType<FileType>("FileType");
}

