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



#ifndef RULESETTABLEVIEW_H
#define RULESETTABLEVIEW_H

#include <QHeaderView>
#include <QTableView>

class RulesetTableView : public QTableView
{
    Q_OBJECT
public:
    explicit RulesetTableView(QWidget *parent = 0);

    virtual void configureView();
    virtual int currentRow();

signals:

    void rowChanged(QModelIndex);

public slots:

    virtual void currentChanged(const QModelIndex &current,
                                const QModelIndex &previous );

};

#endif // RULESETTABLEVIEW_H
