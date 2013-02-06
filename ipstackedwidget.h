#ifndef IPSTACKEDWIDGET_H
#define IPSTACKEDWIDGET_H

#include <QMap>
#include <QStackedWidget>
#include "qiptablesexception.h"

class IpStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit IpStackedWidget(QWidget *parent = 0);
    
    virtual int	addWidget(QString widgetName, QWidget *widget);

    virtual int getPageIndex(QString widgetName);


signals:
    
public slots:

protected:

     QMap<QString, int> *widgetMap;
    
};

#endif // IPSTACKEDWIDGET_H
