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
