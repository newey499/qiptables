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
 \file install.cpp

 \brief Builds application database

*/


#include "install.h"
#include "genlib.h"

const QString Install::INSTALL_DIR = QString("/etc/qiptables");
const QString Install::TOOLS_DIR = QString("/etc/qiptables/tools");

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



QString Install::performInstall(bool forceInstall)
{
    QString msg("");

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


QString Install::createFile(QString path, QString filename, QString content, bool executable)
{
    QString fname = QString("%1/%2").
            arg(path).
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

// calls createFile above by passing the file content as a QString
QString Install::createFile(QString path, QString filename, QStringList content, bool executable)
{
    return createFile(path,
                      filename,
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
                                 " tempdir          varchar(100) not null, "
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
                                 "   (shell, iptables, defaultRuleName, "
                                 "    tempdir ) "
                                 " values "
                                 "   ('/bin/bash', '/sbin/iptables', "
                                 "    'Clean Firewall - Accept everything', '/tmp' ) "
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
        QString shortName = GenLib::getRulesetShortName(rulesName);
        QSqlQuery query;
        QString qryMsg = QString(" insert into %1 "
                                 "   (name, shortname, rules) "
                                 " values "
                                 "   ('%2', '%3', '%4') ");

        ret = query.exec(qryMsg.
                            arg("ruleset").
                            arg(rulesName).
                            arg(shortName).
                            arg(rulesText)
                         );

        if (! ret)
        {
            qDebug("%s %d: Query failed [%s]",
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


        if (! ret)
        {
            qDebug("%s %d: Query failed [%s]",
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
        rulesName = "Test";
        rulesList << "# Test";
        insertRulesetRow(rulesName, rulesList);


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
        rulesList << "# Public location firewall rules" <<
                     "# Clear Firewall rules with Shell script" <<
                     "# Run shell script" <<
                     "/etc/qiptables/tools/clearFirewall.sh" <<
                     "# Refuse any attempt to initiate an incoming connection" <<
                     "iptables -P INPUT DROP" <<
                     "# Forward nothing" <<
                     "iptables -P FORWARD DROP" <<
                     "# Allow anything going out" <<
                     "iptables -P OUTPUT ACCEPT" <<
                     "# All all established connections" <<
                     "iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT";
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

    if (dm->getDb().isOpen())
    {
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
            qDebug("%s %d: Query failed [%s]",
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

        snippetName = "Test Snippet";
        snippetList.clear();
        snippetList << "# Test Snippet";
        insertRuleSnippetRow(snippetName, snippetList);

        snippetName = "Clear Firewall rules with Shell script";
        snippetList.clear();
        snippetList << "# Clear Firewall rules with Shell script" <<
                       "# Run shell script" <<
                       "/etc/qiptables/tools/clearFirewall.sh";
        insertRuleSnippetRow(snippetName, snippetList);


        snippetName = "Clear Firewall Rules";
        snippetList.clear();
        snippetList << "# Clear Firewall Rules" <<
                       "# Does the same as \"Clear Firewall rules with Shell script\"" <<
                       "# without calling a shell script" <<
                       "iptables -F" <<
                       "iptables -X" <<
                       "iptables -t nat -F" <<
                       "iptables -t nat -X" <<
                       "iptables -t mangle -F" <<
                       "iptables -t mangle -X" <<
                       "iptables -P INPUT ACCEPT" <<
                       "iptables -P FORWARD ACCEPT" <<
                       "iptables -P OUTPUT ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);

        snippetName = "Default Policy Accept";
        snippetList.clear();
        snippetList << "# Default Policy Accept" <<
                       "iptables -P INPUT ACCEPT" <<
                       "iptables -P FORWARD ACCEPT" <<
                       "iptables -P OUTPUT ACCEPT" <<
                       "# always allow local loopback" <<
                       "iptables -I INPUT -i lo -j ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);


        snippetName = "Default Policy Drop";
        snippetList.clear();
        snippetList << "# Default Policy Drop" <<
                       "iptables -P INPUT DROP" <<
                       "iptables -P FORWARD DROP" <<
                       "iptables -P OUTPUT DROP" <<
                       "# always allow local loopback" <<
                       "iptables -I INPUT -i lo -j ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);

        snippetName = "Policy Drop but accept outgoing";
        snippetList.clear();
        snippetList << "# Policy Drop but accept outgoing" <<
                       "iptables -P INPUT DROP" <<
                       "iptables -P FORWARD DROP" <<
                       "iptables -P OUTPUT ACCEPT" <<
                       "# always allow local loopback" <<
                       "iptables -I INPUT -i lo -j ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);


        snippetName = "Accept Everything from 192.168.1.0/24";
        snippetList.clear();
        snippetList << "# "
                    << "Accept Everything from 192.168.1.0/24"
                    << "iptables -A INPUT -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT"
                    << "iptables -A INPUT -s 192.168.1.0/24 -m state --state NEW -p tcp -j ACCEPT"
                    << "iptables -A INPUT -s 192.168.1.0/24 -m state --state NEW -p udp -j ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);


        snippetName = "Accept Established Related";
        snippetList.clear();
        snippetList << "# Accept all previously established connections" <<
                       "iptables -A INPUT -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);

        snippetName = "Hide Firewall";
        snippetList.clear();
        snippetList << "# Hide presence of firewall by returning specific codes" <<
                       "iptables -A INPUT -p tcp -j REJECT --reject-with tcp-reset" <<
                       "iptables -A INPUT -p udp -j REJECT --reject-with icmp-port-unreachable";
        insertRuleSnippetRow(snippetName, snippetList);


        snippetName = "Accept Incoming HTTP";
        snippetList.clear();
        snippetList << "# Accept Incoming HTTP" <<
                       "iptables -A INPUT -m state --state NEW -p tcp --dport 80 -j ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);

        snippetName = "Accept Incoming HTTPS";
        snippetList.clear();
        snippetList << "# Accept Incoming HTTPS" <<
                       "iptables -A INPUT -m state --state NEW -p tcp --dport 443 -j ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);


        snippetName = "Accept Incoming SSH";
        snippetList.clear();
        snippetList << "# Accept Incoming SSH" <<
                       "iptables -A INPUT -p tcp --dport 22 -j ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);


        snippetName = "Accept Incoming FTP";
        snippetList.clear();
        snippetList << "# Accept Incoming FTP" <<
                       "iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 20 -j ACCEPT" <<
                       "iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 21 -j ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);


        snippetName = "Permit Windows file sharing";
        snippetList.clear();
        snippetList << "# Accept Windows file sharing" <<
                       "iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 137:139 -j ACCEPT" <<
                       "iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 426 -j ACCEPT" <<
                       "iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 445 -j ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);


        snippetName = "Accept Incoming Samba";
        snippetList.clear();
        snippetList << "# Accept Incoming Samba" <<
                       "# NOTE: 192.168.1.0/24 is a typical home network IP" <<
                       "# This IP may have to be changed for your network" <<
                       "# run the program \"ifconfig\" to find your home network settings." <<
                       "iptables -A INPUT -p udp -m udp -s 192.168.0.0/24 --dport 137 -j ACCEPT" <<
                       "iptables -A INPUT -p udp -m udp -s 192.168.0.0/24 --dport 138 -j ACCEPT" <<
                       "iptables -A INPUT -m state --state NEW -m tcp -p tcp -s 192.168.0.0/24 --dport 139 -j ACCEPT" <<
                       "iptables -A INPUT -m state --state NEW -m tcp -p tcp -s 192.168.0.0/24 --dport 445 -j ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);


        snippetName = "Permit five ports for Bitorrent";
        snippetList.clear();
        snippetList << "# Permit five ports for Bitorrent" <<
                       "iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 6881:6886 -j ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);

        snippetName = "Allow IRC";
        snippetList.clear();
        snippetList << "# Allow IRC" <<
                       "iptables -A INPUT -p tcp --syn --destination-port 6667 -j ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);


        snippetName = "Allow ping";
        snippetList.clear();
        snippetList << "# Allow ping" <<
                       "iptables -A INPUT -p icmp --icmp-type echo-request -j ACCEPT";
        insertRuleSnippetRow(snippetName, snippetList);


        snippetName = "Allow certain critical ICMP types";
        snippetList.clear();
        snippetList << "# Allow certain critical ICMP types" <<
                       "iptables -A INPUT -p icmp --icmp-type destination-unreachable -j ACCEPT # Dest unreachable" <<
                       "iptables -A INPUT -p icmp --icmp-type time-exceeded -j ACCEPT # Time exceeded" <<
                       "iptables -A INPUT -p icmp --icmp-type parameter-problem -j ACCEPT # Parameter Problem";
        insertRuleSnippetRow(snippetName, snippetList);


        snippetName = "Accept Incoming VNC";
        snippetList.clear();
        snippetList << "# Accept Incoming VNC" <<
                       "# A VNC server listens for a VNC client on " <<
                       "# TCP ports 5800+N, 5900+N, and 6000+N where" <<
                       "# N is the display which starts at zero." <<
                       "# 5800+N - Java-based vncviewer" <<
                       "# 5900+N - VNC Client Port" <<
                       "# 6000+N - X Server port" <<
                       "# VNC Display 1" <<
                       "# 5800+N - Java-based vncviewer" <<
                       "iptables -A INPUT -m state --state NEW -m tcp -p tcp --dport 5801  -j ACCEPT" <<
                       "# 5900+N - VNC Client Port" <<
                       "iptables -A INPUT -m state --state NEW -m tcp -p tcp --dport 5901  -j ACCEPT" <<
                       "# 6000+N - X Server port" <<
                       "iptables -A INPUT -m state --state NEW -m tcp -p tcp --dport 6001  -j ACCEPT";
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
    filename = createFile(Install::TOOLS_DIR,  filename, script, true);

    return filename;
}


QString Install::createScriptGetFirewallName()
{
    QStringList script;
    QString filename = "get-firewall-name.sh";

    script << "#!/bin/bash"
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
    filename = createFile(Install::TOOLS_DIR, filename, script, true);

    return filename;
}

