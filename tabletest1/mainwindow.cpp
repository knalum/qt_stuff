#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QUuid>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    timer->setInterval(100);

    connect(timer,&QTimer::timeout,this,[this](){
        quint64 time = QDateTime::currentSecsSinceEpoch();
        qDebug()<<QString::number(time)<<" run...";
        QString uuid = QUuid::createUuid().toString();
        this->append(uuid,QString::number(time));

        this->ui->tableView->scrollToBottom();
    });

    this->model = new QStandardItemModel(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setModel(model);

    connect(ui->tableView,&QTableView::clicked,this,[this](const QModelIndex &idx){
        qDebug()<<idx.row()<<" "<<idx.column();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::append(QString name, QString phone)
{
    QList<QStandardItem*> list;
    list.append(new QStandardItem(name));
    list.append(new QStandardItem(phone));
    model->appendRow(list);
}


void MainWindow::on_pushButton_clicked()
{
    timer->start();

}

void MainWindow::on_pushButton_2_clicked()
{
    this->append("uuid","dfdfdf");
}

void MainWindow::on_tableView_activated(const QModelIndex &index)
{
    qDebug()<<"Entered: "<<QString::number(index.row())<<" "<<QString::number(index.column());
}

void MainWindow::on_pushButton_3_clicked()
{
    timer->stop();
}
