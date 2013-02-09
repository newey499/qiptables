#ifndef FORMCFGSETTINGS_H
#define FORMCFGSETTINGS_H


#include <QFileDialog>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QVariant>
#include <QWidget>

#include "install.h"
#include "databasemanager.h"


namespace Ui {
class FormCfgSettings;
}

class FormCfgSettings : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormCfgSettings(QWidget *parent = 0);
    ~FormCfgSettings();

public slots:

    virtual void slotFileDialog();
    virtual void slotSave();
    virtual void slotCancel();
    virtual void slotButtonStateEnabled();
    virtual void slotButtonStateDisabled();

protected:

    DatabaseManager *databaseManager;
    QSqlQuery *qry;

    int id;
    QString shell;
    QString defaultRuleName;

    virtual void loadSettings();
    virtual void saveSettings();
    virtual QString lastSqlErrorAsString();
    
private:
    Ui::FormCfgSettings *ui;
};

#endif // FORMCFGSETTINGS_H
