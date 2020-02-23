#ifndef JSONARRAYITEM_H
#define JSONARRAYITEM_H

#include <QObject>
#include <QStandardItemModel>

class JsonArrayItem : public QStandardItem
{
public:
    JsonArrayItem();
    JsonArrayItem(QString name) : QStandardItem(QIcon(":/res/img/array.png"),name) {
        QVariant value("JSON");
        setData(value);
    };

};

#endif // JSONARRAYITEM_H
