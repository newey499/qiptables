/************************************************************************
Copyright Chris Newey 2013

qiptables@hotmail.com


This file is part of Qiptables.

Qiptables is distributed under the terms of the GNU General Public License

Qiptables is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Qiptables is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Qiptables.  If not, see <http://www.gnu.org/licenses/>.

***************************************************************************/



#ifndef FORMCFGRULESET_H
#define FORMCFGRULESET_H

#include <QHideEvent>
#include <QPointer>
#include <QShowEvent>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QTableView>
#include <QVariant>
#include <QWidget>

#include "formcfgrulesnippets.h"
#include "rulesettableview.h"
#include "rulesetsqltablemodel.h"
#include "formdlgruleset.h"



namespace Ui {
class FormCfgRuleset;
}


/**
\file formcfgruleset.h
\class FormCfgRuleset

\brief Browse Rulesets and call add amend delete ruleset form


***************************/
class FormCfgRuleset : public QWidget
{
    Q_OBJECT

public:

    static const int REC_ADD;
    static const int REC_EDIT;
    static const int REC_DELETE;

    QPointer<FormCfgRuleSnippets> formSnippets;

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
    virtual void slotCodeSnippets();

    virtual void slotAddSnippet(bool useInclude, int id, QString name, QString snippets);

signals:



protected:

    QPointer<RulesetSqlTableModel> model;
    virtual bool isRulesetDefault();

    void hideEvent(QHideEvent * event);

private:
    Ui::FormCfgRuleset *ui;
};

#endif // FORMCFGRULESET_H
