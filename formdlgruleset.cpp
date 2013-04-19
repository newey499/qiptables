

/************************************************************************
Copyright Chris Newey 2013

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



#include "formdlgruleset.h"
#include "ui_formdlgruleset.h"

/**
 \file formdlgruleset.cpp

 \todo Data validation

 \todo Insert, update and delete sql

 \todo Investigate and implement a mechanism for validating firewall rules

 \todo If ruleset being edited is the default ruleset held on the sysconf table
 then the validation needs to make the ruleset name edit box readonly.


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
        ui->edtRulesetName->setReadOnly(true);
        ui->edtRuleset->setReadOnly(true);
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
        // Cannot edit ruleset name if its the default held
        // on the sysconf table.
        if (isDefaultRuleset())
        {
            ui->edtRulesetName->setReadOnly(true);
        }
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

bool FormDlgRuleset::isDefaultRuleset()
{
    bool result = false;

    QSqlQuery qry;
    qry.prepare(" select count(*) as count "
                " from sysconf "
                " where defaultRuleName = :defaultRuleName");
    qry.bindValue(":defaultRuleName", name);

    if (qry.exec())
    {
        if (qry.first())
        {
            if (qry.record().value("count").toInt() > 0)
            {
                result = true;
            }

        }
        else
        {
            qDebug("query on FormDlgRuleset::isDefaultRuleset() failed to find first row \n[%s]",
                   qry.lastError().text().toAscii().data());
        }

    }
    else
    {
        qDebug("query on FormDlgRuleset::isDefaultRuleset() failed \n[%s]",
               qry.lastError().text().toAscii().data());
    }

    return result;
}


bool FormDlgRuleset::validateData()
{
    bool result = true;
    QString errMsg("");
    QString rulesetName = ui->edtRulesetName->text();
    QString ruleset = ui->edtRuleset->toPlainText();

    // Cannot delete a ruleset if it is in use as the default ruleset
    if (result && opcode == FormCfgRuleset::REC_DELETE)
    {
        if (isDefaultRuleset())
        {
            errMsg = errMsg.append("%1Ruleset Name [%2] is in use as the default."
                          " - not including leading and trailing spaces.").
                    arg((result ? "" : "\n")).arg(rulesetName);
            result = false;
        }
    }

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
            formRuleset->getModel()->submitAll();
            result = true;
        }
        else
        {
            qDebug("row not inserted");
            formRuleset->getModel()->revertAll();
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
            formRuleset->getModel()->submitAll();
            result = true;
        }
        else
        {
            qDebug("row not updated - setRecord call failed");
            formRuleset->getModel()->revertAll();
        }
    }

    if (opcode == FormCfgRuleset::REC_DELETE)
    {
        int currentRow = this->formRuleset->getView()->currentRow();

        // delete row
        if (formRuleset->getModel()->removeRow(currentRow))
        {
            qDebug("row delete submitAll() Ok");
            formRuleset->getModel()->submitAll();
            result = true;
        }
        else
        {
            qDebug("row not deleted - removeRow call failed");
            formRuleset->getModel()->revertAll();
        }
    }

    return result;
}
