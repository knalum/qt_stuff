#ifndef OBJECTNODE_H
#define OBJECTNODE_H

#include "abstractnode.h"



class ObjectNode : public AbstractNode
{
public:
    ObjectNode();
    ObjectNode(QString n) : AbstractNode(n,QIcon(":/res/res/module.png")) {}

    QString getType() override {return "object";}
};

#endif // OBJECTNODE_H
