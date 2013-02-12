#include "databasemanager.h"


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

bool DatabaseManager::createSysconfTable()
{
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query;
        QString qryMsg = QString("create table %1 "
                                 "("
                                 " id integer primary key not null, "
                                 " shell	        varchar(100) not null, "
                                 " iptables         varchar(100) not null, "
                                 " defaultRuleName	varchar(100) not null, "
                                 " foreign key(defaultRuleName) references %2(name) "
                                 "         on delete restrict on update restrict "
                                 ")"
                                );
        ret = query.exec(qryMsg.arg("sysconf").arg("ruleset"));
        ret = query.exec(qryMsg.arg("sysconfdef").arg("rulesetdef"));
    }
    return ret;
}


bool DatabaseManager::createSysconfRow()
{
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query;
        QString qryMsg = QString(" insert into %1 "
                                 "   (shell, iptables, defaultRuleName) "
                                 " values "
                                 "   ('/bin/bash', '/sbin/iptables', 'Clean Firewall') "
                                );
        ret = query.exec(qryMsg.arg("sysconf"));
        ret = query.exec(qryMsg.arg("sysconfdef"));

    }
    return ret;
}


bool DatabaseManager::createInitialRows()
{
    // Ruleset row has to be created before
    // sysconf row due to foreign key
    createRulesetRows();
    createRulesetSnippetRows();
    createSysconfRow();

    return true;
}




bool DatabaseManager::createRulesetTable()
{
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query;
        QString qryMsg = QString("create table %1 "
                                 "("
                                 " id integer primary key not null, "
                                 " name     varchar(100) unique not null, "
                                 " rules	text not null, "
                                 " foreign key(name) references %2(name) "
                                 "         on delete restrict on update restrict "
                                 ")");
        ret = query.exec(qryMsg.arg("ruleset").arg("sysconf"));
        ret = query.exec(qryMsg.arg("rulesetdef").arg("sysconfdef"));
    }
    return ret;
}


bool DatabaseManager::createRulesetRows()
{
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query;
        QString qryMsg = QString(" insert into %1 "
                                 "   (name, rules) "
                                 " values "
                                 "   ('Clean Firewall', 'Clean firewall rules') ");
        ret = query.exec(qryMsg.arg("ruleset"));
        ret = query.exec(qryMsg.arg("rulesetdef"));

        qryMsg = QString(" insert into %1 "
                         "   (name, rules) "
                         " values "
                         "   ('Home', 'Home firewall rules') "
                        );
        ret = query.exec(qryMsg.arg("ruleset"));
        ret = query.exec(qryMsg.arg("rulesetdef"));


        qryMsg = QString(" insert into %1 "
                         "   (name, rules) "
                         " values "
                         "   ('Office', 'Office firewall rules') "
                        );
        ret = query.exec(qryMsg.arg("ruleset"));
        ret = query.exec(qryMsg.arg("rulesetdef"));

        qryMsg = QString(" insert into %1 "
                         "   (name, rules) "
                         " values "
                         "   ('Public location', 'Public location firewall rules') "
                         );
        ret = query.exec(qryMsg.arg("ruleset"));
        ret = query.exec(qryMsg.arg("rulesetdef"));

    }
    return ret;
}


bool DatabaseManager::createRulesetSnippetsTable()
{
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query;
        QString qryMsg = QString("create table %1 "
                                 "("
                                 " id integer primary key not null, "
                                 " name     varchar(100) unique not null, "
                                 " snippets	text not null "
                                 ")");
        ret = query.exec(qryMsg.arg("rulesetsnippets"));
        ret = query.exec(qryMsg.arg("rulesetsnippetsdef"));
    }
    return ret;
}


bool DatabaseManager::createRulesetSnippetRows()
{
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query;
        QString qryMsg = QString(" insert into %1 "
                                 "   (name, snippets) "
                                 " values "
                                 "   ('Snippet 1', 'Snippet 1 iptables statements') ");
        ret = query.exec(qryMsg.arg("rulesetsnippets"));
        ret = query.exec(qryMsg.arg("rulesetsnippetsdef"));

        qryMsg = QString(" insert into %1 "
                         "   (name, snippets) "
                         " values "
                         "   ('Snippet 2', 'Snippet 2 iptables statements') "
                        );
        ret = query.exec(qryMsg.arg("rulesetsnippets"));
        ret = query.exec(qryMsg.arg("rulesetsnippetsdef"));


        qryMsg = QString(" insert into %1 "
                         "   (name, snippets) "
                         " values "
                         "   ('Snippet 3', 'Snippet 3 iptables statements') "
                        );
        ret = query.exec(qryMsg.arg("rulesetsnippets"));
        ret = query.exec(qryMsg.arg("rulesetsnippetsdef"));

        qryMsg = QString(" insert into %1 "
                         "   (name, rules) "
                         " values "
                         "   ('Snippet 4', 'Snippet 4 iptables statements') "
                         );
        ret = query.exec(qryMsg.arg("rulesetsnippets"));
        ret = query.exec(qryMsg.arg("rulesetsnippetsdef"));

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
            qDebug("Query error on sysconf: [%s]", qry.lastError().text().toAscii().data());
        }
    }
    else
    {
        qDebug("Query error on sysconf: [%s]", qry.lastError().text().toAscii().data());
    }

    return rulesetRec;
}
