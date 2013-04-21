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



#ifndef FORMCFGRULESNIPPETS_H
#define FORMCFGRULESNIPPETS_H

#include <QCloseEvent>
#include <QMessageBox>
#include <QPointer>
#include <QSettings>
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


    explicit FormCfgRuleSnippets(QWidget *parent = 0, Qt::WindowFlags f = 0);
    explicit FormCfgRuleSnippets(bool popupSnippetSelect, QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~FormCfgRuleSnippets();

    RuleSnippetsTableView * getView();
    RuleSnippetsSqlTableModel *getModel() { return model; }
    virtual QVariant getColumnData(QString colName);

signals:

    void addSnippet(bool useInclude, int id, QString name, QString snippets);

public slots:

    virtual void showEvent(QShowEvent *event);
    virtual void currentRowChanged();

    virtual void slotBtnAdd();
    virtual void slotBtnEdit();
    virtual void slotBtnDelete();
    virtual void slotIncludeSnippet();
    virtual void slotPasteSnippet();
    virtual void slotCloseWindow();

protected:

    QPointer<RuleSnippetsSqlTableModel> model;
    bool popupSnippetSelect;
    QString organization;
    QString application;

    virtual void commonConstructor();


    virtual void saveSettings();
    void closeEvent(QCloseEvent *event);
    virtual int quitYesNo();

private:

    Ui::FormCfgRuleSnippets *ui;
};

#endif // FORMCFGRULESNIPPETS_H
