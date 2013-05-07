#ifndef GENLIB_H
#define GENLIB_H

#include <QAction>
#include <QApplication>
#include <QList>
#include <QMenu>
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

    /**
    \brief calculate the ruleset shortname from the ruleset full name
    \param rulesetLongName - ruleset full name
    \return ruleset short name
      *******************/
    static QString getRulesetShortName(QString rulesetLongName);

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

    /**
    \brief gets long ruleset name using the the short name which is
           qiptables writes as the name of an empty qiptables chain.

    \return long ruleset name
      **************/
    static QString getRulesetName();

    /**
    \brief get pointer to top level widget using widget instance name

    The return pointer (if not null) has to be cast to the required widget type

    \return pointer to QWidget or 0 if not found.
      *****************************/
    static QWidget * getWidgetPointer(QString widgetObjectName);


    /**
    \brief Clears all firewall chains and sets all policies to Accept
      *************************/
    static void clearAllFirewallRules();

    /**

    \brief Returns a QStringList containing the GNU licence


      *************************/
    static QStringList getGnuLicence();

signals:

public slots:


protected:

private:

};

#endif // GENLIB_H
