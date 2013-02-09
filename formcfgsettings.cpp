
#include "formcfgsettings.h"
#include "ui_formcfgsettings.h"



FormCfgSettings::FormCfgSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCfgSettings)
{

    this->qry = 0;
    ui->setupUi(this);

    ui->btnCancel->setEnabled(false);
    ui->btnSave->setEnabled(false);

    loadSettings();

    connect(ui->edtShell, SIGNAL(textChanged(QString)),
            this, SLOT(slotButtonStateEnabled()));
    connect(ui->cbxDefRuleset, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotButtonStateEnabled()));
    connect(ui->btnCancel, SIGNAL(clicked()),
            SLOT(slotButtonStateDisabled()));


}

FormCfgSettings::~FormCfgSettings()
{
    delete ui;
}

void FormCfgSettings::slotButtonStateEnabled()
{
    qDebug("FormCfgSettings::slotButtonStateEnabled()");
    ui->btnCancel->setEnabled(true);
    ui->btnSave->setEnabled(true);
}

void FormCfgSettings::slotButtonStateDisabled()
{
    qDebug("FormCfgSettings::slotButtonStateDisabled()");
    ui->btnCancel->setEnabled(false);
    ui->btnSave->setEnabled(false);
}

void FormCfgSettings::slotFileDialog()
{
    QFileDialog fd(this);
    QString fileName;

    fd.setReadOnly(true);
    fd.setDirectory("/bin/bash");
    fd.setFileMode(QFileDialog::ExistingFile);
    if (fd.exec())
    {
        fileName = fd.selectedFiles().at(0);
        ui->edtShell->setText(fileName);
    }

    qDebug("FormConfigQiptables::slotFileDialog()\nselected file [%s]",
           fileName.toAscii().data());
    //QString fileName = QFileDialog::getOpenFileName(this,
    //    tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));
    //QString fileName = QFileDialog::getOpenFileName(this,
    //    tr("Select"), "/bin/bash");

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
    ui->edtShell->setText(shell);
    ui->cbxDefRuleset->setCurrentIndex(ui->cbxDefRuleset->findText(defaultRuleName));
}

QString FormCfgSettings::lastSqlErrorAsString()
{
    return qry->lastError().text();
}

void FormCfgSettings::loadSettings()
{

    qDebug("FormCfgSettings::loadSettings()");

    if (! qry)
    {
        qry = new QSqlQuery();
    }

    if (qry->exec("select id, shell, defaultRuleName from sysconf"))
    {
        if (qry->first())
        {
            id = qry->record().value("id").toInt();
            shell = qry->record().value("shell").toString();
            defaultRuleName = qry->record().value("defaultRuleName").toString();
            ui->edtShell->setText(shell);
        }
        else
        {
            qDebug("Query error on sysconf: [%s]", qry->lastError().text().toAscii().data());
        }
    }
    else
    {
        qDebug("Query error on sysconf: [%s]", qry->lastError().text().toAscii().data());
    }


    if (qry->exec("select id, name, rules from ruleset"))
    {
        while (qry->next())
        {
            ui->cbxDefRuleset->addItem(qry->record().value("name").toString(),
                                       qry->record().value("id").toInt());
        }
        ui->cbxDefRuleset->setCurrentIndex(ui->cbxDefRuleset->findText(defaultRuleName));
    }
    else
    {
        qDebug("Query error on ruleset: [%s]", qry->lastError().text().toAscii().data());
    }
}

void FormCfgSettings::saveSettings()
{
    if (! qry)
    {
        qry = new QSqlQuery();
    }

    qry->prepare("update sysconf set shell = :shell, defaultRuleName = :defaultRuleName");
    qry->bindValue(":shell", ui->edtShell->text());
    qry->bindValue(":defaultRuleName", ui->cbxDefRuleset->currentText());

    if (qry->exec())
    {
        qDebug("sysconf updated OK");
    }
    else
    {
        qDebug("Query error on sysconf update: [%s]", qry->lastError().text().toAscii().data());
    }
}
