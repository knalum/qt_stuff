#ifndef BOOLEANNODE_H
#define BOOLEANNODE_H

#include "abstractnode.h"

#include <QString>



class BooleanNode : public AbstractNode{
public:
    BooleanNode();
    BooleanNode(QString n,bool val) : AbstractNode(n,QIcon(":/res/img/boolean.png")) {this->value = val;}
    QString getType() const override {return "boolean";}
    QString getTextValue() const override {return value ? "true" : "false";}
    void setValue(QString val) override {
        if( val == "true"){
            value = true;
        }else{
            value = false;
        }
    }
    void setValue(bool val) {
        this->value = val;
    }
    bool getValue() {return value;}

private:
    bool value;
};

#endif // BOOLEANNODE_H
