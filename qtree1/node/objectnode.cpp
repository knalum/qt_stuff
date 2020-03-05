#include "arraynode.h"
#include "booleannode.h"
#include "numericnode.h"
#include "objectnode.h"
#include "stringnode.h"

#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QJSonValue>

ObjectNode::ObjectNode()
{

}

QJsonObject ObjectNode::writeJson(QJsonObject &json) const{

    for(int i=0;i<rowCount();++i){
        auto ch = child(i,0);

        if( dynamic_cast<ObjectNode*>(ch) != nullptr ){
            auto child = dynamic_cast<ObjectNode*>(ch);
            QJsonObject obj;
            json[child->text()] = child->writeJson(obj);
        }else if ( dynamic_cast<ArrayNode*>(ch) != nullptr ){
            auto child = dynamic_cast<ArrayNode*>(ch);
            QJsonArray arr;
            json[child->text()] = child->writeJsonArray(arr);
        }else if ( dynamic_cast<StringNode*>(ch) != nullptr ){
            auto child = dynamic_cast<StringNode*>(ch);
            json[child->text()] = child->getTextValue();
        }else if ( dynamic_cast<BooleanNode*>(ch) != nullptr ){
            auto child = dynamic_cast<BooleanNode*>(ch);
            json[child->text()] = child->getValue();
        }else if ( dynamic_cast<NumericNode*>(ch) != nullptr ){
            auto child = dynamic_cast<NumericNode*>(ch);
            json[child->text()] = child->getValue();
        }
    }

    return json;
}

QJsonObject ObjectNode::readJson(const QJsonObject &obj){
    QStringList keys = obj.keys();
    for(int i=0;i<keys.size();++i){
        const QString key = keys.at(i);
        const QJsonValue value = obj.value(key);
        if( value.type() == QJsonValue::Type::String ){
            appendRow(new StringNode(key,value.toString()));
        }else if( value.type() == QJsonValue::Type::Double ){
            appendRow(new NumericNode(key,value.toDouble()));
        }else if( value.type() == QJsonValue::Type::Bool ){
            appendRow(new BooleanNode(key,value.toBool()));
        }else if( value.type() == QJsonValue::Type::Object ){
            ObjectNode *obj = new ObjectNode(key);
            obj->readJson(value.toObject());
            appendRow(obj);
        }else if( value.type() == QJsonValue::Type::Array ){
            ArrayNode *arr = new ArrayNode(key);
            arr->readJson(value.toArray());
            appendRow(arr);
        }
    }
    return obj;
}


