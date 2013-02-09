#ifndef FORMCFGRULESET_H
#define FORMCFGRULESET_H

#include <QSqlTableModel>
#include <QTableView>
#include <QWidget>

namespace Ui {
class FormCfgRuleset;
}

class FormCfgRuleset : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormCfgRuleset(QWidget *parent = 0);
    ~FormCfgRuleset();
    
protected:

    QSqlTableModel *model;

private:
    Ui::FormCfgRuleset *ui;
};

#endif // FORMCFGRULESET_H
