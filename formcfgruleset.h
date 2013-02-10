#ifndef FORMCFGRULESET_H
#define FORMCFGRULESET_H

#include <QShowEvent>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QTableView>
#include <QVariant>
#include <QWidget>

#include "formdlgruleset.h"

namespace Ui {
class FormCfgRuleset;
}

class FormCfgRuleset : public QWidget
{
    Q_OBJECT
    
public:

    static const int REC_ADD;
    static const int REC_EDIT;
    static const int REC_DELETE;

    explicit FormCfgRuleset(QWidget *parent = 0);
    ~FormCfgRuleset();
    
    virtual QVariant getColumnData(QString colName);

public slots:

    virtual void showEvent(QShowEvent *event);
    virtual void currentRowChanged();

    virtual void slotBtnAdd();
    virtual void slotBtnEdit();
    virtual void slotBtnDelete();

protected:

    QSqlTableModel *model;

private:
    Ui::FormCfgRuleset *ui;
};

#endif // FORMCFGRULESET_H
