#include "aboutdialog.h"
#include "ui_aboutdialog.h"

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);

    ui->label->setText(QApplication::translate("aboutDialog", "Package Generator v1.00.05 Beta", Q_NULLPTR));
}

aboutDialog::~aboutDialog()
{
    delete ui;
}
