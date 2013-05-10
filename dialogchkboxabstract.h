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

The persistency is provided by assigning a GUID generated in a subclass and
using this GUID to uniquely identify the dialog.

This means that a sub class must be defined for each "Never display this dialog again"
dialog box.


\todo Look at subclassing the QErrorMessage Class and adding persistency to do this.

***************************/
class DialogChkBoxAbstract : public QDialog
{
    Q_OBJECT

public:

    static const QString UNASSIGNED_GUID;
    static QString GUID;

    explicit DialogChkBoxAbstract(QWidget *parent = 0);
    explicit DialogChkBoxAbstract(QString organization,
                                  QString application,
                                  QWidget *parent = 0);

    ~DialogChkBoxAbstract();


    virtual QString getText();
    virtual QString getDetailedText();

    virtual bool getTextVisible();
    virtual bool getDetailedTextVisible();

    /**
    \brief Pure virtual method

    Ensures this class cannot be instantiated.

    \return The generated GUID
      *********************/
    virtual QString createGUID() = 0;

    /**
    \brief Checks to see whether a GUID has been set up

    \return True if GUID set up else False
      **************************/
    virtual bool isGuidAssigned();

    /**
    \brief Displays an error message via qDebug() if a
    GUID has not been set up

    \return QString version of qDebug() output
      **********************/
    virtual QString guidErrMsg();

signals:

public slots:

    virtual int exec();

    virtual void setText(QString text);
    virtual void setTextVisible(bool visible);

    virtual void setDetailedText(QString text);
    virtual void setDetailedTextVisible(bool visible);

protected:

    virtual void commonConstructor(QString organization, QString application);

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

private:


};

#endif // DIALOGCHKBOX_H
