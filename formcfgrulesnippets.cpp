#include "formcfgrulesnippets.h"
#include "ui_formcfgrulesnippets.h"

FormCfgRuleSnippets::FormCfgRuleSnippets(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCfgRuleSnippets)
{
    ui->setupUi(this);
}

FormCfgRuleSnippets::~FormCfgRuleSnippets()
{
    delete ui;
}
