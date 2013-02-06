#include "ipstackedwidget.h"

IpStackedWidget::IpStackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
        widgetMap = new QMap<QString, int>;
}

int	IpStackedWidget::addWidget(QString widgetName, QWidget *widget)
{
    int index = QStackedWidget::addWidget(widget);

    widget->setObjectName(widgetName);

    widgetMap->insert(widgetName, index);

    return index;
}


int IpStackedWidget::getPageIndex(QString widgetName)
{
    return widgetMap->value(widgetName);
}


