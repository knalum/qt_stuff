#include "mainwindow.h"
#include "treemodel.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QDebug>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QUuid>
#include <QTreeWidgetItemIterator>
#include <QRandomGenerator>
#include <QShortcut>
#include <QClipboard>

#include <node/arraynode.h>
#include <node/numericnode.h>
#include <node/objectnode.h>
#include <node/stringnode.h>


AbstractNode *getNode(AbstractNode *item){
    if( dynamic_cast<StringNode*>(item) != nullptr ){
        auto *stringNode = dynamic_cast<StringNode*>(item);
        return new StringNode(stringNode->text(),stringNode->getValue());
    }else if( dynamic_cast<NumericNode*>(item) != nullptr ){
        auto *numericNode = dynamic_cast<NumericNode*>(item);
        return new NumericNode(numericNode->text(),numericNode->getValue());
    }else if( dynamic_cast<ObjectNode*>(item) != nullptr ){
        auto *node = dynamic_cast<ObjectNode*>(item);
        return new ObjectNode(node->text());
    }else if( dynamic_cast<ArrayNode*>(item) != nullptr ){
        auto *node = dynamic_cast<ArrayNode*>(item);
        return new ArrayNode(node->text());
    }
    qDebug()<<"Error";
    return nullptr;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeView->header()->hide();
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    model = new TreeModel(this);
    ui->treeView->setModel(model);


    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged,this, &MainWindow::changed);

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QWidget::customContextMenuRequested, this, &MainWindow::onCustomContextMenu);

    this->on_actionNew_triggered();

    // Copy
    auto shortcutCopy = new QShortcut(QKeySequence::Copy, ui->treeView);
    QObject::connect(shortcutCopy, &QShortcut::activated, [this] () {
        auto selected = ui->treeView->selectionModel()->selectedIndexes();

        if (selected.size() > 0){
            auto *item = model->itemFromIndex(selected.at(0));
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText(QString("Custom clipboard text: %1").arg(item->text()));

            AbstractNode *castNode = castNode = getNode(item);
            this->copy = castNode;
        }
    });

    // Paste
    auto shortcutPaste = new QShortcut(QKeySequence::Paste, ui->treeView);
    QObject::connect(shortcutPaste, &QShortcut::activated, [this] () {
        auto selected = ui->treeView->selectionModel()->selectedIndexes();

        if (selected.size() > 0 && this->copy != nullptr){
            auto *item = model->itemFromIndex(selected.at(0));
            AbstractNode *node = this->copy;
            auto clone = getNode(node);
            item->appendRow(clone);
            ui->treeView->setExpanded(selected.at(0),true);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered(){
    model->clear();
    auto *root = new ObjectNode("root");
    model->appendRow(root);
}

void MainWindow::changed(const QItemSelection &selected, const QItemSelection &deselected){
    QModelIndexList localIndexes = selected.at(0).indexes();
    AbstractNode *item = getNode(model->itemFromIndex(localIndexes.at(0)));

    ui->lineEdit->setText(item->text());
    ui->typeLabel->setText(item->getType());
    ui->valueField->setText(item->getTextValue());

    if( dynamic_cast<ObjectNode*>(item) != nullptr ){
        ui->valueField->setVisible(false);
    }else{
        ui->valueField->setVisible(true);
    }
}

void MainWindow::onCustomContextMenu(const QPoint &point){
    QModelIndex index = ui->treeView->indexAt(point);

    QMenu *menu = new QMenu;
    auto addSubmenu = new QMenu("Add");
    auto addObject = addSubmenu->addAction("Object");
    auto addString = addSubmenu->addAction("String");
    auto addNumber = addSubmenu->addAction("Number");
    auto addArray = addSubmenu->addAction("Array");

    menu->addMenu(addSubmenu);

    QObject::connect(addObject, &QAction::triggered, this, [this, menu, addObject,index](){
        this->addNode(index,new ObjectNode("Object"));
        addObject->deleteLater();
        menu->deleteLater();
    });
    QObject::connect(addString, &QAction::triggered, this, [this, menu, addString,index](){
        this->addNode(index,new StringNode("String","string value"));
        addString->deleteLater();
        menu->deleteLater();
    });
    QObject::connect(addNumber, &QAction::triggered, this, [this, menu, addNumber,index](){
        this->addNode(index,new NumericNode("Number",0));
        addNumber->deleteLater();
        menu->deleteLater();
    });
    QObject::connect(addArray, &QAction::triggered, this, [this, menu, addArray,index](){
        this->addNode(index,new ArrayNode("Array"));
        addArray->deleteLater();
        menu->deleteLater();
    });


    if (index.isValid() && index.row() % 2 == 0) {
        menu->exec(ui->treeView->viewport()->mapToGlobal(point));
    }
}

void MainWindow::addNode(QModelIndex item,AbstractNode *node){
    AbstractNode *parent = model->itemFromIndex(item);

    parent->appendRow(node);
    ui->treeView->setExpanded(item,true);

    ui->treeView->header()->setStretchLastSection(true);
    ui->treeView->resizeColumnToContents(0);
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1){
    QModelIndexList localSelectedIndexes = ui->treeView->selectionModel()->selectedIndexes();
    AbstractNode *item = model->itemFromIndex(localSelectedIndexes.at(0));
    item->setText(arg1);
}


void MainWindow::on_valueField_textChanged(const QString &arg1){
    QModelIndexList localSelectedIndexes = ui->treeView->selectionModel()->selectedIndexes();
    AbstractNode *item = model->itemFromIndex(localSelectedIndexes.at(0));
    item->setValue(arg1);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"About","qtree - a basic tree editor.\nMade by Konstantin Nalum");
}

void MainWindow::on_actionremoveItem_triggered()
{
    QModelIndexList idx = ui->treeView->selectionModel()->selectedIndexes();
    for(auto item : idx){
        AbstractNode *itemToDelete = model->itemFromIndex(item);
        qDebug()<<"Remove :"+QString::number(itemToDelete->row())+" . "<<itemToDelete->text();
        this->model->removeRow(itemToDelete->row(),item.parent());

    }
}

void MainWindow::on_actionString_Item_triggered()
{
    QModelIndexList idx = ui->treeView->selectionModel()->selectedIndexes();
    for(auto item : idx){
        auto name = QUuid::createUuid().toString().mid(1,6);
        addNode(item,new StringNode(name,"value..."));
    }
}

void MainWindow::on_actionArray_triggered(){
    QModelIndexList idx = ui->treeView->selectionModel()->selectedIndexes();
    for(auto item : idx){
        auto name = QUuid::createUuid().toString().mid(1,6);
        addNode(item,new ArrayNode(name));
    }
}

void dfs(AbstractNode *root){
    auto data = root->data().toString();

    QString msg;
    msg.sprintf("type=%s name=%s  value=%s",
                root->getType().toStdString().c_str(),
                root->text().toStdString().c_str(),
                root->getTextValue().toStdString().c_str());
    qDebug()<<msg;

    int rows = root->rowCount();
    for(int i=0;i<rows;i++){
        QStandardItem *localChild = root->child(i,0);
        AbstractNode *casted = dynamic_cast<AbstractNode*>(localChild);
        dfs(casted);
    }
}

void MainWindow::on_actionSave_triggered(){
    QStandardItem *item = model->item(0,0);
    AbstractNode *casted = dynamic_cast<AbstractNode*>(item);
    dfs(casted);
}

void MainWindow::on_actionAdd_object_triggered(){
    QModelIndexList idx = ui->treeView->selectionModel()->selectedIndexes();
    for(auto item : idx){
        auto name = QUuid::createUuid().toString().mid(1,6);
        addNode(item,new ObjectNode(name));
    }
}

void MainWindow::on_actionNumeric_triggered()
{
    QModelIndexList idx = ui->treeView->selectionModel()->selectedIndexes();
    for(auto item : idx){
        auto name = QUuid::createUuid().toString().mid(1,6);
        auto value = QRandomGenerator::global()->generate()%10;
        addNode(item,new NumericNode(name,value));
    }
}

