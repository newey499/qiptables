/************************************************************************
Copyright Chris Newey 2013

qiptables@hotmail.com

This file is part of Qiptables.

Qiptables is distributed under the terms of the GNU General Public License

Qiptables is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Qiptables is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Qiptables.  If not, see <http://www.gnu.org/licenses/>.

***************************************************************************/

/**
\file threadwrapper.cpp

\class ThreadWrapper

\brief Class that executes tasks contained in
       a ThreadWorker object in a new QThread

***************************************/

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
    ThreadWorkerSubClass *worker = new ThreadWorkerSubClass();

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
    //ThreadWorkerSubClass *worker = new ThreadWorkerSubClass();

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
