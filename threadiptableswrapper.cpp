


#include <QThread>

#include "threadiptableswrapper.h"
#include"threadiptablesworkersubclass.h"


class ThreadIptablesWorker;

ThreadIptablesWrapper::ThreadIptablesWrapper(QObject *parent) :
    ThreadWrapper(parent)
{
}


ThreadIptablesWrapper::~ThreadIptablesWrapper()
{
}

void ThreadIptablesWrapper::run()
{
    QThread* thread = new QThread;
    //ThreadWorker *worker = new ThreadWorker();
    ThreadIptablesWorkerSubClass *worker = new ThreadIptablesWorkerSubClass(ThreadIptablesWorkerSubClass::GET_CURRENT_RULESET);

    worker->moveToThread(thread);
    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}


void ThreadIptablesWrapper::run(ThreadIptablesWorkerSubClass *worker)
{
    QThread* thread = new QThread;
    //ThreadWorker *worker = new ThreadWorker();
    //ThreadWorkerSubClass *worker = new ThreadWorkerSubClass();

    worker->moveToThread(thread);
    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}


