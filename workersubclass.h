#ifndef WORKERSUBCLASS_H
#define WORKERSUBCLASS_H

#include "threadworker.h"

class WorkerSubClass : public ThreadWorker
{
    Q_OBJECT

public:

    explicit WorkerSubClass(QObject *parent = 0);
    ~WorkerSubClass();

signals:

public slots:

    virtual void process();  // Start processing data.
                             // Allocate resources using new here.

};

#endif // WORKERSUBCLASS_H
