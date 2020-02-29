#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

#include <QStandardItemModel>


class AbstractNode : public QStandardItem
{
public:
    AbstractNode();
    AbstractNode(QString n,QIcon i) : QStandardItem(i,n) {}

    virtual QString getType() = 0;
    virtual QString getTextValue() = 0;
    virtual void setValue(QString) = 0;



};

#endif // ABSTRACTNODE_H
