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
    rulesnippetstableview.cpp \
    formdlgruleset.cpp \
    rulesetsqltablemodel.cpp \
    rulesnippetssqltablemodel.cpp \
    cmdline.cpp \
    formdlgrulesnippet.cpp

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
    rulesnippetstableview.h \
    formdlgruleset.h \
    rulesetsqltablemodel.h \
    rulesnippetssqltablemodel.h \
    cmdline.h \
    formdlgrulesnippet.h

FORMS    += mainwindow.ui \
    formtest.ui \
    formfirewallrules.ui \
    formconfigqiptables.ui \
    formcfgruleset.ui \
    formcfgrulesnippets.ui \
    formcfgsettings.ui \
    formdlgruleset.ui \
    formdlgrulesnippet.ui

OTHER_FILES += \
    README.md
