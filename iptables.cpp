/************************************************************************
Copyright Chris Newey 2013

qiptables@hotmail.com

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

/**
\file iptables.cpp

\brief Class reads commands from SQLite database
       ruleset table and then performs each command it finds (one per line).

***************************/

#include "iptables.h"

#include "cmdline.h"
#include "rulesnippet.h"


const QString Iptables::IPTABLES_BINARY = QString("/sbin/iptables");

Iptables::Iptables(QObject *parent) :
    QObject(parent)
{

    db = new DatabaseManager(Install::INSTALL_DIR, this);
    setIptablesBinary();

    process = new IpProcess(this);
    userId = new LinuxUserId(this);
    cmdLine = new CmdLine(this);
    ruleSnippet = new RuleSnippet(this);

    connect(process, SIGNAL(cmdOutput(QString, QStringList, int, QString)),
            this,    SLOT(slotCmdOutput(QString, QStringList, int, QString)));

}

Iptables::~Iptables()
{
}


void Iptables::setCommentMark(QString commentMark)
{
    cmdLine->setCommentMark(commentMark);
}

QString Iptables::getCommentMark()
{
    QString cm = cmdLine->getCommentMark();
    return cm;
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
            IptablesBinary = "";
        }
    }
    else
    {
        qDebug("sysconf query failed\n[%s]",
               qry.lastError().text().toAscii().data());
        IptablesBinary = "";
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
        IptablesBinary = "";
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
        result = process->exec(cmd, arguments);
    }
    else
    {
        result = process->exec(cmd, arguments).
                    append("Command [%1] not executed\n%2").
                    arg(cmd).
                    arg(QString(process->readAllStandardError()));
    }

    return result;
}


QString Iptables::printCmdLine(QString cmd, QStringList argList)
{
    QString msg = QString("cmd: %1 %2").
                    arg(cmd.toAscii().data()).
                    arg(argList.join(" ").toAscii().data());

    return msg;
}


QSqlRecord Iptables::getRuleset(QString rulesetName)
{
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


bool Iptables::processRuleset(QString rulesetName)
{
    bool result = true;
    QStringList rulesetList = getRulesetRows(rulesetName);
    rulesetList = cmdLine->stripComments(rulesetList);
    rulesetList = cmdLine->stripBlankLines(rulesetList);

    // process any includes
    rulesetList = processRulesetIncludes(rulesetList);
    processStringList("processRulesetIncludes", rulesetList);
    rulesetList = cmdLine->stripComments(rulesetList);
    rulesetList = cmdLine->stripBlankLines(rulesetList);

    for (int i = 0; i < rulesetList.count(); i++)
    {
        if ("" != rulesetList.at(i).trimmed())
        {
            qDebug("Iptables::processRuleset execCmdLine(\"%s\")",
                   rulesetList.at(i).toAscii().data());
            this->process->execCmdLine(rulesetList.at(i));
        }
    }

    return result;
}


void Iptables::slotCmdOutput(QString program, QStringList arguments, int exitCode, QString result)
{
    emit cmdOutput(program, arguments, exitCode, result);
}



QStringList Iptables::processRulesetIncludes(QStringList rulesetList)
{
    QStringList result;
    QString tmp;

    for (int i = 0; i < rulesetList.count(); i++)
    {
        tmp = rulesetList.at(i);

        if (tmp.indexOf(CmdLine::defaultIncludeMark) == -1)
        {
            // not an include - just copy the string
            result.append(tmp);
        }
        else
        {
            // its an include process the snippet
            QString snippetName = tmp.remove(CmdLine::defaultIncludeMark, Qt::CaseInsensitive);
            snippetName = snippetName.trimmed();
            tmp = QString("%1 [%2] is an include - snippet name [%3]").
                            arg("Iptables::processRulesetIncludes").
                            arg(rulesetList.at(i)).
                            arg(snippetName);
            //qDebug("%s", tmp.toAscii().data());
            QStringList snippet = ruleSnippet->getRuleSnippetRows(snippetName);
            for (int i = 0; i < snippet.count(); i++)
            {
                result.append(snippet.at(i));
            }
        }
    }




    return result;
}



QString Iptables::getRulesetShortName(QString rulesetLongName)
{
    QString result = rulesetLongName;

    result = result.left(Install::IPTABLES_CHAIN_MAX_NAME_LENGTH - 2);
    result = result.toUpper();
    result = result.trimmed();
    result = result.replace(" ", "_");
    result = result.replace("-", "");
    result = result.prepend(Install::IPTABLES_CHAIN_NAME_PREFIX);

    return result;
}

void Iptables::processStringList(QString title, QStringList list)
{
    qDebug("%s", title.toAscii().data());
    for (int i = 0; i < list.count(0); i++)
    {
        qDebug("\t%s", list.at(i).toAscii().data());
    }

}
