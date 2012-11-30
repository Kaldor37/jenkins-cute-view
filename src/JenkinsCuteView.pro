#-------------------------------------------------------------------------------
# Project Jenkins Cute View
#-------------------------------------------------------------------------------
QT				+=	core gui xml network
#-------------------------------------------------------------------------------
TARGET		=	jenkins-cute-view
TEMPLATE		=	app
#-------------------------------------------------------------------------------
SOURCES		+=	main.cpp \
					application.cpp \
					preferences.cpp \
					controllers/jenkinscontroller.cpp \
					models/buildmodel.cpp \
					models/jenkinsxmlapimodel.cpp \
					models/jobmodel.cpp \
					models/nodemodel.cpp \
					models/queuemodel.cpp \
					models/viewmodel.cpp \
					utils/httpgetter.cpp \
					views/aboutdialog.cpp \
					views/autoresizingtextitem.cpp \
					views/jenkinsgraphicsview.cpp \
					views/jobdisplaydata.cpp \
					views/jobgraphicsitem.cpp \
					views/mainwindow.cpp \
					views/messagegraphicsitem.cpp \
					views/nodegraphicsitem.cpp \
					views/preferencesdialog.cpp \
					views/weathergraphicsitem.cpp
#-------------------------------------------------------------------------------
HEADERS		+=	application.h \
					preferences.h \
					controllers/jenkinscontroller.h \
					models/buildmodel.h \
					models/jenkinsxmlapimodel.h \
					models/jobmodel.h \
					models/nodemodel.h \
					models/queuemodel.h \
					models/viewmodel.h \
					utils/httpgetter.h \
					views/aboutdialog.h \
					views/autoresizingtextitem.h \
					views/jenkinsgraphicsview.h \
					views/jobdisplaydata.h \
					views/jobgraphicsitem.h \
					views/mainwindow.h \
					views/messagegraphicsitem.h \
					views/nodegraphicsitem.h \
					views/preferencesdialog.h \
					views/weathergraphicsitem.h
#-------------------------------------------------------------------------------
FORMS			+=	views/uis/aboutdialog.ui \
					views/uis/mainwindow.ui \
					views/uis/preferencesdialog.ui
#-------------------------------------------------------------------------------
RESOURCES	+=	../res/weather-icons.qrc
#-------------------------------------------------------------------------------
