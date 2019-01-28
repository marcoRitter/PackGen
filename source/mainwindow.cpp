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

//  ui->outScrollArea->setWidget(outInfo);

    ui->setupUi(this);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->vSplitter->setCollapsible(0,false);
    ui->vSplitter->setCollapsible(1,false);
    ui->hSplitter->setCollapsible(0,false);
    ui->hSplitter->setCollapsible(1,false);
    pMainMenu=new QMenu(this);
    // create new project node

    Project *p = new Project(this);
    p->setDescription("Main Project");


    // add node to tree view
    m_model.appendRow(p);

    ui->treeView->setModel(&m_model);

    ui->treeView->expandAll();

    variantManager = new VariantManager();
    variantFactory = new VariantFactory(this);

    connect(variantManager, &QtVariantPropertyManager::valueChanged, this, &MainWindow::handleValueChanged);

    propertyEditor = new QtTreePropertyBrowser(ui->scrollArea);
    propertyEditor->setHeaderVisible(true);

    propertyEditor->setResizeMode(QtTreePropertyBrowser::ResizeMode(0));
    propertyEditor->setFactoryForManager(variantManager, variantFactory);
    ui->scrollArea->setWidget(propertyEditor);
    ui->outInfo->setReadOnly(true);
    pClearAction = new QAction("Clear",ui->outInfo);

    ui->outInfo->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->outInfo, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(outInfoMenu(QPoint)));


    QString winTitle;
    winTitle = m_winTitle + " - untitled";
    QMainWindow::setWindowTitle(winTitle);


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
    const QString DEFAULT_PKG_DIR ("default_pkg_dir");
    QSettings mySettings;
    Project_FileName = QFileDialog::getSaveFileName(this, tr("Save File"),mySettings.value(DEFAULT_PKG_DIR).toString(),tr("Pack Gen Files (*.pkg)"));
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
    const QString DEFAULT_PKG_DIR ("default_pkg_dir");
    QSettings mySettings;
    QDir CurrentDir;
    Project_FileName = QFileDialog::getOpenFileName(this, tr("Open File"),mySettings.value(DEFAULT_PKG_DIR).toString(),tr("Pack Gen Files (*.pkg)"));
    if (Project_FileName != "")
    {
        //QStandardItem *project;
        m_model.load(Project_FileName, this);
        ui->treeView->expandAll();
        mySettings.setValue(DEFAULT_PKG_DIR,CurrentDir.absoluteFilePath(Project_FileName));
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

// Menu on right click
void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{

    QTreeView *tree = ui->treeView;
    QModelIndex index = tree->indexAt(pos);
    QStandardItem *selected = m_model.itemFromIndex(index);

    if (!selected)
        return;

    Node *n = static_cast<Node *>(selected);
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
                f.selectedsize = val.toUInt();
                QVariant a;
                a.setValue<FlashSize>(f);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(), a);

            }
            if (strcmp(v.typeName(),"DualBoot") == 0)
            {
                QVariant a;
                DualBoot dualboot;
                dualboot.dualbootena = val.toUInt();
                a.setValue<DualBoot>(dualboot);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(),a);
            }
            if (strcmp(v.typeName(),"FpgaType") == 0)
            {
                QVariant a;
                FpgaType fpgatype;
                fpgatype.selectedfpga = val.toUInt();
                a.setValue<FpgaType>(fpgatype);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(),a);
            }
            if (strcmp(v.typeName(),"VerState") == 0)
            {
                QVariant a;
                VerState verstate;
                verstate.selectedVersion = val.toUInt();
                a.setValue<VerState>(verstate);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(),a);
            }
            if(strcmp(v.typeName(),"FileType") == 0)
            {
                QVariant a;
                FileType filetype;
                filetype.selectedType = val.toUInt();
                a.setValue<FileType>(filetype);
                m_currentItem->setProperty(property->propertyName().toStdString().c_str(),a);
            }

        }
        else
              m_currentItem->setProperty(property->propertyName().toStdString().c_str(), val);
              /*if ((m_currentItem->getType().contains("M86")) && (property->propertyName().contains("description"))) {
                  m_currentItem->setType("M86 " + val.toString());
              }*/
    }

    if (m_currentItem->get_need_redraw() == 1)
            MainWindow::draw_property_browser();
}

