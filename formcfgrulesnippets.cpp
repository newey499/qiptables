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
\file formcfgrulesnippets.cpp

\brief Browse Rulesnippets and call add amend delete rulesnippet form

***************************/

#include "formcfgrulesnippets.h"
#include "ui_formcfgrulesnippets.h"

const int FormCfgRuleSnippets::REC_ADD       = 1;
const int FormCfgRuleSnippets::REC_EDIT      = 2;
const int FormCfgRuleSnippets::REC_DELETE    = 3;

FormCfgRuleSnippets::FormCfgRuleSnippets(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::FormCfgRuleSnippets)
{
    popupSnippetSelect = false;

    commonConstructor();

    ui->btnAdd->setVisible(true);
    ui->btnEdit->setVisible(true);
    ui->btnDelete->setVisible(true);
    ui->btnIncludeSnippet->setVisible(false);
    ui->btnPasteSnippet->setVisible(false);
    ui->btnClose->setVisible(false);

}


FormCfgRuleSnippets::FormCfgRuleSnippets(bool popupSnippetSelect, QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    ui(new Ui::FormCfgRuleSnippets)
{

    this->popupSnippetSelect = popupSnippetSelect;
    commonConstructor();


    if (this->popupSnippetSelect)
    {
        QSettings settings(organization, application);
        restoreGeometry(settings.value("geometry").toByteArray());

        connect(ui->tblRuleSnippets, SIGNAL(menuItemSelected(QAction*)),
                this, SLOT(slotMenuSelection(QAction *)));
    }

    ui->btnAdd->setVisible(false);
    ui->btnEdit->setVisible(false);
    ui->btnDelete->setVisible(false);
    ui->btnIncludeSnippet->setVisible(true);
    ui->btnPasteSnippet->setVisible(true);
    ui->btnClose->setVisible(true);

}


void FormCfgRuleSnippets::commonConstructor()
{

    organization = "git:bitbucket.org:newey499.qiptables.git";
    application  = "QiptablesFormCfgRuleSnippets";

    // Zero pointers
    model = 0;

    ui->setupUi(this);

    ui->btnIncludeSnippet->setToolTip("Use #include to incorporate snippet in rule");
    ui->btnPasteSnippet->setToolTip("Paste contents of snippet into rule");

    ui->edtRuleSnippets->setReadOnly(true);


    model = new RuleSnippetsSqlTableModel(this);
    model->setTable("rulesetsnippets");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //model->setSort(1, Qt::AscendingOrder); // Sort by ruleset Name
    model->setHeaderData(0, Qt::Horizontal, tr("Id"));
    model->setHeaderData(1, Qt::Horizontal, tr("Snippet Name"));
    model->setHeaderData(2, Qt::Horizontal, tr("Snippet"));

    model->select();
    ui->tblRuleSnippets->setModel(model);

    ui->tblRuleSnippets->hideColumn(0); // don't show the ID
    ui->tblRuleSnippets->hideColumn(2); // don't show the Ruleset Snippets - displayed
                                   // in text edit box
    ui->tblRuleSnippets->selectRow(0);  // select first row

    connect(ui->tblRuleSnippets, SIGNAL(rowChanged(QModelIndex)),
            this, SLOT(currentRowChanged()));

    ui->tblRuleSnippets->show();
}


FormCfgRuleSnippets::~FormCfgRuleSnippets()
{
    delete ui;
    if (model)
    {
        delete model;
    }
}

RuleSnippetsTableView * FormCfgRuleSnippets::getView()
{
    return ui->tblRuleSnippets;
}

/**
\brief When the form is displayed set the focus to the rulesnippet browse
 window and select the first row
****************/
void FormCfgRuleSnippets::showEvent(QShowEvent *event)
{
    // disable compiler warning for unused parameter - gcc optimizes out this code
    event = event;

    ui->tblRuleSnippets->setFocus();
    currentRowChanged();
}

QVariant FormCfgRuleSnippets::getColumnData(QString colName)
{
    return model->record(ui->tblRuleSnippets->currentIndex().row()).value(colName);
}

void FormCfgRuleSnippets::currentRowChanged()
{
    QString txt = getColumnData("snippets").toString();

    qDebug("column text [%s]",txt.toAscii().data());

    ui->edtRuleSnippets->clear();
    ui->edtRuleSnippets->appendPlainText(txt);
}


void FormCfgRuleSnippets::slotBtnAdd()
{
    qDebug("FormCfgRuleSnippets::slotBtnAdd()");

    FormDlgRuleSnippet dlg(FormCfgRuleSnippets::REC_ADD, this);

    dlg.setWindowTitle("Add a Ruleset");
    dlg.setModal(true);
    if (dlg.exec())
    {
        //getModel()->submitAll();
        ui->tblRuleSnippets->selectRow(model->rowCount() - 1);
    }
    else
    {
        //getModel()->revertAll();
    }

}

void FormCfgRuleSnippets::slotBtnEdit()
{
    qDebug("FormCfgRuleSnippets::slotBtnEdit()");

    FormDlgRuleSnippet dlg(FormCfgRuleSnippets::REC_EDIT, this);

    qDebug("FormCfgRuleset::slotBtnEdit()");
    dlg.setWindowTitle("Edit this Ruleset");
    dlg.setModal(true);
    int currentRow = getView()->currentRow();
    if (dlg.exec())
    {
        getModel()->submitAll();
        QString rules = getColumnData("rules").toString();
        ui->edtRuleSnippets->clear();
        ui->edtRuleSnippets->setPlainText(rules);
        ui->tblRuleSnippets->selectRow(currentRow);
    }
    else
    {
        getModel()->revertAll();
    }
}

void FormCfgRuleSnippets::slotBtnDelete()
{
    qDebug("FormCfgRuleSnippets::slotBtnDelete()");


    FormDlgRuleSnippet dlg(REC_DELETE, this);
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
        ui->tblRuleSnippets->selectRow(currentRow);
    }
    else
    {
        getModel()->revertAll();
    }

}


