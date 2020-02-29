#ifndef STRINGNODE_H
#define STRINGNODE_H

#include "abstractnode.h"


class StringNode : public AbstractNode
{
public:
    StringNode();
    StringNode(QString n,const QString &value) : AbstractNode(n,QIcon(":/res/img/text.png")) {this->value=value;}

    QString getType() override {return "string";}
    QString getValue() {return value;}
    QString getTextValue() override {return value;}
    void setValue(QString v) override {value = v;}

private:
    QString value;
};
#endif // STRINGNODE_H
