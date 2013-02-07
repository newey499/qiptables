#include <QtGui/QApplication>
#include "mainwindow.h"
#include "ipprocess.h"

MainWindow *globalMainWindow = 0;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    globalMainWindow = &w;
    IpProcess ip;

    if (ip.checkForRoot())
    {
        w.show();
        return a.exec();
    }

}
