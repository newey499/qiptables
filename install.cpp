
/************************************************************************
Copyright Chris Newey 2013

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




#include "install.h"


const QString Install::INSTALL_DIR = QString("/etc/qiptables");
// Max length of iptables chain name as defined by iptables program.
const int Install::IPTABLES_CHAIN_MAX_NAME_LENGTH = 30;
// Prefix used to help identify qiptables ruleset on iptables
const QString Install::IPTABLES_CHAIN_NAME_PREFIX = QString("Q_");

Install::Install(QObject *parent) :
    QObject(parent)
{
    dir = new QDir();
    file = new QFile(this);
    dm = new DatabaseManager(Install::INSTALL_DIR, this);
}

Install::~Install()
{
    if (dir)
    {
        delete dir;
    }
    if (file)
    {
        delete file;
    }
    if (dm)
    {
        delete dm;
    }
}

QString Install::getRulesetShortName(QString rulesetLongName)
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

QString Install::performInstall(bool forceInstall)
{
    QString msg("Install::performInstall()");
    qDebug("%s", msg.toAscii().data());
    msg = "";
    if (forceInstall)
    {
        qDebug("Install forced");
    }

    if ( (! dm->databaseExists()) || forceInstall)
    {
        msg = "Performing Qiptables Install";

        createQiptablesDir();
        msg = msg.append("\n%1 - Ok").arg(Install::INSTALL_DIR);

        createQiptablesDatabase();
        msg = msg.append("\n%1 database - Ok").arg(dm->getDatabaseFileName());

        QFile dbFile(dm->getDatabaseFileName());
        dbFile.setPermissions(QFile::ReadOwner  | QFile::WriteOwner |
                              QFile::ReadGroup  | QFile::WriteGroup |
                              QFile::ReadOther  | QFile::WriteOther );
    }
    else
    {
        msg = "Install not required - not performed";
        qDebug("%s", msg.toAscii().data());
    }

    return msg;
}


bool Install::createQiptablesDir()
{
    bool result = true;
    QString tmp;

    createDir(Install::INSTALL_DIR);
    createDir(QString("%1/%2").arg(Install::INSTALL_DIR).arg("tmp"));
    createDir(QString("%1/%2").arg(Install::INSTALL_DIR).arg("tools"));

    createShellScripts();

    return result;
}


bool Install::createDir(QString dirName)
{
    bool result;

    /**********
     This looks strange - but the docs say:

    bool QDir::mkdir ( const QString & dirName ) const
    Creates a sub-directory called dirName.

    Returns true on success; otherwise returns false.

    If the directory already exists when this function is called, it will return false.
     ********************/
    if (dir->exists(dirName))
    {
        qDebug("Directory [%s] already exists", dirName.toAscii().data());
        result = false;
    }
    else
    {
        dir->mkdir(dirName);
        qDebug("Directory [%s] created OK", dirName.toAscii().data());
        result = true;
    }

    return result;

}


bool Install::createQiptablesDatabase()
{
    if (! dm->databaseExists())
    {
        qDebug("Creating database");
        dm->openDB();

        qDebug("Creating Ruleset Table");
        qDebug("%s", createRulesetTable() ? "true" : "false");
        qDebug("%s", dm->lastError().text().toAscii().data());

        qDebug("Creating Sysconf Table");
        qDebug("%s", createSysconfTable() ? "true" : "false");
        qDebug("%s", dm->lastError().text().toAscii().data());

        qDebug("Creating Ruleset Snippets Table");
        qDebug("%s", createRulesetSnippetsTable() ? "true" : "false");
        qDebug("%s", dm->lastError().text().toAscii().data());


        qDebug("Creating Initial data");
        qDebug("%s", createInitialRows() ? "true" : "false");
        qDebug("%s", dm->lastError().text().toAscii().data());

    }
    else
    {
        qDebug("database already exists");
    }

    return true;
}


QString Install::createFile(QString filename, QString content, bool executable)
{
    QString fname = QString("%1/%2/%3").
            arg(Install::INSTALL_DIR).
            arg("tools").
            arg(filename);

    QFile data(fname);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        out << content << "\n\n";
    }
    data.close();
    if (executable)
    {
        data.setPermissions(QFile::ReadOwner  |
                            QFile::WriteOwner |
                            QFile::ReadGroup  |
                            QFile::WriteGroup |
                            QFile::ReadOther  |
                            QFile::ExeOwner   |
                            QFile::ExeGroup);
    }
    else
    {
        data.setPermissions(QFile::ReadOwner  |
                            QFile::WriteOwner |
                            QFile::ReadGroup  |
                            QFile::WriteGroup |
                            QFile::ReadOther);
    }

    return fname;
}

QString Install::createFile(QString filename, QStringList content, bool executable)
{
    return createFile(filename,
                      content.join("\n"),
                      executable);
}

