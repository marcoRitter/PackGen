/********************************************************************************
** Form generated from reading UI file 'output.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OUTPUT_H
#define UI_OUTPUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_output
{
public:
    QDialogButtonBox *buttonBox;
    QTextEdit *textEdit;

    void setupUi(QDialog *output)
    {
        if (output->objectName().isEmpty())
            output->setObjectName(QStringLiteral("output"));
        output->resize(400, 279);
        buttonBox = new QDialogButtonBox(output);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        textEdit = new QTextEdit(output);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(10, 10, 381, 221));

        retranslateUi(output);
        QObject::connect(buttonBox, SIGNAL(accepted()), output, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), output, SLOT(reject()));

        QMetaObject::connectSlotsByName(output);
    } // setupUi

    void retranslateUi(QDialog *output)
    {
        output->setWindowTitle(QApplication::translate("output", "Dialog", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class output: public Ui_output {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTPUT_H
