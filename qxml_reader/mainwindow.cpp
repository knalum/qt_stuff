#include "item.h"
#include "mainwindow.h"
#include "pugixml.hpp"
#include "ui_mainwindow.h"

#include <QFile>
#include <QStringListModel>
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    model = new QStandardItemModel(0,2);
    ui->tableView->setModel(model);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    QList<QStandardItem*> list{new QStandardItem("a"),new QStandardItem("b")};
    model->appendRow(list);
}

MainWindow::~MainWindow()
{
    delete ui;
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


void MainWindow::on_pushButton_clicked(){
    auto items = testRead(QDir::homePath()+"/myFile.xml");
    qDebug()<<"Read: "<<items.size()<<" items";

    for(const auto &item : items){
        QList<QStandardItem*> list{new QStandardItem(item->getName()),new QStandardItem(QString::number(item->getQuantity()))};
        model->appendRow(list);
    }
    for(auto p : items) delete p;
    items.clear();
}
