#include "formdlgruleset.h"
#include "ui_formdlgruleset.h"

FormDlgRuleset::FormDlgRuleset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormDlgRuleset)
{
    ui->setupUi(this);
}

FormDlgRuleset::~FormDlgRuleset()
{
    delete ui;
}
