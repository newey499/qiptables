#include "ipprocess.h"


extern MainWindow *globalMainWindow;

const int IpProcess::PROCESS_CANNOT_BE_STARTED = -2;

IpProcess::IpProcess(QObject *parent) :
    QProcess(parent)
{
}


QProcessEnvironment IpProcess::systemEnvironment()
{
    return QProcessEnvironment::systemEnvironment();
}


bool IpProcess::checkForRoot()
{
    bool result = true;

    QString exit = QString("Sorry, %1 ").arg(globalMainWindow->application);
    exit = exit.append("may only be run as root.\n");
    exit = exit.append("This is because iptables requires root access to\n");
    exit = exit.append("make changes to firewall rules.");

    LinuxUserId *uid = new LinuxUserId(this);

    if (! uid->isUserRoot())
    {

        QMessageBox::critical(0, tr(globalMainWindow->application.toAscii().data()),
                                   tr(exit.toAscii().data()),
                                   QMessageBox::Ok,
                                   QMessageBox::Ok);
        result = false;

    }

    return result;
}


int	IpProcess::execute (const QString &program, const QStringList &arguments)
{
    int result = IpProcess::PROCESS_CANNOT_BE_STARTED;

    if (! checkForRoot())
    {
        result = QProcess::execute(program, arguments);
    }

    return result;
}


int	IpProcess::execute (const QString &program)
{
    int result = IpProcess::PROCESS_CANNOT_BE_STARTED;

    if (! checkForRoot())
    {
        result = QProcess::execute(program);
    }

    return result;
}


QString IpProcess::executeSynchronous(const QString &program, const QStringList &arguments)
{
    QString empty("");

    start(program, arguments);
    if (! waitForStarted())
        return empty;

    //write("Qt rocks!");
    //closeWriteChannel();

    if (! waitForFinished())
        return empty;

    //QByteArray std = readAllStandardOutput();
    //QByteArray err = readAllStandardError();
    QString result = QString("%1 %2").arg(QString(readAllStandardOutput())).
                                                  arg(QString(readAllStandardError()));

    return QString(result);
}


QString IpProcess::executeSynchronous(const QString &program)
{
    QString empty("");

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
