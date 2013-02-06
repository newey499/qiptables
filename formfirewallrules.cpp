#include "formfirewallrules.h"
#include "ui_formfirewallrules.h"

FormFirewallRules::FormFirewallRules(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFirewallRules)
{
    ui->setupUi(this);
}

FormFirewallRules::~FormFirewallRules()
{
    delete ui;
}
