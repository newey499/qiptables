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


#ifndef THREADIPTABLESWORKERSUBCLASS_H
#define THREADIPTABLESWORKERSUBCLASS_H

#include <QPointer>

#include "threadworker.h"

class IpProcess;
class Iptables;

class ThreadIptablesWorkerSubClass : public ThreadWorker
{
    Q_OBJECT

public:

    static const int UNKNOWN_REQUEST;
    static const int GET_CURRENT_RULESET;
    static const int SET_CURRENT_RULESET;

    explicit ThreadIptablesWorkerSubClass(int processRequired = UNKNOWN_REQUEST);
    explicit ThreadIptablesWorkerSubClass(int processRequired, QString rulesetName);

    ~ThreadIptablesWorkerSubClass();

signals:

    void sigCurrentRules(QString msg);

public slots:

    // Start processing data.
    // Allocate resources using new here.
    virtual void process();

protected slots:

    virtual void getCurrentRuleset();
    virtual void setCurrentRuleset();

protected:

    int processRequired;
    QString rulesetName;

    QPointer<Iptables> iptables;
    QPointer<IpProcess> proc;

};

#endif // THREADIPTABLESWORKERSUBCLASS_H
