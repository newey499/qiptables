#ifndef QIPTABLESEXCEPTION_H
#define QIPTABLESEXCEPTION_H

// standard exceptions
#include <iostream>
#include <exception>
using namespace std;

#include <QString>

class QIptablesException : public exception
{
public:

    QIptablesException(QString msg);
    ~QIptablesException() throw();

    virtual const char* what();

private:

    QString errMsg;

};


#endif // QIPTABLESEXCEPTION_H
