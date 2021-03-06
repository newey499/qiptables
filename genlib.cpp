#include "genlib.h"
#include "formfirewallrules.h"
#include "mainwindow.h"
#include "ipprocess.h"

#include "threadwrapper.h"
#include "threadiptablesworkersubclass.h"

GenLib::GenLib(QObject *parent) :
    QObject(parent)
{
}

GenLib::~GenLib()
{
}

QString GenLib::getIncludeString(QString snippetName)
{
    QString result = QString("\n#include %1").arg(snippetName);

    return result.trimmed();
}


QString GenLib::getRulesetName()
{
    FormFirewallRules fr;
    QString shortName = fr.getCurrentFirewallShortName();
    QString rulesetName("");
    if (shortName.isEmpty())
    {
        rulesetName = "";
    }
    else
    {
        rulesetName = fr.getRulesetNameFromShortName(shortName);
    }

    return rulesetName;
}


QString GenLib::getCurrentFirewallShortName()
{
    IpProcess proc;
    QString result = proc.execCmdLine("/etc/qiptables/tools/get-firewall-name.sh");
    result = result.trimmed();
    return result;
}


QString GenLib::getRulesetNameFromShortName(QString shortname)
{
    FormFirewallRules fr;

    return fr.getRulesetNameFromShortName(shortname);

}


QWidget * GenLib::getWidgetPointer(QString widgetObjectName)
{
    foreach (QWidget *widget, QApplication::topLevelWidgets())
    {
        if (widget->objectName().compare(widgetObjectName, Qt::CaseSensitive) == 0)
        {
            return widget;
        }
    }

    return 0; // Widget not found - return null pointer
}


QString GenLib::getRulesetShortName(QString rulesetLongName)
{
    QString result = rulesetLongName;

    result = result.left(Install::IPTABLES_CHAIN_MAX_NAME_LENGTH - 2);
    result = result.toUpper();
    result = result.trimmed();
    result = result.replace(" ", "_");
    result = result.replace("-", "");
    result = result.prepend(Install::IPTABLES_CHAIN_NAME_PREFIX);

    return result;
}


void GenLib::clearAllFirewallRules()
{
    qDebug("GenLib::clearAllFirewallRules()");
    QPointer<IpProcess> ipProcess = new IpProcess();

    ipProcess->execCmdLine("/etc/qiptables/tools/clearFirewall.sh");

    if (ipProcess)
    {
        delete ipProcess;
    }
}


QStringList GenLib::getGnuLicence()
{
    QStringList licence;

    licence.append("");
    licence.append("");
    licence.append("##########################################################################");
    licence.append("# Copyright Chris Newey 2013");
    licence.append("# ");
    licence.append("# qiptables@hotmail.com");
    licence.append("# ");
    licence.append("# This file is part of Qiptables.");
    licence.append("# ");
    licence.append("# Qiptables is distributed under the terms of the GNU General Public License");
    licence.append("# ");
    licence.append("# Qiptables is free software: you can redistribute it and/or modify");
    licence.append("# it under the terms of the GNU General Public License as published by");
    licence.append("# the Free Software Foundation, either version 3 of the License, or");
    licence.append("# (at your option) any later version.");
    licence.append("# ");
    licence.append("# Qiptables is distributed in the hope that it will be useful,");
    licence.append("# but WITHOUT ANY WARRANTY; without even the implied warranty of");
    licence.append("# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the");
    licence.append("# GNU General Public License for more details.");
    licence.append("# ");
    licence.append("# You should have received a copy of the GNU General Public License");
    licence.append("# along with Qiptables.  If not, see <http://www.gnu.org/licenses/>.");
    licence.append("# ");
    licence.append("##########################################################################");
    licence.append("");
    licence.append("");


    return licence;
}


QString GenLib::cleanFileName(QString path, QString filename)
{
    QString fname = QString("%1%2%3").
            arg(path).
            arg(QDir::separator()).
            arg(filename);

    fname = QDir::cleanPath(fname);
    return fname;
}
