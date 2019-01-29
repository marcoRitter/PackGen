#include "help.h"
#include "ui_helpdialog.h"

helpDialog::helpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpDialog)
{
    ui->setupUi(this);

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->vSplitter->setCollapsible(0,false);
    ui->vSplitter->setCollapsible(1,false);
    ui->hSplitter->setCollapsible(0,false);
    ui->hSplitter->setCollapsible(1,false);
    pHelpMenu=new QMenu(this);

    Project *p = new Project(this);

    M86_Spartan6 *m86 = new M86_Spartan6(p);
    m86->setType("M86 Package");
    Fpga *f = new Fpga(m86);
    firmware *firm = new firmware(m86);
    firm->setType("Firmware");

    Masterfile *m = new Masterfile(p);
    file *fileFile = new file(m);
    fileFile->setObject_name("File");
    file *fpgaFile = new file(m);
    fpgaFile->setObject_name("FPGA");
    file *firmFile = new file(m);
    firmFile->setObject_name("Firmware");
    file *goldenFile = new file(m);
    goldenFile->setObject_name("Golden File");

    goldenReference *g = new goldenReference(p);
    g->setType("Golden Reference");

    p->setChild(p->rowCount(),m86);
    m86->setChild(m86->rowCount(),f);
    m86->setChild(m86->rowCount(),firm);

    p->setChild(p->rowCount(),m);
    m->setChild(m->rowCount(),fileFile);
    m->setChild(m->rowCount(),fpgaFile);
    m->setChild(m->rowCount(),firmFile);
    m->setChild(m->rowCount(),goldenFile);

    p->setChild(p->rowCount(),g);


    // add node to tree view
    QStandardItem *info = new QStandardItem();
    info->setText("Allgemeines");
    m_model.appendRow(info);
    m_model.appendRow(p);

    ui->treeView->setModel(&m_model);

    ui->treeView->expandAll();


    //propertyEditor->setResizeMode(QtTreePropertyBrowser::ResizeMode(0));
    //propertyEditor->setFactoryForManager(variantManager, variantFactory);
    //ui->scrollArea->setWidget(propertyEditor);
}

helpDialog::~helpDialog()
{
    delete ui;
}

void helpDialog::on_treeView_clicked(const QModelIndex &index)
{
    m_model.set_index(index);

    helpDialog::draw_property_browser();
}
void helpDialog::on_treeView_customContextMenuRequested(const QPoint &pos)
{

    QTreeView *tree = ui->treeView;
    QModelIndex index = tree->indexAt(pos);
    QStandardItem *selected = m_model.itemFromIndex(index);

    if (!selected)
        return;

    Node *n = static_cast<Node *>(selected);
    pHelpMenu->clear();
    n->node_menue(pHelpMenu);
    pHelpMenu->popup(tree->viewport()->mapToGlobal(pos));
}


void helpDialog::draw_property_browser()
{
    /*QStandardItem *selected = m_model.itemFromIndex(m_model.get_index());

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
    }*/
}

void helpDialog::resizeEvent(QResizeEvent * event)
{
    QDialog::resizeEvent(event);

    //QSize treeViewSize;
    //QSize propertyViewSize;
    QSize splitterSize;

    splitterSize.setHeight(QDialog::height()-65);
    splitterSize.setWidth(QDialog::width()-8);
    ui->vSplitter->resize(splitterSize);
    ui->vSplitter->move(4,1);
}

void helpDialog::treeMenu()
{

}
