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


SOURCES += main.cpp\
        mainwindow.cpp \
    node.cpp \
    project.cpp \
    fpga.cpp \
    model.cpp \
    property_browser/qtbuttonpropertybrowser.cpp \
    property_browser/qteditorfactory.cpp \
    property_browser/qtgroupboxpropertybrowser.cpp \
    property_browser/qtpropertybrowser.cpp \
    property_browser/qtpropertybrowserutils.cpp \
    property_browser/qtpropertymanager.cpp \
    property_browser/qttreepropertybrowser.cpp \
    property_browser/qtvariantproperty.cpp \
    M86_Spartan6.cpp \
    JADE_Package.cpp \
    JADE_Moduledescription.cpp \
    JADE_History.cpp \
    Masterfile.cpp \
    customtype.cpp \
    property_browser/extension/fileedit.cpp \
    property_browser/extension/fileeditfactory.cpp \
    property_browser/extension/filepathmanager.cpp \
    property_browser/extension/variantfactory.cpp \
    property_browser/extension/variantmanager.cpp \
    property_browser/extension/decoratedstring.cpp

HEADERS  += mainwindow.h \
    node.h \
    project.h \
    fpga.h \
    model.h \
    property_browser/qtbuttonpropertybrowser.h \
    property_browser/qteditorfactory.h \
    property_browser/qtgroupboxpropertybrowser.h \
    property_browser/qtpropertybrowser.h \
    property_browser/qtpropertybrowserutils_p.h \
    property_browser/qtpropertymanager.h \
    property_browser/qttreepropertybrowser.h \
    property_browser/qtvariantproperty.h \
    M86_Spartan6.h \
    JADE_Package.h \
    JADE_Moduledescription.h \
    JADE_History.h \
    Masterfile.h \
    customtype.h \
    property_browser/extension/fileedit.h \
    property_browser/extension/fileeditfactory.h \
    property_browser/extension/filepathmanager.h \
    property_browser/extension/variantfactory.h \
    property_browser/extension/variantmanager.h \
    property_browser/extension/decoratedstring.h

FORMS    += mainwindow.ui
