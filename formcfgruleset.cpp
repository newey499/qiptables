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
    //model->setSort(1, Qt::AscendingOrder); // Sort by ruleset Name
    model->setHeaderData(0, Qt::Horizontal, tr("Id"));
    model->setHeaderData(1, Qt::Horizontal, tr("Ruleset Name"));
    model->setHeaderData(2, Qt::Horizontal, tr("Ruleset"));
    model->select();

    ui->tblRuleset->setModel(model);
    ui->tblRuleset->hideColumn(0); // don't show the ID
    ui->tblRuleset->hideColumn(2); // don't show the Ruleset - displayed
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

/**
\brief When the form is displayed set the focus to the ruleset browse
 window and select the first row
****************/
void FormCfgRuleset::showEvent(QShowEvent *event)
{
    ui->tblRuleset->setFocus();
    currentRowChanged();
}

QVariant FormCfgRuleset::getColumnData(QString colName)
{
    return model->record(ui->tblRuleset->currentIndex().row()).value(colName);
}

//void FormCfgRuleset::currentRowChanged(QModelIndex currentRow)
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
