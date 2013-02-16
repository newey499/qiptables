#include "formfirewallrules.h"
#include "ui_formfirewallrules.h"

FormFirewallRules::FormFirewallRules(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::FormFirewallRules)
{
    ui->setupUi(this);
}

FormFirewallRules::~FormFirewallRules()
{
    delete ui;
}
