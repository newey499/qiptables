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


    IpProcess *process = new IpProcess(this);
    process->setProcessEnvironment(IpProcess::systemEnvironment());
    QStringList sl = IpProcess::systemEnvironment().toStringList();

    ui->plainTextEdit->clear();
    for (int i = 0; i < sl.count(); i++)
    {
        ui->plainTextEdit->appendPlainText(sl.at(i));
    }

    ui->plainTextEdit->appendPlainText("============");

    LinuxUserId *luid = new LinuxUserId(this);
    QString tmp1 = QString("user id: %1").arg(luid->getUserId());
    QString tmp2 = QString("effective user id: %1").arg(luid->getEffectiveUserId());
    ui->plainTextEdit->appendPlainText(tmp1);
    ui->plainTextEdit->appendPlainText(tmp2);

    Iptables *iptables = new Iptables(this);
    ui->plainTextEdit->appendPlainText(iptables->list());
}



