#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

#include <QRandomGenerator>
#include <QDebug>

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
    ui->setupUi(this);

    ui->graphicsView->addGraph();
    ui->graphicsView->graph(0)->setData(x,y);
    ui->graphicsView->xAxis->setRange(0,100);
    ui->graphicsView->yAxis->setRange(-20,20);


    this->on_actiongenRand_triggered();
    startX=0;
    endX=100;

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(20);

    for(int i=0;i<20;++i){
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(i)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(i*2)));
    }
    this->resizeTable();

    QList<int> sizes{3,7};
    ui->splitter->setSizes(sizes);
    ui->graphicsView->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
}

void MainWindow::resizeTable(){
    QHeaderView* header = ui->tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::autoscale(){
    int minY=0,maxY=10;
    for(int i=0;i<x.size();++i){
        if( this->y[i]<minY ) minY=y[i];
        if( this->y[i]>maxY ) maxY=y[i];
    }

    int minX=x.first(),maxX=x.last();
    ui->graphicsView->yAxis->setRange(minY,maxY);
    ui->graphicsView->xAxis->setRange(minX,maxX);

    ui->graphicsView->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actiongenRand_triggered(){
    x.clear();
    y.clear();

    auto val = 0;
    for(int i=0;i<100;++i){
        this->x.append(i);
        val = QRandomGenerator::global()->generate()%10;
        this->y.append(val);
    }

    ui->graphicsView->graph(0)->setData(x,y);
    ui->graphicsView->replot();
}

void MainWindow::on_actionstart_triggered()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePlot()));
    this->timer->start(20);
}

void MainWindow::updatePlot(){
    auto val = QRandomGenerator::global()->generate()%10;
    x.append(x.length());
    y.append(val);


    ui->graphicsView->xAxis->setRange(startX,endX);
    ++startX;
    ++endX;

    ui->graphicsView->graph(0)->setData(x,y);
    ui->graphicsView->replot();
}

void MainWindow::on_actionstop_triggered(){
    this->timer->stop();
}

void MainWindow::on_actionplotData_triggered()
{
    const int ROWS = ui->tableWidget->rowCount();
    const int COLS = ui->tableWidget->columnCount();

    this->x.clear();
    this->y.clear();
    for(int i=0;i<ROWS;++i){
       auto x = ui->tableWidget->item(i,0)->text().toDouble();
       auto y = ui->tableWidget->item(i,1)->text().toDouble();

       this->x.append(x);
       this->y.append(y);
    }
    ui->graphicsView->graph(0)->setData(this->x,this->y);
    this->autoscale();
}

void MainWindow::on_actionrandTable_triggered(){
    int newRows = 0;
    auto nRowsText = ui->nRowsField->toPlainText();
    if( nRowsText.isNull() || nRowsText.isEmpty() ){
        // Field is blank or invalid
        newRows = 100*(QRandomGenerator::global()->generate()%10)+1; //[1,1000]
    }else{
        // Use field
        newRows = nRowsText.toInt();
    }

    this->x.clear();
    this->y.clear();
    ui->tableWidget->setRowCount(newRows);
    auto val = 0;
    for(int i=0;i<newRows;++i){
        val += 10*(QRandomGenerator::global()->generate()%10-5);
        this->x.append(i);
        this->y.append(val);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(i)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(val)));
    }

    ui->graphicsView->graph(0)->setData(this->x,this->y);
    this->autoscale();
}
