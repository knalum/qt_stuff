#include "arraynode.h"
#include "booleannode.h"
#include "numericnode.h"
#include "objectnode.h"
#include "stringnode.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

ArrayNode::ArrayNode()
{

}

// TODO: Refactor into common with obj
QJsonArray ArrayNode::writeJsonArray(QJsonArray &arr) const{
    for(int i=0;i<rowCount();++i){
        auto ch = child(i,0);

        if( dynamic_cast<ObjectNode*>(ch) != nullptr ){
            auto child = dynamic_cast<ObjectNode*>(ch);
            QJsonObject obj;
            child->writeJson(obj);
            arr.append(QJsonValue(obj));
        }else if ( dynamic_cast<ArrayNode*>(ch) != nullptr ){
            auto child = dynamic_cast<ArrayNode*>(ch);
            QJsonArray arr;
            child->writeJsonArray(arr);
            arr.append(arr);

        }else if ( dynamic_cast<StringNode*>(ch) != nullptr ){
            auto child = dynamic_cast<StringNode*>(ch);

            arr.append(child->getTextValue());
        }else if ( dynamic_cast<NumericNode*>(ch) != nullptr ){
            auto child = dynamic_cast<NumericNode*>(ch);
            arr.append(child->getValue());
        }else if ( dynamic_cast<BooleanNode*>(ch) != nullptr ){
            auto child = dynamic_cast<BooleanNode*>(ch);
            arr.append(child->getValue());
        }
    }

    return arr;
}

// TODO: Refactor into common with obj
QJsonArray ArrayNode::readJson(const QJsonArray &arr){

    for(int i=0;i<arr.size();++i){
        QJsonValue value = arr.at(i);
        if( value.type() == QJsonValue::Type::String ){
            appendRow(new StringNode(value.toString(),value.toString()));
        }else if( value.type() == QJsonValue::Type::Double ){
            appendRow(new NumericNode("",value.toDouble()));
        }else if( value.type() == QJsonValue::Type::Bool ){
            appendRow(new BooleanNode("",value.toBool()));
        }else if( value.type() == QJsonValue::Type::Object ){
            ObjectNode *obj = new ObjectNode("");
            obj->readJson(value.toObject());
            appendRow(obj);
        }else if( value.type() == QJsonValue::Type::Array ){
            ArrayNode *arr = new ArrayNode();
            arr->readJson(value.toArray());
            appendRow(arr);
        }
    }
    return arr;
}

void ArrayNode::readObj(QJsonDocument doc){
    if( doc.isObject() ){
        QJsonObject obj = doc.object();
        QString key = obj.begin().key();
        QJsonValueRef valueRef = obj.begin().value();
        if( valueRef.isString() ){
            auto str = valueRef.toString();
            appendRow(new StringNode(key,str));
        }else if( valueRef.isDouble() ){
            auto num = valueRef.toDouble();
            appendRow(new NumericNode(key,num));
        }else if( valueRef.isBool() ){
            auto num = valueRef.toBool();
            appendRow(new BooleanNode(key,num));
        }else if( valueRef.isObject() ){
            QJsonObject jsonObj = valueRef.toObject();
            ObjectNode *objNode = new ObjectNode(key);
            objNode->readJson(jsonObj);
            appendRow(objNode);
        }
    }
}
