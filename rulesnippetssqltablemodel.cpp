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

/**
 \file rulesnippetssqltablemodel.cpp

 \brief Subclass of QSqlTableModel tweaked for Rulesnippets


*/



#include "rulesnippetssqltablemodel.h"

RuleSnippetsSqlTableModel::RuleSnippetsSqlTableModel(QObject *parent) :
    QSqlTableModel(parent)
{
    readOnly = false;
    connectNameUpperCaseSlot();
}

RuleSnippetsSqlTableModel::RuleSnippetsSqlTableModel(bool readOnly, QObject *parent) :
    QSqlTableModel(parent)
{
    this->readOnly = readOnly;
    if (this->readOnly)
    {
        connect(this, SIGNAL(beforeInsert(QSqlRecord &)),
                this, SLOT(dropChanges()) );
        connect(this, SIGNAL(primeInsert(int, QSqlRecord &)),
                this, SLOT(dropChanges()) );
        connect(this, SIGNAL(beforeUpdate(int, QSqlRecord &)),
                this, SLOT(dropChanges()) );
        connect(this, SIGNAL(beforeDelete(int)),
                this, SLOT(dropChanges()) );
    }
    else
    {
        connectNameUpperCaseSlot();
    }
}


void RuleSnippetsSqlTableModel::connectNameUpperCaseSlot()
{
    connect(this, SIGNAL(beforeInsert(QSqlRecord&)),
            this, SLOT(setNameUpperCase(QSqlRecord&)));
    connect(this, SIGNAL(beforeUpdate(int, QSqlRecord&)),
            this, SLOT(setNameUpperCaseUpdate(int, QSqlRecord&)));
}


bool RuleSnippetsSqlTableModel::updateRowInTable(int row, const QSqlRecord &values)
{
    return QSqlTableModel::updateRowInTable(row, values);
}


void RuleSnippetsSqlTableModel::dropChanges()
{
    //revertAll();
}

void RuleSnippetsSqlTableModel::setNameUpperCaseUpdate(int row, QSqlRecord &record)
{
    row = row; // suppress compiler warning
    setNameUpperCase(record);
}

void RuleSnippetsSqlTableModel::setNameUpperCase(QSqlRecord &record)
{
    qDebug("RuleSnippetsSqlTableModel::setNameUpperCase(QSqlRecord &record)");
    QString name = record.value("name").toString();
    record.setValue("nameupper", QVariant(name.toUpper()));
}

