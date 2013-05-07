/************************************************************************
Copyright Chris Newey 2013

qiptables@hotmail.com

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

#include <QComboBox>
#include <QListView>
#include <QMessageBox>
#include <QPointer>
#include <QShowEvent>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStatusBar>
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

/**
 \file formfirewallrules.h

 \class FormFirewallRules

 \brief Get Ruleset from application database


*/
class FormFirewallRules : public QWidget
{
    Q_OBJECT

public:

    explicit FormFirewallRules(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~FormFirewallRules();

    /**
    \brief Get the short name of the currently functioning firewall ruleset

    When a ruleset is exported from the database and made the active firewall
    configuration by the use of iptables commands the ruleset full name is
    massaged to create a short name which is stored in the firewall

    \return String containing short firewall name
      *****************/
    QString getCurrentFirewallShortName();

    /**
    \brief Get the ruleset row from the database using the short name

    \param shortName - The short name of the ruleset
    \return the row of the ruleset

      **********************/
    QSqlRecord getFirewallRulesetFromShortName(QString shortName);

    /**
    \brief Get the full ruleset name from the short name

    \param shortName - The short name of the ruleset
    \return the full name of the ruleset
      ****************/
    QString getRulesetNameFromShortName(QString shortName);



signals:

public slots:

    /**
    \brief Sets the text of the combobox containing firewall ruleset names
      ********************/
    virtual void setCbxFirewallsText(QString text);


    /**
    \brief Displays string in plain text edit box
      ******************************/
    virtual void slotDisplayString(QString msg);

    /**
    \brief synonym for showCurrentFirewallRules()
      ***************/
    virtual void slotCurrentRules();

    /**
    \brief Enables currently selected firewall ruleset

    Loads the currently selected ruleset using iptables and then displays
    the new firewall rules in the text edit box
      *****************/
    virtual void slotEnableRuleset();

    /**
    \brief Called when the ruleset selected in the combobox changes
      *****************/
    virtual void slotCbxFirewallsIndexChanged(int index);


    /**
    \brief fills firewall ruleset name combobox with currently defined ruleset names
      ***********************/
    virtual void fillCbxFirewallRulesetNames();

protected:

    QPointer<DatabaseManager> databaseManager;
    QPointer<IpProcess> proc;
    QPointer<Iptables> ipTables;

    /**
    \brief Called when form becomes visible
    \param event - QT generated event
      **************/
    virtual void showEvent(QShowEvent *event);

private:
    Ui::FormFirewallRules *ui;
};

#endif // FORMFIREWALLRULES_H
