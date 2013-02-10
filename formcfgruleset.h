#ifndef FORMCFGRULESET_H
#define FORMCFGRULESET_H

#include <QShowEvent>
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
    
public slots:

    virtual void showEvent(QShowEvent *event);
    virtual void currentRowChanged(int rowNo);

protected:

    QSqlTableModel *model;

private:
    Ui::FormCfgRuleset *ui;
};

#endif // FORMCFGRULESET_H
