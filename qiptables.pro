###########################################################################
#
# Copyright Chris Newey 2013
#
# qiptables@hotmail.com
#
# This file is part of Qiptables.
#
# Qiptables is distributed under the terms of the GNU General Public License
#
# Qiptables is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Qiptables is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Qiptables.  If not, see <http://www.gnu.org/licenses/>.
#
###########################################################################




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
    formdlgrulesnippet.cpp \
    rulesnippet.cpp \
    genlib.cpp \
    threadworker.cpp \
    threadwrapper.cpp \
    workersubclass.cpp



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
    formdlgrulesnippet.h \
    rulesnippet.h \
    genlib.h \
    threadworker.h \
    threadwrapper.h \
    workersubclass.h


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
    README.md \
    LICENCE
