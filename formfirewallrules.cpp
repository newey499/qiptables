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

/**
 \file formfirewallrules.cpp

 \brief Get Ruleset from application database

*/

#include "formfirewallrules.h"
#include "ui_formfirewallrules.h"

#include "genlib.h"
#include "mainwindow.h"
#include "threadwrapper.h"
#include "threadiptablesworkersubclass.h"


FormFirewallRules::FormFirewallRules(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::FormFirewallRules)
{
    ui->setupUi(this);

    ui->btnCurrentRules->setToolTip("Display iptables rules currently operating on this computer");
    ui->btnEnableRuleset->setToolTip("Overwrite current firewall rules with selected Ruleset");

    connect(
            ui->cbxFirewalls, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotCbxFirewallsIndexChanged(int))
           );

    proc = new IpProcess(this);
    ipTables = new Iptables(this);

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
    //showCurrentFirewallRules();
    QWidget::showEvent(event);
    qDebug("FormFirewallRules::showEvent(QShowEvent *event) Called - WHY???");
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
            qDebug("FormFirewallRules::getFirewallRulesetFromShortName Can't move to first row [%s]",
                   qry.lastError().text().toAscii().data());
        }

    }
    else
    {
        qDebug("FormFirewallRules::getFirewallRulesetFromShortName query failed [%s]",
               qry.lastError().text().toAscii().data());
    }

    return result;
}

void FormFirewallRules::slotCurrentRules()
{
    MainWindow *mainWindow = (MainWindow *) GenLib::getWidgetPointer("MainWindow");
    if (mainWindow)
    {
        mainWindow->statusBar()->showMessage("Please Wait - Obtaining Ruleset from iptables..........");
    }

    ThreadWrapper *wrapper = new ThreadWrapper(this);
    ThreadIptablesWorkerSubClass *worker =
            new ThreadIptablesWorkerSubClass(ThreadIptablesWorkerSubClass::GET_CURRENT_RULESET);

    connect(worker, SIGNAL(sigCurrentRules(QString)),
            this, SLOT(slotDisplayString(QString)));

    wrapper->run(worker);

    if (mainWindow)
    {
        mainWindow->statusBar()->clearMessage();
    }


}


void FormFirewallRules::slotDisplayString(QString msg)
{
    qDebug("FormFirewallRules::slotDisplayString()");

    ui->edtCurrentRules->clear();
    ui->edtCurrentRules->appendPlainText(msg);
}

void FormFirewallRules::slotCbxFirewallsIndexChanged(int index)
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


void FormFirewallRules::slotEnableRuleset()
{
    if (ui->cbxFirewalls->currentIndex() == 0)
    {
        QMessageBox::warning(this, "Qiptables",
                             "A Ruleset is not selected",
                             QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    MainWindow *mainWindow = (MainWindow *) GenLib::getWidgetPointer("MainWindow");
    if (mainWindow)
    {
        mainWindow->statusBar()->showMessage("Please Wait - Activating Ruleset..........");
    }

    ThreadWrapper *wrapper = new ThreadWrapper(this);
    ThreadIptablesWorkerSubClass *worker =
        new ThreadIptablesWorkerSubClass(ThreadIptablesWorkerSubClass::SET_CURRENT_RULESET,
                                         ui->cbxFirewalls->currentText());

    connect(worker, SIGNAL(sigCurrentRules(QString)),
            this, SLOT(slotDisplayString(QString)));

    wrapper->run(worker);

    // Display the firewall rules after the selected ruleset has been run
    slotCurrentRules();

    if (mainWindow)
    {
        mainWindow->statusBar()->clearMessage();
    }

}




