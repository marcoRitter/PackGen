#include "mainwindow.h"
#include "aboutdialog.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMetaProperty>
#include <QFileDialog>
#include <QRegExp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_currentItem(nullptr)
{

    ui->setupUi(this);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->splitter->setCollapsible(0,false);
    ui->splitter->setCollapsible(1,false);
    pMainMenu=new QMenu(this);
    // create new project node
    Project *p = new Project(this);
    p->setDescription("Main Project");

//  Project *p1 = new Project();
//  p1->setDescription("Second Project");
//  p1->setType("Project2");

    // add node to tree view
    m_model.appendRow(p);
//  m_model.appendRow(p1);

    ui->treeView->setModel(&m_model);

    ui->treeView->expandAll();

//    variantManager = new QtVariantPropertyManager();
//    variantFactory = new QtVariantEditorFactory(this);
    variantManager = new VariantManager();
    variantFactory = new VariantFactory(this);

    connect(variantManager, &QtVariantPropertyManager::valueChanged, this, &MainWindow::handleValueChanged);

    propertyEditor = new QtTreePropertyBrowser(ui->scrollArea);
    propertyEditor->setHeaderVisible(true);

//  propertyEditor->setResizeMode();
//  qDebug () << "resize mode" << propertyEditor->resizeMode();
    propertyEditor->setResizeMode(QtTreePropertyBrowser::ResizeMode(0));
    propertyEditor->setFactoryForManager(variantManager, variantFactory);
    ui->scrollArea->setWidget(propertyEditor);

    QString winTitle;
    winTitle = m_winTitle + " - untitled";
    QMainWindow::setWindowTitle(winTitle);

//  ui->actionGenerateFpga->setDisabled(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    m_model.set_index(index);

    MainWindow::draw_property_browser();
}


void MainWindow::on_actionSave_as_triggered()
{

    qDebug() << "Save as";
    Project_FileName = QFileDialog::getSaveFileName(this, tr("Save File"),".",tr("Pack Gen Files (*.pkg)"));
    if (Project_FileName != "")
    {
        QStandardItem *project = m_model.item(0,0);
        m_model.save(Project_FileName,project);
    }
    QStringRef winTitle = Project_FileName.midRef(Project_FileName.lastIndexOf("/")+1,
                                                  Project_FileName.lastIndexOf(".") - Project_FileName.lastIndexOf("/")-1);
    setWindowTitle(m_winTitle + " - " + winTitle);

}

void MainWindow::on_actionLoad_triggered()
{
//  qDebug() << "Load";
    Project_FileName = QFileDialog::getOpenFileName(this, tr("Open File"),".",tr("Pack Gen Files (*.pkg)"));
//  qDebug() << "file opened";
    if (Project_FileName != "")
    {
        QStandardItem *project;
        m_model.load(Project_FileName, this);
        ui->treeView->expandAll();
    }
    propertyEditor->clear();
    QStringRef winTitle = Project_FileName.midRef(Project_FileName.lastIndexOf("/")+1,
                                                  Project_FileName.lastIndexOf(".") - Project_FileName.lastIndexOf("/")-1);
    setWindowTitle(m_winTitle + " - " + winTitle);
}

void MainWindow::on_actionNew_triggered()
{
    auto *old = m_model.takeItem(0,0);
    delete old;
    m_model.clear();
    Project *p = new Project(this);
    p->setDescription("Main Project");
    m_model.appendRow(p);
    propertyEditor->clear();
    Project_FileName = "";
    setWindowTitle(m_winTitle + " - untitled*");
}

void MainWindow::on_actionSave_triggered()
{
    if (Project_FileName == "")
        Project_FileName = QFileDialog::getSaveFileName(this, tr("Save File"),".",tr("Pack Gen Files (*.pkg)"));
    if (Project_FileName != "")
    {
        QStandardItem *project = m_model.item(0,0);
        m_model.save(Project_FileName,project);
    }
    QStringRef winTitle = Project_FileName.midRef(Project_FileName.lastIndexOf("/")+1,
                                                  Project_FileName.lastIndexOf(".") - Project_FileName.lastIndexOf("/")-1);
    setWindowTitle(m_winTitle + " - " + winTitle);
}

// Menue on right click
void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{
//  qWarning() << "on_treeView_customContextMenuRequested";

    QTreeView *tree = ui->treeView;
    QModelIndex index = tree->indexAt(pos);
    QStandardItem *selected = m_model.itemFromIndex(index);

    if (!selected)
        return;

    Node *n = (Node *)selected;
//  QMenu *menu=new QMenu(this);
//  qDebug() << "new Menu created";
//  qDebug() << this->children();
    pMainMenu->clear();
    n->node_menue(pMainMenu);
    pMainMenu->popup(tree->viewport()->mapToGlobal(pos));
}

