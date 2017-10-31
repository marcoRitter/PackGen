#ifndef MODEL_H
#define MODEL_H

#include <QStandardItemModel>
#include <QtWidgets>
#include "node.h"
#include "fpga.h"
#include "project.h"
#include "M86_Spartan6.h"
#include "JADE_Moduledescription.h"
#include "JADE_History.h"


class Model : public QStandardItemModel
{
public:
    Model(QObject *parent = nullptr);

    bool load(const QString &filename, QObject *parent);
    bool save(const QString &filename, QStandardItem *n);
    bool delete_item (int row,int col);

    void delete_node(Node *n);
    void set_index(const QModelIndex &index);
    QModelIndex get_index();
private:
//    void saveTreeNode(QStandardItem *pItem);
    Node *loadTreeNode(QJsonObject *jsonObj, Node *parent);
    QModelIndex last_sel_index;
};

#endif // MODEL_H
