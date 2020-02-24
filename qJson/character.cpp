#include "character.h"

Character::Character()
{

}

void Character::read(const QJsonObject &json){
    name = json["name"].toString();
}

void Character::write(QJsonObject &json) const{
    json["name"] = name;
}
