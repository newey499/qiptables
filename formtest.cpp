#include "formtest.h"
#include "ui_formtest.h"

FormTest::FormTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTest)
{
    ui->setupUi(this);
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

    //ui->plainTextEdit->appendPlainText("\n");

    /***************
    IpProcess *process = new IpProcess(this);
    process->setProcessEnvironment(IpProcess::systemEnvironment());
    QStringList sl = IpProcess::systemEnvironment().toStringList();

    for (int i = 0; i < sl.count(); i++)
    {
        ui->plainTextEdit->appendPlainText(sl.at(i));
    }

    LinuxUserId *luid = new LinuxUserId(this);
    QString tmp1 = QString("user id: %1").arg(luid->getUserId());
    QString tmp2 = QString("effective user id: %1").arg(luid->getEffectiveUserId());
    ui->plainTextEdit->appendPlainText(tmp1);
    ui->plainTextEdit->appendPlainText(tmp2);

    Iptables *iptables = new Iptables(this);
    ui->plainTextEdit->appendPlainText(iptables->listIptablesRules());
    *************************/

    QPointer<Iptables> iptables = new Iptables(this);
    QPointer<DatabaseManager> db = new DatabaseManager(Install::INSTALL_DIR, this);

    QSqlRecord rec = iptables->getRuleset("aaa");

    QStringList rulesList = iptables->getRulesetRows("aaa");

    ui->plainTextEdit->appendPlainText(rec.value("rules").toString());
    ui->plainTextEdit->appendPlainText("====================================");

    for (int i = 0; i < rulesList.count(); i++)
    {
        QString line = QString("%1 \t[%2]").
                arg(i).
                arg(rulesList.at(i));
        ui->plainTextEdit->appendPlainText(line);
    }

    ui->plainTextEdit->appendPlainText("====================================");

    QStringList tmp = iptables->stripBlankLine(rulesList);
    for (int i = 0; i < tmp.count(); i++)
    {
        QString line = QString("%1 \t[%2]").
                arg(i).
                arg(tmp.at(i));
        ui->plainTextEdit->appendPlainText(line);
    }

    ui->plainTextEdit->appendPlainText("====================================");

    tmp = iptables->stripComments(rulesList);
    for (int i = 0; i < tmp.count(); i++)
    {
        QString line = QString("%1 \t[%2]").
                arg(i).
                arg(tmp.at(i));
        ui->plainTextEdit->appendPlainText(line);
    }

    delete db;
    ui->plainTextEdit->appendPlainText("====================================");
    ui->plainTextEdit->appendPlainText("FormTest::slotBtnTest() : End");
    ui->plainTextEdit->appendPlainText("====================================");

}

void FormTest::slotBtnRun()
{
    qDebug("FormTest::slotBtnRun()");

    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText("FormTest::slotBtnRun()");
    ui->plainTextEdit->appendPlainText("====================================");

    IpProcess *proc = new IpProcess(this);

    /****************
    QStringList args = (QStringList() << "-l" );
    //QString result = proc->exec("ls", args);
    QString result = proc->executeSynchronous("ls", args);
    ui->plainTextEdit->appendPlainText(result);
    *********************/
    QString cmdLine = ui->edtCmd->text();
    ui->plainTextEdit->appendPlainText(proc->execCmdLine(cmdLine));
}



