/****************************************************************************
**
** Copyright (C) 2006 Trolltech ASA. All rights reserved.
**
** This file is part of the documentation of Qt. It was originally
** published as part of Qt Quarterly.
**
** This file may be used under the terms of the GNU General Public License
** version 2.0 as published by the Free Software Foundation or under the
** terms of the Qt Commercial License Agreement. The respective license
** texts for these are provided with the open source and commercial
** editions of Qt.
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "fileedit.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QFileDialog>
#include <QFocusEvent>
#include <QDebug>
#include <customtype.h>
#include <mainwindow.h>

FileEdit::FileEdit(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    theLineEdit = new QLineEdit(this);
    theLineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    QToolButton *button = new QToolButton(this);
    button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
    button->setText(QLatin1String("..."));
    layout->addWidget(theLineEdit);
    layout->addWidget(button);
    setFocusProxy(theLineEdit);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_InputMethodEnabled);
    connect(theLineEdit, SIGNAL(textEdited(const QString &)),
                this, SIGNAL(filePathChanged(const QString &)));
    connect(button, SIGNAL(clicked()),
                this, SLOT(buttonClicked()));
    p_parent = parent;
}

void FileEdit::buttonClicked()
{

    QObject * pObject = getObjectWithName((QObject *) p_parent, "MainWindow");
    MainWindow *pMainWindow = (MainWindow*) pObject;

    const QString DEFAULT_M86_DIR ("default_m86_dir");
    const QString DEFAULT_FW_DIR ("default_fw_dir");
    QSettings mySettings;
    QDir CurrentDir;

    QString filePath;
    bool file = true;
    if(pMainWindow->getMCurrent()->node_type() == "goldenReference")
    {
        goldenReference *goldRef = static_cast<goldenReference*>(pMainWindow->getMCurrent());
        if(goldRef->goldenRef_file().filestring.isEmpty())
        {
            file = false;
        }
    }
    if (pMainWindow->getMCurrent()->node_type() != "M86" && pMainWindow->getMCurrent()->node_type() != "Masterfile" && file) {
        filePath = QFileDialog::getOpenFileName(this, tr("Choose a file"), mySettings.value(DEFAULT_M86_DIR).toString());//theLineEdit->text(), theFilter);
        if (!filePath.isNull())
            mySettings.setValue(DEFAULT_M86_DIR,CurrentDir.absoluteFilePath(filePath));
    }
    else {
        filePath = QFileDialog::getExistingDirectory(this, tr("Choosa a folder"), mySettings.value(DEFAULT_FW_DIR).toString());//theLineEdit->text(), 0);
        if (!filePath.isNull())
            mySettings.setValue(DEFAULT_FW_DIR,CurrentDir.absoluteFilePath(filePath));
    }

    if (filePath.isNull())
        return;
    theLineEdit->setText(filePath);
    emit filePathChanged(filePath);
}

void FileEdit::focusInEvent(QFocusEvent *e)
{
    theLineEdit->event(e);
    if (e->reason() == Qt::TabFocusReason || e->reason() == Qt::BacktabFocusReason) {
        theLineEdit->selectAll();
    }
    QWidget::focusInEvent(e);
}

void FileEdit::focusOutEvent(QFocusEvent *e)
{
    theLineEdit->event(e);
    QWidget::focusOutEvent(e);
}

void FileEdit::keyPressEvent(QKeyEvent *e)
{
    theLineEdit->event(e);
}

void FileEdit::keyReleaseEvent(QKeyEvent *e)
{
    theLineEdit->event(e);
}
