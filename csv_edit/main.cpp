#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/res/res/icon.png"));
    a.setStyle(QStyleFactory::create("fusion"));
    MainWindow w;
    w.setWindowTitle("csvtool");
    w.show();
    return a.exec();
}
