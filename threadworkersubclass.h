#ifndef THREADWORKERSUBCLASS_H
#define THREADWORKERSUBCLASS_H

#include "threadworker.h"

class ThreadWorkerSubClass : public ThreadWorker
{
    Q_OBJECT

public:

    explicit ThreadWorkerSubClass(QObject *parent = 0);
    ~ThreadWorkerSubClass();

signals:

public slots:

    virtual void process();  // Start processing data.
                             // Allocate resources using new here.

};

#endif // THREADWORKERSUBCLASS_H
