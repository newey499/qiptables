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

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);

    model->setSort(2, Qt::AscendingOrder); // Sort by upper case rulesnippet Name
    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    model->setHeaderData(0, Qt::Horizontal, tr("Id"));
    model->setHeaderData(1, Qt::Horizontal, tr("Snippet Name"));
    model->setHeaderData(2, Qt::Horizontal, tr("Snippet"));

    model->select();
    ui->tblRuleSnippets->setModel(proxyModel);

    connect(proxyModel, SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int, int)),
            this, SLOT(slotRowsAboutToBeInserted(const QModelIndex &, int, int)));

    ui->tblRuleSnippets->hideColumn(0); // don't show the ID

    ui->tblRuleSnippets->hideColumn(2); // don't show the upper case snippet name
    ui->tblRuleSnippets->hideColumn(3); // don't show the Ruleset Snippets - displayed
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
    ui->edtRuleSnippets->clear();
    ui->edtRuleSnippets->appendPlainText(txt);
}


void FormCfgRuleSnippets::slotBtnAdd()
{
    FormDlgRuleSnippet dlg(FormCfgRuleSnippets::REC_ADD, this);
    DatabaseManager dm(Install::INSTALL_DIR, this);

    /*
    Don't allow snippet name to be added if the snippet name already exists
    **************/
    dlg.setWindowTitle("Add a Rulesnippet");
    dlg.setModal(true);
    if (dlg.exec())
    {
        getModel()->submitAll();
        //ui->tblRuleSnippets->selectRow(model->rowCount() - 1);
        QModelIndex start = getModel()->index(0,1);
        QModelIndexList indexList = getModel()->match(start, Qt::DisplayRole, getUpdatedName());
        if (indexList.count() > 0)
        {
            ui->tblRuleSnippets->selectRow(indexList[0].row());
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

void FormCfgRuleSnippets::slotBtnEdit()
{
    FormDlgRuleSnippet dlg(FormCfgRuleSnippets::REC_EDIT, this);
    DatabaseManager dm(Install::INSTALL_DIR, this);

    /*
    Don't allow snippet name to be changed if the snippet is included in a ruleset
    *********/
    if (dm.isRulesnippetInUse(getColumnData("name").toString()).count() > 0)
    {
        dlg.setSnippetNameReadOnly(true);
    }
    dlg.setWindowTitle("Edit this Rulesnippet");
    dlg.setModal(true);
    if (dlg.exec())
    {
        getModel()->submitAll();
        QModelIndex start = getModel()->index(0,1);
        QModelIndexList indexList = getModel()->match(start, Qt::DisplayRole, getUpdatedName());
        if (indexList.count() > 0)
        {
            ui->tblRuleSnippets->selectRow(indexList[0].row());
            qDebug("name [%s] row [%d]",
                   getUpdatedName().toString().toAscii().data(),
                   indexList[0].row());
        }

        QString rules = getColumnData("rules").toString();
        ui->edtRuleSnippets->clear();
        ui->edtRuleSnippets->setPlainText(rules);
    }
    else
    {
        getModel()->revertAll();
    }
}

void FormCfgRuleSnippets::slotBtnDelete()
{
    DatabaseManager dm(Install::INSTALL_DIR, this);

    // Do not allow a snippet to be deleted if it is included in a ruleset.
    QString snippet = this->getColumnData("name").toString();
    QStringList rulesets = dm.isRulesnippetInUse(snippet);
    if (rulesets.count() > 0)
    {
        displayMsgBox("Delete Snippet", snippet, rulesets);
        return;
    }

    FormDlgRuleSnippet dlg(REC_DELETE, this);
    dlg.setWindowTitle("Delete this Snippet");
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
    //int row = ui->tblRuleSnippets->currentRow();
    int id = getColumnData("id").toInt();
    QString name = getColumnData("name").toString();
    QString snippets = getColumnData("snippets").toString();
    emit addSnippet(true, id, name, snippets);
}


void FormCfgRuleSnippets::slotPasteSnippet()
{
    //int row = ui->tblRuleSnippets->currentRow();
    int id = getColumnData("id").toInt();
    QString name = getColumnData("name").toString();
    QString snippets = getColumnData("snippets").toString();
    emit addSnippet(false, id, name, snippets);
}


void FormCfgRuleSnippets::slotCloseWindow()
{
    close();
}


void FormCfgRuleSnippets::enableContextMenu(bool enable)
{
    ui->tblRuleSnippets->enableContextMenu(enable);
}


void FormCfgRuleSnippets::slotMenuSelection(QAction * selectedAction)
{
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


void FormCfgRuleSnippets::displayMsgBox(QString title, QString snippet, QStringList rulesets)
{
    QString message = rulesets.join("\n");
    QString tmp = QString("Snippet [%1] \n may not be deleted as it\n").arg(snippet);
    tmp = tmp.append("is included in the following rulesets.\n\n");
    message = message.prepend(tmp);
    QMessageBox::warning(this, title, message, QMessageBox::Ok);
}


QVariant FormCfgRuleSnippets::getUpdatedName()
{
    return newName;
}

void FormCfgRuleSnippets::setUpdatedName(QVariant name)
{
    newName = name;
}


void FormCfgRuleSnippets::slotRowsAboutToBeInserted(const QModelIndex &index, int start, int end)
{
    int proxyRow = proxyModel->mapFromSource(index).row();
    qDebug("slotRowsAboutToBeInserted start [%d] end [%d] row [%d] proxyRow [%d]",
           start, end, index.row(), proxyRow);

}
