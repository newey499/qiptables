#ifndef CMDLINE_H
#define CMDLINE_H

#include <QObject>
#include <QString>
#include <QStringList>

class CmdLine : public QObject
{
    Q_OBJECT
public:
    explicit CmdLine(QObject *parent = 0);
    explicit CmdLine(QString commentMark, QObject *parent = 0);

    virtual void setCommentMark(QString commentMark);
    virtual QString getCommentMark();

    // Strip any comments from the ruleset
    virtual QStringList stripComments(QStringList rulesetList, QString commentMark = "#");
    virtual QString     stripComments(QString rule, QString commentMark = "#");

    // Strip any blank lines from the ruleset
    virtual QStringList stripBlankLines(QStringList rulesetList);

signals:

public slots:


protected:

    QString commentMark;

};

#endif // CMDLINE_H
