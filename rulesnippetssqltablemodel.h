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


#ifndef RULESNIPPETSSQLTABLEMODEL_H
#define RULESNIPPETSSQLTABLEMODEL_H

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QVariant>


/**
 \file rulesnippetssqltablemodel.h

 \class RuleSnippetsSqlTableModel

 \brief Subclass of QSqlTableModel tweaked for Rulesnippets


*/
class RuleSnippetsSqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit RuleSnippetsSqlTableModel(QObject *parent = 0);

    /**
    \brief Constructor that allows model to be set up as readonly
      **************/
    explicit RuleSnippetsSqlTableModel(bool readOnly, QObject *parent = 0);

    /**
    \brief updates rulesnippet row

    \param row - row number (primary key of table)
    \param values - row data

    \return true if successful else false
      ************************/
    virtual bool updateRowInTable(int row, const QSqlRecord &values);

    bool readOnly;

signals:

public slots:

    /**
    \brief redundant debug code - does nothing except issue a qDebug message
      ******************/
    virtual void dropChanges();

    virtual void connectNameUpperCaseSlot();

    virtual void setNameUpperCase(QSqlRecord &record);
    virtual void setNameUpperCaseUpdate(int row, QSqlRecord &record);

protected:





};

#endif // RULESNIPPETSSQLTABLEMODEL_H
