#include "formdlgruleset.h"
#include "ui_formdlgruleset.h"

FormDlgRuleset::FormDlgRuleset(int opCode, FormCfgRuleset *parent) :
    QDialog(parent),
    ui(new Ui::FormDlgRuleset)
{
    this->opcode = opCode;
    formRuleset = parent;

    ui->setupUi(this);

    id = formRuleset->getColumnData("id").toInt();
    name = formRuleset->getColumnData("name").toString();
    rules = formRuleset->getColumnData("rules").toString();

    if (opcode == FormCfgRuleset::REC_DELETE)
    {
        ui->edtRuleset->setReadOnly(true);
        ui->edtRulesetName->setReadOnly(true);
        ui->btnSave->setText("&Delete");
        buttonsEnabled(true);
    }
    else
    {
        buttonsEnabled(false);
    }

    if (opcode == FormCfgRuleset::REC_ADD)
    {
        ui->btnSave->setText("&Create");
        ui->edtRulesetName->setText("");
        ui->edtRuleset->clear();
        ui->edtRuleset->appendPlainText("");
    }
    else
    {
        ui->edtRulesetName->setText(name);
        ui->edtRuleset->clear();
        ui->edtRuleset->appendPlainText(rules);
    }



    connect(ui->edtRulesetName, SIGNAL(textEdited(QString)),
            this, SLOT(dataChanged()));
    connect(ui->edtRuleset, SIGNAL(textChanged()),
            this, SLOT(dataChanged()));

}

void FormDlgRuleset::buttonsEnabled(bool enabled)
{
    ui->btnSave->setEnabled(enabled);
    ui->btnCancel->setEnabled(enabled);

}

void FormDlgRuleset::dataChanged()
{
    buttonsEnabled(true);
}

FormDlgRuleset::~FormDlgRuleset()
{
    delete ui;
}


void FormDlgRuleset::slotSave()
{
    accept();
}

void FormDlgRuleset::slotCancel()
{
    reject();
}
