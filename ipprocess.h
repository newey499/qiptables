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

    virtual bool checkForRoot(QString title = "Application Name");
    virtual QString	execute (const QString &program, const QStringList &arguments = QStringList());
    virtual QString executeSynchronous(const QString &program, const QStringList &arguments = QStringList());
    virtual QString exec(QString cmd, QStringList argList);
    virtual QString printCmdLine(QString cmd, QStringList argList = QStringList());

    virtual QString execCmdLine(QString cmd);

signals:

    void procCmdOutput(QString, QStringList, int, QString);

public slots:

};

#endif // IPPROCESS_H
