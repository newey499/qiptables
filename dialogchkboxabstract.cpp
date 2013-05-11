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


#include "dialogchkboxabstract.h"

QString application;
QString organization;

DialogChkBoxAbstract::DialogChkBoxAbstract(QWidget *parent) :
    QDialog(parent)
{
    commonConstructor("organization", "application");
}

DialogChkBoxAbstract::DialogChkBoxAbstract(QString organization,
                                           QString application,
                                           QWidget *parent) :
    QDialog(parent)
{
    commonConstructor(organization, application);
}


void DialogChkBoxAbstract::commonConstructor(QString organization, QString application)
{
    this->organization = organization;
    this->application = application;

    settings = new QSettings(organization, application, this);

    setSizeGripEnabled(false);

    chkBox = new QCheckBox(this);
    chkBox->setChecked(false);

    lblText = new QLabel(this);
    lblText->setWordWrap(true);
    lblText->setMargin(10);
    lblText->setText("<b>Label Text</b>");

    lblDetailedText = new QLabel(this);
    lblDetailedText->setWordWrap(true);
    lblDetailedText->setMargin(10);
    lblDetailedText->setText("Label Detailed Text line 1\n"
                             "Label Detailed Text line 2\n"
                             "Label Detailed Text line 3\n"
                             "Label Detailed Text line 4\n"
                             "Label Detailed Text line 5\n"
                             "Label Detailed Text line 6"
                             );

    lblNeverShow = new QLabel(this);
    lblNeverShow->setText("Never show this Dialogue again");

    btnOk = new QPushButton(this);
    btnOk->setText("&Ok");
    connect(btnOk, SIGNAL(clicked()),
            this, SLOT(accept()));


    btnCancel = new QPushButton(this);
    btnCancel->setText("&Cancel");
    connect(btnCancel, SIGNAL(clicked()),
            this, SLOT(reject()));

    mainLayout = new QVBoxLayout();
    layoutH = new QHBoxLayout();
    layoutV = new QVBoxLayout();

    layoutV->setMargin(0);
    layoutV->addWidget(lblText);
    layoutV->addWidget(lblDetailedText);

    layoutNeverShow = new QHBoxLayout();

    layoutH->addWidget(lblNeverShow);
    layoutH->addWidget(chkBox);
    layoutH->addWidget(btnOk);
    layoutH->addWidget(btnCancel);
    layoutH->insertStretch(0);


    mainLayout->addLayout(layoutV);
    mainLayout->addLayout(layoutH);

    this->setLayout(mainLayout);

    settings = new QSettings (this->organization, this->application, this);

    //this->setMinimumWidth(300);
}

DialogChkBoxAbstract::~DialogChkBoxAbstract()
{
    saveSettings();
}





QString DialogChkBoxAbstract::getText()
{
   return lblText->text();
}

void DialogChkBoxAbstract::setText(QString text)
{
   lblText->setText(text);
}

QString DialogChkBoxAbstract::getDetailedText()
{
    return lblDetailedText->text();
}

void DialogChkBoxAbstract::setDetailedText(QString text)
{
    lblDetailedText->setText(text);
}

void DialogChkBoxAbstract::setTextVisible(bool visible)
{
    lblText->setVisible(visible);
}

void DialogChkBoxAbstract::setDetailedTextVisible(bool visible)
{
    lblDetailedText->setVisible(visible);
}

bool DialogChkBoxAbstract::getTextVisible()
{
    return lblText->isVisible();
}

bool DialogChkBoxAbstract::getDetailedTextVisible()
{
    return lblDetailedText->isVisible();
}


int DialogChkBoxAbstract::exec()
{

    if (chkBox->isChecked())
    {
        qDebug("\nNever display again check box is checked - "
               "don't display and return QDialog::Accepted");
        return QDialog::Accepted;
    }

    qDebug("========================");
    qDebug("int DialogChkBox::exec()");

    int result = QDialog::exec();

    switch (result)
    {
        case QDialog::Accepted :
            qDebug("result = QDialog::Accepted");
            break;


        case QDialog::Rejected :
            qDebug("result = QDialog::Rejected");
            break;

        default:
            qDebug("result = unexpected [%d]", result);
            break;
    }

    qDebug("========================");

    return result;
}


void DialogChkBoxAbstract::loadSettings(QString key)
{
    qDebug("void DialogChkBoxAbstract::loadSettings()");
    chkBoxSettingsKey = key.append("_doNoShowEverAgainCheckBox");
    bool checked = settings->value(chkBoxSettingsKey, false).toBool();
    chkBox->setChecked(checked);

    qDebug("key [%s] Do not show again CheckBox Checked [%s]",
           chkBoxSettingsKey.toAscii().data(),
           chkBox->isChecked() ? QString("Yes").toAscii().data() :
                                 QString("No").toAscii().data());
}


void DialogChkBoxAbstract::saveSettings()
{
    qDebug("void DialogChkBoxAbstract::saveSettings()");

    // Debug force clear of "never display again" checkbox
    //chkBox->setChecked(false);

    settings->setValue(chkBoxSettingsKey, chkBox->isChecked());

    qDebug("key [%s] Do not show again CheckBox Checked [%s]",
           chkBoxSettingsKey.toAscii().data(),
           chkBox->isChecked() ? QString("Yes").toAscii().data() :
                                 QString("No").toAscii().data());
}


void DialogChkBoxAbstract::setNeverAgainCheckBox(bool isChecked)
{
    chkBox->setChecked(isChecked);
}




