#include "arraynode.h"
#include "numericnode.h"
#include "objectnode.h"
#include "stringnode.h"

#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>

ObjectNode::ObjectNode()
{

}

QJsonObject ObjectNode::writeJson(QJsonObject &json) const{


    for(int i=0;i<rowCount();++i){
        auto ch = child(i,0);
        qDebug()<<ch->text();

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
        }else if ( dynamic_cast<NumericNode*>(ch) != nullptr ){
            auto child = dynamic_cast<NumericNode*>(ch);
            json[child->text()] = child->getValue();
        }
    }



    return json;
}
