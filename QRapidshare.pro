HEADERS += src/mainwindow.h \
 src/Ui_AddDownloadFile.h \
 src/rapidsharemanager/debugutils.h \
 src/rapidsharemanager/qrapidsharedownload.h \
 src/rapidsharemanager/qrapidshareuser.h \
 src/rapidsharemanager/rapidsharedownloadmanager.h \
 src/common.h \
 src/configurationdialog.h \
 src/pages.h \
 src/rapidsharemanager/rslogger.h
SOURCES += src/main.cpp \
 src/mainwindow.cpp \
 src/Ui_AddDownloadFile.cpp \
 src/rapidsharemanager/qrapidsharedownload.cpp \
 src/rapidsharemanager/qrapidshareuser.cpp \
 src/rapidsharemanager/rapidsharedownloadmanager.cpp \
 src/rapidsharemanager/debugutils.cpp \
 src/configurationdialog.cpp \
 src/pages.cpp \
 src/rapidsharemanager/rslogger.cpp
FORMS += ui/adddownload.ui
QT += network
TEMPLATE = app
INCLUDEPATH += src/
RESOURCES += resources/main_resources.qrc
