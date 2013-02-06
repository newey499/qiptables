#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "qiptablesexception.h"


#include <QAction>
#include <QCloseEvent>
#include <QEvent>
#include <QMainWindow>
#include <QMap>
#include <QMenuBar>
#include <QMenuItem>
#include <QMessageBox>
#include <QSettings>
#include <QStackedWidget>

#include "formtest.h"
#include "formfirewallrules.h"
#include "ipstackedwidget.h"
#include "linuxuserid.h"
#include "ipprocess.h"

class FormTest;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    static const QString VERSION_NUMBER;

    explicit MainWindow(QString organization = "git:bitbucket.org:newey499.qiptables.git",
                        QString application  = "Qiptables",
                        QWidget *parent = 0);
    ~MainWindow();

    QString organization;
    QString application;

public slots:

    virtual void quitApplication();
    virtual void aboutQiptables();
    virtual void aboutQt();
    virtual void selectTestPage();
    virtual void selectFirewallRulesPage();
    virtual void saveSettings();

protected:

    virtual void closeEvent(QCloseEvent *event);

    virtual int quitYesNo();
    virtual void buildMenuBar();

    IpStackedWidget *widgetStack;
    FormTest *formTest;
    FormFirewallRules *formFirewallRules;


    QMenu *fileMenu;
            QAction *actFirewallRules;
            QAction *actQuit;

    QMenu *toolsMenu;
            QAction *actTest;

    QMenu *settingsMenu;

    QMenu *helpMenu;
        QAction *actAboutQt;
        QAction *actAboutQiptables;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
