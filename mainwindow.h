/************************************************************************
Copyright Chris Newey 2013

qiptables@hotmail.com

This file is part of Qiptables.

Qiptables is distributed under the terms of the GNU General Public License

Qiptables is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Qiptables is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Qiptables.  If not, see <http://www.gnu.org/licenses/>.

***************************************************************************/



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
    //virtual void selectConfigQiptables();
    virtual void selectCfgRuleset();
    virtual void selectCfgRuleSnippet();
    //virtual void selectCfgSettings();
    virtual void saveSettings();

signals:


protected:

    virtual void closeEvent(QCloseEvent *event);

    virtual int quitYesNo();
    virtual void buildMenusAndForms();

    IpStackedWidget *widgetStack;
    FormTest *formTest;
    FormFirewallRules *formFirewallRules;
    FormConfigQiptables *formConfigQiptables;
    FormCfgRuleset *formCfgRuleset;
    FormCfgRuleSnippets *formCfgRuleSnippets;
    FormCfgSettings *formCfgSettings;


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
