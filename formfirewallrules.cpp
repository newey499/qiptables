#include "formfirewallrules.h"
#include "ui_formfirewallrules.h"

FormFirewallRules::FormFirewallRules(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::FormFirewallRules)
{
    ui->setupUi(this);

    databaseManager = new DatabaseManager(Install::INSTALL_DIR, this);
    QStringList sl = databaseManager->getRulesetNames();
    for (int i = 0; i < sl.count(); i++)
    {
        ui->cbxFirewalls->addItem(sl.at(i));
    }
}

FormFirewallRules::~FormFirewallRules()
{
    delete ui;
}
