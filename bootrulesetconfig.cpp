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


#include "bootrulesetconfig.h"

/***********
const int BootRulesetConfig::BOOT_NONE   = 0;
const int BootRulesetConfig::BOOT_INIT_D = 1;
****************/

const QString BootRulesetConfig::BOOT_INIT_D_SHELL_SCRIPT =
                    QString("qiptables.sh");
const QString BootRulesetConfig::BOOT_INIT_D_RULESET =
                    QString("qiptables-default-ruleset.txt");
const QString BootRulesetConfig::CLEAR_FIREWALL_RULES_SCRIPT =
                    QString("clearFirewall.sh");
const QString BootRulesetConfig::SAVE_IPTABLES_CURRENT_RULES_SCRIPT =
                    QString("saveiptables.sh");
const QString BootRulesetConfig::RESTORE_IPTABLES_CURRENT_RULES_SCRIPT =
                    QString("restoreiptables.sh");

BootRulesetConfig::BootRulesetConfig(QObject *parent) :
    QObject(parent)
{
    proc = new IpProcess(this);
}


QStringList BootRulesetConfig::getBootOptions()
{
    QStringList options;

    options.append("Do not start at boot");
    options.append("init.d start");

    return options;
}


bool BootRulesetConfig::setBootProcess(int bootMethod)
{
    bool result = true;

    qDebug("BootRulesetConfig::setBootProcess(int bootMethod)");

    if (bootMethod == BOOT_NONE)
    {
        qDebug("BOOT_NONE = Enable");
        processBootInitD(false);
    }

    if (bootMethod == BOOT_INIT_D)
    {
        qDebug("BOOT_INIT_D = Enable");
        // Note disable before enable
        processBootInitD(true);
    }

    return result;
}



bool BootRulesetConfig::processBootInitD(bool enable)
{
    bool result = true;
    QString rulesetName = GenLib::cleanFileName(Install::TMP_DIR,
                                                BootRulesetConfig::BOOT_INIT_D_RULESET);
    QString initScriptName = BootRulesetConfig::BOOT_INIT_D_SHELL_SCRIPT;
    QString fromPath = Install::TOOLS_DIR;
    QString toPath = "/etc/init.d";
    QString cmd;

    qDebug("BootRulesetConfig::processBootInitD(bool enable) [%d]", enable);

    if (enable)
    {
        // Clear the current firewall rules
        clearFirewallRules();

        // write out current ruleset file used by iptables-restore
        cmd = GenLib::cleanFileName(Install::TOOLS_DIR,
                                    SAVE_IPTABLES_CURRENT_RULES_SCRIPT);
        proc->execCmdLine(cmd);
        qDebug("%s", cmd.toAscii().data());

        // copy shell script to init.d directory
        cmd = QString("cp %1 %2").
                arg(GenLib::cleanFileName(fromPath, initScriptName)).
                arg(GenLib::cleanFileName(toPath, initScriptName));

        proc->execCmdLine(cmd);
        qDebug("%s", cmd.toAscii().data());

        // to be tidy
        // Delete the runtime start up link files
        cmd = QString("update-rc.d -f %1 remove").
                arg(initScriptName);
        proc->execCmdLine(cmd);
        qDebug("%s", cmd.toAscii().data());

        // now create new links
        // run update-rc.d
        cmd = QString("update-rc.d %1 defaults").
                arg(initScriptName);
        proc->execCmdLine(cmd);
        qDebug("%s", cmd.toAscii().data());
    }
    else
    {
        // Clear the current firewall rules
        // Disabling the boot process should not change the current firewall
        //clearFirewallRules();

        // Delete ruleset file used by iptables-restore
        QString cmd = QString("rm -rf %1").arg(rulesetName);
        proc->execCmdLine(cmd);
        qDebug("%s", cmd.toAscii().data());

        // Delete the runtime start up link files
        cmd = QString("update-rc.d -f %1 remove").
                arg(initScriptName);
        proc->execCmdLine(cmd);
        qDebug("%s", cmd.toAscii().data());

        // Delete the shell script
        cmd = QString("rm %1").
                arg(GenLib::cleanFileName(toPath, initScriptName));
        proc->execCmdLine(cmd);
        qDebug("%s", cmd.toAscii().data());
    }

    return result;
}


// Clear the current firewall rules
void BootRulesetConfig::clearFirewallRules()
{
    QString cmd = BootRulesetConfig::CLEAR_FIREWALL_RULES_SCRIPT;
    proc->execCmdLine(cmd);
}
