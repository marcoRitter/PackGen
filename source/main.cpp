#include "mainwindow.h"
#include <QApplication>
#include "customtype.h"

int main(int argc, char *argv[])
{
    CustomType::setup();

    QApplication a(argc, argv);
    a.setApplicationName("PackGen");
    a.setOrganizationName("Bachmann GmbH");
    a.setOrganizationDomain("home.org");
    MainWindow w;
    w.show();

    return a.exec();
}
