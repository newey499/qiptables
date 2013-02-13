#include "ipprocess.h"


const int IpProcess::PROCESS_CANNOT_BE_STARTED = -2;

IpProcess::IpProcess(QObject *parent) :
    QProcess(parent)
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    // Add an environment variable
    //env.insert("TMPDIR", "C:\\MyApp\\temp");
    // Append a value to an environment variable
    //env.insert("PATH", env.value("Path") + ";C:\\Bin");
    setProcessEnvironment(env);
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

    start(program, arguments);
    if (! waitForStarted())
        return empty;

    closeWriteChannel();

    if (! waitForFinished())
        return empty;

    //QByteArray std = readAllStandardOutput();
    //QByteArray err = readAllStandardError();
    // exit code of zero means command completed ok
    // and that output is from stdout - error output is on stderrs
    if (exitCode() == 0)
    {
        result = QString(readAllStandardOutput());
    }
    else
    {
        result = QString(readAllStandardError());
    }

    // Send the result of executing the command
    emit procCmdOutput(program, arguments, exitCode(), result);


    return QString(result);
}

QString IpProcess::printCmdLine(QString cmd, QStringList argList)
{
    qDebug("Iptables::printCmdLine(QString cmd, QStringList argList)");
    QString msg = QString("cmd: %1 %2").
                    arg(cmd.toAscii().data()).
                    arg(argList.join(" ").toAscii().data());

    qDebug("%s", msg.toAscii().data());

    return msg;
}


QString IpProcess::exec(QString cmd, QStringList argList)
{
    QString result;
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

    qDebug("cmd [%s] args [%s]",
           prog.toAscii().data(),
           args.join(" ").toAscii().data());

    return exec(prog, args);
}
