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
 \file formdlgrulesnippet.cpp

 \brief Add/Amend/Delete Rulesnippets

*/

#include <QToolTip>

#include "formdlgrulesnippet.h"
#include "ui_formdlgrulesnippet.h"

FormDlgRuleSnippet::FormDlgRuleSnippet(int opCode, FormCfgRuleSnippets *parent) :
    QDialog(parent),
    ui(new Ui::FormDlgRuleSnippet)
{
    ui->setupUi(this);
    formCfgRuleSnippets = parent;
    this->opCode = opCode;

    id = formCfgRuleSnippets->getColumnData("id").toInt();
    name = formCfgRuleSnippets->getColumnData("name").toString();
    snippets = formCfgRuleSnippets->getColumnData("snippets").toString();

    if (opCode == FormCfgRuleSnippets::REC_DELETE)
    {
        ui->edtRuleSnippetName->setReadOnly(true);
        ui->edtRuleSnippet->setReadOnly(true);
        ui->btnSave->setText("&Delete");
        buttonsEnabled(true);
    }
    else
    {
        buttonsEnabled(false);
    }

    if (opCode == FormCfgRuleSnippets::REC_ADD)
    {
        ui->btnSave->setText("&Create");
        ui->edtRuleSnippetName->setText("");
        ui->edtRuleSnippet->clear();
        ui->edtRuleSnippet->appendPlainText("");
    }
    else
    {
        ui->edtRuleSnippetName->setText(name);
        ui->edtRuleSnippet->clear();
        ui->edtRuleSnippet->appendPlainText(snippets);
    }



    connect(ui->edtRuleSnippetName, SIGNAL(textEdited(QString)),
            this, SLOT(dataChanged()));
    connect(ui->edtRuleSnippet, SIGNAL(textChanged()),
            this, SLOT(dataChanged()));


}

FormDlgRuleSnippet::~FormDlgRuleSnippet()
{
    delete ui;
}

void FormDlgRuleSnippet::dataChanged()
{
    buttonsEnabled(true);
}

void FormDlgRuleSnippet::slotBtnSave()
{
    if (validateData())
    {
        writeRow();
        accept();
    }
}

void FormDlgRuleSnippet::slotBtnCancel()
{
    reject();
}

void FormDlgRuleSnippet::buttonsEnabled(bool enabled)
{
    ui->btnSave->setEnabled(enabled);
    ui->btnCancel->setEnabled(true); // always leave the cancel button enabled

}



bool FormDlgRuleSnippet::validateData()
{
    bool result = true;
    QString errMsg("");
    QString snippetName = ui->edtRuleSnippetName->text();
    QString snippet = ui->edtRuleSnippet->toPlainText();

    // check snippetName has something in it
    if (snippetName.isEmpty())
    {
        errMsg = errMsg.append("%1Snippet Name is empty").
                arg((result ? "" : "\n"));
        result = false;
    }
    else if (snippetName.trimmed().size() <= 2)
    {
        errMsg = errMsg.append("%1Snippet Name must be at least two characters"
                      " - not including leading and trailing spaces.").
                arg((result ? "" : "\n"));
        result = false;
    }

    // Editing an existing record
    // If ok so far make a case insensitive check that a ruleset with
    // the entered name does not already exist
    if (result && opCode == FormCfgRuleSnippets::REC_EDIT)
    {
        if (result)
        {
            QSqlQuery qry;
            qry.prepare("select count(*) as count from rulesetsnippets "
                        " where id <> :id and upper(name) = upper(:name)");
            qry.bindValue(":id", this->id);
            qry.bindValue(":name", ui->edtRuleSnippetName->text());

            if (qry.exec())
            {
                if (qry.first())
                {
                    if (qry.record().value("count").toInt() > 0 )
                    {
                        errMsg = errMsg.append("%1%2").
                                arg((result ? "" : "\n")).
                                arg("This Snippet name already exists\n(case insensitive check).");
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


    // Editing a new record
    // If ok so far make a case insensitive check that a rulesnippet with
    // the entered name does not already exist
    if (result && opCode == FormCfgRuleSnippets::REC_ADD)
    {
        if (result)
        {
            QSqlQuery qry;
            qry.prepare("select count(*) as count from rulesetsnippets "
                        " where upper(name) = upper(:name)");
            qry.bindValue(":name", ui->edtRuleSnippetName->text());

            if (qry.exec())
            {
                if (qry.first())
                {
                    if (qry.record().value("count").toInt() > 0 )
                    {
                        errMsg = errMsg.append("%1%2").
                                arg((result ? "" : "\n")).
                                arg("This Snippet name already exists\n(case insensitive check).");
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
                qDebug("%s \nExecuted Query [%s]",
                       qry.lastError().text().toAscii().data(),
                       qry.executedQuery().toAscii().data());
                result = false;
            }

            }
        }


    // Check snippet has something in it
    if (snippet.trimmed().isEmpty())
    {
        errMsg = errMsg.append("%1Snippet is empty").
                arg((result ? "" : "\n"));
        result = false;
    }

    if (! result)
    {
        QMessageBox::information(this, "Data Validation Error", errMsg);
    }

    return result;
}



bool FormDlgRuleSnippet::writeRow()
{
    bool result = false;
    QString qryStr;

    if (opCode == FormCfgRuleSnippets::REC_ADD)
    {
        QSqlRecord rec = formCfgRuleSnippets->getModel()->record();
        rec.setValue("name", ui->edtRuleSnippetName->text());
        rec.setValue("snippets", ui->edtRuleSnippet->toPlainText());

        // insert new row at end of model
        if (formCfgRuleSnippets->getModel()->insertRecord(-1, rec))
        {
            formCfgRuleSnippets->getModel()->submitAll();
            result = true;
        }
        else
        {
            formCfgRuleSnippets->getModel()->revertAll();
        }
    }

    if (opCode == FormCfgRuleSnippets::REC_EDIT)
    {
        int currentRow = formCfgRuleSnippets->getView()->currentRow();
        QSqlRecord rec = formCfgRuleSnippets->getModel()->record(currentRow);
        rec.setValue("name", ui->edtRuleSnippetName->text());
        rec.setValue("snippets", ui->edtRuleSnippet->toPlainText());

        // update row
        if (formCfgRuleSnippets->getModel()->setRecord(currentRow, rec))
        {
            formCfgRuleSnippets->getModel()->submitAll();
            result = true;
        }
        else
        {
            formCfgRuleSnippets->getModel()->revertAll();
        }
    }

    if (opCode == FormCfgRuleSnippets::REC_DELETE)
    {
        int currentRow = this->formCfgRuleSnippets->getView()->currentRow();

        if (QMessageBox::Yes == QMessageBox::question(this,
                                                      "Delete Snippet",
                                                      "Confirm deletion of Snippet",
                                                      QMessageBox::Yes, QMessageBox::Cancel))
        {
            // delete row
            if (formCfgRuleSnippets->getModel()->removeRow(currentRow))
            {
                formCfgRuleSnippets->getModel()->submitAll();
                result = true;
            }
            else
            {
                formCfgRuleSnippets->getModel()->revertAll();
            }
        }
    }

    return result;
}


void FormDlgRuleSnippet::setSnippetNameReadOnly(bool readOnly)
{
    if (readOnly)
    {
        ui->edtRuleSnippetName->setReadOnly(true);
        ui->edtRuleSnippetName->setToolTip("Snippet name cannot be changed as it \nis included in one or more rulesets");
    }
    else
    {
        ui->edtRuleSnippetName->setReadOnly(false);
        ui->edtRuleSnippetName->setToolTip("");
    }
    return;
}
