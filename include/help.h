#ifndef HELP_H
#define HELP_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class helpDialog;
}

class helpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit helpDialog(QWidget *parent = nullptr);
    ~helpDialog();

private:
    Ui::helpDialog *ui;

    QStringList outputText;
};

#endif // HELP_H

