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
\file formcfgruleset.cpp

\brief Browse Rulesets and call add amend delete ruleset form

***************************/


#include "formcfgruleset.h"
#include "ui_formcfgruleset.h"

const int FormCfgRuleset::REC_ADD       = 1;
const int FormCfgRuleset::REC_EDIT      = 2;
const int FormCfgRuleset::REC_DELETE    = 3;

FormCfgRuleset::FormCfgRuleset(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCfgRuleset)
{
    ui->setupUi(this);
    //formSnippets = new FormCfgRuleSnippets(true, this, Qt::Popup);
    formSnippets = new FormCfgRuleSnippets(true, this, Qt::Window);
    //formSnippets->enableDrag();
    formSnippets->enableContextMenu(true);
    connect(formSnippets, SIGNAL(addSnippet(bool, int, QString, QString)),
            this, SLOT(slotAddSnippet(bool, int, QString, QString)));

    ui->edtRuleSet->setReadOnly(true);

    model = new RulesetSqlTableModel(this);
    model->setTable("ruleset");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //model->setSort(1, Qt::AscendingOrder); // Sort by ruleset Name
    model->setHeaderData(0, Qt::Horizontal, tr("Id"));
    model->setHeaderData(1, Qt::Horizontal, tr("Ruleset Name"));
    model->setHeaderData(2, Qt::Horizontal, tr("Internal Name"));
    model->setHeaderData(3, Qt::Horizontal, tr("Ruleset"));
    model->select();

    ui->tblRuleset->setModel(model);
    ui->tblRuleset->hideColumn(0); // don't show the ID
    ui->tblRuleset->hideColumn(2); // don't show the internal Ruleset name
    ui->tblRuleset->hideColumn(3); // don't show the Ruleset - displayed
                                   // in text edit box


    ui->tblRuleset->selectRow(0);  // select first row

    connect(ui->tblRuleset, SIGNAL(rowChanged(QModelIndex)),
            this, SLOT(currentRowChanged()));

    ui->tblRuleset->show();
}

FormCfgRuleset::~FormCfgRuleset()
{
    if (formSnippets)
    {
        delete formSnippets;
    }
    delete ui;
}


void FormCfgRuleset::showEvent(QShowEvent *event)
{
    // disable compiler warning for unused parameter - gcc optimizes out this code
    event = event;
    ui->tblRuleset->setFocus();
    currentRowChanged();
}

QVariant FormCfgRuleset::getColumnData(QString colName)
{
    return model->record(ui->tblRuleset->currentIndex().row()).value(colName);
}


void FormCfgRuleset::currentRowChanged()
{
    QString txt = getColumnData("rules").toString();

    qDebug("column text [%s]",txt.toAscii().data());
    ui->edtRuleSet->clear();
    ui->edtRuleSet->appendPlainText(txt);
}

void FormCfgRuleset::slotBtnAdd()
{
    FormDlgRuleset dlg(REC_ADD, this);
    qDebug("FormCfgRuleset::slotBtnAdd()");
    dlg.setWindowTitle("Add a Ruleset");
    dlg.setModal(true);
    if (dlg.exec())
    {
        getModel()->submitAll();
        ui->tblRuleset->selectRow(model->rowCount() - 1);
    }
    else
    {
        getModel()->revertAll();
    }
}

void FormCfgRuleset::slotBtnEdit()
{
    FormDlgRuleset dlg(REC_EDIT, this);
    qDebug("FormCfgRuleset::slotBtnEdit()");
    dlg.setWindowTitle("Edit this Ruleset");
    dlg.setModal(true);
    int currentRow = getView()->currentRow();
    if (dlg.exec())
    {
        getModel()->submitAll();
        QString rules = getColumnData("rules").toString();
        ui->edtRuleSet->clear();
        ui->edtRuleSet->setPlainText(rules);
        ui->tblRuleset->selectRow(currentRow);
    }
    else
    {
        getModel()->revertAll();
    }
}

void FormCfgRuleset::slotBtnDelete()
{
    if (! isRulesetDefault())
    {
        FormDlgRuleset dlg(REC_DELETE, this);
        qDebug("FormCfgRuleset::slotBtnDelete()");
        dlg.setWindowTitle("Delete this Ruleset");
        dlg.setModal(true);
        int currentRow = getView()->currentRow();
        if (dlg.exec())
        {
            getModel()->submitAll();
            if (currentRow > 0)
            {
                currentRow--;   // select the row above the deleted one
            }
            ui->tblRuleset->selectRow(currentRow);
        }
        else
        {
            getModel()->revertAll();
        }
    }
}


RulesetTableView * FormCfgRuleset::getView()
{
    return ui->tblRuleset;
}


bool FormCfgRuleset::isRulesetDefault()
{
    bool result = false;
    QString errMsg("");
    QString rulesetName = getColumnData("name").toString();

    // Cannot delete a ruleset if it is in use as the default ruleset
    QSqlQuery qry;
    qry.prepare(" select count(*) as count "
                " from sysconf "
                " where defaultRuleName = :defaultRuleName");
    qry.bindValue(":defaultRuleName", rulesetName);

    if (qry.exec())
    {
        if (qry.first())
        {
            if (qry.record().value("count").toInt() > 0)
            {
                errMsg = errMsg.append("%1Ruleset Name [%2] may not be deleted "
                                       "as it is in use as the default.").
                                arg((result ? "" : "\n")).arg(rulesetName);
                result = true;
            }

        }
        else
        {
            qDebug("query on delete validation failed to find first row \n[%s]",
                   qry.lastError().text().toAscii().data());
        }

    }
    else
    {
        qDebug("query on delete validation failed \n[%s]",
               qry.lastError().text().toAscii().data());
    }

    if (result)
    {
        QMessageBox::information(this, "Data Validation Error", errMsg);
    }

    return result;
}


void FormCfgRuleset::slotCodeSnippets()
{
    qDebug("FormCfgRuleset::slotCodeSnippets()");
    formSnippets->show();
}


void FormCfgRuleset::slotAddSnippet(bool useInclude, int id, QString name, QString snippets)
{
    id = id;  // suppress compiler warning - compiler optimises this out

    qDebug("FormCfgRuleset::slotAddSnippet(int id, QString name, QString snippets)");
    QString include;
    ui->edtRuleSet->appendPlainText(include);

    int currentRow = getView()->currentRow();
    QSqlRecord rec = getModel()->record(currentRow);
    //rec.setValue("name", ui->edtRulesetName->text());
    QString rules = rec.value("rules").toString();
    if (useInclude)
    {
        include = QString("\n#include %1").arg(name);
    }
    else
    {
        include = QString("\n%1").arg(snippets);
    }
    rules = rules.append(include);
    rec.setValue("rules", rules);

    // update row
    if (getModel()->setRecord(currentRow, rec))
    {
        qDebug("row update write submitAll() Ok");
        getModel()->submitAll();
    }
    else
    {
        qDebug("row not updated - setRecord call failed");
        getModel()->revertAll();
    }
    ui->tblRuleset->selectRow(currentRow);

}

void FormCfgRuleset::hideEvent(QHideEvent *event)
{
    qDebug("FormCfgRuleset::hideEvent(QHideEvent *event)");
    if (formSnippets->isVisible())
    {
        formSnippets->hide();
    }
    event->ignore();
}
