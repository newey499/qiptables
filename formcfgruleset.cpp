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

    model = new QSqlTableModel(this);
    model->setTable("ruleset");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, tr("Rules"));

    ui->tblRuleset->setModel(model);
    ui->tblRuleset->hideColumn(0); // don't show the ID
    ui->tblRuleset->hideColumn(2); // don't show the Rules - displayed
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
    //qDebug("FormCfgRuleset::currentRowChanged(int rowNo) [%d]",
    //       ui->tblRuleset->currentRow());

    QSqlTableModel *model = (QSqlTableModel *) ui->tblRuleset->model();

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
    dlg.exec();
}

void FormCfgRuleset::slotBtnEdit()
{
    FormDlgRuleset dlg(REC_EDIT, this);
    qDebug("FormCfgRuleset::slotBtnEdit()");
    dlg.setWindowTitle("Edit this Ruleset");
    dlg.setModal(true);
    dlg.exec();
}

void FormCfgRuleset::slotBtnDelete()
{
    FormDlgRuleset dlg(REC_DELETE, this);
    qDebug("FormCfgRuleset::slotBtnDelete()");
    dlg.setWindowTitle("Delete this Ruleset");
    dlg.setModal(true);
    dlg.exec();
}
