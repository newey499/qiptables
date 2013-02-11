#include "formdlgruleset.h"
#include "ui_formdlgruleset.h"

/**
 \file formdlgruleset.cpp

 \todo Data validation

 \todo Insert, update and delete sql

 \todo Investigate and implement a mechanism for validating firewall rules


iptables

DIAGNOSTICS
       Various error messages are printed to standard error.  The exit code is
       0 for correct functioning.  Errors which appear to be caused by invalid
       or  abused  command  line parameters cause an exit code of 2, and other
       errors cause an exit code of 1.

Perhaps load the ruleset into a stringlist, trim each line and pass
each line that starts with "iptables " (note trailing space) to iptables
and check return code.

Would have to save and restore current firewall.

machine firewall would be knackered during the test - must think
this through <g>.



*/

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
    ui->btnCancel->setEnabled(true); // always leave the cancel button enabled

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
    if (validateData())
    {
        writeRow();
        accept();
    }
}

void FormDlgRuleset::slotCancel()
{
    reject();
}


bool FormDlgRuleset::validateData()
{
    bool result = true;
    QString errMsg("");
    QString rulesetName = ui->edtRulesetName->text();
    QString ruleset = ui->edtRuleset->toPlainText();

    // check rulesetName has something in it
    if (rulesetName.isEmpty())
    {
        errMsg = errMsg.append("%1Ruleset Name is empty").
                arg((result ? "" : "\n"));
        result = false;
    }
    else if (rulesetName.trimmed().size() <= 2)
    {
        errMsg = errMsg.append("%1Ruleset Name must be at least two characters"
                      " - not including leading and trailing spaces.").
                arg((result ? "" : "\n"));
        result = false;
    }

    // Editing an existing record
    // If ok so far make a case insensitive check that a ruleset with
    // the entered name does not already exist
    if (result && opcode == FormCfgRuleset::REC_EDIT)
    {
        if (result)
        {
            QSqlQuery qry;
            qry.prepare(" select count(*) as count from ruleset"
                        " where id <> :id and upper(name) = upper(:name)");
            qry.bindValue(":id", this->id);
            qry.bindValue(":name", ui->edtRulesetName->text());

            if (qry.exec())
            {
                if (qry.first())
                {
                    if (qry.record().value("count").toInt() > 0 )
                    {
                        errMsg = errMsg.append("%1%2").
                                arg((result ? "" : "\n")).
                                arg("This ruleset name already exists\n(case insensitive check).");
                        result = false;
                    }
                }
                else
                {
                    qDebug("first row not returned from query\n%s",
                           qry.lastError().text().toAscii().data());
                }

            }
            else
            {
                errMsg = errMsg.append("%1%2").
                        arg((result ? "" : "\n")).
                        arg(qry.lastError().text());
                qDebug("%s",qry.lastError().text().toAscii().data());
                result = false;
            }

            }
        }


    // Editing a new record
    // If ok so far make a case insensitive check that a ruleset with
    // the entered name does not already exist
    if (result && opcode == FormCfgRuleset::REC_ADD)
    {
        if (result)
        {
            QSqlQuery qry;
            qry.prepare(" select count(*) as count from ruleset"
                        " where upper(name) = upper(:name)");
            qry.bindValue(":name", ui->edtRulesetName->text());

            if (qry.exec())
            {
                if (qry.first())
                {
                    if (qry.record().value("count").toInt() > 0 )
                    {
                        errMsg = errMsg.append("%1%2").
                                arg((result ? "" : "\n")).
                                arg("This ruleset name already exists\n(case insensitive check).");
                        result = false;
                    }
                }
                else
                {
                    qDebug("first row not returned from query\n%s",
                           qry.lastError().text().toAscii().data());
                }

            }
            else
            {
                errMsg = errMsg.append("%1%2").
                        arg((result ? "" : "\n")).
                        arg(qry.lastError().text());
                qDebug("%s",qry.lastError().text().toAscii().data());
                result = false;
            }

            }
        }

    // Check ruleset has something in it
    if (ruleset.trimmed().isEmpty())
    {
        errMsg = errMsg.append("%1Ruleset is empty").
                arg((result ? "" : "\n"));
        result = false;
    }

    if (! result)
    {
        QMessageBox::information(this, "Data Validation Error", errMsg);
    }

    return result;
}


bool FormDlgRuleset::writeRow()
{
    bool result = false;
    QString qryStr;

    qDebug("FormDlgRuleset::writeRow()");

    if (opcode == FormCfgRuleset::REC_ADD)
    {
        QSqlRecord rec = this->formRuleset->getModel()->record();
        rec.setValue("name", ui->edtRulesetName->text());
        rec.setValue("rules", ui->edtRuleset->toPlainText());

        // insert new row at end of model
        if (formRuleset->getModel()->insertRecord(-1, rec))
        {
            qDebug("row inserted");
        }
        else
        {
            qDebug("row not inserted");
        }
    }

    if (opcode == FormCfgRuleset::REC_EDIT)
    {
        int currentRow = this->formRuleset->getView()->currentRow();
        QSqlRecord rec = this->formRuleset->getModel()->record(currentRow);
        rec.setValue("name", ui->edtRulesetName->text());
        rec.setValue("rules", ui->edtRuleset->toPlainText());

        // update row
        if (formRuleset->getModel()->setRecord(currentRow, rec))
        {
            qDebug("row update write submitAll() Ok");
        }
        else
        {
            qDebug("row not updated - setRecord call failed");
        }
    }

    if (opcode == FormCfgRuleset::REC_DELETE)
    {
        int currentRow = this->formRuleset->getView()->currentRow();

        // delete row
        if (formRuleset->getModel()->removeRow(currentRow))
        {
            qDebug("row delete submitAll() Ok");
        }
        else
        {
            qDebug("row not deleted - removeRow call failed");
        }
    }

    return result;
}
