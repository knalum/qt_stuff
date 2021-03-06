#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "abstractnode.h"

#include <QMainWindow>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QItemSelection>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

private:
    Ui::MainWindow *ui;
    AbstractNode *copy;

};
#endif // MAINWINDOW_H
