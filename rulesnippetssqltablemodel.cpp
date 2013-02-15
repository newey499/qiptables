#include "rulesnippetssqltablemodel.h"

RuleSnippetsSqlTableModel::RuleSnippetsSqlTableModel(QObject *parent) :
    QSqlTableModel(parent)
{
}

bool RuleSnippetsSqlTableModel::updateRowInTable(int row, const QSqlRecord &values)
{
    return QSqlTableModel::updateRowInTable(row, values);
}
