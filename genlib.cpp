#include "genlib.h"
#include "formfirewallrules.h"
#include "mainwindow.h"

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
    FormFirewallRules * fr = new FormFirewallRules(0);
    QString shortName = fr->getCurrentFirewallShortName();
    QString rulesetName("Qiptables ruleset not in use");
    if (! shortName.isEmpty())
    {
        rulesetName = fr->getRulesetNameFromShortName(shortName);
    }

    return rulesetName;
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
