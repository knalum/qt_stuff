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
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileDialog>
#include <QJsonArray>
#include <QComboBox>

#include <node/arraynode.h>
#include <node/booleannode.h>
#include <node/numericnode.h>
#include <node/objectnode.h>
#include <node/stringnode.h>


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

            QString json(toJsonWithSelectedNode(item).toJson());

            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText(json);
        }
    });

    // Paste
    auto shortcutPaste = new QShortcut(QKeySequence::Paste, ui->treeView);
    QObject::connect(shortcutPaste, &QShortcut::activated, [this] () {
        auto selected = ui->treeView->selectionModel()->selectedIndexes();

        if (selected.size() > 0 ){

            QClipboard *clipboard = QApplication::clipboard();
            QString jsonStr = clipboard->text();
            QJsonDocument loadDoc(QJsonDocument::fromJson(clipboard->text().toUtf8()));



            AbstractNode *item = model->itemFromIndex(selected.at(0));
            if( dynamic_cast<ObjectNode*>(item) != nullptr ){
                ObjectNode *aNode = dynamic_cast<ObjectNode*>(item);
                aNode->readJson(loadDoc.object());
            }else if( dynamic_cast<ArrayNode*>(item) != nullptr ){
                ArrayNode *aNode = dynamic_cast<ArrayNode*>(item);
                aNode->readObj(loadDoc);
            }


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

    // obj
    /*
    auto ch1 = new ObjectNode("ch1");
    ch1->appendRow(new ObjectNode("sub1"));
    auto ch2 = new ObjectNode("ch2");
    auto ch3 = new ObjectNode("ch3");
    root->appendRow(ch1);
    root->appendRow(ch2);
    root->appendRow(ch3);
    */

    // arr
    /*
    auto arr = new ArrayNode("arr");
    arr->appendRow(new StringNode("str1","text"));
    root->appendRow(arr);
    model->appendRow(root);
    */

    model->appendRow(root);

    ui->treeView->setExpanded(root->index(),true);
    QModelIndex idx = model->indexFromItem(root);
    ui->treeView->selectionModel()->select(idx,QItemSelectionModel::SelectCurrent);
}

void MainWindow::changed(const QItemSelection &selected, const QItemSelection &deselected){

    QModelIndexList localIndexes = selected.at(0).indexes();

    AbstractNode *item = dynamic_cast<AbstractNode*>(model->itemFromIndex(localIndexes.at(0)));

    ui->lineEdit->setText(item->text());
    ui->typeLabel->setText(item->getType());

    if( dynamic_cast<BooleanNode*>(item) != nullptr ){
        ui->comboBox->setVisible(true);
        auto boolNode = dynamic_cast<BooleanNode*>(item);
        ui->comboBox->setCurrentText(boolNode->getTextValue());
    }else{
        ui->comboBox->setVisible(false);
    }
    ui->valueField->setText(item->getTextValue());

    if( dynamic_cast<ObjectNode*>(item) != nullptr || dynamic_cast<BooleanNode*>(item) != nullptr){
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
    int rows = root->rowCount();
    for(int i=0;i<rows;i++){
        QStandardItem *localChild = root->child(i,0);
        AbstractNode *casted = dynamic_cast<AbstractNode*>(localChild);
        dfs(casted);
    }
}


void MainWindow::on_actionOpen_triggered(){
    auto fileName = QFileDialog::getOpenFileName(this,"Open the file",QDir::homePath());
    if( fileName.isEmpty() || fileName.isNull() ){
        return;
    }else{
        this->filepath = fileName;
    }

    QFile file(fileName);
    if( !file.open( QFile::ReadOnly | QFile::Text) ){
        QMessageBox::warning(this,"","Can't open file");
        return;
    }
    this->filepath = fileName;

    QTextStream in(&file);

    QByteArray jsonByteArr = file.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(jsonByteArr));

    QString jsonString = loadDoc.toJson(QJsonDocument::Indented);


    ObjectNode *root = new ObjectNode("newRoot");
    root->readJson(loadDoc.object());

    model->clear();
    model->appendRow(root);

    on_actionExpand_children_triggered();
    file.close();
}


void MainWindow::on_save_triggered(){
    if( this->filepath == nullptr ){
        auto filename = QFileDialog::getSaveFileName(this,"Save as",QDir::homePath());
        if( filename.isNull() || filename.isEmpty() ){
            return;
        }else{
            this->filepath = filename;
        }
    }


    QFile file(this->filepath);
    if( !file.open(QFile::WriteOnly | QFile::Text) ){
        QMessageBox::warning(this,"","Couldn't open file");
        return;
    }

    QStandardItem *nodeIdx = model->item(0,0);
    QJsonDocument saveDoc = this->toJson(nodeIdx);

    file.write(saveDoc.toJson());
    file.close();

}

void MainWindow::on_actionSave_triggered(){
    auto filename = QFileDialog::getSaveFileName(this,"Save as",QDir::homePath());
    if( filename.isNull() || filename.isEmpty() ){
        return;
    }else{
        this->filepath = filename;
    }

    QFile file(this->filepath);
    if( !file.open(QFile::WriteOnly | QFile::Text) ){
        QMessageBox::warning(this,"","Couldn't open file");
        return;
    }

    QStandardItem *nodeIdx = model->item(0,0);
    QJsonDocument saveDoc = this->toJson(nodeIdx);

    file.write(saveDoc.toJson());
    file.close();
}


QJsonDocument MainWindow::toJsonWithSelectedNode(QStandardItem *nodeIdx) const{
    QJsonObject topRoot;

    if( dynamic_cast<ObjectNode*>(nodeIdx) != nullptr ){
        QJsonObject rootJsonObj;
        ObjectNode *item = dynamic_cast<ObjectNode*>(nodeIdx);
        item->writeJson(rootJsonObj);
        topRoot[nodeIdx->text()] = rootJsonObj;
    }else if( dynamic_cast<ArrayNode*>(nodeIdx) != nullptr ){
        QJsonArray arr;
        ArrayNode *item = dynamic_cast<ArrayNode*>(nodeIdx);
        item->writeJsonArray(arr);
        topRoot[nodeIdx->text()] = arr;
    }else if( dynamic_cast<StringNode*>(nodeIdx) != nullptr ){
        QJsonObject obj;
        StringNode *item = dynamic_cast<StringNode*>(nodeIdx);
        topRoot[item->text()] = item->getValue();
    }else if( dynamic_cast<NumericNode*>(nodeIdx) != nullptr ){
        QJsonObject obj;
        NumericNode *item = dynamic_cast<NumericNode*>(nodeIdx);
        topRoot[item->text()] = item->getValue();
    }else if( dynamic_cast<BooleanNode*>(nodeIdx) != nullptr ){
        QJsonObject obj;
        BooleanNode *item = dynamic_cast<BooleanNode*>(nodeIdx);
        topRoot[item->text()] = item->getValue();
    }


    QJsonDocument saveDoc(topRoot);
    return saveDoc;
}

QJsonDocument MainWindow::toJson(QStandardItem *nodeIdx) const{
    QJsonObject rootJsonObj;
    ObjectNode *item = dynamic_cast<ObjectNode*>(nodeIdx);
    item->writeJson(rootJsonObj);
    QJsonDocument saveDoc(rootJsonObj);
    return saveDoc;
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

void MainWindow::on_actionBoolean_triggered(){
    QModelIndexList idx = ui->treeView->selectionModel()->selectedIndexes();
    for(auto item : idx){
        auto name = QUuid::createUuid().toString().mid(1,6);
        addNode(item,new BooleanNode(name,false));
    }
}


void MainWindow::on_actionRandom_tree_triggered(){
    const int RANDOM_LEVEL = 10;

    this->on_actionNew_triggered();
    auto root = dynamic_cast<AbstractNode*>(this->model->item(0,0));
    randomize(root,RANDOM_LEVEL);

    this->count=0;
    this->countNodes(root);
    ui->statusBar->showMessage("Nodecount = "+QString::number(this->count));
}


AbstractNode* MainWindow::randomize(AbstractNode *root,int level){
    int rows = QRandomGenerator::global()->generate()%level;
    for(int i=0;i<rows;i++){

        auto name = QUuid::createUuid().toString().mid(1,6);


        int type = QRandomGenerator::global()->generate()%3;
        if( type == 0 ){
            auto value = QUuid::createUuid().toString().mid(1,6);
            auto node = new StringNode(name,value);
            root->appendRow(node);
        }else if (type == 1 ){
            auto value = 100*QRandomGenerator::global()->generate()%10;
            auto node = new NumericNode(name,value);
            root->appendRow(node);
        }else if (type == 2 ){
            auto newNode = new ObjectNode(name);
            randomize(newNode,level-1);
            root->appendRow(newNode);
        }
    }


}

int MainWindow::countNodes(AbstractNode *root){
    for(int i=0;i<root->rowCount();++i){
        ++count;
        auto ch = dynamic_cast<AbstractNode*>(root->child(i,0));
        countNodes(ch);
    }
}

void MainWindow::dfs(QStandardItem * item,bool setExpanded){
    ui->treeView->setExpanded(item->index(),setExpanded);
    for(int i=0;i<item->rowCount();++i){
        dfs(item->child(i,0),setExpanded);
    }
}


void MainWindow::on_actionExpand_children_triggered(){
    QModelIndex idx;
    if( !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ){
        idx = ui->treeView->selectionModel()->selectedIndexes().at(0);
    }else{
        idx = model->index(0,0);
    }

    auto item = model->itemFromIndex(idx);
    dfs(item,true);
}

void MainWindow::on_tabWidget_tabBarClicked(int index){
    QModelIndex idx;
    auto item = model->item(0,0);

    QJsonDocument json = toJson(item);
    QString jsonString = json.toJson(QJsonDocument::Indented);
    ui->jsonTextField->setPlainText(jsonString);
}


void MainWindow::on_actionUnexpand_children_triggered()
{
    QModelIndex idx;
    if( !ui->treeView->selectionModel()->selectedIndexes().isEmpty() ){
        idx = ui->treeView->selectionModel()->selectedIndexes().at(0);
    }else{
        idx = model->index(0,0);
    }

    auto item = model->itemFromIndex(idx);
    dfs(item,false);
}



void MainWindow::on_comboBox_currentTextChanged(const QString &arg){
    QModelIndexList localSelectedIndexes = ui->treeView->selectionModel()->selectedIndexes();
    AbstractNode *item = model->itemFromIndex(localSelectedIndexes.at(0));
    auto boolNode = dynamic_cast<BooleanNode*>(item);
    if( arg == "true" ){
        boolNode->setValue(true);
    }else if( arg == "false" ){
        boolNode->setValue(false);
    }
}
