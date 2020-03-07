#ifndef OBJECTNODE_H
#define OBJECTNODE_H

#include "abstractnode.h"



class ObjectNode : public AbstractNode
{

public:
    ObjectNode();
    ObjectNode(QString n) : AbstractNode(n+" : {}",QIcon(":/res/img/module.png")) {
        this->name = n;
    }

    QString getType() const override {return "object";}
    QString getTextValue() const override {return "";}
    void setValue(QString) override {}

    QJsonObject writeJson(QJsonObject &json) const;
    QJsonObject readJson(const QJsonObject &obj);

    QString getName() const override {return name;}
    void setName(QString name) override {this->name = name;}


};

#endif // OBJECTNODE_H
