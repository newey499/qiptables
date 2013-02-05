#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QString organization = "git:bitbucket.org:newey499.qiptables.git",
                        QString application  = "qiptables",
                        QWidget *parent = 0);
    ~MainWindow();

    QString organization;
    QString application;

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
