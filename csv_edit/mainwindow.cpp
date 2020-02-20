#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QTextStream>
#include <QDebug>
#include <QKeyEvent>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->path = QDir::homePath()+"/myData.csv";

    resizeTable();

    int ROWS = 10;
    int COLS = 2;

    ui->tableWidget->setRowCount(ROWS);
    ui->tableWidget->setColumnCount(COLS);

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableWidget->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                 SLOT(slotSelectionChange(const QItemSelection &, const QItemSelection &))
                );

    QObject::connect(ui->tableWidget, &QTableWidget::customContextMenuRequested, this, [this](){
            QMenu *menu = new QMenu;
            auto clearField = menu->addAction("Delete");
            auto removeRow = menu->addAction("Remove row");
            auto removeCol = menu->addAction("Remove column");
            QObject::connect(clearField, &QAction::triggered, this, [this, menu, clearField](){
                for(auto &item : ui->tableWidget->selectedItems())  {
                    ui->tableWidget->setItem(item->row(),item->column(),new QTableWidgetItem(""));
                }
                clearField->deleteLater();
                menu->deleteLater();
            });

            QObject::connect(removeRow, &QAction::triggered, this, [this, menu, removeRow](){
                for(auto &item : ui->tableWidget->selectedItems()){
                    ui->tableWidget->removeRow(item->row());
                }
                removeRow->deleteLater();
                menu->deleteLater();

            });

            QObject::connect(removeCol, &QAction::triggered, this, [this, menu, removeCol](){
                for(auto &item : ui->tableWidget->selectedItems()){
                    ui->tableWidget->removeColumn(item->column());
                }
                removeCol->deleteLater();
                menu->deleteLater();

                this->resizeTable();
            });

            menu->exec(QCursor::pos());
            menu->clear();
        });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotSelectionChange(const QItemSelection &a, const QItemSelection &b){
    QItemSelectionModel *selectionModel = ui->tableWidget->selectionModel();

    bool showSum = true;

    const auto selectedIndexes = selectionModel->selectedIndexes();
    int sum = 0;
    for( QModelIndex idx : selectedIndexes){
        QTableWidgetItem *item = ui->tableWidget->item(idx.row(),idx.column());

        if( item == nullptr || item->text() == nullptr) continue;

        bool ok=false;
        auto val = item->text().toInt(&ok);
        if( ok ) {
            sum += val;
        }
        else{
            showSum = false;
        }

    }
    const int nCells = selectionModel->selectedIndexes().length();

    QString message;
    if( selectedIndexes.length()==1 ){
        QString str;
        str = "(%1,%2)";
        message += str.arg(selectedIndexes.at(0).row()+1).arg(selectedIndexes.at(0).column()+1);
    }

    message += " \t "+QString::number(nCells)+" cells selected \t";
    if( showSum == true ){
        message += "Sum = "+QString::number(sum);
    }

    ui->statusbar->showMessage(message);
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if( event->key() == Qt::Key_C && event->modifiers().testFlag(Qt::ControlModifier) ){
        qDebug()<<"Copy cell";
        QList<QTableWidgetItem *> items = ui->tableWidget->selectedItems();

        const int fromX = items.at(0)->row();
        const int fromY = items.at(0)->column();

        const int toX = items.at(items.length()-1)->row();
        const int toY = items.at(items.length()-1)->column();

        qDebug()<<QString::number(fromX)+" "+QString::number(fromY)+" ... "+QString::number(toX)+" "+QString::number(toY);

        QString str;
        for(int i=fromX;i<=toX;++i){
            for(int j=fromY;j<=toY;++j){
                str += ui->tableWidget->item(i,j)->text();
                str += "\t";
            }
            str += "\n";
        }

        QClipboard *cb = QApplication::clipboard();
        cb->setText(str);
    }
}


void MainWindow::couldNotOpenFile(){
    QMessageBox::warning(this,"Warning","Could not open file");
}

void MainWindow::resizeTable(){
    QHeaderView* header = ui->tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::on_actionOpen_triggered(){
    qDebug()<<"Open file";
    QFile file(this->path);
    bool isOpen = file.open(QIODevice::ReadWrite);
    if( !isOpen ){
        return;
    }
    ui->tableWidget->clear();

    QTextStream stream(&file);
    QString str = stream.readAll();

    QStringList strRows = str.split("\n");
    int ROWS = strRows.length();
    int COLS = strRows[0].split(",").length();

    ui->tableWidget->setRowCount(ROWS);
    ui->tableWidget->setColumnCount(COLS);

    for(int row=0;row<ROWS;++row){
        qDebug()<<strRows[row];
            QStringList line = strRows[row].split(",");
            for(int col=0;col<COLS;++col){
                QString cell = line[col];
                ui->tableWidget->setItem(row,col,new QTableWidgetItem(cell));
            }
    }



    file.close();
}

void MainWindow::on_actionSave_triggered(){
    QFile file(this->path);
    if( !file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text) ){
        this->couldNotOpenFile();
        return;
    }

    QTextStream stream(&file);


    const int ROWS = ui->tableWidget->rowCount();
    const int COLS = ui->tableWidget->columnCount();
    for(int row=0;row<ROWS;++row){
        QString line = "";
        for(int col=0;col<COLS;++col){
            QTableWidgetItem *cell = ui->tableWidget->item(row,col);
            if( cell != nullptr ){
                line += ui->tableWidget->item(row,col)->text();
            }else{
                line += " ";
            }
            if( col+1<COLS ) line += ",";
        }
        stream<<line;
        if( row+1<ROWS ) stream<<"\n";
    }

    stream.flush();
    file.close();

}

void MainWindow::on_actionNew_triggered()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(2);
}

void MainWindow::on_actionExit_triggered(){
    QApplication::quit();
}

void MainWindow::on_actionAdd_Row_triggered()
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
}

void MainWindow::on_actionAdd_Column_triggered()
{
    ui->tableWidget->setColumnCount(ui->tableWidget->columnCount()+1);
}

void MainWindow::on_actionGenRand_triggered(){
    const int rows = QRandomGenerator::global()->generate()%10+1;
    const int cols = QRandomGenerator::global()->generate()%10+1;

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setColumnCount(cols);
    for(auto i=0;i<rows;++i){
        for(auto j=0;j<cols;++j){
            int val = QRandomGenerator::global()->generate()%100;
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(QString::number(val)));
        }
    }
}

void MainWindow::on_actionDelete_row_triggered(){
    for(auto item : ui->tableWidget->selectedItems()) ui->tableWidget->removeRow(item->row());

}

void MainWindow::on_actionDelete_column_triggered(){
    for(auto item : ui->tableWidget->selectedItems()) ui->tableWidget->removeColumn(item->column());
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"About","csvedit");
}
