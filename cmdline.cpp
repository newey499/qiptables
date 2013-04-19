
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


#include "cmdline.h"



const QString CmdLine::defaultCommentMark = "#";
const QString CmdLine::defaultIncludeMark = "#include ";

CmdLine::CmdLine(QObject *parent) :
    QObject(parent)
{
    this->commentMark = CmdLine::defaultCommentMark;
    //ruleSnippet = new RuleSnippet(this);
}

void CmdLine::setCommentMark(QString commentMark)
{
    this->commentMark = commentMark;
}

QString CmdLine::getCommentMark()
{
    return commentMark;
}

QString CmdLine::stripComments(QString rule, QString commentMark)
{
    setCommentMark(commentMark);
    // first trim off leading and trailing spaces
    rule = rule.trimmed();

    int offset = rule.indexOf(getCommentMark(), 0, Qt::CaseSensitive);
    int offsetInclude = rule.indexOf(CmdLine::defaultIncludeMark.trimmed(), 0, Qt::CaseSensitive);

    if (offsetInclude == -1) // no includes to worry about
    {
        if (offset != -1)
        {
            // remove the comment mark and everything after it
            // then remove leading and trailing spaces
            rule = rule.left(offset).trimmed();
        }
    }
    else
    {
        if (rule.indexOf(CmdLine::defaultIncludeMark, 0, Qt::CaseSensitive) == -1)
        {
            /*******
            qDebug("invalid call to #include - must be followed by space [%s]",
                    rule.toAscii().data());
            *********************/
        }
        // find any comment after the include
        int newOffset = offset + CmdLine::defaultIncludeMark.length();
        offset = rule.indexOf(getCommentMark(), newOffset, Qt::CaseSensitive);
        if (offset != -1)
        {
            // remove the comment mark and everything after it
            rule = rule.left(offset);
        }
        // remove the CmdLine::defaultIncludeMark String
        // rule = rule.replace(CmdLine::defaultIncludeMark, "", Qt::CaseSensitive);
        // remove single and double quotes
        rule = rule.replace("'", "", Qt::CaseSensitive);
        rule = rule.replace("\"", "", Qt::CaseSensitive);
        rule = rule.trimmed(); // remove leading and trailing spaces
        //qDebug("processed include line [%s]", rule.toAscii().data());

    }
    return rule;
}

QStringList CmdLine::stripComments(QStringList rulesetList, QString commentMark)
{
    QStringList result;

    setCommentMark(commentMark);

    for (int i = 0; i < rulesetList.count(); i++)
    {
        QString tmp = rulesetList.at(i);
        result.append(stripComments(rulesetList.at(i), commentMark));
    }

    result = stripBlankLines(result);

    return result;
}


QStringList CmdLine::stripBlankLines(QStringList rulesetList)
{
    QStringList result;

    for (int i = 0; i < rulesetList.count(); i++)
    {
        if ("" != rulesetList.at(i).trimmed())
        {
            result.append(rulesetList.at(i));
        }
    }

    return result;
}




