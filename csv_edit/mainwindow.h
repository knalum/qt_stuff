#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelection>
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

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionNew_triggered();

    void on_actionExit_triggered();

    void on_actionAdd_Row_triggered();

    void on_actionAdd_Column_triggered();

    void on_actionGenRand_triggered();

    void on_actionDelete_row_triggered();

    void on_actionDelete_column_triggered();

    void slotSelectionChange(const QItemSelection &, const QItemSelection &);
    void on_actionAbout_triggered();

protected:
    void keyPressEvent(QKeyEvent* event);

private:
    Ui::MainWindow *ui;
    QString path;
    void couldNotOpenFile();
    void resizeTable();
};
#endif // MAINWINDOW_H
