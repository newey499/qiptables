#ifndef FORMCFGRULESNIPPETS_H
#define FORMCFGRULESNIPPETS_H

#include <QWidget>

namespace Ui {
class FormCfgRuleSnippets;
}

class FormCfgRuleSnippets : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormCfgRuleSnippets(QWidget *parent = 0);
    ~FormCfgRuleSnippets();
    
private:
    Ui::FormCfgRuleSnippets *ui;
};

#endif // FORMCFGRULESNIPPETS_H
