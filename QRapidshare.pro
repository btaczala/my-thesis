HEADERS += src/mainwindow.h \
 src/Ui_AddDownloadFile.h \
 src/rapidsharemanager/debugutils.h \
 src/rapidsharemanager/qrapidsharedownload.h \
 src/rapidsharemanager/qrapidshareuser.h \
 src/rapidsharemanager/rapidsharedownloadmanager.h \
 src/rslogger.h
SOURCES += src/main.cpp \
 src/mainwindow.cpp \
 src/Ui_AddDownloadFile.cpp \
 src/rapidsharemanager/qrapidsharedownload.cpp \
 src/rapidsharemanager/qrapidshareuser.cpp \
 src/rapidsharemanager/rapidsharedownloadmanager.cpp \
 src/rapidsharemanager/debugutils.cpp \
 src/rslogger.cpp
FORMS += ui/adddownload.ui
QT += network
TEMPLATE = app
INCLUDEPATH += src/

RESOURCES += resources/main_resources.qrc

