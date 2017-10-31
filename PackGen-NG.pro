#-------------------------------------------------
#
# Project created by QtCreator 2017-03-15T14:02:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PackGen-NG
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG+= static
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include\
    property_browser/include\
    property_browser/extension/include\

SOURCES += source/main.cpp\
        source/mainwindow.cpp \
    source/node.cpp \
    source/project.cpp \
    source/fpga.cpp \
    source/model.cpp \
    property_browser/source/qtbuttonpropertybrowser.cpp \
    property_browser/source/qteditorfactory.cpp \
    property_browser/source/qtgroupboxpropertybrowser.cpp \
    property_browser/source/qtpropertybrowser.cpp \
    property_browser/source/qtpropertybrowserutils.cpp \
    property_browser/source/qtpropertymanager.cpp \
    property_browser/source/qttreepropertybrowser.cpp \
    property_browser/source/qtvariantproperty.cpp \
    source/M86_Spartan6.cpp \
    source/JADE_Package.cpp \
    source/JADE_Moduledescription.cpp \
    source/JADE_History.cpp \
    source/Masterfile.cpp \
    source/customtype.cpp \
    property_browser/extension/source/fileedit.cpp \
    property_browser/extension/source/fileeditfactory.cpp \
    property_browser/extension/source/filepathmanager.cpp \
    property_browser/extension/source/variantfactory.cpp \
    property_browser/extension/source/variantmanager.cpp \
    property_browser/extension/source/decoratedstring.cpp \
    source/aboutdialog.cpp \
    source/srec_wrapper.cpp

HEADERS  += include/mainwindow.h \
    include/node.h \
    include/project.h \
    include/fpga.h \
    include/model.h \
    property_browser/include/qtbuttonpropertybrowser.h \
    property_browser/include/qteditorfactory.h \
    property_browser/include/qtgroupboxpropertybrowser.h \
    property_browser/include/qtpropertybrowser.h \
    property_browser/include/qtpropertybrowserutils_p.h \
    property_browser/include/qtpropertymanager.h \
    property_browser/include/qttreepropertybrowser.h \
    property_browser/include/qtvariantproperty.h \
    include/M86_Spartan6.h \
    include/JADE_Package.h \
    include/JADE_Moduledescription.h \
    include/JADE_History.h \
    include/Masterfile.h \
    include/customtype.h \
    property_browser/extension/include/fileedit.h \
    property_browser/extension/include/fileeditfactory.h \
    property_browser/extension/include/filepathmanager.h \
    property_browser/extension/include/variantfactory.h \
    property_browser/extension/include/variantmanager.h \
    property_browser/extension/include/decoratedstring.h \
    include/aboutdialog.h \
    include/srec_wrapper.h

FORMS    += resource/mainwindow.ui \
    resource/aboutdialog.ui

RESOURCES += \
    resource/resources.qrc
