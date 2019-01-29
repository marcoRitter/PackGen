#ifndef HELP_H
#define HELP_H

#include "fpga.h"
#include "firmware.h"
#include "file.h"
#include "goldenReference.h"
#include "model.h"
#include "node.h"
#include "project.h"
#include "M86_Spartan6.h"

#include <QDialog>
#include <QStringList>

#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include "qtpropertymanager.h"
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"
#include "qtbuttonpropertybrowser.h"
#include "qtgroupboxpropertybrowser.h"
#include "variantmanager.h"
#include "variantfactory.h"
#include "filepathmanager.h"
#include "fileeditfactory.h"
#include "fileedit.h"

namespace Ui {
class helpDialog;
}

class helpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit helpDialog(QWidget *parent = nullptr);
    ~helpDialog();

    Model m_model;

    Node *getMCurrent () {return m_currentItem;}
    void node_menue(QMenu *menu);

private slots:
    void on_treeView_clicked(const QModelIndex &index);
    void on_treeView_customContextMenuRequested(const QPoint &pos);
    void resizeEvent(QResizeEvent * event);

    void treeMenu();
    void draw_property_browser();

private:
    Ui::helpDialog *ui;

    QString m_winTitle = "Help Dialog";

    QtVariantPropertyManager *variantManager;
    QtVariantEditorFactory *variantFactory;

    QTextEdit *propertyEditor;

    Node *m_currentItem;
    QMenu *pHelpMenu;

    QStringList outputText;
};

QObject * getObjectWithName (const QObject * pobject,const QString &name);
#endif // HELP_H

