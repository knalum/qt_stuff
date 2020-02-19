#ifndef ITEM_H
#define ITEM_H

#include <QString>



class Item{
private:
    QString name;
    int quantity;
public:
    Item(QString,int);
    QString getName() {return name;}
    int getQuantity() {return quantity;}
};

#endif // ITEM_H
