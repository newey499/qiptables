#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
using namespace std;

#include <QAction>
#include <QCloseEvent>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenuItem>
#include <QMessageBox>
#include <QSettings>

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


    void aboutQiptables();
    void aboutQt();

protected:

    virtual void closeEvent(QCloseEvent *event);
    virtual void buildMenuBar();


    QMenu *fileMenu;
    QMenu *toolsMenu;
    QMenu *settingsMenu;
    QMenu *helpMenu;
        QAction *actAboutQt;
        QAction *actAboutQiptables;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
