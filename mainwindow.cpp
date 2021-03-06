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

/**
 \file mainwindow.cpp

 \brief Main Window of application


*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "genlib.h"

const QString MainWindow::VERSION_NUMBER = "0.2";
const QString MainWindow::FIREWALL_SUB_MENU_NAME = "fireWallSubMenu";
QString MainWindow::ORGANIZATION = "";
QString MainWindow::APPLICATION  = "";


MainWindow::MainWindow(QString organization, QString application, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setObjectName("MainWindow");
    this->organization = organization;
    this->application = application;

    MainWindow::ORGANIZATION = this->organization;
    MainWindow::APPLICATION = this->application;


    ui->setupUi(this);
    QSettings settings(organization, application);
    restoreGeometry(settings.value("geometry").toByteArray());

    Install *install = new Install(this);
    QString instStr(install->performInstall());

    DatabaseManager dm(Install::INSTALL_DIR, this);
    dm.openDB();

    genLib = new GenLib(this);

    buildMenusAndForms();

    this->setCentralWidget(widgetStack);

}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::quitYesNo()
{
    QString exit = QString("Exit ");
    exit = exit.append(this->application);

    int ret = QMessageBox::question(this, tr(this->application.toAscii().data()),
                                   tr(exit.toAscii().data()),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::Yes);

    return ret;
}

void MainWindow::saveSettings()
{
    QSettings settings(organization,application);
    settings.setValue("geometry", saveGeometry());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int ret = quitYesNo();

    if (ret == QMessageBox::Yes)
    {
        saveSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}


void MainWindow::aboutQiptables()
{
    QString title = QString("%1 %2 %3").arg(application).
                                        arg(" version: ").
                                        arg(MainWindow::VERSION_NUMBER);
    QString text("iptables firewall configuration switching.");
    QMessageBox::about(this, title, text);
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this, "Qiptables");
}

void MainWindow::quitApplication()
{
    int ret = quitYesNo();
    if (ret == QMessageBox::Yes)
    {
        saveSettings();
        qApp->quit();
    }
}



void MainWindow::buildMenusAndForms()
{
    widgetStack = new IpStackedWidget(this);
    formTest = new FormTest(this);
    formFirewallRules = new FormFirewallRules(this);
    // tabbed widget  - no longer used
    // formConfigQiptables = new FormConfigQiptables(this);
    FormCfgRuleset *formCfgRuleset = new FormCfgRuleset(this);
    FormCfgRuleSnippets *formCfgRuleSnippets = new FormCfgRuleSnippets(this);
    FormCfgSettings *formCfgSettings = new FormCfgSettings(this);

    fileMenu = menuBar()->addMenu(tr("&Firewall"));

        actFirewallRules = new QAction(tr("&Firewall Rules"), this);
        actFirewallRules->setStatusTip(tr("Firewall rules"));
        connect(actFirewallRules, SIGNAL(triggered()),
                this, SLOT(selectFirewallRulesPage()));
        fileMenu->addAction(actFirewallRules);

        fileMenu->addSeparator();

        QMenu *subMenu = new QMenu("Firewall Rulesets", this);
        subMenu->setObjectName(MainWindow::FIREWALL_SUB_MENU_NAME);
        subMenu->setTitle("&Select Ruleset");
        fileMenu->addMenu(subMenu);

        addFirewallMenuOptions();

        fileMenu->addSeparator();

        actQuit = new QAction(tr("&Quit"), this);
        actQuit->setStatusTip(tr("Exit Qiptables"));
        connect(actQuit, SIGNAL(triggered()), this, SLOT(quitApplication()));
        fileMenu->addAction(actQuit);


    toolsMenu = menuBar()->addMenu(tr("&Tools"));

        actTest = new QAction(tr("&Test"), this);
        actTest->setStatusTip(tr("Test Page for odds and sods during development"));
        connect(actTest, SIGNAL(triggered()), this, SLOT(selectTestPage()));
        toolsMenu->addAction(actTest);

        /*********
        No longer used
        actFwSetUp = new QAction(tr("&Firewall Settings"), this);
        actFwSetUp->setStatusTip(tr("Firewall settings for different environments"));
        connect(actFwSetUp, SIGNAL(triggered()), this, SLOT(selectFwSetUp()));
        toolsMenu->addAction(actFwSetUp);
        ******************/

    settingsMenu = menuBar()->addMenu(tr("&Settings"));

        actCfgRuleset = new QAction(tr("&Rulesets"), this);
        actCfgRuleset->setStatusTip(tr("Configure Rulesets"));
        connect(actCfgRuleset, SIGNAL(triggered()), this, SLOT(selectCfgRuleset()));
        settingsMenu->addAction(actCfgRuleset);

        actCfgRuleSnippet = new QAction(tr("Rule &Snippets"), this);
        actCfgRuleSnippet->setStatusTip(tr("Configure Rule Snippets"));
        connect(actCfgRuleSnippet, SIGNAL(triggered()), this, SLOT(selectCfgRuleSnippet()));
        settingsMenu->addAction(actCfgRuleSnippet);


        actCfgRuleSettings = new QAction(tr("&Defaults"), this);
        actCfgRuleSettings->setStatusTip(tr("Configure Defaults"));
        connect(actCfgRuleSettings, SIGNAL(triggered()), this, SLOT(selectCfgSettings()));
        settingsMenu->addAction(actCfgRuleSettings);


        /***************
        actConfigQiptables = new QAction(tr("&Defaults"), this);
        actConfigQiptables->setStatusTip(tr("Configure Defaults"));
        connect(actConfigQiptables, SIGNAL(triggered()),
                this, SLOT(selectConfigQiptables()));
        settingsMenu->addAction(actConfigQiptables);
        ****************************/

    helpMenu = menuBar()->addMenu(tr("&Help"));
        actAboutQt = new QAction(tr("About &Qt"), this);
        actAboutQt->setStatusTip(tr("About Qt"));
        connect(actAboutQt, SIGNAL(triggered()),
                this, SLOT(aboutQt()));
        helpMenu->addAction(actAboutQt);

        actAboutQiptables = new QAction(tr("About &Qiptable"), this);
        actAboutQiptables->setStatusTip(tr("About Qiptables"));
        connect(actAboutQiptables, SIGNAL(triggered()),
                this, SLOT(aboutQiptables()));
        helpMenu->addAction(actAboutQiptables);




    widgetStack->addWidget("formTest", formTest);
    widgetStack->addWidget("formFirewallRules", formFirewallRules);

    // Tabbed Window - no longer used
    // widgetStack->addWidget("formConfigQiptables", formConfigQiptables);

    widgetStack->addWidget("formCfgRuleset",     formCfgRuleset);
    widgetStack->addWidget("formCfgRuleSnippet", formCfgRuleSnippets);
    widgetStack->addWidget("formCfgSettings",    formCfgSettings);

    widgetStack->setCurrentIndex(widgetStack->getPageIndex("formTest"));
}




