#ifndef FORMCFGRULESET_H
#define FORMCFGRULESET_H

#include <QPointer>
#include <QShowEvent>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QTableView>
#include <QVariant>
#include <QWidget>

#include "rulesettableview.h"
#include "rulesetsqltablemodel.h"
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

    virtual RulesetTableView *getView();
    virtual RulesetSqlTableModel *getModel() { return model; }

public slots:

    virtual void showEvent(QShowEvent *event);
    virtual void currentRowChanged();

    virtual void slotBtnAdd();
    virtual void slotBtnEdit();
    virtual void slotBtnDelete();

protected:

    QPointer<RulesetSqlTableModel> model;


    virtual bool isRulesetDefault();

private:
    Ui::FormCfgRuleset *ui;
};

#endif // FORMCFGRULESET_H
