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


#ifndef FORMDLGRULESNIPPET_H
#define FORMDLGRULESNIPPET_H

#include <QDialog>
#include <QPointer>
#include <QSettings>
#include <QVariant>

#include "formcfgrulesnippets.h"



class FormCfgRuleSnippets;

namespace Ui {
class FormDlgRuleSnippet;
}


/**
 \file formdlgrulesnippet.h

 \class FormDlgRuleSnippet

 \brief Add/Amend/Delete Rulesnippets


*/
class FormDlgRuleSnippet : public QDialog
{
    Q_OBJECT

public:

    /**
    \brief  Constructor
    \param  opCode - whether the dialogue is to be used to add, amend or
            delete a rulesnippet.
    \param  parent - Pointer to configuration form parent
      ***********************/
    explicit FormDlgRuleSnippet(int opCode, FormCfgRuleSnippets *parent = 0);
    ~FormDlgRuleSnippet();

    /**
    \brief set snippet name readonly

    This method can also be used to make the snippet name text box writeable

    \param readOnly - default true
      ****************/
    virtual void setSnippetNameReadOnly(bool readOnly = true);

public slots:


protected:

    QString organization;
    QString application;

    QPointer<FormCfgRuleSnippets> formCfgRuleSnippets;
    int opCode;

    /**
    \brief Sanity check on data
      *******************/
    bool validateData();

    /**
    \brief Write data to table
      *******************/
    bool writeRow();

    // row contents
    int id;
    QString name;
    QString snippets;

protected slots:

    /**
    \brief Changes the enabled state of form buttons if the displayed data is changed
      *******************/
    virtual void dataChanged();

    /**
    \brief Save changes
      ***************/
    virtual void slotBtnSave();

    /**
    \brief Cancel changes
      ***************/
    virtual void slotBtnCancel();

    /**
    \brief Enable/disable buttons dependant on edit state
      ***************/
    virtual void buttonsEnabled(bool enabled);


private:

    Ui::FormDlgRuleSnippet *ui;

};

#endif // FORMDLGRULESNIPPET_H
