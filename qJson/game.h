#ifndef GAME_H
#define GAME_H

#include "character.h"

#include <QJsonObject>
#include <QString>


class Game
{
public:
    Game();
    Game(QString n,int y) : name(n),releasedYear(y) {};
    void setName(const QString &n) {this->name=n;}
    void setReleaseYear(const int &y) {this->releasedYear=y;}
    void setPlayer(Character c) {this->player = c;}

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    bool saveGame() const;
    bool loadGame();

private:
    QString name;
    int releasedYear;
    Character player;

};

#endif // GAME_H
