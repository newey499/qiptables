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


#ifndef FORMINSTALL_H
#define FORMINSTALL_H

#include <QDir>
#include <QFile>
#include <QPointer>
#include <QStringList>
#include <QTextStream>
#include <QWidget>

#include "databasemanager.h"

class DatabaseManager;

/**
 \file install.h

 \class Install

 \brief Builds application database


*/
class Install : public QObject
{
    Q_OBJECT

public:

    static const QString INSTALL_DIR;
    static const QString TOOLS_DIR;

    static const int IPTABLES_CHAIN_MAX_NAME_LENGTH;
    static const QString IPTABLES_CHAIN_NAME_PREFIX;

    explicit Install(QObject *parent = 0);
    ~Install();

    /**
    \brief Set up database

    \param forceInstall - force the database to be rebuilt
    \return String containing informative message
      *******************/
    QString performInstall(bool forceInstall = false);

    /**
    \brief Create the System Configuration table
      ******************/
    bool createSysconfTable();

    /**
    \brief Create the Ruleset table
      ******************/
    bool createRulesetTable();

    /**
    \brief Create the Rulesnippets table
      ******************/
    bool createRulesetSnippetsTable();


    /**
    \brief Populate the newly created database
      ****************/
    bool createInitialRows();

signals:

public slots:

protected:

    QDir *dir;
    QFile *file;
    DatabaseManager *dm;

    /**
    \brief Create the database

    \return true if successful created else false
      ****************/
    bool createQiptablesDatabase();

    /**
    \brief  Create directory
    \return true if successful created else false
      *******************/
    bool createDir(QString dirName);

    /**
    \brief  Create directory for qiptables database and other files
    \return true if successful created else false
      *******************/
    bool createQiptablesDir();

    /**
    \brief  Create directory for qiptables temporary files
    \return true if successful created else false
      *******************/
    bool createQiptablesTmpDir();

    /**
    \brief  Create directory for qiptables tool files
    \return true if successful created else false
      *******************/
    bool createQiptablesToolsDir();


    /**
    \brief create initial ruleset rows
    \return true if successful created else false
    *****************/
    bool createRulesetRows();

    /**
    \brief create an initial ruleset row
    \param rulesName - name of ruleset
    \param rulesList - contents of ruleset
    \return true if successful created else false
    *****************/
    bool insertRulesetRow(QString rulesName, QStringList rulesList);

    /**
    \brief create initial rulesnippet rows
    \return true if successful created else false
    *****************/
    bool createRulesetSnippetRows();

    /**
    \brief create an initial rulesnippet row
    \param snippetName - name of rulesnippet
    \param snippetList - contents of rulesnippet
    \return true if successful created else false
    *****************/
    bool insertRuleSnippetRow(QString snippetName, QStringList snippetList);

    /**
    \brief create initial system configuration table row
    \return true if successful created else false
    *****************/
    bool createSysconfRow();

    /**
    \brief Create a file providing its content as string

    \param filename - path and filename of file to be created
    \param content - string containing file contents
    \param executable - whether file is to be marked as executable

    \return name of file
      ****************/
    QString createFile(QString path, QString filename, QString content, bool executable = false);

    /**
    \brief Create a file providing its content as string list

    \param filename - path and filename of file to be created
    \param content - string containing file contents
    \param executable - whether file is to be marked as executable

    \return name of file
      ****************/
    QString createFile(QString path, QString filename, QStringList content, bool executable = false);

    /**
    \brief Create shell scripts used by qiptables

    \return always returns true
      ****************/
    bool createShellScripts();

    /**
    \brief Create bash script to clear firewall rules using iptables

    \return path and name of file created
      ***********************/
    QString createScriptClearFirewall();

    /**
    \brief Get short name of current firewall ruleset using iptables

    \return shortname of active firewall ruleset
      ******************/
    QString createScriptGetFirewallName();

private:

};

#endif // FORMINSTALL_H
