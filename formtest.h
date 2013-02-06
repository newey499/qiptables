#ifndef FORMTEST_H
#define FORMTEST_H

#include <QProcessEnvironment>
#include <QString>
#include <QStringList>
#include <QWidget>


#include "qiptablesexception.h"
#include "linuxuserid.h"
#include "ipprocess.h"
#include "iptables.h"

namespace Ui {
class FormTest;
}

class FormTest : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormTest(QWidget *parent = 0);
    ~FormTest();
    
public slots:

    void slotBtnTest();

private:
    Ui::FormTest *ui;
};

#endif // FORMTEST_H