void MainWindow::addFirewallMenuOptions()
{
    QString title;
    QSqlQuery qry;

    GenLib *genLib = new GenLib();
    MainWindow *mainWindowPtr = ( MainWindow *) genLib->getWidgetPointer("MainWindow");
    QMenu * subMenuPtr = (QMenu *) genLib->getWidgetPointer(MainWindow::FIREWALL_SUB_MENU_NAME);

    subMenuPtr->clear();

    if (qry.exec("select id, name, shortname, rules from ruleset"))
    {
        while(qry.next())
        {
            title = qry.record().value("name").toString();
            //QAction *tmpAction = new QAction(title, subMenuPtr);
            MenuAction *tmpAction = new MenuAction(title, subMenuPtr);

            tmpAction->setData(qry.record().value("shortname").toString());
            //tmpAction->setStatusTip(tr(title));
            //connect(tmpAction, SIGNAL(triggered()),
            //        this, SLOT(selectFirewallRulesPage()));
            connect(tmpAction, SIGNAL(menuActionSelected(MenuAction *)),
                    mainWindowPtr, SLOT(processSelectedAction(MenuAction *)));
            //fileMenu->addAction(tmpAction);
            subMenuPtr->addAction(tmpAction);
        }
    }
    else
    {
        qDebug("MainWindow::addFirewallMenuOptions() - Query Error\n%s",
               qry.lastError().text().toAscii().data());
    }

}


void MainWindow::processSelectedAction(MenuAction *action)
{
    qDebug("=====================================");
    qDebug("Action selected slot");
    qDebug("[%s] [%s]",
           action->text().toAscii().data(),
           action->data().toString().toAscii().data());
    qDebug("=====================================");

    selectFirewallRulesPage();
    formFirewallRules->setCbxFirewallsText(action->text());
}



void MainWindow::selectFwSetUp()
{
    widgetStack->setCurrentIndex(widgetStack->getPageIndex("formFirewallRules"));
}

void MainWindow::selectCfgRuleset()
{
    widgetStack->setCurrentIndex(widgetStack->getPageIndex("formCfgRuleset"));
}


void MainWindow::selectCfgRuleSnippet()
{
    widgetStack->setCurrentIndex(widgetStack->getPageIndex("formCfgRuleSnippet"));
}


void MainWindow::selectCfgSettings()
{
    int pageIndex = widgetStack->getPageIndex("formCfgSettings");
    widgetStack->setCurrentIndex(pageIndex);
}



void MainWindow::selectFirewallRulesPage()
{
    widgetStack->setCurrentIndex(widgetStack->getPageIndex("formFirewallRules"));
}

void MainWindow::selectTestPage()
{
    widgetStack->setCurrentIndex(widgetStack->getPageIndex("formTest"));
}
