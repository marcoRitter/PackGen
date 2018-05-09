#include "mainwindow.h"
#include <QApplication>
#include "customtype.h"

int main(int argc, char *argv[])
{
    CustomType::setup();

    QApplication a(argc, argv);
    a.setApplicationName("PackGen v0.41");
    MainWindow w;
    w.show();

    return a.exec();
}
