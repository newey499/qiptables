

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


#ifndef FORMINSTALL_H
#define FORMINSTALL_H

#include <QDir>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QWidget>

#include "databasemanager.h"

class DatabaseManager;

class Install : public QObject
{
    Q_OBJECT

public:

    static const QString INSTALL_DIR;
    static const int IPTABLES_CHAIN_MAX_NAME_LENGTH;
    static const QString IPTABLES_CHAIN_NAME_PREFIX;

    explicit Install(QObject *parent = 0);
    ~Install();

    QString performInstall(bool forceInstall = false);

    bool createSysconfTable();
    bool createRulesetTable();
    bool createRulesetSnippetsTable();

    QString getRulesetShortName(QString rulesetLongName);

    bool createInitialRows();

signals:

public slots:

protected:

    QDir *dir;
    QFile *file;
    DatabaseManager *dm;

    bool createQiptablesDatabase();

    bool createDir(QString dirName);
    bool createQiptablesDir();
    bool createQiptablesTmpDir();
    bool createQiptablesToolsDir();



    bool createRulesetRows();
    bool insertRulesetRow(QString rulesName, QStringList rulesList);
    bool createRulesetSnippetRows();
    bool insertRuleSnippetRow(QString snippetName, QStringList snippetList);
    bool createSysconfRow();

    QString createFile(QString filename, QString content, bool executable = false);
    QString createFile(QString filename, QStringList content, bool executable = false);

    bool createShellScripts();
    QString createScriptClearFirewall();
    QString createScriptGetFirewallName();

private:

};

#endif // FORMINSTALL_H
