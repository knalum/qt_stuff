#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "treemodel.h"

#include <QAbstractItemModel>
#include <QItemSelection>
#include <QMainWindow>
#include <QStandardItemModel>

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
    void on_actionNew_triggered();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_actionAbout_triggered();

    void on_actionremoveItem_triggered();

    void on_actionString_Item_triggered();

    void on_actionArray_triggered();

    void on_actionSave_triggered();

    void on_actionAdd_object_triggered();

    void on_actionNumeric_triggered();

    void on_valueField_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    TreeModel *model;
    void changed(const QItemSelection &selected, const QItemSelection &deselected);
    const QIcon folder = QIcon(":/res/img/folderIcon.png");
    const QIcon greenSphere = QIcon(":/res/img/kpiIcon.png");
    void onCustomContextMenu(const QPoint &pos);
    void addNode(QModelIndex,AbstractNode*);
    AbstractNode *copy;
};
#endif // MAINWINDOW_H
