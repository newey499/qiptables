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

/**
\file formcfgsettings.h
\class FormCfgSettings

\brief Maintain application configuration settings


***************************/
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
    QSqlQuery qry;

    int id;
    QString shell;
    QString iptables;
    QString defaultRuleName;

    virtual void loadSettings();
    virtual void saveSettings();
    virtual QString lastSqlErrorAsString();

private:
    Ui::FormCfgSettings *ui;
};

#endif // FORMCFGSETTINGS_H
