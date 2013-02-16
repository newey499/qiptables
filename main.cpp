#include <QtGui/QApplication>
#include <QWidget>

#include "qiptablesexception.h"
#include "mainwindow.h"
#include "ipprocess.h"


/**
\file main.cpp
\brief Qiptables application entry point

\todo check for root user id disabled during development.
      Remember to enable it before release.

\todo If the ruleset is passed on the command line then don't
start the gui but run the ruleset. This will mean that the
application can be started from a shell script thus making
it possible to deploy it from the init.d start up process or
/etc/network/if-pre-up.d
***************************/


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
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
