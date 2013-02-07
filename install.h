#ifndef FORMINSTALL_H
#define FORMINSTALL_H

#include <QDir>
#include <QFile>
#include <QWidget>

#include "databasemanager.h"

class DatabaseManager;

class Install : public QObject
{
    Q_OBJECT
    
public:

    static const QString INSTALL_DIR;

    explicit Install(QObject *parent = 0);
    ~Install();

    QString performInstall();

signals:

public slots:

protected:

    QDir *dir;
    QFile *file;
    DatabaseManager *dm;

    bool createQiptablesDir();
    bool createQiptablesDatabase();

    
private:

};

#endif // FORMINSTALL_H
