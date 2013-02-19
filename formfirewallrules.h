#ifndef FORMFIREWALLRULES_H
#define FORMFIREWALLRULES_H



#include <QListView>
#include <QMessageBox>
#include <QPointer>
#include <QShowEvent>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QTreeView>
#include <QTextEdit>
#include <QVariant>
#include <QWidget>


#include "install.h"
#include "ipprocess.h"
#include "iptables.h"
#include "qiptablesexception.h"
#include "databasemanager.h"


namespace Ui {
class FormFirewallRules;
}

class DatabaseManager;

class FormFirewallRules : public QWidget
{
    Q_OBJECT

public:
    explicit FormFirewallRules(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~FormFirewallRules();

    QString getCurrentFirewallShortName();
    QSqlRecord getFirewallRulesetFromShortName(QString shortName);
    QString getRulesetNameFromShortName(QString shortName);

signals:

public slots:

    virtual void showCurrentFirewallRules();
    virtual void slotCurrentRules();
    virtual void slotEnableRuleset();
    virtual void slotCbxFirewallsIndexChanged(int index);

protected:

    QPointer<DatabaseManager> databaseManager;
    QPointer<IpProcess> proc;
    QPointer<Iptables> ipTables;

    virtual void showEvent(QShowEvent *event);

private:
    Ui::FormFirewallRules *ui;
};

#endif // FORMFIREWALLRULES_H
