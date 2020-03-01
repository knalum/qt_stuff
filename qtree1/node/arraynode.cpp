#include "arraynode.h"
#include "numericnode.h"
#include "objectnode.h"
#include "stringnode.h"

#include <QJsonArray>

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
        }else if( value.type() == QJsonValue::Type::Object ){
            ObjectNode *obj = new ObjectNode();
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