bool Install::createSysconfTable()
{
    bool ret = false;

    if (dm->getDb().isOpen())
    {
        QSqlQuery query;
        QString qryMsg = QString("create table %1 "
                                 "("
                                 " id integer primary key not null, "
                                 " shell	        varchar(100) not null, "
                                 " iptables         varchar(100) not null, "
                                 " defaultRuleName	varchar(100) not null, "
                                 " foreign key(defaultRuleName) references %2(name) "
                                 "         on delete restrict on update restrict "
                                 ")"
                                );
        ret = query.exec(qryMsg.arg("sysconf").arg("ruleset"));
        ret = query.exec(qryMsg.arg("sysconfdef").arg("rulesetdef"));
    }
    return ret;
}


bool Install::createSysconfRow()
{
    bool ret = false;
    if (dm->getDb().isOpen())
    {
        QSqlQuery query;
        QString qryMsg = QString(" insert into %1 "
                                 "   (shell, iptables, defaultRuleName) "
                                 " values "
                                 "   ('/bin/bash', '/sbin/iptables', 'Clean Firewall') "
                                );
        ret = query.exec(qryMsg.arg("sysconf"));
        ret = query.exec(qryMsg.arg("sysconfdef"));

    }
    return ret;
}


bool Install::createInitialRows()
{
    // Ruleset row has to be created before
    // sysconf row due to foreign key
    createRulesetRows();
    createRulesetSnippetRows();
    createSysconfRow();

    return true;
}




bool Install::createRulesetTable()
{
    bool ret = false;
    if (dm->getDb().isOpen())
    {
        QSqlQuery query;

        // NOTE the shortname column length of 30 matches the
        // maximum length of an Iptables chain name

        QString qryMsg = QString("create table %1 "
                                 "("
                                 " id integer primary key not null, "
                                 " name         varchar(100) unique not null, "
                                 " shortname    varchar(%2) unique not null, "
                                 " rules        text not null, "
                                 " foreign key(name) references %3(name) "
                                 "         on delete restrict on update restrict "
                                 ")");
        ret = query.exec(qryMsg.
                    arg("ruleset").
                    arg(Install::IPTABLES_CHAIN_MAX_NAME_LENGTH).
                    arg("sysconf"));

        if (! ret)
        {
            qDebug("%s - %s",
                "Install::createRulesetTable()",
                query.lastError().text().toAscii().data());
        }

        ret = query.exec(qryMsg.
                         arg("rulesetdef").
                         arg(Install::IPTABLES_CHAIN_MAX_NAME_LENGTH).
                         arg("sysconfdef"));
    }
    return ret;
}


bool Install::insertRulesetRow(QString rulesName, QStringList rulesList)
{
    bool ret = true;

    // always run a list of the current firewall rules
    //rulesList << "# List current firewall rules" << "iptables -L";

    if (dm->getDb().isOpen())
    {
        QString rulesText = rulesList.join("\n");
        QString shortName = getRulesetShortName(rulesName);
        qDebug("Ruleset->shortname [%s]", shortName.toAscii().data());
        QSqlQuery query;
        QString qryMsg = QString(" insert into %1 "
                                 "   (name, shortname, rules) "
                                 " values "
                                 "   ('%2', '%3', '%4') ");
        qDebug("1 qryMsg - [%s]", qryMsg.toAscii().data());

        ret = query.exec(qryMsg.
                            arg("ruleset").
                            arg(rulesName).
                            arg(shortName).
                            arg(rulesText)
                         );

        qDebug("2  qryMsg - [%s]", qryMsg.toAscii().data());

        if (! ret)
        {
            qDebug("%s %d\t%s",
                __FILE__ ,
                __LINE__ ,
                query.lastError().text().toAscii().data());
        }

        ret = query.exec(qryMsg.
                            arg("rulesetdef").
                            arg(rulesName).
                            arg(shortName).
                            arg(rulesText)
                        );
        qDebug("3 qryMsg - [%s]", qryMsg.toAscii().data());


        if (! ret)
        {
            qDebug("%s %d\t%s",
                __FILE__ ,
                __LINE__ ,
                query.lastError().text().toAscii().data());
        }

    }
    return ret;
}


bool Install::createRulesetRows()
{
    QString qryMsg;
    QString rulesName;
    QStringList rulesList;
    bool ret = false;

    if (dm->getDb().isOpen())
    {
        rulesName = "Clean Firewall - Accept everything";
        rulesList << "# Run shell script" << createScriptClearFirewall();
        insertRulesetRow(rulesName, rulesList);

        rulesName = "Home";
        rulesList.clear();
        rulesList << "# Home firewall rules - Accept everything"
                  << "# Run shell script"
                  << createScriptClearFirewall();
        insertRulesetRow(rulesName, rulesList);

        rulesName = "Office";
        rulesList.clear();
        rulesList << "# Office firewall rules";
        insertRulesetRow(rulesName, rulesList);

        rulesName = "Public location";
        rulesList.clear();
        rulesList << "# Public location firewall rules";
        insertRulesetRow(rulesName, rulesList);

    }
    return ret;
}


