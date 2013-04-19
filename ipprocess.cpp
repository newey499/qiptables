
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



#include "ipprocess.h"


const int IpProcess::PROCESS_CANNOT_BE_STARTED = -2;

IpProcess::IpProcess(QObject *parent) :
    QProcess(parent)
{
    errStr = "";
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    // Add an environment variable
    //env.insert("TMPDIR", "C:\\MyApp\\temp");
    // Append a value to an environment variable
    //env.insert("PATH", env.value("Path") + ";C:\\Bin");
    setProcessEnvironment(env);

    connect(this, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(slotError(QProcess::ProcessError)) );
}


QProcessEnvironment IpProcess::systemEnvironment()
{
    return QProcessEnvironment::systemEnvironment();
}


bool IpProcess::checkForRoot(QString title)
{
    bool result = true;

    QString exit = QString("Sorry, %1 ").arg(title);
    exit = exit.append("may only be run as root.\n");
    exit = exit.append("This is because iptables requires root access to\n");
    exit = exit.append("make changes to firewall rules.");

    LinuxUserId *uid = new LinuxUserId(this);

    if (! uid->isUserRoot())
    {

        QMessageBox::critical(0, tr(title.toAscii().data()),
                                   tr(exit.toAscii().data()),
                                   QMessageBox::Ok,
                                   QMessageBox::Ok);
        result = false;

    }

    return result;
}


QString	IpProcess::execute (const QString &program, const QStringList &arguments)
{
    QString result;

    if (! checkForRoot())
    {
        result = executeSynchronous(program, arguments);
    }

    return result;
}

QString IpProcess::executeSynchronous(const QString &program, const QStringList &arguments)
{
    QString empty("");
    QString result("");

    printCmdLine(program, arguments);

    if (arguments.count() > 0)
    {
        start(program, arguments);
    }
    else
    {
        start(program);
    }

    if (! waitForStarted())
        return empty;

    closeWriteChannel();

    if (! waitForFinished())
        return empty;

    //qDebug("IpProcess::executeSynchronous");
    //QByteArray std = readAllStandardOutput();
    //QByteArray err = readAllStandardError();
    // exit code of zero means command completed ok
    // and that output is from stdout - error output is on stderrs
    if ( (exitCode() == 0) && (errStr.isEmpty()) )
    {
        result = QString(QString(readAllStandardOutput()));
    }
    else
    {
        result = QString(QString(readAllStandardError()));
    }

    if (result.isEmpty())
    {
        // result = parseErrorCode();
    }

    // Send the result of executing the command
    emit cmdOutput(program, arguments, exitCode(), result);

    /***********
    qDebug("IpProcess::executeSynchronous\n"
           "cmd [%s] exit code [%d]\n"
           "result %s",
           program.toAscii().data(),
           exitCode(),
           result.toAscii().data() );
    ********************/
    return QString(result);
}

QString IpProcess::printCmdLine(QString cmd, QStringList argList)
{

    //qDebug("IpProcess::printCmdLine(QString cmd, QStringList argList)");
    QString msg = QString("cmd: %1 %2").
                    arg(cmd.toAscii().data()).
                    arg(argList.join(" ").toAscii().data());

    //qDebug("%s", msg.toAscii().data());

    return msg;
}


QString IpProcess::exec(QString cmd, QStringList argList)
{
    QString result;

    qDebug("Executing cmd [%s] args [%s]",
           cmd.toAscii().data(),
           argList.join(" ").toAscii().data());
    result = executeSynchronous(cmd, argList);
    return result;
}


QString IpProcess::execCmdLine(QString cmd)
{
    QString prog("");
    QStringList args;

    args = cmd.split(" ", QString::SkipEmptyParts);

    if (! args.isEmpty())
    {
        prog = args.takeFirst();
    }

    return exec(prog, args);
}

void IpProcess::slotError(QProcess::ProcessError error)
{
    procError = error;
    QString tmp = parseErrorCode();
    //qDebug("%s", tmp.toAscii().data());
}


QString IpProcess::parseErrorCode()
{
    QString result("");

    switch (procError)
    {
        case QProcess::FailedToStart :
             result = "0	The process failed to start. "
                      "Either the invoked program is missing, "
                      "or you may have insufficient permissions to invoke the program.";
             break;

        case QProcess::Crashed :
             result = "1	The process crashed some time after starting successfully.";
             break;

        case QProcess::Timedout :
             result = "2	The last waitFor...() function timed out. "
                      "The state of QProcess is unchanged, and you can "
                      "try calling waitFor...() again.";
             break;

        case QProcess::WriteError :
             result = "4	An error occurred when attempting to write to "
                      "the process. For example, the process may not be running, "
                      "or it may have closed its input channel.";
             break;

        case QProcess::ReadError :
             result = "3	An error occurred when attempting to read from the "
                      "process. For example, the process may not be running.";
             break;

        case QProcess::UnknownError :
             result = "5	An unknown error occurred. This is the default return "
                      "value of error().";
              break;

        default :
              result = "";
              break;
    }

    errStr = result;

    return errStr;
}
