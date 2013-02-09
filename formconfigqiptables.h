#ifndef FORMCONFIGQIPTABLES_H
#define FORMCONFIGQIPTABLES_H

#include <QFileDialog>
#include <QStringList>
#include <QWidget>

#include "formcfgruleset.h"
#include "formcfgrulesnippets.h"
#include "formcfgsettings.h"

namespace Ui {
class FormConfigQiptables;
}

class FormConfigQiptables : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormConfigQiptables(QWidget *parent = 0);
    ~FormConfigQiptables();

public slots:



protected:

    FormCfgRuleset *formCfgRuleset;
    FormCfgRuleSnippets *formCfgRuleSnippets;
    FormCfgSettings *formCfgSettings;
    
private:
    Ui::FormConfigQiptables *ui;

};

#endif // FORMCONFIGQIPTABLES_H
