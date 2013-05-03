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

#include "threadiptablesworkersubclass.h"
#include "iptables.h"
#include "ipprocess.h"


const int ThreadIptablesWorkerSubClass::UNKNOWN_REQUEST     = 0;
const int ThreadIptablesWorkerSubClass::GET_CURRENT_RULESET = 1;
const int ThreadIptablesWorkerSubClass::SET_CURRENT_RULESET = 2;


ThreadIptablesWorkerSubClass::ThreadIptablesWorkerSubClass(int processRequired) :
    ThreadWorker(0)
{
    // Copy data from constructor arguments to internal variables here.
    this->processRequired = processRequired;
}


ThreadIptablesWorkerSubClass::ThreadIptablesWorkerSubClass(int processRequired,
                                                           QString rulesetName) :
    ThreadWorker(0)
{
    // Copy data from constructor arguments to internal variables here.
    this->processRequired = processRequired;
    this->rulesetName = rulesetName;
}


ThreadIptablesWorkerSubClass::~ThreadIptablesWorkerSubClass()
{
    // free resources
}

// --- PROCESS ---
// Start processing data.
void ThreadIptablesWorkerSubClass::process()
{
    // allocate resources using new here
    qDebug("Hello World from ThreadIptablesWorkerSubClass! I am a subclass of a ThreadWorker Object operating in a new QThread");

    switch (processRequired)
    {
        case ThreadIptablesWorkerSubClass::UNKNOWN_REQUEST :
            qDebug("ThreadIptablesWorkerSubClass::UNKNOWN_REQUEST [%d]", processRequired);
            break;

        case ThreadIptablesWorkerSubClass::GET_CURRENT_RULESET :
            qDebug("ThreadIptablesWorkerSubClass::GET_CURRENT_RULESET [%d]", processRequired);
            getCurrentRuleset();
            break;

        case ThreadIptablesWorkerSubClass::SET_CURRENT_RULESET :
            qDebug("ThreadIptablesWorkerSubClass::SET_CURRENT_RULESET [%d]", processRequired);
            setCurrentRuleset();
            break;

        default:
            qDebug("ThreadIptablesWorkerSubClass::process() Unknown Option [%d]", processRequired);
            break;
    }




    emit finished();
}


void ThreadIptablesWorkerSubClass::getCurrentRuleset()
{
    qDebug("ThreadIptablesWorkerSubClass::getCurrentRuleset()");
    QString msg("");


    proc = new IpProcess(this);

    msg = proc->execCmdLine("iptables -L");

    emit sigCurrentRules(msg);

    // Clean up Heap
    if (proc)
    {
        delete proc;
    }
}


void ThreadIptablesWorkerSubClass::setCurrentRuleset()
{
    qDebug("ThreadIptablesWorkerSubClass::setCurrentRuleset()");
    QString msg("");

    iptables = new Iptables(this);

    proc = new IpProcess(this);

    QString shortName = iptables->getRulesetShortName(rulesetName);

    iptables->processRuleset(rulesetName);

    // create a new empty chain with the short name of the Ruleset
    QString tmp = shortName;
    tmp.prepend("iptables -N ");
    proc->execCmdLine(tmp);

    msg = proc->execCmdLine("iptables -L");

    emit sigCurrentRules(msg);

    // Clean up Heap
    if (iptables)
    {
        delete iptables;
    }

    if (proc)
    {
        delete proc;
    }
}
