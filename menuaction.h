#ifndef MENUACTION_H
#define MENUACTION_H

#include <QAction>

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
