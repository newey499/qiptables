

/************************************************************************
Copyright Chris Newey 2013

This file is part of Qiptables.

Qiptables is distributed under the terms of the GNU General Public License

Qiptables is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Qiptables is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Qiptables.  If not, see <http://www.gnu.org/licenses/>.

***************************************************************************/



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
