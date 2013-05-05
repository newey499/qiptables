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


#ifndef FORMDLGRULESET_H
#define FORMDLGRULESET_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include "formcfgrulesnippets.h"
#include "rulesetsqltablemodel.h"
#include "formcfgruleset.h"
#include "formdlgrulesnippet.h"

class FormCfgRuleset;

namespace Ui {
class FormDlgRuleset;
}

/**
 \file formdlgruleset.h

 \class FormDlgRuleset

 \brief Add/Amend/Delete Rulesets

 \todo Investigate and implement a mechanism for validating firewall rules


*/
class FormDlgRuleset : public QDialog
{
    Q_OBJECT

public:

    QPointer<FormCfgRuleSnippets> formSnippets;

    /**
    \brief  Constructor
    \param  opCode - whether the dialogue is to be used to add, amend or
            delete a ruleset.
    \param  parent - Pointer to configuration form parent
      ***********************/
    explicit FormDlgRuleset(int opCode, FormCfgRuleset *parent);
    ~FormDlgRuleset();

public slots:

    /**
    \brief Save changes
      ***************/
    virtual void slotSave();

    /**
    \brief Cancel changes
      ***************/
    virtual void slotCancel();

    /**
    \brief Launch the rulesnippet form to allow snippets to be added
    to a ruleset.
      ***************/
    virtual void slotLaunchFormRuleSnippets();


    /**
    \brief Append a snippet to the ruleset
      ***************/
    virtual void slotAddSnippet(bool useInclude, int id, QString name, QString snippets);

protected:

    FormCfgRuleset *formRuleset;
    int id;
    QString name;
    QString rules;
    int opcode;

    /**
    \brief checks to see if current ruleset is the default
    \return true if ruleset is default else false
      *******************/
    bool isDefaultRuleset();

    /**
    \brief Sanity check on data
      *******************/
    bool validateData();

    /**
    \brief Write data to table
      *******************/
    bool writeRow();


    void hideEvent(QHideEvent *event);

protected slots:

    /**
    \brief Enable/disable buttons dependant on edit state
      ***************/
    virtual void buttonsEnabled(bool enabled);

    /**
    \brief Changes the enabled state of form buttons if the displayed data is changed
      *******************/
    virtual void dataChanged();


private:
    Ui::FormDlgRuleset *ui;
};

#endif // FORMDLGRULESET_H
