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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    QString exit = QString("Exit ");
    exit = exit.append(this->application);

    int ret = QMessageBox::question(this, tr(this->application.toAscii().data()),
                                   tr(exit.toAscii().data()),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::Yes);
    if (ret == QMessageBox::Yes)
    {
        QSettings settings(organization,application);
        settings.setValue("geometry", saveGeometry());
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

void MainWindow::buildMenuBar()
{
    fileMenu = menuBar()->addMenu(tr("&Firewall"));

    toolsMenu = menuBar()->addMenu(tr("&Tools"));

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
