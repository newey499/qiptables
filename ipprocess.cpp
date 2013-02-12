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

/*******************************
int	IpProcess::execute (const QString &program)
{
    int result = IpProcess::PROCESS_CANNOT_BE_STARTED;

    if (! checkForRoot())
    {
        result = QProcess::execute(program);
    }

    return result;
}
********************************/


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
    if (exitCode() == 0)
    {
        result = QString(readAllStandardOutput());
    }
    else
    {
        result = QString(readAllStandardError());
    }

    /*********
    result = QString("stdout:\n%1\nstderr:\n%2\nexit code: [%3]\nexit status: [%4]\n").
                      arg(QString(readAllStandardOutput())).
                      arg(QString(readAllStandardError())).
                      arg(this->exitCode()).
                      arg(this->exitStatus());
    ************/
    return QString(result);
}

/**********************
QString IpProcess::executeSynchronous(const QString &program)
{
    QString empty("");

    printCmdLine(program);

    start(program);
    if (! waitForStarted())
        return empty;

    //write("Qt rocks!");
    closeWriteChannel();

    if (! waitForFinished())
        return empty;

    //QByteArray result = readAllStandardOutput();
    QString result = QString("%1 %2").arg(QString(readAllStandardOutput())).
                                                  arg(QString(readAllStandardError()));

    return QString(result);
}
***************************************/

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
