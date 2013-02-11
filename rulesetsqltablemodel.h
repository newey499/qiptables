#ifndef RULESETSQLTABLEMODEL_H
#define RULESETSQLTABLEMODEL_H

#include <QSqlTableModel>

class RulesetSqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit RulesetSqlTableModel(QObject *parent = 0);

    virtual bool updateRowInTable(int row, const QSqlRecord &values);

signals:

public slots:

protected:



};

#endif // RULESETSQLTABLEMODEL_H
