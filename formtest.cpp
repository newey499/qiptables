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
 \file formtest.cpp


 \brief Form for various test routines used during development

*/

#include <QStatusBar>

#include "formtest.h"
#include "ui_formtest.h"
#include "genlib.h"

#include "mainwindow.h"

#include "threadwrapper.h"
#include "threadiptablesworkersubclass.h"

FormTest::FormTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTest)
{
    ui->setupUi(this);

    iptables = new Iptables(this);
    db = new DatabaseManager(Install::INSTALL_DIR, this);
    proc = new IpProcess(this);

    ruleSnippet = new RuleSnippet(this);

    ui->cbxRuleset->clear();
    ui->cbxRuleset->addItems(db->getRulesetNames());
    ui->cbxRuleset->setCurrentIndex(0);

    connect(iptables, SIGNAL(cmdOutput(QString, QStringList, int, QString)),
            this,    SLOT(slotCmdOutput(QString, QStringList, int, QString)));

}

FormTest::~FormTest()
{
    delete ui;
}


void FormTest::slotBtnTest()
{
    ui->plainTextEdit->clear();
    //ui->plainTextEdit->appendPlainText("====================================");
    //ui->plainTextEdit->appendPlainText("FormTest::slotBtnTest() : Start");
    //ui->plainTextEdit->appendPlainText("====================================");

    //iptables->processRuleset("aaa");
    //iptables->processRuleset("bbb");
    //QStringList sl = ruleSnippet->getRuleSnippetRows("test");
    //QStringList sl = ruleSnippet->getIncludeRuleSnippetRows("test");

    ui->plainTextEdit->appendPlainText("No test currently set up - nothing to report");


    //ui->plainTextEdit->appendPlainText("====================================");
    //ui->plainTextEdit->appendPlainText("FormTest::slotBtnTest() : End");
    //ui->plainTextEdit->appendPlainText("====================================");

}

void FormTest::slotBtnRun()
{
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText("FormTest::slotBtnRun()");
    ui->plainTextEdit->appendPlainText("====================================");

    QString cmdLine = ui->edtCmd->text();
    ui->plainTextEdit->appendPlainText(proc->execCmdLine(cmdLine));


}

void FormTest::slotCmdOutput(QString program, QStringList arguments, int exitCode, QString result)
{
    QString args = arguments.join(" ");

    // Suppress compiler warnings - the compiler optimizes these lines out
    exitCode = exitCode;
    result = result;

    QString tmp = QString("%1 %2\n").
            arg(program).
            arg(args);

    ui->plainTextEdit->appendPlainText(tmp);
}


void FormTest::slotRunRuleset()
{
    MainWindow *mainWindow = (MainWindow *) GenLib::getWidgetPointer("MainWindow");
    if (mainWindow)
    {
        mainWindow->statusBar()->showMessage("Please Wait - Activating Ruleset..........");
    }

    ThreadWrapper *wrapper = new ThreadWrapper(this);
    ThreadIptablesWorkerSubClass *worker =
        new ThreadIptablesWorkerSubClass(ThreadIptablesWorkerSubClass::SET_CURRENT_RULESET,
                                         ui->cbxRuleset->currentText());

    connect(worker, SIGNAL(sigCurrentRules(QString)),
            this, SLOT(slotDisplayString(QString)));

    wrapper->run(worker);

    // Display the firewall rules after the selected ruleset has been run
    slotIptablesList();

    if (mainWindow)
    {
        mainWindow->statusBar()->clearMessage();
    }

}


void FormTest::slotIptablesList()
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


void FormTest::slotDisplayString(QString msg)
{
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(msg);
}



void FormTest::fillCbxRulesetNames()
{
    QStringList sl = db->getRulesetNames();

    ui->cbxRuleset->clear();
    ui->cbxRuleset->addItem("Not running a Qiptables ruleset");
    for (int i = 0; i < sl.count(); i++)
    {
        ui->cbxRuleset->addItem(sl.at(i));
    }
}


void FormTest::showEvent(QShowEvent *event)
{
    static int counter =1;
    // disable compiler warning for unused parameter - gcc optimizes out this code
    event = event;
    int index = 0;

    // Ensure ruleset names are up to date
    fillCbxRulesetNames();

    index = ui->cbxRuleset->findText(GenLib::getRulesetNameFromShortName());
    if (index == -1)
    {
        index = 0;
    }
    ui->cbxRuleset->setCurrentIndex(index);

    slotIptablesList();

    qDebug("\n");
    qDebug("==============================================");
    qDebug("FormTest::showEvent(QShowEvent *event) [%d]", counter++);
    qDebug("Short Name [%s]", GenLib::getCurrentFirewallShortName().toAscii().data());
    qDebug("Ruleset Name [%s]", GenLib::getRulesetNameFromShortName().toAscii().data());
    qDebug("==============================================");

}
