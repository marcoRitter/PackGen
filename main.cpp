#include "mainwindow.h"
#include <QApplication>
#include "customtype.h"

int main(int argc, char *argv[])
{
    CustomType::setup();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
