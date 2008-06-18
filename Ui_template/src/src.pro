SOURCES += main.cpp \
 mainwindow.cpp \
 downloadviewdelegate.cpp
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt
TARGET = ../bin/qrapidshare_ui_template

HEADERS += mainwindow.h \
downloadviewdelegate.h
