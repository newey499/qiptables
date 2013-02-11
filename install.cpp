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
    bool result;
    /**********
     This looks strange - but the docs say:

    bool QDir::mkdir ( const QString & dirName ) const
    Creates a sub-directory called dirName.

    Returns true on success; otherwise returns false.

    If the directory already exists when this function is called, it will return false.
     ********************/
    if (dir->exists(Install::INSTALL_DIR))
    {
        qDebug("Directory [%s] already exists", Install::INSTALL_DIR.toAscii().data());
        result = false;
    }
    else
    {
        dir->mkdir(Install::INSTALL_DIR);
        qDebug("Directory [%s] created OK", Install::INSTALL_DIR.toAscii().data());
        result = true;
    }

    createQiptablesTmpDir();

    return result;

}

bool Install::createQiptablesTmpDir()
{
    bool result;
    QString tmpDir(Install::INSTALL_DIR);
    tmpDir = tmpDir.append("/tmp");

    /**********
     This looks strange - but the docs say:

    bool QDir::mkdir ( const QString & dirName ) const
    Creates a sub-directory called dirName.

    Returns true on success; otherwise returns false.

    If the directory already exists when this function is called, it will return false.
     ********************/
    if (dir->exists(tmpDir))
    {
        qDebug("Directory [%s] already exists", tmpDir.toAscii().data());
        result = false;
    }
    else
    {
        dir->mkdir(tmpDir);
        qDebug("Directory [%s] created OK", tmpDir.toAscii().data());
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
        qDebug("%s", dm->createRulesetTable() ? "true" : "false");
        qDebug("%s", dm->lastError().text().toAscii().data());

        qDebug("Creating Sysconf Table");
        qDebug("%s", dm->createSysconfTable() ? "true" : "false");
        qDebug("%s", dm->lastError().text().toAscii().data());

        qDebug("Creating Ruleset Snippets Table");
        qDebug("%s", dm->createRulesetSnippetsTable() ? "true" : "false");
        qDebug("%s", dm->lastError().text().toAscii().data());


        qDebug("Creating Initial data");
        qDebug("%s", dm->createInitialRows() ? "true" : "false");
        qDebug("%s", dm->lastError().text().toAscii().data());

    }
    else
    {
        qDebug("database already exists");
    }

    return true;
}
