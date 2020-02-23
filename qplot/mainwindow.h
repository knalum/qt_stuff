#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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


    void on_actiongenRand_triggered();

    void on_actionstart_triggered();
    void updatePlot();

    void on_actionstop_triggered();

    void on_actionplotData_triggered();

    void on_actionrandTable_triggered();

private:
    Ui::MainWindow *ui;
    QVector<double> x;
    QVector<double> y;
    int startX;
    int endX;
    QTimer *timer;
    void resizeTable();
    void autoscale();
};
#endif // MAINWINDOW_H
