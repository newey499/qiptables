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

        static QSqlDatabase dbm;

        DatabaseManager(QString dbPath, QObject *parent = 0);
        ~DatabaseManager();

        void closeDB();
        bool databaseExists();
        bool deleteDB();
        bool dropTable(QString tableName);
        QString getDatabaseFileName();
        QSqlDatabase getDb();
        bool openDB();
        QSqlRecord getSysconfRow();
        QSqlRecord getRulesetRow(QVariant rulesetName);
        QString getRuleset(QVariant rulesetName);
        QStringList getRulesetNames();

        QSqlRecord getRuleSnippetRow(QVariant ruleSnippetName);

        QSqlError lastError();

    public slots:

    signals:

    protected:

        QString dbPath;
        QSqlDatabase db;
        QSqlRecord syconfRec;
        QSqlRecord rulesetRec;
        QSqlRecord ruleSnippetRec;



    private:

};

#endif // DATABASEMANAGER_H
