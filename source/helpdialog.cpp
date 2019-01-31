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
    info->setEnabled(false);
    m_model.appendRow(info);
    m_model.appendRow(p);

    ui->treeView->setModel(&m_model);

    ui->treeView->expandAll();

    propertyEditor = new QTextEdit(ui->scrollArea);
    //propertyEditor->setHeaderVisible(true);
    propertyEditor->setReadOnly(true);


    QFont font;
    font.setFamily("arial");
    font.setWeight(1);
    font.setPixelSize(13);
    font.setItalic(false);
    propertyEditor->setFont(font);

    QTextCursor cur;
    cur.setPosition(0);
    propertyEditor->setTextCursor(cur);

    readTXTdata("allgemeines");

    ui->scrollArea->setWidget(propertyEditor);
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
/*void helpDialog::on_treeView_customContextMenuRequested(const QPoint &pos)
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
}*/


void helpDialog::draw_property_browser()
{
        propertyEditor->clear();

        QStandardItem *selected = m_model.itemFromIndex(m_model.get_index());
        Node *a = static_cast<Node*>(selected);

        if(!a->isEnabled())
        {
            readTXTdata("allgemeines");
        }
        else if(a->getType().toLower().contains("project"))
        {
            readTXTdata("project");
        }
        else if (a->getType().toLower().contains("m86"))
        {
            readTXTdata("m86");
        }
        else if (a->getType().toLower().contains("fpga") &&a->objectName().toLower() == "fpga")
        {
            readTXTdata("fpga");
        }
        else if (a->getType().toLower().contains("firmware") &&a->objectName().toLower() == "firmware")
        {
            readTXTdata("firmware");
        }
        else if (a->getType().toLower().contains("masterfile"))
        {
            readTXTdata("masterfile");
        }
        else if (a->getType().toLower().contains("fpga") &&a->objectName().toLower() == "file")
        {
            readTXTdata("fpgaFile");
        }
        else if (a->getType().toLower().contains("firmware") &&a->objectName().toLower() == "file")
        {
            readTXTdata("firmwareFile");
        }
        else if (a->getType().toLower().contains("golden") &&a->objectName().toLower() == "file")
        {
            readTXTdata("goldenFile");
        }
        else if (a->getType().toLower().contains("file") &&a->objectName().toLower() == "file")
        {
            readTXTdata("file");
        }
        else if (a->getType().toLower().contains("golden reference"))
        {
            readTXTdata("golden");
        }

        QTextCursor cur;
        cur.setPosition(0);
        propertyEditor->setTextCursor(cur);

    ui->scrollArea->setWidget(propertyEditor);
}

void helpDialog::resizeEvent(QResizeEvent * event)
{
    QDialog::resizeEvent(event);
    QSize splitterSize;

    splitterSize.setHeight(QDialog::height()-65);
    splitterSize.setWidth(QDialog::width()-8);
    ui->vSplitter->resize(splitterSize);
    ui->vSplitter->move(4,1);

}

void helpDialog::treeMenu()
{

}

void helpDialog::readTXTdata(QString txtFile)
{
    QString filename = ":/Helptxt/txt/"+txtFile+".txt";
    QFile file(filename);

    if(file.open(QFile::ReadOnly|QFile::Text))
    {
        while(!file.atEnd())
        {
            QString line = QString::fromLatin1(file.readLine());
            line.remove("\n");
            propertyEditor->append(line);
        }
        file.close();
    }
}
