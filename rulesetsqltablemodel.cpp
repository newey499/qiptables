#include "rulesetsqltablemodel.h"

RulesetSqlTableModel::RulesetSqlTableModel(QObject *parent) :
    QSqlTableModel(parent)
{
}

bool RulesetSqlTableModel::updateRowInTable(int row, const QSqlRecord &values)
{
    return QSqlTableModel::updateRowInTable(row, values);
}
