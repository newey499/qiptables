#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QString organization, QString application, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->organization = organization;
    this->application = application;

    ui->setupUi(this);
    QSettings settings(organization, application);
    restoreGeometry(settings.value("geometry").toByteArray());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    int ret = QMessageBox::warning(this, tr("My Application"),
                                   tr("Exit the Application?"),
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



