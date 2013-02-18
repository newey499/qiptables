#include "formtest.h"
#include "ui_formtest.h"

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
    ui->plainTextEdit->appendPlainText("====================================");
    ui->plainTextEdit->appendPlainText("FormTest::slotBtnTest() : Start");
    ui->plainTextEdit->appendPlainText("====================================");

    //iptables->processRuleset("aaa");
    //iptables->processRuleset("bbb");
    //QStringList sl = ruleSnippet->getRuleSnippetRows("test");
    //QStringList sl = ruleSnippet->getIncludeRuleSnippetRows("test");

    ui->plainTextEdit->appendPlainText("No test currently set up - nothing to report");


    ui->plainTextEdit->appendPlainText("====================================");
    ui->plainTextEdit->appendPlainText("FormTest::slotBtnTest() : End");
    ui->plainTextEdit->appendPlainText("====================================");

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
    QString tmp = QString("%1 args [%2] returned [%3]\n"
                          "=============================\n"
                          "%4\n"
                          "=============================\n"
                          ).
            arg(program).
            arg(args).
            arg(exitCode).
            arg(result);
    ui->plainTextEdit->appendPlainText(tmp);
}


void FormTest::slotRunRuleset()
{
    //qDebug("void FormTest::slotRunRuleset()");

    ui->plainTextEdit->clear();
    /**************
    ui->plainTextEdit->appendPlainText("====================================");
    ui->plainTextEdit->appendPlainText("FormTest::slotRunRuleset() : Start");
    ui->plainTextEdit->appendPlainText("====================================");
    *********************/

    QString ruleset = ui->cbxRuleset->currentText();
    iptables->processRuleset(ruleset);

    /**************
    ui->plainTextEdit->appendPlainText("====================================");
    ui->plainTextEdit->appendPlainText("FormTest::slotRunRuleset() : End");
    ui->plainTextEdit->appendPlainText("====================================");
    ********************/
}


void FormTest::slotIptablesList()
{
    qDebug("FormTest::slotIptablesList()");
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText("FormTest::slotIptablesList()");
    ui->plainTextEdit->appendPlainText("====================================");

    QString cmdLine = ui->edtCmd->text();
    ui->plainTextEdit->appendPlainText(proc->execCmdLine("iptables -L"));
}
