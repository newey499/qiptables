#ifndef FORMCFGRULESNIPPETS_H
#define FORMCFGRULESNIPPETS_H

#include <QMessageBox>
#include <QPointer>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QWidget>

#include "formdlgrulesnippet.h"
#include "rulesnippetssqltablemodel.h"
#include "rulesnippetstableview.h"




namespace Ui {
class FormCfgRuleSnippets;
}

class FormCfgRuleSnippets : public QWidget
{
    Q_OBJECT

public:

    static const int REC_ADD;
    static const int REC_EDIT;
    static const int REC_DELETE;


    explicit FormCfgRuleSnippets(QWidget *parent = 0);
    ~FormCfgRuleSnippets();

    RuleSnippetsTableView * getView();
    RuleSnippetsSqlTableModel *getModel() { return model; }
    virtual QVariant getColumnData(QString colName);

public slots:

    virtual void showEvent(QShowEvent *event);
    virtual void currentRowChanged();

    virtual void slotBtnAdd();
    virtual void slotBtnEdit();
    virtual void slotBtnDelete();

protected:

    QPointer<RuleSnippetsSqlTableModel> model;

private:

    Ui::FormCfgRuleSnippets *ui;
};

#endif // FORMCFGRULESNIPPETS_H
