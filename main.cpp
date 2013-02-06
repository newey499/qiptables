#include <QtGui/QApplication>
#include "mainwindow.h"

MainWindow *globalMainWindow = 0;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    globalMainWindow = &w;

    w.show();

    return a.exec();

}
