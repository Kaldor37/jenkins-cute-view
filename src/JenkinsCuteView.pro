#-------------------------------------------------------------------------------
# Project Jenkins Cute View
#-------------------------------------------------------------------------------
QT			+=		core gui xml network
#-------------------------------------------------------------------------------
TARGET	=		jenkins-cute-view
TEMPLATE	=		app
#-------------------------------------------------------------------------------
SOURCES	+=		main.cpp \
					application.cpp \
					preferences.cpp \
					controllers/jenkinscontroller.cpp \
					models/buildmodel.cpp \
					models/jenkinsxmlapimodel.cpp \
					models/jobmodel.cpp \
					models/viewmodel.cpp \
					utils/httpgetter.cpp \
					views/aboutdialog.cpp \
					views/jenkinsgraphicsview.cpp \
					views/jobdisplaydata.cpp \
					views/jobgraphicsitem.cpp \
					views/mainwindow.cpp \
					views/preferencesdialog.cpp \
    views/autoresizingtextitem.cpp
#-------------------------------------------------------------------------------
HEADERS	+=		application.h \
					preferences.h \
					controllers/jenkinscontroller.h \
					models/buildmodel.h \
					models/jenkinsxmlapimodel.h \
					models/jobmodel.h \
					models/viewmodel.h \
					utils/httpgetter.h \
					views/aboutdialog.h \
					views/jenkinsgraphicsview.h \
					views/jobdisplaydata.h \
					views/jobgraphicsitem.h \
					views/mainwindow.h \
					views/preferencesdialog.h \
    views/autoresizingtextitem.h
#-------------------------------------------------------------------------------
FORMS		+=		views/uis/aboutdialog.ui \
					views/uis/mainwindow.ui \
					views/uis/preferencesdialog.ui
#-------------------------------------------------------------------------------
