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

    QPointer<DatabaseManager> db;

    explicit Iptables(QObject *parent = 0);
    ~Iptables();

    virtual QString getIptablesBinary();
    virtual void setIptablesBinary();

    virtual QString listIptablesRules();
    virtual QString printCmdLine(QString cmd, QStringList argList);

    /* ****************************
     Methods to process a ruleset
    ********************************/

    // Get the required SQLRecord containing the ruleset
    virtual QSqlRecord getRuleset(QString rulesetName);

    // Strip any comments from the ruleset
    virtual QStringList stripComments(QStringList rulesetList, QString commentMark = "#");
    virtual QString     stripComments(QString rule, QString commentMark = "#");

    // Strip any blank lines from the ruleset
    virtual QStringList stripBlankLines(QStringList rulesetList);

    // Get the ruleset as a QStringList off the ruleset table using the ruleset name
    virtual QStringList getRulesetRows(QString rulesetName);


    // Execute the commands in the ruleset
    virtual bool    processRuleset(QString rulesetName);

signals:

    void cmdOutput(QString program, QStringList arguments, int exitCode, QString result);

public slots:


protected slots:

    void slotCmdOutput(QString program, QStringList arguments, int exitCode, QString result);

protected:

    QPointer<IpProcess> process;
    QPointer<LinuxUserId> userId;
    QString IptablesBinary;

};

#endif // IPTABLES_H
