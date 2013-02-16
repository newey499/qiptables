#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QAction>
#include <QCloseEvent>
#include <QEvent>
#include <QMainWindow>
#include <QMap>
#include <QMenuBar>
#include <QMenuItem>
#include <QMessageBox>
#include <QPointer>
#include <QSettings>
#include <QStackedWidget>

#include "qiptablesexception.h"
#include "formtest.h"
#include "formfirewallrules.h"
#include "formconfigqiptables.h"
#include "ipstackedwidget.h"
#include "linuxuserid.h"
#include "ipprocess.h"


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
    virtual void selectFwSetUp();
    virtual void selectFirewallRulesPage();
    virtual void selectConfigQiptables();
    virtual void selectCfgRuleset();
    virtual void selectCfgRuleSnippet();
    //virtual void selectCfgSettings();
    virtual void saveSettings();

protected:

    virtual void closeEvent(QCloseEvent *event);

    virtual int quitYesNo();
    virtual void buildMenuBar();

    IpStackedWidget *widgetStack;
    FormTest *formTest;
    FormFirewallRules *formFirewallRules;
    FormConfigQiptables *formConfigQiptables;


    QMenu *fileMenu;
        QAction *actFirewallRules;
        QAction *actQuit;

    QMenu *toolsMenu;
        QAction *actTest;
        QAction *actFwSetUp;

    QMenu *settingsMenu;
        QAction *actConfigQiptables;
        QAction *actCfgRuleset;
        QAction *actCfgRuleSnippet;
        QAction *actCfgRuleSettings;

    QMenu *helpMenu;
        QAction *actAboutQt;
        QAction *actAboutQiptables;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
