#ifndef FORMTEST_H
#define FORMTEST_H

#include <QPointer>
#include <QProcessEnvironment>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QStringList>
#include <QWidget>


#include "qiptablesexception.h"
#include "linuxuserid.h"
#include "ipprocess.h"
#include "iptables.h"
#include "install.h"
#include "databasemanager.h"

class Iptables;
class IpProcess;

namespace Ui {
class FormTest;
}

class FormTest : public QWidget
{
    Q_OBJECT

public:

    QPointer<DatabaseManager> db;
    QPointer<Iptables> iptables;
    QPointer<IpProcess> proc;


    explicit FormTest(QWidget *parent = 0);
    ~FormTest();




public slots:

    virtual void slotBtnTest();
    virtual void slotBtnRun();

protected:


protected slots:

    void slotCmdOutput(QString program, QStringList arguments, int exitCode, QString result);
    void slotRunRuleset();

private:
    Ui::FormTest *ui;
};

#endif // FORMTEST_H
