#ifndef ABSTRACTNODE_H
#define ABSTRACTNODE_H

#include <QJsonObject>
#include <QStandardItemModel>


class AbstractNode : public QStandardItem
{
public:
    AbstractNode();
    AbstractNode(QString n,QIcon i) : QStandardItem(i,n) {name = n;}

    virtual QString getType() const = 0;
    virtual QString getTextValue() const = 0;
    virtual void setValue(QString) = 0;

    virtual QString getName() const = 0;
    virtual void setName(QString name) = 0;

    QString name;

};

#endif // ABSTRACTNODE_H
