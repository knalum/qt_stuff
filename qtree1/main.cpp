#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setStyle(QStyleFactory::create("fusion"));
    MainWindow w;
    w.setWindowTitle("qtree1");
    w.show();
    return a.exec();
}
