
PROJECT(qrsdm)
find_package(Qt4 REQUIRED )
#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE OFF)
SET(QT_USE_QTNETWORK 1)

SUBDIRS(rsdm)
SUBDIRS(app)
INCLUDE(${QT_USE_FILE})
include_directories(rsdm)

ADD_EXECUTABLE(qrsdmtest qrsdmtest.cpp)

#need to link to some other libraries ? just add them here
TARGET_LINK_LIBRARIES(qrsdmtest RapidshareDownloadManager ${QT_LIBRARIES} )