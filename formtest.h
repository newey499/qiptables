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


#ifndef FORMTEST_H
#define FORMTEST_H

#include <QFrame>
#include <QPointer>
#include <QProcessEnvironment>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QStringList>
#include <QWidget>


#include "qiptablesexception.h"
#include "linuxuserid.h"
#include "ipprocess.h"
#include "iptables.h"
#include "install.h"
#include "databasemanager.h"
#include "rulesnippet.h"


namespace Ui {
class FormTest;
}


/**
 \file formtest.h

 \class FormTest

 \brief Form for various test routines used during development

 This form is not documented as its used as a testbed during development
 and subject to ad-hoc changes.

*/
class FormTest : public QWidget
{
    Q_OBJECT

public:

    QPointer<DatabaseManager> db;
    QPointer<Iptables> iptables;
    QPointer<IpProcess> proc;


    explicit FormTest(QWidget *parent = 0);
    ~FormTest();




public slots:

    virtual void slotBtnTest();
    virtual void slotBtnRun();
    virtual void slotIptablesList();

protected:

    QPointer<RuleSnippet> ruleSnippet;


protected slots:

    void slotCmdOutput(QString program, QStringList arguments, int exitCode, QString result);
    void slotRunRuleset();

private:
    Ui::FormTest *ui;
};

#endif // FORMTEST_H