void MainWindow::draw_property_browser()
{
    QStandardItem *selected = m_model.itemFromIndex(m_model.get_index());

    QtVariantProperty *property;
    //QtBrowserItem *item;

    Node *n = static_cast<Node *>(selected);
    m_currentItem = n;


    const QMetaObject *meta = n->metaObject();
    int cnt = meta->propertyCount();

    propertyEditor->clear();

    // Property Editor aufbauen
    for ( int i = 1; i < cnt; i++ ) {
        QMetaProperty prop = meta->property(i);

        if ( prop.isWritable() ) {

        // only sample. We could separate Set and Enum...
           QVariant v = prop.read(n);
           switch ( v.type() ) {
           case QVariant::String :
               if(static_cast<QString>(prop.name()) != "object_name")
               {
                 property = variantManager->addProperty(QVariant::String, prop.name());
                 property->setValue(v.toString());
                 property->setToolTip(setTipForProperty(prop));
               }
              if (!((m_currentItem->getType().contains("M86")) && (property->propertyName().contains("description"))))
                 property->setAttribute("regExp", setRegExpForProperty(prop));
                 break ;
           case QVariant::UInt :
               property = variantManager->addProperty(QVariant::Int, prop.name());
               property->setAttribute(QLatin1String("minimum"), 0);
               property->setValue(v.toInt());
               property->setToolTip(setTipForProperty(prop));
                 break ;
           case QVariant::Int :
               property = variantManager->addProperty(QVariant::Int, prop.name());
               property->setAttribute(QLatin1String("minimum"), 0);
               property->setValue(v.toInt());
               property->setToolTip(setTipForProperty(prop));
                 break ;

           case QVariant::Bool :
               property = variantManager->addProperty(QVariant::Bool, prop.name());
               property->setValue(v.toBool());
               property->setToolTip(setTipForProperty(prop));
                 break ;

           case QVariant::UserType :
               if (strcmp(v.typeName(),"FileString") == 0)
               {
                   property = variantManager->addProperty(VariantManager::filePathTypeId(), prop.name());
                   property->setValue(v);
                   //property->setToolTip(setTipForProperty(prop));
               }
               if (strcmp(v.typeName(),"HexString") == 0)
               {
                   property = variantManager->addProperty(QVariant::String, prop.name());
                   property->setValue(v.value<HexString>().hexstring);
                   property->setAttribute("regExp", QRegExp("0x[0-9A-Fa-f]{1,8}"));
                  // property->setToolTip("Enter Address as 0x1324");
                }
               if (strcmp(v.typeName(),"FlashSize") == 0)
               {
                   property = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), prop.name());
                   property->setAttribute("enumNames",v.value<FlashSize>().memorysize);
                   property->setValue(v.value<FlashSize>().selectedsize);
                  // property->setToolTip(setTipForProperty(prop));
                }
               if (strcmp(v.typeName(),"DualBoot") == 0)
               {
                   property = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), prop.name());
                   property->setAttribute("enumNames",v.value<DualBoot>().dualboot);
                   property->setValue(v.value<DualBoot>().dualbootena);
                  // property->setToolTip(setTipForProperty(prop));
                }
               if (strcmp(v.typeName(),"FpgaType") == 0 && !m_currentItem->getType().contains("File") && !m_currentItem->getType().contains("Firmware") && !m_currentItem->getType().contains("Golden File"))
               {
                   property = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), prop.name());
                   property->setAttribute("enumNames",v.value<FpgaType>().fpgatype);
                   property->setValue(v.value<FpgaType>().selectedfpga);
                 //  property->setToolTip(setTipForProperty(prop));
                }
               if (strcmp(v.typeName(),"VerState") == 0)
               {
                   property = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), prop.name());
                   property->setAttribute("enumNames",v.value<VerState>().verstate);
                   property->setValue(v.value<VerState>().selectedVersion);
                  // property->setToolTip(setTipForProperty(prop));
                }
               if(strcmp(v.typeName(),"FileType") == 0)
               {
                   property = variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), prop.name());
                   property->setAttribute("enumNames",v.value<FileType>().filetype);
                   property->setValue(v.value<FileType>().selectedType);
                  // property->setToolTip(setTipForProperty(prop));
               }
                break;
           default :
               break;
           }
           propertyEditor->addProperty(property);
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    QMainWindow::resizeEvent(event);

    //QSize treeViewSize;
    //QSize propertyViewSize;
    QSize splitterSize;

    splitterSize.setHeight(QMainWindow::height()-65);
    splitterSize.setWidth(QMainWindow::width()-8);
    ui->vSplitter->resize(splitterSize);
    ui->vSplitter->move(4,1);
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
    QObject * pointer = const_cast<QObject *>(pobject);
    if (pointer->objectName() == name)
        return pointer;
    else
    {
        if (!pointer)
            return nullptr;
        else
        {
            pointer = pointer->parent();
            return getObjectWithName(pointer, name);
        }

    }
    return pointer;
}

