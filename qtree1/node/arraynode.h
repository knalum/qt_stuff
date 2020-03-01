#include "abstractnode.h"

#ifndef ARRAYNODE_H
#define ARRAYNODE_H


class ArrayNode  : public AbstractNode
{
public:
    ArrayNode();
    ArrayNode(QString n) : AbstractNode(n,QIcon(":/res/img/array.png")) {}

    QString getType() const override {return "array";}
    QString getTextValue() const override {return "";}
    void setValue(QString) override {}

    QJsonArray writeJsonArray(QJsonArray &arr) const;
    QJsonArray readJson(const QJsonArray &arr);

};

#endif // ARRAYNODE_H
