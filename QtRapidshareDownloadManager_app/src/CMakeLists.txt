## CMAKE File for app/src
PROJECT( qrsdm-mainApp )
set(QT_USE_QTNETWORK 1)
SET( APP_SRCS
    common.cpp
    configurationdialog.cpp
    downloaddelegate.cpp
    main.cpp
    mainwindow.cpp
    pages.cpp
    DownloadView.cpp
    downloadItem.cpp
    Ui_AddDownloadFileImpl.cpp
    Ui_UserSettingsImpl.cpp
)
SET( APP_HDRS
	configurationdialog.h
    downloaddelegate.h
    mainwindow.h
    pages.h
    Ui_AddDownloadFileImpl.h
    Ui_UserSettingsImpl.h
	common.h
	DownloadView.h
	downloadItem.h
)
SET( APP_MOC_HDRS
    configurationdialog.h
    downloaddelegate.h
    mainwindow.h
    pages.h
    DownloadView.h
    downloadItem.h
    Ui_AddDownloadFileImpl.h
    Ui_UserSettingsImpl.h
)
SET( APP_UI_FORMS 
    ui/adddownload.ui 	
    ui/usersettings.ui
    )
SET(APP_RSC resources/main_resources.qrc )
#ADD_DEFINITIONS( -Wall )
find_package(Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE})
QT4_ADD_RESOURCES(APP_RSC_SCRS ${APP_RSC})
QT4_WRAP_UI (APP_UI_HDRS ${APP_UI_FORMS} )
QT4_WRAP_CPP (APP_MOC_SRCS ${APP_MOC_HDRS})

INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR}/QtRapidshareDownloadManager_app/src )
INCLUDE_DIRECTORIES( ./../../QtRapidshareDownloadManager_corelib/src )

add_executable( qrapidshare ${APP_UI_HDRS} ${APP_MOC_SRCS} ${APP_SRCS} ${APP_RSC_SCRS} ${APP_HDRS} )
target_link_libraries(qrapidshare ${QT_LIBRARIES} QtRapidshareDownloadManager_corelib )