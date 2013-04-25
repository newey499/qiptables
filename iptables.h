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
class CmdLine;
class RuleSnippet;


/**
\file iptables.h
\class Iptables

\brief Class reads commands from SQLite database
       ruleset table and then performs each command it finds (one per line).

This class "knows" about line comments in the style of bash "#" comments.
The comment identifier may be changed by using set/get functions but only
line comments are recognised. Block  comments are not although they may
be supported in the future.

\todo Add support for block comments

***************************/
class Iptables : public QObject
{
    Q_OBJECT

public:

    static const QString IPTABLES_BINARY;

    QPointer<DatabaseManager> db;

    explicit Iptables(QObject *parent = 0);
    ~Iptables();

    /**
    \brief get a ruleset shortname from the full name of the ruleset

    \param rulesetLongName - The full name of the ruleset

    \return the short name of the ruleset which is used to create an empty
            iptables chain which identifies which ruleset is in use

      ****************************/
    QString getRulesetShortName(QString rulesetLongName);

    /**
    \brief get the path and name of the iptables binary

    \return the path and name of the iptables binary
      *********************/
    virtual QString getIptablesBinary();

    /**
    \brief sets the IptablesBinary variable of the class with the path and filename
           of the iptables variable.

    The path and file name of the iptables application is placed in the
    IptablesBinary. If the iptables program does not exist or any other error
    occurs the IptablesBinary is set to an empty string.
    The path and filename of the iptables application is obtained from the sysconf
    table in the application database.
      ***********************/
    virtual void setIptablesBinary();

    /**
    \brief Interrogate the current firewall using iptables and list the current
           rules.

    \return Current firewall rules
      ******************************/
    virtual QString listIptablesRules();

    /**
    \brief create a single string from the arguments

    \param cmd - name of program to execute
    \param arglist - list of arguments

    \return a single string from the arguments
      *******************/
    virtual QString printCmdLine(QString cmd, QStringList argList);

    /**
    \brief Sets the comment mark to the argument

    \param commentMark - string to be treated as start of comment
           marker - normally "#"
      ******************/
    virtual void setCommentMark(QString commentMark);

    /**
    \brief get the current comment mark

    \return current comment mark
      ***********************/
    virtual QString getCommentMark();

    /* ****************************
     Methods to process a ruleset
    ********************************/

    /**
    \brief Get the required SQLRecord containing the ruleset

    ******************/
    virtual QSqlRecord getRuleset(QString rulesetName);

    /**
    \brief Get the ruleset as a QStringList off the ruleset table using the ruleset name

    *********************/
    virtual QStringList getRulesetRows(QString rulesetName);

    /**
    \brief  replace included ruleset snippets with the snippet contents

    \param rulesetList - list containing ruleset

    \return ruleset with any includes expanded
      ****************************/
    virtual QStringList processRulesetIncludes(QStringList rulesetList);

    /**
    \brief Execute the commands in a ruleset

    \param rulesetName - name of ruleset

    \return always returns true
    *********************/
    virtual bool    processRuleset(QString rulesetName);

    /**
    \brief For debugging purposes prints arguments to console using qDebug
      ****************/
    virtual void processStringList(QString title, QStringList list);

signals:

    /**
    \brief Propogates result of executing a program
      *********************/
    void cmdOutput(QString program, QStringList arguments, int exitCode, QString result);

public slots:


protected slots:

    /**
    \brief This slot is only ever called from the IpProcess Class via the signal slot
    mechanism. It is used to propogate the results of one command in a ruleset when
    it is processed by the ipProcess class
    QString program, QStringList arguments, int exitCode, QString result

    \param  program - name of program
    \param  arguments - list of arguments to program
    \param  exitCode - exit code from executed program
    \param  result - output from executed program

        ****************/
    void slotCmdOutput(QString program, QStringList arguments, int exitCode, QString result);

protected:

    QPointer<IpProcess> process;
    QPointer<LinuxUserId> userId;
    QPointer<CmdLine> cmdLine;
    QPointer<RuleSnippet> ruleSnippet;
    QString IptablesBinary;

private:


};

#endif // IPTABLES_H
