#ifndef IPTABLES_H
#define IPTABLES_H

#include <QFile>
#include <QObject>

#include "ipprocess.h"
#include "linuxuserid.h"

class IpProcess;

class Iptables : public QObject
{
    Q_OBJECT

public:

    static const QString IPTABLES_BINARY;

    explicit Iptables(QObject *parent = 0);
    ~Iptables();

    QString getIptablesBinary();
    void setIptablesBinary(QString location);

    QString list();

signals:
    
public slots:

protected:

    IpProcess *process;
    LinuxUserId *userId;
    QString IptablesBinary;
    
};

#endif // IPTABLES_H
