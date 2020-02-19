#include "item.h"

#include <QCoreApplication>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QUuid>
#include "pugixml.hpp"

void writeToXmlFile(QVector<Item*> &items,const QString path){
    QFile file(path);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("items");

    for(Item *item : items){
        stream.writeStartElement("item");
        stream.writeTextElement("name",item->getName());
        stream.writeTextElement("quantity",QString::number(item->getQuantity()));
        stream.writeEndElement();
    }

    stream.writeEndElement();

    stream.writeEndDocument();
    file.close();
}

const QString createShortUuid() {
    return QUuid::createUuid().toString().mid(1,5);
}

void testWrite(const QString path){
    QVector<Item*> items;
    for(int i=0;i<10;++i) items.append(new Item(createShortUuid(),2));

    writeToXmlFile(items,path);

    for(auto p : items) delete p;
    items.clear();
}

QVector<Item*> testRead(const QString path){
    QFile file(path);
    file.open(QIODevice::ReadOnly);

    QVector<Item*> items;

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.toStdWString().c_str());
    if (!result) qDebug()<<"XML error";
    for ( pugi::xml_node  node : doc.child("items").children() ){
        const pugi::char_t *name = node.child("name").text().get();
        const pugi::char_t *quantity = node.child("quantity").text().get();
        QString nameS(name);
        QString quantityS(quantity);
        int quantityI = quantityS.toInt();

        items.append(new Item(nameS,quantityI));
    }


    file.close();

    return items;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    const QString path = QDir::homePath()+"/myFile.xml";

    //testWrite(path);
    QVector<Item *> localTestRead = testRead(path);

    for(const auto &i : localTestRead) qDebug()<<i->getName();

    for(auto p : localTestRead) delete p;
    localTestRead.clear();


    return a.exec();
}
