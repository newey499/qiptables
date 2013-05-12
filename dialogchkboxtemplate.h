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

#include <QMetaObject>


/**
\file dialogchkboxtemplate.h
\class DialogChkBoxTemplate

\brief Subclass of the Abstract Class DialogChkBoxAbstract to provide a persistent
 "Never display this dialog again" dialog box.

The persistency is provided by assigning a generated key and
using this key to uniquely identify the dialog. The key is the name of this class
with the string "_doNoShowEverAgainCheckBox" appended.

This means that a sub class of DialogChkBoxAbstract (such as this) must be defined
for each "Never display this dialog again" dialog box.

In other words if numerous instances of DialogChkBoxTemplate were instantiated then
the "Never display this dialog again" behaviour would be global across these
instances.

***************************/
class DialogChkBoxTemplate : public DialogChkBoxAbstract
{
    Q_OBJECT

public:

    explicit DialogChkBoxTemplate(QWidget *parent = 0);
    explicit DialogChkBoxTemplate(QString organization,
                                  QString application,
                                  QWidget *parent = 0);


signals:


public slots:


protected:

    void commonConstructor();

private:

    void forceAbstractClass();

};

#endif // DIALOGCHKBOXTEMPLATE_H
