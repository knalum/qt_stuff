#ifndef STRINGNODE_H
#define STRINGNODE_H

#include "abstractnode.h"



class StringNode : public AbstractNode
{
public:
    StringNode();
    StringNode(QString n,const QString &value) : AbstractNode(n,QIcon(":/res/res/font.png")) {this->value=value;}

    QString getType() override {return "string";}
    QString getValue() {return value;}

private:
    QString value;
};

#endif // STRINGNODE_H
