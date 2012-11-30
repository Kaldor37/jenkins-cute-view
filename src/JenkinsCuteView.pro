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
					models/viewmodel.cpp \
					utils/httpgetter.cpp \
					views/aboutdialog.cpp \
					views/jenkinsgraphicsview.cpp \
					views/jobdisplaydata.cpp \
					views/mainwindow.cpp \
					views/preferencesdialog.cpp \
					views/graphicsitems/autoresizingtextitem.cpp \
					views/graphicsitems/jobgraphicsitem.cpp \
					views/graphicsitems/messagegraphicsitem.cpp \
					views/graphicsitems/nodegraphicsitem.cpp \
					views/graphicsitems/weathergraphicsitem.cpp
#-------------------------------------------------------------------------------
HEADERS		+=	application.h \
					preferences.h \
					controllers/jenkinscontroller.h \
					models/buildmodel.h \
					models/jenkinsxmlapimodel.h \
					models/jobmodel.h \
					models/nodemodel.h \
					models/viewmodel.h \
					utils/httpgetter.h \
					views/aboutdialog.h \
					views/jenkinsgraphicsview.h \
					views/jobdisplaydata.h \
					views/preferencesdialog.h \
					views/mainwindow.h \
					views/graphicsitems/autoresizingtextitem.h \
					views/graphicsitems/jobgraphicsitem.h \
					views/graphicsitems/messagegraphicsitem.h \
					views/graphicsitems/nodegraphicsitem.h \
					views/graphicsitems/weathergraphicsitem.h
#-------------------------------------------------------------------------------
FORMS			+=	views/uis/aboutdialog.ui \
					views/uis/mainwindow.ui \
					views/uis/preferencesdialog.ui
#-------------------------------------------------------------------------------
RESOURCES	+=	../res/weather-icons.qrc
#-------------------------------------------------------------------------------
