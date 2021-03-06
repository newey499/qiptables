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
\file linuxuserid.cpp

\brief Obtains user id using Linux system calls

***********/

#include "linuxuserid.h"


const unsigned int LinuxUserId::ROOT_USERID = 0;

LinuxUserId::LinuxUserId(QObject *parent) :
    QObject(parent)
{
    userId =  getuid();
    effectiveUserId =  geteuid();
}


unsigned int LinuxUserId::getUserId()
{
    return userId;
}

unsigned int LinuxUserId::getEffectiveUserId()
{
    return effectiveUserId;
}

bool LinuxUserId::isUserRoot()
{
    return userId == LinuxUserId::ROOT_USERID;
}

bool LinuxUserId::isEffectiveUserRoot()
{
    return effectiveUserId == LinuxUserId::ROOT_USERID;
}
