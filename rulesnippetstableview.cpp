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




#include "rulesnippetstableview.h"

RuleSnippetsTableView::RuleSnippetsTableView(QWidget *parent) :
    QTableView(parent)
{
    configureView();
}


void RuleSnippetsTableView::configureView()
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    resizeColumnsToContents();
    horizontalHeader()->setStretchLastSection(true);
    selectRow(0);
}

int RuleSnippetsTableView::currentRow()
{
    QModelIndex index = selectedIndexes().at(0);
    return index.row();
}

void RuleSnippetsTableView::currentChanged(const QModelIndex &current,
                                      const QModelIndex &previous)
{
    QTableView::currentChanged(current, previous);
    selectRow(current.row());
    emit rowChanged(current);
}

/***********
void RuleSnippetsTableView::show()
{
    QTableView::show();
    selectRow(currentRow());
}
*************/


/*******************
void RuleSnippetsTableView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragStartPosition = event->pos();
    }
    event->ignore();
}
**************************/

/*************************
void RuleSnippetsTableView::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength()
         < QApplication::startDragDistance() )
    {
        return;
    }

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    //mimeData->setData(mimeType, data);
    mimeData->setText("QQQQQQQQQQQQQQQQQQ");
    drag->setMimeData(mimeData);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);

}
**********************************/
