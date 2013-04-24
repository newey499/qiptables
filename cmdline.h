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



#ifndef CMDLINE_H
#define CMDLINE_H

#include <QObject>
#include <QString>
#include <QStringList>

#include "rulesnippet.h"

/**
\file cmdline.h
\class CmdLine

\brief Class to process a command line


Strips comments (#) and include (#include) from
a command line.

***************************/
class RuleSnippet;

class CmdLine : public QObject
{
    Q_OBJECT
public:

    static const QString defaultCommentMark;
    static const QString defaultIncludeMark;

    explicit CmdLine(QObject *parent = 0);

    virtual void setCommentMark(QString commentMark);
    virtual QString getCommentMark();

    // Strip any comments from the ruleset
    virtual QStringList stripComments(QStringList rulesetList,
                                      QString commentMark = CmdLine::defaultCommentMark);
    virtual QString     stripComments(QString rule,
                                      QString commentMark = CmdLine::defaultCommentMark);

    // Strip any blank lines from the ruleset
    virtual QStringList stripBlankLines(QStringList rulesetList);

    // Find any "#includes" in ruleset

signals:

public slots:


protected:

    QString commentMark;
    QPointer<RuleSnippet> ruleSnippet;

};

#endif // CMDLINE_H
