cmake_minimum_required(VERSION 2.8)

# Project name
SET(PROJECT_NAME	jenkins-cute-view)
project(${PROJECT_NAME})

# Default CMake build type : Release
IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
ENDIF()

# Default Intall directory : /usr/local/bin
IF(NOT INSTALL_DIRECTORY)
	SET(INSTALL_DIRECTORY /usr/local/bin)
ENDIF()

# Warnings
add_definitions(-Wall -Wextra)

find_package(Qt5Core)
IF(Qt5Core_FOUND)
	message(STATUS "Building with Qt5 (${Qt5Core_VERSION_STRING})")
	find_package(Qt5Gui)
	find_package(Qt5Widgets)
	find_package(Qt5Xml)
	find_package(Qt5Network)
ELSE()
	message(STATUS "Qt5 not found, trying Qt4...")
	find_package(Qt4 4.6 COMPONENTS QtCore QtGui QtXml QtNetwork REQUIRED)
	include(${QT_USE_FILE})
ENDIF()

# SVN Revision informations
find_package(Subversion)
IF(Subversion_FOUND)
	Subversion_WC_INFO(${PROJECT_SOURCE_DIR} SVN)
	message(STATUS "Project revision : ${SVN_WC_REVISION}")
ENDIF()

# Configure file with SVN revision informations
configure_file(
	${PROJECT_SOURCE_DIR}/src/svninfos.cmake.h
	${CMAKE_SOURCE_DIR}/src/svninfos.h
)

# Include directories (sources and bin for generated moc headers)
include_directories(
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_BINARY_DIR}
	${Qt5Core_INCLUDE_DIRS}
)

# Headers
SET(HEADERS
	src/views/jobdisplaydata.h
	src/svninfos.h
)

# Moc headers (QObjects .h)
SET(MOC_HDRS
	src/application.h
	src/jenkinsnamespace.h
	src/preferences.h
	src/controllers/jenkinscontroller.h
	src/models/buildmodel.h
	src/models/jenkinsxmlapimodel.h
	src/models/jobmodel.h
	src/models/nodemodel.h
	src/models/viewmodel.h
	src/utils/httpgetter.h
	src/views/aboutdialog.h
	src/views/jenkinsgraphicsview.h
	src/views/preferencesdialog.h
	src/views/mainwindow.h
	src/views/graphicsitems/autoresizingtextitem.h
	src/views/graphicsitems/jobgraphicsitem.h
	src/views/graphicsitems/messagegraphicsitem.h
	src/views/graphicsitems/nodegraphicsitem.h
	src/views/graphicsitems/weathergraphicsitem.h
)

# Sources (.cpp)
SET(SOURCES
	src/main.cpp
	src/application.cpp
	src/preferences.cpp
	src/controllers/jenkinscontroller.cpp
	src/models/buildmodel.cpp
	src/models/jenkinsxmlapimodel.cpp
	src/models/jobmodel.cpp
	src/models/nodemodel.cpp
	src/models/viewmodel.cpp
	src/utils/httpgetter.cpp
	src/views/aboutdialog.cpp
	src/views/jenkinsgraphicsview.cpp
	src/views/jobdisplaydata.cpp
	src/views/mainwindow.cpp
	src/views/preferencesdialog.cpp
	src/views/graphicsitems/autoresizingtextitem.cpp
	src/views/graphicsitems/jobgraphicsitem.cpp
	src/views/graphicsitems/messagegraphicsitem.cpp
	src/views/graphicsitems/nodegraphicsitem.cpp
	src/views/graphicsitems/weathergraphicsitem.cpp
)

# Forms (.ui)
SET(FORMS
	src/views/uis/aboutdialog.ui
	src/views/uis/mainwindow.ui
	src/views/uis/preferencesdialog.ui
)

# Resources
SET(RESOURCES
	res/weather-icons.qrc
)

IF(Qt5Core_FOUND)
	qt5_wrap_cpp(MOC_SRCS ${MOC_HDRS})
	qt5_wrap_ui(FORMS_SRCS ${FORMS})
	qt5_add_resources(RESOURCES_SRCS ${RESOURCES})
ELSE()
	qt4_wrap_cpp(MOC_SRCS ${MOC_HDRS})
	qt4_wrap_ui(FORMS_SRCS ${FORMS})
	qt4_add_resources(RESOURCES_SRCS ${RESOURCES})
ENDIF()

add_executable(
	${PROJECT_NAME}
		${SOURCES}
		${HEADERS}
		${MOC_SRCS}
		${FORMS_SRCS}
		${RESOURCES_SRCS}
)

IF(Qt5Core_FOUND)
	target_link_libraries(${PROJECT_NAME}
		Qt5::Core
		Qt5::Gui
		Qt5::Widgets
		Qt5::Xml
		Qt5::Network
	)
ELSE()
	target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
ENDIF()

# Install
INSTALL(
	PROGRAMS ${PROJECT_BINARY_DIR}/${PROJECT_NAME}
	DESTINATION ${INSTALL_DIRECTORY}
)
