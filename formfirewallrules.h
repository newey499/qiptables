#ifndef FORMFIREWALLRULES_H
#define FORMFIREWALLRULES_H



#include <QListView>
#include <QPointer>
#include <QTreeView>
#include <QTextEdit>
#include <QWidget>


#include "install.h"
#include "qiptablesexception.h"
#include "databasemanager.h"


namespace Ui {
class FormFirewallRules;
}

class DatabaseManager;

class FormFirewallRules : public QWidget
{
    Q_OBJECT

public:
    explicit FormFirewallRules(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~FormFirewallRules();

signals:

public slots:


protected:

    QPointer<DatabaseManager> databaseManager;



private:
    Ui::FormFirewallRules *ui;
};

#endif // FORMFIREWALLRULES_H
