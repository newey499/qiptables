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


====================================
NOTE

typedef unsigned int    __kernel_uid32_t;
typedef __kernel_uid32_t        uid_t;

**********************************************************/

class LinuxUserId : public QObject
{
    Q_OBJECT
public:

    static const unsigned int ROOT_USERID;

    explicit LinuxUserId(QObject *parent = 0);

    virtual unsigned int getUserId();
    virtual unsigned int getEffectiveUserId();

    virtual bool isUserRoot();
    virtual bool isEffectiveUserRoot();

signals:
    
public slots:

protected:

    unsigned int userId;
    unsigned int effectiveUserId;
    
};

#endif // LINUXUSERID_H
