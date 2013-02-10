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
    }

    if (opcode == FormCfgRuleset::REC_ADD)
    {
        ui->btnSave->setText("&Create");
    }

    ui->edtRulesetName->setText(name);
    ui->edtRuleset->clear();
    ui->edtRuleset->appendPlainText(rules);
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
