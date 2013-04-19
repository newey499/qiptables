

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


#ifndef IPPROCESS_H
#define IPPROCESS_H

#include <QMessageBox>
#include <QPointer>
#include <QProcess>
#include <QProcessEnvironment>

//#include "mainwindow.h"
#include "linuxuserid.h"

class IpProcess : public QProcess
{
    Q_OBJECT
public:

    static const int PROCESS_CANNOT_BE_STARTED;

    static QProcessEnvironment systemEnvironment();

    explicit IpProcess(QObject *parent = 0);

    virtual bool checkForRoot(QString title = "Application Name");

    virtual QString exec(QString cmd, QStringList argList);
    virtual QString execCmdLine(QString cmd);

protected:

    virtual QString	execute (const QString &program, const QStringList &arguments = QStringList());
    virtual QString executeSynchronous(const QString &program, const QStringList &arguments = QStringList());
    virtual QString printCmdLine(QString cmd, QStringList argList = QStringList());

signals:

    void cmdOutput(QString, QStringList, int, QString);

public slots:


protected slots:

    void slotError(QProcess::ProcessError error);

protected:

    QString errStr;
    ProcessError procError;

    virtual QString parseErrorCode();

};

#endif // IPPROCESS_H
