#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "abstractnode.h"

#include <QAbstractItemModel>
#include <QStandardItemModel>



class TreeModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(QObject *parent = nullptr) : QStandardItemModel(parent) {}

    AbstractNode *itemFromIndex(const QModelIndex &index) const {
        AbstractNode *item = static_cast<AbstractNode*>( QStandardItemModel::itemFromIndex(index) );
        return item;
    }

private:
    AbstractNode *rootItem;
};

#endif // TREEMODEL_H
