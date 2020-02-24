#include "game.h"
#include "item.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QUuid>
#include <QJsonDocument>
#include <QJsonArray>
#include <QRandomGenerator>

void loadGame(){
    Game game2;
    game2.loadGame();
}

void saveGame(){
    Game game;
    game.setName("Age of empires");
    game.setReleaseYear(1998);
    game.setPlayer(Character("Knight"));
    game.saveGame();
}

QList<Game> createRandomGames(const int num){
    QList<Game> games;
    for(int i=0;i<num;++i){
        auto title = QUuid::createUuid().toString().mid(1,6);
        int year = 1990+QRandomGenerator::global()->generate()%10;
        games.append(Game(title,year));
    }
    return games;
}

void saveGames(){
    QList<Game> games = createRandomGames(100);

    QJsonArray arr;
    for(const auto g : games){
        QJsonObject gObj;
        g.write(gObj);
        arr.append(gObj);
    }

    QJsonObject obj;
    obj["games"] = arr;
    QJsonDocument doc(obj);
    QByteArray json = doc.toJson();

    QFile file(QDir::homePath()+"/jsonList.json");
    if( !file.open(QIODevice::WriteOnly) ){
        qWarning("Errro json list");
        return;
    }
    file.write(json);

    file.close();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //saveGame();
    //loadGame();

    saveGames();
    return a.exec();
}
