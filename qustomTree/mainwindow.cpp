#include "mainwindow.h"
#include "numericnode.h"
#include "stringnode.h"
#include "treemodel.h"
#include "ui_mainwindow.h"
#include <QAbstractItemModel>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    auto *model = new TreeModel(this);
    ui->treeView->setModel(model);
    ui->treeView->header()->hide();

    auto *root = new StringNode("myStr","my text value");

    root->appendRow(new StringNode("Child_1","child value"));
    root->appendRow(new NumericNode("Child_2",123.456));
    model->appendRow(root);

    connect(ui->treeView->selectionModel(),&QItemSelectionModel::selectionChanged,this,[this,model](const QItemSelection &selected, const QItemSelection &deselected){
        QModelIndexList localIndexes = selected.at(0).indexes();
        AbstractNode *item = model->itemFromIndex(localIndexes.at(0));

        this->ui->typeField->setText(item->getType());
        this->ui->nameField->setText(item->text());


        if( dynamic_cast<StringNode*>(item) != nullptr ){
            auto *stringNode = dynamic_cast<StringNode*>(item);
            qDebug("String node with text=%s",stringNode->getValue().toStdString().c_str());

            this->ui->valueField->setText(stringNode->getValue());
        }else if( dynamic_cast<NumericNode*>(item) != nullptr ){
            auto *numericNode = dynamic_cast<NumericNode*>(item);
            qDebug("Numeric node with value=%f",numericNode->getValue());
            this->ui->valueField->setText(QString::number(numericNode->getValue()));
        }else{
            qDebug("Default node");
            this->ui->valueField->setText("");
        }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
