#include "game.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>

Game::Game(){
    qDebug()<<"Constructor for game";
}

void Game::read(const QJsonObject &json){
    name = json["name"].toString();
    releasedYear = json["releasedYear"].toInt();

    QJsonObject character = json["character"].toObject();
    player.read(character);
}

void Game::write(QJsonObject &json) const{
    json["name"] = this->name;
    json["releasedYear"] = this->releasedYear;

    QJsonObject characterObj;
    player.write(characterObj);
    json["character"] = characterObj;
}

bool Game::saveGame() const{

    QFile file(QDir::homePath()+"/save.json");
    if( !file.open(QIODevice::WriteOnly) ){
        qWarning("Couldn't open save file");
        return false;
    }

    QJsonObject gameObject;
    write(gameObject);
    QJsonDocument saveDoc(gameObject);

    file.write(saveDoc.toJson());

    file.close();
    return true;
}

bool Game::loadGame()
{
    QFile file(QDir::homePath()+"/save.json");
    if( !file.open(QIODevice::ReadOnly) ){
        qWarning("Couldn't load file");
        return false;
    }

    QByteArray saveData = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QString jsonString = loadDoc.toJson(QJsonDocument::Indented);
    qDebug()<<jsonString;

    read(loadDoc.object());
}
