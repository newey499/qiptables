/************************************************************************
Copyright Chris Newey 2013

qiptables@hotmail.com

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


#ifndef IPPROCESS_H
#define IPPROCESS_H

#include <QMessageBox>
#include <QPointer>
#include <QProcess>
#include <QProcessEnvironment>

//#include "mainwindow.h"
#include "linuxuserid.h"

/**
 \file ipprocess.h

 \class IpProcess

 \brief Executes commands read from database


*/
class IpProcess : public QProcess
{
    Q_OBJECT
public:

    static const int PROCESS_CANNOT_BE_STARTED;

    /**
    \brief Get a copy of the system environment
    \return aQProcessEnvironment object
      *************************/
    static QProcessEnvironment systemEnvironment();

    explicit IpProcess(QObject *parent = 0);

    /**
    \brief check to see if application is running as root

    If application is not being run with root permissions an alert
    box is popped up.

    \param title - used as title for alert box

    \return true if running with root permissions else false
      ********************************/
    virtual bool checkForRoot(QString title = "Application Name");

    /**
    \brief exec a command with arguments to the command passed in argList parameter

    \param cmd - The command to execute
    \param argList - List of arguments to parameter cmd

    \return output from executed command
      *****************/
    virtual QString exec(QString cmd, QStringList argList);

    /**
    \brief exec the command line

    The command is parsed to construct a list of any arguments and then the exec()
    method of this class is called.

    \arg cmd - a command to exec together with any arguments

    \return output from executed command
      *********************/
    virtual QString execCmdLine(QString cmd);

protected:

    /**
    \brief exec <program> passing <arguments>

    \param program - name of program to execute
    \param arguments - list of arguments to program which is to be executed

    \return output of command
      **************/
    virtual QString	execute (const QString &program, const QStringList &arguments = QStringList());

    /**
    \brief exec <program> passing <arguments>

    The output of the command is propogated from this method by the
    <cmdOutput> signal.

    \param program - name of program to execute
    \param arguments - list of arguments to program which is to be executed

    \return output of command
      **************/
    virtual QString executeSynchronous(const QString &program, const QStringList &arguments = QStringList());

    /**
    \brief Produces a command line string by concatanating the cmd and arglist

    \param cmd - name of command
    \param argList - list of argumants to command

    \return command and arguments in one string
      ***************************/
    virtual QString printCmdLine(QString cmd, QStringList argList = QStringList());

signals:

    /**
    \brief propogates result of executing command

    The signal gives the following information.
    1) program name
    2) program arguments
    3) exit code of program
    4) program output
      ******************/
    void cmdOutput(QString, QStringList, int, QString);

public slots:


protected slots:

    /**
    \brief obtains string explaining error code then does nothing with it

    This method is redundant and will be removed.
      **********************/
    void slotError(QProcess::ProcessError error);

protected:

    QString errStr;
    ProcessError procError;

    /**
    \brief Obtains string explaining error code

    \return string containing explanation of error code
      **********************/
    virtual QString parseErrorCode();

};

#endif // IPPROCESS_H
