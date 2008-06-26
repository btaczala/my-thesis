HEADERS += src/Ui_AddDownloadFileImpl.h \
    src/Ui_UserSettingsImpl.h \
    src/mainwindow.h \
    src/rapidsharemanager/debugutils.h \
    src/rapidsharemanager/qrapidsharedownload.h \
    src/rapidsharemanager/qrapidshareuser.h \
    src/rapidsharemanager/rapidsharedownloadmanager.h \
    src/common.h \
    src/configurationdialog.h \
    src/pages.h \
    src/rapidsharemanager/rslogger.h
SOURCES += src/Ui_AddDownloadFileImpl.cpp \
    src/Ui_UserSettingsImpl.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/rapidsharemanager/qrapidsharedownload.cpp \
    src/rapidsharemanager/qrapidshareuser.cpp \
    src/rapidsharemanager/rapidsharedownloadmanager.cpp \
    src/rapidsharemanager/debugutils.cpp \
    src/configurationdialog.cpp \
    src/pages.cpp \
    src/rapidsharemanager/rslogger.cpp
FORMS += ui/usersettings.ui \
    ui/adddownload.ui 
QT += network
TEMPLATE = app
INCLUDEPATH += src/
RESOURCES += resources/main_resources.qrc
