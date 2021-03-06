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


#ifndef IPSTACKEDWIDGET_H
#define IPSTACKEDWIDGET_H

#include <QMap>
#include <QStackedWidget>
#include "qiptablesexception.h"

/**
 \file ipstackedwidget.h

 \class IpStackedWidget

 \brief Class to contain application forms


*/
class IpStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:

    explicit IpStackedWidget(QWidget *parent = 0);

    /**
    \brief Adds a widget to IpStackedWidget

    \param widgetName - name to identify widget
    \param widget - pointer to widget to add

    \return index of newly added widget
      **************************/
    virtual int	addWidget(QString widgetName, QWidget *widget);

    /**
    \brief get index number of widget using widget name

    \param widgetName - name of widget as used when calling addWidget method

    \return index number of widget
      **********************/
    virtual int getPageIndex(QString widgetName);


signals:

public slots:

protected:

     QMap<QString, int> *widgetMap;

};

#endif // IPSTACKEDWIDGET_H
