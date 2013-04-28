#include "genlib.h"
#include "formfirewallrules.h"

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
