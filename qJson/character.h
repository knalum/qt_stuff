#ifndef CHARACTER_H
#define CHARACTER_H

#include <QJsonObject>
#include <QString>



class Character
{
public:
    Character();
    Character(QString n) : name(n) {};

    void read(const QJsonObject&);
    void write(QJsonObject&) const;

    QString name;
};

#endif // CHARACTER_H
