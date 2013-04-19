

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





