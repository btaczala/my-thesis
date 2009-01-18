# ## CMAKE File for app/src
# PROJECT( qrsdm-mainApp )
# set(QT_USE_QTNETWORK 1)
# SET( APP_SRCS
    # main.cpp
    # mainwindow.cpp
    # qdownloadwidget.cpp
    # menubar.cpp
    # toolbar.cpp
    # actions.cpp
# )
# SET( APP_HDRS
    # mainwindow.h
    # qdownloadwidget.h
    # menubar.h
    # toolbar.h
    # actions.h
# )
# SET( APP_MOC_HDRS
    # mainwindow.h
    # qdownloadwidget.h
    # menubar.h
    # toolbar.h
    # actions.h
# )
# SET( APP_UI_FORMS 
    # )
# SET(APP_RSC resources/main_resources.qrc )
# #ADD_DEFINITIONS( -Wall )
# find_package(Qt4 REQUIRED )
# INCLUDE( ${QT_USE_FILE})
# QT4_ADD_RESOURCES(APP_RSC_SCRS ${APP_RSC})
# QT4_WRAP_UI (APP_UI_HDRS ${APP_UI_FORMS} )
# QT4_WRAP_CPP (APP_MOC_SRCS ${APP_MOC_HDRS})

# #INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR}/QtRapidshareDownloadManager_app/src )
# #INCLUDE_DIRECTORIES( ./../../QtRapidshareDownloadManager_corelib/src )

# add_executable( new_layout ${APP_UI_HDRS} ${APP_MOC_SRCS} ${APP_SRCS} ${APP_RSC_SCRS} ${APP_HDRS} )
# target_link_libraries(new_layout ${QT_LIBRARIES})