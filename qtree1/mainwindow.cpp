#include "jsonarrayitem.h"
#include "mainwindow.h"
#include "stringitem.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QDebug>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QUuid>
#include <QTreeWidgetItemIterator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeView->header()->hide();
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    model = new QStandardItemModel(1,1,this);
    ui->treeView->setModel(model);


    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged,this, &MainWindow::changed);

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QWidget::customContextMenuRequested, this, &MainWindow::onCustomContextMenu);

    this->on_actionNew_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered()
{
    model->clear();
    auto *root = new QStandardItem(QIcon(":/res/img/defaultIcon.png"),"Root");

    model->appendRow(root);
}

void MainWindow::changed(const QItemSelection &selected, const QItemSelection &deselected)
{

    QModelIndexList localIndexes = selected.at(0).indexes();
    QStandardItem *selectedItem = model->itemFromIndex(localIndexes.at(0));
    ui->lineEdit->setText(selectedItem->text());

    QVariant localData = selectedItem->data();

    auto type = localData.toString();
    ui->typeLabel->setText(type);
}

void MainWindow::onCustomContextMenu(const QPoint &point)
{
    QModelIndex index = ui->treeView->indexAt(point);

    qDebug()<<"ctx menu";
    QMenu *menu = new QMenu;
    auto addNode = menu->addAction("Add");

    QObject::connect(addNode, &QAction::triggered, this, [this, menu, addNode,index](){
        qDebug()<<"Add";
        //this->addNode(index); // TODO
        addNode->deleteLater();
        menu->deleteLater();
    });

        if (index.isValid() && index.row() % 2 == 0) {
            menu->exec(ui->treeView->viewport()->mapToGlobal(point));
        }
}

void MainWindow::addNode(QModelIndex item,QStandardItem *node){
    QStandardItem *parent = model->itemFromIndex(item);

    parent->appendRow(node);
    ui->treeView->setExpanded(item,true);

    ui->treeView->header()->setStretchLastSection(true);
    ui->treeView->resizeColumnToContents(0);
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1){
    QModelIndexList localSelectedIndexes = ui->treeView->selectionModel()->selectedIndexes();
    QStandardItem *item = model->itemFromIndex(localSelectedIndexes.at(0));
    item->setText(arg1);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"About","qtree - a basic tree editor.\nMade by Konstantin Nalum");
}

void MainWindow::on_actionremoveItem_triggered()
{
    QModelIndexList idx = ui->treeView->selectionModel()->selectedIndexes();
    for(auto item : idx){
        QStandardItem *itemToDelete = model->itemFromIndex(item);
        qDebug()<<"Remove :"+QString::number(itemToDelete->row())+" . "<<itemToDelete->text();
        this->model->removeRow(itemToDelete->row(),item.parent());

    }
}

void MainWindow::on_actionString_Item_triggered()
{
    QModelIndexList idx = ui->treeView->selectionModel()->selectedIndexes();
    for(auto item : idx){
        auto name = QUuid::createUuid().toString().mid(1,6);
        addNode(item,new StringItem(name));
    }
}

void MainWindow::on_actionArray_triggered(){
    QModelIndexList idx = ui->treeView->selectionModel()->selectedIndexes();
    for(auto item : idx){
        auto name = QUuid::createUuid().toString().mid(1,6);
        addNode(item,new JsonArrayItem(name));
    }
}

void dfs(QStandardItem *root){
    auto data = root->data().toString();
    qDebug()<<root->text()+"\ttype="+data;
    int rows = root->rowCount();
    for(int i=0;i<rows;i++){
        QStandardItem *localChild = root->child(i,0);
        dfs(localChild);
    }
}

void MainWindow::on_actionSave_triggered(){
    dfs(model->item(0,0));

}
