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

    connect(iptables, SIGNAL(cmdOutput(QString, QStringList, int, QString)),
            this,    SLOT(slotCmdOutput(QString, QStringList, int, QString)));

}

FormTest::~FormTest()
{
    delete ui;
}


void FormTest::slotBtnTest()
{
    qDebug("FormTest::slotBtnTest()");

    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText("====================================");
    ui->plainTextEdit->appendPlainText("FormTest::slotBtnTest() : Start");
    ui->plainTextEdit->appendPlainText("====================================");

    iptables->processRuleset("bbb");

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
