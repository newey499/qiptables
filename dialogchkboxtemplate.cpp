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


#include "dialogchkboxtemplate.h"


DialogChkBoxTemplate::DialogChkBoxTemplate(QWidget *parent) :
    DialogChkBoxAbstract(parent)
{
    commonConstructor();
}


DialogChkBoxTemplate::DialogChkBoxTemplate(QString organization,
                                           QString application,
                                           QWidget *parent) :
    DialogChkBoxAbstract(organization, application, parent)
{
    commonConstructor();
}

void DialogChkBoxTemplate::commonConstructor()
{
    // Use newly generated GUID here
    // See method "DialogChkBoxTemplate::createGUID()"
    // for GUID Generation.
    //GUID = "{50f73000-e5ff-462c-ac6a-b35f5de1c907}";
}

int DialogChkBoxTemplate::exec()
{
    if (! isGuidAssigned())
    {
        createGUID();
    }
    return DialogChkBoxAbstract::exec();
}


QString DialogChkBoxTemplate::createGUID()
{
    QUuid guid = QUuid::createUuid();

    QString guidStr = guid.toString();

    qDebug("==========================================");
    qDebug("QString DialogChkBoxAbstract::createGUID()");
    qDebug("New GUID %s", guidStr.toAscii().data());
    qDebug("Paste this newly generated GUID into the");
    qDebug("variable");
    qDebug("const QString DialogChkBoxAbstract::GUID");
    qDebug("defined at the top of the file");
    qDebug("\"dialogchkboxabstract.cpp\"");
    qDebug("==========================================");

    return guidStr;
}
