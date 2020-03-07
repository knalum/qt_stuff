#ifndef STRINGNODE_H
#define STRINGNODE_H

#include "abstractnode.h"


class StringNode : public AbstractNode
{
public:
    StringNode();
    StringNode(QString name,const QString &value) : AbstractNode(name+" : "+value,QIcon(":/res/img/text.png")) {
        this->value=value;
        this->name = name;
    }

    QString getType() const override {return "string";}
    QString getValue() {return value;}
    QString getTextValue() const override {return value;}
    void setValue(QString v) override {value = v;}

    QString getName() const override {return name;}
    void setName(QString name) override {this->name = name;}


private:
    QString value;
};
#endif // STRINGNODE_H
