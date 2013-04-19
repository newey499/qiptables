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

\todo method "Iptables::processRuleset(QString rulesetName)" - add code to process
each command found.

\todo method "Iptables::processRuleset(QString rulesetName)" - emit signals for
each command executed and the output of each command.

***************************/
class Iptables : public QObject
{
    Q_OBJECT

public:

    static const QString IPTABLES_BINARY;

    QPointer<DatabaseManager> db;

    explicit Iptables(QObject *parent = 0);
    ~Iptables();

    QString getRulesetShortName(QString rulesetLongName);

    virtual QString getIptablesBinary();
    virtual void setIptablesBinary();

    virtual QString listIptablesRules();
    virtual QString printCmdLine(QString cmd, QStringList argList);

    virtual void setCommentMark(QString commentMark);
    virtual QString getCommentMark();

    /* ****************************
     Methods to process a ruleset
    ********************************/

    // Get the required SQLRecord containing the ruleset
    virtual QSqlRecord getRuleset(QString rulesetName);


    // Get the ruleset as a QStringList off the ruleset table using the ruleset name
    virtual QStringList getRulesetRows(QString rulesetName);


    virtual QStringList processRulesetIncludes(QStringList rulesetList);

    // Execute the commands in the ruleset
    virtual bool    processRuleset(QString rulesetName);

    virtual void processStringList(QString title, QStringList list);

signals:

    void cmdOutput(QString program, QStringList arguments, int exitCode, QString result);

public slots:


protected slots:

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
