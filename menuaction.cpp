#include "menuaction.h"

MenuAction::MenuAction(QObject *parent) :
    QAction(parent)
{
    commonConstructor();
}


MenuAction::MenuAction(const QString &text, QObject *parent) :
    QAction(text, parent)
{
    commonConstructor();
}


MenuAction::MenuAction(const QIcon &icon, const QString &text, QObject *parent) :
    QAction(icon, text, parent)
{
    commonConstructor();
}


void MenuAction::commonConstructor()
{
    connect(this, SIGNAL(triggered()),
            this, SLOT(handleTriggered()));
}


void MenuAction::handleTriggered()
{
    emit menuActionSelected(this);
}
