cmake_minimum_required(VERSION 2.6)
PROJECT( DownloadManager_ui )

## CMAKE File for app/src

set(QT_USE_QTNETWORK 1)
SET( APP_SRCS
    src/main.cpp
    src/mainwindow.cpp
    src/qdownloadwidget.cpp
    src/menubar.cpp
    src/toolbar.cpp
    src/actions.cpp
)
SET( APP_HDRS
    src/mainwindow.h
    src/qdownloadwidget.h
    src/menubar.h
    src/toolbar.h
    src/actions.h
)
SET( APP_MOC_HDRS
    src/mainwindow.h
    src/qdownloadwidget.h
    src/menubar.h
    src/toolbar.h
    src/actions.h
)
SET( APP_UI_FORMS 
    )
SET(APP_RSC src/resources/main_resources.qrc )
#ADD_DEFINITIONS( -Wall )
find_package(Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE})
QT4_ADD_RESOURCES(APP_RSC_SCRS ${APP_RSC})
QT4_WRAP_UI (APP_UI_HDRS ${APP_UI_FORMS} )
QT4_WRAP_CPP (APP_MOC_SRCS ${APP_MOC_HDRS})

#INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR}/QtRapidshareDownloadManager_app/src )
#INCLUDE_DIRECTORIES( ./../../QtRapidshareDownloadManager_corelib/src )

add_executable( DownloadManager_ui ${APP_UI_HDRS} ${APP_MOC_SRCS} ${APP_SRCS} ${APP_RSC_SCRS} ${APP_HDRS} )
target_link_libraries(DownloadManager_ui ${QT_LIBRARIES})