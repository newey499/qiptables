#ifndef FORMFIREWALLRULES_H
#define FORMFIREWALLRULES_H

#include <QWidget>
#include "qiptablesexception.h"

namespace Ui {
class FormFirewallRules;
}

class FormFirewallRules : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormFirewallRules(QWidget *parent = 0);
    ~FormFirewallRules();
    
private:
    Ui::FormFirewallRules *ui;
};

#endif // FORMFIREWALLRULES_H
