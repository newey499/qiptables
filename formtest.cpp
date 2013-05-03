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

    /***********
    QString tmp = QString("%1 args [%2] returned [%3]\n"
                          "=============================\n"
                          "%4\n"
                          "=============================\n"
                          ).
            arg(program).
            arg(args).
            arg(exitCode).
            arg(result);
    *******************/
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

    ui->plainTextEdit->clear();
    /**************
    ui->plainTextEdit->appendPlainText("====================================");
    ui->plainTextEdit->appendPlainText("FormTest::slotRunRuleset() : Start");
    ui->plainTextEdit->appendPlainText("====================================");
    *********************/

    //QString ruleset = ui->cbxRuleset->currentText();
    //iptables->processRuleset(ruleset);

    QString shortName = iptables->getRulesetShortName(ui->cbxRuleset->currentText());

    ui->plainTextEdit->appendPlainText("===========================================");
    ui->plainTextEdit->appendPlainText("Executing Commands to set firewall rules");
    ui->plainTextEdit->appendPlainText("===========================================");
    iptables->processRuleset(ui->cbxRuleset->currentText());

    // create a new empty chain with the short name of the Ruleset
    QString tmp = shortName;
    tmp.prepend("iptables -N ");
    proc->execCmdLine(tmp);

    // Display the firewall rules after the selected ruleset has been run
    ui->plainTextEdit->appendPlainText("===========================================");
    ui->plainTextEdit->appendPlainText("Firewall Rules");
    ui->plainTextEdit->appendPlainText("===========================================");
    ui->plainTextEdit->appendPlainText(proc->execCmdLine("iptables -L"));

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

    ui->plainTextEdit->clear();
    //ui->plainTextEdit->appendPlainText("FormTest::slotIptablesList()");
    //ui->plainTextEdit->appendPlainText("====================================");

    QString cmdLine = ui->edtCmd->text();
    ui->plainTextEdit->appendPlainText(proc->execCmdLine("iptables -L"));

    if (mainWindow)
    {
        mainWindow->statusBar()->clearMessage();
    }
}
