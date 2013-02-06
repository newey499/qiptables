#ifndef IPPROCESS_H
#define IPPROCESS_H

#include <QMessageBox>
#include <QProcess>
#include <QProcessEnvironment>

#include "mainwindow.h"
#include "linuxuserid.h"

class IpProcess : public QProcess
{
    Q_OBJECT
public:

    static const int PROCESS_CANNOT_BE_STARTED;

    static QProcessEnvironment systemEnvironment();

    explicit IpProcess(QObject *parent = 0);

    virtual bool checkForRoot();

    virtual int	execute (const QString &program, const QStringList &arguments);
    virtual int	execute (const QString &program);

    virtual QString executeSynchronous(const QString &program, const QStringList &arguments);
    virtual QString executeSynchronous(const QString &program);

signals:
    
public slots:
    
};

#endif // IPPROCESS_H
