#ifndef IPTABLES_H
#define IPTABLES_H

#include <QFile>
#include <QObject>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QStringList>
#include <QVariant>

#include "ipprocess.h"
#include "linuxuserid.h"

#include "install.h"
#include "databasemanager.h"

class IpProcess;

class Iptables : public QObject
{
    Q_OBJECT

public:

    static const QString IPTABLES_BINARY;

    explicit Iptables(QObject *parent = 0);
    ~Iptables();

    virtual QString getIptablesBinary();
    virtual void setIptablesBinary();

    virtual QString listIptablesRules();
    virtual QString printCmdLine(QString cmd, QStringList argList);


    QPointer<DatabaseManager> db;

    QSqlRecord getRuleset(QString rulesetName);
    QStringList getRulesetRows(QString rulesetName);
    QStringList stripComments(QStringList rulesetList, QString commentMark = "#");
    QString     stripComments(QString rule, QString commentMark = "#");
    QStringList stripBlankLine(QStringList rulesetList);

signals:

public slots:

protected:

    IpProcess *process;
    LinuxUserId *userId;
    QString IptablesBinary;

};

#endif // IPTABLES_H
