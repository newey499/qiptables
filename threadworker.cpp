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
