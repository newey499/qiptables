#include "cmdline.h"

CmdLine::CmdLine(QObject *parent) :
    QObject(parent)
{
    commentMark = "#";
}

CmdLine::CmdLine(QString commentMark, QObject *parent) :
    QObject(parent)
{
    setCommentMark(commentMark);
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

    int offset = rule.indexOf(getCommentMark(), 0, Qt::CaseSensitive);

    if (offset != -1)
    {
        // remove the comment mark and everything after it
        // then remove leading and trailing spaces
        rule = rule.left(offset).trimmed();
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




