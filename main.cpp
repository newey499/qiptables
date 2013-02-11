#include <QtGui/QApplication>
#include "mainwindow.h"
#include "ipprocess.h"

/**
\file main.cpp
\brief Qiptables application entry point

\todo check for root user id disabled during development.
      Remember to enable it before release.


***************************/

MainWindow *globalMainWindow = 0;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    globalMainWindow = &w;
    IpProcess ip;

    /**********
    if (ip.checkForRoot())
    {
        w.show();
        return a.exec();
    }
    ******************/
    w.show();
    return a.exec();

}
