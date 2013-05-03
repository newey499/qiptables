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
