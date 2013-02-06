#include "qiptablesexception.h"


QIptablesException::QIptablesException(QString msg) :
    exception()
{
    errMsg = msg;
}

QIptablesException::~QIptablesException() throw()
{

};

const char* QIptablesException::what()
{
    QString tmp = QString("QIptablesException: %1").arg(errMsg);
    return tmp.toAscii().data();
}

