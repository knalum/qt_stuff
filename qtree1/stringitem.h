#ifndef STRINGITEM_H
#define STRINGITEM_H

#include <QIcon>
#include <QObject>
#include <QStandardItemModel>
#include <QVariant>

class StringItem : public QStandardItem
{
public:
    StringItem();
    StringItem(QString name) : QStandardItem(QIcon(":/res/img/text.png"),name) {
        QVariant value("string");
        setData(value);
    };
};

#endif // STRINGITEM_H
