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
\file formcfgsettings.cpp


\brief Maintain application configuration settings

***************************/

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

void FormCfgSettings::slotIptables()
{
    QFileDialog fd(this);
    QString fileName;

    fd.setReadOnly(true);
    fd.setDirectory("/sbin");
    fd.setFileMode(QFileDialog::ExistingFile);
    if (fd.exec())
    {
        fileName = fd.selectedFiles().at(0);
        ui->edtIptables->setText(fileName);
    }
}


void FormCfgSettings::slotShell()
{
    QFileDialog fd(this);
    QString fileName;

    fd.setReadOnly(true);
    fd.setDirectory("/bin");
    fd.setFileMode(QFileDialog::ExistingFile);
    if (fd.exec())
    {
        fileName = fd.selectedFiles().at(0);
        ui->edtShell->setText(fileName);
    }
}


void FormCfgSettings::slotTempDir()
{
    QFileDialog fd(this);
    QString fileName;

    fd.setReadOnly(true);
    fd.setDirectory("/tmp");
    fd.setFileMode(QFileDialog::Directory);
    fd.setOption(QFileDialog::ShowDirsOnly);
    if (fd.exec())
    {
        fileName = fd.selectedFiles().at(0);
        ui->edtTempDir->setText(fileName);
    }
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

    if (qry.exec("select id, shell, iptables, tempdir, defaultRuleName from sysconf"))
    {
        if (qry.first())
        {
            id = qry.record().value("id").toInt();
            shell = qry.record().value("shell").toString();
            iptables = qry.record().value("iptables").toString();
            tempdir = qry.record().value("tempdir").toString();
            defaultRuleName = qry.record().value("defaultRuleName").toString();
            ui->edtShell->setText(shell);
            ui->edtIptables->setText(iptables);
            ui->edtTempDir->setText(tempdir);
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
    qry.prepare("update sysconf set "
                "  iptables = :iptables, "
                "  shell = :shell, "
                "  tempdir = :tempdir, "
                "  defaultRuleName = :defaultRuleName ");
    qry.bindValue(":iptables", ui->edtIptables->text());
    qry.bindValue(":shell", ui->edtShell->text());
    qry.bindValue(":tempdir", ui->edtTempDir->text());
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
