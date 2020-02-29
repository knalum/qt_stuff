#include "abstractnode.h"

#ifndef ARRAYNODE_H
#define ARRAYNODE_H


class ArrayNode  : public AbstractNode
{
public:
    ArrayNode();
    ArrayNode(QString n) : AbstractNode(n,QIcon(":/res/img/array.png")) {}

    QString getType() override {return "object";}
    QString getTextValue() override {return "";}
    void setValue(QString) override {}
};

#endif // ARRAYNODE_H
