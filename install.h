#ifndef FORMINSTALL_H
#define FORMINSTALL_H

#include <QDir>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QWidget>

#include "databasemanager.h"

class DatabaseManager;

class Install : public QObject
{
    Q_OBJECT

public:

    static const QString INSTALL_DIR;

    explicit Install(QObject *parent = 0);
    ~Install();

    QString performInstall(bool forceInstall = false);

    bool createSysconfTable();
    bool createRulesetTable();
    bool createRulesetSnippetsTable();


    bool createInitialRows();

signals:

public slots:

protected:

    QDir *dir;
    QFile *file;
    DatabaseManager *dm;

    bool createQiptablesDatabase();

    bool createDir(QString dirName);
    bool createQiptablesDir();
    bool createQiptablesTmpDir();
    bool createQiptablesToolsDir();

    bool createRulesetRows();
    bool insertRulesetRow(QString rulesName, QStringList rulesList);
    bool createRulesetSnippetRows();
    bool insertRuleSnippetRow(QString snippetName, QStringList snippetList);
    bool createSysconfRow();

    QString createFile(QString filename, QString content, bool executable = false);
    QString createFile(QString filename, QStringList content, bool executable = false);


    QString createScriptClearFirewall();


private:

};

#endif // FORMINSTALL_H
