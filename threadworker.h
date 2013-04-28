#ifndef WORKER_H
#define WORKER_H
/**
\file threadworker.h

\class ThreadWorker

\brief Class that performs work in a QThread

***************************************/

#include <QObject>


class ThreadWorker : public QObject {
    Q_OBJECT

public:
    explicit ThreadWorker();  // you could copy data from constructor arguments to internal variables here.
    explicit ThreadWorker(QObject *parent = 0);  // you could copy data from constructor arguments to internal variables here.
    ~ThreadWorker(); // free resources

public slots:
    virtual void process();  // Start processing data.
                             // Allocate resources using new here.

signals:
    void finished();
    void error(QString err);

private:
    // add your variables here
};

#endif // WORKER_H