void MainWindow::printOutInfo(const QString &textToOut, const QColor &color)
{
    ui->outInfo->setTextColor(color);
    ui->outInfo->append(textToOut);
}

void MainWindow::outInfoMenu(const QPoint &pt)
{
    QMenu *menu = ui->outInfo->createStandardContextMenu();
    menu->addAction(pClearAction);
    connect(pClearAction,SIGNAL(triggered(bool)),this,SLOT(clearOut()));
    menu->exec(ui->outInfo->mapToGlobal(pt));
    delete menu;
}

void MainWindow::clearOut()
{
    ui->outInfo->clear();
}
QString MainWindow::setTipForProperty(const QMetaProperty & prop)
{
    QString toolTip = "default";
    if (strcmp(prop.name(), "description") == 0)
        toolTip = "File description (not used)";
    if (strcmp(prop.name(), "pkg_name") == 0)
        toolTip = "Output filename (without type ending)";
    if (strcmp(prop.name(), "masterfile_name") == 0)
        toolTip = "Output filename (without type ending)";
    if (strcmp(prop.name(), "goldenRef_name") == 0)
        toolTip = "Output filename (without type ending)";
    if (strcmp(prop.name(), "ver_major") == 0)
        toolTip = "Major version of project (%d%d, 0-9)";
    if (strcmp(prop.name(), "ver_minor") == 0)
        toolTip = "Minor version of project (%d%d, 0-9)";
    if (strcmp(prop.name(), "ver_subminor") == 0)
        toolTip = "Minor subversion of project (%d%d, 0-9)";
    if (strcmp(prop.name(), "typecode") == 0)
        toolTip = "Module type for final package (0x..)";
    if (strcmp(prop.name(), "variant") == 0)
        toolTip = "Module variant for final package (0xff for multiple)";
    if(strcmp(prop.name(), "location") == 0)
        toolTip = "final folder for generated files";
    if(strcmp(prop.name(), "verstate") == 0)
        toolTip = "development status";
    if(strcmp(prop.name(), "filename") == 0)
        toolTip = "final filename";
    if(strcmp(prop.name(), "designnumber") == 0)
        toolTip = "Designnumber according to logic \n reference list (0x..)";
    if(strcmp(prop.name(), "revision") == 0)
        toolTip = "Main revision of logic (0x..)";
    if(strcmp(prop.name(), "testversion") == 0)
        toolTip = "Testversion of logic (0x..)";
    if(strcmp(prop.name(), "start_addr") == 0)
        toolTip = "Code jump in address (0x..)";
    if(strcmp(prop.name(), "name") == 0)
        toolTip = "Project name";
    if(strcmp(prop.name(), "version") == 0)
        toolTip = "File version (not used; %d%d, 0-9)";
    if(strcmp(prop.name(), "JadeProject") == 0)
        toolTip = "true = PackGen for Jade \n flase = PackGen for M200";

    return toolTip;
}

QRegExp MainWindow::setRegExpForProperty(const QMetaProperty &prop)
{
    QRegExp regexp;
    regexp.setPattern(".*");
    if (strcmp(prop.name(), "testversion") == 0 ||
        strcmp(prop.name(), "variant") == 0 ||
        strcmp(prop.name(), "typecode") == 0)
            regexp.setPattern("0x[0-9A-Fa-f]{1,2}");
    if(strcmp(prop.name(), "revision") == 0 ||
       strcmp(prop.name(), "ver_major") == 0 ||
       strcmp(prop.name(), "ver_minor") == 0 ||
       strcmp(prop.name(), "version") == 0 ||
       strcmp(prop.name(), "ver_subminor") == 0)
            regexp.setPattern("[0-9]{1,2}");
    if (strcmp(prop.name(), "designnumber") == 0)
        regexp.setPattern("0x[0-9A-Fa-f]{1,4}");
    if (strcmp(prop.name(), "start_addr") == 0)
        regexp.setPattern("0x[0-9A-Fa-f]{1,8}");
    if(strcmp(prop.name(), "masterfile_name") == 0 ||
        strcmp(prop.name(), "goldenRef_name") == 0 ||
        strcmp(prop.name(), "pkg_name") == 0)
        regexp.setPattern("[0-9A-Za-z]{1,100}");

    /*if (strcmp(prop.name(),"description") == 0)
        regexp.setPattern("[0-9A-Za-z_-]{1,8}");*/
    return regexp;
}
