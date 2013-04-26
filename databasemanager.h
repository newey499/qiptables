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



/**
\file databasemanager.h
\class DatabaseManager

\brief Class to manipulate application database

***************************/
class DatabaseManager : public QObject
{
    public:

        static const QString DB_NAME;

        static QSqlDatabase dbm;

        /**
          \brief Constructor
          \param dbPath - Path to application database.
                 This path should not include a trailing backslash.
          \param parent
          **************/
        DatabaseManager(QString dbPath, QObject *parent = 0);
        ~DatabaseManager();

        /**
          \brief Close the database
          *************/
        virtual void closeDB();

        /**
          \brief Checks for existence of database
          \return true if database exists else false

          **************/
        virtual bool databaseExists();

        /**
          \brief Deletes Database

          Copes with attempt to delete non existent database.

          \return true if database deleted else false
          ****************/
        virtual bool deleteDB();

        /**
          \brief Deletes table from database
          \param tableName - name of table to be deleted
          \return true if table deleted else false
          *****************/
        virtual bool dropTable(QString tableName);

        /**
          \brief returns file name of current database
          \return file name of current database
          ***************/
        virtual QString getDatabaseFileName();

        /**
          \brief Get a reference to the current database
          \return reference to the current database
          *************************/
        virtual QSqlDatabase getDb();

        /**
          \brief Open the application database
          \return true if database opened else false
          *****************/
        virtual bool openDB();

        /**
          \brief Get system configuration settings

          System configuration settings are held in a table on the application
          database. This table only has one row. Each column of the table contains
          a configuration setting.

          \return Row of system configuration table.
          ********************/
        virtual QSqlRecord getSysconfRow();

        /**
          \brief Get row of ruleset table
          \param rulesetName - name of ruleset used to identify required row
          \return Row of ruleset table
          ******************/
        virtual QSqlRecord getRulesetRow(QVariant rulesetName);

        /**
          \brief Get ruleset from ruleset name.
          \param rulesetName - name of ruleset used to identify required row
          \return string containing ruleset
          ***********************/
        virtual QString getRuleset(QVariant rulesetName);

        /**
          \brief Get list of ruleset names
          \return List of ruleset names
          *******************/
        virtual QStringList getRulesetNames();

        /**
          \brief Get row of rulesnippet table
          \param rulesnippetName - name of rulesnippet used to identify required row
          \return Row of rulesnippet table
          ******************/
        virtual QSqlRecord getRuleSnippetRow(QVariant ruleSnippetName);

        /**
          \brief Get error (if any) resulting from most recent SQL command
          \return SQL error object
          **********************/
        virtual QSqlError lastError();

        /**
        \brief Check to see if rulesnippet is in use as an include in a ruleset

        \param rulesnippetName;

        \return true if rulesnippet is in use else false
          *********************/
        virtual QStringList isRulesnippetInUse(QString rulesnippetName);

        /**
        \brief check if rulesnippet is included in line of ruleset

        \param rulesnippetName - name of rule snippet
        \param rulesetline - line of ruleset

        \return true if line contains included rulesnippet name else false
          *****************/
        virtual bool rulesnippetInLine(QString rulesnippetName, QString rulesetLine);

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
