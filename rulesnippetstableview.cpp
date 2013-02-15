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


