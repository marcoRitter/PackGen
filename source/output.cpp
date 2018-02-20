#include "output.h"
#include "ui_output.h"

output::output(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::output)
{
    ui->setupUi(this);
    ui->textEdit->setText("Hello");

}

output::~output()
{
    delete ui;
}