bool Install::createRulesetSnippetsTable()
{
    bool ret = false;
    if (dm->getDb().isOpen())
    {
        QSqlQuery query;
        QString qryMsg = QString("create table %1 "
                                 "("
                                 " id integer primary key not null, "
                                 " name     varchar(100) unique not null, "
                                 " snippets	text not null "
                                 ")");
        ret = query.exec(qryMsg.arg("rulesetsnippets"));
        ret = query.exec(qryMsg.arg("rulesetsnippetsdef"));
    }
    return ret;
}


bool Install::insertRuleSnippetRow(QString snippetName, QStringList snippetList)
{
    QString snippetText = snippetList.join("\n");
    QSqlQuery query;
    QString qryMsg;
    bool ret = true;
    qDebug("Install::insertRuleSnippetRow(QString snippetName, QStringList snippetList)");

    if (dm->getDb().isOpen())
    {
        qDebug("Install::insertRuleSnippetRow -- Database is open");

        snippetText = snippetList.join("\n");
        qryMsg = QString(" insert into %1 "
                         "   (name, snippets) "
                         " values "
                         "   ('%2', '%3') ");
        ret = query.exec(qryMsg.
                            arg("rulesetsnippets").
                            arg(snippetName).
                            arg(snippetText)
                         );
        if (! ret)
        {
            qDebug("%s %d\t%s",
                __FILE__ ,
                __LINE__ ,
                query.lastError().text().toAscii().data());
        }
        ret = query.exec(qryMsg.
                            arg("rulesetsnippetsdef").
                            arg(snippetName).
                            arg(snippetText)
                        );
        if (! ret)
        {
            qDebug("%s %d\t%s",
                __FILE__ ,
                __LINE__ ,
                query.lastError().text().toAscii().data());
        }

    }
    return ret;
}

bool Install::createRulesetSnippetRows()
{
    bool ret = false;
    QString snippetName = "Snippet 1";
    QStringList snippetList;

    if (dm->getDb().isOpen())
    {
        snippetName = "Snippet 1";
        snippetList.clear();
        snippetList << "# Snippet 1 iptables statements";
        insertRuleSnippetRow(snippetName, snippetList);

        snippetName = "Snippet 2";
        snippetList.clear();
        snippetList << "# Snippet 2 iptables statements";
        insertRuleSnippetRow(snippetName, snippetList);

        snippetName = "Snippet 3";
        snippetList.clear();
        snippetList << "# Snippet 3 iptables statements";
        insertRuleSnippetRow(snippetName, snippetList);

        snippetName = "Snippet 4";
        snippetList.clear();
        snippetList << "# Snippet 4 iptables statements";
        insertRuleSnippetRow(snippetName, snippetList);

    }
    return ret;
}

bool Install::createShellScripts()
{
    bool result = true;

    createScriptClearFirewall();
    createScriptGetFirewallName();

    return result;
}

QString Install::createScriptClearFirewall()
{
    QStringList script;
    QString filename = "clearFirewall.sh";

    script  << "#!/bin/bash"
            << "################################################"
            << "#"
            << QString("# ").append(filename)
            << "#"
            << "# created by Qiptables install program"
            << "#"
            << "# referenced from ruleset table of Sqlite database"
            << "#"
            << "#"
            << "#"
            << "#"
            << "################################################"
            << "echo \"Stopping firewall and allowing everyone...\""
            << "iptables -F"
            << "iptables -X"
            << "iptables -t nat -F"
            << "iptables -t nat -X"
            << "iptables -t mangle -F"
            << "iptables -t mangle -X"
            << "iptables -P INPUT ACCEPT"
            << "iptables -P FORWARD ACCEPT"
            << "iptables -P OUTPUT ACCEPT"
            << " "
            << " ";

    // Create the file
    filename = createFile(filename, script, true);

    return filename;
}


QString Install::createScriptGetFirewallName()
{
    QStringList script;
    QString filename = "get-firewall-name.sh";

    script << "#!/bin/bash"
           << "#!/bin/bash "
           << "#####################"
           << "#"
           << "# firewall-name.sh"
           << "#"
           << "# reports name of current firewall using the qiptables convention"
           << "# of using a chain of zero references whose name is prefixed with"
           << "# \"Q_\" to match the shortname column of the ruleset table."
           << "#"
           << "#####################"
           << ""
           << "ZERO_REFERENCES=\"(0 references)\""
           << "Q_IPTABLES_PREFIX=\"Q_\""
           << ""
           << "iptables -L | grep \"$Q_IPTABLES_PREFIX\" | awk '{ print $2; }'";


    // Create the file
    filename = createFile(filename, script, true);

    return filename;
}

