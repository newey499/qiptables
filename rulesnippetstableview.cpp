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
 \file rulesnippetstableview.cpp

 \brief Subclass of QTableView tweaked for Rulesnippets

*/

#include "rulesnippetstableview.h"



const int RuleSnippetsTableView::INCLUDE_SNIPPET = 1;
const int RuleSnippetsTableView::PASTE_SNIPPET   = 2;



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

void RuleSnippetsTableView::enableContextMenu(bool enable)
{
    if (enable)
    {
        this->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
                this, SLOT(slotShowContextMenu(const QPoint&)));
    }
}


void RuleSnippetsTableView::slotShowContextMenu(const QPoint & pos)
{
    // for most widgets
    QPoint globalPos = this->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QMenu myMenu;

    QString tmp1 = QString("Include Snippet");
    QString tmp2 = QString("Paste Snippet");

    myMenu.addAction(tmp1);
    myMenu.addAction(tmp2);

    myMenu.actions()[0]->setData(INCLUDE_SNIPPET);
    myMenu.actions()[1]->setData(PASTE_SNIPPET);

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        emit menuItemSelected(selectedItem);
    }
    else
    {
        // nothing was chosen
    }
}

