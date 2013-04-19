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



#ifndef RULESNIPPET_H
#define RULESNIPPET_H

#include <QPointer>
#include <QObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QString>
#include <QStringList>
#include <QVariant>

#include "cmdline.h"
#include "databasemanager.h"
#include "iptables.h"

class CmdLine;

class RuleSnippet : public QObject
{
    Q_OBJECT
public:

    explicit RuleSnippet(QObject *parent = 0);
    ~RuleSnippet();

    virtual QString getIptablesBinary();
    virtual void setIptablesBinary();


    QPointer<DatabaseManager> db;

    // Get the required SQLRecord containing the rule snippet
    virtual QSqlRecord getRuleSnippet(QString ruleSnippetName);


    // Get the ruleset as a QStringList off the rule snippet table using the rule snippet name
    virtual QStringList getRuleSnippetRows(QString ruleSnippetName);

    virtual QStringList getIncludeRuleSnippetRows(QString ruleSnippetName);

signals:

public slots:


protected:

    QString IptablesBinary;
    QPointer<CmdLine> cmdLine;


private:


};

#endif // RULESNIPPET_H
