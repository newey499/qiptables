#ifndef GENLIB_H
#define GENLIB_H

#include <QObject>
#include <QString>

/**
 \file genlib.h

 \class GenLib

 \brief Class containing various utility functions

 Class used to house various utility functions used throughout
 the qiptables application.

*/

class GenLib : public QObject
{
    Q_OBJECT

public:

    explicit GenLib(QObject *parent = 0);
    ~GenLib();

    /**
    \brief Returns #include statement for snippet to be included in ruleset

    A Carriage Return is prepended to the statement to ensure it is placed
    on a new line.

    \param snippetName - name of snippet to be included

    \return #include statement for snippet to be included in ruleset
      **********************/
    static QString getIncludeString(QString snippetName);

signals:

public slots:

protected:

private:

};

#endif // GENLIB_H