void MainWindow::treeMenu()
{
    qDebug()<<"treeMenu";
}

void MainWindow::handleValueChanged(QtProperty *property, const QVariant &val)
{
//  qDebug()<<"valueChanged" << property << val;
    if (m_currentItem)
    {
        QVariant v =  m_currentItem->property(property->propertyName().toStdString().c_str());
        if (v.type() == QVariant::UserType)
        {
            if (strcmp(v.typeName(),"FileString") == 0)
            {
                FileString s;
                s.filestring = val.toString();
                QVariant a;
                a.setValue<FileString>(s);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(), a);
            }
            if (strcmp(v.typeName(),"HexString") == 0)
            {
                HexString s;
                s.hexstring = val.toString();
                QVariant a;
                a.setValue<HexString>(s);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(), a);
            }
            if (strcmp(v.typeName(),"FlashSize") == 0)
            {
                FlashSize f;
                f.selectedsize = val.toInt();
                QVariant a;
                a.setValue<FlashSize>(f);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(), a);

            }
            if (strcmp(v.typeName(),"DualBoot") == 0)
            {
                QVariant a;
                DualBoot dualboot;
                dualboot.dualbootena = val.toInt();
                a.setValue<DualBoot>(dualboot);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(),a);
            }
            if (strcmp(v.typeName(),"FpgaType") == 0)
            {
                QVariant a;
                FpgaType fpgatype;
                fpgatype.selectedfpga = val.toInt();
                a.setValue<FpgaType>(fpgatype);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(),a);
            }
            if (strcmp(v.typeName(),"VerState") == 0)
            {
                QVariant a;
                VerState verstate;
                verstate.selectedVersion = val.toInt();
                a.setValue<VerState>(verstate);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(),a);
            }

        }
        else
//          if (v.type() == QVariant::Bool)
//              m_currentItem->setProperty(property->propertyName().toStdString().c_str(), val.toBool());
//          else
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(), val);
    }

    if (m_currentItem->get_need_redraw() == 1)
            MainWindow::draw_property_browser();
}

void MainWindow::draw_property_browser()
{
    QStandardItem *selected = m_model.itemFromIndex(m_model.get_index());

    QtVariantProperty *property;
    QtBrowserItem *item;

    Node *n = (Node *)selected;
    m_currentItem = n;


    const QMetaObject *meta = n->metaObject();
    int cnt = meta->propertyCount();
//  qDebug() << "number of properties : " << cnt;
//  qDebug() << "current item " << m_currentItem->node_type();
/*
    if (m_currentItem->node_type() == "M86_Spartan6")
        ui->actionGenerateFpga->setEnabled(true);
    else
        ui->actionGenerateFpga->setDisabled(true);
*/

    propertyEditor->clear();

    // Property Editor aufbauen
    for ( int i = 1; i < cnt; i++ ) {
        QMetaProperty prop = meta->property(i);
//      qDebug() << "property " << i << " = " << prop.name() << "; value = " << prop.read(n);

        if ( prop.isWritable() ) {

        // only sample. We could separate Set and Enum...
           QVariant v = prop.read(n);
           switch ( v.type() ) {
           case QVariant::String :
                 property = variantManager->addProperty(QVariant::String, prop.name());
                 property->setValue(v.toString());
                 property->setToolTip(setTipForProperty(prop));
                 property->setAttribute("regExp", setRegExpForProperty(prop));
                 break ;
           case QVariant::UInt :
               property = variantManager->addProperty(QVariant::Int, prop.name());
               property->setAttribute(QLatin1String("minimum"), 0);
               property->setValue(v.toInt());
                 break ;
           case QVariant::Int :
               property = variantManager->addProperty(QVariant::Int, prop.name());
               property->setAttribute(QLatin1String("minimum"), 0);
               property->setValue(v.toInt());
                 break ;

           case QVariant::Bool :
               property = variantManager->addProperty(QVariant::Bool, prop.name());
               property->setValue(v.toBool());
                 break ;

           case QVariant::UserType :
               if (strcmp(v.typeName(),"FileString") == 0)
               {
                   property = variantManager->addProperty(VariantManager::filePathTypeId(), prop.name());
                   property->setValue(v);
                }
               if (strcmp(v.typeName(),"HexString") == 0)
               {
                   property = variantManager->addProperty(QVariant::String, prop.name());
                   property->setValue(v.value<HexString>().hexstring);
                   property->setAttribute("regExp", QRegExp("0x[0-9A-Fa-f]{1,8}"));
                   property->setToolTip("Enter Address as 0x1324");
                }
               if (strcmp(v.typeName(),"FlashSize") == 0)
               {
                   property = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), prop.name());
                   property->setAttribute("enumNames",v.value<FlashSize>().memorysize);
                   property->setValue(v.value<FlashSize>().selectedsize);
                }
               if (strcmp(v.typeName(),"DualBoot") == 0)
               {
                   property = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), prop.name());
                   property->setAttribute("enumNames",v.value<DualBoot>().dualboot);
                   property->setValue(v.value<DualBoot>().dualbootena);
                }
               if (strcmp(v.typeName(),"FpgaType") == 0)
               {
                   property = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), prop.name());
                   property->setAttribute("enumNames",v.value<FpgaType>().fpgatype);
                   property->setValue(v.value<FpgaType>().selectedfpga);
                }
               if (strcmp(v.typeName(),"VerState") == 0)
               {
                   property = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), prop.name());
                   property->setAttribute("enumNames",v.value<VerState>().verstate);
                   property->setValue(v.value<VerState>().selectedVersion);
                }
                break;
           default :
               break;
           }
