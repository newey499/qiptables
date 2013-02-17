#ifndef CMDLINE_H
#define CMDLINE_H

#include <QObject>
#include <QString>
#include <QStringList>

#include "rulesnippet.h"

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
