#ifndef UI_HELPDIALOG_H
#define UI_HELPDIALOG_H


#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>
#include <qsplitter.h>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_helpDialog
{
public:
    QAction *actionLoad;
    QAction *actionSave;
    QAction *actionQuit;
    QAction *actionNew;
    QAction *actionSave_as;
    QAction *actionProperties;
    QAction *actionHelpDialog;
    QAction *actionGenerateFpga;
    QWidget *centralWidget;
    QSplitter *vSplitter;
    QSplitter *hSplitter;
    QTreeView *treeView;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QTextEdit *outInfo;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QDialog *helpDialog)
    {
        if (helpDialog->objectName().isEmpty())
            helpDialog->setObjectName(QStringLiteral("Help Dialog"));
        helpDialog->resize(500, 500);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(helpDialog->sizePolicy().hasHeightForWidth());
        helpDialog->setSizePolicy(sizePolicy);
        helpDialog->setMinimumSize(QSize(700, 500));
        QFont font;
        font.setPointSize(10);
        helpDialog->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/Images/images/icons8-robot.png"), QSize(), QIcon::Normal, QIcon::Off);
        helpDialog->setWindowIcon(icon);


        vSplitter = new QSplitter(helpDialog);
        vSplitter->setObjectName(QStringLiteral("vSplitter"));
        vSplitter->setGeometry(QRect(10, 10, 257, 384));
        vSplitter->setOrientation(Qt::Vertical);
        hSplitter = new QSplitter(vSplitter);
        hSplitter->setObjectName(QStringLiteral("hSplitter"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(100);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(hSplitter->sizePolicy().hasHeightForWidth());
        hSplitter->setSizePolicy(sizePolicy1);
        hSplitter->setMinimumSize(QSize(100, 0));
        hSplitter->setOrientation(Qt::Horizontal);
        treeView = new QTreeView(vSplitter);
        treeView->setObjectName(QStringLiteral("treeView"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(40);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(treeView->sizePolicy().hasHeightForWidth());
        treeView->setSizePolicy(sizePolicy2);
        treeView->setMinimumSize(QSize(150, 0));
        QFont font1;
        font1.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font1.setPointSize(11);
        treeView->setFont(font1);
        treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        treeView->setAutoExpandDelay(-1);
        treeView->setItemsExpandable(true);
        hSplitter->addWidget(treeView);
        treeView->header()->setVisible(false);
        scrollArea = new QScrollArea(vSplitter);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(100);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy3);
        scrollArea->setMinimumSize(QSize(100, 0));
        QFont font2;
        font2.setPointSize(13);
        scrollArea->setFont(font2);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setEnabled(true);
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 98, 190));
        scrollArea->setWidget(scrollAreaWidgetContents);
        hSplitter->addWidget(treeView);
        hSplitter->addWidget(scrollArea);
        vSplitter->addWidget(hSplitter);
        helpDialog->setLayout(vSplitter->layout());


        retranslateUi(helpDialog);
        //QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(helpDialog);
    } // setupUi

    void retranslateUi(QDialog *helpDialog)
    {
        helpDialog->setWindowTitle(QApplication::translate("helpDialog", "Help Dialog", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class helpDialog: public Ui_helpDialog {};
} // namespace Ui

QT_END_NAMESPACE
#endif // UI_HELPDIALOG_H
