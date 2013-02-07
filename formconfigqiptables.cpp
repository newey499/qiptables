#include "formconfigqiptables.h"
#include "ui_formconfigqiptables.h"

FormConfigQiptables::FormConfigQiptables(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormConfigQiptables)
{
    ui->setupUi(this);
}

FormConfigQiptables::~FormConfigQiptables()
{
    delete ui;
}
