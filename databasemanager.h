#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QFile>
#include <QDir>
#include <QObject>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>


#include "install.h"



class DatabaseManager : public QObject
{
    public:

        static const QString DB_NAME;

        DatabaseManager(QString dbPath, QObject *parent = 0);
        ~DatabaseManager();

        void closeDB();
        bool databaseExists();
        bool deleteDB();
        bool dropTable(QString tableName);
        QString getDatabaseFileName();
        bool openDB();
        QSqlRecord getSysconfRow();
        QSqlRecord getRulesetRow(QVariant rulesetName);

        QSqlError lastError();

        bool createSysconfTable();
        bool createRulesetTable();
        bool createRulesetSnippetsTable();


        bool createInitialRows();


    public slots:

    signals:

    protected:

        QString dbPath;
        QSqlDatabase db;
        QSqlRecord syconfRec;
        QSqlRecord rulesetRec;

        bool createRulesetRows();
        bool createRulesetSnippetRows();
        bool createSysconfRow();

    private:

};

#endif // DATABASEMANAGER_H
