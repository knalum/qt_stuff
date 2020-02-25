#ifndef NUMERICNODE_H
#define NUMERICNODE_H

#include "abstractnode.h"



class NumericNode : public AbstractNode
{
public:
    NumericNode();
    NumericNode(QString n,const double &d) : AbstractNode(n,QIcon(":/res/res/page_excel.png")) {this->value=d;}

    QString getType() override {return "numeric";}
    double getValue() {return value;}

private:
    double value;
};

#endif // NUMERICNODE_H
