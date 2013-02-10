#include "rulesettableview.h"

RulesetTableView::RulesetTableView(QWidget *parent) :
    QTableView(parent)
{
    configureView();
}


void RulesetTableView::configureView()
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    resizeColumnsToContents();
    horizontalHeader()->setStretchLastSection(true);
    selectRow(0);
}

int RulesetTableView::currentRow()
{
    QModelIndex index = selectedIndexes().at(0);
    return index.row();
}

void RulesetTableView::currentChanged(const QModelIndex &current,
                                      const QModelIndex &previous)
{
    QTableView::currentChanged(current, previous);
    selectRow(current.row());
    emit rowChanged(currentRow());
}

/***********
void RulesetTableView::show()
{
    QTableView::show();
    selectRow(currentRow());
}
*************/
