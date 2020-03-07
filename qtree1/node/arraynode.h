#include "abstractnode.h"

#ifndef ARRAYNODE_H
#define ARRAYNODE_H


class ArrayNode  : public AbstractNode
{
public:
    ArrayNode();
    ArrayNode(QString n) : AbstractNode(n+" : []",QIcon(":/res/img/array.png")) {
        this->name = n;
    }

    QString getType() const override {return "array";}
    QString getTextValue() const override {return "";}
    void setValue(QString) override {}

    QJsonArray writeJsonArray(QJsonArray &arr) const;
    QJsonArray readJson(const QJsonArray &arr);
    void readObj(QJsonDocument doc);

    QString getName() const override {return name;}
    void setName(QString name) override {this->name = name;}

};

#endif // ARRAYNODE_H
