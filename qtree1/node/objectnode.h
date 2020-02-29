#ifndef OBJECTNODE_H
#define OBJECTNODE_H

#include "abstractnode.h"



class ObjectNode : public AbstractNode
{
public:
    ObjectNode();
    ObjectNode(QString n) : AbstractNode(n,QIcon(":/res/img/module.png")) {}

    QString getType() const override {return "object";}
    QString getTextValue() const override {return "";}
    void setValue(QString) override {}

    QJsonObject writeJson(QJsonObject &json) const;
};

#endif // OBJECTNODE_H
