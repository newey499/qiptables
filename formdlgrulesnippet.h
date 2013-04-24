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
    explicit FormDlgRuleSnippet(int opCode, FormCfgRuleSnippets *parent = 0);
    ~FormDlgRuleSnippet();

public slots:


protected:

    QString organization;
    QString application;

    QPointer<FormCfgRuleSnippets> formCfgRuleSnippets;
    int opCode;

    bool validateData();
    bool writeRow();

    // row contents
    int id;
    QString name;
    QString snippets;

protected slots:

    virtual void dataChanged();
    virtual void slotBtnSave();
    virtual void slotBtnCancel();

    virtual void buttonsEnabled(bool enabled);


private:

    Ui::FormDlgRuleSnippet *ui;

};

#endif // FORMDLGRULESNIPPET_H
