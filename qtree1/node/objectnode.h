#ifndef OBJECTNODE_H
#define OBJECTNODE_H

#include "abstractnode.h"



class ObjectNode : public AbstractNode
{
public:
    ObjectNode();
    ObjectNode(QString n) : AbstractNode(n,QIcon(":/res/img/module.png")) {}

    QString getType() override {return "object";}
    QString getTextValue() override {return "";}
    void setValue(QString) override {}
};

#endif // OBJECTNODE_H
