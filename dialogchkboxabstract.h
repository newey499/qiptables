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


#ifndef DIALOGCHKBOX_H
#define DIALOGCHKBOX_H

#include <Qt>

#include <QCheckBox>
#include <QDialog>
#include <QLabel>
#include <QLayout>
#include <QPointer>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSettings>
#include <QUuid>


/**
\file dialogchkboxabstract.h
\class DialogChkBoxAbstract

\brief Abstract Class to provide a persistent "Never display this dialog again"
dialog box.

The persistency is provided by assigning a key generated in a subclass and
using this key to uniquely identify the dialog. The generated key is the name
of the subclass

This means that a sub class must be defined for each "Never display this dialog again"
dialog box.


\todo Look at subclassing the QErrorMessage Class and adding persistency to do this.

***************************/
class DialogChkBoxAbstract : public QDialog
{
    Q_OBJECT

public:

    explicit DialogChkBoxAbstract(QWidget *parent = 0);
    explicit DialogChkBoxAbstract(QString organization,
                                  QString application,
                                  QWidget *parent = 0);

    ~DialogChkBoxAbstract();


    virtual QString getText();
    virtual QString getDetailedText();

    virtual bool getTextVisible();
    virtual bool getDetailedTextVisible();

signals:

public slots:

    virtual int exec();

    virtual void setText(QString text);
    virtual void setTextVisible(bool visible);

    virtual void setDetailedText(QString text);
    virtual void setDetailedTextVisible(bool visible);

    virtual void setNeverAgainCheckBox(bool isChecked = false);

protected:

    virtual void commonConstructor(QString organization, QString application);

    virtual void loadSettings(QString key);
    virtual void saveSettings();

    QLayout *thisLayout;

    QVBoxLayout *mainLayout;
    QVBoxLayout *layoutV;
    QHBoxLayout *layoutH;
    QHBoxLayout *layoutNeverShow;

    QCheckBox *chkBox;
    QLabel *lblNeverShow;
    QLabel *lblText;
    QLabel *lblDetailedText;
    QPushButton *btnOk;
    QPushButton *btnCancel;

    QString application;
    QString organization;

    QPointer<QSettings> settings;
    QString chkBoxSettingsKey;

private:

    /**
    \brief Pure virtual method

    Ensures this class cannot be instantiated.

      *********************/
    virtual void forceAbstractClass() = 0;

};

#endif // DIALOGCHKBOX_H
