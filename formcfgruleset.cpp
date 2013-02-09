#include "formcfgruleset.h"
#include "ui_formcfgruleset.h"

FormCfgRuleset::FormCfgRuleset(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCfgRuleset)
{
    ui->setupUi(this);

    model = new QSqlTableModel(this);
    model->setTable("ruleset");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, tr("Rules"));

    ui->tblRuleset->setModel(model);
    ui->tblRuleset->hideColumn(0); // don't show the ID
    ui->tblRuleset->show();
}

FormCfgRuleset::~FormCfgRuleset()
{
    delete ui;
}
