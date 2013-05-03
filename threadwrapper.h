#ifndef THREADTEST_H
#define THREADTEST_H
/**
\file threadwrapper.h

\class ThreadWrapper

\brief Class that executes tasks contained in
       a ThreadWorker object in a new QThread

***************************************/


#include <QThread>

#include "threadworker.h"
#include "threadworkersubclass.h"

class ThreadWrapper : public QObject
{
    Q_OBJECT
public:

    explicit ThreadWrapper(QObject *parent = 0);
    ~ThreadWrapper();

    /**
    \brief Execute a ThreadWorker Object in a new QThread.

      *******************/
    virtual void run();

    /**
    \brief Execute a ThreadWorker Object in a new QThread.

      *******************/
    virtual void run(ThreadWorker *worker);


public slots:

    /**
    \brief Receives any error messages from the ThreadWorker Object.

    \param errStr - Error message propogated from ThreadWorker Object.
      ********************/
    virtual void errorString(QString errStr);

signals:

public slots:

};

#endif // THREADTEST_H
