#-------------------------------------------------
#
# Project created by QtCreator 2013-02-05T04:23:31
#
#-------------------------------------------------

QT       += core gui sql


TARGET = qiptables
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    formtest.cpp \
    formfirewallrules.cpp \
    ipstackedwidget.cpp \
    qiptablesexception.cpp \
    linuxuserid.cpp \
    ipprocess.cpp \
    iptables.cpp \
    databasemanager.cpp \
    formconfigqiptables.cpp \
    install.cpp \
    formcfgruleset.cpp \
    formcfgrulesnippets.cpp \
    formcfgsettings.cpp \
    rulesettableview.cpp \
    formdlgruleset.cpp \
    rulesetsqltablemodel.cpp \
    cmdline.cpp

HEADERS  += mainwindow.h \
    formtest.h \
    formfirewallrules.h \
    ipstackedwidget.h \
    qiptablesexception.h \
    linuxuserid.h \
    ipprocess.h \
    iptables.h \
    databasemanager.h \
    formconfigqiptables.h \
    install.h \
    formcfgruleset.h \
    formcfgrulesnippets.h \
    formcfgsettings.h \
    rulesettableview.h \
    formdlgruleset.h \
    rulesetsqltablemodel.h \
    cmdline.h

FORMS    += mainwindow.ui \
    formtest.ui \
    formfirewallrules.ui \
    formconfigqiptables.ui \
    formcfgruleset.ui \
    formcfgrulesnippets.ui \
    formcfgsettings.ui \
    formdlgruleset.ui

OTHER_FILES += \
    README.md
