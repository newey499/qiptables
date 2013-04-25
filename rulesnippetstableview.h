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



#ifndef RULESNIPPETSTABLEVIEW_H
#define RULESNIPPETSTABLEVIEW_H

#include <QApplication>
#include <QDrag>
#include <QHeaderView>
#include <QMenu>
#include <QMimeData>

#include <QModelIndex>
#include <QMouseEvent>
#include <QPoint>
#include <QTableView>
#include <QVariant>


/**
 \file rulesnippetstableview.h

 \class RuleSnippetsTableView

 \brief Subclass of QTableView tweaked for Rulesnippets


*/
class RuleSnippetsTableView : public QTableView
{
    Q_OBJECT
public:

    static const int INCLUDE_SNIPPET;
    static const int PASTE_SNIPPET;

    explicit RuleSnippetsTableView(QWidget *parent = 0);

    /**
    \brief Configures view
      ***************/
    virtual void configureView();

    /**
    \brief Get current row number

    \return number of current row
      **********************/
    virtual int currentRow();

    /**
    \brief enable/disable context menu activated by right mouse button click
      *********************/
    virtual void enableContextMenu(bool enable = true);

signals:

    /**
    \brief prpogate the index of the row that has been moved to
      ******************/
    void rowChanged(QModelIndex);

    /**
    \brief propogates selected context menu option
      ******************/
    void menuItemSelected(QAction *);

public slots:

    /**
    \brief Selects <current> row and emits rowChanged signal passing <current> row
      ***************************/
    virtual void currentChanged(const QModelIndex &current,
                                const QModelIndex &previous );

    /**
    \brief builds and displays context menu as a result of right mouse button click

    The menu is positioned with its top left corner at <pos>
      *********************/
    virtual void slotShowContextMenu(const QPoint & pos);

protected:

};

#endif // RULESNIPPETSTABLEVIEW_H
