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



const QString DialogChkBoxAbstract::UNASSIGNED_GUID = "GUID not assigned";
QString DialogChkBoxAbstract::GUID = DialogChkBoxAbstract::UNASSIGNED_GUID;


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
    lblDetailedText->setText("Label Detailed Text");

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

    if (! isGuidAssigned())
    {
        qDebug("\nDialogChkBoxAbstract::exec()");
        guidErrMsg();
        return QDialog::Rejected;
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


bool DialogChkBoxAbstract::isGuidAssigned()
{
    bool result;

    result = QString(GUID).
                compare(UNASSIGNED_GUID, Qt::CaseSensitive) != 0;

    return result;
}


QString DialogChkBoxAbstract::guidErrMsg()
{
    QStringList sl;
    QString result;

    sl.append("============================");
    sl.append("GUID not assigned");
    sl.append("This class will not work");
    sl.append("without an assigned GUID.");
    sl.append("See Files:");
    sl.append("\tdialogchkboxabstract.h, dialogchkboxabstract.h");
    sl.append("\tdialogchkboxtemplate.h, dialogchkboxtemplate.h");
    sl.append("============================");

    result = sl.join("\n");
    qDebug("%s\n", result.toAscii().data());

    return result;
}






