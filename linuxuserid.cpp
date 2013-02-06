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
