#ifndef MENUACTION_H
#define MENUACTION_H

#include <QAction>


/**
\file menuaction.h
\class MenuAction

\brief Adds a new signal to QAction which emits a pointer to the instance of
the class

***********/
class MenuAction : public QAction
{
    Q_OBJECT

public:

    explicit MenuAction(QObject *parent = 0);
    explicit MenuAction(const QString &text, QObject *parent= 0);
    explicit MenuAction(const QIcon &icon, const QString &text, QObject *parent = 0);

signals:

    void menuActionSelected(MenuAction *);

public slots:


protected:

    virtual void commonConstructor();


protected slots:

    virtual void handleTriggered();


private:

};

#endif // MENUACTION_H
