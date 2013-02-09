#include "formconfigqiptables.h"
#include "ui_formconfigqiptables.h"



FormConfigQiptables::FormConfigQiptables(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormConfigQiptables)
{
    ui->setupUi(this);

    // Clear any existing tabs
    while (ui->tabWidget->count() > 0)
    {
        ui->tabWidget->removeTab(0);
    }


    qDebug("Add rulesets page to tabbed widget");
    formCfgRuleset = new FormCfgRuleset(this);
    ui->tabWidget->addTab(formCfgRuleset, "Rule Sets");

    qDebug("Add rule snippets page to tabbed widget");
    formCfgRuleSnippets = new FormCfgRuleSnippets(this);
    ui->tabWidget->addTab(formCfgRuleSnippets, "Rule Snippets");

    qDebug("Add settings page to tabbed widget");
    formCfgSettings = new FormCfgSettings(this);
    ui->tabWidget->addTab(formCfgSettings, "Settings");
}

FormConfigQiptables::~FormConfigQiptables()
{
    delete ui;
}





