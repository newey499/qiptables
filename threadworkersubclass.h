#ifndef THREADWORKERSUBCLASS_H
#define THREADWORKERSUBCLASS_H

#include "threadworker.h"

class ThreadWorkerSubClass : public ThreadWorker
{
    Q_OBJECT

public:

    static const int UNKNOWN_REQUEST;

    explicit ThreadWorkerSubClass(QObject *parent = 0);
    ~ThreadWorkerSubClass();

signals:

public slots:

    // Start processing data.
    // Allocate resources using new here.
    virtual void process(int processRequired = ThreadWorkerSubClass::UNKNOWN_REQUEST);


};

#endif // THREADWORKERSUBCLASS_H
