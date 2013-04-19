
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
