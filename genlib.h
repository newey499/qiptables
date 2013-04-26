#ifndef GENLIB_H
#define GENLIB_H

#include <QObject>
#include <QString>

/**
 \file genlib.h

 \class GenLib

 \brief Class containing various utility functions

 A Class used to house various utility functions used throughout
 the qiptables application.

*/

class GenLib : public QObject
{
    Q_OBJECT
public:
    explicit GenLib(QObject *parent = 0);
    ~GenLib();

    QString getIncludeString(QString snippetName);

signals:

public slots:

};

#endif // GENLIB_H