//         item = propertyEditor->addProperty(property);
           propertyEditor->addProperty(property);
//         propertyEditor->setExpanded(item, false);
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    QMainWindow::resizeEvent(event);

    QSize treeViewSize;
    QSize propertyViewSize;
    QSize splitterSize;

    splitterSize.setHeight(QMainWindow::height()-55);
    splitterSize.setWidth(QMainWindow::width()-8);
    ui->splitter->resize(splitterSize);
    ui->splitter->move(4,1);
}

// this function emits event by changing parameters in property editor
void MainWindow::changeProperty (const QString & name, const QVariant a)
{
    QList<QtProperty*> props = propertyEditor->properties();
    int i = 0;
    for(auto const& pr : props)
    {
        if (pr->propertyName()== name)
            variantManager->setValue(props[i],a);
        i++;
    }
}

void MainWindow::on_actionProperties_triggered()
{

    aboutDialog propWin;
    propWin.exec();
    qDebug() << "properties started";

}


void MainWindow::on_actionGenerateFpga_triggered()
{
    MainWindow::generateFpga();
}


QObject * getObjectWithName (const QObject * pobject,const QString &name)
{
    QObject * pointer =(QObject *) pobject;
    if (pointer->objectName() == name)
        return pointer;
    else
    {
        if (pointer == 0)
            return 0;
        else
        {
            pointer = pointer->parent();
            return getObjectWithName(pointer, name);
        }

    }
    return pointer;
}

QString MainWindow::setTipForProperty(const QMetaProperty & prop)
{
    QString toolTip = "defalut";
    if (strcmp(prop.name(), "description") == 0)
        toolTip = "description";
    if (strcmp(prop.name(), "pkgName") == 0)
        toolTip = "Output filename";
    if (strcmp(prop.name(), "ver_major") == 0)
        toolTip = "Major version of project (0x..)";
    if (strcmp(prop.name(), "ver_minor") == 0)
        toolTip = "Minor version of project(0x..)";
    if (strcmp(prop.name(), "ver_subminor") == 0)
        toolTip = "Minor subversion of project(0x..)";
    if (strcmp(prop.name(), "typecode") == 0)
        toolTip = "Variant (0x..)";
    if (strcmp(prop.name(), "typecode") == 0)
        toolTip = "Variant (0x..)";
    return toolTip;
}

QRegExp MainWindow::setRegExpForProperty(const QMetaProperty &prop)
{
    QRegExp regexp;
    regexp.setPattern(".*");
    if (strcmp(prop.name(), "ver_major") == 0 ||
        strcmp(prop.name(), "ver_minor") == 0 ||
        strcmp(prop.name(), "ver_subminor") == 0 ||
        strcmp(prop.name(), "revision") == 0 ||
        strcmp(prop.name(), "testversion") == 0 ||
        strcmp(prop.name(), "variant") == 0 ||
        strcmp(prop.name(), "typecode") == 0)
            regexp.setPattern("0x[0-9A-Fa-f]{1,2}");
    if (strcmp(prop.name(), "designnumber") == 0)
        regexp.setPattern("0x[0-9A-Fa-f]{1,4}");
    if (strcmp(prop.name(), "start_addr") == 0)
        regexp.setPattern("0x[0-9A-Fa-f]{1,8}");
    return regexp;
}
