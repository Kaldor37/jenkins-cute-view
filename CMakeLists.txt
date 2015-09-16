cmake_minimum_required(VERSION 2.8.11)

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

# Compile definitions
add_definitions(-Wall -Wextra -std=c++11)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

# Include directories (sources and bin for generated moc headers)
include_directories(
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_BINARY_DIR}
	${Qt5Core_INCLUDE_DIRS}
)

# Headers
SET(HEADERS
	src/views/jobdisplaydata.h
)

# Moc headers (QObjects .h)
SET(MOC_HDRS
	src/application.h
	src/preferences.h
	src/controllers/jenkinscontroller.h
	src/models/buildmodel.h
	src/models/jenkinsapimodel.h
	src/models/jobmodel.h
	src/models/nodeslistmodel.h
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
	src/models/jenkinsapimodel.cpp
	src/models/jobmodel.cpp
	src/models/nodeslistmodel.cpp
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

qt5_wrap_cpp(MOC_SRCS ${MOC_HDRS})
qt5_wrap_ui(FORMS_SRCS ${FORMS})
qt5_add_resources(RESOURCES_SRCS ${RESOURCES})

SET(executableOpts "")
IF(WIN32)
	SET(executableOpts "WIN32")
ENDIF()

add_executable(${PROJECT_NAME}
	${executableOpts}
	${SOURCES}
	${HEADERS}
	${MOC_SRCS}
	${FORMS_SRCS}
	${RESOURCES_SRCS}
)

target_link_libraries(${PROJECT_NAME}
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::Network
)

find_package(Git)
IF(GIT_FOUND)
	exec_program(
		${GIT_EXECUTABLE}
		${CMAKE_CURRENT_SOURCE_DIR}
		ARGS "rev-parse" "HEAD"
		OUTPUT_VARIABLE GIT_REVISION
	)
	IF(GIT_REVISION)
		message(STATUS "Git revision: ${GIT_REVISION}")
		set_target_properties(${PROJECT_NAME} PROPERTIES
			COMPILE_DEFINITIONS "GIT_REVISION=\"${GIT_REVISION}\""
		)
	ENDIF()
ENDIF()

# Install
INSTALL(
	PROGRAMS ${PROJECT_BINARY_DIR}/${PROJECT_NAME}
	DESTINATION ${INSTALL_DIRECTORY}
)