void FormCfgRuleSnippets::saveSettings()
{
    QSettings settings(organization,application);
    settings.setValue("geometry", saveGeometry());
}

void FormCfgRuleSnippets::closeEvent(QCloseEvent *event)
{

    qDebug("MainWindow::closeEvent");
    //int ret = quitYesNo();

    //if (ret == QMessageBox::Yes)
    if (true)
    {
        saveSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}


int FormCfgRuleSnippets::quitYesNo()
{
    QString exit = QString("Exit ");
    exit = exit.append(this->application);

    int ret = QMessageBox::question(this, tr(this->application.toAscii().data()),
                                   tr(exit.toAscii().data()),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::Yes);

    return ret;
}


void FormCfgRuleSnippets::slotIncludeSnippet()
{
    int row = ui->tblRuleSnippets->currentRow();
    int id = getColumnData("id").toInt();
    QString name = getColumnData("name").toString();
    QString snippets = getColumnData("snippets").toString();
    emit addSnippet(true, id, name, snippets);
    qDebug("FormCfgRuleSnippets::slotIncludeSnippet() row [%d] name [%s]",
           row, name.toAscii().data());
}


void FormCfgRuleSnippets::slotPasteSnippet()
{
    int row = ui->tblRuleSnippets->currentRow();
    int id = getColumnData("id").toInt();
    QString name = getColumnData("name").toString();
    QString snippets = getColumnData("snippets").toString();
    emit addSnippet(false, id, name, snippets);
    qDebug("FormCfgRuleSnippets::slotPasteSnippet() row [%d] name [%s]",
           row, name.toAscii().data());
}


void FormCfgRuleSnippets::slotCloseWindow()
{
    qDebug("FormCfgRuleSnippets::slotCloseWindow()");
    close();
}


void FormCfgRuleSnippets::enableContextMenu(bool enable)
{
    ui->tblRuleSnippets->enableContextMenu(enable);
}


void FormCfgRuleSnippets::slotMenuSelection(QAction * selectedAction)
{
    qDebug("FormCfgRuleSnippets::slotMenuSelection(QAction * selectedAction) [%s]",
           selectedAction->text().toAscii().data());
    if (selectedAction->data().toInt() == RuleSnippetsTableView::INCLUDE_SNIPPET)
    //if (selectedAction->data().toInt() == RuleSnippetsTableView::INCLUDE_SNIPPET)
    {
        slotIncludeSnippet();
    }
    if (selectedAction->data().toInt() == RuleSnippetsTableView::PASTE_SNIPPET)
    {
        slotPasteSnippet();
    }

}
