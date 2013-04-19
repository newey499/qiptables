
/************************************************************************
Copyright Chris Newey 2013

This file is part of Qiptables.

Qiptables is distributed under the terms of the GNU General Public License

Qiptables is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Qiptables is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Qiptables.  If not, see <http://www.gnu.org/licenses/>.

***************************************************************************/



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
