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


bool DatabaseManager::createRulesetTable()
{
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query;
        QString qryMsg = QString("create table %1 "
                                 "("
                                 " id integer primary key not null, "
                                 " name     varchar(100) not null, "
                                 " rules	text not null "
                                 ")");
        ret = query.exec(qryMsg.arg("ruleset"));
        ret = query.exec(qryMsg.arg("rulesetdef"));
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
                                 " defaultRuleName	varchar(100) not null, "
                                 " foreign key(defaultRuleName) references ruleset(name) "
                                 "         on delete restrict on update restrict "
                                 ")"
                                );
        ret = query.exec(qryMsg.arg("sysconf"));
        ret = query.exec(qryMsg.arg("sysconfdef"));

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
                                 "   (shell, defaultRuleName) "
                                 " values "
                                 "   ('/bin/bash', 'Clean Firewall') "
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
    createSysconfRow();

    return true;
}


