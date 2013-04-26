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

/**
\file databasemanager.cpp

\brief Manipulate application database


***************************/

#include "genlib.h"
#include "databasemanager.h"
#include "cmdline.h"

const QString DatabaseManager::DB_NAME = QString("qiptables.db.sqlite");


DatabaseManager::DatabaseManager(QString dbPath, QObject *parent) :
    QObject(parent)
{
    this->dbPath = dbPath;
    openDB();
}

DatabaseManager::~DatabaseManager()
{

}

QSqlDatabase DatabaseManager::getDb()
{
    return this->db;
}

bool DatabaseManager::databaseExists()
{
    bool result = false;

    result = QFile::exists(getDatabaseFileName());

    return result;
}


QString DatabaseManager::getDatabaseFileName()
{
    QString path(dbPath);
    path.append(QDir::separator()).append(DB_NAME);
    path = QDir::toNativeSeparators(path);

    return path;
}


bool DatabaseManager::openDB()
{
    bool result = false;

    if (db.isOpen())
    {
        result = true;
    }
    else
    {
        // Find QSLite driver
        db =  QSqlDatabase::addDatabase("QSQLITE");

        db.setDatabaseName(getDatabaseFileName());

        // Open database
        result = db.open();
        if (! result)
        {
            qDebug("DatabaseManager::openDB(): failed to open db - error \n[%s]",
                db.lastError().text().toAscii().data());
        }
    }

    return result;
}

QSqlError DatabaseManager::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return db.lastError();
}

void DatabaseManager::closeDB()
{
    // Close database
    db.close();
}

bool DatabaseManager::deleteDB()
{
    closeDB();

    return QFile::remove(getDatabaseFileName());
}


bool DatabaseManager::dropTable(QString tableName)
{
    // Drop table <tableName>
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query;
        QString txt = QString("drop table %1").arg(tableName);
        ret = query.exec(txt);

    }
    return ret;
}



QSqlRecord DatabaseManager::getSysconfRow()
{
    QSqlQuery qry;
    syconfRec = QSqlRecord();

    if (qry.exec("select id, shell, iptables, defaultRuleName from sysconf"))
    {
        if (qry.first())
        {
            syconfRec = qry.record();
        }
        else
        {
            qDebug("Query error on sysconf: [%s]", qry.lastError().text().toAscii().data());
        }
    }
    else
    {
        qDebug("Query error on sysconf: [%s]", qry.lastError().text().toAscii().data());
    }

    return syconfRec;
}


QSqlRecord DatabaseManager::getRulesetRow(QVariant rulesetName)
{
    QSqlQuery qry;
    rulesetRec = QSqlRecord();

    qry.prepare("select id, name, rules from ruleset where name = :name");
    qry.bindValue(QString(":name"), rulesetName);
    if (qry.exec())
    {
        if (qry.first())
        {
            rulesetRec = qry.record();
        }
        else
        {
            qDebug("Query error on ruleset: [%s]", qry.lastError().text().toAscii().data());
        }
    }
    else
    {
        qDebug("Query error on ruleset: [%s]", qry.lastError().text().toAscii().data());
    }

    return rulesetRec;
}



QSqlRecord DatabaseManager::getRuleSnippetRow(QVariant ruleSnippetName)
{
    QSqlQuery qry;
    ruleSnippetRec = QSqlRecord();

    qry.prepare("select id, name, snippets from rulesetsnippets where name = :name");
    qry.bindValue(QString(":name"), ruleSnippetName);
    if (qry.exec())
    {
        if (qry.first())
        {
            ruleSnippetRec = qry.record();
        }
        else
        {
            //qDebug("Query error on rulesetsnippets: [%s]", qry.lastError().text().toAscii().data());
        }
    }
    else
    {
        //qDebug("Query error on rulesetsnippets: [%s]", qry.lastError().text().toAscii().data());
    }

    return ruleSnippetRec;
}


QString DatabaseManager::getRuleset(QVariant rulesetName)
{
    QSqlRecord rec = getRulesetRow(rulesetName);

    return rec.value("rules").toString();
}

QStringList DatabaseManager::getRulesetNames()
{
    QStringList rs;
    QSqlQuery qry;

    qry.prepare("select name from ruleset");

    if (qry.exec())
    {
        while (qry.next())
        {
            rs.append(qry.record().value("name").toString());
        }
    }
    else
    {
        qDebug("DatabaseManager::getRulesetNames() - failed to exec qry");
    }

    return rs;
}


QStringList DatabaseManager::isRulesnippetInUse(QString rulesnippetName)
{
    QStringList result;
    QSqlQuery qry;
    QStringList ruleset;

    qry.prepare("select id, name, shortname, rules from ruleset");

    if (! qry.exec())
    {
        qDebug("%s", lastError().text().toAscii().data());
        result.append(lastError().text());
        return result;
    }

    while (qry.next())
    {
        ruleset.clear();
        ruleset = qry.record().value("rules").toString().split("\n");
        qDebug("=============================");
        for (int i = 0; i < ruleset.count(); i++)
        {
            // does line contain #include for the rulesnippet
            if (rulesnippetInLine(rulesnippetName, ruleset.at(i)))
            {
                result.append(qry.record().value("name").toString().trimmed());
            }
        }

    }


    return result;
}


bool DatabaseManager::rulesnippetInLine(QString rulesnippetName, QString rulesetLine)
{
    bool result = false;
    QString includeString = GenLib::getIncludeString(rulesnippetName);
    CmdLine cmdLine(this);

    qDebug("DatabaseManager::rulesnippetInLine(QString rulesnippetName, QString rulesetLine)");
    qDebug("Searching for snippet [%s]", rulesnippetName.toAscii().data());
    qDebug("Original rulesetLine [%s]", rulesetLine.toAscii().data());
    rulesetLine = cmdLine.stripComments(rulesetLine);
    rulesetLine = rulesetLine.trimmed();
    qDebug("rulesetLine after comment strip [%s]", rulesetLine.toAscii().data());

    qDebug("Searching for Include [%s] in ruleset line [%s]",
           includeString.toAscii().data(),
           rulesetLine.toAscii().data());

    if ( rulesetLine.contains(includeString, Qt::CaseSensitive))
    {
        result = true;
        qDebug("Include [%s] found in ruleset line [%s]",
               includeString.toAscii().data(),
               rulesetLine.toAscii().data());
    }

    return result;
}
