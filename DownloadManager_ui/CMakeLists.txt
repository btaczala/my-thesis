cmake_minimum_required(VERSION 2.6)
PROJECT( DownloadManager_ui )

## CMAKE File for app/src
FIND_PACKAGE(Qt4 REQUIRED )
FIND_PACKAGE ( Boost REQUIRED )   

SET(QT_USE_QTNETWORK 1)
SET(VS_SUBSYSTEM "")

IF(WIN32)
	IF( CONSOLE )
		SET(VS_SUBSYSTEM "")
	ELSE(CONSOLE)
		SET(VS_SUBSYSTEM WIN32)
		SET(QT_USE_QTMAIN TRUE)
	ENDIF (CONSOLE)
ENDIF (WIN32)

SET( APP_SRCS
    src/main.cpp
    src/mainwindow.cpp
    src/qdownloadwidget.cpp
    src/menubar.cpp
    src/toolbar.cpp
    src/actions.cpp
	src/columnsconfigdialog.cpp
	src/settings_ui/settingsdialog.cpp
)
SET( APP_HDRS
    src/mainwindow.h
    src/qdownloadwidget.h
    src/menubar.h
    src/toolbar.h
    src/actions.h
    src/columnsconfigdialog.h
	src/settings_ui/settingsdialog.h
	src/settings_ui/isettingspage.h
)
SET( APP_MOC_HDRS
    src/mainwindow.h
    src/qdownloadwidget.h
	src/columnsconfigdialog.h
    src/menubar.h
    src/toolbar.h
	src/settings_ui/settingsdialog.h
	src/settings_ui/isettingspage.h
)
SET( APP_UI_FORMS src/ui/columnsConfig.ui
    )
SET(APP_RSC src/resources/main_resources.qrc )
#ADD_DEFINITIONS( -Wall )

INCLUDE( ${QT_USE_FILE})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})  
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})
QT4_ADD_RESOURCES(APP_RSC_SCRS ${APP_RSC})
QT4_WRAP_UI (APP_UI_HDRS ${APP_UI_FORMS} )
QT4_WRAP_CPP (APP_MOC_SRCS ${APP_MOC_HDRS})

ADD_EXECUTABLE( DownloadManager_ui ${VS_SUBSYSTEM} ${APP_UI_HDRS} ${APP_MOC_SRCS} ${APP_SRCS} ${APP_RSC_SCRS} ${APP_HDRS} )
TARGET_LINK_LIBRARIES(DownloadManager_ui ${QT_LIBRARIES})
