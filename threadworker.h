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
