#ifndef FORMCONFIGQIPTABLES_H
#define FORMCONFIGQIPTABLES_H

#include <QWidget>

namespace Ui {
class FormConfigQiptables;
}

class FormConfigQiptables : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormConfigQiptables(QWidget *parent = 0);
    ~FormConfigQiptables();
    
private:
    Ui::FormConfigQiptables *ui;
};

#endif // FORMCONFIGQIPTABLES_H
