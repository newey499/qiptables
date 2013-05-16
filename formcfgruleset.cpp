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

#include "genlib.h"
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

    ui->edtRuleSet->setReadOnly(true);

    model = new RulesetSqlTableModel(this);
    model->setTable("ruleset");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);

    connect(proxyModel, SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int, int)),
            this, SLOT(slotRowsAboutToBeInserted(const QModelIndex &, int, int)));

    model->setSort(1, Qt::AscendingOrder); // Sort by ruleset Name


    model->setHeaderData(0, Qt::Horizontal, tr("Id"));
    model->setHeaderData(1, Qt::Horizontal, tr("Ruleset Name"));
    model->setHeaderData(2, Qt::Horizontal, tr("Internal Name"));
    model->setHeaderData(3, Qt::Horizontal, tr("Ruleset"));
    model->select();

    ui->tblRuleset->setModel(proxyModel);

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

    ui->edtRuleSet->clear();
    ui->edtRuleSet->appendPlainText(txt);
}

void FormCfgRuleset::slotBtnAdd()
{
    FormDlgRuleset dlg(REC_ADD, this);
    dlg.setWindowTitle("Add a Ruleset");
    dlg.setModal(true);
    if (dlg.exec())
    {
        getModel()->submitAll();
        QModelIndex start = getModel()->index(0,1);
        QModelIndexList indexList = getModel()->match(start, Qt::DisplayRole, getUpdatedName());
        //ui->tblRuleset->selectRow(model->rowCount() - 1);
        if (indexList.count() > 0)
        {
            ui->tblRuleset->selectRow(indexList[0].row());
            qDebug("name [%s] row [%d]",
                   getUpdatedName().toString().toAscii().data(),
                   indexList[0].row());

        }
    }
    else
    {
        getModel()->revertAll();
    }
}

void FormCfgRuleset::slotBtnEdit()
{
    FormDlgRuleset dlg(REC_EDIT, this);
    dlg.setWindowTitle("Edit this Ruleset");
    dlg.setModal(true);
    if (dlg.exec())
    {
        getModel()->submitAll();
        QModelIndex start = getModel()->index(0,1);
        QModelIndexList indexList = getModel()->match(start, Qt::DisplayRole, getUpdatedName());
        if (indexList.count() > 0)
        {
            ui->tblRuleset->selectRow(indexList[0].row());
            qDebug("name [%s] row [%d]",
                   getUpdatedName().toString().toAscii().data(),
                   indexList[0].row());
        }

        QString rules = getColumnData("rules").toString();
        ui->edtRuleSet->clear();
        ui->edtRuleSet->setPlainText(rules);
        //ui->tblRuleset->selectRow(currentRow);
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



void FormCfgRuleset::slotAddSnippet(bool useInclude, int id, QString name, QString snippets)
{
    id = id;  // suppress compiler warning - compiler optimises this out

    QString include;
    ui->edtRuleSet->appendPlainText(include);

    int currentRow = getView()->currentRow();
    QSqlRecord rec = getModel()->record(currentRow);
    //rec.setValue("name", ui->edtRulesetName->text());
    QString rules = rec.value("rules").toString();
    if (useInclude)
    {
        include = GenLib::getIncludeString(name).prepend("\n");
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
        getModel()->submitAll();
    }
    else
    {
        getModel()->revertAll();
    }
    ui->tblRuleset->selectRow(currentRow);

}


void FormCfgRuleset::slotRowsAboutToBeInserted(const QModelIndex &index, int start, int end)
{
    int proxyRow = proxyModel->mapFromSource(index).row();
    qDebug("slotRowsAboutToBeInserted start [%d] end [%d] row [%d] proxyRow [%d]",
           start, end, index.row(), proxyRow);

}


QVariant FormCfgRuleset::getUpdatedName()
{
    return newName;
}

void FormCfgRuleset::setUpdatedName(QVariant name)
{
    newName = name;
}

