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


#ifndef DIALOGCHKBOXTEMPLATE_H
#define DIALOGCHKBOXTEMPLATE_H

#include "dialogchkboxabstract.h"


/**
\file dialogchkboxtemplate.h
\class DialogChkBoxTemplate

\brief Subclass of the Abstract Class DialogChkBoxAbstract to provide a persistent
 "Never display this dialog again" dialog box.

The persistency is provided by assigning a generated GUID and
using this GUID to uniquely identify the dialog.

This means that a sub class of DialogChkBoxAbstract (such as this) must be defined
for each "Never display this dialog again" dialog box.

***************************/
class DialogChkBoxTemplate : public DialogChkBoxAbstract
{
    Q_OBJECT

public:

    explicit DialogChkBoxTemplate(QWidget *parent = 0);
    explicit DialogChkBoxTemplate(QString organization,
                                  QString application,
                                  QWidget *parent = 0);

    /**
    \brief execs the dialog

    If the class has not been set up with a GUID then one is
    generated and displayed with qDebug().

    \return Result from executing dialog - either QDialog::Accepted or
    QDialog::Rejected
      ***********************/
    virtual int exec();

    /**
    \brief Generate a GUID and display it using qDebug()

    \return Generated GUID
      ******************/
    virtual QString createGUID();

signals:


public slots:


protected:

    void commonConstructor();

private:

};

#endif // DIALOGCHKBOXTEMPLATE_H
