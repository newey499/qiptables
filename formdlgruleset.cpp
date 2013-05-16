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
 \file formdlgruleset.cpp

 \brief Add/Amend/Delete Rulesets

*/

#include "genlib.h"
#include "formdlgruleset.h"
#include "ui_formdlgruleset.h"
#include "mainwindow.h"

FormDlgRuleset::FormDlgRuleset(int opCode, FormCfgRuleset *parent) :
    QDialog(parent),
    ui(new Ui::FormDlgRuleset)
{
    this->opcode = opCode;
    formRuleset = parent;

    ui->setupUi(this);

    formSnippets = new FormCfgRuleSnippets(true, this, Qt::Window);
    //formSnippets->enableDrag();
    formSnippets->enableContextMenu(true);
    connect(formSnippets, SIGNAL(addSnippet(bool, int, QString, QString)),
            this, SLOT(slotAddSnippet(bool, int, QString, QString)));


    id = formRuleset->getColumnData("id").toInt();
    name = formRuleset->getColumnData("name").toString();
    rules = formRuleset->getColumnData("rules").toString();

    if (opcode == FormCfgRuleset::REC_ADD)
    {
        ui->btnSave->setText("&Create");
        ui->edtRulesetName->setText("");
        ui->edtRuleset->clear();
        ui->edtRuleset->appendPlainText("");
        ui->btnAddSnippet->setVisible(true);
    }

    if (opcode == FormCfgRuleset::REC_EDIT)
    {
        ui->edtRulesetName->setText(name);
        ui->edtRuleset->clear();
        ui->edtRuleset->appendPlainText(rules);
        ui->btnAddSnippet->setVisible(true);
        // Cannot edit ruleset name if its the default held
        // on the sysconf table.
        if (isDefaultRuleset())
        {
            ui->edtRulesetName->setReadOnly(true);
        }
    }


    if (opcode == FormCfgRuleset::REC_DELETE)
    {
        ui->edtRulesetName->setReadOnly(true);
        ui->edtRuleset->setReadOnly(true);
        ui->edtRulesetName->setText(name);
        ui->edtRuleset->clear();
        ui->edtRuleset->appendPlainText(rules);
        ui->btnSave->setText("&Delete");
        ui->btnAddSnippet->setVisible(false);
        buttonsEnabled(true);
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
    if (formSnippets)
    {
        delete formSnippets;
    }
    delete ui;
}


void FormDlgRuleset::slotSave()
{
    if (validateData())
    {
        writeRow();
        accept();
        MainWindow::addFirewallMenuOptions();
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
    else if (rulesetName.trimmed().size() < 2)
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
            qry.prepare("select count(*) as count from ruleset "
                        "where id <> :id and upper(name) = upper(:name)");
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
                qDebug("%s\nExecuted Query [%s]",
                       qry.lastError().text().toAscii().data(),
                       qry.executedQuery().toAscii().data());
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
                    result = false;
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

    if (opcode == FormCfgRuleset::REC_ADD)
    {
        QSqlRecord rec = this->formRuleset->getModel()->record();
        rec.setValue("name", ui->edtRulesetName->text());
        rec.setValue("shortname", ui->edtRulesetName->text());
        rec.setValue("rules", ui->edtRuleset->toPlainText());

        // insert new row at end of model
        if (formRuleset->getModel()->insertRecord(-1, rec))
        {
            this->formRuleset->setUpdatedName(ui->edtRulesetName->text());
            result = true;
        }
        else
        {
            qDebug("2) Error writing new record [%s]",
                formRuleset->getModel()->lastError().text().toAscii().data());
            formRuleset->getModel()->revertAll();
        }
    }

    if (opcode == FormCfgRuleset::REC_EDIT)
    {
        int currentRow = this->formRuleset->getView()->currentRow();
        QSqlRecord rec = this->formRuleset->getModel()->record(currentRow);
        rec.setValue("name", ui->edtRulesetName->text());
        rec.setValue("shortname", ui->edtRulesetName->text());
        rec.setValue("rules", ui->edtRuleset->toPlainText());

        // update row
        if (formRuleset->getModel()->setRecord(currentRow, rec))
        {
            this->formRuleset->setUpdatedName(ui->edtRulesetName->text());
            result = true;
        }
        else
        {
            qDebug("4) Error writing new record [%s]",
                    formRuleset->getModel()->lastError().text().toAscii().data());
            formRuleset->getModel()->revertAll();
        }
    }

    if (opcode == FormCfgRuleset::REC_DELETE)
    {
        int currentRow = this->formRuleset->getView()->currentRow();

        if (QMessageBox::Yes == QMessageBox::question(this,
                                                      "Delete Ruleset",
                                                      "Confirm deletion of ruleset",
                                                      QMessageBox::Yes, QMessageBox::Cancel))
        {
            // delete row
            if (formRuleset->getModel()->removeRow(currentRow))
            {
                /****************
                if (! formRuleset->getModel()->submitAll())
                {
                    qDebug("5) Error writing new record [%s]",
                        formRuleset->getModel()->lastError().text().toAscii().data());
                }
                else
                {
                    result = true;
                }
                ***********************/
            }
        }
        else
        {
            qDebug("6) Error writing new record [%s]",
                formRuleset->getModel()->lastError().text().toAscii().data());
            formRuleset->getModel()->revertAll();
        }
    }

    return result;
}


void FormDlgRuleset::hideEvent(QHideEvent *event)
{
    if (formSnippets)
    {
        if (formSnippets->isVisible())
        {
            formSnippets->hide();
        }
    }

    event->ignore();
}


void FormDlgRuleset::slotLaunchFormRuleSnippets()
{
    formSnippets->show();
}


void FormDlgRuleset::slotAddSnippet(bool useInclude, int id, QString name, QString snippets)
{
    id = id;  // suppress compiler warning - compiler optimises this out

    QString include;

    if (useInclude)
    {
        include = GenLib::getIncludeString(name);
    }
    else
    {
        include = snippets;
    }

    ui->edtRuleset->appendPlainText(include);

}
