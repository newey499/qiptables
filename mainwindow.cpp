#include "mainwindow.h"
#include "ui_mainwindow.h"


const QString MainWindow::VERSION_NUMBER = "0.1";

MainWindow::MainWindow(QString organization, QString application, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->organization = organization;
    this->application = application;

    ui->setupUi(this);
    QSettings settings(organization, application);
    restoreGeometry(settings.value("geometry").toByteArray());

    buildMenuBar();

    widgetStack = new IpStackedWidget(this);
    formTest = new FormTest(this);
    formFirewallRules = new FormFirewallRules(this);

    widgetStack->addWidget("formTest", formTest);
    widgetStack->addWidget("formFirewallRules", formFirewallRules);

    widgetStack->setCurrentIndex(widgetStack->getPageIndex("formTest"));

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

    qDebug("MainWindow::closeEvent");
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
    qDebug("void MainWindow::aboutQiptables()");
    QString title = QString("%1 %2 %3").arg(application).
                                        arg(" version: ").
                                        arg(MainWindow::VERSION_NUMBER);
    QString text("iptables firewall configuration switching.");
    QMessageBox::about(this, title, text);
}

void MainWindow::aboutQt()
{
    qDebug("void MainWindow::aboutQt()");
    QMessageBox::aboutQt(this, "Qiptables");
}

void MainWindow::selectFirewallRulesPage()
{
    qDebug("MainWindow::selectFirewallRulesPage()");
    widgetStack->setCurrentIndex(widgetStack->getPageIndex("formFirewallRules"));
}

void MainWindow::selectTestPage()
{
    qDebug("MainWindow::selectTestPage()");
    widgetStack->setCurrentIndex(widgetStack->getPageIndex("formTest"));
}

void MainWindow::quitApplication()
{
    qDebug("MainWindow::quitApplication()");
    int ret = quitYesNo();
    if (ret == QMessageBox::Yes)
    {
        saveSettings();
        qApp->quit();
    }
}

void MainWindow::buildMenuBar()
{
    fileMenu = menuBar()->addMenu(tr("&Firewall"));
        actFirewallRules = new QAction(tr("Firewall &Rules"), this);
        actFirewallRules->setStatusTip(tr("Firewall rules"));
        connect(actFirewallRules, SIGNAL(triggered()), this, SLOT(selectFirewallRulesPage()));
        fileMenu->addAction(actFirewallRules);

        actQuit = new QAction(tr("&Quit"), this);
        actQuit->setStatusTip(tr("Exit Qiptables"));
        connect(actQuit, SIGNAL(triggered()), this, SLOT(quitApplication()));
        fileMenu->addAction(actQuit);


    toolsMenu = menuBar()->addMenu(tr("&Tools"));
        actTest = new QAction(tr("&Test"), this);
        actTest->setStatusTip(tr("Test Page for odds and sods during development"));
        connect(actTest, SIGNAL(triggered()), this, SLOT(selectTestPage()));
        toolsMenu->addAction(actTest);


    settingsMenu = menuBar()->addMenu(tr("&Settings"));

    helpMenu = menuBar()->addMenu(tr("&Help"));

        actAboutQt = new QAction(tr("About &Qt"), this);
        actAboutQt->setStatusTip(tr("About Qt"));
        connect(actAboutQt, SIGNAL(triggered()), this, SLOT(aboutQt()));
        helpMenu->addAction(actAboutQt);

        actAboutQiptables = new QAction(tr("About &Qiptable"), this);
        actAboutQiptables->setStatusTip(tr("About Qiptables"));
        connect(actAboutQiptables, SIGNAL(triggered()), this, SLOT(aboutQiptables()));
        helpMenu->addAction(actAboutQiptables);

}





