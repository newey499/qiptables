#include "formfirewallrules.h"
#include "ui_formfirewallrules.h"

FormFirewallRules::FormFirewallRules(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::FormFirewallRules)
{
    ui->setupUi(this);

    connect(
            ui->cbxFirewalls, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotCbcFirewallsIndexChanged(int))
           );

    proc = new IpProcess(this);

    databaseManager = new DatabaseManager(Install::INSTALL_DIR, this);
    QStringList sl = databaseManager->getRulesetNames();

    ui->cbxFirewalls->addItem("Not running a Qiptables ruleset");
    for (int i = 0; i < sl.count(); i++)
    {
        ui->cbxFirewalls->addItem(sl.at(i));
    }

}

FormFirewallRules::~FormFirewallRules()
{
    delete ui;
}


void FormFirewallRules::showEvent(QShowEvent *event)
{
    showCurrentFirewallRules();
    QWidget::showEvent(event);
}

void FormFirewallRules::showCurrentFirewallRules()
{
    QString shortName = getCurrentFirewallShortName();
    QString rulesetName("");
    if (! shortName.isEmpty())
    {
        rulesetName = getRulesetNameFromShortName(shortName);
    }


    ui->edtCurrentRules->clear();
    ui->edtCurrentRules->appendPlainText("Current Rules");
    ui->edtCurrentRules->appendPlainText("=============");
    ui->edtCurrentRules->appendPlainText(proc->execCmdLine("iptables -L"));

    if (! rulesetName.isEmpty())
    {
        ui->edtCurrentRules->appendPlainText(QString("Ruleset short name [%1]").
                                             arg(shortName));
        ui->edtCurrentRules->appendPlainText(QString("Ruleset Name [%1]").
                                             arg(rulesetName));
        ui->edtCurrentFirewall->setText(rulesetName);
        int index = ui->cbxFirewalls->findText(rulesetName);
        if (index != -1)
        {
            ui->cbxFirewalls->setCurrentIndex(index);
        }
        else
        {
            ui->cbxFirewalls->setCurrentIndex(0);
        }
     }

}

QString FormFirewallRules::getCurrentFirewallShortName()
{
    QString result = proc->execCmdLine("/etc/qiptables/tools/get-firewall-name.sh");
    result = result.trimmed();
    return result;

}

QString FormFirewallRules::getRulesetNameFromShortName(QString shortName)
{
   QSqlRecord rec = getFirewallRulesetFromShortName(shortName);
   QString result = rec.value("name").toString();
   return result;
}

QSqlRecord FormFirewallRules::getFirewallRulesetFromShortName(QString shortName)
{
    QSqlRecord result = QSqlRecord();
    QSqlQuery qry;
    QVariant sn(shortName);

    qry.prepare(" select id, name, shortname, rules "
                " from ruleset "
                " where shortname = :shortname");
    qry.bindValue(":shortname", sn);

    if (qry.exec())
    {
        if (qry.first())
        {
            result = qry.record();
        }
        else
        {
            qDebug("1 FormFirewallRules::getFirewallRulesetFromShortName [%s]",
                   qry.lastError().text().toAscii().data());
        }

    }
    else
    {
        qDebug("2 FormFirewallRules::getFirewallRulesetFromShortName [%s]",
               qry.lastError().text().toAscii().data());
    }

    return result;
}

void FormFirewallRules::slotCurrentRules()
{
    showCurrentFirewallRules();
}

void FormFirewallRules::slotEnableRuleset()
{
    qDebug("FormFirewallRules::slotEnableRuleset()");
    if (ui->cbxFirewalls->currentIndex() == 0)
    {
        QMessageBox::warning(this, "Qiptables",
                             "A Ruleset is not selected",
                             QMessageBox::Ok,
                             QMessageBox::Ok);
    }

}


void FormFirewallRules::slotCbcFirewallsIndexChanged(int index)
{
    if (index == 0)
    {
        ui->btnEnableRuleset->setEnabled(false);
    }
    else
    {
        ui->btnEnableRuleset->setEnabled(true);
    }
}
