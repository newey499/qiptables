#include "install.h"


const QString Install::INSTALL_DIR = QString("/etc/qiptables");

Install::Install(QObject *parent) :
    QObject(parent)
{
    dir = new QDir();
    file = new QFile(this);
    dm = new DatabaseManager(Install::INSTALL_DIR, this);

    performInstall();
}

Install::~Install()
{
    if (dir)
    {
        delete dir;
    }
}


QString Install::performInstall()
{
    QString msg("Performing Qiptables Install");

    createQiptablesDir();
    msg = msg.append("\n%1 - Ok").arg(Install::INSTALL_DIR);

    createQiptablesDatabase();
    msg = msg.append("\n%1 database - Ok").arg(dm->getDatabaseFileName());

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

    return result;

}


bool Install::createQiptablesDatabase()
{
    if (! dm->databaseExists())
    {
        qDebug("Creating database");
        dm->openDB();
        dm->createPersonTable();
        dm->closeDB();
    }
    else
    {
        qDebug("database already exists");
    }

    return true;
}
