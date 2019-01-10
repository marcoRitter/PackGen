#include "model.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>

Model::Model(QObject *parent) :
    QStandardItemModel(parent)
{

}

void saveTreeNode(QStandardItem * pItem, QJsonObject *jsonObj)
{
    if (!pItem)
        return;

    Node *n = static_cast<Node *>(pItem);
//    QJsonObject jsonObj;

//  qDebug() << "Nodename : " << n->description();
//   const QMetaObject *meta = n->metaObject() ;
//  int cnt = meta->propertyCount();
//  qDebug() << "number of properties : " << cnt;
//    for ( int i = 0; i < cnt; i++ )
//    {
//        QMetaProperty prop = meta->property(i);
//        QVariant v = meta->property(i).read(x);
//        qDebug() << "  Prop : " << "property " << prop.name() << v.value<QString>();
//        jsonObj->insert(prop.name(),v.value<QString>());
//    }
    n->writeJson(jsonObj);
    // Now save children
    QJsonArray jsonArray;
    int nKids = pItem->rowCount();

    for ( int nKid = 0; nKid < nKids; ++nKid )
    {
        QJsonObject new_jsonObj;    // new json object for next children
        saveTreeNode(pItem->child(nKid),&new_jsonObj);
        jsonArray.insert(0,new_jsonObj);
    }
    jsonObj->insert("childes",jsonArray);
}

bool Model::save(const QString &filename, QStandardItem *n)
{
    QFile jsonFile(filename);


    jsonFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QJsonObject jsonObj;

    saveTreeNode(n,&jsonObj);
    QJsonDocument jsonDoc(jsonObj);

    jsonFile.write(jsonDoc.toJson());
    jsonFile.close();

    return true;
}

void Model::delete_node(Node *n)
{
   QStandardItem *n1 = static_cast<QStandardItem*>(n);
   Node *p = static_cast<Node*>(n1->parent());
   p->removeRow(n->row());
}

void Model::set_index(const QModelIndex &index)
{
    last_sel_index = index;
}

QModelIndex Model::get_index()
{
    return (last_sel_index);
}


bool Model::load(const QString &filename, QObject *parent)
{
    QString projectConfig;
    QFile projectFile(filename);
    projectFile.open(QIODevice::ReadOnly | QIODevice::Text);
    // read file
    projectConfig = projectFile.readAll();
    // close file
    projectFile.close();
    // convert input to json document
    QJsonDocument config = QJsonDocument::fromJson(projectConfig.toUtf8());
    // Get json object from json document
    QJsonObject jsonObj;
    jsonObj = config.object();

    auto *old = takeItem(0, 0);
    delete old;

    clear();
    Node *root = loadTreeNode(&jsonObj, static_cast<Node *>(parent));

    root->dumpObjectTree();

    appendRow(root);

    return (true);
}

 Node *Model::loadTreeNode(QJsonObject *jsonObj, Node *parent)
 {
     Node *n = nullptr;

     QJsonValue jsonVal = jsonObj->value("node_type");
     QString node_type = jsonVal.toString();
//   qDebug() << "  node_type : " << node_type;
     if (node_type == "Project")
     {
         n = new Project(parent);
         Project *x = static_cast<Project *>(n);
         x->readJson(jsonObj);
     }
     if (node_type.toLower() == "m86")
     {
         n = new M86_Spartan6(parent);
         M86_Spartan6 *x = static_cast<M86_Spartan6 *>(n);
         x->readJson(jsonObj);
         x->setType("M86 " + x->description());
     }
     if (node_type.toLower() == "fpga")
     {
         n = new Fpga(parent);
         Fpga *x = static_cast<Fpga *>(n);
         x->readJson(jsonObj);
     }
     if (node_type.toLower() == "firmware")
     {
         n = new firmware(parent);
         firmware *x = static_cast<firmware *>(n);
         x->readJson(jsonObj);
     }
     if (node_type.toLower() == "file")
     {
         n = new file(parent);
         file *x = static_cast<file *>(n);
         x->readJson(jsonObj);
     }
     if (node_type.toLower() == "jade_package")
     {
         n = new JADE_Package(parent);
         JADE_Package *x = static_cast<JADE_Package *>(n);
         x->readJson(jsonObj);
     }
     if (node_type.toLower() == "jade_moduledescription")
     {
         n = new JADE_Moduledescription(parent);
         JADE_Moduledescription *x = static_cast<JADE_Moduledescription *>(n);
         x->readJson(jsonObj);
     }
     if (node_type.toLower() == "jade_history")
     {
         n = new JADE_History(parent);
         JADE_History *x = static_cast<JADE_History *>(n);
         x->readJson(jsonObj);
     }

     if (node_type.toLower() == "masterfile")
     {
         n = new Masterfile(parent);
         Masterfile *x = static_cast<Masterfile *>(n);
         x->readJson(jsonObj);
     }

     if (node_type.toLower() == "goldenreference")
     {
         n = new goldenReference(parent);
         goldenReference *x = static_cast<goldenReference *>(n);
         x->readJson(jsonObj);
     }

     // parse child objects (has anumber as name)
     QJsonValue childes = jsonObj->value("childes");
     QJsonArray jsonArray = childes.toArray();
     int i = 0;
     foreach (const QJsonValue & v, jsonArray)
     {
         QJsonObject next_jsonObj = v.toObject();
         Node *c = loadTreeNode(&next_jsonObj, n);
         n->setChild(i, c);
         i++;
     }
     return n;
 }
