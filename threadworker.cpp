#include "threadworker.h"



ThreadWorker::ThreadWorker() :
    QObject()
{
    // you could copy data from constructor arguments to internal variables here.
}

ThreadWorker::ThreadWorker(QObject *parent) :
    QObject(parent)
{
    // you could copy data from constructor arguments to internal variables here.
}


ThreadWorker::~ThreadWorker()
{
    // free resources
}

// --- PROCESS ---
// Start processing data.
void ThreadWorker::process()
{
    // allocate resources using new here
    qDebug("Hello World! I am a ThreadWorker Object operating in a new QThread");
    emit finished();
}
