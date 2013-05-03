#ifndef THREADIPTABLESWORKERSUBCLASS_H
#define THREADIPTABLESWORKERSUBCLASS_H

#include <QPointer>

#include "threadworker.h"

class IpProcess;
class Iptables;

class ThreadIptablesWorkerSubClass : public ThreadWorker
{
    Q_OBJECT

public:

    static const int UNKNOWN_REQUEST;
    static const int GET_CURRENT_RULESET;
    static const int SET_CURRENT_RULESET;

    explicit ThreadIptablesWorkerSubClass(int processRequired = UNKNOWN_REQUEST);
    explicit ThreadIptablesWorkerSubClass(int processRequired, QString rulesetName);

    ~ThreadIptablesWorkerSubClass();

signals:

    void sigCurrentRules(QString msg);

public slots:

    // Start processing data.
    // Allocate resources using new here.
    virtual void process();

protected slots:

    virtual void getCurrentRuleset();
    virtual void setCurrentRuleset();

protected:

    int processRequired;
    QString rulesetName;

    QPointer<Iptables> iptables;
    QPointer<IpProcess> proc;

};

#endif // THREADIPTABLESWORKERSUBCLASS_H
