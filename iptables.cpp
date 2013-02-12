#include "iptables.h"

const QString Iptables::IPTABLES_BINARY = QString("/sbin/iptables");

Iptables::Iptables(QObject *parent) :
    QObject(parent)
{
    db = 0;
    setIptablesBinary();
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

void Iptables::setIptablesBinary()
{
    QString location(Iptables::IPTABLES_BINARY);

    QSqlQuery qry;
    qry.prepare("select id, shell, iptables, defaultRuleName from sysconf");
    if (qry.exec())
    {
        if (qry.first())
        {
            location = qry.record().value("iptables").toString();
        }
        else
        {
            qDebug("Cannot find first record on sysconf table\n[%s]",
                   qry.lastError().text().toAscii().data());
        }
    }
    else
    {
        qDebug("sysconf query failed\n[%s]",
               qry.lastError().text().toAscii().data());
    }

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


QString Iptables::listIptablesRules()
{
    QString result("");
    QString cmd("iptables");
    QStringList arguments;
    arguments << "-L";

    printCmdLine(cmd, arguments);

    if (process->checkForRoot())
    {
        result = process->executeSynchronous(cmd, arguments);
    }
    else
    {
        result = process->executeSynchronous(cmd, arguments).
                    append("Command [%1] not executed\n%2").
                    arg(cmd).
                    arg(QString(process->readAllStandardError()));
    }

    return result;
}


QString Iptables::printCmdLine(QString cmd, QStringList argList)
{
    qDebug("Iptables::printCmdLine(QString cmd, QStringList argList)");
    QString msg = QString("cmd: %1 %2").
                    arg(cmd.toAscii().data()).
                    arg(argList.join(" ").toAscii().data());

    qDebug("%s", msg.toAscii().data());

    return msg;
}


QSqlRecord Iptables::getRuleset(QString rulesetName)
{
    if (db == 0)
    {
        db = new DatabaseManager(Install::INSTALL_DIR, this);
    }
    QSqlRecord rec = db->getRulesetRow(rulesetName);

    return rec;
}


QStringList Iptables::getRulesetRows(QString rulesetName)
{
    QSqlRecord rec = getRuleset(rulesetName);
    QString rules = rec.value("rules").toString();
    QStringList rulesList = rules.split("\n");
    return rulesList;
}

QString Iptables::stripComments(QString rule, QString commentMark)
{
    int offset = rule.indexOf(commentMark, 0, Qt::CaseSensitive);
    if (offset != -1)
    {
        rule = rule.left(offset).trimmed();
    }
    return rule;
}

QStringList Iptables::stripComments(QStringList rulesetList, QString commentMark)
{
    QStringList result;

    for (int i = 0; i < rulesetList.count(); i++)
    {
        QString tmp = rulesetList.at(i).trimmed();
        result.append(stripComments(rulesetList.at(i), commentMark));
    }

    result = stripBlankLine(result);

    return result;
}


QStringList Iptables::stripBlankLine(QStringList rulesetList)
{
    QStringList result;

    for (int i = 0; i < rulesetList.count(); i++)
    {
        if ("" != rulesetList.at(i).trimmed())
        {
            result.append(rulesetList.at(i));
        }
    }

    return result;
}
