#include "workersubclass.h"

WorkerSubClass::WorkerSubClass(QObject *parent) :
    ThreadWorker(parent)
{
    // Copy data from constructor arguments to internal variables here.
}

WorkerSubClass::~WorkerSubClass()
{
    // free resources
}

// --- PROCESS ---
// Start processing data.
void WorkerSubClass::process()
{
    // allocate resources using new here
    qDebug("Hello World from WorkerSubClass! I am a subclass of a ThreadWorker Object operating in a new QThread");
    emit finished();
}
