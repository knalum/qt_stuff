#ifndef NUMERICNODE_H
#define NUMERICNODE_H

#include "abstractnode.h"

class NumericNode : public AbstractNode
{
public:
    NumericNode();
    NumericNode(QString n,const double &d) : AbstractNode(n+" : "+QString::number(d),QIcon(":/res/img/numeric.png")) {
        this->value=d;
        this->name = n;
    }

    QString getType() const override {return "numeric";}
    double getValue() {return this->value;}
    QString getTextValue() const override {return QString::number(value);}
    void setValue(QString str) override {
        bool ok;
        auto val = str.toDouble(&ok);
        if( ok == true ){
            value = val;
        }
    }

    QString getName() const override {return name;}
    void setName(QString name) override {this->name = name;}


private:
    double value;
};

#endif // NUMERICNODE_H
