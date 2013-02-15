#ifndef RULESNIPPETSTABLEVIEW_H
#define RULESNIPPETSTABLEVIEW_H

#include <QHeaderView>
#include <QModelIndex>
#include <QTableView>

class RuleSnippetsTableView : public QTableView
{
    Q_OBJECT
public:
    explicit RuleSnippetsTableView(QWidget *parent = 0);

    virtual void configureView();
    virtual int currentRow();

signals:

    void rowChanged(QModelIndex);

public slots:

    virtual void currentChanged(const QModelIndex &current,
                                const QModelIndex &previous );

protected:


};

#endif // RULESNIPPETSTABLEVIEW_H
