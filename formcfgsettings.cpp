
#include "formcfgsettings.h"
#include "ui_formcfgsettings.h"



FormCfgSettings::FormCfgSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCfgSettings)
{

    ui->setupUi(this);

    ui->edtIptables->setReadOnly(true);

    loadSettings();

    connect(ui->edtIptables, SIGNAL(textChanged(QString)),
            this, SLOT(slotButtonStateEnabled()));
    connect(ui->cbxDefRuleset, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotButtonStateEnabled()));
    connect(ui->btnCancel, SIGNAL(clicked()),
            SLOT(slotButtonStateDisabled()));

    slotButtonStateDisabled();
}

FormCfgSettings::~FormCfgSettings()
{
    delete ui;
}

void FormCfgSettings::slotButtonStateEnabled()
{
    //qDebug("FormCfgSettings::slotButtonStateEnabled()");
    ui->btnCancel->setEnabled(true);
    ui->btnSave->setEnabled(true);
}

void FormCfgSettings::slotButtonStateDisabled()
{
    //qDebug("FormCfgSettings::slotButtonStateDisabled()");
    ui->btnCancel->setEnabled(true);
    ui->btnSave->setEnabled(false);
}

void FormCfgSettings::slotFileDialog()
{
    QFileDialog fd(this);
    QString fileName;

    fd.setReadOnly(true);
    fd.setDirectory("/sbin/iptables");
    fd.setFileMode(QFileDialog::ExistingFile);
    if (fd.exec())
    {
        fileName = fd.selectedFiles().at(0);
        ui->edtIptables->setText(fileName);
    }

    qDebug("FormConfigQiptables::slotFileDialog()\nselected file [%s]",
           fileName.toAscii().data());

}


void FormCfgSettings::slotSave()
{
    qDebug("FormConfigQiptables::slotSave()");
    slotButtonStateDisabled();
    saveSettings();
    loadSettings();
}


void FormCfgSettings::slotCancel()
{
    qDebug("FormConfigQiptables::slotCancel()");
    this->loadSettings();
    //ui->edtIptables->setText(shell);
    //ui->cbxDefRuleset->setCurrentIndex(ui->cbxDefRuleset->findText(defaultRuleName));
    slotButtonStateDisabled();
}

QString FormCfgSettings::lastSqlErrorAsString()
{
    return qry.lastError().text();
}

void FormCfgSettings::loadSettings()
{

    //qDebug("FormCfgSettings::loadSettings()");

    if (qry.exec("select id, shell, iptables, defaultRuleName from sysconf"))
    {
        if (qry.first())
        {
            id = qry.record().value("id").toInt();
            shell = qry.record().value("shell").toString();
            iptables = qry.record().value("iptables").toString();
            defaultRuleName = qry.record().value("defaultRuleName").toString();
            ui->edtIptables->setText(iptables);
        }
        else
        {
            qDebug("Query error on sysconf: [%s]", qry.lastError().text().toAscii().data());
        }
    }
    else
    {
        qDebug("Query error on sysconf: [%s]", qry.lastError().text().toAscii().data());
    }

    ui->cbxDefRuleset->clear();
    if (qry.exec("select id, name, rules from ruleset"))
    {
        while (qry.next())
        {
            ui->cbxDefRuleset->addItem(qry.record().value("name").toString(),
                                       qry.record().value("id").toInt());
        }
        ui->cbxDefRuleset->setCurrentIndex(ui->cbxDefRuleset->findText(defaultRuleName));
    }
    else
    {
        qDebug("Query error on ruleset: [%s]", qry.lastError().text().toAscii().data());
    }
}

void FormCfgSettings::saveSettings()
{
    qry.prepare("update sysconf set iptables = :iptables, defaultRuleName = :defaultRuleName");
    qry.bindValue(":iptables", ui->edtIptables->text());
    qry.bindValue(":defaultRuleName", ui->cbxDefRuleset->currentText());

    if (qry.exec())
    {
        qDebug("sysconf updated OK");
        slotButtonStateDisabled();
    }
    else
    {
        qDebug("Query error on sysconf update: [%s]", qry.lastError().text().toAscii().data());
    }
}
