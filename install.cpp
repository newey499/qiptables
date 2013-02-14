#include "install.h"


const QString Install::INSTALL_DIR = QString("/etc/qiptables");

Install::Install(QObject *parent) :
    QObject(parent)
{
    dir = new QDir();
    file = new QFile(this);
    dm = new DatabaseManager(Install::INSTALL_DIR, this);
}

Install::~Install()
{
    if (dir)
    {
        delete dir;
    }
}


QString Install::performInstall(bool forceInstall)
{
    QString msg("Install::performInstall()");
    qDebug("%s", msg.toAscii().data());
    msg = "";
    if (forceInstall)
    {
        qDebug("Install forced");
    }

    if ( (! dm->databaseExists()) || forceInstall)
    {
        msg = "Performing Qiptables Install";

        createQiptablesDir();
        msg = msg.append("\n%1 - Ok").arg(Install::INSTALL_DIR);

        createQiptablesDatabase();
        msg = msg.append("\n%1 database - Ok").arg(dm->getDatabaseFileName());

        QFile dbFile(dm->getDatabaseFileName());
        dbFile.setPermissions(QFile::ReadOwner  | QFile::WriteOwner |
                              QFile::ReadGroup  | QFile::WriteGroup |
                              QFile::ReadOther  | QFile::WriteOther );
    }
    else
    {
        msg = "Install not required - not performed";
        qDebug("%s", msg.toAscii().data());
    }

    return msg;
}


bool Install::createQiptablesDir()
{
    bool result = true;
    QString tmp;

    createDir(Install::INSTALL_DIR);
    createDir(QString("%1/%2").arg(Install::INSTALL_DIR).arg("tmp"));
    createDir(QString("%1/%2").arg(Install::INSTALL_DIR).arg("tools"));

    return result;
}


bool Install::createDir(QString dirName)
{
    bool result;

    /**********
     This looks strange - but the docs say:

    bool QDir::mkdir ( const QString & dirName ) const
    Creates a sub-directory called dirName.

    Returns true on success; otherwise returns false.

    If the directory already exists when this function is called, it will return false.
     ********************/
    if (dir->exists(dirName))
    {
        qDebug("Directory [%s] already exists", dirName.toAscii().data());
        result = false;
    }
    else
    {
        dir->mkdir(dirName);
        qDebug("Directory [%s] created OK", dirName.toAscii().data());
        result = true;
    }

    return result;

}


bool Install::createQiptablesDatabase()
{
    if (! dm->databaseExists())
    {
        qDebug("Creating database");
        dm->openDB();

        qDebug("Creating Ruleset Table");
        qDebug("%s", createRulesetTable() ? "true" : "false");
        qDebug("%s", dm->lastError().text().toAscii().data());

        qDebug("Creating Sysconf Table");
        qDebug("%s", createSysconfTable() ? "true" : "false");
        qDebug("%s", dm->lastError().text().toAscii().data());

        qDebug("Creating Ruleset Snippets Table");
        qDebug("%s", createRulesetSnippetsTable() ? "true" : "false");
        qDebug("%s", dm->lastError().text().toAscii().data());


        qDebug("Creating Initial data");
        qDebug("%s", createInitialRows() ? "true" : "false");
        qDebug("%s", dm->lastError().text().toAscii().data());

    }
    else
    {
        qDebug("database already exists");
    }

    return true;
}


QString Install::createScriptClearFirewall()
{
    QStringList script;
    QString filename = QString("%1/%2/%3").
            arg(Install::INSTALL_DIR).
            arg("tools").
            arg("clearFirewall.sh");

    //QString tmp = QString("# ").append(filename);

    script  << "#!/bin/bash"
            << "################################################"
            << "#"
            << QString("# ").append(filename)
            << "#"
            << "# created by Qiptables install program"
            << "#"
            << "# referenced from ruleset table of Sqlite database"
            << "#"
            << "#"
            << "#"
            << "#"
            << "################################################"
            << "echo \"Stopping firewall and allowing everyone...\""
            << "iptables -F"
            << "iptables -X"
            << "iptables -t nat -F"
            << "iptables -t nat -X"
            << "iptables -t mangle -F"
            << "iptables -t mangle -X"
            << "iptables -P INPUT ACCEPT"
            << "iptables -P FORWARD ACCEPT"
            << "iptables -P OUTPUT ACCEPT"
            << " "
            << "iptables -L"
            << " ";

    // Create the file
    createFile(filename, script, true);

    return filename;
}




bool Install::createFile(QString filename, QString content, bool executable)
{
    QFile data(filename);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        out << content << "\n\n";
    }
    data.close();
    if (executable)
    {
        data.setPermissions(QFile::ReadOwner  |
                            QFile::WriteOwner |
                            QFile::ReadGroup  |
                            QFile::WriteGroup |
                            QFile::ReadOther  |
                            QFile::ExeOwner   |
                            QFile::ExeGroup);
    }

    return true;
}

bool Install::createFile(QString filename, QStringList content, bool executable)
{
    return createFile(filename,
                      content.join("\n"),
                      executable);
}

bool Install::createSysconfTable()
{
    bool ret = false;

    if (dm->getDb().isOpen())
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


bool Install::createSysconfRow()
{
    bool ret = false;
    if (dm->getDb().isOpen())
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


bool Install::createInitialRows()
{
    // Ruleset row has to be created before
    // sysconf row due to foreign key
    createRulesetRows();
    createRulesetSnippetRows();
    createSysconfRow();

    return true;
}




bool Install::createRulesetTable()
{
    bool ret = false;
    if (dm->getDb().isOpen())
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


bool Install::createRulesetRows()
{
    bool ret = false;
    if (dm->getDb().isOpen())
    {

        QString filename = createScriptClearFirewall();
        QSqlQuery query;
        QString qryMsg = QString(" insert into %1 "
                                 "   (name, rules) "
                                 " values "
                                 "   ('Clean Firewall - Accept everything', '%2') ");
        ret = query.exec(qryMsg.arg("ruleset").arg(filename));
        ret = query.exec(qryMsg.arg("rulesetdef").arg(filename));

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


bool Install::createRulesetSnippetsTable()
{
    bool ret = false;
    if (dm->getDb().isOpen())
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


bool Install::createRulesetSnippetRows()
{
    bool ret = false;
    if (dm->getDb().isOpen())
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
