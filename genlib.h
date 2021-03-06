#ifndef GENLIB_H
#define GENLIB_H

#include <QAction>
#include <QApplication>
#include <QDir>
#include <QFile>
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
    \brief calculate the ruleset shortname from the ruleset full name
    \param rulesetLongName - ruleset full name
    \return ruleset short name
      *******************/
    static QString getRulesetShortName(QString rulesetLongName);

    /**
    \brief gets long ruleset name using the the short name which is
           qiptables writes as the name of an empty qiptables chain.

    \return long ruleset name
      **************/
    static QString getRulesetName();


    /**
    \brief get short name of current ruleset using
           /tc/qiptables/tools/get-firewall-name.sh

      **************************/
    static QString getCurrentFirewallShortName();


    /**
    \brief Gets the full ruleset name from the short name.

      ****************************************/
    static QString getRulesetNameFromShortName(QString shortname = GenLib::getCurrentFirewallShortName());


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

    /**
    \brief Returns the canonical path to <filename>

      ***********************/
    static QString cleanFileName(QString path, QString filename);

signals:

public slots:


protected:

private:

};

#endif // GENLIB_H
