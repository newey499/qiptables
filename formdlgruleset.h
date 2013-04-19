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

#include "rulesetsqltablemodel.h"
#include "formcfgruleset.h"
#include "formdlgrulesnippet.h"

class FormCfgRuleset;

namespace Ui {
class FormDlgRuleset;
}

class FormDlgRuleset : public QDialog
{
    Q_OBJECT

public:
    explicit FormDlgRuleset(int opCode, FormCfgRuleset *parent);
    ~FormDlgRuleset();

public slots:

    virtual void slotSave();
    virtual void slotCancel();

protected:

    FormCfgRuleset *formRuleset;
    int id;
    QString name;
    QString rules;
    int opcode;

    bool isDefaultRuleset();
    bool validateData();
    bool writeRow();

protected slots:

    virtual void buttonsEnabled(bool enabled);
    virtual void dataChanged();


private:
    Ui::FormDlgRuleset *ui;
};

#endif // FORMDLGRULESET_H
