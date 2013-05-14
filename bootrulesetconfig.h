#ifndef BOOTRULESETCONFIG_H
#define BOOTRULESETCONFIG_H

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


#include <QObject>
#include <QPointer>
#include <QStringList>
#include <QVariant>

#include "genlib.h"
#include "install.h"
#include "ipprocess.h"

/**
 \file bootrulesetconfig.h

 \class BootRulesetConfig

 \brief Set boot time enabling of firewall.

 Options are:
 1) Don't start firewall
 2) Start firewall using init.d script file
 3) Start firewall using file in /etc/dhcp3/dhclient-enter-hooks.d


*/
class BootRulesetConfig : public QObject
{
    Q_OBJECT

public:

    static const int BOOT_NONE = 0;
    static const int BOOT_INIT_D = 1;

    static const QString BOOT_INIT_D_SHELL_SCRIPT;
    static const QString BOOT_INIT_D_RULESET;
    static const QString CLEAR_FIREWALL_RULES_SCRIPT;
    static const QString SAVE_IPTABLES_CURRENT_RULES_SCRIPT;
    static const QString RESTORE_IPTABLES_CURRENT_RULES_SCRIPT;

    explicit BootRulesetConfig(QObject *parent = 0);

    virtual QStringList getBootOptions();

    virtual bool setBootProcess(int bootMethod);

    // Clear the current firewall rules
    virtual void clearFirewallRules();

signals:

    void cmdResult(QString);

public slots:


protected:

    QPointer<IpProcess> proc;

    //virtual bool processBootDhcp(bool enable); // not supported
    virtual bool processBootInitD(bool enable);


private:


};

#endif // BOOTRULESETCONFIG_H
