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

    /**
      Comments are shell script style "#". Everything from # to end
      of line are treated as a comment.
      ******************/
    static const QString defaultCommentMark;

    /**
      "#include" is used to include a code snippet into a ruleset
      instead of pasting the content of the snippet into a ruleset.
      ************************/
    static const QString defaultIncludeMark;

    explicit CmdLine(QObject *parent = 0);

    /**
      \brief Change comment mark
      \param commentMark - new comment mark
      ********************/
    virtual void setCommentMark(QString commentMark);

    /**
      \brief Get current comment mark
      \return Current comment mark
      **********************/
    virtual QString getCommentMark();

    /**
    \brief Strip any comments from the ruleset
    \param rulesetList - Series of lines from ruleset
    \param commentMark - String to be used to recognize a comment
    \return rulesetList with comments removed
    ****************/
    virtual QStringList stripComments(QStringList rulesetList,
                                      QString commentMark = CmdLine::defaultCommentMark);

    /**
    \brief Strip any comments from a single ruleset line
    \param rule - line from ruleset
    \param commentMark - String to be used to recognize a comment
    \return rule with comment removed
    ****************/
    virtual QString     stripComments(QString rule,
                                      QString commentMark = CmdLine::defaultCommentMark);

    /**
    \brief Strip any blank lines from the ruleset
    \param rulesetList - Series of lines from ruleset
    \return rulesetList with comments removed
    ****************************/
    virtual QStringList stripBlankLines(QStringList rulesetList);


signals:

public slots:


protected:

    QString commentMark;
    QPointer<RuleSnippet> ruleSnippet;

};

#endif // CMDLINE_H
