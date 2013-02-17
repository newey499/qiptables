#include "rulesnippet.h"

/**
\file rulesnippet.cpp

\brief RuleSnippet class processes rule snippet text from the
rulesetsnippets table.



******************/

RuleSnippet::RuleSnippet(QObject *parent) :
    QObject(parent)
{

    db = new DatabaseManager(Install::INSTALL_DIR, this);
    setIptablesBinary();
    cmdLine = new CmdLine(this);

}

RuleSnippet::~RuleSnippet()
{

}

QString RuleSnippet::getIptablesBinary()
{
    return IptablesBinary;
}

void RuleSnippet::setIptablesBinary()
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



QSqlRecord RuleSnippet::getRuleSnippet(QString ruleSnippetName)
{
    QSqlRecord rec = db->getRuleSnippetRow(ruleSnippetName);
    return rec;
}


QStringList RuleSnippet::getRuleSnippetRows(QString ruleSnippetName)
{
    QSqlRecord rec = getRuleSnippet(ruleSnippetName);
    QString rules = rec.value("snippets").toString();
    QStringList ruleSnippetList = rules.split("\n");
    return ruleSnippetList;
}

QStringList RuleSnippet::getIncludeRuleSnippetRows(QString ruleSnippetName)
{
    QSqlRecord rec = getRuleSnippet(ruleSnippetName);
    QString name  = rec.value("name").toString();
    QString rules = rec.value("snippets").toString();

    QStringList ruleSnippetList;
    QStringList snippet;

    ruleSnippetList << QString("# Begin Rule Snippet: [%1]").arg(name);

    snippet = rules.split("\n");
    for (int i = 0; i < snippet.count(); i++)
    {
        ruleSnippetList.append(snippet.at(i));
    }

    ruleSnippetList.append(QString("# End Rule Snippet: [%1]").arg(name));

    return ruleSnippetList;
}
