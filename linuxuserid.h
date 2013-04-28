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



#ifndef LINUXUSERID_H
#define LINUXUSERID_H

#include <QObject>

#include <unistd.h>
#include <sys/types.h>
/************************************************

type:
    man getuid
to get the below manual page.

=================================================
NAME
       getuid, geteuid - get user identity

SYNOPSIS
       #include <unistd.h>
       #include <sys/types.h>

       uid_t getuid(void);
       uid_t geteuid(void);

DESCRIPTION
       getuid() returns the real user ID of the calling process.

       geteuid() returns the effective user ID of the calling process.

       When  a  normal program is executed, the effective and real user ID of the process are set to the ID of the
       user executing the file.  When a set ID program is executed the real user ID is set to the calling user and
       the effective user ID corresponds to the set ID bit on the file being executed.

ERRORS
       These functions are always successful.


NOTE

typedef unsigned int    __kernel_uid32_t;
typedef __kernel_uid32_t        uid_t;

**********************************************************/

/**
\file linuxuserid.h
\class LinuxUserId

\brief Obtains user id using Linux system calls


Main use is checking for root. The application needs to be running as root
to run iptables which is the firewall manipulation tool.

***********/
class LinuxUserId : public QObject
{
    Q_OBJECT
public:

    /**
    Linux numeric user id of root. This is always zero.
    ****************************/
    static const unsigned int ROOT_USERID;

    /**
      \brief Constructor
      \param parent
      *******************/
    explicit LinuxUserId(QObject *parent = 0);

    /**
      \return user id of user running application
      ***************/
    virtual unsigned int getUserId();

    /**
      \return effective user id of user running application
      ***************/
    virtual unsigned int getEffectiveUserId();

    /**
      \return true if user running application is root else false
      ***************/
    virtual bool isUserRoot();

    /**
      \return true if effective user running application is root else false
      ***************/
    virtual bool isEffectiveUserRoot();

signals:

public slots:

protected:

    unsigned int userId;
    unsigned int effectiveUserId;

};

#endif // LINUXUSERID_H
