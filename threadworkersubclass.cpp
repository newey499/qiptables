#include "threadworkersubclass.h"

ThreadWorkerSubClass::ThreadWorkerSubClass(QObject *parent) :
    ThreadWorker(parent)
{
    // Copy data from constructor arguments to internal variables here.
}

ThreadWorkerSubClass::~ThreadWorkerSubClass()
{
    // free resources
}

// --- PROCESS ---
// Start processing data.
void ThreadWorkerSubClass::process()
{
    // allocate resources using new here
    qDebug("Hello World from ThreadWorkerSubClass! I am a subclass of a ThreadWorker Object operating in a new QThread");
    emit finished();
}
