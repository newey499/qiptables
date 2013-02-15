#ifndef RULESNIPPETSSQLTABLEMODEL_H
#define RULESNIPPETSSQLTABLEMODEL_H

#include <QSqlTableModel>

class RuleSnippetsSqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit RuleSnippetsSqlTableModel(QObject *parent = 0);

    virtual bool updateRowInTable(int row, const QSqlRecord &values);

signals:

public slots:

protected:



};

#endif // RULESNIPPETSSQLTABLEMODEL_H
