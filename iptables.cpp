#include "iptables.h"

const QString Iptables::IPTABLES_BINARY = QString("/sbin/iptables");

Iptables::Iptables(QObject *parent) :
    QObject(parent)
{
    setIptablesBinary(Iptables::IPTABLES_BINARY);
    process = new IpProcess(this);
    userId = new LinuxUserId(this);
}

Iptables::~Iptables()
{
}

QString Iptables::getIptablesBinary()
{
    return IptablesBinary;
}

void Iptables::setIptablesBinary(QString location)
{
    QFile file(location);

    if (file.exists())
    {
        IptablesBinary = location;
    }
    else
    {
        qDebug("Iptables::setIptablesBinary: location for iptables [%s] does not exist",
               location.toAscii().data());
    }
}

QString Iptables::list()
{
    QString result("");
    QStringList arguments;
    arguments << "-L";

    if (process->checkForRoot())
    {
        result = process->executeSynchronous(IptablesBinary, arguments);
    }
    else
    {
        result = process->executeSynchronous(IptablesBinary, arguments).
                    append("Command [%1] not executed\n%2").
                    arg(IptablesBinary).
                    arg(QString(process->readAllStandardError()));
    }

    return result;
}



