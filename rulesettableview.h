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

    void rowChanged(int);
    
public slots:

    virtual void currentChanged(const QModelIndex &current,
                                const QModelIndex &previous );
    
};

#endif // RULESETTABLEVIEW_H
