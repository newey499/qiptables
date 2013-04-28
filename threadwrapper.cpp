#include "threadwrapper.h"

ThreadWrapper::ThreadWrapper(QObject *parent) :
    QObject(parent)
{
}

ThreadWrapper::~ThreadWrapper()
{
}

void ThreadWrapper::run()
{
    QThread* thread = new QThread;
    //ThreadWorker *worker = new ThreadWorker();
    WorkerSubClass *worker = new WorkerSubClass();

    worker->moveToThread(thread);
    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}


void ThreadWrapper::run(ThreadWorker *worker)
{
    QThread* thread = new QThread;
    //ThreadWorker *worker = new ThreadWorker();
    //WorkerSubClass *worker = new WorkerSubClass();

    worker->moveToThread(thread);
    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}


void ThreadWrapper::errorString(QString errStr)
{
    qDebug("ThreadTest::errorString(QString errStr)\nError [%s]",
           errStr.toAscii().data());
}
