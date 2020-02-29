#include "mainwindow.h"
#include "numericnode.h"
#include "objectnode.h"
#include "stringnode.h"
#include "treemodel.h"
#include "ui_mainwindow.h"
#include <QAbstractItemModel>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QDebug>
#include <QKeyEvent>
#include <QShortcut>
#include <QClipboard>

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
    }
    qDebug()<<"Error";
    return nullptr;

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    auto *model = new TreeModel(this);
    ui->treeView->setModel(model);
    ui->treeView->header()->hide();
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto *root = new ObjectNode("root");

    root->appendRow(new StringNode("Child_1","child value"));
    root->appendRow(new NumericNode("Child_2",123.456));
    root->appendRow(new ObjectNode("obj"));
    model->appendRow(root);

    // Copy
    auto shortcutCopy = new QShortcut(QKeySequence::Copy, ui->treeView);
    QObject::connect(shortcutCopy, &QShortcut::activated, [this,model] () {
        qDebug()<<"Copy";
        auto selected = ui->treeView->selectionModel()->selectedIndexes();

        // Get only first selected item's text.
        if (selected.size() > 0)
        {
            auto *item = model->itemFromIndex(selected.at(0));
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText(QString("Custom clipboard text: %1").arg(item->text()));

            AbstractNode *castNode = castNode = getNode(item);
            this->copy = castNode;
        }
    });


    // Paste
    auto shortcutPaste = new QShortcut(QKeySequence::Paste, ui->treeView);
    QObject::connect(shortcutPaste, &QShortcut::activated, [this,model] () {
        auto selected = ui->treeView->selectionModel()->selectedIndexes();



        // Get only first selected item's text.
        if (selected.size() > 0 && this->copy != nullptr)
        {
            auto *item = model->itemFromIndex(selected.at(0));
            AbstractNode *node = this->copy;
            auto clone = getNode(node);
            item->appendRow(clone);
            ui->treeView->setExpanded(selected.at(0),true);
        }
    });


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
        }else if( dynamic_cast<ObjectNode*>(item) != nullptr ){
            qDebug("ObjectNode node");
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
