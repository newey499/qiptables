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
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStackedWidget>
#include <QVariant>

#include "qiptablesexception.h"
#include "formtest.h"
#include "formfirewallrules.h"
#include "formconfigqiptables.h"
#include "ipstackedwidget.h"
#include "linuxuserid.h"
#include "ipprocess.h"
#include "menuaction.h"

class GenLib;

namespace Ui {
class MainWindow;
}

/**
 \file mainwindow.h

 \class MainWindow

 \brief Main Window of application


*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    static const QString VERSION_NUMBER;
    static const QString FIREWALL_SUB_MENU_NAME;
    static QString ORGANIZATION;
    static QString APPLICATION;

    /**
    \brief build firewall menu

    The firewall menu is built in this method as the contents of this menu changes
    whenever a ruleset is added/amended/deleted.
      *******************************/
    static void addFirewallMenuOptions();


    /**
    \brief Constructor the organization and application are used to uniquely identify
           the application when storing window size and position so that the windows
           can be restored.

      **************************/
    explicit MainWindow(QString organization = "git:bitbucket.org:newey499.qiptables.git",
                        QString application  = "Qiptables",
                        QWidget *parent = 0);
    ~MainWindow();

    QString organization;
    QString application;

public slots:

    /**
    \brief Message Box popped to confirm exit from application
      *************/
    virtual void quitApplication();

    /**
    \brief About box for application
    ****************/
    virtual void aboutQiptables();

    /**
    \brief About box for QT
    ****************/
    virtual void aboutQt();

    /**
    \brief selects development test page
      *************/
    virtual void selectTestPage();

    /**
    \brief selects firewall set up page
      *************/
    virtual void selectFwSetUp();

    /**
    \brief selects firewall Rules page
      *************/
    virtual void selectFirewallRulesPage();

    //virtual void selectConfigQiptables();

    /**
    \brief selects ruleset configuration page
      *************/
    virtual void selectCfgRuleset();

    /**
    \brief selects rulesnippet configuration page
      *************/
    virtual void selectCfgRuleSnippet();

    /**
    \brief selects application configuration page
      ****************/
    virtual void selectCfgSettings();


    /**
    \brief save form size and position on exit
      **************/
    virtual void saveSettings();


    virtual void processSelectedAction(MenuAction *action);

signals:


protected:

    /**
    \brief called by QT when request is made to exit the application
      ***************/
    virtual void closeEvent(QCloseEvent *event);

    /**
    \brief Message Box that asks for confirmation of application exit
      ******************/
    virtual int quitYesNo();

    /**
    \brief Create menus and forms used by application
      ************************/
    virtual void buildMenu
    sAndForms();

    QPointer<IpStackedWidget> widgetStack;
    QPointer<FormTest> formTest;
    QPointer<FormFirewallRules> formFirewallRules;
    QPointer<FormConfigQiptables> formConfigQiptables;
    QPointer<FormCfgRuleset> formCfgRuleset;
    QPointer<FormCfgRuleSnippets> formCfgRuleSnippets;
    QPointer<FormCfgSettings> formCfgSettings;
    QPointer<GenLib> genLib;


    QPointer<QMenu> fileMenu;
        QPointer<QAction> actFirewallRules;
        QPointer<QAction> actQuit;

    QPointer<QMenu> toolsMenu;
        QPointer<QAction> actTest;
        QPointer<QAction> actFwSetUp;

    QPointer<QMenu> settingsMenu;
        QPointer<QAction> actConfigQiptables;
        QPointer<QAction> actCfgRuleset;
        QPointer<QAction> actCfgRuleSnippet;
        QPointer<QAction> actCfgRuleSettings;

    QPointer<QMenu> helpMenu;
        QPointer<QAction> actAboutQt;
        QPointer<QAction> actAboutQiptables;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
