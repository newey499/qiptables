#ifndef THREADIPTABLESWRAPPER_H
#define THREADIPTABLESWRAPPER_H

#include "threadwrapper.h"

class ThreadIptablesWorkerSubClass;



class ThreadIptablesWrapper :
        public ThreadWrapper
{
    Q_OBJECT

public:
    explicit ThreadIptablesWrapper(QObject *parent = 0);
    ~ThreadIptablesWrapper();


    virtual void run();
    virtual void run(ThreadIptablesWorkerSubClass *worker);

signals:

public slots:

};

#endif // THREADIPTABLESWRAPPER_H
